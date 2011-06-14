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
//! Lua-обертка для работы с видеодрайвером
/*!
	Параметры комманд даны с lua-синтаксисом,
	Имена комманд lua и с++ совпадают
*/
class CLuaDriver : public ILuaObject<irr::video::IVideoDriver>
{
public:
	DEFINE_PROPERTIES_AND_METHODS(CLuaDriver)
	static const char* ClassName();

	//! Конструктор
	/*!
		\param указатель на реальный видео драйвер
	*/
	CLuaDriver(lua_State *L, bool ex);

	//! Выдает размеры экрана в формате ширина-высота
	int GetScreenSize( lua_State *L );

	//! Выдает указатель на текстуру, NULL в случае ошибки
	/*!
		\param textureName имя текстуры
	*/
	int GetTexture( lua_State *L );

	//! Размещает в памяти текстуру с заданными параметрами, описание пиксела - 4 байта
	/*!
		\param width	ширина текстуры
		\param height	высота текстуры
		\param name		имя текстуры
	*/
	int AddTexture( lua_State *L );

	//! Удаляет текстуру с указанным именем из памяти
	/*!
		\param name  имя текстуры
	*/
	int RemoveTexture( lua_State *L );

	int AddRenderTargetTexture( lua_State* L );

	int SetRenderTarget( lua_State* L );
	int CreateBlur( lua_State* L );
    int CreateGrayscale( lua_State* L );
};

}//namespace nrp