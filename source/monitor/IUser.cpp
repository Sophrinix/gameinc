#include "StdAfx.h"
#include "IUser.h"
#include "INrpProject.h"
#include "IniFile.h"
#include "NrpCompany.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"
#include "NrpProjectModule.h"
#include "INrpDevelopProject.h"
#include "NrpInvention.h"
#include "nrpScript.h"
#include "NrpRelation.h"
#include "OpFileSystem.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{

IUser::IUser(const std::string className, const std::string systemName ) : INrpConfig( className, systemName )
{
	CreateValue<std::string>( NAME, systemName );
	CreateValue<int>( CODE_SPEED, 0 );
	CreateValue<int>( CODE_QUALITY, 0 );
	CreateValue<int>( KNOWLEDGE_LEVEL, 0 );
	CreateValue<int>( TALANT, 0 );
	CreateValue<int>( STAMINA, 0 );
	CreateValue<int>( MOOD, 0 );
	CreateValue<int>( POPULARITY, 0 );
	CreateValue<int>( SALARY, 0 );
	CreateValue<int>( STABILITY, 0 );
	CreateValue<int>( BALANCE, 0 );
	CreateValue<int>( CHARACTER, 0 );
	CreateValue<int>( WANTMONEY, 0 );
	CreateValue<int>( CONTRACTMONEY, 0 );
	CreateValue<int>( WORKNUMBER, 0 );
	CreateValue<std::string>( USERSTATE, "readyToWork" );
	CreateValue<std::string>( ROOMSTATE, "unknown" );
	CreateValue<int>( HANGRY, 100 );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<int>( EXPERIENCE, 0 );
	CreateValue<std::string>( TEXTURENORMAL, "" );
}

IUser::~IUser(void)
{
}

void IUser::SetSkill( int typen, int valuel )
{
	knowledges_[ typen ] = valuel;

	CalculateWantSalary_();
}

void IUser::SetSkill( std::string name, int valuel )
{
	SetValue<int>( name, valuel );

	CalculateWantSalary_();
}

void IUser::CalculateWantSalary_()
{
	KNOWLEDGE_MAP::iterator pIter = knowledges_.begin();
	float sum = 0;
	float cash = 500;
	for( ; pIter != knowledges_.end(); pIter++)
	{
		 sum += pIter->second * cash / 100;
		 cash *= (cash > 100 ? 0.9f : 1);
	}

	sum += GetValue<int>( CODE_QUALITY ) * 10;

	SetValue<int>( WANTMONEY, (int)sum );

	CalculateKnowledgeLevel_();
}

void IUser::CalculateKnowledgeLevel_()
{
	int sum = 0;
	KNOWLEDGE_MAP::iterator pIter = knowledges_.begin();
	for( ; pIter != knowledges_.end(); pIter++)
	{
				sum += pIter->second;
				sum /= 2;
	}
	
	SetValue<int>( KNOWLEDGE_LEVEL, sum );
}

int IUser::GetSkill( int typen )
{
	return knowledges_.find( typen ) != knowledges_.end() ? knowledges_[ typen ] : 0;
}

std::string IUser::Save( const std::string& folderPath )
{
	try
	{
		assert( OpFileSystem::IsExist( folderPath ) );

		std::string fileName = OpFileSystem::CheckEndSlash( folderPath )+ GetValue<std::string>( NAME ) + ".user";
		DeleteFile( fileName.c_str() );

		KNOWLEDGE_MAP::iterator gnrIter = genrePreferences_.begin();
		for( ; gnrIter != genrePreferences_.end(); gnrIter++ )
			IniFile::Write( "genrePreference", IntToStr( gnrIter->first ), gnrIter->second, fileName );
		
		KNOWLEDGE_MAP::iterator gnrExp = genreExperience_.begin();
		for( ; gnrExp != genreExperience_.end(); gnrExp++ )
			IniFile::Write( "genreExperience", IntToStr( gnrExp->first ), gnrExp->second, fileName );

		KNOWLEDGE_MAP::iterator knIter = knowledges_.begin();
		for( ; knIter != knowledges_.end(); knIter++ )
			IniFile::Write( "knowledges", IntToStr( knIter->first ), knIter->second, fileName );

		USERACTION_LIST::iterator uaIter = peopleFeels_.begin();
		//for( ; uaIter != peopleFeels_.end(); ++uaIter )
		//	IniFile::Write( "knowledges", uaIter->first, uaIter->second, fileName );

		WORK_LIST::iterator tlIter = works_.begin();
		for( int i=0; tlIter != works_.end(); tlIter++, i++ )
		{
			if( CNrpProjectModule* prjModule = dynamic_cast< CNrpProjectModule* >( *tlIter )  )
			{
				std::string projectName = prjModule->GetValue<INrpProject*>( PARENT )->GetString( NAME );
				std::string name = prjModule->GetString(NAME);
				IniFile::Write( SECTION_WORKS, "work_"+IntToStr( i ), "project", fileName );
				IniFile::Write( SECTION_WORKS, "projectName_" + IntToStr( i ), projectName, fileName );
				IniFile::Write( SECTION_WORKS, "moduleName_" + IntToStr( i ), name, fileName );
			}
			else if( CNrpInvention* invention = dynamic_cast< CNrpInvention* >( *tlIter ) )
			{
				std::string name = invention->GetString(NAME);
				IniFile::Write( SECTION_WORKS, "work_"+IntToStr( i ), "invention", fileName );
				IniFile::Write( SECTION_WORKS, "inventioName_" + IntToStr( i ), name, fileName );
			}
		}

		INrpConfig::Save( fileName );

		return fileName;
	}
	catch(...)
	{
		return "";	
	}
}

