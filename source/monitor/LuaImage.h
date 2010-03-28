#pragma once

#include "ILuaGuiElement.h"

namespace irr
{
	namespace gui
	{
		class IGUIImage;
	}
}

namespace nrp
{

//! Lu�-������� ��� ������ � CGUIImage
/*!
	��������� ������� ���� � lua-�����������,
	����� ������� lua � �++ ���������
*/
class CLuaImage : public ILuaGuiElement<irr::gui::IGUIImage>
{
public:
	static Luna<CLuaImage>::RegType methods[];				

	//! �����������
	/*!
		\param width	������ �����������
		\param height	������ �����������
		\param name		��� ��������
	*/
	CLuaImage(lua_State *L);

	//! ��������� �������� ��� �����������
	/*!
		\param ptrTexture ��������� �� �������� ��������
	*/
	int SetImage( lua_State *L );
	int SetScaleImage( lua_State *L );
	int SetUseAlphaChannel( lua_State* L );
																
	static const char* StaticGetLuaName() { return "CLuaImage"; }
	
};

}//namespace nrp