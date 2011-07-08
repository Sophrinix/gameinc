#include "StdAfx.h"
#include "NrpGameMarket.h"
#include "NrpCompany.h"
#include "NrpGame.h"
#include "NrpGameBox.h"
#include "NrpRetailer.h"
#include "NrpPlatform.h"
#include "NrpTechnology.h"
#include "NrpApplication.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "NrpGameEngine.h"
#include "NrpHistory.h"
#include "NrpLinkHolder.h"

namespace nrp
{

CLASS_NAME CLASS_NRPGAMEMARKET( "CNrpGameMarket" );

CNrpGameMarket* globalGameMarket = NULL;

const NrpText CNrpGameMarket::saveTemplate = L"gameMarket.ini";

CNrpGameMarket::CNrpGameMarket(void) : INrpConfig( CLASS_NRPGAMEMARKET, CLASS_NRPGAMEMARKET )
{
    RegProperty( GAMENUMBER, (int)0 );
    RegProperty( ENGINES_NUMBER, (int)0 );
    RegProperty( PLATFORMNUMBER, (int)0 );
}

CNrpGameMarket::~CNrpGameMarket(void)
{
}

float GetConsumerAbility( int price )
{
    if( price > 150 ) return 10.f / price;

    if( price > 100 && price <= 150 ) return 20.f / price;

    if( price > 50 && price <= 100) return 40.f / price;

    if( price > 25 && price <= 50 ) return 50.f / price;

    if( price > 10 && price <= 25) return 25.f / price;
    else return 30.f / price;
}

int CNrpGameMarket::GetPlatformSales( CNrpGame& game, NrpTime& endTime )
{
    int summ=0;
    for( int i=0; i < (int)game[ PLATFORMNUMBER ]; i++ )
    {
        NrpText plName = game.GetPlatformName( i );
        CNrpPlatform* pl = GetPlatform( plName );

        CNrpHistory* hs = pl ? (*pl)[ HISTORY ].As<CNrpHistory*>() : NULL;

        summ += hs ? hs->GetSummFor( COPYSELL, endTime ) : 0;

        if( !pl )
            Log( HW ) << "Can't find platform " << plName << " for game " << (NrpText)game[ NAME ] << term;
    }

    return summ;
}

int CNrpGameMarket::_GetSalesNumber( CNrpGame& game )
{
    CNrpCompany* cmp = game[ PARENTCOMPANY ].As<CNrpCompany*>();

    assert( cmp );
    if( !cmp )
        Log( HW ) << "Can't find company with name " << (NrpText)game[ COMPANYNAME ] << " for game " << (NrpText)game[ NAME ] << term;

    //получим количество платформ на которых может быть продана игра
    int freePlatformNumber = GetPlatformSales( game, _nrpApp[ CURRENTTIME ].As<NrpTime>() );

    //найдем количество игр этого жанра
    float gamesInThisGenre = 1;
    for( u32 i=0; i < _games.size(); i++ )
    {
        CNrpGame* tmpGame = _games[ i ];
        if( (&game != tmpGame) && 
            (bool)game[ GAMEISSALING ] &&
            (tmpGame->GetGenreName( 0 ) == game.GetGenreName( 0 )) )
            gamesInThisGenre += (int)game[ CURRENTGAMERATING ] / 100.f; 
    }

    freePlatformNumber -= (int)game[ COPYSELL ];
    float userModificator = 1, compannyFamous = 1;
    if( cmp )
    {
        userModificator = cmp->GetUserModificatorForGame( game );
        compannyFamous = (*cmp)[ FAMOUS ]; 
    }

    float authorFamous = game.GetAuthorFamous();
    NrpText retailerName = game[ GAMERETAILER ];
    PNrpRetailer retailer = GetRetailer( retailerName );

    float retailerFamous = 0.1f;
    if( retailer )
        retailerFamous = (*retailer)[ FAMOUS ];

    float genreInterest = GetGameGenreInterest( game );

    //столько игр может быть продано сегодня
    freePlatformNumber /= 365;
    //вероятность что покупатель обратит внимание на нашу игру после прочтения обзора в прессе
    int gameMaySaledToday = (int)((freePlatformNumber*genreInterest) / gamesInThisGenre);

    //повышение продаж игры за счет рекламы игры, известности авторов и личностных модификаторов
    gameMaySaledToday = static_cast< int >(gameMaySaledToday * ( (float)game[ FAMOUS ] + userModificator + authorFamous ));

    //коэффициент продаж по известности ретейлера и компании
    gameMaySaledToday = static_cast< int >( gameMaySaledToday * (compannyFamous + retailerFamous) * 0.5f );

    //коэффициент покупательской способности
    if( PNrpGameBox box = game[ GBOX ].As<PNrpGameBox>() )
    {
        gameMaySaledToday = static_cast< int >( gameMaySaledToday * GetConsumerAbility( (*box)[ PRICE ] ) );
    }

    return gameMaySaledToday;
}

void CNrpGameMarket::_UpdateMarketGames()
{
    for( u32 i=0; i < _games.size(); i++ )
    {
        CNrpGame& rGame = *_games[ i ];
        if( !(bool)rGame[ GAMEISSALING ] || (bool)rGame[ OLDGAME ] )
            continue;

         
        rGame.AddSales( _GetSalesNumber( rGame ), _nrpApp[ CURRENTTIME ].As<NrpTime>() );
    }
}

void CNrpGameMarket::BeginNewDay()
{
    _UpdateMarketGames();
}

CNrpGameEngine* CNrpGameMarket::GetGameEngine( const NrpText& name ) 
{
    return FindByNameAndIntName<ENGINES, CNrpGameEngine>( _engines, name );
}

CNrpPlatform* CNrpGameMarket::GetPlatform( const NrpText& name )
{
    return FindByNameAndIntName< PLATFORMS, CNrpPlatform >( _platforms, name );
}

CNrpPlatform* CNrpGameMarket::GetPlatform( size_t index )
{
    assert( index < _platforms.size() );
    return index < _platforms.size() ? _platforms[ index ] : NULL;
}

bool CNrpGameMarket::AddPlatform( CNrpPlatform* platform )
{
    if( GetPlatform( (NrpText)(*platform)[ INTERNAL_NAME ] ) == NULL )
    {
        _platforms.push_back( platform );
        _self[ PLATFORMNUMBER ] = static_cast< int >( _platforms.size() );
        return true;
    }

    return false;
}

void CNrpGameMarket::AddGame( CNrpGame& ptrGame )
{
    CNrpGame* hG = GetGame( (NrpText)ptrGame[ INTERNAL_NAME ] );

    if( !hG ) 
    {
        _games.push_back( &ptrGame );
        _self[ GAMENUMBER ] = static_cast< int >( _games.size() );
    }
    else
        Log( HW ) << "Also have game " << (NrpText)ptrGame[ INTERNAL_NAME ] << " on market" << term;
}

//интерес к жанру меняется в противоположную сторону на 10% от рейтинга игры
float CNrpGameMarket::GetGameGenreInterest( CNrpGame& game )
{
    float summ = 0;

    CNrpTechnology* tech = _nrpApp.GetTechnology( game.GetGenreName( 0 ) );
    assert( tech );
    if( tech )
    {
        summ = (*tech)[ INTEREST ];
        int gm = game[ GENRE_MODULE_NUMBER ];
        for( int i=1; i < gm; i++ )
        {
            tech = _nrpApp.GetTechnology( game.GetGenreName( i ) );	
            assert( tech );
            if( tech )
                summ += (*tech)[ INTEREST ].As<float>() / i;
        }
    }
    //надо скомпенсировать понижение интереса к жанру, которое принесла
    //игра при размещении на рынке
    return (summ + (int)game[CURRENTGAMERATING] / 1000.f );
}

void CNrpGameMarket::Load( const NrpText& folderName )
{
    NrpText profileIni = OpFileSystem::CheckEndSlash( folderName ) + CNrpGameMarket::saveTemplate;

    IniFile rv( profileIni );

    for( int i=0; i < (int)_self[ PLATFORMNUMBER ]; i++ )
    {
        NrpText fileName = rv.Get( SECTION_PLATFORMS, CreateKeyPlatform(i), NrpText("") );
        CNrpPlatform* plt = new CNrpPlatform( fileName ); //loading
        _platforms.push_back( plt );
    }

    for( int i=0; i < (int)_self[ ENGINES_NUMBER ]; i++ )
    {
        NrpText saveFolder = rv.Get( SECTION_ENGINES, CreateKeyEngine(i), NrpText("") );
        _engines.push_back( new CNrpGameEngine( saveFolder, true ) );
    }

    for( int i=0; i < (int)_self[ GAMENUMBER ]; i++ )
    {
        NrpText fileName = rv.Get( SECTION_GAMES,  CreateKeyGame( i ), NrpText("") );
        if( !OpFileSystem::IsExist( fileName ) )
            continue;

        PNrpGame game = new CNrpGame( fileName );
        if( (bool)(*game)[ LOADOK ] )
            _games.push_back( game );
        else
            Log( HW ) << "!!!WARNING!!! Can't load game from " << fileName << term;
    }
    _self[ GAMENUMBER ] = static_cast< int >( _games.size() );
}

void CNrpGameMarket::_UpdateGameRating( CNrpGame& game )
{
    if( !(bool)game[ GAMEISSALING ] )
        return;

    std::map< int, int > qualityMap;

    //вычисляем сколько месяцев на рынке игра понижаем рейтинг из-за времени на рынке
    int monthInMarket = game[ STARTDATE ].As<NrpTime>().GetMonthToDate( _self[ CURRENTTIME ].As<NrpTime>() ) + 1;
    int bs = (bool)game[ BESTSALER ] ? 50 : 0;

    if( (bool)game[ NPC_GAME ] )
    {
        //игры которые не выпущены игроком, расчитывают рейтинг простым снижением
        qualityMap[ PT_VIDEOTECH ] = (int)game[ STARTGRAPHICRATING ] / monthInMarket;
        qualityMap[ PT_SOUNDTECH ] = (int)game[ STARTSOUNDRATING ] / monthInMarket;
        qualityMap[ PT_GENRE ] = (int)game[ CURRENTGENRERATING ] / monthInMarket;
    }
    else
    {
        //подсчитаем рейтинги модулей для игр, которые были выпущены игроком
        for( int k=0; k < (int)game[ MODULE_NUMBER ]; k++ )
        {
            NrpText name = game.GetTechName( k );
            if( CNrpTechnology* tech = _nrpApp.GetTechnology( name ) )
            {
                int tg = (*tech)[ TECHGROUP ];
                int rt = (int)(*tech)[ QUALITY ] / monthInMarket;

                //распределим технологии по группам
                if( qualityMap[ tg ] == 0 )
                    qualityMap[ tg ] = rt;
                else
                    qualityMap[ tg ] = (qualityMap[ tg ] + rt) / 2;		
            }
        }
    }

    //результат подсчета рейтинга
    if( CNrpHistoryStep* step = game.GetHistory()->AddStep( _self[ CURRENTTIME ].As<NrpTime>() ) )
    {
        //занесем рейтинг в историю игры
        //если игрушка хитовая, то её рейтинг не опускается ниже 50%
        //todo: Слежение за хитовостью игры ограничением рейтинга можно перенести в саму игру
        game[ CURRENTGRAPHICRATING ] = (std::max)( bs, qualityMap[ PT_VIDEOTECH ] );
        step->AddValue( CURRENTGRAPHICRATING, (int)game[ CURRENTGRAPHICRATING ] );

        game[ CURRENTSOUNDRATING ] = (std::max)( bs, qualityMap[ PT_SOUNDTECH ] );
        step->AddValue( CURRENTSOUNDRATING, (int)game[ CURRENTSOUNDRATING ] );

        game[ CURRENTGENRERATING ] = (std::max)( bs, qualityMap[ PT_GENRE ] );
        step->AddValue( CURRENTGENRERATING, (int)game[ CURRENTGENRERATING ] );

        step->AddValue( CURRENTBUGRATING, (int)game[ CURRENTBUGRATING ] );

        int midRating = ( qualityMap[ PT_VIDEOTECH ] + qualityMap[ PT_SOUNDTECH ] + qualityMap[ PT_GENRE ] ) / 3;
        step->AddValue( CURRENTGAMERATING, midRating );
    }
}

void CNrpGameMarket::BeginNewMonth()
{
    for( u32 i=0; i < _games.size(); i++ )
        _UpdateGameRating( *_games[ i ] );
}

void CNrpGameMarket::StartSale( CNrpGame& game )
{
    if( (bool)game[ GAMEISSALING ] || (bool)game[ OLDGAME ] )
        return;

    game[ GAMEISSALING ] = true;

    //если игра не скриптовая, то надо расчитать и запомнить начальные рейтинги
    if( !(bool)game[ NPC_GAME ] )
    {
        _UpdateGameRating( game );
        game[ STARTGAMERATING ] = game[ CURRENTGAMERATING ];
        game[ STARTGRAPHICRATING ] = game[ CURRENTGRAPHICRATING ];
        game[ STARTSOUNDRATING ] = game[ CURRENTSOUNDRATING ];
        game[ STARTGENRERATING ] = game[ CURRENTGENRERATING ];
    }
    else
    {//когда игра загружается по ходу процесса, то её начальные рейтинги становятся текущими
        //до первого обновления рейтингов
        game[ CURRENTGAMERATING ] = game[ STARTGAMERATING ];
        game[ CURRENTGRAPHICRATING ] = game[ STARTGRAPHICRATING ];
        game[ CURRENTSOUNDRATING ] = game[ STARTSOUNDRATING ];
        game[ CURRENTGENRERATING ] = game[ STARTGENRERATING ];
    }

    //когда игра выходит на рынок, то она влияет на него
    for( int i=0; i < (int)game[ GENRE_MODULE_NUMBER ]; i++ )
    {
        NrpText genreName = game.GetGenreName( i );
        //влияние приводит к изменению интереса к жанру игры
        CNrpTechnology* tech = _nrpApp.GetTechnology( genreName );
        //такой жанр уже есть на рынке
        if( tech != NULL )
        {
            //значение каждого последующего жанра в игре оказывает все меньшее влияние
            float koeff = (int)game[ STARTRATING ] / (1000.f + i*300.f) * (float)(*tech)[ INTEREST ];
            (*tech)[ INTEREST ] -= koeff;
            Log(HW) << "techName " << (NrpText)(*tech)[ NAME ] << ": Interest " << (float)(*tech)[ INTEREST ] << term;
        }
        else //игры такого жанра на рынке нет, значит надо добавить интереса к игре
        {
            NrpText fileName = _nrpLinks.Get( genreName );
            assert( fileName.size() > 0 );
            //и вывести технологию на рынок
            if( fileName.size() > 0 )
            {
                CNrpTechnology* newGenre = new CNrpTechnology( fileName );
                _nrpApp.AddTechnology( newGenre );
            }
        }
    }

    AddGame( game );
    _nrpApp.PCall( APP_NEWGAME_ONMARKET, this, &game );
}

CNrpRetailer* CNrpGameMarket::GetRetailer( const NrpText& name )
{
    return NULL;
}

CNrpGame* CNrpGameMarket::GetGame( const NrpText& name )
{
    return FindByNameAndIntName< GAMES, CNrpGame >( _games, name );
}

CNrpGame* CNrpGameMarket::GetGame( u32 index )
{
    assert( index < _games.size() );
    return index < _games.size() ? _games[ index ] : NULL;
}

void CNrpGameMarket::AddGameEngine( nrp::CNrpGameEngine* ptrEngine )
{
    CNrpGameEngine* engine = GetGameEngine( (*ptrEngine)[ INTERNAL_NAME ] );
    if( engine == NULL )
    {
        _engines.push_back( ptrEngine );
        _self[ ENGINES_NUMBER ] = static_cast<int>( _engines.size() );
        return;
    }

    Log( HW ) << "такой движок уже есть" << term;
}

NrpText CNrpGameMarket::ClassName()
{
    return CLASS_NRPGAMEMARKET;
}

NrpText CNrpGameMarket::Save( const NrpText& folderName )
{
    NrpText saveFile = OpFileSystem::CheckEndSlash( folderName ) + CNrpGameMarket::saveTemplate;
    INrpConfig::Save( saveFile );

    IniFile sv( saveFile );

    for( u32 i=0; i < _engines.size(); i++ )
    {
        NrpText saveFolder = _engines[ i ]->Save( _nrpApp[ SAVEDIR_ENGINES ] );
        sv.Set( SECTION_ENGINES, CreateKeyEngine(i), saveFolder );
    }

    for( u32 i=0; i < _games.size(); i++ )
    {
        NrpText saveDir = _games[ i ]->Save( _nrpApp[ SAVEDIR_GAMES ] );
        sv.Set( SECTION_GAMES, CreateKeyGame( i ), saveDir );
    }

    for( u32 i=0; i < _platforms.size(); i++ )
    {
        NrpText saveDir = _platforms[ i ]->Save( _nrpApp[ SAVEDIR_PLATFORMS ] );
        sv.Set( SECTION_PLATFORMS, CreateKeyPlatform( i ), saveDir );
    }

    sv.Save();

    return saveFile;
}

CNrpGameMarket& CNrpGameMarket::Instance()
{
    if( !globalGameMarket )
        globalGameMarket = new CNrpGameMarket();

    return *globalGameMarket;
}

int CNrpGameMarket::CalcAmpChange( int curValue, int amp, int minPr )
{
    return (std::max)( (int)(curValue * ( (1.f - amp / 200.f) + ( (rand() % amp) / 100) )), minPr );
}
}