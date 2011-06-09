#ifndef _INCLUDE_NRPQUESTENGINE_
#define _INCLUDE_NRPQUESTENGINE_

#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
class CNrpQuest;

OPTION_NAME ACTIVE_QUESTS_NUMBER("activeQuestNumber");
OPTION_NAME QUEST_NUMBER( "questNumber" ); 

class CNrpQuestEngine : public INrpConfig
{
public:
	static CNrpQuestEngine& Instance();
	~CNrpQuestEngine(void);

	void AddQuest( const NrpText& name, const NrpText& fileName, int startRef );
	void RemoveQuest( const NrpText& name );

	CNrpQuest* GetQuest( size_t index );
	CNrpQuest* GetQuest( const NrpText& name );

	void AddActiveQuest( const NrpText& name );
	void RemoveActiveQuest( const NrpText& name );

	CNrpQuest* GetActiveQuest( size_t index );
	CNrpQuest* GetActiveQuest( const NrpText& name );

	void StartQuest( const NrpText& name );
	void EndQuest( const NrpText& name );

	void AddResourceDirectory( const NrpText& name );

private:
	CNrpQuestEngine(void);

	typedef std::map< NrpText, CNrpQuest* > QUEST_MAP;
	QUEST_MAP _quests;
	QUEST_MAP _activeQuests;
	STRINGS _directories;

	CNrpQuest* _LoadQuest( const NrpText& dir );
	NrpText _CheckFile( const NrpText& name );
};

}

#endif