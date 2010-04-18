#include "StdAfx.h"
#include "IUser.h"
#include "INrpProject.h"

#include <io.h>
#include <errno.h>

namespace nrp
{

IUser::IUser(const char* className, const char* systemName ) : INrpConfig( className, systemName )
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
	KNOWLEDGE_LIST::iterator pIter = knowledges_.begin();
	float sum = 0;
	float cash = 500;
	for( ; pIter != knowledges_.end(); ++pIter)
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
	KNOWLEDGE_LIST::iterator pIter = knowledges_.begin();
	for( ; pIter != knowledges_.end(); ++pIter)
	{
				sum += pIter->second;
				sum /= 2;
	}
	
	SetValue<int>( KNOWLEDGE_LEVEL, sum );
}

int IUser::GetSkill( int typen )
{
	return knowledges_[ typen ];
}

void IUser::Save( std::string folderPath )
{
	if( _access( folderPath.c_str(), 0 ) == -1 )
		CreateDirectory( folderPath.c_str(), NULL );

	std::string fileName = folderPath + GetValue<std::string>( NAME ) + ".ini";
	DeleteFile( fileName.c_str() );

	NAMEVALUE_MAP::iterator gnrIter = genrePreferences_.begin();
	for( ; gnrIter != genrePreferences_.end(); ++gnrIter )
		IniFile::Write( "genrePreference", gnrIter->first, gnrIter->second, fileName );
	
	NAMEVALUE_MAP::iterator gnrExp = genreExperience_.begin();
	for( ; gnrExp != genreExperience_.end(); ++gnrExp )
		IniFile::Write( "genreExperience", gnrExp->first, gnrExp->second, fileName );

	KNOWLEDGE_LIST::iterator knIter = knowledges_.begin();
	for( ; knIter != knowledges_.end(); ++knIter )
		IniFile::Write( "knowledges", IntToStr( knIter->first ), knIter->second, fileName );

	USERACTION_LIST::iterator uaIter = peopleFeels_.begin();
	//for( ; uaIter != peopleFeels_.end(); ++uaIter )
	//	IniFile::Write( "knowledges", uaIter->first, uaIter->second, fileName );

	TECH_LIST::iterator tlIter = techWorks_.begin();
	//for( ; tlIter != techWorks_.end(); ++tlIter )
	//	IniFile::Write( "knowledges", tlIter->first, tlIter->second, fileName );

	INrpConfig::Save( PROPERTIES, fileName );
}

void IUser::Load( std::string fileName )
{
	INrpConfig::Load( PROPERTIES, fileName );

/*	NAMEVALUE_MAP::iterator gnrIter = genrePreferences_.begin();
	for( ; gnrIter != genrePreferences_.end(); ++gnrIter )
	IniFile::Write( "genrePreference", gnrIter->first, gnrIter->second, fileName );

	NAMEVALUE_MAP::iterator gnrExp = genreExperience_.begin();
	for( ; gnrExp != genreExperience_.end(); ++gnrExp )
		IniFile::Write( "genreExperience", gnrExp->first, gnrExp->second, fileName );

	KNOWLEDGE_LIST::iterator knIter = knowledges_.begin();
	for( ; knIter != knowledges_.end(); ++knIter )
		IniFile::Write( "knowledges", IntToStr( knIter->first ), knIter->second, fileName );

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
}//namespace nrp