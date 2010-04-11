#include "StdAfx.h"
#include "NrpGameProject.h"
#include "NrpGameEngine.h"
#include "NrpScenario.h"
#include "NrpLicense.h"
#include "NrpTechnology.h"
#include "IUser.h"

namespace nrp
{

CNrpGameProject::CNrpGameProject( std::string name ) : INrpProject( "CNrpGameProject", name )
{
	PROJECT_TYPE typed = PT_GAME;
	SetOption<PROJECT_TYPE>( TYPE, typed );
	SetOption<std::string>( NAME, name );
	options_[ GAME_ENGINE ] = new PNrpGameEngine( NULL );
	options_[ GENRE_MODULE_NUMBER ] = new int( 0 );
	options_[ PREV_GAME ] = new PNrpGameProject( NULL );
	options_[ CODEVOLUME ] = new int( 0 );
	options_[ BASE_CODEVOLUME ] = new int( 0 );
	options_[ TECHTYPE ] = new int( 0 );
	options_[ SCENARIO ] = new PNrpScenario( NULL );
	options_[ GLICENSE ] = new PNrpLicense( NULL ); 
	options_[ SCRIPTENGINE ] = new PNrpTechnology( NULL );
	options_[ MINIGAMEENGINE ] = new PNrpTechnology( NULL );
	options_[ PHYSICSENGINE ] = new PNrpTechnology( NULL );
	options_[ GRAPHICQUALITY ] = new PNrpTechnology( NULL );
	options_[ VIDEOTECHNUMBER ] = new int( 0 );
	options_[ SOUNDQUALITY ] = new PNrpTechnology( NULL );
	options_[ SOUNDTECHNUMBER ] = new int( 0 );
	options_[ LANGNUMBER ] = new int( 0 );
	options_[ PLATFORMNUMBER ] = new int( 0 );
	options_[ BASEQUALITY ] = new int( 0 );
	options_[ PROJECTREADY ] = new bool( false );
	options_[ ADVTECHNUMBER ] = new int( 0 );
	options_[ ENGINE_CODEVOLUME ] = new int( 0 );
	options_[ QUALITY ] = new int( 0 );
}

CNrpGameProject::CNrpGameProject( CNrpGameProject* nProject ) : INrpProject( "CNrpGameProject", nProject->GetOption<std::string>( NAME ) )
{
	SetOption<PNrpGameEngine>( GAME_ENGINE, GetOption<PNrpGameEngine>( GAME_ENGINE ) );
	SetOption<int>( GENRE_MODULE_NUMBER, nProject->GetOption<int>( GENRE_MODULE_NUMBER ) );
	SetOption<PNrpGameProject>( PREV_GAME, nProject->GetOption<PNrpGameProject>( PREV_GAME ) );
	SetOption<int>( BASE_CODEVOLUME, nProject->GetOption<int>( BASE_CODEVOLUME ) );
	SetOption<int>( TECHTYPE, nProject->GetOption<int>( TECHTYPE ) );
	SetOption<PNrpScenario>( SCENARIO, nProject->GetOption<PNrpScenario>( SCENARIO ) );
	SetOption<PNrpLicense>( GLICENSE, nProject->GetOption<PNrpLicense>( GLICENSE ) );
	SetOption<PNrpTechnology>( SCRIPTENGINE, new CNrpTechnology( *nProject->GetOption<PNrpTechnology>( SCRIPTENGINE ) ) );
	SetOption<PNrpTechnology>( MINIGAMEENGINE, new CNrpTechnology( *nProject->GetOption<PNrpTechnology>( MINIGAMEENGINE ) ) );
	SetOption<PNrpTechnology>( PHYSICSENGINE, new CNrpTechnology( *nProject->GetOption<PNrpTechnology>( PHYSICSENGINE ) ) );
	SetOption<PNrpTechnology>( GRAPHICQUALITY, new CNrpTechnology( *nProject->GetOption<PNrpTechnology>( GRAPHICQUALITY ) ) );
	SetOption<int>( VIDEOTECHNUMBER, nProject->GetOption<int>( VIDEOTECHNUMBER ) );
	SetOption<PNrpTechnology>( SOUNDQUALITY, new CNrpTechnology( *nProject->GetOption<PNrpTechnology>( SOUNDQUALITY ) ) );
	SetOption<int>( SOUNDTECHNUMBER, nProject->GetOption<int>( SOUNDTECHNUMBER ) );
	SetOption<int>( LANGNUMBER, nProject->GetOption<int>( LANGNUMBER ) );
	SetOption<int>( PLATFORMNUMBER, nProject->GetOption<int>( PLATFORMNUMBER ) );
	SetOption<int>( BASEQUALITY, nProject->GetOption<int>( BASEQUALITY ) );
	SetOption<bool>( PROJECTREADY, false );
	SetOption<int>( ADVTECHNUMBER, nProject->GetOption<int>( ADVTECHNUMBER ) );
	SetOption<int>( ENGINE_CODEVOLUME, nProject->GetOption<int>( ENGINE_CODEVOLUME) );
	SetOption<int>( QUALITY, 0 );

	TECH_LIST::iterator pIter = nProject->technologies_.begin();
	technologies_.clear();
	for( ; pIter != nProject->technologies_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetOption<PNrpGameProject>( PARENT, this );
		nTech->SetOption<PUser>( COMPONENTLIDER, NULL ); 
		technologies_.push_back( nTech );
	}

	pIter = nProject->genres_.begin();
	genres_.clear();
	for( ; pIter != nProject->genres_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetOption<PNrpGameProject>( PARENT, this );
		nTech->SetOption<PUser>( COMPONENTLIDER, NULL ); 
		genres_.push_back( nTech );
	}

	pIter = nProject->videoTechnologies_.begin();
	videoTechnologies_.clear();
	for( ; pIter != nProject->videoTechnologies_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetOption<PNrpGameProject>( PARENT, this );
		nTech->SetOption<PUser>( COMPONENTLIDER, NULL ); 
		videoTechnologies_.push_back( nTech );
	}

	pIter = nProject->soundTechnologies_.begin();
	soundTechnologies_.clear();
	for( ; pIter != nProject->soundTechnologies_.end(); ++pIter )
	{
		CNrpTechnology* nTech = new CNrpTechnology( *(*pIter) );
		nTech->SetOption<PNrpGameProject>( PARENT, this );
		nTech->SetOption<PUser>( COMPONENTLIDER, NULL ); 
		soundTechnologies_.push_back( nTech );
	}
}

CNrpGameProject::CNrpGameProject( CNrpGameProject& ptr ) : INrpProject( "CNrpGameProject", ptr.GetOption<std::string>( NAME ) )
{

}

CNrpGameProject::~CNrpGameProject(void)
{
}

void CNrpGameProject::SetGameEngine( CNrpGameEngine* gameEng )
{
	SetOption<PNrpGameEngine>( GAME_ENGINE, (PNrpGameEngine)gameEng );

	int modNumber = gameEng ? gameEng->GetOption<int>( GENRE_MODULE_NUMBER ) : 0;
	SetOption<int>( GENRE_MODULE_NUMBER, modNumber );

	int codeVol = gameEng ? gameEng->GetOption<int>( CODEVOLUME ) : 0;
	SetOption<int>( BASE_CODEVOLUME, codeVol );
	
	int quality = gameEng ? gameEng->GetOption<int>( QUALITY ) : 0;
	SetOption<int>( BASEQUALITY, quality );

	genres_.resize( modNumber, NULL );
	CalculateCodeVolume();
}

bool CNrpGameProject::IsGenreIncluded( GENRE_TYPE typen )
{
	TECH_LIST::iterator pIter = genres_.begin();

	for( ; pIter != genres_.end(); ++pIter)
		if( (*pIter) && (*pIter)->GetOption<GENRE_TYPE>(TECHTYPE) == typen )
			return true;

	return false;
}

void CNrpGameProject::FindPlaformsAndLanguages_()
{
	PropertyArray::iterator pIter = options_.begin();

	int platformNumber = 0;
	int langNumber = 0;
	for( ; pIter != options_.end(); ++pIter )
	{
		if( (*pIter).first.find( "platform_" ) == 0 )
			platformNumber += GetOption<bool>( (*pIter).first ) ? 1 : 0;

		if( (*pIter).first.find( "language_" ) == 0 )
			langNumber += GetOption<bool>( (*pIter).first ) ? 1 : 0;
	}

	SetOption<int>( LANGNUMBER, langNumber );
	SetOption<int>( PLATFORMNUMBER, platformNumber );
}

void CNrpGameProject::CalculateCodeVolume()
{
	FindPlaformsAndLanguages_();
	float baseCode = (float)GetOption<int>( BASE_CODEVOLUME );

	float summ = 0;
	float engCoeff = 1;
	int quality = 0;

	TECH_LIST rt = genres_;
	rt.insert( rt.end(), technologies_.begin(), technologies_.end() );
	rt.insert( rt.end(), videoTechnologies_.begin(), videoTechnologies_.end() );
	rt.insert( rt.end(), soundTechnologies_.begin(), soundTechnologies_.end() );
	rt.push_back( GetOption<PNrpTechnology>( SCRIPTENGINE ) );
	rt.push_back( GetOption<PNrpTechnology>( MINIGAMEENGINE ) );
	rt.push_back( GetOption<PNrpTechnology>( PHYSICSENGINE ) );
	rt.push_back( GetOption<PNrpTechnology>( SOUNDQUALITY ) );
	rt.push_back( GetOption<PNrpTechnology>( GRAPHICQUALITY ) );
	rt.push_back( GetOption<PNrpTechnology>( SCENARIO ) );

	TECH_LIST::iterator techIter = rt.begin();
	for( ; techIter != rt.end(); ++techIter)
		if( (*techIter) != NULL )
		{
			engCoeff += (*techIter)->GetOption<float>( ENGINE_CODE );
			quality += (*techIter)->GetOption<int>( QUALITY );
			quality /= 2;
		}

	baseCode *= engCoeff;
	SetOption<int>( ENGINE_CODEVOLUME, (int)baseCode );

	techIter = rt.begin();
	for( ; techIter != rt.end(); ++techIter)
		if( (*techIter) != NULL ) summ += (int)((*techIter)->GetOption<float>( BASE_CODE ) * baseCode );

	summ += baseCode;
	for( int cnt=0; cnt < GetOption<int>( LANGNUMBER ); cnt++ )
		summ += ( summ * cnt * 0.1f );

	for( int cnt=0; cnt < GetOption<int>( PLATFORMNUMBER ); cnt++ )
		summ += ( summ * cnt * 0.2f );
	
	bool projectReady = GetOption<PNrpGameEngine>( GAME_ENGINE ) != NULL;
	projectReady &= ( GetOption<PNrpScenario>( SCENARIO ) != NULL || GetOption<PNrpLicense>( GLICENSE ) != NULL );
	projectReady &= ( GetOption<int>( LANGNUMBER ) > 0 && GetOption<int>( PLATFORMNUMBER ) > 0 );
	projectReady &= ( GetGenre( 0 ) != NULL );
	
	SetOption<int>( CODEVOLUME, (int)summ );
	SetOption<int>( QUALITY, quality ); 
	SetOption<bool>( PROJECTREADY, projectReady );
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
		GENRE_TYPE techType = genre->GetOption<GENRE_TYPE>( TECHTYPE );

		if( GetOption<PNrpGameEngine>( GAME_ENGINE )->IsGenreAvailble( techType ) &&
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
	rt.push_back( GetOption<PNrpTechnology>(MINIGAMEENGINE));
	rt.push_back( GetOption<PNrpTechnology>(SCRIPTENGINE));
	rt.push_back( GetOption<PNrpTechnology>(PHYSICSENGINE));
	rt.push_back( GetOption<PNrpTechnology>(GRAPHICQUALITY));

	rt.insert(rt.end(), videoTechnologies_.begin(), videoTechnologies_.end() );
	rt.insert(rt.end(), technologies_.begin(), technologies_.end() );
	rt.insert(rt.end(), soundTechnologies_.begin(), soundTechnologies_.end() );
	TECH_LIST::iterator pIter = rt.begin();

	for( ; pIter != rt.end(); ++pIter)
		if( (*pIter) && (*pIter)->GetOption<ADV_TECH_TYPE>(TECHTYPE) == typen )
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
	SetOption<int>( optname, listd.size() );
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

}//namespace nrp