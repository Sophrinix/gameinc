#include "StdAfx.h"
#include "NrpLaborMarket.h"
#include "IUser.h"
#include "NrpApplication.h"
#include "NrpTechnology.h"
#include "NrpTester.h"
#include "NrpCompany.h"
#include "PeopleName.h"
#include "NrpAiUser.h"
#include "OpFileSystem.h"
#include "IniFile.h"
#include "NrpPlayer.h"

static nrp::CNrpLaborMarket* globalLaborMarket = NULL;

namespace nrp
{
CLASS_NAME CLASS_NRPLABORMARKET( "CNrpLaborMarket" );
static const NrpText MY_SAVE( L"users.list" );

CNrpLaborMarket::CNrpLaborMarket(void) : INrpConfig( CLASS_NRPLABORMARKET, CLASS_NRPLABORMARKET )
{
	Add( USERNUMBER, (int)0 );
}

CNrpLaborMarket::~CNrpLaborMarket(void)
{
}

int CNrpLaborMarket::_GetTechsByGroup( int type, TECHS& arrayt )
{
	const TECHS techs = _nrpApp.GetTechnologies();
	for( u32 i=0; i < techs.size(); i++ )
		if( (*techs[ i ])[ TECHGROUP ] == type )
			arrayt.push_back( techs[ i ] );

	return arrayt.size();
}

int RandomUserParam( int lim, float koeff )
{
	return static_cast< int >( (rand() % lim) * koeff );
}

void CNrpLaborMarket::_LoadUsers( const NrpText& fileName )
{
	IniFile sv( fileName );
	int maxUser = _self[ USERNUMBER ];
	for( int i=0; i < maxUser; i++ )
	{
		NrpText name = sv.Get( SECTION_USERS, CreateKeyUser(i), NrpText("") );
		NrpText className = sv.Get( SECTION_USERS, CreateKeyType(i), NrpText("") );

		NrpText fileName = (NrpText)_nrpApp[ SAVEDIR_USERS ] + name + ".user";

		if( !OpFileSystem::IsExist( fileName ) )
		{
			assert( false && "user config file not found" );
			continue;
		}

		CNrpUser* user = NULL;
		if( className == CNrpPlayer::ClassName() ) 
			user = new CNrpPlayer( fileName );
		else
		{
			if( className == CNrpAiUser::ClassName() )
				user = new CNrpAiUser( fileName );
			else
			{
				if( className == NrpTester::ClassName() )
					user = new NrpTester( name );
				else
					user = new CNrpUser( className, name );


				user->Load( fileName );
			}
		}

		assert( user );
		AddUser( user );
	}
}

void CNrpLaborMarket::CreateNewFreeUsers()
{
	USERS coders, designer, composer, tester;
	core::map< NrpText, USERS* > group;
	group[ NrpCoder::ClassName() ] = &coders;
	group[ NrpDesigner::ClassName() ] = &designer;
	group[ NrpComposer::ClassName() ] = &composer;
	group[ NrpTester::ClassName() ] = &tester;

	for( u32 i=0; i < _users.size(); i++ )
	{
		CNrpUser* user = _users[ i ];
		NrpText typeName = user->ObjectTypeName();
		if( (*user)[ PARENTCOMPANY ].As<PNrpCompany>() != NULL )
			continue;

		if( core::map< NrpText, USERS* >::Node* node = group.find( typeName ) )
			node->getValue()->push_back( user );
	}

	size_t USER_GROUP_COUNT = 6;

	core::map< NrpText, USERS* >::Iterator gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
	{
		USERS& tmpList = *(gIter->getValue());
		for( u32 k=0; k < tmpList.size(); k++ )
			RemoveUser( tmpList[ k ] );

		tmpList.clear();

		for( size_t cnt=tmpList.size(); cnt < USER_GROUP_COUNT; cnt++ )
			tmpList.push_back( CreateRandomUser( gIter->getKey() ) );
	}

	gIter = group.getIterator();
	for( ; !gIter.atEnd(); gIter++ )
		for( u32 cnt=0; cnt < gIter->getValue()->size(); cnt++ )
			_users.push_back( (*gIter->getValue())[ cnt ] );

	if( _self[ USERNUMBER ] != static_cast< int >( _users.size() ) )
		PCall( APP_USER_MARKETUPDATE, NULL );

	_self[ USERNUMBER ] = static_cast< int >( _users.size() );
}

CNrpUser* CNrpLaborMarket::CreateRandomUser( NrpText userType )
{
	TECHS genres;
	_GetTechsByGroup( PT_GENRE, genres );
	size_t randomParams = 1 + rand() % genres.size();//сколько параметров будем создавать
	int maxParamValue = 1 + rand() % 100;//максимальное значение параметров

	std::map< NrpText, NrpText > skillMap;
	skillMap[ "coder" ] = SKILL_CODING;
	skillMap[ "designer" ] = SKILL_DRAWING;
	skillMap[ "composer" ] = SKILL_SOUND;
	skillMap[ NrpTester::ClassName() ] = SKILL_TESTING;

	NrpText userName;

	CNrpUser* ptrUser = NULL;
	do 
	{
		userName = GlobalPeopleName[ rand() % PEOPLE_NAME_COUNT ] + " " + GlobalPeopleSurname[ rand() % PEOPLE_SURNAME_COUNT ];
		ptrUser = GetUser( userName );
	} while ( ptrUser != NULL );

	if( userType == CNrpAiUser::ClassName() )
		ptrUser = new CNrpAiUser( userName, NULL );
	else if( userType == NrpTester::ClassName() )
		ptrUser = new NrpTester( userName );
	else
		ptrUser = new CNrpUser( userType, userName );

	CNrpUser& refUser = *ptrUser;
	ptrUser->SetSkill( skillMap[ userType ], maxParamValue );
	refUser[ WORK_QUALITY ] = RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] );
	refUser[ WORK_SPEED ] = RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] );
	refUser[ TALANT ] = RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] );
	refUser[ STAMINA ] = RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] );
	refUser[ STABILITY ] = RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] );
	refUser[ CHARACTER ] = RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] );

	if( (int)refUser[ WANTMONEY ] < (int)_nrpApp[ MINIMUM_USER_SALARY ] )
		refUser[ WANTMONEY ] = _nrpApp[ MINIMUM_USER_SALARY ];

	char name[64] = { 0 };
	snprintf( name, 64, "media/face/face%03d.png", rand() % 2 );
	refUser[ TEXTURENORMAL ] = NrpText( name );

	for( size_t cnt=0; cnt < randomParams; cnt++ )
	{
		NrpText genreName = (*genres[ rand() % genres.size() ])[ INTERNAL_NAME ];
		ptrUser->SetGenreExperience( genreName, RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] ) );
		ptrUser->SetGenrePreferences( genreName, RandomUserParam( maxParamValue, _nrpApp[ DEV_FORCE ] ) );
	} 

	return ptrUser;
}

