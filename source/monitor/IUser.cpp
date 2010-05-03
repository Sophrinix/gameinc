#include "StdAfx.h"
#include "IUser.h"
#include "INrpProject.h"
#include "IniFile.h"
#include "NrpCompany.h"
#include "NrpTechnology.h"
#include "NrpGameProject.h"

#include <io.h>
#include <errno.h>
#include <assert.h>

namespace nrp
{

IUser::IUser(const char* className, const char* systemName, CNrpCompany* ptrCmp ) : INrpConfig( className, systemName )
{
	CreateValue<std::string>( NAME, "" );
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
	CreateValue<int>( TECHNUMBER, 0 );
	CreateValue<std::string>( USERSTATE, "readyToWork" );
	CreateValue<std::string>( ROOMSTATE, "unknown" );
	if( ptrCmp != NULL )
	{
		CreateValue<std::string>( COMPANYNAME, ptrCmp->GetValue<std::string>( NAME ) );
		CreateValue<PNrpCompany>( PARENTCOMPANY, ptrCmp );
	}
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

void IUser::Save( std::string folderPath )
{
	if( _access( folderPath.c_str(), 0 ) == -1 )
		CreateDirectory( folderPath.c_str(), NULL );

	std::string fileName = folderPath + GetValue<std::string>( NAME ) + ".ini";
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

	TECH_LIST::iterator tlIter = techWorks_.begin();
	for( int i=0; tlIter != techWorks_.end(); tlIter++, i++ )
		 IniFile::Write( TECHTYPE, TECHTYPE+IntToStr( i ), (*tlIter)->GetValue<std::string>(NAME), fileName );

	INrpConfig::Save( PROPERTIES, fileName );
}

void IUser::Load( std::string fileName )
{
	INrpConfig::Load( PROPERTIES, fileName );

	IniFile::ReadValueList_( "knowledges", knowledges_, fileName );

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

void IUser::AddTechWork( CNrpTechnology* techWork )
{
	assert( techWork != NULL );
	techWorks_.push_back( techWork );
	techWork->SetLider( this );
	SetValue<int>( TECHNUMBER, techWorks_.size() );
}

void IUser::RemoveTechWork( CNrpTechnology* techWork )
{
	assert( techWork != NULL );

	TECH_LIST::iterator tIter = techWorks_.begin();
	for( ; tIter != techWorks_.end(); tIter++ )
		if( (*tIter) == techWork )
		{
			techWork->SetLider( NULL );
			techWorks_.erase( tIter );
			SetValue<int>( TECHNUMBER, techWorks_.size() );
			return;
		}
	std::string text = "Не могу найти компонент для удаления " + techWork->GetValue<std::string>( NAME );
	OutputDebugString( text.c_str() );
}

CNrpTechnology* IUser::GetTechWork( int index )
{
	assert( index < (int)techWorks_.size() );
	return index < (int)techWorks_.size() ? techWorks_[ index ] : NULL;
}

void IUser::Update( const SYSTEMTIME& time )
{
	if( time.wHour == 9 && GetValue<std::string>( USERSTATE ) == "readyToWork" )
	{
		SetValue<std::string>( USERSTATE, "work" );
		std::string text = GetValue<std::string>( NAME ) + " приступил к работе\n";
		OutputDebugString( text.c_str() );
	}

	if( time.wHour == 18 )
	{
		SetValue<std::string>( USERSTATE, "readyToWork" );
	}

	if( GetValue<std::string>( USERSTATE ) == "work" )
	{
		if( techWorks_.size() > 0 )
		{
			float beforeUpdate = techWorks_[ 0 ]->GetValue<float>( READYWORKPERCENT );
			techWorks_[ 0 ]->Update( this );

			float afterUpdate = techWorks_[ 0 ]->GetValue<float>( READYWORKPERCENT );
			//закончили обработку компонента
			if( beforeUpdate != afterUpdate && afterUpdate == 1 )
			{
				CNrpGameProject* parent = techWorks_[ 0 ]->GetValue<PNrpGameProject>( PARENT );
				float growExp = techWorks_[ 0 ]->GetValue<int>( CODEVOLUME ) / (float)parent->GetValue<int>( CODEVOLUME );
				int techType = techWorks_[ 0 ]->GetValue<int>( TECHTYPE );
				
				if( genreExperience_.find( techType ) == genreExperience_.end() )
					genreExperience_[ techType ] = (int)growExp;
				else
					genreExperience_[ techType ] += (int)growExp;

				if( genrePreferences_.find( techType ) != genrePreferences_.end() )
					genrePreferences_[ techType ] += (int)growExp;

				techWorks_[ 0 ]->SetLider( NULL );
			}
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
}//namespace nrp