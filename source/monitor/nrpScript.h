#ifndef NERPA_SCRIPT_H
#define NERPA_SCRIPT_H

#include "nrpConfig.h"
#include <stdexcept>
#include <string>

struct lua_State; //fwd

namespace nrp
{

const std::string LOAD_FUNCTIONS_FILENAME("functionsFilename");
const std::string SHOW_CALL_FUNCTION_NAME("showCallFunctionName");

struct CNrpScriptError : std::runtime_error
{
	CNrpScriptError(std::string file, size_t line, std::string msg)
		: std::runtime_error(msg),file_(file),line_(line) {}
	std::string file_;
	size_t line_;
};

class CNrpScript : public INrpConfig
{
	CNrpScript();
	~CNrpScript();
public:
	static CNrpScript& Instance();
	lua_State* GetVirtualMachine() { return vm_; }
	void DoFile( const char* file );
	void DoString( const char* s );
	void LoadFile( const char* fileName );
	void CallFunction( const char* funcName, void* userData=NULL );
	void CreateTemporaryScript( const std::string& fileName );
	void AddActionToTemporaryScript( const std::string& fileName, const std::string& action );
	
private:
	lua_State* vm_;

	void Load_( char* file_name );
	void RegisterLuaClasses_();
};

}//namespace nrp
#endif