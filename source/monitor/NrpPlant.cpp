#include "StdAfx.h"
#include "NrpPlant.h"
#include "NrpPlantWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"
#include "NrpDiskMachine.h"
#include "NrpReklameWork.h"
#include "OpFileSystem.h"
#include "IniFile.h"

#include <assert.h>

static nrp::CNrpPlant* _GlobalObjectPlant_ = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPPLANT( L"CNrpPlant" );

template< class R >
int _SaveWorks( R& arrayt, int startNum, NrpText (*func)(int), const NrpText& fileName, const NrpText saveFolder )
{
	IniFile sv( fileName );

	for( u32 i=0; i < arrayt.size(); i++ )
	{
		NrpText myFolder = OpFileSystem::CheckEndSlash( saveFolder ) + func( startNum );
		OpFileSystem::CreateDirectory( myFolder );
		NrpText workFile = arrayt[ i ]->Save( myFolder );

		sv.Set( SECTION_WORKS, CreateKeyType( startNum ), arrayt[ i ]->ObjectTypeName() );
		sv.Set( SECTION_WORKS, CreateKeyWork( startNum ), workFile );
		startNum++;
	}

	sv.Save();

	return startNum;
}

CNrpPlant::CNrpPlant(void) : INrpConfig( CLASS_NRPPLANT, "" )
{
	Add<int>( WORKNUMBER, 0 );
	Add<int>( BASEREKLAMENUMBER, 0 );
	Add<int>( REKLAMENUMBER, 0 );
	Add<int>( DISKMACHINENUMBER, 0 );
}

CNrpPlant::~CNrpPlant(void)
{
}

NrpText CNrpPlant::Save( const NrpText& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );

	NrpText myFolder = OpFileSystem::CheckEndSlash( saveFolder );
	NrpText fileName = myFolder + "plant.ini";
	INrpConfig::Save( fileName );

	int lastNumber = _SaveWorks( _works, 0, CreateKeyWork, fileName, myFolder );
	_SaveWorks( _reklameWorks, lastNumber, CreateKeyReklame, fileName, myFolder );	

	return fileName;
}

void CNrpPlant::Load( const NrpText& saveFolder )
{
	NrpText fileName = OpFileSystem::CheckEndSlash( saveFolder ) + "plant.ini";
	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::Load( fileName );
	IniFile rv( fileName );

	int maxNumber = (int)Param( WORKNUMBER ) + (int)Param( REKLAMENUMBER );
	for( int k=0; k < maxNumber; k++ )
	{
		NrpText type = rv.Get( SECTION_WORKS, CreateKeyType( k ), NrpText("") );
		NrpText saveFile = rv.Get( SECTION_WORKS, CreateKeyWork( k ), NrpText("") );
		
		if( OpFileSystem::IsExist( saveFile ) )
		{
			if( type == CNrpPlantWork::ClassName() )
				AddWork( new CNrpPlantWork( saveFile, true ) );
			else if( type == CNrpReklameWork::ClassName() )
				AddReklame( CNrpReklameWork( saveFile ) );
		}
	}
}

