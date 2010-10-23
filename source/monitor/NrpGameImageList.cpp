#include "StdAfx.h"
#include "NrpGameImageList.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "nrpConfig.h"

#define SECTION_IMAGES "images"
#define KEY_IMAGE "image_"
#define SECTION_BOXIMAGES "imagesBox"
#define KEY_BOX_IMAGE "boxImage_"

namespace nrp
{

CNrpGameImageList::CNrpGameImageList(void) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
}

CNrpGameImageList::CNrpGameImageList( const CNrpGameImageList& a ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, a.GetValue<std::string>( NAME ) );
	SetValue<bool>( ISUSED, a.GetValue<bool>( ISUSED ) );
	SetValue<std::string>( GENRETECH, a.GetValue<std::string>( GENRETECH ) );
	SetValue<int>( YEAR, a.GetValue<int>( YEAR ) );

	imagesPath = a.imagesPath;
	imagesBoxPath = a.imagesBoxPath;
	SetValue<int>( IMAGESNUMBER, imagesPath.size() );
	SetValue<int>( IMAGESBOXNUMBER, imagesBoxPath.size() );
}

CNrpGameImageList::CNrpGameImageList( const char* name ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
	SetValue<std::string>( NAME, name );
}

CNrpGameImageList::CNrpGameImageList( const std::string& fileName, bool load ) : INrpConfig( CLASS_IMAGEGAMELIST, "" )
{
	InitializeOptions_();
	Load( fileName );
}

CNrpGameImageList::~CNrpGameImageList(void)
{
}

std::string CNrpGameImageList::Save( const std::string& fileIni )
{
	INrpConfig::Save( fileIni );

	for( size_t k=0; k < imagesPath.size(); k++ )
		IniFile::Write( SECTION_IMAGES, KEY_IMAGE+IntToStr(k), imagesPath[ k ], fileIni );

	for( size_t k=0; k < imagesBoxPath.size(); k++ )
		IniFile::Write( SECTION_BOXIMAGES, KEY_BOX_IMAGE+IntToStr(k), imagesBoxPath[ k ], fileIni );

	return fileIni;
}

void CNrpGameImageList::Load( const std::string& fileName )
{
	INrpConfig::Load( fileName );

	for( int k=0; k < GetValue<int>( IMAGESNUMBER ); k++ )
		imagesPath.push_back( IniFile::Read( SECTION_IMAGES, KEY_IMAGE+IntToStr(k), std::string(""), fileName ) );

	for( int k=0; k < GetValue<int>( IMAGESBOXNUMBER ); k++ )
		imagesBoxPath.push_back( IniFile::Read( SECTION_BOXIMAGES, KEY_BOX_IMAGE+IntToStr(k), std::string(""), fileName ) );
}

void CNrpGameImageList::InitializeOptions_()
{
	CreateValue<std::string>( NAME, "" );
	CreateValue<bool>( ISUSED, false );
	CreateValue<std::string>( GENRETECH, "" );
	CreateValue<int>( YEAR, 0 );
	CreateValue<int>( IMAGESNUMBER, 0 );
	CreateValue<int>( IMAGESBOXNUMBER, 0 );
}

}//end namespace nrp