#include "stdafx.h"
#include "nativeinifile.h"
#include <algorithm>
#include <iostream>
#include <fstream>

// Only valid if C++
#ifndef __cplusplus
#error C++ compiler required.
#endif

// In the event you want to trace the calls can define _TRACE_CINIFILE
#ifdef _TRACE_CINIFILE
#define _CINIFILE_DEBUG
#endif

// _CRLF is used in the Save() function
// The class will read the correct data regardless of how the file linefeeds are defined <CRLF> or <CR>
// It is best to use the linefeed that is default to the system. This reduces issues if needing to modify
// the file with ie. notepad.exe which doesn't recognize unix linefeeds.
#ifdef _WIN32 // Windows default is \r\n
#ifdef _FORCE_UNIX_LINEFEED
#define _CRLFA "\n"
#define _CRLFW L"\n"
#else
#define _CRLFA "\r\n"
#define _CRLFW L"\r\n"
#endif

#else // Standard format is \n for unix
#ifdef _FORCE_WINDOWS_LINEFEED
#define _CRLFA "\r\n"
#define _CRLFW L"\r\n"
#else
#define _CRLFA "\n"
#define _CRLFW L"\n"
#endif
#endif

// Helper Functions
void RTrim(std::string &str, const std::string& chars = " \t")
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "RTrim()" << std::endl;
#endif
    str.erase(str.find_last_not_of(chars)+1);
}

void LTrim(std::string &str, const std::string& chars = " \t" )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "LTrim()" << std::endl;
#endif
    str.erase(0, str.find_first_not_of(chars));
}

void Trim( std::string& str , const std::string& chars = " \t" )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "Trim()" << std::endl;
#endif
    str.erase(str.find_last_not_of(chars)+1);
    str.erase(0, str.find_first_not_of(chars));
}

// Stream Helpers

std::ostream& operator<<(std::ostream& output, NativeIniFile& obj)
{
    obj.Save( output );
    return output;
}

std::istream& operator>>(std::istream& input, NativeIniFile& obj)
{
    obj.Load( input );
    return input;
}

std::istream& operator>>(std::istream& input, NativeIniFile::CIniMergeA merger)
{
    return merger(input);
}

// CIniFileA class methods

NativeIniFile::NativeIniFile()
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::CIniFileA()" << std::endl;
#endif
}

NativeIniFile::~NativeIniFile()
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::~CIniFileA()" << std::endl;
#endif
    RemoveAllSections( );
}

const char* const NativeIniFile::LF = _CRLFA;

void NativeIniFile::Save( std::ostream& output )
{
    std::string sSection;

    for( SecIndexA::iterator itr = m_sections.begin() ; itr != m_sections.end() ; ++itr )
    {
        sSection = "[" + (*itr)->GetSectionName() + "]";

#ifdef _CINIFILE_DEBUG
        std::cout <<  "Writing Section " << sSection << std::endl;
#endif

        output << sSection << _CRLFA;

        for( KeyIndexA::iterator klitr = (*itr)->m_keys.begin() ; klitr !=  (*itr)->m_keys.end() ; ++klitr )
        {
            std::string sKey = (*klitr)->GetKey() + "=" + (*klitr)->GetValue();
#ifdef _CINIFILE_DEBUG
            std::cout <<  "Writing Key [" << sKey << "]" << std::endl;
#endif
            output << sKey << _CRLFA;
        }
    }
}

bool NativeIniFile::Save( const NrpText& fileName )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::Save() - " << fileName << std::endl;
#endif

    std::ofstream output;

    output.open( const_cast< NrpText& >( fileName ).ToStr() , std::ios::binary );

    if( !output.is_open() )
        return false;

    Save(output);

    output.close();
    return true;
}


