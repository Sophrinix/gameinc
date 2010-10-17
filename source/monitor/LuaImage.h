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
//! Luа-обертка для работы с CGUIImage
/*!
	Параметры комманд даны с lua-синтаксисом,
	Имена комманд lua и с++ совпадают
*/
class CLuaImage : public ILuaGuiElement<irr::gui::IGUIImage>
{
public:
	static Luna<CLuaImage>::RegType methods[];				

	//! Конструктор
	/*!
		\param width	ширина изображения
		\param height	высота изображения
		\param name		имя элемента
	*/
	CLuaImage(lua_State *L);

	//! Установка текстуры для отображения
	/*!
		\param ptrTexture указатель на реальную текстуру
	*/
	int SetImage( lua_State *L );
	int SetScaleImage( lua_State *L );
	int SetUseAlphaChannel( lua_State* L );
								
	virtual std::string ObjectName() { return CLASS_LUAIMAGE; }
	static const char* ClassName() { return CLASS_LUAIMAGE.c_str(); }
	
};

}//namespace nrp