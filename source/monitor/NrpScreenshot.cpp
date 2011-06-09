#include "StdAfx.h"
#include "NrpScreenshot.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "nrpConfig.h"
#include "NrpGame.h"

//Префикс для картинок, которые будем искать( скрины и сплеши )
static const NrpText SCREEN( L"scr_" );
static const NrpText SPLASH( L"spl_" );
static const NrpText NO_IMAGE_PNG( L"media/textures/no_image.png" );

//Функция возвращает относительное имя файла
NrpText _GetImageFileName( const NrpText& prefix, int index, const NrpText& ext )
{
	return prefix + NrpText( index ) + ext;
}

namespace nrp
{
CLASS_NAME CLASS_EXTINFO( "CNrpExtInfo" );

CNrpExtInfo::CNrpExtInfo(void) : INrpConfig( CLASS_EXTINFO, "" )
{
	//Инициализация переменных, которые используются в классе
	InitializeOptions_();
}

CNrpExtInfo::CNrpExtInfo( const CNrpExtInfo& a ) : INrpConfig( CLASS_EXTINFO, "" )
{
	InitializeOptions_();
	_self[ INTERNAL_NAME ] = a[ INTERNAL_NAME ];

	_imagesPath = a._imagesPath;
	_imagesBoxPath = a._imagesBoxPath;
	_self[ IMAGESNUMBER ] = static_cast< int >( _imagesPath.size() );
	_self[ IMAGESBOXNUMBER ] = static_cast< int >( _imagesBoxPath.size() );
}

CNrpExtInfo::CNrpExtInfo( const NrpText& fileName ) : INrpConfig( CLASS_EXTINFO, "" )
{
	InitializeOptions_();
	//Загрузка данных из файла
	Load( fileName );
}

CNrpExtInfo::~CNrpExtInfo(void)
{
}

//Функция проверяет меньше ли указанный год, чем год выхода игры
bool CNrpExtInfo::IsMyYear( int year )
{
	int myYear = _self[ STARTDATE ].As<NrpTime>().RYear();
	return core::abs_(year -  myYear) <= 1;
}

int CNrpExtInfo::GetEqualeRating( const CNrpGame& game )
{
	int equale = 0;
	//подсчитаем количество совпавших жанров
	for( u32 i=0; i < _genres.size(); i++ )
	{
		equale += game.IsGenreAvaible( _genres[ i ] ) ? 1 : 0;
	}

	//вычислим процент совпадения для игры по жанрам
	return ( equale * 100 / _genres.size() );
}

bool CNrpExtInfo::_AddImage( STRINGS& art, const NrpText& fileName )
{
	if( OpFileSystem::IsExist( fileName ) )
	{
		art.push_back( fileName );
		return true;
	}

	return false;
}

void CNrpExtInfo::_LoadImages( STRINGS& imgs, const NrpText& folder, const NrpText& prefix )
{
	int k = 0;
	bool mayAdded = true;

	while( mayAdded )
	{
		mayAdded = false;

		mayAdded |= _AddImage( imgs, folder + _GetImageFileName( prefix, k, ".png" ) );
		mayAdded |= _AddImage( imgs, folder + _GetImageFileName( prefix, k, ".jpg" ) );

		k++;
	}

	if( !imgs.size() )
	{
		Log(HW) << "can't find any image for game " << (NrpText)_self[ INTERNAL_NAME ] << " with prefix " << prefix << term;
		imgs.push_back( NO_IMAGE_PNG );
	}
}

void CNrpExtInfo::Load( const NrpText& fileName )
{
	//Загрузим данные из файла
	INrpConfig::Load( fileName );
	//теперь получим доступ к файлу с данными, для дальнейшей работы
	IniFile rv( fileName );

	assert( ((NrpText)_self[ INTERNAL_NAME ]).size() );

	//получим имя корневой папки
	NrpText folder = OpFileSystem::UpDir( fileName );
	//найдем все скриншоты игры
	_LoadImages( _imagesPath, folder, SCREEN );
	_self[ IMAGESNUMBER ] = static_cast< int >( _imagesPath.size() );

	//найдем все скриншоты заставок
	_LoadImages( _imagesBoxPath, folder, SPLASH );
	_self[ IMAGESBOXNUMBER ] = static_cast< int >( _imagesBoxPath.size() );
	_self[ NAME ] = _self[ INTERNAL_NAME ];

	//загрузим жанры, которые связаны с этим набором изображений
	rv.Get( SECTION_GENRES, CreateKeyGenre, -1, _genres );
	assert( _genres.size() > 0 && "can't find any genres for CNrpScreenshot" );
	if( _genres.size() == 0 )
		Log( HW ) << "CNrpScreenshot Warning: Can't find any genres in " << fileName << term;

	_self[ GENRE_MODULE_NUMBER ] = static_cast< int >( _genres.size() );

	rv.Get( SECTION_RECENSE, CreateKeyRecense, -1, _recenses );
	if( _recenses.size() == 0 )
		Log( HW ) << "CNrpScreenshot Warning: Can't find any recenses in " << fileName << term;
	
}

void CNrpExtInfo::InitializeOptions_()
{
	Add<NrpText>( INTERNAL_NAME, "" );
	Add( STARTDATE, NrpTime( 0. ) );
	Add<int>( IMAGESNUMBER, 0 );
	Add<int>( IMAGESBOXNUMBER, 0 );
	Add<NrpText>( NAME, "" );
	Add<int>( GENRE_MODULE_NUMBER, 0 );
}

NrpText CNrpExtInfo::ClassName()
{
	return CLASS_EXTINFO;
}

}//end namespace nrp