void NativeIniFile::Load( std::istream& input , bool bMerge )
{
    if( !bMerge )
        RemoveAllSections();

    IniSection* pSection = NULL;
    std::string sRead;
    enum { KEY , SECTION , COMMENT , OTHER };

    while( std::getline( input , sRead ) )
    {

        // Trim all whitespace on the left
        LTrim( sRead );
        // Trim any returns
        RTrim( sRead , "\n\r");

        if( !sRead.empty() )
        {
            unsigned int nType = ( sRead.find_first_of("[") == 0 && ( sRead[sRead.find_last_not_of(" \t\r\n")] == ']' ) ) ? SECTION : OTHER ;
            nType = ( (nType == OTHER) && ( sRead.find_first_of("=") ) > 0 ) ? KEY : nType ;
            nType = ( (nType == OTHER) && ( sRead.find_first_of("#") == 0) ) ? COMMENT : nType ;

            switch( nType )
            {
            case SECTION:
#ifdef _CINIFILE_DEBUG
                std::cout <<  "Parsing: Secton - " << sRead << std::endl;
#endif
                pSection = AddSection( sRead.substr( 1 , sRead.size() - 2 ) );
                break;

            case KEY:
            {
#ifdef _CINIFILE_DEBUG
                std::cout <<  "Parsing: Key - " << sRead << std::endl;
#endif
                // Check to ensure valid section... or drop the keys listed
                if( pSection )
                {
                    size_t iFind = sRead.find_first_of("=");
                    std::string sKey = sRead.substr(0,iFind);
                    std::string sValue = sRead.substr(iFind + 1);
					IniKey* pKey = pSection->AddKey( sKey );
                    if( pKey )
                    {
#ifdef _CINIFILE_DEBUG
                        std::cout <<  "Parsing: Key Value - " << sValue << std::endl;
#endif
                        pKey->SetValue( sValue );
                    }
                }
            }
            break;
            case COMMENT:
#ifdef _CINIFILE_DEBUG
                std::cout <<  "Parsing: Comment - " << sRead << std::endl;
#endif
                break;
            case OTHER:
#ifdef _CINIFILE_DEBUG
                std::cout <<  "Parsing: Other - " << sRead << std::endl;
#endif
                break;
            }
        }
    }
}

bool NativeIniFile::Load(const NrpText& fileName , bool bMerge )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::Load() - " << fileName << std::endl;
#endif

    std::ifstream input;

    input.open( const_cast< NrpText& >( fileName ).ToStr() , std::ios::binary);

    if( !input.is_open() )
        return false;

    Load( input , bMerge );

    input.close();
    return true;
}

const SecIndexA& NativeIniFile::GetSections() const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::GetSections()" << std::endl;
#endif
    return m_sections;
}


IniSection* NativeIniFile::GetSection( std::string sSection ) const
{
#ifdef _CINIFILE_DEBUG
    std::cout << "CIniFileA::GetSection()" << std::endl;
#endif
    Trim(sSection);
    SecIndexA::const_iterator itr = _find_sec( sSection );
    if( itr != m_sections.end() )
        return *itr;
    return NULL;
}

IniSection* NativeIniFile::AddSection( std::string sSection )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::AddSection()" << std::endl;
#endif

    Trim(sSection);
    SecIndexA::const_iterator itr = _find_sec( sSection );
    if( itr == m_sections.end() )
    {
        // Note constuctor doesn't trim the string so it is trimmed above
        IniSection* pSection = new IniSection( this , sSection );
        m_sections.insert(pSection);
        return pSection;
    }
    else
        return *itr;
}

IniKey* NativeIniFile::GetKey( const std::string& sSection, const std::string& sKey ) const
{
#ifdef _CINIFILE_DEBUG
	std::cout <<  "CIniFileA::GetKeyValue()" << std::endl;
#endif
	IniSection* pSec = GetSection( sSection );
	if( pSec )
		return pSec->GetKey( sKey );

	return NULL;
}

std::string NativeIniFile::GetKeyValue( const std::string& sSection, const std::string& sKey ) const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::GetKeyValue()" << std::endl;
#endif
    std::string sValue;
    IniSection* pSec = GetSection( sSection );
    if( pSec )
    {
        IniKey* pKey = pSec->GetKey( sKey );
        if( pKey )
            sValue = pKey->GetValue();
    }

	return sValue;
}

void NativeIniFile::SetKeyValue( const std::string& sSection, const std::string& sKey, const std::string& sValue )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::SetKeyValue()" << std::endl;
#endif
    IniSection* pSec = AddSection( sSection );
    if( pSec )
    {
        IniKey* pKey = pSec->AddKey( sKey );
        if( pKey )
            pKey->SetValue( sValue );
    }
}


void NativeIniFile::RemoveSection( std::string sSection )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::RemoveSection()" << std::endl;
#endif
    Trim(sSection);
    SecIndexA::iterator itr = _find_sec( sSection );
    if( itr != m_sections.end() )
    {
        delete *itr;
        m_sections.erase( itr );
    }
}

void NativeIniFile::RemoveSection( IniSection* pSection )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::RemoveSection()" << std::endl;
#endif
    // No trim since internal object not from user
    SecIndexA::iterator itr = _find_sec( pSection->m_sSectionName );
    if( itr != m_sections.end() )
    {
        delete *itr;
        m_sections.erase( itr );
    }
}

