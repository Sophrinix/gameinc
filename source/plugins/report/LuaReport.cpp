#include "StdAfx.h"
#include "LuaReport.h"
#include <WinSock.h>
#include <mysql.h>

// Глобальный (для этого модуля) хэндл базы данных. Вынести его в определение
// класса Logger не представляется возможным, так как для этого придется в Logger.h
// включать еще и winsock.h и mysql.h. Можно сделать pimpl, но лень.
static MYSQL mysql;
static MYSQL_RES *res;
static MYSQL_ROW row;

void HandleLoggerErrors(const char* fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	const int BUF_SIZE = 8192;
	char msgBuf[ BUF_SIZE ] = { 0 };

	// Посмотрим на ваши "секьюрные" функции, :)
	_vsnprintf_s(msgBuf, BUF_SIZE, BUF_SIZE, fmt, list);
	OutputDebugString("\n-------------------------------------------------------\n");
	OutputDebugString(msgBuf);
	OutputDebugString("\n-------------------------------------------------------\n");

	// TODO: Заменить MessageBox на что-нибудь более .
	MessageBox(0, msgBuf, "Logger Service Message", MB_ICONSTOP | MB_OK);
	va_end(list);
}

Luna< CLuaReport >::RegType CLuaReport::methods[] = 
{
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetFlags ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetUser ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, Open ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, Close ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetMessage ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetClass ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetType ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetTimeInterval ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetDateInterval ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, SetColumn ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, Export ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, IsOpen ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, AddFlag ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, RemFlag ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, GetFieldCount ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, GetFieldName ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, Query ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, GetCellValue ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, GetRowCount ),
	LUNA_AUTONAME_FUNCTION( CLuaReport, NextRow ),
	{0,0}
};

int CLuaReport::Open( lua_State *L )
{
	mysql_init( &mysql );
	mysql_thread_init();

	isOpen_=false;
	// Подключаемся к БД
	// Устанавливаем виндовую кодировку для сообщений для libmysql.
	isOpen_ = ( mysql_real_connect(&mysql, host_.c_str(), user_.c_str(), password_.c_str(), db_.c_str(), 3306, 0, 0) != NULL )
			 && !mysql_set_character_set( &mysql, "cp1251") ;

	lua_pushboolean( L, isOpen_ );
	return 1;
}

CLuaReport::CLuaReport( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 4, 4, "Function CLuaReport need 4 parameter");

	db_ = lua_tostring( L, 1 );
	user_ = lua_tostring( L, 2 );
	password_ = lua_tostring( L, 3 );
	host_ = lua_tostring( L, 4 );

	isOpen_ = true;
}

CLuaReport::~CLuaReport( void )
{
	int k=0;
}

int CLuaReport::RemFlag( lua_State *L )
{
	return 1;
}

int CLuaReport::AddFlag( lua_State *L )
{
	return 1;
}

int CLuaReport::IsOpen( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 1, 1, "Function CLuaReport:IsOpen not need any parameter");

	lua_pushboolean( L, isOpen_ );

	return 1;
}

int CLuaReport::Export( lua_State *L )
{
	return 1;
}

int CLuaReport::SetColumn( lua_State *L )
{
	return 1;
}

int CLuaReport::SetDateInterval( lua_State *L )
{
	return 1;
}

int CLuaReport::SetTimeInterval( lua_State *L )
{
	return 1;
}

int CLuaReport::SetType( lua_State *L )
{
	return 1;
}

int CLuaReport::SetClass( lua_State *L )
{
	return 1;
}

int CLuaReport::SetMessage( lua_State *L )
{
	return 1;
}

int CLuaReport::Close( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 1, 1, "Function CLuaReport:Close not need any parameter");

	mysql_free_result( res );
	mysql_close( &mysql );

	return 1;
}

int CLuaReport::SetUser( lua_State *L )
{
	return 1;
}

int CLuaReport::SetFlags( lua_State *L )
{
	return 1;
}

int CLuaReport::Query( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 1, 1, "Function CLuaReport:Query not need any parameter");
	bool result=true;

	if( mysql_query( &mysql, "SELECT class,type,event_date,description,user FROM logs" ) != 0 ) 
	{
		OutputDebugString( "Не могу найти таблицу событий\n" );
		result = false;
	}

	if( result && !(res = mysql_store_result(&mysql)) )
	{
		OutputDebugString( "Не могу обработать результаты запроса" );
		result = false;
	}
	
	lua_pushboolean( L, result );
	return 1;
}

int CLuaReport::GetFieldCount( lua_State* L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 1, 1, "Function CLuaReport:GetFieldCount not need any parameter");

	int k = ( res != NULL ? mysql_num_fields( res ) : 0 );
	lua_pushinteger( L, k );

	return 1;
}

int CLuaReport::GetFieldName( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 2, 2, "Function CLuaReport:GetFieldName need integer parameter");

	size_t pos = lua_tointeger( L, 2 );
	MYSQL_FIELD* fields = mysql_fetch_fields( res );
	std::string name = (fields != NULL ? fields[ pos ].name : "error");
	lua_pushstring( L, name.c_str() );

	return 1;
}

int CLuaReport::GetCellValue( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 2, 2, "Function CLuaReport:GetCellValue need integer parameter");

	size_t pos = lua_tointeger( L, 2 );
	lua_pushstring( L, (row != NULL ? row[ pos ] : "error") );

	return 1;
}

int CLuaReport::GetRowCount( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 1, 1, "Function CLuaReport:GetRowCount not need any parameter");

	size_t k = (res != NULL ? mysql_num_rows( res ) : 0);
	lua_pushinteger( L, k );

	return 1;
}

int CLuaReport::NextRow( lua_State *L )
{
	int argc = lua_gettop(L);
	luaL_argcheck( L, argc == 1, 1, "Function CLuaReport:NextRow not need any parameter");
	
	row = mysql_fetch_row( res );

	return 1;
}