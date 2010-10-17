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
CLASS_NAME CLASS_LUAIMAGE("CLuaImage");
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
								
	virtual std::string ObjectName() { return CLASS_LUAIMAGE; }
	static const char* ClassName() { return CLASS_LUAIMAGE.c_str(); }
	
};

}//namespace nrp