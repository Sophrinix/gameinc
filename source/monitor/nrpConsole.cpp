#include "stdafx.h"
#include "nrpConsole.h"
#include "nrpConsoleCmds.h"
#include "NrpText.h"
#include "nrpVideoConfig.h"
#include "IniFile.h"
#include "NrpConsoleConfig.h"
#include "nrpEngine.h"
#include "AlignmentNames.h"

using namespace nrp;

namespace irr
{

namespace gui
{

CNrpConsole::CNrpConsole( IGUIEnvironment* env, IGUIElement* parent, s32 id, core::recti& rectangle ) 
			: IGUIElement( EGUI_ELEMENT_TYPE(EGUIET_CONSOLE), env, parent, id, rectangle ),
							 consoleHistoryIndex_(0),
							 toggle_visible_(NONE)//! constructor
{
	setName( CNrpConsoleConfig::Instance()[ CONSOLE_GUINAME ].As<NrpText>().ToWide() );

	InitializeCriticalSectionAndSpinCount(&cs_dataaccess_, 1000);									//критическая секция для доступа к сообщениям консоли
	
	CalculateConsoleRect( env->getVideoDriver()->getScreenSize() );										//calculate the console rectangle

	AppendMessage( NrpText( L"NerpaConsole initialized" ) );								//append a message

	ResizeMessages();														//resize message array

	RegisterDefaultCommands_();

	IsVisible = false;
	AlphaBlend = 3;
	cursorPos_ = 1;
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::SaveCommands_()																	//сохранение введенных комманд
{					
	//при выходе из программы
	NrpText path = CNrpConsoleConfig::Instance()[ CONSOLE_COMMANDS_FILE ];							//путь к файлу		

	try
	{
		IniFile sv( path );
		sv.Set( "commands", "number_commands", (int)console_history_.size() );	//сколько команд будет записано

		for( u32 cnt=0; cnt < console_history_.size(); cnt++ )							//скидываем комманды на диск
		{
			sv.Set( "commands", NrpText("type_") + (int)cnt, NrpText( console_history_[ cnt ] ) ); 
		}	
	}
	catch(...)
	{
#ifdef _DEBUG
		Log(HW) << "Can't write command to " << path << term;
#endif
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::LoadSaveCommands_()													//загрузка комманд консоли
{
	NrpText path = CNrpConsoleConfig::Instance()[ CONSOLE_COMMANDS_FILE ];

	IniFile sv( path );
	int num_com = sv.Get( "commands", "number_commands", (int)0 );

	for( int cnt=0; cnt < num_com; cnt++ )
	{
		NrpText strk = sv.Get( "commands", NrpText("type_") + NrpText( cnt ), NrpText( "null" ) );								//если команды не она будет отображена ка "null"

		console_history_.push_back( strk );
	}	
}
//////////////////////////////////////////////////////////////////////////

CNrpConsole::~CNrpConsole()													//! destructor
{
	SaveCommands_();
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::RegisterDefaultCommands_()			//! loads a few default commands into the console
{
	LoadSaveCommands_();
	
	RegisterCommand(new core::IC_Command_ECHO());
	RegisterCommand(new core::IC_Command_HELP());
	RegisterCommand(new core::IC_Command_LIST());
	RegisterCommand(new core::IC_Command_INFO( _nrpEngine.GetDevice() ) );
	RegisterCommand(new core::IC_Command_ACTIONDATA() );
	RegisterCommand(new core::IC_Command_SCRIPT() );
	RegisterCommand(new core::IC_Command_UPDATE() );
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::ResizeMessages()											//! resize the message count
{
	u32 maxLines = 0;
	u32 lineHeight = 0;
	s32 fontHeight = 0;
	if(CalculateLimits(maxLines,lineHeight,fontHeight))						//вычислям сколько сообщения помещаяется в консоли
	{
		u32 messageCount = console_messages_.size();	
		if(messageCount > maxLines)											//остальные удаляем
			console_history_.erase( 0, messageCount - maxLines );
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::ToggleVisible()											//! toggle the visibility of the console
{
	toggle_visible_ = IsVisible ? DOWNLIGTH : UPLIGTH;
	IsVisible = true;
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::AppendMessage( const core::stringw& message )					//добавление сообщения в консоль
{
	EnterCriticalSection( &cs_dataaccess_ );

	try
	{
		console_messages_.push_back( message );								//после добавления надо обновить список сообщений

		ResizeMessages();
	}
	catch(...)
	{
		//ErrLog(gfx) << all << "Ошибка размещения сообщения в консоли " << term;
	}

	LeaveCriticalSection( &cs_dataaccess_ );
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::ClearMessages()											//! clear all the messages in the sink
{
	console_messages_.clear();
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::draw()
{
	video::IVideoDriver *driver = Environment->getVideoDriver();
	CNrpConsoleConfig& conf = CNrpConsoleConfig::Instance();
	gui::IGUIFont* font = Environment->getFont( conf[ CONSOLE_FONTNAME ].As<NrpText>().ToStr() );

	if( IsVisible )															// render only if the console is visible
	{
		if( toggle_visible_ != NONE )
		{
			if( toggle_visible_ == DOWNLIGTH )
			{
				if( AlphaBlend > 5 ) AlphaBlend-=3;
				else IsVisible = false;
			}
			else
			{
				if( (int)AlphaBlend < (int)conf[ CONSOLE_MAX_BLEND ] )	AlphaBlend+=3;
				else toggle_visible_ = NONE;
			}
		}

		if( (bool)conf[ CONSOLE_SHOW_BACKGROUND ] )											//if bg is to be drawn fill the console bg with color
		{
			video::SColor color = conf[ CONSOLE_BG_COLOR ].As< video::SColor >();
			color.setAlpha( AlphaBlend );
			driver->draw2DRectangle( color, AbsoluteRect );	//draw the bg as per configured color
		}
		
		core::recti textRect,shellRect;										//we calculate where the message log shall be printed and where the prompt shall be printed
		CalculatePrintRects(textRect,shellRect);

		u32 maxLines, lineHeight;											//now, render the messages
		s32 fontHeight=0;													//будет рассчитано дальше
		if(!CalculateLimits(maxLines,lineHeight,fontHeight))
		{
			return;
		}
		
		core::recti lineRect( textRect.UpperLeftCorner.X,						//calculate the line rectangle
							  textRect.UpperLeftCorner.Y,
							  textRect.LowerRightCorner.X,
							  textRect.UpperLeftCorner.Y + lineHeight);

		EnterCriticalSection( &cs_dataaccess_ );
		video::SColor fontcolor = conf[ CONSOLE_FONT_COLOR ].As<video::SColor>();
		fontcolor.setAlpha( AlphaBlend );

		try
		{
			for(u32 i = 0; i < console_messages_.size(); i++)
			{
				font->draw( console_messages_[i].c_str(), 
					 		lineRect, 
							fontcolor, 
							false, true,
							&AbsoluteRect );									//we draw each line with the configured font and color vertically centered in the rectangle

				lineRect.UpperLeftCorner.Y += lineHeight;						//update line rectangle
				lineRect.LowerRightCorner.Y += lineHeight;
			}
		}
		catch(...)
		{
			//ErrLog(gfx) << all << "Ошибка отрисовки сообщений консоли" << term;
		}

		LeaveCriticalSection( &cs_dataaccess_);

		NrpText shellText = conf[ CONSOLE_PROMT ];	//now, render the prompt
		shellText.append( L"$>" );
		size_t textSize = shellText.size();
		shellText.append( currentCommand_ );
		
		font->draw(   shellText.ToWide(),
			          shellRect,
				      fontcolor,
					  false, false, 
					  &AbsoluteRect );											//draw the prompt string

		if( ( GetTickCount() % 700 ) < 350 ) 
		{
			core::dimension2du pos = font->getDimension( shellText.subString( 0, textSize + cursorPos_ - 1 ).c_str() );
			font->draw( L"_", core::recti( pos.Width , 0, pos.Width + 20, pos.Height ) + shellRect.UpperLeftCorner,
						0xffff0000,
						false, false, &AbsoluteRect);
		}		
	}

	IGUIElement::draw();
}

void CNrpConsole::ResolveCommand_()											//обработчик кнопки Enter
{
	AddToHistory( currentCommand_ );											//добавление команды в историю
	HandleCommandString( currentCommand_ );									//выполнение команды
	currentCommand_ = L"";
	consoleHistoryIndex_ = 0;
	cursorPos_ = 1;
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::SetPrevCommand_()
{
	if(console_history_.size() > 0)
	{
		s32 index = console_history_.size() - 1 - consoleHistoryIndex_;
		if(index >= 0 && index < static_cast< s32 >( console_history_.size() ) )
		{
			consoleHistoryIndex_++;
			currentCommand_ = console_history_[index].c_str();
			cursorPos_ = currentCommand_.size() + 1;
		}
		else
		{
			consoleHistoryIndex_ = 0;
		}
	}
	else
	{
		consoleHistoryIndex_ = 0;
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::SetNextCommand_()
{
	if(console_history_.size() > 0)
	{

		s32 index = console_history_.size() - consoleHistoryIndex_;
		if(index >= 0 && index < static_cast< s32 >( console_history_.size() ) )
		{
			consoleHistoryIndex_--;
			currentCommand_ = console_history_[index].c_str();
			cursorPos_ = currentCommand_.size() + 1;
		}
		else
		{
			consoleHistoryIndex_ = console_history_.size() - 1;
		}
	}
	else
	{
		consoleHistoryIndex_ = 0;
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::InputChar_( wchar_t key_char, bool shift_down )		//обработка нажатий клавиатуры
{
	if(key_char)
	{
		wchar_t buf[2];
		buf[0] = key_char;
		buf[1] = 0;
		core::stringw astr = buf;
		
		if(shift_down)
			astr.make_upper();
		
		currentCommand_ = currentCommand_.subString( 0, cursorPos_-1 ) + astr + currentCommand_.subString( cursorPos_-1, 0xff );
		cursorPos_++;
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::KeyPress( const irr::SEvent& event )							//обработчик событий консоли
{
	if( event.KeyInput.Key == irr::KEY_RETURN )
	{
		if( currentCommand_.size() > 0 )
			ResolveCommand_(); 	
	}
	else if( event.KeyInput.PressedDown )
			switch ( event.KeyInput.Key )
			{
				case irr::KEY_BACK:	
							if( currentCommand_.size() > 0 && cursorPos_ > 1 )
							{	
								cursorPos_--;
								currentCommand_.erase( cursorPos_-1 );
							}//удаление букв 
							break;
				case irr::KEY_DELETE:
				  			if( cursorPos_ <= currentCommand_.size() )
							{
								 currentCommand_.erase( cursorPos_-1 );
							}
							break;
				case irr::KEY_UP:									//перемещение по истории
							SetPrevCommand_();
							break;
				case irr::KEY_LEFT:
				case irr::KEY_RIGHT:
							MoveCursor_( event.KeyInput.Key == irr::KEY_LEFT );
							break;
				case irr::KEY_DOWN:
							SetNextCommand_();
							break;
				case irr::KEY_TAB:									//автозаполнение комманда
							TabComplete();
							break;
				default:
							InputChar_( event.KeyInput.Char, event.KeyInput.Shift );
						    break;
			}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::HandleCommandString( const core::stringw& wstr)						//! handle the current command string
{
	if(wstr.size() > 0 )							//check if it is a command
	{
		core::stringw cmdLine = wstr;

		//append the message
		core::stringw msg = L">> Command : ";
		msg += cmdLine;
		AppendMessage( msg );

		//parsing logic
		
		core::array< core::stringw > args;
		core::stringw cmdName;
		CNrpConsoleParser parser(cmdLine);
		if( parser.parse(cmdName,args) )
		{		
			Dispatch(cmdName,args,this);
		}
		else
		{
			core::stringw errorMessage = L"The command syntax is incorrect or it could not be parsed";
			LogError(errorMessage);
		}
	}
	else
	{
		/*try
		{
			//AppendMessage( wstr );												//add to console
			//Glukensoft::NerpaScript& scripter = CNrpApplication::GetInstance().GetScriptEngine();	
			//scripter.DoString( K2E_StrConv::ToString( wstr.c_str() ) );
		}
		catch (Glukensoft::NerpaScriptError& e)
		{
			AppendMessage( NrpText( e.what() ) );												//add to console
		}
		catch (...)
		{
			//здесь ничего не делаем, учимся писать софт.
		}*/
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::AddToHistory( const core::stringw& wstr)								//! add to history and readjust history
{
	for( size_t cnt=0; cnt < console_history_.size(); cnt++ )						//если команда уже есть в истории
		if( console_history_[ cnt ] == wstr )										//больше не надо её добавлять
			return;
 
	if( console_history_.size() >= (int)CNrpConsoleConfig::Instance()[CONSOLE_HISTORY_SIZE] )
		console_history_.erase( 0 );

	console_history_.push_back( wstr.c_str() );
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::CalculateConsoleRect(const irr::core::dimension2du& screenSize)	//! calculate the whole console rect
{
	core::recti console_rect;
	CNrpConsoleConfig& conf = CNrpConsoleConfig::Instance();
	core::dimension2df scr_rat = conf[CONSOLE_RELATIVE_SIZE].As< core::dimension2df >();

	if( scr_rat.Width == 0 || scr_rat.Height == 0)
	{
		console_rect = core::recti(0,0,0,0);
	}
	else
	{
		core::dimension2du consoleDim = Environment->getVideoDriver()->getScreenSize();									//calculate console dimension

		consoleDim.Width = (s32)(consoleDim.Width  * scr_rat.Width );
		consoleDim.Height= (s32)(consoleDim.Height * scr_rat.Height );

		ELEMENT_ALIGN al_v = conf[ CONSOLE_VERT_ALIGN ].As<NrpText>();
		if( al_v == ALIGN_TOP )									//set vertical alignment
		{
			console_rect.UpperLeftCorner.Y = 0;
		}
		else if( al_v== ALIGN_BOTTOM )
		{
			console_rect.UpperLeftCorner.Y = screenSize.Height - consoleDim.Height;
		}
		else if( al_v == ALIGN_MIDDLE )
		{
			console_rect.UpperLeftCorner.Y = (screenSize.Height - consoleDim.Height) / 2; 
		}
	
		ELEMENT_ALIGN al_h = conf[ CONSOLE_HORT_ALIGN ].As<NrpText>();
		if( al_h == ALIGN_LEFT )								//set horizontal alignment
		{
			console_rect.UpperLeftCorner.X = 0;
		}
		else if( al_h == ALIGN_RIGTH )
		{
			console_rect.UpperLeftCorner.X = screenSize.Width - consoleDim.Width;
		}
		else if( al_h == ALIGN_CENTER )
		{
			console_rect.UpperLeftCorner.X = (screenSize.Width - consoleDim.Width) / 2; 
		}
		
		console_rect.LowerRightCorner.X = console_rect.UpperLeftCorner.X + consoleDim.Width;	 		//set the lower right corner stuff
		console_rect.LowerRightCorner.Y = console_rect.UpperLeftCorner.Y + consoleDim.Height;

		setRelativePosition( console_rect );
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::CalculatePrintRects( core::recti& textRect, core::recti& shellRect)  //! calculate the messages rect and prompt / shell rect
{
	u32 maxLines, lineHeight;
	s32 fontHeight;

	if( CalculateLimits(maxLines,lineHeight,fontHeight) )
	{
		shellRect = AbsoluteRect;
		shellRect.UpperLeftCorner.Y = AbsoluteRect.LowerRightCorner.Y - lineHeight;

		textRect = AbsoluteRect;
		textRect.LowerRightCorner.Y = textRect.UpperLeftCorner.Y + lineHeight;
	}
	else
	{
		textRect = core::recti(0,0,0,0);
		shellRect = core::recti(0,0,0,0);
	}
}
//////////////////////////////////////////////////////////////////////////

bool CNrpConsole::CalculateLimits(u32& maxLines, u32& lineHeight,s32& fontHeight)
{
	CNrpConsoleConfig& conf = CNrpConsoleConfig::Instance();
	gui::IGUIFont* font = Environment->getFont( conf[ CONSOLE_FONTNAME ].As<NrpText>().ToWide() );
	u32 consoleHeight = AbsoluteRect.getHeight();

	if(font != 0 && consoleHeight > 0)
	{
		fontHeight = font->getDimension(L"X").Height + 2;
		lineHeight = fontHeight + (int)conf[ CONSOLE_LINE_SPACING ];
		maxLines = consoleHeight / lineHeight;
		if(maxLines > 2)
		{
			maxLines -= 2;
		}
		return true;
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////

void CNrpConsole::TabComplete()											//функция автодополнения команды
{
	if(currentCommand_.size() == 0)
	{
		return;
	}

	core::stringw ccStr = currentCommand_.subString(1,currentCommand_.size() - 1); //запоминаем введенные символы без слэша
	
	core::array< core::stringw > names;						//получаем список команд

	GetRegisteredCommands( names );

	core::array< core::stringw > commands_find;				//здесь хранится список подходящих комманд

	for(u32 i = 0; i < names.size(); i++)
	{
		core::stringw thisCmd = names[i];
		if(thisCmd.size() == ccStr.size())				
		{
			if(thisCmd == ccStr)
			{
				return;
			}
		}
		else if(thisCmd.size() > ccStr.size())
		{
			if(thisCmd.subString(0,ccStr.size()) == ccStr) //пробегаем по списку команда
			{												// и ищем вхожденя первых букв комманды
				commands_find.push_back( thisCmd );
			}
		}
	}

	if( commands_find.size() == 1 )							//нашлась команда с таким именем
	{	
		currentCommand_ = commands_find[ 0 ];
		return;
	}
	else													//выводим их на экран и комманду не дополняем
	{
		for( size_t cnt=0; cnt < commands_find.size(); cnt++ )
			AppendMessage( commands_find[ cnt ] );
	}
}

void CNrpConsole::MoveCursor_( bool leftStep )
{
	if( leftStep )
		cursorPos_ -= (cursorPos_ > 0 && currentCommand_.size() ) & 1;
	else 
		cursorPos_ += (cursorPos_ < currentCommand_.size() + 1) & 1;
}

} //namespace gui

} //namespace irr