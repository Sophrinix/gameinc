#pragma once

#include "ILuaObject.h"

namespace irr
{
	namespace video
	{
		class IVideoDriver;
	}
}

namespace nrp
{
CLASS_NAME CLASS_LUADRIVER( "CLuaDriver" );

//! Lua-������� ��� ������ � ��������������
/*!
	��������� ������� ���� � lua-�����������,
	����� ������� lua � �++ ���������
*/
class CLuaDriver : public ILuaObject<irr::video::IVideoDriver>
{
public:
	static Luna<CLuaDriver>::RegType methods[];

	//! �����������
	/*!
		\param ��������� �� �������� ����� �������
	*/
	CLuaDriver(lua_State *L);

	//! ������ ������� ������ � ������� ������-������
	int GetScreenSize( lua_State *L );

	//! ������ ��������� �� ��������, NULL � ������ ������
	/*!
		\param textureName ��� ��������
	*/
	int GetTexture( lua_State *L );

	//! ��������� � ������ �������� � ��������� �����������, �������� ������� - 4 �����
	/*!
		\param width	������ ��������
		\param height	������ ��������
		\param name		��� ��������
	*/
	int AddTexture( lua_State *L );

	//! ������� �������� � ��������� ������ �� ������
	/*!
		\param name  ��� ��������
	*/
	int RemoveTexture( lua_State *L );

	int AddRenderTargetTexture( lua_State* L );

	int SetRenderTarget( lua_State* L );

	static const char* ClassName() { return CLASS_LUADRIVER.c_str(); }
};

}//namespace nrp