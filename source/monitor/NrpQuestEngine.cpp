#include "StdAfx.h"
#include "NrpQuestEngine.h"
#include "NrpQuest.h"
#include "OpFileSystem.h"
#include "nrpScript.h"
#include "IniFile.h"

namespace nrp
{
const NrpText MY_SAVE( "quest.list" );

static CNrpQuestEngine* globalQuestEngine = NULL;

CLASS_NAME CLASS_NRPQUESTENGINE( "CNrpQuestEngine" );

const NrpText CNrpQuestEngine::UnExist = "__unexist";

CNrpQuestEngine::CNrpQuestEngine(void) : INrpConfig( CLASS_NRPQUESTENGINE, CLASS_NRPQUESTENGINE )
{
	RegProperty<NrpText>( CLASSOBJECT, CLASS_NRPQUESTENGINE );
	RegProperty( ACTIVE_QUESTS_NUMBER, 0 );
	RegProperty( QUEST_NUMBER, 0 );
    RegProperty( QUEST_RESULT_NUMBER, 0 );
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
        Log( HW ) << "StartQuest: Can't find quest with name " << name << term;
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
                Log( HW ) << "_LoadQuest: Some error in quest " << name << " file:" << realPath << term;
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

void CNrpQuestEngine::AddActiveQuest( const NrpText& name )
{
    CNrpQuest* quest = GetQuest( name );

    assert( quest );
    if( !quest )
    {
        Log( HW ) << "AddActiveQuest: Can't find quest with name " << name << term;
        return;
    }
    else
        Log( HW ) << "AddActiveQuest: Added quest with name " << name << term;

    (*quest)[ OBSOLETE ] = false;
    if( (bool)(*quest)[ ACTIVE ] )
        return;
    
    (*quest)[ ACTIVE ] = true;
    _activeQuests[ name ] = quest;
    _self[ ACTIVE_QUESTS_NUMBER ] = static_cast< int >( _activeQuests.size() );
}

void CNrpQuestEngine::ObsoleteQuest( const NrpText& name )
{
    CNrpQuest* quest = GetQuest( name );

    assert( quest );
    if( !quest )
    {
        Log( HW ) << "ObsoleteQuest: Can't find quest with name " << name << term;
        return;
    }
 
    _activeQuests.erase( name );

    (*quest)[ OBSOLETE ] = true;
    (*quest)[ ACTIVE ] = false;
}

NrpText CNrpQuestEngine::Save( const NrpText& folderName )
{
    assert( OpFileSystem::IsExist( folderName ) );

    NrpText fileName = OpFileSystem::CheckEndSlash( folderName ) + MY_SAVE;

    INrpConfig::Save( fileName );

    IniFile sv( fileName );

    int seqNumber=0;
    for( QUEST_MAP::iterator i=_activeQuests.begin(); i != _activeQuests.end(); i++ )
    {
        sv.Set( SECTION_ACTIVE_QUESTS, CreateKeyQuest( seqNumber ), (NrpText)(*(i->second))[ INTERNAL_NAME ]);
        seqNumber++;
    }

    seqNumber = 0;
    for( QUEST_RESULT_MAP::iterator iy=_questResults.begin(); iy != _questResults.end(); iy++ )
    {
        NrpText tmp = iy->first;
        sv.Set( SECTIONS_QUEST_RESULTS, CreateKeyItem( seqNumber ), tmp );
        seqNumber++;
        
        RESULTS& rets = iy->second;
        for( RESULTS::iterator pIter=rets.begin(); pIter != rets.end(); pIter++ )
            sv.Set( iy->first, pIter->first, pIter->second );
    }
         
    sv.Save();

    return fileName;
}

void CNrpQuestEngine::Load( const NrpText& folderName )
{
    assert( OpFileSystem::IsExist( folderName ) );
    
    NrpText fileName = OpFileSystem::CheckEndSlash( folderName ) + MY_SAVE;

    if( !OpFileSystem::IsExist( fileName ) )
    {
        Log( HW ) << "Can't find config fot QuestEngine in " << fileName << term;
        return;
    }

    INrpConfig::Load( fileName );

    IniFile rv( fileName );

    int activeQuestNumber = _self[ ACTIVE_QUESTS_NUMBER ];
    for( int i=0; i < activeQuestNumber; i++ )
        AddActiveQuest( rv.Get( SECTION_ACTIVE_QUESTS, CreateKeyQuest( i ), NrpText("error") ) );

    int questResultsNumber = _self[ QUEST_RESULT_NUMBER ];
    for( int i=0; i < questResultsNumber; i++ )
    {
        NrpText sectionName = rv.Get( SECTIONS_QUEST_RESULTS, CreateKeyItem( i ), NrpText("") );

        IniSection* sct = rv.GetSection( sectionName );
        if( sct )
        {
            const IniSection::KeyIndexA& indexes = sct->GetKeys();

            for( IniSection::KeyIndexA::const_iterator pIter = indexes.begin(); pIter != indexes.end(); pIter++ )
                SetResult( sectionName, (*pIter)->GetKey().c_str(), (*pIter)->GetValue().c_str() );
        }
    }
}

void CNrpQuestEngine::SetResult( const NrpText& quest, const NrpText& name, const NrpText& result )
{
    _questResults[ quest ][ name ] = result;
}

NrpText CNrpQuestEngine::GetResult( const NrpText& quest, const NrpText& name )
{
    QUEST_RESULT_MAP::iterator qIter = _questResults.find( quest );

    assert( qIter != _questResults.end() );
    if( qIter != _questResults.end() )
    {
        const RESULTS& rets = qIter->second;
        RESULTS::const_iterator ret = rets.find( name );

        assert( ret != rets.end() );
        if( ret != rets.end() )
            return ret->second;
    }

    return CNrpQuestEngine::UnExist;
}

}