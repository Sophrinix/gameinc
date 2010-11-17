#pragma once

#include <irrMap.h>

using irr::core::map;

namespace nrp
{

class INrpConfig;
class KeyPair;
class IniFile;
class CNrpProperty;

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
	typedef void (CNrpConfigLooder::*SomeWriter)( const CNrpProperty*, const NrpText&, IniFile* );

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
	void _ReadUnknown( KeyPair* p );
	void _ReadDim2u( KeyPair* p );

	void _WriteInt( const CNrpProperty*, const NrpText&, IniFile* );
	void _WriteString( const CNrpProperty*, const NrpText&, IniFile* );
	void _WriteBool( const CNrpProperty*, const NrpText& , IniFile*  );
	void _WriteTime( const CNrpProperty*, const NrpText& , IniFile*  );
	void _WriteFloat( const CNrpProperty*, const NrpText&, IniFile* );
	void _WriteUser( const CNrpProperty*, const NrpText& , IniFile*  );
	void _WriteUnknown( const CNrpProperty*,  const NrpText& , IniFile*  );
	void _WriteTechnology( const CNrpProperty*, const NrpText&, IniFile* );
	INrpConfig* _config;
};
}//end namespace nrp