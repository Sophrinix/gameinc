#include "StdAfx.h"
#include "NrpSoundEngine.h"
#include "OpFileSystem.h"

namespace nrp
{

static CNrpSoundEngine* globalSoundEngine = NULL;

CNrpSoundEngine::CNrpSoundEngine(void)
{
	_engine = irrklang::createIrrKlangDevice();

}

CNrpSoundEngine::~CNrpSoundEngine(void)
{
}

CNrpSoundEngine& CNrpSoundEngine::Instance()
{
	if( !globalSoundEngine )	
	{
		globalSoundEngine = new CNrpSoundEngine();
	}

	return *globalSoundEngine;
}

void CNrpSoundEngine::_RemoveFinishedSounds()
{
	for( SOUND_MAP::iterator pIter=_sounds.begin(); pIter != _sounds.end();  )
	{
		if( pIter->second->isFinished() )
		{
			pIter->second->stop();
			pIter->second->drop();
			_sounds.erase( pIter++ );
		}
        else
            ++pIter;
	}
}

void CNrpSoundEngine::Play( NrpText name, const NrpText& pathToFile, bool loop )
{
	Remove( name );
	_RemoveFinishedSounds();

    if( !pathToFile.size() )
        return;

	NrpText realPath = _CheckFile( pathToFile );

	if( OpFileSystem::IsExist( realPath ) )
	{
		if( name == L"" )
			name = NrpText( L"random_" ) + NrpText( rand() );

		_sounds[ name ] = _engine->play2D( realPath, loop, false, true );
	}
	else
		Log( HW ) << "Can't find sound " << realPath << term;
}

void CNrpSoundEngine::Play( const NrpText& name, bool play )
{
	SOUND_MAP::iterator pIter = _sounds.find( name );
	if( pIter != _sounds.end() )
		pIter->second->setIsPaused( play );
}

void CNrpSoundEngine::Remove( const NrpText& name )
{
	SOUND_MAP::iterator pIter = _sounds.find( name );
	if( pIter != _sounds.end() )
	{
		if( pIter->second )
		{
			pIter->second->stop();
			pIter->second->drop();
		}
		_sounds.erase( pIter );
	}
}

void CNrpSoundEngine::Stop( const NrpText& name )
{
	SOUND_MAP::iterator pIter = _sounds.find( name );
	if( pIter != _sounds.end() )
		pIter->second->stop();
}

void CNrpSoundEngine::Effect( const NrpText& name, const NrpText& effect )
{

}

NrpText CNrpSoundEngine::_CheckFile( const NrpText& name )
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

void CNrpSoundEngine::AddSoundsDirectory( const NrpText& dir )
{
	for( u32 i=0; i < _directories.size(); i++ )
		if( _directories[ i ] == dir )
			return;

	_directories.push_back( dir );
}

}//end namespace nrp