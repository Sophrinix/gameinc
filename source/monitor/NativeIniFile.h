#ifndef __NATIVEINIFILE_H_
#define __NATIVEINIFILE_H_

#include <set>
#include <string>
#include <string.h>
#include <wchar.h>
#include <algorithm>

#define INI_TOKEN_A_ANSI "\a"	// I.E. Item1;Item2;Item3 - '\a' used in place of ';'
#define INI_TOKEN_B_ANSI "\b"	// I.E. Item1,Item1b;Item2,Item2b;Item3,Item3b - '\b' used in place of ','
#define INI_EMPTY_ANSI "*"		// Used to indicate empty value in token string. I.E. *;Item2;*;Item3;

class IniKey
{
	friend class IniSection; // Allow CIniSectionA to create keys
	friend struct ci_less_a;
private: // CIniFileA acts as a class factory for CIniSectionA Objects
	IniKey( IniSection* pSection , const std::string& sKeyName );
	IniKey( const IniKey& ); // No Copy
	IniKey& operator=(const IniKey&); // No Copy
	~IniKey( );
public:
	// Sets the value of the key
	void SetValue( const std::string& sValue );
	// Returns the value of the key
	std::string GetValue() const;
	std::string GetShortKey() const;
	std::string GetType() const;
	// Sets the key name, returns true on success, fails if the section
	// name sKeyName already exists
	bool SetKeyName( std::string sKeyName );
	// Returns the name of the Key
	std::string GetKey() const;
private:
	// Pointer to the parent CIniSectionA
	IniSection* m_pSection;
	// Name of the Key
	std::string m_sKeyName;
	// Value associated
	std::string m_sValue;
}; // End of CIniKeyA

class IniSection
{
	friend class NativeIniFile; // Allow CIniFileA to create sections
	friend struct ci_less_a;
public:
	struct ci_less_a;
	// Typedef of set of CIniKeyA pointers
	struct ci_less_a
	{
		bool operator() (const IniKey* s1, const IniKey* s2) const
		{
#ifndef _WIN32
			return strcasecmp(s1->m_sKeyName.c_str(), s2->m_sKeyName.c_str()) < 0;
#else
			return _stricmp(s1->m_sKeyName.c_str(), s2->m_sKeyName.c_str()) < 0;
#endif
		}
	};

	typedef std::set<IniKey*,ci_less_a> KeyIndexA;

	friend class IniKey;
private: // CIniSectionA acts as a class factory for CIniKeyA Objects
	IniSection( NativeIniFile* pIniFile , const std::string& sSectionName );
	IniSection( const IniSection& ); // No Copy
	IniSection& operator=(const IniSection&); // No Copy
	~IniSection( );
public:
	// Adds a key to the CIniSectionA object, returns a CIniKeyA pointer to the new or existing object
	IniKey* AddKey( std::string sKeyName );
	// Removes a single key by pointer
	void RemoveKey( IniKey* pKey );
	// Removes a single key by string
	void RemoveKey( std::string sKey );
	// Removes all the keys in the section
	void RemoveAllKeys( );
	// Returns a CIniKeyA pointer to the key by name, NULL if it was not found
	IniKey* GetKey( std::string sKeyName ) const;
	// Returns all keys in the section by KeyIndex only to be used for enumeration
	const KeyIndexA& GetKeys() const;
	// Returns a KeyValue at a certain section
	std::string GetKeyValue( std::string sKey ) const;
	// Sets a KeyValuePair at a certain section
	void SetKeyValue( std::string sKey, const std::string& sValue );
	// Sets the section name, returns true on success, fails if the section
	// name sSectionName already exists
	bool SetSectionName( std::string sSectionName );
	// Returns the section name
	std::string GetSectionName() const;
private:
	KeyIndexA::const_iterator _find_key( const std::string& sKeyName ) const;
	KeyIndexA::iterator _find_key( const std::string& sKeyName );
private:
	// CIniFileA pointer back to the object that instanciated the section
	NativeIniFile* m_pIniFile;
	// Name of the section
	std::string m_sSectionName;
	// List of CIniKeyA pointers ( Keys in the section )
	KeyIndexA m_keys;
}; // End of CIniSectionA

