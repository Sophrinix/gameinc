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
//! Lua-������� ��� ������ � ��������������
/*!
	��������� ������� ���� � lua-�����������,
	����� ������� lua � �++ ���������
*/
class CLuaDriver : public ILuaObject<irr::video::IVideoDriver>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaDriver)
	static const char* ClassName();

	//! �����������
	/*!
		\param ��������� �� �������� ����� �������
	*/
	CLuaDriver(lua_State *L, bool ex);

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
	int CreateBlur( lua_State* L );
    int CreateGrayscale( lua_State* L );
};

}//namespace nrp