void CNrpPlant::AddWork( CNrpPlantWork* work )
{
	CNrpCompany* cmp = _nrpApp.GetCompany( work->Param( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
		_works.push_back( new CNrpPlantWork( *work ) );

	_self[ WORKNUMBER ] = static_cast< int >( _works.size() );
}

//начало нового дня на фабрике
void CNrpPlant::BeginNewDay()
{
	//обработаем все задания по дискам на сегодня
	for( u32 k=0; k < _works.size(); k++ )
	{
		 _works[ k ]->BeginNewDay();
		 if( (bool)_works[ k ]->Param( FINISHED ) )
		 { //если это задание закончилось
			 delete _works[ k ];
			 _works.erase( k );
			 //удалим его из списка
			 k--;
		 }
	}
	//обновим значение текущих заданий
	_self[ WORKNUMBER ] = static_cast< int >( _works.size() );

	//обработаем зажпния по рекламе игр
	for( int k=0; k < (int)_reklameWorks.size(); k++ )
	{
		_reklameWorks[ k ]->BeginNewDay();
		//не кончился ли срок рекламирования товара
		if( _reklameWorks[ k ]->Param( FINISHED ) )
		{
			_nrpApp.DoLuaFunctionsByType( APP_REKLAME_FINISHED, _reklameWorks[ k ] );
			delete _reklameWorks[ k ];
			//если кончился, то надо удалить его из списка активных заданий
			_reklameWorks.erase( k );
			k--;
		}
	}
	//и обновить число текущих рекламных кампаний
	_self[ REKLAMENUMBER ] = static_cast< int >( _reklameWorks.size() );
}

CNrpReklameWork* CNrpPlant::CreateReklame( const NrpText& type, 
										   const NrpText& gameName, 
										   const NrpText& company )
{
	CNrpReklameWork* baseWork = GetBaseReklame( type );
	if( baseWork != NULL )
	{
		CNrpReklameWork* newReklame = new CNrpReklameWork( *baseWork );
		(*newReklame)[ GAMENAME ] = gameName;
		(*newReklame)[ COMPANYNAME ] = company;
		return newReklame;
	}
	else
	{
		if( type == "base" )
			return new CNrpReklameWork( gameName, "" );
	}

	return NULL;
}

bool CNrpPlant::AddBaseReklame( CNrpReklameWork* pReklame )
{
	assert( pReklame != NULL );

	bool ret = false;
	if( GetBaseReklame( (NrpText)(*pReklame)[ INTERNAL_NAME ] ) == NULL )
	{
		baseReklame_.push_back( pReklame );
		ret = true;
	}

	_self[ BASEREKLAMENUMBER ] = static_cast< int >( baseReklame_.size() );
	return ret;
}

CNrpReklameWork* CNrpPlant::GetBaseReklame( const NrpText& name )
{
	assert( name.size() != 0 );

	for( u32 i=0; i < baseReklame_.size(); i++ ) 
		if( (*baseReklame_[ i ])[ INTERNAL_NAME ] == name )
			return baseReklame_[ i ];

	return NULL;
}

void CNrpPlant::AddReklame( CNrpReklameWork& reklame )
{
	CNrpReklameWork* rWork = GetReklame( reklame[ INTERNAL_NAME ], reklame[ GAMENAME ] );

	if( rWork != NULL )
	{
		rWork->Update( reklame );
	}
	else
		_reklameWorks.push_back( new CNrpReklameWork( reklame ) );

	_self[ REKLAMENUMBER ] = static_cast< int >( _reklameWorks.size() );
}

CNrpReklameWork* CNrpPlant::GetReklame( const NrpText& type, 
										const NrpText& gameName )
{
	for( u32 i=0; i < _reklameWorks.size(); i++ ) 
		if( _reklameWorks[ i ]->Equale( type, gameName ) )
			return _reklameWorks[ i ];

	return NULL;
}

CNrpReklameWork* CNrpPlant::GetBaseReklame( int index )
{
	if( index < (int)baseReklame_.size() )
		return baseReklame_[ index ];

	return NULL;
}

CNrpPlant& nrp::CNrpPlant::Instance()
{
	if( _GlobalObjectPlant_ == NULL )
		_GlobalObjectPlant_ = new CNrpPlant();

	return *_GlobalObjectPlant_;
}

CNrpDiskMachine* CNrpPlant::GetDiskMachine( const NrpText& name )
{
	for( u32 i=0; i < diskMachines_.size(); i++)
		if( diskMachines_[ i ]->Param( NAME ) == name )
			return diskMachines_[ i ];

	return NULL;		
}

CNrpDiskMachine* CNrpPlant::GetDiskMachine( size_t index )
{
	return index < diskMachines_.size() ? diskMachines_[ index ] : NULL;
}

void CNrpPlant::AddDiskMachine( CNrpDiskMachine* pDm )
{
	diskMachines_.push_back( pDm );
	Param( DISKMACHINENUMBER) = static_cast< int >( diskMachines_.size() );
}

NrpText CNrpPlant::ClassName()
{
	return CLASS_NRPPLANT;
}

}//end namespace nrp