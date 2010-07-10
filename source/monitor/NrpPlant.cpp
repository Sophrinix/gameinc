#include "StdAfx.h"
#include "NrpPlant.h"
#include "NrpPlantWork.h"
#include "NrpCompany.h"
#include "NrpApplication.h"
#include "NrpGame.h"
#include "NrpDiskMachine.h"
#include "NrpReklameWork.h"

#include <io.h>
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
	std::string fileName = saveFolder + "plant.ini";
	INrpConfig::Save( scetionName, fileName );

	saveFolder += "plant/";
	if( _access( saveFolder.c_str(), 0 ) == -1 )
		CreateDirectory( saveFolder.c_str(), NULL );

	WORK_LIST::iterator wIter = works_.begin();
	for( int k=0; wIter != works_.end(); wIter++, k++ )
	{
		std::string workFile = saveFolder + (*wIter)->GetValue<std::string>( NAME ) + ".work";

		(*wIter)->Save( "options", workFile );
		IniFile::Write( "works", "work_" + IntToStr( k ), workFile, fileName );
	}
}

void CNrpPlant::Load( std::string scetionName, std::string saveFolder )
{
	std::string fileName = saveFolder + "plant.ini";
	std::string plantFolder = saveFolder + "plantWork/";

	INrpConfig::Load( scetionName, fileName );

	for( int k=0; k < GetValue<int>( WORKNUMBER ); k++ )
	{
		std::string workFile = IniFile::Read( "works", "work_" + IntToStr( k ), std::string(""), fileName );

		if( _access( workFile.c_str(), 0 ) != -1 )
		{
			CNrpPlantWork* work = new CNrpPlantWork( "" );
			work->Load( "options", workFile );
			AddWork( work );
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
			delete reklameWorks_[ k ];
			//если кончился, то надо удалить его из списка активных заданий
			reklameWorks_.erase( reklameWorks_.begin() + k );
			k--;
		}
	}
	//и обновить число текущих рекламных кампаний
	SetValue<int>( REKLAMENUMBER, reklameWorks_.size() );
}

CNrpReklameWork* CNrpPlant::CreateReklame( std::string type, std::string gameName )
{
	CNrpReklameWork* baseWork = GetBaseReklame( type );
	if( baseWork != NULL )
	{
		CNrpReklameWork* newReklame = new CNrpReklameWork( *baseWork );
		newReklame->SetValue<std::string>( GAMENAME, gameName );
		return newReklame;
	}
	else
	{
		if( type == "base" )
			return new CNrpReklameWork( gameName, "" );
	}

	return NULL;
}

void CNrpPlant::AddBaseReklame( CNrpReklameWork* pReklame )
{
	if( GetBaseReklame( pReklame->GetValue<std::string>( NAME ) ) == NULL )
		baseReklame_.push_back( pReklame );

	SetValue<int>( BASEREKLAMENUMBER, baseReklame_.size() );
}

CNrpReklameWork* CNrpPlant::GetBaseReklame( std::string name )
{
	REKLAME_LIST::iterator pIter = baseReklame_.begin();
	for( ; pIter != baseReklame_.end(); pIter++ ) 
		if( (*pIter)->GetValue<std::string>( TECHTYPE ) == name )
			return *pIter;

	return NULL;
}

void CNrpPlant::AddReklame( CNrpReklameWork* reklame )
{
	CNrpReklameWork* rWork = GetReklame( reklame->GetValue<std::string>( TECHTYPE ), 
										 reklame->GetValue<std::string>( NAME ) );

	if( rWork != NULL )
	{
		rWork->Update( reklame );
	}
	else
		reklameWorks_.push_back( new CNrpReklameWork( *reklame ) );

	SetValue<int>( REKLAMENUMBER, reklameWorks_.size() );
}

CNrpReklameWork* CNrpPlant::GetReklame( std::string type, std::string gameName )
{
	REKLAME_LIST::iterator pIter = reklameWorks_.begin();
	for( ; pIter != reklameWorks_.end(); pIter++ ) 
		if( (*pIter)->GetValue<std::string>( TECHTYPE ) == type &&
			(*pIter)->GetValue<std::string>( GAMENAME ) == gameName )
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