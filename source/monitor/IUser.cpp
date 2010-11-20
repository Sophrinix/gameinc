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
#include "NrpUserModificator.h"
#include "timeHelpers.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{
CLASS_NAME CLASS_USER( "IUser" );

IUser::IUser(const NrpText& className, const NrpText& systemName ) : INrpConfig( className, systemName )
{
	assert( systemName.size() );
	Push<NrpText>( NAME, systemName );
	Push<int>( CODE_SPEED, 0 );
	Push<int>( CODE_QUALITY, 0 );
	Push<int>( KNOWLEDGE_LEVEL, 0 );
	Push<NrpText>( INTERNAL_NAME, systemName );
	Push<int>( TALANT, 0 );
	Push<int>( STAMINA, 0 );
	Push<int>( MOOD, 0 );
	Push<int>( POPULARITY, 0 );
	Push<int>( SALARY, 0 );
	Push<int>( STABILITY, 0 );
	Push<int>( BALANCE, 0 );
	Push<int>( CHARACTER, 0 );
	Push<int>( WANTMONEY, 0 );
	Push<int>( CONTRACTMONEY, 0 );
	Push<int>( WORKNUMBER, 0 );
	Push<NrpText>( USERSTATE, "readyToWork" );
	Push<NrpText>( ROOMSTATE, "unknown" );
	Push<int>( HANGRY, 100 );
	Push<PNrpCompany>( PARENTCOMPANY, NULL );
	Push<int>( EXPERIENCE, 0 );
	Push<NrpText>( TEXTURENORMAL, "" );
}

IUser::~IUser(void)
{
}

void IUser::SetSkill( int typen, int valuel )
{
	knowledges_[ typen ] = valuel;

	CalculateWantSalary_();
}

void IUser::SetSkill( const NrpText& name, int valuel )
{
	Param( name ) = valuel;

	CalculateWantSalary_();
}

void IUser::CalculateWantSalary_()
{
	KNOWLEDGE_MAP::Iterator pIter = knowledges_.getIterator();
	float sum = 0;
	float cash = 500;
	for( ; !pIter.atEnd(); pIter++)
	{
		 sum += pIter->getValue() * cash / 100;
		 cash *= (cash > 100 ? 0.9f : 1);
	}

	sum += (int)Param( CODE_QUALITY ) * 10;

	Param( WANTMONEY ) = (int)sum;

	CalculateKnowledgeLevel_();
}

void IUser::CalculateKnowledgeLevel_()
{
	int sum = 0;
	KNOWLEDGE_MAP::Iterator pIter = knowledges_.getIterator();
	for( ; !pIter.atEnd(); pIter++)
	{
				sum += pIter->getValue();
				sum /= 2;
	}
	
	Param( KNOWLEDGE_LEVEL ) = sum;
}

int IUser::GetSkill( int typen )
{
	return knowledges_.find( typen ) != NULL ? knowledges_[ typen ] : 0;
}

