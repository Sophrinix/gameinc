#ifndef _INCLUDE_SOUNDENGINE_
#define _INCLUDE_SOUNDENGINE_

#include "irrKlang.h"
#include "ik_ISoundEngine.h"
#include "ik_ISound.h"
#include "NrpText.h"
#include "nrpArrays.h"
#include <map>

namespace nrp
{

class CNrpSoundEngine
{
public:
	static CNrpSoundEngine& Instance();
	~CNrpSoundEngine(void);

	void Play( const NrpText& name, bool play );
	void Play( NrpText name, const NrpText& pathToFile, bool loop );
	void Stop( const NrpText& name );
	void Remove( const NrpText& name );
	void Effect( const NrpText& name, const NrpText& effect );
	void AddSoundsDirectory( const NrpText& dir );

private:
	CNrpSoundEngine(void);
	irrklang::ISoundEngine* _engine;

	typedef std::map< NrpText, irrklang::ISound* > SOUND_MAP;
	SOUND_MAP _sounds;

	STRINGS _directories;

	NrpText _CheckFile( const NrpText& name );
	void _RemoveFinishedSounds();
};

#define _nrpSound CNrpSoundEngine::Instance()

} //end namespace nrp
#endif
