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
//! Luа-обертка для работы с CGUIImage
/*!
	Параметры комманд даны с lua-синтаксисом,
	Имена комманд lua и с++ совпадают
*/
class CLuaImage : public ILuaGuiElement<irr::gui::IGUIImage>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaImage)

	//! Конструктор
	/*!
		\param width	ширина изображения
		\param height	высота изображения
		\param name		имя элемента
	*/
	CLuaImage(lua_State *L, bool);

	//! Установка текстуры для отображения
	/*!
		\param ptrTexture указатель на реальную текстуру
	*/
	int SetImage( lua_State *L );
	int SetScaleImage( lua_State *L );
	int SetUseAlphaChannel( lua_State* L );
	//************************************
	// Method:    ClassName
	// FullName:  nrp::CLuaImage::ClassName
	// Access:    public static 
	// Returns:   const char*
	// Qualifier:
	//************************************
	static const char* ClassName();							
};

}//namespace nrp