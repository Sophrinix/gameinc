#include "StdAfx.h"
#include "NrpGame.h"
#include "NrpCompany.h"
#include "NrpGameProject.h"
#include "NrpTechnology.h"
#include "NrpGameBox.h"
#include "NrpExtInfo.h"
#include "NrpApplication.h"
#include "NrpDevelopGame.h"
#include "NrpProjectModule.h"
#include "IUser.h"
#include "OpFileSystem.h"
#include "NrpHistory.h"
#include "NrpGameEngine.h"
#include "IniFile.h"
#include "NrpTime.h"
#include "NrpLaborMarket.h"
#include "NrpScreenshotHolder.h"
#include "NrpGameMarket.h"

#include <errno.h>
#include <OleAuto.h>

namespace nrp
{
CLASS_NAME CLASS_NRPGAME( "CNrpGame" );

const NrpText CNrpGame::saveTemplate = L"item.game";
const NrpText CNrpGame::historyTemplate = L"game.history";
const NrpText CNrpGame::extension = L".game";
	
void CNrpGame::_InitializeOptions()
{
	RegProperty<PNrpCompany>( PARENTCOMPANY, NULL );
	RegProperty<NrpText>( CLASSOBJECT, CLASS_NRPGAME );
	RegProperty<NrpText>( COMPANYNAME, "" );
	RegProperty<NrpText>( NAME, "" );
	RegProperty<NrpText>( INTERNAL_NAME, "" );
	RegProperty<NrpTime>( STARTDATE, NrpTime( 0. ) );
	RegProperty<NrpTime>( ENDDATE, NrpTime( 0. ) );
	RegProperty( MONEYONDEVELOP, 0 );
	RegProperty( CASH, 0 );
	RegProperty<NrpText>( OWNER, "" );
	RegProperty( COPYSELL, 0 );
	RegProperty( STARTRATING, 0 );
	RegProperty( STARTGAMERATING, 0 );
	RegProperty( STARTGRAPHICRATING, 0 );
	RegProperty( STARTGENRERATING, 0 );
	RegProperty( STARTSOUNDRATING, 0 );
	RegProperty( STARTADVFUNCRATING, 0 );
	RegProperty( CURRENTADVFUNCRATING, 0 );
	RegProperty( STARTBUGRATING, 0 );
	RegProperty( CURRENTGAMERATING, 0 );
	RegProperty( CURRENTGRAPHICRATING, 0 );
	RegProperty( CURRENTGENRERATING, 0 );
	RegProperty( CURRENTSOUNDRATING, 0 );
	RegProperty( LOCALIZATIONRATING, 0 );
	RegProperty( CURRENTBUGRATING, 0 );	
    RegProperty( OLDGAME, false );
	RegProperty<NrpText>( GAME_ENGINE, "" );
	RegProperty<NrpText>( PREV_GAME, "" );
	RegProperty( GENRE_MODULE_NUMBER, 0 );
	RegProperty<PNrpGameBox>( GBOX, NULL );
	RegProperty( FAMOUS, 0.f );
	RegProperty( GAMEISSALING, false );
	RegProperty<CNrpExtInfo*>( EXTINFO, NULL );
	RegProperty<NrpText>( EXTINFOLINK, "" );
	RegProperty<NrpText>( GAMERETAILER, "" );
	RegProperty( PLATFORMNUMBER, 0 );
	RegProperty( MODULE_NUMBER, 0 );
	RegProperty( USERNUMBER, 0 );
	RegProperty<NrpText>( DESCRIPTIONPATH, "" );
	RegProperty<NrpText>( TEXTURENORMAL, "" );
	RegProperty( READYWORKPERCENT, 1.f );
	RegProperty<NrpText>( VIEWIMAGE, "" );
	RegProperty<NrpText>( RECENSE, "" );
	RegProperty( NPC_GAME, false );
	RegProperty( LOADOK, false );
	RegProperty( BESTSALER, false );
    RegProperty( URL, NrpText() );
	_history = NULL;
}

CNrpGame::CNrpGame( const CNrpDevelopGame& devGame, CNrpCompany* ptrCompany ) : INrpConfig( CLASS_NRPGAME, (NrpText)devGame[ NAME ] )
{
	_InitializeOptions(); 

	_self[ PARENTCOMPANY ] = ptrCompany;
	_self[ COMPANYNAME ] = (*ptrCompany)[ NAME ];
	_self[ NAME ] = devGame[ NAME ];
	_self[ INTERNAL_NAME ] = devGame[ INTERNAL_NAME ];
	_self[ MONEYONDEVELOP ] = devGame[ MONEYONDEVELOP ];
	CNrpGameEngine* ge = devGame[ GAME_ENGINE ].As<CNrpGameEngine*>();
	if( ge )
		_self[ GAME_ENGINE_NAME ] = (*ge)[ NAME ];

	_self[ FAMOUS ] = devGame[ FAMOUS ];
	assert( devGame[ PLATFORMNUMBER ] != (int)0 );
	_self[ PLATFORMNUMBER ] = devGame[ PLATFORMNUMBER ];
	_self[ USERNUMBER ] = devGame[ USERNUMBER ];
	//_self[ PREV_GAME ] = devGame[ NAME ]; 
	
	AddArrayTo( _developers, const_cast< CNrpDevelopGame& >( devGame ).GetDevelopers() );

	_self[ GENRE_MODULE_NUMBER ] = devGame[ GENRE_MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)_self[ GENRE_MODULE_NUMBER ]; cnt++ )
	{
		 CNrpTechnology* genre = devGame.GetGenre( cnt );
		 if( genre )
			_genres.push_back( (*genre)[ INTERNAL_NAME ] );
	}

