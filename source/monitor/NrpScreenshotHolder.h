#ifndef _INCLUDE_NRPSCREENSHOTHOLDER_
#define _INCLUDE_NRPSCREENSHOTHOLDER_

#include "nrpArrays.h"
#include <map>

namespace nrp
{

class CNrpScreenshotHolder 
{
public:
    static CNrpScreenshotHolder& Instance();
    ~CNrpScreenshotHolder(void);

    NrpText GetFreeInternalName( const CNrpGame& game );
    CNrpExtInfo* GetExtInfo( const NrpText& name );
    void Load( const NrpText& fileName, const NrpText& funcName );

private:
    CNrpScreenshotHolder(void);
    typedef std::pair< NrpText, CNrpExtInfo* > EXT_PATH;
    typedef std::map< NrpText, EXT_PATH > SCREENSHOTS_MAP;
    SCREENSHOTS_MAP _screenshots;

    CNrpExtInfo* _GetExtInfo( SCREENSHOTS_MAP::iterator& iterr );
};

#define _nrpScreenshots CNrpScreenshotHolder::Instance()

}

#endif