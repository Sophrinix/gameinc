#ifndef NERPA_SCRIPT_H
#define NERPA_SCRIPT_H

#include "nrpConfig.h"
#include <stdexcept>
#include <string>
#include <typeinfo>

struct lua_State; //fwd

namespace nrp
{
CLASS_NAME CLASS_NRPSCRIPT( "CNrpScript" );

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
	typedef enum { SA_CREATE, SA_EXEC } SCRIPT_ACTION;

	static CNrpScript& Instance();
	lua_State* GetVirtualMachine() { return vm_; }
	void DoFile( const char* file );
	void DoString( const char* s );
	void LoadFile( const char* fileName );
	void CallFunction( const char* funcName, void* userData=NULL );
	void TemporaryScript( const std::string& fileName, SCRIPT_ACTION action );
	void AddActionToTemporaryScript( const std::string& fileName, const std::string& action );
	void SetSender( void* ptr ) { _sender = ptr; }
	void* GetSender() { return _sender; }
	
	static std::string ClassName() { return CLASS_NRPSCRIPT; }
private:
	lua_State* vm_;
	void* _sender;

	void Load( const std::string& sectionName, const std::string& fileName );
	void Save( const std::string& scetionName, const std::string& fileName ) {};

	void RegisterLuaClasses_();
};

}//namespace nrp
#endif