void IUser::Load( const std::string& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::Load( fileName );
	assert( GetString( NAME ).size() > 0 );

	IniFile::ReadValueList_( "knowledges", knowledges_, fileName );

	for( int k=0; k < GetValue<int>( WORKNUMBER ); k++ )
	{
		std::string action = "";
		std::string workType = IniFile::Read( SECTION_WORKS, "work_" + IntToStr( k ), std::string(""), fileName );
		if( !workType.empty() )
		{
			if( workType == "project" )
			{
				std::string projectName = IniFile::Read( SECTION_WORKS, "projectName_" + IntToStr( k ), std::string(""), fileName );	
				std::string moduleName = IniFile::Read( SECTION_WORKS, "moduleName_" + IntToStr( k ), std::string(""), fileName );
				action = "autoscript:AddUserToGameProject(\"" + GetString( NAME ) + "\", \"" + projectName + "\", \"" + moduleName + "\")";
			}
			else if( workType == "invention" )
			{
				std::string inventionName = IniFile::Read( SECTION_WORKS, "inventioName_" + IntToStr( k ), std::string(""), fileName );	
				action = "autoscript:AddUserToInvention(\"" + GetString( NAME ) + "\" , \"" + inventionName + "\")";	
			}				
			
			CNrpScript::Instance().AddActionToTemporaryScript( AFTER_LOAD_SCRIPT, action );
		}
	}

/*	NAMEVALUE_MAP::iterator gnrIter = genrePreferences_.begin();
	for( ; gnrIter != genrePreferences_.end(); ++gnrIter )
	IniFile::Write( "genrePreference", gnrIter->first, gnrIter->second, fileName );

	NAMEVALUE_MAP::iterator gnrExp = genreExperience_.begin();
	for( ; gnrExp != genreExperience_.end(); ++gnrExp )
		IniFile::Write( "genreExperience", gnrExp->first, gnrExp->second, fileName );

	USERACTION_LIST::iterator uaIter = peopleFeels_.begin();
	//for( ; uaIter != peopleFeels_.end(); ++uaIter )
	//	IniFile::Write( "knowledges", uaIter->first, uaIter->second, fileName );

	TECH_LIST::iterator tlIter = techWorks_.begin();
	//for( ; tlIter != techWorks_.end(); ++tlIter )
	//	IniFile::Write( "knowledges", tlIter->first, tlIter->second, fileName );

	PropertyArray::iterator paIter = GetProperties().begin();
	for( ; paIter != GetProperties().end(); ++paIter)
	{
		INrpProperty* prop = paIter->second;
		if( prop->GetValueType() == typeid( int ).name() )
			IniFile::Write( PROPERTIES, paIter->first, ((CNrpProperty<int>*)prop)->GetValue(), fileName );

		if( prop->GetValueType() == typeid( std::string ).name() )
			IniFile::Write( PROPERTIES, paIter->first, ((CNrpProperty<std::string>*)prop)->GetValue(), fileName );
	}*/
}

void IUser::AddWork( IWorkingModule* module, bool toFront )
{
	assert( module != NULL );

	if( GetWork( module->GetValue<std::string>( NAME ) ) == NULL )
	{
		if( toFront )
			works_.insert( works_.begin(), module );
		else
			works_.push_back( module );

		module->AddUser( this );
	}

	SetValue<int>( WORKNUMBER, works_.size() );
}

