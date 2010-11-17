#include "StdAfx.h"
#include "NrpScreenshot.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "nrpConfig.h"
#include "NrpGame.h"

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
	SetValue<NrpText>( NAME, a.GetValue<NrpText>( NAME ) );

	_imagesPath = a._imagesPath;
	_imagesBoxPath = a._imagesBoxPath;
	SetValue<int>( IMAGESNUMBER, _imagesPath.size() );
	SetValue<int>( IMAGESBOXNUMBER, _imagesBoxPath.size() );
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
	return GetValue<SYSTEMTIME>( STARTDATE ).wYear <= year;
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

	for( int k=0; k < GetValue<int>( IMAGESNUMBER ); k++ )
		_imagesPath.push_back( rv.Get( SECTION_IMAGES, CreateKeyImage(k), NrpText("") ) );

	for( int k=0; k < GetValue<int>( IMAGESBOXNUMBER ); k++ )
		_imagesBoxPath.push_back( rv.Get( SECTION_BOXIMAGES, CreateKeyBoxImage(k), NrpText("") ) );

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
		_genres.push_back( rv.Get( SECTION_GENRES, CreateKeyGenre(i), NrpText("") ) );
}

void CNrpScreenshot::InitializeOptions_()
{
	Push<NrpText>( NAME, "" );
	Push<SYSTEMTIME>( STARTDATE, SYSTEMTIME() );
	Push<int>( IMAGESNUMBER, 0 );
	Push<int>( IMAGESBOXNUMBER, 0 );
	Push<int>( GENRE_MODULE_NUMBER, 0 );
}

NrpText CNrpScreenshot::ClassName()
{
	return CLASS_IMAGEGAMELIST;
}

}//end namespace nrp