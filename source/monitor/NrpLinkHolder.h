#ifndef _INCLUDE_NRPLINKHOLDER_
#define _INCLUDE_NRPLINKHOLDER_

#include "nrpArrays.h"

class NativeIniFile;

namespace nrp
{

class CNrpLinkHolder
{
public:
    static CNrpLinkHolder& Instance();
    ~CNrpLinkHolder(void);

    static const NrpText numberTemplate;
    static const NrpText saveTemplate;

    void Set( NrpText& name, NrpText& pathto );
    NrpText Get( NrpText& name );
   
    void Load( const NrpText& fileName, const NrpText& templateName );
    void Load( const NrpText& fileName );
    void Save( const NrpText& fileName );

private:
    CNrpLinkHolder(void);

    NativeIniFile* _general;
};

#define _nrpLinks CNrpLinkHolder::Instance()

}

#endif