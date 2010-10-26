#pragma once

#include <map>

namespace nrp
{

class INrpConfig;
class KeyPair;
class INrpProperty;

class CNrpConfigLooder
{
public:
	CNrpConfigLooder(INrpConfig* ptrConfig);
	~CNrpConfigLooder(void);

	void Save( const std::string& fileName );
	void Load( const std::string& fileName );

private:
	CNrpConfigLooder(void);
	

	typedef void (CNrpConfigLooder::*SomeReader)( KeyPair* );
	typedef void (CNrpConfigLooder::*SomeWriter)( const INrpProperty*, const std::string&, 
												  const std::string& );

	typedef std::map< std::string, SomeReader > READERS_MAP;
	typedef std::map< std::string, SomeWriter > WRITERS_MAP;

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

	void _WriteInt( const INrpProperty*, const std::string&, const std::string& );
	void _WriteString( const INrpProperty*, const std::string&, const std::string& );
	void _WriteBool( const INrpProperty*, const std::string& , const std::string&  );
	void _WriteTime( const INrpProperty*, const std::string& , const std::string&  );
	void _WriteFloat( const INrpProperty*, const std::string& , const std::string&  );
	void _WriteUser( const INrpProperty*, const std::string& , const std::string&  );
	void _WriteUnknown( const INrpProperty*,  const std::string& , const std::string&  );
	INrpConfig* _config;
};
}//end namespace nrp