class NativeIniFile
{
public:
    static const char* const LF;
public:
    NativeIniFile();
    ~NativeIniFile();

    // Used to save the data back to the file or your choice
    bool Save( const NrpText& fileName );

    // Save data to an output stream
    void Save( std::ostream& output );

    // Loads the Reads the data in the ini file into the IniFile object
    bool Load( const NrpText& fileName , bool bMerge = false );

    // Load data from an input stream
    void Load( std::istream& input , bool bMerge = false );

public:
    class CIniMergeA
    {
    public:
        explicit CIniMergeA(NativeIniFile& ini):_ini(ini) {}
        std::istream &operator()(std::istream& input) const
        {
            _ini.Load( input , true );
            return input;
        }
    private:
        NativeIniFile& _ini;
    };
public:
    struct ci_less_a;
    // Typedef of a List of CIniSectionA pointers
    struct ci_less_a
    {
        bool operator() (const IniSection* s1, const IniSection* s2) const
        {
#ifndef _WIN32
            return strcasecmp(s1->m_sSectionName.c_str(), s2->m_sSectionName.c_str()) < 0;
#else
            return _stricmp(s1->m_sSectionName.c_str(), s2->m_sSectionName.c_str()) < 0;
#endif
        }
    };

    typedef std::set<IniSection*,ci_less_a> SecIndexA;

    friend class IniSection;
public:
    // Adds a section to the CIniFileA object, returns a CIniFileA pointer to the new or existing object
    IniSection* AddSection( std::string sSection );
    // Removes section by pointer
    void RemoveSection( IniSection* pSection );
    // Removes a section by its name sSection
    void RemoveSection( std::string sSection );
    // Removes all existing sections
    void RemoveAllSections( );
    // Returns a CIniSectionA* to the section by name, NULL if it was not found
    IniSection* GetSection( std::string sSection ) const;
    // Returns all sections in the inifile by SecIndex, only to be used for enumeration (DO NOT KEEP THE REF OR TRY TO DELETE STUFF!)
    const SecIndexA& GetSections() const;
    // Returns a KeyValue at a certain section
    std::string GetKeyValue( const std::string& sSection, const std::string& sKey ) const;
	IniKey* GetKey( const std::string& sSection, const std::string& sKey ) const;

    // Sets a KeyValuePair at a certain section
	void SetKeyValue( const std::string& sSection, const std::string& sKey, const std::string& sValue );

    // Renames an existing section returns true on success, false if the section didn't exist or there was another section with the same sNewSectionName
    bool RenameSection( const std::string& sSectionName  , const std::string& sNewSectionName );
    // Renames an existing key returns true on success, false if the key didn't exist or there was another section with the same sNewSectionName
    bool RenameKey( const std::string& sSectionName  , const std::string& sKeyName , const std::string& sNewKeyName);
private:
    SecIndexA::const_iterator _find_sec( const std::string& sSection ) const;
    SecIndexA::iterator _find_sec( const std::string& sSection );
private:
    NativeIniFile( const NativeIniFile&); // No Copy
    NativeIniFile& operator=(const NativeIniFile&); // No Copy
	// List of CIniSectionA pointers ( List of sections in the class )
    SecIndexA m_sections;
}; // End of CIniFileA

// Pointers
typedef NativeIniFile* PCINIA;
typedef IniKey* PCINIKEYA;
typedef IniSection* PCINISECA;

// Map Types
typedef IniSection::KeyIndexA KeyIndexA;
typedef NativeIniFile::SecIndexA SecIndexA;

std::ostream& operator<<(std::ostream& output, NativeIniFile& obj);
std::istream& operator>>(std::istream& input, NativeIniFile& obj);
std::istream& operator>>(std::istream& input, NativeIniFile::CIniMergeA merger);

#endif

