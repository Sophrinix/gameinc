#pragma once

#include <irrMap.h>

using irr::core::map;

class IniKey;

namespace nrp
{

class INrpConfig;
class KeyPair;
class IniFile;
class NParam;

class CNrpConfigLooder
{
public:
	CNrpConfigLooder(INrpConfig* ptrConfig);
	~CNrpConfigLooder(void);

	void Save( const NrpText& fileName );
	void Load( const NrpText& fileName );

private:
	CNrpConfigLooder(void);
	
	typedef void (CNrpConfigLooder::*SomeReader)( IniKey* );
	typedef void (CNrpConfigLooder::*SomeWriter)( const NParam&, const NrpText&, IniFile& );

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