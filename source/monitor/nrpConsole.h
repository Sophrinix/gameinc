#ifndef _IrrConsole_Console_h_
#define _IrrConsole_Console_h_

#include "nrpCommandDispatcher.h"
#include "nrpMessageSink.h"
#include "nrpConsoleUtils.h"
#include <vector>
#include <string>
#include <irrlicht.h>

namespace irr
{

namespace gui
{

const int EGUIET_CONSOLE=EGUIET_COUNT+1;

class CNrpConsole : public gui::IGUIElement, public core::CCommandDispatcher, public core::CMessageSink
{
public:
	CNrpConsole( IGUIEnvironment* env, IGUIElement* parent, s32 id, core::recti& rectangle );															//! constructor
	virtual ~CNrpConsole();													//! destructor	

	void ToggleVisible();

	void draw();

	void AppendMessage( const core::stringw& message );						//добавление строки к консоли
	void ClearMessages();													//очистка сообщений
	const c8* getTypeName() const { return "nrpConsole"; }

	void KeyPress( const irr::SEvent& event );								//передача нажатий клавы в обработчик консоли

private:
	void HandleCommandString( const core::stringw& wstr);							//парсер комманд
	void AddToHistory( const core::stringw& line);								//добавление комманды к истории
	void CalculateConsoleRect(const core::dimension2du& screenSize); //вычисление параметров отображения консоли
	void CalculatePrintRects(core::recti& textRect, core::recti& shellRect);
	bool CalculateLimits(u32& maxLines, u32& lineHeight,s32& fontHeight);	//обработчик изменения размера
	void ResizeMessages();
	void TabComplete();														//обработчик таба
	void SetNextCommand_();
	void SetPrevCommand_();
	void ResolveCommand_();
	void InputChar_( wchar_t key_char, bool shift_down );
	void RegisterDefaultCommands_();					//загрузка комманд
	void MoveCursor_( bool leftStep );
	
	CRITICAL_SECTION cs_dataaccess_;	

	std::vector< core::stringw > console_messages_;								//что выводится на консоль		
	std::vector< core::stringw > console_history_;								//история комманд
	u32 consoleHistoryIndex_;

	typedef enum { NONE=0, UPLIGTH, DOWNLIGTH } TOGGLE_TYPE;
	TOGGLE_TYPE toggle_visible_;

	core::stringw currentCommand_;												//текущая команда	
	u32 cursorPos_;

	void SaveCommands_();
	void LoadSaveCommands_();
};
//////////////////////////////////////////////////////////////////////////

}//namespace gui

}//namespace irr

#endif