void NativeIniFile::RemoveAllSections( )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::RemoveAllSections()" << std::endl;
#endif
    for( SecIndexA::iterator itr = m_sections.begin() ; itr != m_sections.end() ; ++itr )
    {
#ifdef _CINIFILE_DEBUG
        std::cout <<  "Deleting Section: CIniSectionAName[" << (*itr)->GetSectionName() << "]" << std::endl;
#endif
        delete *itr;
    }
    m_sections.clear();
}


bool NativeIniFile::RenameSection( const std::string& sSectionName  , const std::string& sNewSectionName )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::RenameSection()" << std::endl;
#endif
    // Note string trims are done in lower calls.
    bool bRval = false;
    IniSection* pSec = GetSection( sSectionName );
    if( pSec )
    {
        bRval = pSec->SetSectionName( sNewSectionName );
    }
    return bRval;
}

bool NativeIniFile::RenameKey( const std::string& sSectionName  , const std::string& sKeyName , const std::string& sNewKeyName)
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniFileA::RenameKey()" << std::endl;
#endif
    // Note string trims are done in lower calls.
    bool bRval = false;
    IniSection* pSec = GetSection( sSectionName );
    if( pSec != NULL)
    {
        IniKey* pKey = pSec->GetKey( sKeyName );
        if( pKey != NULL )
            bRval = pKey->SetKeyName( sNewKeyName );
    }
    return bRval;
}

// Returns a constant iterator to a section by name, string is not trimmed
SecIndexA::const_iterator NativeIniFile::_find_sec( const std::string& sSection ) const
{
    IniSection bogus(NULL,sSection);
    return m_sections.find( &bogus );
}

// Returns an iterator to a section by name, string is not trimmed
SecIndexA::iterator NativeIniFile::_find_sec( const std::string& sSection )
{
    IniSection bogus(NULL,sSection);
    return m_sections.find( &bogus );
}

// CIniFileA functions end here

// CIniSectionA functions start here

IniSection::IniSection( NativeIniFile* pIniFile , const std::string& sSectionName ) : m_pIniFile(pIniFile) , m_sSectionName(sSectionName)
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::CIniSectionA()" << std::endl;
#endif
}


IniSection::~IniSection()
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::~CIniSectionA()" << std::endl;
#endif
    RemoveAllKeys();
}

IniKey* IniSection::GetKey( std::string sKeyName ) const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::GetKey()" << std::endl;
#endif
    Trim(sKeyName);
    KeyIndexA::const_iterator itr = _find_key( sKeyName );
    if( itr != m_keys.end() )
        return *itr;
    return NULL;
}

void IniSection::RemoveAllKeys()
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::RemoveAllKeys()" << std::endl;
#endif
    for( KeyIndexA::iterator itr = m_keys.begin() ; itr != m_keys.end() ; ++itr )
    {
#ifdef _CINIFILE_DEBUG
        std::cout <<  "Deleting Key: " << (*itr)->GetKeyName() << std::endl;
#endif
        delete *itr;
    }
    m_keys.clear();
}

void IniSection::RemoveKey( std::string sKey )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::RemoveKey()" << std::endl;
#endif
    Trim( sKey );
    KeyIndexA::iterator itr = _find_key( sKey );
    if( itr != m_keys.end() )
    {
        delete *itr;
        m_keys.erase( itr );
    }
}

void IniSection::RemoveKey( IniKey* pKey )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::RemoveKey()" << std::endl;
#endif
    // No trim is done to improve efficiency since CIniKeyA* should already be trimmed
    KeyIndexA::iterator itr = _find_key( pKey->m_sKeyName );
    if( itr != m_keys.end() )
    {
        delete *itr;
        m_keys.erase( itr );
    }
}

IniKey* IniSection::AddKey( std::string sKeyName )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::AddKey()" << std::endl;
#endif
    Trim(sKeyName);
    KeyIndexA::const_iterator itr = _find_key( sKeyName );
    if( itr == m_keys.end() )
    {
        // Note constuctor doesn't trim the string so it is trimmed above
        IniKey* pKey = new IniKey( this , sKeyName );
        m_keys.insert(pKey);
        return pKey;
    }
    else
        return *itr;
}

