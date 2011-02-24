#ifndef NERPA_SCRIPT_CORE_H
#define NERPA_SCRIPT_CORE_H

struct lua_State; //fwd

namespace nrp
{

// ����� C-���� �������� ��� ������������� �������� ������ � Lua
typedef struct
{
	// ������ �������
	int size;
	// ��������� �� ������ � ������� (�_�)
	double values[1];
} NumArray;

// ����� C++-���������� �� ������� �����
extern "C" 
{
	NumArray *check(lua_State *vm, int index);
	double &getvalue(lua_State *vm);
	int NewArray(lua_State *vm);
	int SetArray(lua_State *vm);
	int GetArray(lua_State *vm);
	int GetSize(lua_State *vm);
	int tostring(lua_State *vm);
	
	// ���������������� lua-���������
	//
	// lua-backend ���������� �� �������
	// �������� �� ����������
	int AboutRobots(lua_State *vm);

	// lua-backend ������� ���� �������� - ����
	// �������� �� ����������
	int SocketOpen(lua_State *vm);

	// lua-backend ������� ���� �������� - ����
	// �������� �� ����������
	int SocketClose(lua_State *vm);

	int CreateTerrainShader( lua_State *vm );

	int GetGuiEnvironment( lua_State *vm );

	int GetSceneManager( lua_State *vm );

	int GetVideoDriver( lua_State *vm );

	// ���� �������
	// ���������� ��������� � ���
	int ScriptLog( lua_State *vm );

	int AddScene( lua_State* vm );

	int SetNextScene( lua_State* vm );

	int InitializeLoginScene( lua_State* vm );

	int InitializeWorkScene( lua_State* vm );

	int SetSceneOption( lua_State* vm );

	int LoadPlugins( lua_State* vm );

	int GetApplication( lua_State* vm );

	int ApplicationClose( lua_State* vm );

	int ApplicationSave( lua_State* vm );

	int GetBrowser( lua_State *vm );

	int ApplicationGetTranslate( lua_State* vm );

	int ApplicationGetSender( lua_State* vm );

	// ���� �������
	int Help(lua_State *vm);

	int ApplicationDumpStack( lua_State *vm );
};

}//namespace nrp
#endif