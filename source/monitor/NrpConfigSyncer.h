#pragma once

#include <irrMap.h>
#include "SectionNames.h"

using irr::core::map;

class IniKey;

namespace nrp
{

class INrpConfig;
class KeyPair;
class IniFile;
class NParam;

class CNrpConfigSyncer
{
public:
    static const NrpText t_int;
    static const NrpText t_float;
    static const NrpText t_bool;
    static const NrpText t_string;
    static const NrpText t_time;
    static const NrpText t_user;
    static const NrpText t_tech;
    static const NrpText t_enum; 
    static const NrpText t_unknown;
    static const NrpText t_dim2u;
    static const NrpText t_path;

	CNrpConfigSyncer(INrpConfig* ptrConfig);
	~CNrpConfigSyncer(void);

	void Save( const NrpText& fileName, SECTION_NAME& section=SECTION_PROPERTIES );
    void Save( IniFile& ini, SECTION_NAME& section );
    void Load( const NrpText& fileName, SECTION_NAME& section=SECTION_PROPERTIES );
	void Load( IniFile&  ini, SECTION_NAME& section );

private:
	CNrpConfigSyncer(void);
	
	typedef void (CNrpConfigSyncer::*SomeReader)( IniKey* );
	typedef void (CNrpConfigSyncer::*SomeWriter)( const NParam&, const NrpText&, IniFile& );

	typedef map< NrpText, SomeReader > READERS_MAP;
	typedef map< NrpText, SomeWriter > WRITERS_MAP;

	READERS_MAP _readers;
	WRITERS_MAP _writers;

	void _InitReaders();
	void _InitWriters();

	void _ReadFloat( IniKey* p );
	void _ReadBool( IniKey* p );
	void _ReadInt( IniKey* );
	void _ReadUser( IniKey* p );
	void _ReadTime( IniKey* p );
	void _ReadString( IniKey* p );
	void _ReadTechnology( IniKey* p );
	void _ReadUnknown( IniKey* p );
	void _ReadDim2u( IniKey* p );
	void _ReadPath( IniKey* p );

	void _WriteInt( const NParam&, const NrpText&, IniFile& );
	void _WriteString( const NParam&, const NrpText&, IniFile& );
	void _WriteBool( const NParam&, const NrpText& , IniFile&  );
	void _WriteTime( const NParam&, const NrpText& , IniFile&  );
	void _WriteFloat( const NParam&, const NrpText&, IniFile& );
	void _WriteUser( const NParam&, const NrpText& , IniFile&  );
	void _WriteUnknown( const NParam&,  const NrpText& , IniFile&  );
	void _WriteTechnology( const NParam&, const NrpText&, IniFile& );
	INrpConfig* _config;
	NrpText _fileName;
};
}//end namespace nrp