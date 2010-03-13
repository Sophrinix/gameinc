#pragma once
#include <luna.h>
#include <string>

class CLuaReport
{
public:
	static Luna<CLuaReport>::RegType methods[];

	//! Конструктор
	/*!
	\param указатель на реальный видео драйвер
	*/
	CLuaReport(lua_State *L);
	int SetFlags(lua_State *L);
	int AddFlag(lua_State *L);
	int RemFlag(lua_State *L);
	int SetUser(lua_State *L);
	int SetMessage(lua_State *L);
	int SetClass(lua_State *L);
	int SetType(lua_State *L);
	int SetTimeInterval(lua_State *L);
	int SetDateInterval(lua_State *L);
	int SetColumn(lua_State *L);
	int Open(lua_State *L);
	int Query(lua_State *L);
	int Close(lua_State *L);
	int Export(lua_State *L); //html, csv
	int SaveQuery(lua_State *L);
	int LoadQuery(lua_State *L);
	int GetFieldCount(lua_State* L);
	int GetFieldName(lua_State *L);
	int GetRowCount(lua_State *L);
	int NextRow(lua_State *L);
	int GetCellValue(lua_State *L);
	int IsOpen(lua_State *L);

	~CLuaReport(void);

	static const char* StaticGetLuaName() { return "CLuaReport"; }
private:
	CLuaReport(void) {}

	std::string db_, user_, password_, host_;
	bool isOpen_;
};
