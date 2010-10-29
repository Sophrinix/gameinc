#include "StdAfx.h"
#include "NrpScreenshot.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "nrpConfig.h"
#include "NrpGame.h"

#define SECTION_IMAGES "images"
#define SECTION_BOXIMAGES "imagesBox"

#define KEY_IMAGE(index) ("image_"+IntToStr(index))
#define KEY_BOX_IMAGE(index) ("boxImage_"+IntToStr(index))

namespace nrp
{

CNrpScreenshot::CNrpScreenshot(void) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
}

CNrpScreenshot::CNrpScreenshot( const CNrpScreenshot& a ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, a.GetValue<std::string>( NAME ) );

	_imagesPath = a._imagesPath;
	_imagesBoxPath = a._imagesBoxPath;
	SetValue<int>( IMAGESNUMBER, _imagesPath.size() );
	SetValue<int>( IMAGESBOXNUMBER, _imagesBoxPath.size() );
}

CNrpScreenshot::CNrpScreenshot( const std::string& fileName ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
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
	for( STRING_LIST::const_iterator pIter=_genres.begin();
		 pIter != _genres.end(); 
		 pIter++ )
	{
		equale += game->IsGenreAvaible( *pIter ) ? 1 : 0;
	}

	return ( equale * 100 / _genres.size() );
}

void CNrpScreenshot::Load( const std::string& fileName )
{
	INrpConfig::Load( fileName );

	for( int k=0; k < GetValue<int>( IMAGESNUMBER ); k++ )
		_imagesPath.push_back( IniFile::Read( SECTION_IMAGES, KEY_IMAGE(k), std::string(""), fileName ) );

	for( int k=0; k < GetValue<int>( IMAGESBOXNUMBER ); k++ )
		_imagesBoxPath.push_back( IniFile::Read( SECTION_BOXIMAGES, KEY_BOX_IMAGE(k), std::string(""), fileName ) );

	for( int i=0; i < GetValue<int>( GENRE_MODULE_NUMBER ); ++i )
		_genres.push_back( IniFile::Read( SECTION_GENRES, KEY_GENRE(i), std::string(""), fileName ) );
}

void CNrpScreenshot::InitializeOptions_()
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<int>( STARTDATE, 0 );
	CreateValue<int>( IMAGESNUMBER, 0 );
	CreateValue<int>( IMAGESBOXNUMBER, 0 );
	CreateValue<int>( GENRE_MODULE_NUMBER, 0 );
}

}//end namespace nrp