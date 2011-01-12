#include "StdAfx.h"
#include "NrpScreenshot.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "nrpConfig.h"
#include "NrpGame.h"

#define SCREENSHOTNAME( index ) "scr_" + NrpText( index ) + ".png"
#define SPLASHSHOTNAME( index ) "spl_" + NrpText( index ) + ".png"

namespace nrp
{
CLASS_NAME CLASS_IMAGEGAMELIST( "CNrpGameImageList" );

CNrpScreenshot::CNrpScreenshot(void) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
}

CNrpScreenshot::CNrpScreenshot( const CNrpScreenshot& a ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
	_self[ INTERNAL_NAME ] = a[ INTERNAL_NAME ];

	_imagesPath = a._imagesPath;
	_imagesBoxPath = a._imagesBoxPath;
	_self[ IMAGESNUMBER ] = static_cast< int >( _imagesPath.size() );
	_self[ IMAGESBOXNUMBER ] = static_cast< int >( _imagesBoxPath.size() );
}

CNrpScreenshot::CNrpScreenshot( const NrpText& fileName ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
	Load( fileName );
}

CNrpScreenshot::~CNrpScreenshot(void)
{
}

bool CNrpScreenshot::IsMyYear( int year )
{
	return _self[ STARTDATE ].As<SYSTEMTIME>().wYear <= year;
}

int CNrpScreenshot::GetEqualeRating( CNrpGame* game )
{
	int equale = 0;
	for( u32 i=0; i < _genres.size(); i++ )
	{
		equale += game->IsGenreAvaible( _genres[ i ] ) ? 1 : 0;
	}

	return ( equale * 100 / _genres.size() );
}

void CNrpScreenshot::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
	IniFile rv( fileName );

	assert( ((NrpText)_self[ INTERNAL_NAME ]).size() );

	NrpText folder = OpFileSystem::UpDir( fileName );
	//найдем все скриншоты игры
	int k = 0;
	NrpText imageName = folder + SCREENSHOTNAME( k );
	while( OpFileSystem::IsExist( imageName ) )
	{
		_imagesPath.push_back( imageName );
		k++;
		imageName = folder + SCREENSHOTNAME( k );
	}
	_self[ IMAGESNUMBER ] = static_cast< int >( _imagesPath.size() );
	//найдем все скриншоты заставок
	k = 0;
	imageName = folder + SPLASHSHOTNAME( k );
	while( OpFileSystem::IsExist( imageName ) )
	{
		_imagesBoxPath.push_back( imageName );
		k++;
		imageName = folder + SCREENSHOTNAME( k );
	}
	_self[ IMAGESBOXNUMBER ] = static_cast< int >( _imagesBoxPath.size() );


	rv.Get( SECTION_GENRES, CreateKeyGenre,(int)_self[ GENRE_MODULE_NUMBER ], _genres );
}

void CNrpScreenshot::InitializeOptions_()
{
	Add<NrpText>( INTERNAL_NAME, "" );
	Add<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Add<int>( IMAGESNUMBER, 0 );
	Add<int>( IMAGESBOXNUMBER, 0 );
	Add<NrpText>( NAME, "" );
	Add<int>( GENRE_MODULE_NUMBER, 0 );
}

NrpText CNrpScreenshot::ClassName()
{
	return CLASS_IMAGEGAMELIST;
}

}//end namespace nrp