NrpText IUser::Save( const NrpText& folderPath )
{
	try
	{
		assert( OpFileSystem::IsExist( folderPath ) );

		NrpText fileName = OpFileSystem::CheckEndSlash( folderPath )+ Param( NAME ).As<NrpText>() + ".user";
		assert( !OpFileSystem::IsExist( fileName ) );
		
		IniFile sv( fileName );

		KNOWLEDGE_MAP::Iterator gnrIter = genrePreferences_.getIterator();
		for( ; !gnrIter.atEnd(); gnrIter++ )
			sv.Set( "genrePreference", gnrIter->getKey(), gnrIter->getValue() );
		
		KNOWLEDGE_MAP::Iterator gnrExp = genreExperience_.getIterator();
		for( ; !gnrExp.atEnd(); gnrExp++ )
			sv.Set( "genreExperience", gnrExp->getKey(), gnrExp->getValue() );

		KNOWLEDGE_MAP::Iterator knIter = knowledges_.getIterator();
		for( ; !knIter.atEnd(); knIter++ )
			sv.Set( "knowledges", knIter->getKey(), knIter->getValue() );

		for( u32 i=0; i < works_.size(); i++ )
		{
			if( CNrpProjectModule* prjModule = dynamic_cast< CNrpProjectModule* >( works_[ i ] )  )
			{
				NrpText projectName = (*prjModule)[ PARENT ].As<INrpProject*>()->Text( NAME );
				NrpText name = (*prjModule)[NAME];
				sv.Set( SECTION_WORKS, CreateKeyWork( i ), "project" );
				sv.Set( SECTION_WORKS, CreateKeyProject( i ), projectName );
				sv.Set( SECTION_WORKS, CreateKeyModule( i ), name );
			}
			else if( CNrpInvention* invention = dynamic_cast< CNrpInvention* >( works_[ i ] ) )
			{
				NrpText name = (*invention)[NAME];
				sv.Set( SECTION_WORKS, CreateKeyWork( i ), "invention" );
				sv.Set( SECTION_WORKS, CreateKeyInvention( i ), name );
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

void IUser::Load( const NrpText& fileName )
{
	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::Load( fileName );
	assert( Param( NAME ).As<NrpText>().size() > 0 );

	IniFile rv( fileName );
	rv.Get( "knowledges", knowledges_ );

	for( int k=0; k < (int)Param( WORKNUMBER ); k++ )
	{
		NrpText action = "";
		NrpText workType = rv.Get( SECTION_WORKS, CreateKeyWork( k ), NrpText("") );
		if( workType.size() )
		{
			if( workType.equals_ignore_case( "project" ) )
			{
				NrpText projectName = rv.Get( SECTION_WORKS, CreateKeyProject( k ), NrpText("") );	
				NrpText moduleName = rv.Get( SECTION_WORKS, CreateKeyModule( k ), NrpText("") );
				action = NrpText( "autoscript:AddUserToGameProject(\"" ) + Text( NAME ) + "\", \"" + projectName + "\", \"" + moduleName + "\")";
			}
			else if( workType.equals_ignore_case( "invention" ) )
			{
				NrpText inventionName = rv.Get( SECTION_WORKS, CreateKeyInvention( k ), NrpText("") );	
				action = NrpText( "autoscript:AddUserToInvention(\"" ) + Text( NAME ) + "\" , \"" + inventionName + "\")";	
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

	TECH_LIST::iterator tlIter = techWorks_.begin();
	//for( ; tlIter != techWorks_.end(); ++tlIter )
	//	IniFile::Write( "knowledges", tlIter->first, tlIter->second, fileName );

	PropertyArray::iterator paIter = GetProperties().begin();
	for( ; paIter != GetProperties().end(); ++paIter)
	{
		INrpProperty* prop = paIter->second;
		if( prop->GetValueType() == typeid( int ).name() )
			IniFile::Write( PROPERTIES, paIter->first, ((CNrpProperty<int>*)prop)->GetValue(), fileName );

		if( prop->GetValueType() == typeid( NrpText ).name() )
			IniFile::Write( PROPERTIES, paIter->first, ((CNrpProperty<NrpText>*)prop)->GetValue(), fileName );
	}*/
}

void IUser::AddWork( IWorkingModule* module, bool toFront )
{
	assert( module != NULL );

	if( GetWork( module->Text( NAME ) ) == NULL )
	{
		if( toFront )
			works_.insert( module, 0 );
		else
			works_.push_back( module );

		module->AddUser( this );
	}

	Param( WORKNUMBER ) = static_cast< int >( works_.size() );
}

void IUser::RemoveWork( IWorkingModule* techWork )
{
	assert( techWork != NULL );
	if( techWork == NULL )
		return;

	for( u32 i=0; i < works_.size(); i++ )
		if( works_[ i ] == techWork )
		{
			techWork->RemoveUser( Text( NAME ) );
			works_.erase( i );
			Param( WORKNUMBER ) = static_cast< int >( works_.size() );
			return;
		}
		
	NrpText text = NrpText( "Не могу найти компонент для удаления " ) + techWork->Text( NAME );
	Log(HW) << text << term;
}

IWorkingModule* IUser::GetWork( u32 index ) const
{
	assert( index < works_.size() );
	return ( index < works_.size()) ? works_[ index ] : NULL;
}

IWorkingModule* IUser::GetWork( const NrpText& name ) const
{
	for( u32 i=0; i < works_.size(); i++ )
		if( (*works_[ i ])[ NAME ] == name )
			return works_[ i ];

	return NULL;
}

void IUser::BeginNewHour( const SYSTEMTIME& time )
{
	if( time.wHour == 9 && Text( USERSTATE ).equals_ignore_case( "readyToWork" ) )
	{
		Param( USERSTATE ) = NrpText( "work" );
	}

	Param( HANGRY ) -= 15;
	if(	(int)Param( HANGRY ) < 30 )
	{
		SYSTEMTIME endTime = time;
		endTime.wHour = 23;
		AddModificator( new CNrpUserModificator<int>( this, endTime, MOOD, true, 45 ) );
	}

	if( time.wHour == 18 )
	{
		Param( USERSTATE ) = NrpText( "readyToWork" );
	}

	if( Text( USERSTATE ).equals_ignore_case( "work" ) )
	{
		if( works_.size() > 0 )
		{
			//закончили обработку компонента
			if( (*works_[ 0 ] )[ READYWORKPERCENT ] >= 1.f )
				RemoveWork( works_[ 0 ] );
			else
				works_[ 0 ]->Update( this );			
		}
	}
}

int IUser::GetGenreExperience( GENRE_TYPE typen )
{
	KNOWLEDGE_MAP::Node* kIter = genreExperience_.find( typen );
	return kIter != NULL ? kIter->getValue() : 0;
}

int IUser::GetGenrePreferences( GENRE_TYPE typen )
{
	KNOWLEDGE_MAP::Node* kIter = genrePreferences_.find( typen );
	return kIter != NULL ? kIter->getValue() : 0;
}

void IUser::SetGenreExperience( GENRE_TYPE typen, int valuel )
{
	genreExperience_[ typen ] = valuel;
}

void IUser::SetGenrePreferences( GENRE_TYPE typen, int valuel )
{
	genrePreferences_[ typen ] = valuel;
}

void IUser::RemoveOldModificators_( const SYSTEMTIME& time )
{
	for( size_t cnt=0; cnt < modificators_.size(); cnt++ )
		if( TimeHelper::TimeCmp( time, modificators_[ cnt ]->GetTime() ) == -1  )
		{
			delete modificators_[ cnt ];
			modificators_.erase( cnt );
			cnt--;
		}
}

void IUser::BeginNewDay( const SYSTEMTIME& time )
{
	Param( HANGRY ) = 100;
	RemoveOldModificators_( time );
}

void IUser::AddModificator( IModificator* ptrModificator )
{
	modificators_.push_back( ptrModificator );
}

void IUser::IncreaseExperience( int techGroup, int grow )
{
	genreExperience_[ techGroup ] = (genreExperience_.find( techGroup ) != NULL ? genreExperience_[ techGroup ] : 0) + grow;
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

CNrpRelation* IUser::GetRelation( const NrpText& name )
{
	if( _relations.find( name ) == NULL )
		_relations[ name ] = new CNrpRelation( name, 0 );
	
	return  _relations[ name ];
}

bool IUser::Equale( const NrpText& name )
{
	return Text( NAME ) == name;
}

NrpText IUser::ClassName()
{
	return CLASS_USER;
}
}//namespace nrp