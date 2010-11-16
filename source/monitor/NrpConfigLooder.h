#pragma once

#include <irrMap.h>

using irr::core::map;

namespace nrp
{

class INrpConfig;
class KeyPair;
class IniFile;
class INrpProperty;

class CNrpConfigLooder
{
public:
	CNrpConfigLooder(INrpConfig* ptrConfig);
	~CNrpConfigLooder(void);

	void Save( const NrpText& fileName );
	void Load( const NrpText& fileName );

private:
	CNrpConfigLooder(void);
	
	typedef void (CNrpConfigLooder::*SomeReader)( KeyPair* );
	typedef void (CNrpConfigLooder::*SomeWriter)( const INrpProperty*, const NrpText&, IniFile* );

	typedef map< NrpText, SomeReader > READERS_MAP;
	typedef map< NrpText, SomeWriter > WRITERS_MAP;

	READERS_MAP _readers;
	WRITERS_MAP _writers;

	void _InitReaders();
	void _InitWriters();

	void _ReadFloat( KeyPair* p );
	void _ReadBool( KeyPair* p );
	void _ReadInt( KeyPair* );
	void _ReadUser( KeyPair* p );
	void _ReadTime( KeyPair* p );
	void _ReadString( KeyPair* p );
	void _ReadTechnology( KeyPair* p );

	void _WriteInt( const INrpProperty*, const NrpText&, IniFile* );
	void _WriteString( const INrpProperty*, const NrpText&, IniFile* );
	void _WriteBool( const INrpProperty*, const NrpText& , IniFile*  );
	void _WriteTime( const INrpProperty*, const NrpText& , IniFile*  );
	void _WriteFloat( const INrpProperty*, const NrpText&, IniFile* );
	void _WriteUser( const INrpProperty*, const NrpText& , IniFile*  );
	void _WriteUnknown( const INrpProperty*,  const NrpText& , IniFile*  );
	void _WriteTechnology( const INrpProperty*, const NrpText&, IniFile* );
	INrpConfig* _config;
};
}//end namespace nrp