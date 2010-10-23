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

CNrpPlant::CNrpPlant(void) : INrpConfig( CLASS_NRPPLANT, "" )
{
	CreateValue<int>( WORKNUMBER, 0 );
	CreateValue<int>( BASEREKLAMENUMBER, 0 );
	CreateValue<int>( REKLAMENUMBER, 0 );
}

CNrpPlant::~CNrpPlant(void)
{
}

void CNrpPlant::Save( std::string scetionName, std::string saveFolder )
{
	assert( OpFileSystem::IsExist( saveFolder ) );

	std::string fileName = saveFolder + "plant.ini";
	INrpConfig::Save( fileName );

	WORK_LIST::iterator wIter = works_.begin();
	for( int k=0; wIter != works_.end(); wIter++, k++ )
	{
		std::string workFile = saveFolder + (*wIter)->GetValue<std::string>( NAME ) + ".work";

		(*wIter)->Save( workFile );
		IniFile::Write( SECTION_WORKS, KEY_WORK( k ), workFile, fileName );
	}

	REKLAME_LIST::iterator rIter = reklameWorks_.begin();
	for( int k=0; rIter != reklameWorks_.end(); rIter++, k++ )
	{
		std::string reglameSaveFolder = saveFolder + KEY_REKLAME( k );
		OpFileSystem::CreateDirectory( reglameSaveFolder );
		std::string reklameSaveFile = (*rIter)->Save( reglameSaveFolder );
		IniFile::Write( SECTION_REKLAMES, KEY_REKLAME( k ), reklameSaveFile, fileName );
	}
}

void CNrpPlant::Load( std::string scetionName, std::string saveFolder )
{
	std::string fileName = saveFolder + "plant.ini";
	assert( OpFileSystem::IsExist( fileName ) );

	INrpConfig::Load( fileName );

	for( int k=0; k < GetValue<int>( WORKNUMBER ); k++ )
	{
		std::string workFile = IniFile::Read( SECTION_WORKS, KEY_WORK( k ), std::string(""), fileName );

		assert( OpFileSystem::IsExist( workFile ) );
		if( OpFileSystem::IsExist( workFile ) )
		{
			CNrpPlantWork* work = new CNrpPlantWork( workFile, true );
			AddWork( work );
		}
	}

	for( int k=0; k < GetValue<int>( REKLAMENUMBER ); k++ )
	{
		std::string rekFile = IniFile::Read( SECTION_REKLAMES, KEY_REKLAME( k ), std::string(""), fileName );

		assert( OpFileSystem::IsExist( rekFile ) );
		if( OpFileSystem::IsExist( rekFile.c_str() ) )
		{
			CNrpReklameWork* reklame = new CNrpReklameWork( rekFile );
			AddReklame( reklame );
		}
	}
}

void CNrpPlant::AddWork( CNrpPlantWork* work )
{
	CNrpCompany* cmp = CNrpApplication::Instance().GetCompany( work->GetValue<std::string>( COMPANYNAME ) );
	assert( cmp != NULL );

	if( cmp != NULL )
	{
		cmp->AddValue( BALANCE, -work->GetValue<int>( RENTPRICE ) );
		works_.push_back( new CNrpPlantWork( *work ) );
	}

	SetValue<int>( WORKNUMBER, works_.size() );
}

//начало нового дня на фабрике
void CNrpPlant::BeginNewDay()
{
	//обработаем все задания по дискам на сегодня
	for( int k=0; k < (int)works_.size(); k++ )
	{
		 works_[ k ]->BeginNewDay();
		 if( works_[ k ]->GetValue<bool>( FINISHED ) )
		 { //если это задание закончилось
			 delete works_[ k ];
			 works_.erase( works_.begin() + k );
			 //удалим его из списка
			 k--;
		 }
	}
	//обновим значение текущих заданий
	SetValue<int>( WORKNUMBER, works_.size() );

	//обработаем зажпния по рекламе игр
	for( int k=0; k < (int)reklameWorks_.size(); k++ )
	{
		reklameWorks_[ k ]->BeginNewDay();
		//не кончился ли срок рекламирования товара
		if( reklameWorks_[ k ]->GetValue<bool>( FINISHED ) )
		{
			CNrpApplication::Instance().DoLuaFunctionsByType( APP_REKLAME_FINISHED, reklameWorks_[ k ] );
			delete reklameWorks_[ k ];
			//если кончился, то надо удалить его из списка активных заданий
			reklameWorks_.erase( reklameWorks_.begin() + k );
			k--;
		}
	}
	//и обновить число текущих рекламных кампаний
	SetValue<int>( REKLAMENUMBER, reklameWorks_.size() );
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
		reklameWorks_.push_back( new CNrpReklameWork( *reklame ) );

	SetValue<int>( REKLAMENUMBER, reklameWorks_.size() );
}

CNrpReklameWork* CNrpPlant::GetReklame( const std::string& type, 
										const std::string& gameName )
{
	REKLAME_LIST::iterator pIter = reklameWorks_.begin();
	for( ; pIter != reklameWorks_.end(); pIter++ ) 
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

}//end namespace nrp