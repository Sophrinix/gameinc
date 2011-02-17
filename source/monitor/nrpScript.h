#ifndef NERPA_SCRIPT_H
#define NERPA_SCRIPT_H

#include "nrpConfig.h"
#include <stdexcept>
#include <typeinfo>

struct lua_State; //fwd

namespace nrp
{
OPTION_NAME LOAD_FUNCTIONS_FILENAME(L"functionsFilename");
OPTION_NAME SHOW_CALL_FUNCTION_NAME(L"showCallFunctionName");

struct CNrpScriptError : std::runtime_error
{
	CNrpScriptError(std::string file, size_t line, std::string msg)
		: std::runtime_error( msg ), file_(file),line_(line) {}

	std::string file_;
	size_t line_;
};

class CNrpScript : public INrpConfig
{
	CNrpScript();
	~CNrpScript();

public:
	typedef enum { SA_CREATE, SA_EXEC } SCRIPT_ACTION;

	static CNrpScript& Instance();
	lua_State* GetVirtualMachine() { return vm_; }
	void DoFile( const NrpText& file );
	void DoString( const NrpText& s );
	void LoadFile( const NrpText& fileName );
	void CallFunction( const NrpText& funcName, void* sender=NULL, void* userData=NULL );
	void CallFunction( int funcRef, void* sender=NULL, void* userData=NULL  );
	void TemporaryScript( const NrpText& fileName, SCRIPT_ACTION action );
	void AddActionToTemporaryScript( const NrpText& fileName, const NrpText& action );
	void ReleaseRef( int action );
	
	static NrpText ClassName();
private:
	lua_State* vm_;

	void RegisterLuaClasses_();
};

}//namespace nrp
#endif