void IUser::RemoveWork( IWorkingModule* techWork )
{
	assert( techWork != NULL );
	if( techWork == NULL )
		return;

	WORK_LIST::iterator tIter = works_.begin();
	for( ; tIter != works_.end(); tIter++ )
		if( (*tIter) == techWork )
		{
			techWork->RemoveUser( GetValue<std::string>( NAME ) );
			works_.erase( tIter );
			SetValue<int>( WORKNUMBER, works_.size() );
			return;
		}
		
	std::string text = "�� ���� ����� ��������� ��� �������� " + techWork->GetValue<std::string>( NAME );
	Log(HW) << text << term;
}

IWorkingModule* IUser::GetWork( int index ) const
{
	assert( index >= 0 && index < (int)works_.size() );
	return ( index >= 0 && index < (int)works_.size()) ? works_[ index ] : NULL;
}

IWorkingModule* IUser::GetWork( const std::string& name ) const
{
	WORK_LIST::const_iterator tIter = works_.begin();
	for( ; tIter != works_.end(); tIter++ )
		if( (*tIter)->GetValue<std::string>( NAME ) == name )
			return (*tIter);

	return NULL;
}

void IUser::BeginNewHour( const SYSTEMTIME& time )
{
	if( time.wHour == 9 && GetValue<std::string>( USERSTATE ) == "readyToWork" )
	{
		SetValue<std::string>( USERSTATE, "work" );
	}

	if(	AddValue<int>( HANGRY, -15 ) < 30 )
	{
		SYSTEMTIME endTime = time;
		endTime.wHour = 23;
		AddModificator( new CNrpUserModificator<int>( this, endTime, MOOD, true, 45 ) );
	}

	if( time.wHour == 18 )
	{
		SetValue<std::string>( USERSTATE, "readyToWork" );
	}

	if( GetValue<std::string>( USERSTATE ) == "work" )
	{
		if( works_.size() > 0 )
		{
			//��������� ��������� ����������
			if( works_[ 0 ]->GetValue<float>( READYWORKPERCENT ) >= 1 )
				RemoveWork( works_[ 0 ] );
			else
				works_[ 0 ]->Update( this );			
		}
	}
}

int IUser::GetGenreExperience( int typen )
{
	KNOWLEDGE_MAP::iterator kIter = genreExperience_.find( typen );
	return kIter != genreExperience_.end() ? kIter->second : 0;
}

int IUser::GetGenrePreferences( int typen )
{
	KNOWLEDGE_MAP::iterator kIter = genrePreferences_.find( typen );
	return kIter != genrePreferences_.end() ? kIter->second : 0;
}

void IUser::SetGenreExperience( int typen, int valuel )
{
	genreExperience_[ typen ] = valuel;
}

void IUser::SetGenrePreferences( int typen, int valuel )
{
	genrePreferences_[ typen ] = valuel;
}

void IUser::RemoveOldModificators_( const SYSTEMTIME& time )
{
	for( size_t cnt=0; cnt < modificators_.size(); cnt++ )
		if( modificators_[ cnt ]->GetTime() < time )
		{
			delete modificators_[ cnt ];
			modificators_.erase( modificators_.begin() + cnt );
			cnt--;
		}
}

void IUser::BeginNewDay( const SYSTEMTIME& time )
{
	SetValue<int>( HANGRY, 100 );
	RemoveOldModificators_( time );
}

void IUser::AddModificator( IModificator* ptrModificator )
{
	modificators_.push_back( ptrModificator );
}

void IUser::IncreaseExperience( int techGroup, int grow )
{
	if( genreExperience_.find( techGroup ) != genreExperience_.end() )
		genreExperience_[ techGroup ] += grow;
	else 
		genreExperience_[ techGroup ] = grow;
}

void IUser::CheckModificators_()
{
	/*
	MODIFICATOR_LIST::iterator pIter = modificators_.begin();

	for( ; pIter < modificators_.end(); pIter++ )
	{
		if( ((CNrpUserModificator<R>*)*pIter)->GetName() == name )
			paramValue += ((CNrpUserModificator<R>*)*pIter)->GetValue();
	}
	*/
}

CNrpRelation* IUser::GetRelation( const std::string& name )
{
	RELATION_MAP::iterator rIter = _relations.find( name );
	if( rIter == _relations.end() )
		_relations[ name ] = new CNrpRelation( name, 0 );
	return  _relations[ name ];
}

}//namespace nrp