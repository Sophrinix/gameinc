#include "StdAfx.h"
#include "NrpPlant.h"
#include "NrpPlantWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"
#include "NrpDiskMachine.h"
#include "NrpReklameWork.h"
#include "OpFileSystem.h"

#include <assert.h>

static nrp::CNrpPlant* _GlobalObjectPlant_ = NULL;

namespace nrp
{

template< class R >
int _SaveWorks( R& arrayt, int startNum, std::string (*func)(int), const std::string& fileName, const std::string saveFolder )
{
	R::iterator wIter = arrayt.begin();
	for( ; wIter != arrayt.end(); wIter++ )
	{
		std::string myFolder = OpFileSystem::CheckEndSlash( saveFolder ) + func( startNum );
		OpFileSystem::CreateDirectory( myFolder );
		std::string workFile = (*wIter)->Save( myFolder );
		IniFile::Write( SECTION_WORKS, KEY_TYPE( startNum ), (*wIter)->ObjectTypeName(), fileName );
		IniFile::Write( SECTION_WORKS, CreateKeyWork( startNum ), workFile, fileName );
		startNum++;
	}

	return startNum;
}

CNrpPlant::CNrpPlant(void) : INrpConfig( CLASS_NRPPLANT, "" )
{
	CreateValue<int>( WORKNUMBER, 0 );
	CreateValue<int>( BASEREKLAMENUMBER, 0 );
	CreateValue<int>( REKLAMENUMBER, 0 );
	CreateValue<int>( DISKMACHINENUMBER, 0 );
}

CNrpPlant::~CNrpPlant(void)
{
}

std::string CNrpPlant::Save( const std::string& saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );

	std::string myFolder = OpFileSystem::CheckEndSlash( saveFolder );
	std::string fileName = myFolder + "plant.ini";
	INrpConfig::Save( fileName );

	int lastNumber = _SaveWorks( _works, 0, CreateKeyWork, fileName, myFolder );
	_SaveWorks( _reklameWorks, lastNumber, CreateKeyReklame, fileName, myFolder );	

	return fileName;
}

void CNrpPlant::Load( const std::string& saveFolder )
{
	std::string fileName = OpFileSystem::CheckEndSlash( saveFolder ) + "plant.ini";
	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::Load( fileName );

	int maxNumber = GetValue<int>( WORKNUMBER ) + GetValue<int>( REKLAMENUMBER );
	for( int k=0; k < maxNumber; k++ )
	{
		std::string type = IniFile::Read( SECTION_WORKS, KEY_TYPE( k ), std::string(""), fileName );
		std::string saveFile = IniFile::Read( SECTION_WORKS, CreateKeyWork( k ), std::string(""), fileName );
		
		if( OpFileSystem::IsExist( saveFile ) )
		{
			if( type == CNrpPlantWork::ClassName() )
				AddWork( new CNrpPlantWork( saveFile, true ) );
			else if( type == CNrpReklameWork::ClassName() )
				AddReklame( new CNrpReklameWork( saveFile ) );
		}
	}
}

void CNrpPlant::AddWork( CNrpPlantWork* work )
{
	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( work->GetString( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
		_works.push_back( new CNrpPlantWork( *work ) );

	SetValue<int>( WORKNUMBER, _works.size() );
}

//начало нового дня на фабрике
void CNrpPlant::BeginNewDay()
{
	//обработаем все задания по дискам на сегодня
	for( int k=0; k < (int)_works.size(); k++ )
	{
		 _works[ k ]->BeginNewDay();
		 if( _works[ k ]->GetValue<bool>( FINISHED ) )
		 { //если это задание закончилось
			 delete _works[ k ];
			 _works.erase( _works.begin() + k );
			 //удалим его из списка
			 k--;
		 }
	}
	//обновим значение текущих заданий
	SetValue<int>( WORKNUMBER, _works.size() );

	//обработаем зажпния по рекламе игр
	for( int k=0; k < (int)_reklameWorks.size(); k++ )
	{
		_reklameWorks[ k ]->BeginNewDay();
		//не кончился ли срок рекламирования товара
		if( _reklameWorks[ k ]->GetValue<bool>( FINISHED ) )
		{
			CNrpApplication::Instance().DoLuaFunctionsByType( APP_REKLAME_FINISHED, _reklameWorks[ k ] );
			delete _reklameWorks[ k ];
			//если кончился, то надо удалить его из списка активных заданий
			_reklameWorks.erase( _reklameWorks.begin() + k );
			k--;
		}
	}
	//и обновить число текущих рекламных кампаний
	SetValue<int>( REKLAMENUMBER, _reklameWorks.size() );
}

CNrpReklameWork* CNrpPlant::CreateReklame( const std::string& type, 
										   const std::string& gameName, 
										   const std::string& company )
{
	CNrpReklameWork* baseWork = GetBaseReklame( type );
	if( baseWork != NULL )
	{
		CNrpReklameWork* newReklame = new CNrpReklameWork( *baseWork );
		newReklame->SetValue( GAMENAME, gameName );
		newReklame->SetValue( COMPANYNAME, company );
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
	if( GetBaseReklame( pReklame->GetString( TECHTYPE ) ) == NULL )
	{
		baseReklame_.push_back( pReklame );
		ret = true;
	}

	SetValue<int>( BASEREKLAMENUMBER, baseReklame_.size() );
	return ret;
}

CNrpReklameWork* CNrpPlant::GetBaseReklame( const std::string& name )
{
	assert( name.size() != 0 );

	REKLAME_LIST::iterator pIter = baseReklame_.begin();
	for( ; pIter != baseReklame_.end(); pIter++ ) 
		if( (*pIter)->GetString( TECHTYPE ) == name )
			return *pIter;

	return NULL;
}

void CNrpPlant::AddReklame( CNrpReklameWork* reklame )
{
	CNrpReklameWork* rWork = GetReklame( reklame->GetString( TECHTYPE ), 
										 reklame->GetString( GAMENAME ) );

	if( rWork != NULL )
	{
		rWork->Update( reklame );
	}
	else
		_reklameWorks.push_back( new CNrpReklameWork( *reklame ) );

	SetValue<int>( REKLAMENUMBER, _reklameWorks.size() );
}

CNrpReklameWork* CNrpPlant::GetReklame( const std::string& type, 
										const std::string& gameName )
{
	REKLAME_LIST::iterator pIter = _reklameWorks.begin();
	for( ; pIter != _reklameWorks.end(); pIter++ ) 
		if( (*pIter)->GetString( TECHTYPE ) == type &&
			(*pIter)->GetString( GAMENAME ) == gameName )
			return *pIter;

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

CNrpDiskMachine* CNrpPlant::GetDiskMachine( const std::string& name )
{
	DISKMACHINES_LIST::iterator dIter = diskMachines_.begin();
	for( ; dIter != diskMachines_.end(); dIter++)
		if( (*dIter)->GetString( NAME ) == name )
			return *dIter;

	return NULL;		
}

CNrpDiskMachine* CNrpPlant::GetDiskMachine( size_t index )
{
	return index < diskMachines_.size() ? diskMachines_[ index ] : NULL;
}

void CNrpPlant::AddDiskMachine( CNrpDiskMachine* pDm )
{
	diskMachines_.push_back( pDm );
	SetValue<int>( DISKMACHINENUMBER, diskMachines_.size() );
}

}//end namespace nrp