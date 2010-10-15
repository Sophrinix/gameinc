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

		(*wIter)->Save( SECTION_OPTIONS, workFile );
		IniFile::Write( "works", "work_" + IntToStr( k ), workFile, fileName );
	}

	REKLAME_LIST::iterator rIter = reklameWorks_.begin();
	for( int k=0; rIter != reklameWorks_.end(); rIter++, k++ )
	{
		std::string rekFile = saveFolder + (*rIter)->GetValue<std::string>( GAMENAME ) + "." + (*rIter)->GetValue<std::string>( TECHTYPE );

		(*rIter)->Save( SECTION_OPTIONS, rekFile );
		IniFile::Write( "reklames", "reklame_" + IntToStr( k ), rekFile, fileName );
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
			work->Load( SECTION_OPTIONS, workFile );
			AddWork( work );
		}
	}

	for( int k=0; k < GetValue<int>( REKLAMENUMBER ); k++ )
	{
		std::string rekFile = IniFile::Read( "reklames", "reklame_" + IntToStr( k ), std::string(""), fileName );

		if( _access( rekFile.c_str(), 0 ) != -1 )
		{
			CNrpReklameWork* reklame = new CNrpReklameWork( "error", "error" );
			reklame->Load( SECTION_OPTIONS, rekFile );
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

//������ ������ ��� �� �������
void CNrpPlant::BeginNewDay()
{
	//���������� ��� ������� �� ������ �� �������
	for( int k=0; k < (int)works_.size(); k++ )
	{
		 works_[ k ]->BeginNewDay();
		 if( works_[ k ]->GetValue<bool>( FINISHED ) )
		 { //���� ��� ������� �����������
			 delete works_[ k ];
			 works_.erase( works_.begin() + k );
			 //������ ��� �� ������
			 k--;
		 }
	}
	//������� �������� ������� �������
	SetValue<int>( WORKNUMBER, works_.size() );

	//���������� ������� �� ������� ���
	for( int k=0; k < (int)reklameWorks_.size(); k++ )
	{
		reklameWorks_[ k ]->BeginNewDay();
		//�� �������� �� ���� �������������� ������
		if( reklameWorks_[ k ]->GetValue<bool>( FINISHED ) )
		{
			delete reklameWorks_[ k ];
			//���� ��������, �� ���� ������� ��� �� ������ �������� �������
			reklameWorks_.erase( reklameWorks_.begin() + k );
			k--;
		}
	}
	//� �������� ����� ������� ��������� ��������
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
	if( GetBaseReklame( pReklame->GetValue<std::string>( NAME ) ) == NULL )
	{
		baseReklame_.push_back( pReklame );
		ret = true;
	}

	SetValue<int>( BASEREKLAMENUMBER, baseReklame_.size() );
	return ret;
}

CNrpReklameWork* CNrpPlant::GetBaseReklame( std::string name )
{
	assert( name.size() != 0 );

	REKLAME_LIST::iterator pIter = baseReklame_.begin();
	for( ; pIter != baseReklame_.end(); pIter++ ) 
		if( (*pIter)->GetValue<std::string>( TECHTYPE ) == name )
			return *pIter;

	return NULL;
}

void CNrpPlant::AddReklame( CNrpReklameWork* reklame )
{
	CNrpReklameWork* rWork = GetReklame( reklame->GetValue<std::string>( TECHTYPE ), 
										 reklame->GetValue<std::string>( GAMENAME ) );

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