bool IniSection::SetSectionName( std::string sSectionName )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::SetSectionName()" << std::endl;
#endif
    Trim(sSectionName);
    // Does this already exist.
    if( m_pIniFile->_find_sec( sSectionName ) == m_pIniFile->m_sections.end() )
    {
#ifdef _CINIFILE_DEBUG
        std::cout <<  "Setting Section Name: [" << m_sSectionName <<  "] --> [" << sSectionName << "]" << std::endl;
#endif

        // Find the current section if one exists and remove it since we are renaming
        SecIndexA::iterator itr = m_pIniFile->_find_sec( m_sSectionName );

        // Just to be safe make sure the old section exists
        if( itr != m_pIniFile->m_sections.end() )
            m_pIniFile->m_sections.erase(itr);

        // Change name prior to ensure tree balance
        m_sSectionName = sSectionName;

        // Set the new map entry we know should not exist
        m_pIniFile->m_sections.insert(this);

        return true;
    }
    else
    {
#ifdef _CINIFILE_DEBUG
        std::cout <<  "Section existed could not rename" << std::endl;
#endif
        return false;
    }
}

std::string IniSection::GetSectionName() const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::GetSectionName()" << std::endl;
#endif
    return m_sSectionName;
}

const KeyIndexA& IniSection::GetKeys() const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::GetKeys()" << std::endl;
#endif
    return m_keys;
}

std::string IniSection::GetKeyValue( std::string sKey ) const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::GetKeyValue()" << std::endl;
#endif
    std::string sValue;
    IniKey* pKey = GetKey( sKey );
    if( pKey )
    {
        sValue = pKey->GetValue();
    }
    return sValue;
}

void IniSection::SetKeyValue( std::string sKey, const std::string& sValue )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::SetKeyValue()" << std::endl;
#endif
    IniKey* pKey = AddKey( sKey );
    if( pKey )
    {
        pKey->SetValue( sValue );
    }
}

// Returns a constant iterator to a key by name, string is not trimmed
KeyIndexA::const_iterator IniSection::_find_key( const std::string& sKey ) const
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::_find_key() const" << std::endl;
#endif
    IniKey bogus(NULL,sKey);
    return m_keys.find( &bogus );
}

// Returns an iterator to a key by name, string is not trimmed
KeyIndexA::iterator IniSection::_find_key( const std::string& sKey )
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniSectionA::_find_key()" << std::endl;
#endif
    IniKey bogus(NULL,sKey);
    return m_keys.find( &bogus );
}

// CIniSectionA function end here

// CIniKeyA Functions Start Here

IniKey::IniKey( IniSection* pSection , const std::string& sKeyName ) : m_pSection(pSection) , m_sKeyName(sKeyName)
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniKeyA::CIniKeyA()" << std::endl;
#endif
}


IniKey::~IniKey()
{
#ifdef _CINIFILE_DEBUG
    std::cout <<  "CIniKeyA::~CIniKeyA()" << std::endl;
#endif
}

void IniKey::SetValue( const std::string& sValue )
{
#ifdef _CINIFILE_DEBUG
    std::cout << "CIniKeyA::SetValue()" << std::endl;
#endif
    m_sValue = sValue;
}

std::string IniKey::GetValue() const
{
#ifdef _CINIFILE_DEBUG
    std::cout << "CIniKeyA::GetValue()" << std::endl;
#endif
    return m_sValue;
}

bool IniKey::SetKeyName( std::string sKeyName )
{
#ifdef _CINIFILE_DEBUG
    std::cout << "CIniKeyA::SetKeyName()" << std::endl;
#endif
    Trim( sKeyName );

    // Check for key name conflict
    if( m_pSection->_find_key( sKeyName ) == m_pSection->m_keys.end() )
    {
        KeyIndexA::iterator itr = m_pSection->_find_key( m_sKeyName );

        // Find the old map entry and remove it
        if( itr != m_pSection->m_keys.end() )
            m_pSection->m_keys.erase( itr );

        // Change name prior to ensure tree balance
        m_sKeyName = sKeyName;

        // Make the new map entry
        m_pSection->m_keys.insert(this);
        return true;
    }
    else
    {
#ifdef _CINIFILE_DEBUG
        std::cout << "Could not set key name, key by that name already exists!" << std::endl;
#endif
        return false;
    }
}

std::string IniKey::GetKey() const
{
#ifdef _CINIFILE_DEBUG
    std::cout << "CIniKeyA::GetKeyName()" << std::endl;
#endif
    return m_sKeyName;
}

std::string IniKey::GetShortKey() const
{
	int delimPos = m_sKeyName.find_first_of( ":" );
	return m_sKeyName.substr( 0, delimPos );

}

std::string IniKey::GetType() const
{
	int delimPos = m_sKeyName.find_first_of( ":" );
	return m_sKeyName.substr( delimPos + 1, 0xff );
}
