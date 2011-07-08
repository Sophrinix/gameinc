#include "StdAfx.h"
#include "NrpLinkHolder.h"
#include "NativeIniFile.h"
#include "SectionNames.h"
#include "IniFile.h"


namespace nrp
{

static CNrpLinkHolder* globalLinkHolder = NULL;
const NrpText CNrpLinkHolder::numberTemplate = L"Number";
const NrpText CNrpLinkHolder::saveTemplate = L"links.list";

CNrpLinkHolder::CNrpLinkHolder(void)
{
    _general = new NativeIniFile();
}

CNrpLinkHolder::~CNrpLinkHolder(void)
{
    delete _general;
}

CNrpLinkHolder& CNrpLinkHolder::Instance()
{
    if( !globalLinkHolder )
        globalLinkHolder = new CNrpLinkHolder();

    return *globalLinkHolder;
}

void CNrpLinkHolder::Set( NrpText& name, NrpText& pathto )
{
    assert( pathto.size() > 0 );
    IniKey* key = _general->GetKey( const_cast< NrpText& >( SECTION_OPTIONS ).ToStr(), name.ToStr() );
    if( key )
    {
        assert( !key  );
        Log( HW ) << "Link with internal name " << name << " also exist in " << key->GetValue() << term;
    }
    else
        _general->SetKeyValue( const_cast< NrpText& >( SECTION_OPTIONS ).ToStr(), name, pathto );
}

NrpText CNrpLinkHolder::Get( NrpText& name )
{
    IniKey* key = _general->GetKey( const_cast< NrpText& >( SECTION_OPTIONS ).ToStr(), name );
    assert( key );
    if( key )
        return key->GetValue().c_str();

    Log( HW ) << "LinkWarning: link with name " << name << " not found " << term;
    return NrpText();
}

void CNrpLinkHolder::Load( const NrpText& fileName, const NrpText& templateName )
{
    IniFile ini( fileName );

    int number= ini.Get( SECTION_OPTIONS, templateName + CNrpLinkHolder::numberTemplate, (int)0 );
    for( int i=0; i < number; i++ )
    {
        NrpText intName = ini.Get( SECTION_OPTIONS, CreateKeyName( i ), NrpText("") );
        NrpText pathto = ini.Get( SECTION_OPTIONS, templateName + NrpText( (int)i ), NrpText("") );
        Set( intName, pathto );
    }
}

void CNrpLinkHolder::Load( const NrpText& fileName )
{
    _general->Load( fileName );
}

void CNrpLinkHolder::Save( const NrpText& fileName )
{
    _general->Save( fileName );
}
}//end namespace nrp