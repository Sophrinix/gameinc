#include "StdAfx.h"
#include "NrpScreenshotHolder.h"
#include "IniFile.h"
#include "OpFileSystem.h"
#include "NrpExtInfo.h"
#include "nrpScript.h"
#include "NrpApplication.h"
#include "NrpGame.h"
#include "NrpGameEngine.h"
#include "NrpGameMarket.h"

namespace nrp
{

static CNrpScreenshotHolder* gloabalScreenshotHolder = NULL;

CNrpScreenshotHolder::CNrpScreenshotHolder(void)
{
}

CNrpScreenshotHolder::~CNrpScreenshotHolder(void)
{
}

void CNrpScreenshotHolder::Load( const NrpText& fileName, const NrpText& funcName )
{
    IniFile rv( fileName );
    size_t imageListNumber = rv.Get( SECTION_OPTIONS, "screenshotNumber", (int)0 );

    if( !OpFileSystem::IsExist( fileName ) )
        return;

    int lastUpdateTime = GetTickCount();
    for( size_t i=0; i < imageListNumber; i++ )
    {
        NrpText uniq = rv.Get( SECTION_OPTIONS, CreateKeyName( i ), NrpText() );
        NrpText scrFile = rv.Get( SECTION_OPTIONS, CreateKeyScreenshot( i ), NrpText("") );

        SCREENSHOTS_MAP::iterator pIter = _screenshots.find( uniq );
        
        if( pIter == _screenshots.end() )
        {
            _screenshots[ uniq ] = EXT_PATH( scrFile, NULL );
        }
        else
        {
            Log( HW ) << "duplicate name screenshot " << uniq << " from " << scrFile << term;
            assert( false && "duplicate name screenshot" );
        }

        if( funcName.size() > 0 && GetTickCount() - lastUpdateTime > 500 )
            CNrpScript::Instance().DoString( funcName+NrpText( "(" )+ NrpText( (int)(100 * i / imageListNumber) ) + NrpText( ")" ) );
    }
}

CNrpExtInfo* CNrpScreenshotHolder::_GetExtInfo( SCREENSHOTS_MAP::iterator& iterr )
{
    if( iterr->second.second != NULL )
        return iterr->second.second;

    iterr->second.second = new CNrpExtInfo( iterr->second.first );
    return iterr->second.second;
}

CNrpExtInfo* CNrpScreenshotHolder::GetExtInfo( const NrpText& name )
{
    SCREENSHOTS_MAP::iterator pIter = _screenshots.find( name );

    if( pIter != _screenshots.end() )
    {
        return _GetExtInfo( pIter );
    }
    else
    {
        Log( HW ) << "can't find extinfo to name " << name << term;
        assert( false && "ext info not found" );
        return NULL;
    }
}

NrpText _ToYearStr( int year )
{
    return NrpText( "/" ) + NrpText( year ) + NrpText( "/" );
}

//получение имени изображени€ с которым будет дальше св€зана игра
NrpText CNrpScreenshotHolder::GetFreeInternalName( const CNrpGame& game )
{
    STRINGS	someEqualeGames;

    int minimumRating = 1;
    //инициализаци€ года выпуска игры
    int year = _nrpApp[ CURRENTTIME ].As<NrpTime>().RYear();

    NrpText checkYearStrMy = _ToYearStr( year ),
            checkYearStrFw = _ToYearStr( year + 1);

    //если у игры есть движок, то возьмем год его выпуска... +-1 год
    if( CNrpGameEngine* ge = CNrpGameMarket::Instance().GetGameEngine( game[ GAME_ENGINE ].As<NrpText>() ) )
        year = (*ge)[ STARTDATE ].As<NrpTime>().RYear();

    for( SCREENSHOTS_MAP::iterator iterr=_screenshots.begin(); iterr != _screenshots.end(); iterr++ )
    {
        if( iterr->second.first.find( checkYearStrFw ) == -1 && 
            iterr->second.first.find( checkYearStrMy ) == -1 )
            continue;
        //получим процент совпадени€ жанров
        CNrpExtInfo* tmpInfo = GetExtInfo( iterr->first );
        assert( tmpInfo );
        if( !tmpInfo )
            continue;

        int eqRating = tmpInfo->GetEqualeRating( game );

        //если слишком разн€тс€ годы выпуска или не совпадает по жанрам
        if( !eqRating || eqRating < minimumRating )
            continue;

        if( minimumRating < eqRating )
        {
            someEqualeGames.clear();
            minimumRating = eqRating;
        }

        someEqualeGames.push_back( iterr->first );
    }

    if( someEqualeGames.size() )
    {
        int randomIndex = rand() % someEqualeGames.size();
        return someEqualeGames[ randomIndex ];
    }

    //!!!!!!!!надо както обработать эту ситуацию!!!!!!!!!!!!!!!!!
    assert( false && "No find free name" );
    return "";
}

CNrpScreenshotHolder& nrp::CNrpScreenshotHolder::Instance()
{
    if( !gloabalScreenshotHolder )
        gloabalScreenshotHolder = new CNrpScreenshotHolder();

    return *gloabalScreenshotHolder;
}

}//end namespace nrp