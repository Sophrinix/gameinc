#include "StdAfx.h"
#include "NrpQuestEngine.h"
#include "NrpQuest.h"
#include "OpFileSystem.h"
#include "nrpScript.h"

namespace nrp
{
static CNrpQuestEngine* globalQuestEngine = NULL;

CLASS_NAME CLASS_NRPQUESTENGINE( "CNrpQuestEngine" );

CNrpQuestEngine::CNrpQuestEngine(void) : INrpConfig( CLASS_NRPQUESTENGINE, CLASS_NRPQUESTENGINE )
{
	Add<NrpText>( CLASSOBJECT, CLASS_NRPQUESTENGINE );
	Add( ACTIVE_QUESTS_NUMBER, 0 );
	Add( QUEST_NUMBER, 0 );
}

CNrpQuestEngine::~CNrpQuestEngine(void)
{
}

CNrpQuestEngine& CNrpQuestEngine::Instance()
{
	if( !globalQuestEngine )
		globalQuestEngine = new CNrpQuestEngine();

	return *globalQuestEngine;
}

CNrpQuest* CNrpQuestEngine::GetActiveQuest( size_t index )
{
	if( index < _activeQuests.size() )
	{
		QUEST_MAP::iterator pIter = _activeQuests.begin();
		for( int k=0; k != index; k++ ) pIter++;

		return pIter->second;
	}


	return NULL;
}

CNrpQuest* CNrpQuestEngine::GetActiveQuest( const NrpText& name )
{
	QUEST_MAP::iterator pIter = _activeQuests.find( name );
	return pIter != _activeQuests.end() ? pIter->second : NULL;
}

void CNrpQuestEngine::StartQuest( const NrpText& name )
{
	CNrpQuest* qst = GetQuest( name );
		
	assert( qst && "can't find quest" );

	if( qst )
	{
		qst->Start();
	}
	else
		Log( HW ) << "Can't find quest with name " << name << term;
}

CNrpQuest* CNrpQuestEngine::_LoadQuest( const NrpText& name )
{
	NrpText realPath = _CheckFile( name + ".lua" );

	try
	{
		if( OpFileSystem::IsExist( realPath ) )
		{			
			_quests[ name ] = new CNrpQuest();
			(*_quests[ name ])[ BASEFILE ] = realPath;
			(*_quests[ name ])[ INTERNAL_NAME ] = name;

			bool ok = !CNrpScript::Instance().DoFile( realPath );

			if( ok )
			{
				_self[ QUEST_NUMBER ] = static_cast< int >( _quests.size() );
				return _quests[ name ];
			}
			else
				Log( HW ) << "Some error in quest " << name << " file:" << realPath << term;
		}
	}
	catch(...)
	{}

	delete _quests[ name ];
	_quests.erase( name );
	return NULL;
}

NrpText CNrpQuestEngine::_CheckFile( const NrpText& name )
{
	if( OpFileSystem::IsExist( name ) )
		return name;

	for( u32 i=0; i < _directories.size(); i++ )
	{
		io::path tmp = _directories[ i ] + "/" + name;
		if( OpFileSystem::IsExist( tmp ) )
			return tmp;
	}

	return name;
}

void CNrpQuestEngine::AddResourceDirectory( const NrpText& dir )
{
	for( u32 i=0; i < _directories.size(); i++ )
		if( _directories[ i ] == dir )
			return;

	_directories.push_back( dir );
}

CNrpQuest* CNrpQuestEngine::GetQuest( const NrpText& name )
{
	QUEST_MAP::iterator pIter = _quests.find( name );

	CNrpQuest* qst = NULL;
	if( pIter == _quests.end() )
		qst = _LoadQuest( name );
	else 
		qst = pIter->second;

	return qst;
}

void CNrpQuestEngine::EndQuest( const NrpText& name )
{
	QUEST_MAP::iterator pIter = _activeQuests.find( name );

	if( pIter != _activeQuests.end() )
	{
		(*pIter->second)[ OBSOLETE ] = true;
		_activeQuests.erase( pIter );
	}
	else
		Log( HW ) << "Can't find questwith name=" << name << term;
}

}