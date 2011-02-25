#include "StdAfx.h"
#include "NrpScreenshot.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "nrpConfig.h"
#include "NrpGame.h"

#define SCREENSHOTNAME( index, psx ) "scr_" + NrpText( index ) + (psx)
#define SPLASHSHOTNAME( index, psx ) "spl_" + NrpText( index ) + (psx)


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
	return _self[ STARTDATE ].As<NrpTime>().RYear() <= year;
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

bool CNrpScreenshot::_AddImage( STRINGS& art, const NrpText& fileName )
{
	if( OpFileSystem::IsExist( fileName ) )
	{
		art.push_back( fileName );
		return true;
	}

	return false;
}

void CNrpScreenshot::Load( const NrpText& fileName )
{
	INrpConfig::Load( fileName );
	IniFile rv( fileName );

	assert( ((NrpText)_self[ INTERNAL_NAME ]).size() );

	NrpText folder = OpFileSystem::UpDir( fileName );
	//найдем все скриншоты игры
	int k = 0;
	bool mayAdded = true;
	
	while( mayAdded )
	{
		mayAdded = false;

		mayAdded |= _AddImage( _imagesPath, folder + SCREENSHOTNAME( k, ".png" ) );
		mayAdded |= _AddImage( _imagesPath, folder + SCREENSHOTNAME( k, ".jpg" ) );

		k++;
	}
	_self[ IMAGESNUMBER ] = static_cast< int >( _imagesPath.size() );
	//найдем все скриншоты заставок
	k = 0;
	mayAdded = true;
	while( mayAdded )
	{
		mayAdded = false;
		mayAdded |= _AddImage( _imagesBoxPath, folder + SPLASHSHOTNAME( k, ".png" ) );
		mayAdded |= _AddImage( _imagesBoxPath, folder + SPLASHSHOTNAME( k, ".jpg" ) );

		k++;
	}
	_self[ IMAGESBOXNUMBER ] = static_cast< int >( _imagesBoxPath.size() );
	_self[ NAME ] = _self[ INTERNAL_NAME ];

	rv.Get( SECTION_GENRES, CreateKeyGenre,(int)_self[ GENRE_MODULE_NUMBER ], _genres );
}

void CNrpScreenshot::InitializeOptions_()
{
	Add<NrpText>( INTERNAL_NAME, "" );
	Add( STARTDATE, NrpTime( 0. ) );
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