int CNrpLaborMarket::AddUser( CNrpUser* user )
{
	CNrpUser* tmpUser = GetUser( (*user)[ NAME ] );
	assert( tmpUser == NULL );

	if( user && tmpUser == NULL )
		_users.push_back( user );

	_self[ USERNUMBER ] = static_cast< int >( _users.size() );
	return 1;
}

CNrpUser* CNrpLaborMarket::GetUser( u32 index )
{
	return index < _users.size() ? _users[ index ] : NULL;
}

CNrpUser* CNrpLaborMarket::GetUser( const NrpText& name )
{
	return FindByNameAndIntName<USERS, CNrpUser>( _users, name );
}

int CNrpLaborMarket::RemoveUser( CNrpUser* user )
{
	for( u32 pos=0; pos < _users.size(); pos++ )
		if( _users[ pos ] == user )
		{
			delete _users[ pos ];
			_users.erase( pos );
			_self[ USERNUMBER ] = static_cast< int >( _users.size() );
			return 0;
		}

		return 1;
}

NrpText CNrpLaborMarket::Save( const NrpText& folderName )
{
	assert( OpFileSystem::IsExist( folderName ) );

	NrpText fileName = OpFileSystem::CheckEndSlash( folderName ) + MY_SAVE;

	INrpConfig::Save( fileName );

	IniFile sv( fileName );

	for( u32 i=0; i < _users.size(); i++ )
	{
		_users[ i ]->Save( _nrpApp[ SAVEDIR_USERS ] );
		sv.Set( SECTION_USERS, CreateKeyType(i), _users[ i ]->ObjectTypeName() );
		sv.Set( SECTION_USERS, CreateKeyUser(i), (NrpText)_users[ i ]->Param( NAME ) );
	}

	sv.Save();

	return fileName;
}

void CNrpLaborMarket::Load( const NrpText& folderName )
{
	assert( OpFileSystem::IsExist(folderName) );
	NrpText fileName = OpFileSystem::CheckEndSlash(folderName) + MY_SAVE;

	INrpConfig::Load( fileName );

	_LoadUsers( fileName );	
}

void CNrpLaborMarket::Reset()
{
	ClearArray( _users );
}

CNrpLaborMarket& nrp::CNrpLaborMarket::Instance()
{
	if( !globalLaborMarket )
		globalLaborMarket = new CNrpLaborMarket();

	return *globalLaborMarket;
}

}//end namespace nrp