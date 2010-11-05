#include "StdAfx.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"

#include <errno.h>

namespace nrp
{

CNrpGameProject::CNrpGameProject( std::string name, CNrpCompany* ptrCompany ) : INrpProject( CLASS_GAMEPROJECT, name )
{
	InitializeOptions_( name );
	SetValue<PNrpCompany>( PARENTCOMPANY, ptrCompany );
}

CNrpGameProject::CNrpGameProject( CNrpGameProject& ptr ) : INrpProject( CLASS_GAMEPROJECT, ptr.GetValue<std::string>( NAME ) )
{
	throw "function not done";
}

CNrpGameProject::~CNrpGameProject(void)
{
}

void CNrpGameProject::SetGameEngine( CNrpGameEngine* gameEng )
{
	SetValue<PNrpGameEngine>( GAME_ENGINE, (PNrpGameEngine)gameEng );
	int modNumber = gameEng ? gameEng->GetValue<int>( GENRE_MODULE_NUMBER ) : 0;
	SetValue<int>( GENRE_MODULE_NUMBER, modNumber );
	SetValue<int>( BASE_CODEVOLUME, gameEng ? gameEng->GetValue<int>( CODEVOLUME ) : 0 );
	SetValue<int>( BASEQUALITY, gameEng ? gameEng->GetValue<int>( QUALITY ) : 0 );

	genres_.resize( modNumber, NULL );
	CalculateCodeVolume();
}

bool CNrpGameProject::IsGenreIncluded( GENRE_TYPE typen )
{
	TECH_LIST::iterator pIter = genres_.begin();

	for( ; pIter != genres_.end(); pIter++)
		if( (*pIter) && (*pIter)->GetValue<GENRE_TYPE>(TECHTYPE) == typen )
			return true;

	return false;
}

void CNrpGameProject::FindPlaformsAndLanguages_()
{
	PropertyArray::iterator pIter = GetProperties().begin();

	int platformNumber = 0;
	int langNumber = 0;
	for( ; pIter != GetProperties().end(); pIter++ )
	{
		if( (*pIter).first.find( "platform_" ) == 0 )
			platformNumber += GetValue<bool>( (*pIter).first ) ? 1 : 0;

		if( (*pIter).first.find( "language_" ) == 0 )
			langNumber += GetValue<bool>( (*pIter).first ) ? 1 : 0;
	}

	SetValue<int>( LANGNUMBER, langNumber );
	SetValue<int>( PLATFORMNUMBER, platformNumber );
}

void CNrpGameProject::CalculateCodeVolume()
{
	FindPlaformsAndLanguages_();

	float baseCode = (float)GetValue<int>( BASE_CODEVOLUME );

	float summ = 0;
	float engCoeff = 1;
	int quality = 0;

	TECH_LIST rt;
	GetAllTech_( rt );

	TECH_LIST::iterator techIter = rt.begin();
	for( ; techIter != rt.end(); techIter++)
		if( (*techIter) != NULL )
		{
			engCoeff += (*techIter)->GetValue<float>( ENGINE_CODE );
			quality += (*techIter)->GetValue<int>( QUALITY );
			quality /= 2;
		}

	baseCode *= engCoeff;
	SetValue<int>( ENGINE_CODEVOLUME, (int)baseCode );

	techIter = rt.begin();
	for( ; techIter != rt.end(); techIter++)
		if( (*techIter) != NULL ) 
		{
			float codeVol = (*techIter)->GetValue<float>( BASE_CODE ) * baseCode;
			summ += codeVol;
		}

	summ += baseCode;
	float langSupCode = summ * GetValue<int>( LANGNUMBER ) * 0.05f;
	SetValue<int>( LANGUAGESUPPORTCODE, (int)langSupCode );
	summ += langSupCode;

	float platformSupCode = summ *  GetValue<int>( PLATFORMNUMBER ) * 0.1f;
	SetValue<int>( PLATFORMSUPPORTCODE, (int)platformSupCode );
	summ += platformSupCode;
	
	bool projectReady = GetValue<PNrpGameEngine>( GAME_ENGINE ) != NULL;
	projectReady &= ( GetValue<PNrpScenario>( SCENARIO ) != NULL || GetValue<PNrpLicense>( GLICENSE ) != NULL );
	projectReady &= ( GetValue<int>( LANGNUMBER ) > 0 && GetValue<int>( PLATFORMNUMBER ) > 0 );
	projectReady &= ( GetGenre( 0 ) != NULL );
	
	SetValue<int>( CODEVOLUME, (int)summ );
	SetValue<int>( QUALITY, quality ); 
	SetValue<bool>( PROJECTREADY, projectReady );
}

void CNrpGameProject::SetGenre( CNrpTechnology* genre, int number )
{
	if( number >= (int)genres_.size() )
		return;

	TECH_LIST::iterator pIter = genres_.begin();

	if( !genre )
		genres_[ number ] = NULL;
	else
	{	
		GENRE_TYPE techType = genre->GetValue<GENRE_TYPE>( TECHTYPE );

		if( GetValue<PNrpGameEngine>( GAME_ENGINE )->IsGenreAvailble( techType ) &&
			!IsGenreIncluded( techType ) )
			genres_[ number ] = genre;
	}

	CalculateCodeVolume();
}

CNrpTechnology* CNrpGameProject::GetGenre( int index )
{
	return index < (int)genres_.size() ? genres_[ index ] : NULL;
}

bool CNrpGameProject::IsTechInclude( ADV_TECH_TYPE typen )
{
	TECH_LIST rt;
	GetAllTech_( rt );
	TECH_LIST::iterator pIter = rt.begin();

	for( ; pIter != rt.end(); pIter++)
		if( (*pIter) && (*pIter)->GetValue<ADV_TECH_TYPE>(TECHTYPE) == typen )
			return true;

	return false;
}

void CNrpGameProject::SetTech_( CNrpTechnology* ptrTech, int index, TECH_LIST& listd, OPTION_NAME optname )
{
	if( index >= (int)listd.size() )
		listd.push_back( ptrTech );
	else
	{
		if( !ptrTech )
			listd.erase( listd.begin() + index );
		else
			listd[ index ] = ptrTech;
	}
	SetValue<int>( optname, listd.size() );
	CalculateCodeVolume();
}

void CNrpGameProject::SetTechnology( CNrpTechnology* ptrTech, int index )
{
	SetTech_( ptrTech, index, technologies_, ADVTECHNUMBER );
}

CNrpTechnology* CNrpGameProject::GetTechnology( int index )
{
	return ( index < (int)technologies_.size() ) ? technologies_[ index ] : NULL;
}

void CNrpGameProject::SetVideoTech( CNrpTechnology* ptrTech, int index )
{
	SetTech_( ptrTech, index, videoTechnologies_, VIDEOTECHNUMBER );
}

CNrpTechnology* CNrpGameProject::GetVideoTech( int index )
{
	return index < (int)videoTechnologies_.size() ? videoTechnologies_[ index ] : NULL;
}

CNrpTechnology* CNrpGameProject::GetSoundTech( int index )
{
	return index < (int)soundTechnologies_.size() ? soundTechnologies_[ index ] : NULL;
}

void CNrpGameProject::SetSoundTech( CNrpTechnology* ptrTech, int index )
{
	SetTech_( ptrTech, index, soundTechnologies_, SOUNDTECHNUMBER );
}

void CNrpGameProject::Save( std::string folderSave )
{
	assert( OpFileSystem::IsExist(folderSave) );

	std::string localFolder = OpFileSystem::CheckEndSlash( folderSave + GetValue<std::string>( NAME ) );

	OpFileSystem::CreateDirectory( localFolder );

	std::string fileName = localFolder + "project.ini";
	INrpProject::Save( fileName );

	TECH_LIST::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); tIter++, i++ )
		IniFile::Write( ADVTECH, ADVTECH + conv::ToStr(i), (*tIter)->GetValue<std::string>( NAME ), fileName );

	TECH_LIST::iterator gIter = genres_.begin();
	for( int i=0; gIter != genres_.end(); gIter++, i++ )
		IniFile::Write( GENRETECH, GENRETECH + conv::ToStr(i), (*gIter)->GetValue<std::string>( NAME ), fileName );

	TECH_LIST::iterator vIter = videoTechnologies_.begin();
	for( int i=0; vIter != videoTechnologies_.end(); vIter++, i++ )
		IniFile::Write( VIDEOTECH, VIDEOTECH + conv::ToStr(i), (*vIter)->GetValue<std::string>( NAME ), fileName );

	TECH_LIST::iterator sIter = soundTechnologies_.begin();
	for( int i=0; sIter != soundTechnologies_.end(); sIter++, i++ )
		IniFile::Write( SOUNDTECH, SOUNDTECH + conv::ToStr(i), (*sIter)->GetValue<std::string>( NAME ), fileName );

	if( GetValue<PNrpGameEngine>( GAME_ENGINE ) )
	{
		PNrpGameEngine engine = GetValue<PNrpGameEngine>( GAME_ENGINE );
		IniFile::Write( SECTION_PROPERTIES, GAME_ENGINE, engine->GetValue<std::string>( NAME ), fileName );
	}

	if( GetValue<PNrpGame>( PREV_GAME ) )
		IniFile::Write( SECTION_PROPERTIES, PREV_GAME, GetValue<PNrpGame>( PREV_GAME )->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpScenario>( SCENARIO ) )
	{
		IniFile::Write( SECTION_PROPERTIES, SCENARIO, GetValue<PNrpScenario>( SCENARIO )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpScenario>( SCENARIO )->Save( localFolder + SCENARIO + ".ini" );
	}

	if( GetValue<PNrpLicense>( GLICENSE ) )
	{
		IniFile::Write( SECTION_PROPERTIES, GLICENSE, GetValue<PNrpLicense>( GLICENSE )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpLicense>( GLICENSE )->Save( localFolder + GLICENSE + ".ini");
	}

	if( GetValue<PNrpTechnology>( ENGINEEXTENDED ) )
		IniFile::Write( SECTION_PROPERTIES, ENGINEEXTENDED, GetValue<PNrpTechnology>(ENGINEEXTENDED)->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( LOCALIZATION ) )
		IniFile::Write( SECTION_PROPERTIES, LOCALIZATION, GetValue<PNrpTechnology>(LOCALIZATION)->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( CROSSPLATFORMCODE ) )
		IniFile::Write( SECTION_PROPERTIES, CROSSPLATFORMCODE, GetValue<PNrpTechnology>(CROSSPLATFORMCODE)->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( SCRIPTENGINE ) )
		IniFile::Write( SECTION_PROPERTIES, SCRIPTENGINE, GetValue<PNrpTechnology>( SCRIPTENGINE )->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( MINIGAMEENGINE ) )
		IniFile::Write( SECTION_PROPERTIES, MINIGAMEENGINE, GetValue<PNrpTechnology>( MINIGAMEENGINE )->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( PHYSICSENGINE ) )
		IniFile::Write( SECTION_PROPERTIES, PHYSICSENGINE, GetValue<PNrpTechnology>( PHYSICSENGINE )->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( GRAPHICQUALITY ) )
		IniFile::Write( SECTION_PROPERTIES, GRAPHICQUALITY, GetValue<PNrpTechnology>( GRAPHICQUALITY )->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpTechnology>( SOUNDQUALITY ) )
		IniFile::Write( SECTION_PROPERTIES, SOUNDQUALITY, GetValue<PNrpTechnology>( SOUNDQUALITY )->GetValue<std::string>(NAME), fileName );
}

void CNrpGameProject::Load( std::string loadFolder )
{
	if( loadFolder[ loadFolder.length() - 1 ] != '/' )
		loadFolder += "/";

	std::string fileName = loadFolder + "project.ini";
	CNrpCompany* ptrCompany = GetValue<PNrpCompany>( PARENTCOMPANY );
	INrpProject::Load( fileName );

	for( int i=0; i < GetValue<int>( ADVTECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( ADVTECH, ADVTECH + conv::ToStr(i), std::string(""), fileName );
		PNrpTechnology tech = CNrpApplication::Instance().GetTechnology( name );
		if( tech )
			technologies_.push_back( tech );
	}

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
	{
		std::string name = IniFile::Read( GENRETECH, GENRETECH + conv::ToStr(i), std::string(""), fileName );
		PNrpTechnology tech = CNrpApplication::Instance().GetTechnology( name );
		if( tech )
		    genres_.push_back( tech );		
	}

	for( int i=0; i < GetValue<int>( VIDEOTECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( VIDEOTECH, VIDEOTECH + conv::ToStr(i), std::string(""), fileName );
		PNrpTechnology tech = CNrpApplication::Instance().GetTechnology( name );
		if( tech )
			videoTechnologies_.push_back( tech );
	}

	for( int i=0; i < GetValue<int>( SOUNDTECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( SOUNDTECH, SOUNDTECH + conv::ToStr(i), std::string(""), fileName );
		PNrpTechnology tech = CNrpApplication::Instance().GetTechnology( name );
		if( tech )
			soundTechnologies_.push_back( tech );
	}

	std::string name = IniFile::Read( SECTION_PROPERTIES, GAME_ENGINE, std::string(""), fileName );
	SetValue<PNrpGameEngine>( GAME_ENGINE, CNrpApplication::Instance().GetGameEngine( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, SCENARIO, std::string(""), fileName );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( loadFolder + SCENARIO + ".ini" ); 
	SetValue<PNrpScenario>( SCENARIO, scenario );

	name = IniFile::Read( SECTION_PROPERTIES, GLICENSE, std::string(""), fileName );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( loadFolder + GLICENSE + ".ini" );
	SetValue<PNrpLicense>( GLICENSE, license );

	name = IniFile::Read( SECTION_PROPERTIES, SCRIPTENGINE, std::string(""), fileName );
	SetValue<PNrpTechnology>( SCRIPTENGINE, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, ENGINEEXTENDED, std::string(""), fileName );
	SetValue<PNrpTechnology>( ENGINEEXTENDED, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, LOCALIZATION, std::string(""), fileName );
	SetValue<PNrpTechnology>( LOCALIZATION, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, CROSSPLATFORMCODE, std::string(""), fileName );
	SetValue<PNrpTechnology>( CROSSPLATFORMCODE, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, MINIGAMEENGINE, std::string(""), fileName );
	SetValue<PNrpTechnology>( MINIGAMEENGINE, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, PHYSICSENGINE, std::string(""), fileName );
	SetValue<PNrpTechnology>( PHYSICSENGINE, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, GRAPHICQUALITY, std::string(""), fileName );
	SetValue<PNrpTechnology>( GRAPHICQUALITY, CNrpApplication::Instance().GetTechnology( name ) );

	name = IniFile::Read( SECTION_PROPERTIES, SOUNDQUALITY, std::string(""), fileName );	
	SetValue<PNrpTechnology>( SOUNDQUALITY, CNrpApplication::Instance().GetTechnology( name ) );
}

void CNrpGameProject::InitializeOptions_( std::string name )
{
	SetValue<PROJECT_TYPE>( TECHTYPE, PT_GAME );
	SetValue<std::string>( NAME, name );
	CreateValue<PNrpGameEngine>( GAME_ENGINE, NULL );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
	CreateValue<PNrpGame>( PREV_GAME, NULL );
	CreateValue<int>( CODEVOLUME, 0 );
	CreateValue<int>( BASE_CODEVOLUME, 0 );
	CreateValue<PNrpScenario>( SCENARIO, NULL );
	CreateValue<PNrpLicense>( GLICENSE, NULL ); 
	CreateValue<PNrpTechnology>( SCRIPTENGINE, NULL );
	CreateValue<PNrpTechnology>( MINIGAMEENGINE, NULL );
	CreateValue<PNrpTechnology>( PHYSICSENGINE, NULL );
	CreateValue<PNrpTechnology>( GRAPHICQUALITY, NULL );
	CreateValue<int>( VIDEOTECHNUMBER, 0 );
	CreateValue<PNrpTechnology>( SOUNDQUALITY, NULL );
	CreateValue<int>( SOUNDTECHNUMBER, 0 );
	CreateValue<int>( LANGNUMBER, 0 );
	CreateValue<int>( PLATFORMNUMBER, 0 );
	CreateValue<int>( BASEQUALITY, 0 );
	CreateValue<int>( ADVTECHNUMBER, 0 );
	CreateValue<int>( ENGINE_CODEVOLUME, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<std::string>( COMPANYNAME, "" );
	CreateValue<PNrpCompany>( PARENTCOMPANY, NULL );
	CreateValue<int>( PLATFORMSUPPORTCODE, 0 );
	CreateValue<int>( LANGUAGESUPPORTCODE, 0 );
	CreateValue<PNrpTechnology>( ENGINEEXTENDED, NULL );
	CreateValue<PNrpTechnology>( LOCALIZATION, NULL );
	CreateValue<PNrpTechnology>( CROSSPLATFORMCODE, NULL ); 
	CreateValue<int>( MONEYONDEVELOP, 0 );
	CreateValue<bool>( PROJECTREADY, false );
	CreateValue<float>( FAMOUS, 0 );
}

void CNrpGameProject::GetAllTech_( TECH_LIST& techList )
{
	techList = technologies_;
	techList.push_back( GetValue<PNrpTechnology>(MINIGAMEENGINE));
	techList.push_back( GetValue<PNrpTechnology>(SCRIPTENGINE));
	techList.push_back( GetValue<PNrpTechnology>(PHYSICSENGINE));
	techList.push_back( GetValue<PNrpTechnology>(GRAPHICQUALITY));
	techList.push_back( GetValue<PNrpTechnology>(SOUNDQUALITY));
	techList.push_back( GetValue<PNrpTechnology>(ENGINEEXTENDED));
	techList.push_back( GetValue<PNrpTechnology>(LOCALIZATION));
	techList.push_back( GetValue<PNrpTechnology>(CROSSPLATFORMCODE));

	techList.insert(techList.end(), videoTechnologies_.begin(), videoTechnologies_.end() );
	techList.insert(techList.end(), soundTechnologies_.begin(), soundTechnologies_.end() );
	techList.insert(techList.end(), genres_.begin(), genres_.end() );
}

}//namespace nrp