	_self[ MODULE_NUMBER ] = devGame[ MODULE_NUMBER ];
	for( int cnt=0; cnt < (int)_self[ MODULE_NUMBER ]; cnt++ )
		 _techs.push_back( (*devGame.GetModule( cnt ))[ INTERNAL_NAME ] );

	_self[ EXTINFOLINK ] = _nrpScreenshots.GetFreeInternalName( *this );
	CNrpExtInfo* extInfo = _SearchExtInfo();
	assert( extInfo != NULL );
	if( extInfo != NULL )
	{
		_self[ EXTINFO ] = extInfo;
		_self[ VIEWIMAGE ] = extInfo->GetBoxImages()[ 0 ];
	}

	_history = new CNrpHistory();
}

CNrpGame::CNrpGame( const NrpText& fileName ) : INrpConfig( CLASS_NRPGAME, fileName )
{
	_InitializeOptions();
	Load( fileName );
}

CNrpGame::CNrpGame() : INrpConfig( CLASS_NRPGAME, "" )
{

}

CNrpGame::~CNrpGame(void)
{
}

NrpText CNrpGame::Save( const NrpText& saveFolder )
{
	bool upDirExist = OpFileSystem::IsExist( saveFolder );
	assert( upDirExist );

	if( !upDirExist )
		return "";
	
	NrpText localFolder = OpFileSystem::CheckEndSlash( saveFolder ) + Text( INTERNAL_NAME ); //директория куда сохраняем
	localFolder = OpFileSystem::CheckEndSlash( localFolder ); //добавляем слеш в конец
    NrpText saveFile = localFolder + CNrpGame::saveTemplate;

	OpFileSystem::CreateDirectory( localFolder );
	INrpConfig::Save( saveFile );

	if( PNrpGameBox box = _self[ GBOX ].As<PNrpGameBox>() )
	{
        box->Save( localFolder + CNrpGameBox::saveTemplate );
	}

	IniFile sv( saveFile );
	sv.Set( SECTION_TECHS, CreateKeyTech, _techs );
	sv.Set( SECTION_GENRES, CreateKeyGenre, _genres);
	sv.Set( SECTION_PLATFORMS, CreateKeyPlatform, _platforms );

	sv.Save();

    assert( _history );
    if( _history )
        _history->Save( localFolder + CNrpGame::historyTemplate );

	return localFolder;
}

CNrpExtInfo* CNrpGame::_SearchExtInfo()
{
	CNrpExtInfo* info = _nrpScreenshots.GetExtInfo( _self[ INTERNAL_NAME ] );

	if( !info )
		info = _nrpScreenshots.GetExtInfo( _self[ NAME ] );

	if( !info )
		info = _nrpScreenshots.GetExtInfo( _self[ EXTINFOLINK ] );

	return info;
}

