#ifndef _INCLUDE_NRPQUESTENGINE_
#define _INCLUDE_NRPQUESTENGINE_

#include "nrpConfig.h"
#include "nrpArrays.h"

namespace nrp
{
class CNrpQuest;

OPTION_NAME ACTIVE_QUESTS_NUMBER("activeQuestNumber");
OPTION_NAME QUEST_NUMBER( "questNumber" ); 
OPTION_NAME QUEST_RESULT_NUMBER( "questResultNumber" );

class CNrpQuestEngine : public INrpConfig
{
public:
    static const NrpText UnExist;
	static CNrpQuestEngine& Instance();
	~CNrpQuestEngine(void);

	void AddQuest( const NrpText& name, const NrpText& fileName, int startRef );
	void RemoveQuest( const NrpText& name );

	CNrpQuest* GetQuest( size_t index );
	CNrpQuest* GetQuest( const NrpText& name );

	void AddActiveQuest( const NrpText& name );

	CNrpQuest* GetActiveQuest( size_t index );
	CNrpQuest* GetActiveQuest( const NrpText& name );

	void StartQuest( const NrpText& name );
    void ObsoleteQuest( const NrpText& name );

	void AddResourceDirectory( const NrpText& name );

    void SetResult( const NrpText& quest, const NrpText& name, const NrpText& result );
    NrpText GetResult( const NrpText& quest, const NrpText& name );

    NrpText Save( const NrpText& folderName );
    void Load( const NrpText& folderName );
private:
	CNrpQuestEngine(void);

	typedef std::map< NrpText, CNrpQuest* > QUEST_MAP;
    typedef std::map< NrpText, NrpText > RESULTS;
    typedef std::map< NrpText, RESULTS > QUEST_RESULT_MAP;

    QUEST_RESULT_MAP _questResults;
	QUEST_MAP _quests;
	QUEST_MAP _activeQuests;
	STRINGS _directories;

	CNrpQuest* _LoadQuest( const NrpText& dir );
	NrpText _CheckFile( const NrpText& name );
};

}

#endif