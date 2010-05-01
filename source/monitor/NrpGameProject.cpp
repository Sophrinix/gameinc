#include "StdAfx.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"
#include "IUser.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpApplication.h"

#include <io.h>
#include <errno.h>

namespace nrp
{

CNrpGameProject::CNrpGameProject( std::string name ) : INrpProject( "CNrpGameProject", name )
{
	InitializeOption_( name );
}

CNrpGameProject::CNrpGameProject( CNrpGameProject* nProject ) : INrpProject( "CNrpGameProject", nProject->GetValue<std::string>( NAME ) )
{
	InitializeOption_( nProject->GetValue<std::string>( NAME ) );
	std::vector< PNrpTechnology > techs;

	PNrpGameEngine ge = nProject->GetValue<PNrpGameEngine>( GAME_ENGINE );
	SetValue<PNrpGameEngine>( GAME_ENGINE, ge );
	CNrpTechnology* extEngine = new CNrpTechnology( PT_ENGINDEEXTEND );
	extEngine->SetValue<std::string>( NAME, GetValue<std::string>( NAME ) + " ���������� ������" );
	extEngine->SetValue<int>( CODEVOLUME, nProject->GetValue<int>( ENGINE_CODEVOLUME ) );
	extEngine->SetEmployerSkillRequire( SKL_CODING, ge->GetValue<int>( SKILL_CODING ) );
	SetValue<PNrpTechnology>( ENGINEEXTENDED, extEngine );

	CNrpTechnology* langSupport = new CNrpTechnology( PT_LANGSUPPORT );
	langSupport->SetValue<int>( CODEVOLUME, nProject->GetValue<int>( LANGUAGESUPPORTCODE ) );
	langSupport->SetValue<std::string>( NAME, "�����������" );
	langSupport->SetEmployerSkillRequire( SKL_CODING, 10 );
	SetValue<PNrpTechnology>( LOCALIZATION, langSupport );

	CNrpTechnology* cpCode = new CNrpTechnology( PT_PLATFORMSUPPORT );
	cpCode->SetValue<int>( CODEVOLUME, nProject->GetValue<int>( PLATFORMSUPPORTCODE ) );
	cpCode->SetValue<std::string>( NAME, "������������������ ���" );
	cpCode->SetEmployerSkillRequire( SKL_CODING, ge->GetValue<int>( SKILL_CODING ) );
	SetValue<PNrpTechnology>( CROSSPLATFORMCODE, cpCode );

	SetValue<int>( GENRE_MODULE_NUMBER, nProject->GetValue<int>( GENRE_MODULE_NUMBER ) );
	SetValue<PNrpGameProject>( PREV_GAME, nProject->GetValue<PNrpGameProject>( PREV_GAME ) );
	SetValue<int>( ENGINE_CODEVOLUME, nProject->GetValue<int>( ENGINE_CODEVOLUME) );
	SetValue<int>( BASE_CODEVOLUME, nProject->GetValue<int>( BASE_CODEVOLUME ) );
	SetValue<int>( TECHTYPE, nProject->GetValue<int>( TECHTYPE ) );
	SetValue<PNrpScenario>( SCENARIO, nProject->GetValue<PNrpScenario>( SCENARIO ) );
	SetValue<PNrpLicense>( GLICENSE, nProject->GetValue<PNrpLicense>( GLICENSE ) );

	int engineCodeVolume = GetValue<int>( ENGINE_CODEVOLUME );	
	if( nProject->GetValue<PNrpTechnology>( SCRIPTENGINE ) )
		SetValue<PNrpTechnology>( SCRIPTENGINE, new CNrpTechnology( *nProject->GetValue<PNrpTechnology>( SCRIPTENGINE ) ) );

	if( nProject->GetValue<PNrpTechnology>( MINIGAMEENGINE ) )
		SetValue<PNrpTechnology>( MINIGAMEENGINE, new CNrpTechnology( *nProject->GetValue<PNrpTechnology>( MINIGAMEENGINE ) ) );

	if( nProject->GetValue<PNrpTechnology>( PHYSICSENGINE ) )
		SetValue<PNrpTechnology>( PHYSICSENGINE, new CNrpTechnology( *nProject->GetValue<PNrpTechnology>( PHYSICSENGINE ) ) );

	if( nProject->GetValue<PNrpTechnology>( GRAPHICQUALITY ) )
		SetValue<PNrpTechnology>( GRAPHICQUALITY, new CNrpTechnology( *nProject->GetValue<PNrpTechnology>( GRAPHICQUALITY ) ) );

	if( nProject->GetValue<PNrpTechnology>( SOUNDQUALITY ) )
		SetValue<PNrpTechnology>( SOUNDQUALITY, new CNrpTechnology( *nProject->GetValue<PNrpTechnology>( SOUNDQUALITY ) ) );

	SetValue<int>( VIDEOTECHNUMBER, nProject->GetValue<int>( VIDEOTECHNUMBER ) );
	SetValue<int>( SOUNDTECHNUMBER, nProject->GetValue<int>( SOUNDTECHNUMBER ) );
	SetValue<int>( LANGNUMBER, nProject->GetValue<int>( LANGNUMBER ) );
	SetValue<int>( PLATFORMNUMBER, nProject->GetValue<int>( PLATFORMNUMBER ) );
	SetValue<int>( BASEQUALITY, nProject->GetValue<int>( BASEQUALITY ) );
	SetValue<bool>( PROJECTREADY, false );
	SetValue<int>( ADVTECHNUMBER, nProject->GetValue<int>( ADVTECHNUMBER ) );
	SetValue<int>( QUALITY, 0 );

	TECH_LIST::iterator pIter = nProject->technologies_.begin();
	technologies_.clear();
	for( ; pIter != nProject->technologies_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetValue<PNrpGameProject>( PARENT, this );
		technologies_.push_back( nTech );
	}

	pIter = nProject->genres_.begin();
	genres_.clear();
	for( ; pIter != nProject->genres_.end(); ++pIter )
	{
		if( *pIter == NULL )
			continue;

		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetValue<PNrpGameProject>( PARENT, this );
		genres_.push_back( nTech );
	}

	pIter = nProject->videoTechnologies_.begin();
	videoTechnologies_.clear();
	for( ; pIter != nProject->videoTechnologies_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetValue<PNrpGameProject>( PARENT, this );
		videoTechnologies_.push_back( nTech );
	}

	pIter = nProject->soundTechnologies_.begin();
	soundTechnologies_.clear();
	for( ; pIter != nProject->soundTechnologies_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetValue<PNrpGameProject>( PARENT, this );
		soundTechnologies_.push_back( nTech );
	}

	SetValue<std::string>( PROJECTSTATUS, std::string("develop") );
	CalculateCodeVolume();
}

CNrpGameProject::CNrpGameProject( CNrpGameProject& ptr ) : INrpProject( "CNrpGameProject", ptr.GetValue<std::string>( NAME ) )
{

}

CNrpGameProject::~CNrpGameProject(void)
{
}

void CNrpGameProject::SetGameEngine( CNrpGameEngine* gameEng )
{
	SetValue<PNrpGameEngine>( GAME_ENGINE, (PNrpGameEngine)gameEng );

	int modNumber = gameEng ? gameEng->GetValue<int>( GENRE_MODULE_NUMBER ) : 0;
	SetValue<int>( GENRE_MODULE_NUMBER, modNumber );

	int codeVol = gameEng ? gameEng->GetValue<int>( CODEVOLUME ) : 0;
	SetValue<int>( BASE_CODEVOLUME, codeVol );
	
	int quality = gameEng ? gameEng->GetValue<int>( QUALITY ) : 0;
	SetValue<int>( BASEQUALITY, quality );

	genres_.resize( modNumber, NULL );
	CalculateCodeVolume();
}

bool CNrpGameProject::IsGenreIncluded( GENRE_TYPE typen )
{
	TECH_LIST::iterator pIter = genres_.begin();

	for( ; pIter != genres_.end(); ++pIter)
		if( (*pIter) && (*pIter)->GetValue<GENRE_TYPE>(TECHTYPE) == typen )
			return true;

	return false;
}

void CNrpGameProject::FindPlaformsAndLanguages_()
{
	PropertyArray::iterator pIter = GetProperties().begin();

	int platformNumber = 0;
	int langNumber = 0;
	for( ; pIter != GetProperties().end(); ++pIter )
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
	bool setCodeVolume = GetValue<std::string>( PROJECTSTATUS ) == "develop";
	float baseCode = (float)GetValue<int>( BASE_CODEVOLUME );

	float summ = 0;
	float engCoeff = 1;
	int quality = 0;

	TECH_LIST rt = genres_;
	rt.insert( rt.end(), technologies_.begin(), technologies_.end() );
	rt.insert( rt.end(), videoTechnologies_.begin(), videoTechnologies_.end() );
	rt.insert( rt.end(), soundTechnologies_.begin(), soundTechnologies_.end() );
	rt.insert( rt.end(), genres_.begin(), genres_.end() );
	rt.push_back( GetValue<PNrpTechnology>( SCRIPTENGINE ) );
	rt.push_back( GetValue<PNrpTechnology>( MINIGAMEENGINE ) );
	rt.push_back( GetValue<PNrpTechnology>( PHYSICSENGINE ) );
	rt.push_back( GetValue<PNrpTechnology>( SOUNDQUALITY ) );
	rt.push_back( GetValue<PNrpTechnology>( GRAPHICQUALITY ) );
	rt.push_back( GetValue<PNrpTechnology>( SCENARIO ) );

	TECH_LIST::iterator techIter = rt.begin();
	for( ; techIter != rt.end(); ++techIter)
		if( (*techIter) != NULL )
		{
			engCoeff += (*techIter)->GetValue<float>( ENGINE_CODE );
			quality += (*techIter)->GetValue<int>( QUALITY );
			quality /= 2;
		}

	baseCode *= engCoeff;
	SetValue<int>( ENGINE_CODEVOLUME, (int)baseCode );

	techIter = rt.begin();
	for( ; techIter != rt.end(); ++techIter)
		if( (*techIter) != NULL ) 
		{
			float codeVol = (*techIter)->GetValue<float>( BASE_CODE ) * baseCode;
			if( setCodeVolume )
				(*techIter)->SetValue<int>( CODEVOLUME, (int)codeVol );
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
	TECH_LIST rt = technologies_;
	rt.push_back( GetValue<PNrpTechnology>(MINIGAMEENGINE));
	rt.push_back( GetValue<PNrpTechnology>(SCRIPTENGINE));
	rt.push_back( GetValue<PNrpTechnology>(PHYSICSENGINE));
	rt.push_back( GetValue<PNrpTechnology>(GRAPHICQUALITY));

	rt.insert(rt.end(), videoTechnologies_.begin(), videoTechnologies_.end() );
	rt.insert(rt.end(), technologies_.begin(), technologies_.end() );
	rt.insert(rt.end(), soundTechnologies_.begin(), soundTechnologies_.end() );
	TECH_LIST::iterator pIter = rt.begin();

	for( ; pIter != rt.end(); ++pIter)
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
	if( _access( folderSave.c_str(), 0 ) == -1 )
		CreateDirectory( folderSave.c_str(), NULL );

	std::string localFolder = folderSave + GetValue<std::string>( NAME ) + "/";

	if( _access( localFolder.c_str(), 0 ) == -1 )
		CreateDirectory( localFolder.c_str(), NULL );

	std::string fileName = localFolder + "project.ini";
	INrpProject::Save( PROPERTIES, fileName );

	TECH_LIST::iterator tIter = technologies_.begin();
	for( int i=0; tIter != technologies_.end(); ++tIter, ++i )
	{
		(*tIter)->Save( localFolder + ADVTECH + "/" );
		IniFile::Write( ADVTECH, ADVTECH + IntToStr(i), (*tIter)->GetValue<std::string>( NAME ), fileName );
	}

	TECH_LIST::iterator gIter = genres_.begin();
	for( int i=0; gIter != genres_.end(); ++gIter, ++i )
	{
		(*gIter)->Save( localFolder + GENRETECH  + "/" );
		IniFile::Write( GENRETECH, GENRETECH + IntToStr(i), (*gIter)->GetValue<std::string>( NAME ), fileName );
	}

	TECH_LIST::iterator vIter = videoTechnologies_.begin();
	for( int i=0; vIter != videoTechnologies_.end(); ++vIter, ++i )
	{
		(*vIter)->Save( localFolder + VIDEOTECH + "/" );
		IniFile::Write( VIDEOTECH, VIDEOTECH + IntToStr(i), (*vIter)->GetValue<std::string>( NAME ), fileName );
	}

	TECH_LIST::iterator sIter = soundTechnologies_.begin();
	for( int i=0; sIter != soundTechnologies_.end(); ++sIter, ++i )
	{
		(*sIter)->Save( localFolder + SOUNDTECH + "/" );
		IniFile::Write( SOUNDTECH, SOUNDTECH + IntToStr(i), (*sIter)->GetValue<std::string>( NAME ), fileName );
	}

	if( GetValue<PNrpGameEngine>( GAME_ENGINE ) )
	{
		PNrpGameEngine engine = GetValue<PNrpGameEngine>( GAME_ENGINE );
		IniFile::Write( PROPERTIES, GAME_ENGINE, engine->GetValue<std::string>( NAME ), fileName );
	}

	if( GetValue<PNrpGame>( PREV_GAME ) )
		IniFile::Write( PROPERTIES, PREV_GAME, GetValue<PNrpGame>( PREV_GAME )->GetValue<std::string>(NAME), fileName );

	if( GetValue<PNrpScenario>( SCENARIO ) )
	{
		IniFile::Write( PROPERTIES, SCENARIO, GetValue<PNrpScenario>( SCENARIO )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpScenario>( SCENARIO )->Save( PROPERTIES, localFolder + SCENARIO + ".ini" );
	}

	if( GetValue<PNrpLicense>( GLICENSE ) )
	{
		IniFile::Write( PROPERTIES, GLICENSE, GetValue<PNrpLicense>( GLICENSE )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpLicense>( GLICENSE )->Save( PROPERTIES, localFolder + GLICENSE + ".ini");
	}

	if( GetValue<PNrpTechnology>( ENGINEEXTENDED ) )
	{
		IniFile::Write( PROPERTIES, ENGINEEXTENDED, GetValue<PNrpTechnology>(ENGINEEXTENDED)->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( ENGINEEXTENDED )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( LOCALIZATION ) )
	{
		IniFile::Write( PROPERTIES, LOCALIZATION, GetValue<PNrpTechnology>(LOCALIZATION)->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( LOCALIZATION )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( CROSSPLATFORMCODE ) )
	{
		IniFile::Write( PROPERTIES, CROSSPLATFORMCODE, GetValue<PNrpTechnology>(CROSSPLATFORMCODE)->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( CROSSPLATFORMCODE )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( SCRIPTENGINE ) )
	{
		IniFile::Write( PROPERTIES, SCRIPTENGINE, GetValue<PNrpTechnology>( SCRIPTENGINE )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( SCRIPTENGINE )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( MINIGAMEENGINE ) )
	{
		IniFile::Write( PROPERTIES, MINIGAMEENGINE, GetValue<PNrpTechnology>( MINIGAMEENGINE )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( MINIGAMEENGINE )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( PHYSICSENGINE ) )
	{
		IniFile::Write( PROPERTIES, PHYSICSENGINE, GetValue<PNrpTechnology>( PHYSICSENGINE )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( PHYSICSENGINE )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( GRAPHICQUALITY ) )
	{
		IniFile::Write( PROPERTIES, GRAPHICQUALITY, GetValue<PNrpTechnology>( GRAPHICQUALITY )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( GRAPHICQUALITY )->Save( localFolder );
	}

	if( GetValue<PNrpTechnology>( SOUNDQUALITY ) )
	{
		IniFile::Write( PROPERTIES, SOUNDQUALITY, GetValue<PNrpTechnology>( SOUNDQUALITY )->GetValue<std::string>(NAME), fileName );
		GetValue<PNrpTechnology>( SOUNDQUALITY )->Save( localFolder );
	}
}

void CNrpGameProject::Load( std::string loadFolder )
{
	if( loadFolder[ loadFolder.length() - 1 ] != '/' )
		loadFolder += "/";
	std::string fileName = loadFolder + "project.ini";
	INrpProject::Load( PROPERTIES, fileName );

	for( int i=0; i < GetValue<int>( ADVTECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( ADVTECH, ADVTECH + IntToStr(i), std::string(""), fileName );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE(0) );
		tech->Load( loadFolder + ADVTECH + "/" + name + ".ini" );
		tech->SetValue<LPVOID>( PARENT, this );
		technologies_.push_back( tech );
	}

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
	{
		std::string name = IniFile::Read( GENRETECH, GENRETECH + IntToStr(i), std::string(""), fileName );
		if( !name.empty() )
		{
			CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
			tech->Load( loadFolder + GENRETECH + "/" + name + ".ini" );
			tech->SetValue<LPVOID>( PARENT, this );
			genres_.push_back( tech );		
		}
	}

	for( int i=0; i < GetValue<int>( VIDEOTECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( VIDEOTECH, VIDEOTECH + IntToStr(i), std::string(""), fileName );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
		tech->Load( loadFolder + VIDEOTECH + "/" + name + ".ini" );
		tech->SetValue<LPVOID>( PARENT, this );
		videoTechnologies_.push_back( tech );
	}

	for( int i=0; i < GetValue<int>( SOUNDTECHNUMBER ); ++i )
	{
		std::string name = IniFile::Read( SOUNDTECH, SOUNDTECH + IntToStr(i), std::string(""), fileName );
		CNrpTechnology* tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
		tech->Load( loadFolder + SOUNDTECH + "/" + name + ".ini" );
		tech->SetValue<LPVOID>( PARENT, this );
		soundTechnologies_.push_back( tech );
	}

	std::string name = IniFile::Read( PROPERTIES, GAME_ENGINE, std::string(""), fileName );
	SetValue<PNrpGameEngine>( GAME_ENGINE, CNrpApplication::Instance().GetGameEngine( name ) );

	name = IniFile::Read( PROPERTIES, SCENARIO, std::string(""), fileName );
	PNrpScenario scenario = new CNrpScenario( name );
	scenario->Load( PROPERTIES, loadFolder + SCENARIO + ".ini" ); 
	SetValue<PNrpScenario>( SCENARIO, scenario );

	name = IniFile::Read( PROPERTIES, GLICENSE, std::string(""), fileName );
	PNrpLicense license = new CNrpLicense( name );
	license->Load( PROPERTIES, loadFolder + GLICENSE + ".ini" );
	SetValue<PNrpLicense>( GLICENSE, license );

	name = IniFile::Read( PROPERTIES, SCRIPTENGINE, std::string(""), fileName );
	PNrpTechnology tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( SCRIPTENGINE, tech );

	name = IniFile::Read( PROPERTIES, ENGINEEXTENDED, std::string(""), fileName );
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( ENGINEEXTENDED, tech );

	name = IniFile::Read( PROPERTIES, LOCALIZATION, std::string(""), fileName );
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( LOCALIZATION, tech );

	name = IniFile::Read( PROPERTIES, CROSSPLATFORMCODE, std::string(""), fileName );
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( CROSSPLATFORMCODE, tech );

	name = IniFile::Read( PROPERTIES, MINIGAMEENGINE, std::string(""), fileName );
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( MINIGAMEENGINE, tech );

	name = IniFile::Read( PROPERTIES, PHYSICSENGINE, std::string(""), fileName );
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this ); 
	SetValue<PNrpTechnology>( PHYSICSENGINE, tech );

	name = IniFile::Read( PROPERTIES, GRAPHICQUALITY, std::string(""), fileName );
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( GRAPHICQUALITY, tech );

	name = IniFile::Read( PROPERTIES, SOUNDQUALITY, std::string(""), fileName );	
	tech = new CNrpTechnology( PROJECT_TYPE( 0 ) );
	tech->Load( loadFolder + name + ".ini" );
	tech->SetValue<LPVOID>( PARENT, this );
	SetValue<PNrpTechnology>( SOUNDQUALITY, tech );
}

void CNrpGameProject::InitializeOption_( std::string name )
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
	CreateValue<bool>( PROJECTREADY, false );
	CreateValue<int>( ADVTECHNUMBER, 0 );
	CreateValue<int>( ENGINE_CODEVOLUME, 0 );
	CreateValue<int>( QUALITY, 0 );
	CreateValue<std::string>( COMPANY, "" );
	CreateValue<std::string>( PROJECTSTATUS, "unknown" );
	CreateValue<int>( PLATFORMSUPPORTCODE, 0 );
	CreateValue<int>( LANGUAGESUPPORTCODE, 0 );
	CreateValue<PNrpTechnology>( ENGINEEXTENDED, NULL );
	CreateValue<PNrpTechnology>( LOCALIZATION, NULL );
	CreateValue<PNrpTechnology>( CROSSPLATFORMCODE, NULL ); 
}
}//namespace nrp