void CNrpGame::Load( const NrpText& loadPath )
{
	_self[ LOADOK ] = OpFileSystem::IsExist( loadPath );
	assert( (bool)_self[ LOADOK ] && "can't load game config file" );

	if( !(bool)_self[ LOADOK ] )
		return;

	NrpText loadFolder, loadFile ;
	if( OpFileSystem::IsFolder( loadPath ) )
	{
		loadFolder = loadPath; 
		//стандартное имя
        loadFile = OpFileSystem::CheckEndSlash( loadFolder ) + CNrpGame::saveTemplate;
	}
	else 
	{
        assert( OpFileSystem::GetExtension( loadPath ).equals_ignore_case( CNrpGame::extension ) );
        if( OpFileSystem::GetExtension( loadPath ).equals_ignore_case( CNrpGame::extension ) )
		{
			loadFile = loadPath;
			loadFolder = OpFileSystem::UpDir( loadPath );
		}
		else
			return;
	}

	INrpConfig::Load( loadFile );
	IniFile rv( loadFile );

	rv.Get( SECTION_TECHS, CreateKeyTech, -1, _techs );
	_self[ MODULE_NUMBER ] = static_cast< int >( _techs.size() );

	rv.Get( SECTION_GENRES, CreateKeyGenre, -1,	_genres );
	_self[ GENRE_MODULE_NUMBER ] = static_cast< int >( _genres.size() );

	rv.Get( SECTION_PLATFORMS, CreateKeyPlatform, -1, _platforms );
	_self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );

    NrpText boxIni = loadFolder + CNrpGameBox::saveTemplate;
	if( OpFileSystem::IsExist( boxIni ) )
	{
		PNrpGameBox box = new CNrpGameBox( this );
		box->Load( boxIni );
		_self[ GBOX ] = box;
	}

    int cs = _self[ COPYSELL ];
    NrpText historyIni = loadFolder + CNrpGame::historyTemplate;
	_history = new CNrpHistory();
    if( OpFileSystem::IsExist( historyIni ) )
        _history->Load( historyIni );
	else
        Log( HW ) << "Can't find history for game " << (NrpText)_self[ NAME ] << term;

	CNrpExtInfo* pgList = _SearchExtInfo();
	if( !pgList )
		Log( HW ) << "Can't find extInfo for " << (NrpText)_self[ INTERNAL_NAME ] << " when load from " << loadFile << term;

	assert( pgList != NULL );
	if( pgList != NULL )
	{
		_self[ EXTINFO ] = pgList;

		assert( pgList->GetBoxImages().size() > 0 );
		//если у игры не изображения обложки
		if( ((NrpText)_self[ VIEWIMAGE ]).size() > 0 && pgList->GetBoxImages().size() > 0 )	
			_self[ VIEWIMAGE ] = pgList->GetBoxImages()[ 0 ];
	}

	//конфиг файл был нормально загружен
	_self[ LOADOK ] = true;
}

float CNrpGame::GetAuthorFamous()
{
	float summ = 0.1f;
	for( u32 i=0; i < _developers.size(); i++ )
	{
		CNrpUser* user = _nrpLaborMarkt.GetUser( _developers[ i ] );
		if( user )
		{
			summ += (*user)[ FAMOUS ].As<float>();
			summ /= 2.f;
		}
	}
	return summ;
}

NrpText CNrpGame::GetGenreName( size_t index )
{
	return index < _genres.size() ? _genres[ index ] : "";
}

NrpText CNrpGame::GetTechName( size_t index )
{
    return index < _techs.size() ? _techs[ index ] : "";
}

void CNrpGame::_AddSalesToCompany( int number, int profit, NrpTime& curTime )
{
    PNrpCompany cmp = _self[ PARENTCOMPANY ].As<PNrpCompany>();
    if( cmp )
    {
        (*cmp)[ BALANCE ] += profit;
        if( CNrpHistory* hs = (*cmp)[ HISTORY ].As<CNrpHistory*>() )
        {
            CNrpHistoryStep* step = hs->AddStep( curTime );
            assert( step && "history step must be exist" );
            step->AddValue( BOXNUMBER, number );
            step->AddValue( BALANCE, profit );
        }
    }
}

void CNrpGame::AddSales( int number, NrpTime& curTime, int defPrice )
{
	PNrpGameBox gameBox = _self[ GBOX ].As<PNrpGameBox>();
    int price = defPrice;
    int profit = 0;
    
    if( !_self[ NPC_GAME ] )
    {
	    Log(HW) << "gameBox == NULL " << Text( INTERNAL_NAME ) << term;
    }

	if( gameBox != NULL )
	{
		CNrpGameBox& refBox = *gameBox;
		int boxNumber = refBox[ BOXNUMBER ];
        price = refBox[ PRICE ];
        number = (std::min)( number, boxNumber );
		
		refBox[ BOXNUMBER ] -= number;
	}
    else
    {
        if( (bool)_self[ NPC_GAME ] && _history )
            if( CNrpHistoryStep* step = _history->GetLast() )
                price = CNrpGameMarket::CalcAmpChange( (*step)[ PRICE ], 10, 10 ); 
    }

    assert( price > 0 );
    profit = price * number;

    assert( _history );
    if( _history )
    {
        if( CNrpHistoryStep* step = _history->AddStep( curTime ) )
        {
            step->AddValue( BOXNUMBER, number );
            step->AddValue( BALANCE, profit );
            int lp = step->IsExist( PRICE ) ? (*step)[ PRICE ] : price;
            step->AddValue<int>( PRICE, (price + lp) / 2 );
        }
    }
  
    _self[ CASH ] += profit;
    _self[ COPYSELL ] += number;	

    _AddSalesToCompany( number, profit, curTime );
}

bool CNrpGame::IsGenreAvaible( const NrpText& name ) const
{
	for( size_t k=0; k < _genres.size(); k++ )
		if( _genres[ k ] == name )
			return true;

	return false;
}

CNrpHistory* CNrpGame::GetHistory()
{
	assert( _history );
	return _history;
}

NrpText CNrpGame::ClassName()
{
	return CLASS_NRPGAME;
}

NrpText CNrpGame::GetPlatformName( size_t index )
{
    return index < _platforms.size() ? _platforms[ index ] : "";
}
}//namespace nrp