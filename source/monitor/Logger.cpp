#include "stdafx.h"
#include <winsock.h>
#include <windows.h>
#include "Logger.h"
#include "IniFile.h"
#include <string>
#include <cstdarg>
#include <lua.hpp>
#include "IniFile.h"
#include "Logger.h"
#include "nrpConsole.h"
#include "nrpEngine.h"
#include "StrConversation.h"
#include "nrpScene.h"
#include <irrlicht.h>
using std::string;

// Глобальный (для этого модуля) хэндл базы данных. Вынести его в определение
// класса Logger не представляется возможным, так как для этого придется в Logger.h
// включать еще и winsock.h и mysql.h. Можно сделать pimpl, но лень.
namespace nrp 
{

// shared экземпляр терминатора сообщения сидит в этом модуле.
MsgTerminator term;

// Глобальный (для этого модуля) указатель для реализации синглетон-аксесса.
static Logger* GlobalLogSubSystem = 0;

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
	
	// TODO: Заменить MessageBox на что-нибудь более ХУД'анутое.
	MessageBox(0, msgBuf, "Logger Service Message", MB_ICONSTOP | MB_OK);
	va_end(list);
}

DWORD WINAPI ThreadFuncLog( LPVOID lpParam )
{
	nrp::Logger* classPtr = reinterpret_cast<nrp::Logger*>(lpParam);
	if (classPtr)
	{
		try
		{
			return classPtr->Run();
		}
		catch (...)
		{
			HandleLoggerErrors("Поток обработчика логов рухнул.");
			return EXIT_FAILURE;
		}
	}
	HandleLoggerErrors("Не удалось запустить поток обработчика логов.");
	return EXIT_FAILURE;
}

// ктор по умолчанию
Log::Log() 
{
	GetLocalTime( &createTime );
}

// ктор
Log::Log(SubSystem src, SeverityLevel sev, MessageCode code, int dev)
: Source(src), Code(code), Sev(sev), Device(dev) 
{ 
	GetLocalTime( &createTime );
}

// копирующий ктор
Log::Log(const Log& other)
: Source(other.Source), Code(other.Code), Sev(other.Sev), Device(other.Device), createTime(other.createTime)
{
	Text << other.Text.str();
}

Log& Log::operator=(const Log& other)
{
	Source = other.Source;
	Code = other.Code;
	Sev = other.Sev;
	Device = other.Device;
	createTime = other.createTime;
	Text.clear();
	Text << other.Text.str();
	return *this;
}

void Log::WriteLogEntry()
{
	Logger::Instance().EnqueueLogEntry(*this);
}

Logger& Logger::Instance()
{
	if (!GlobalLogSubSystem)
	{
		//TODO: Здесь на самом деле хорошо бы поставить "двойную проверку"
		// man двойная проверка 
		// или хотя бы обычную блокировку.
		GlobalLogSubSystem = new Logger;
	}

	return *GlobalLogSubSystem;
}

Logger::Logger() 
: hThread_(0), stop_(false)
{
	// Подгрузка конфигурации
	ok_ = true;

	
	InitializeCriticalSectionAndSpinCount( &enqueueCS_, 1000 );
	// Создаем новый тред, который будет обслуживать очередь
	DWORD threadID;
	hThread_ = CreateThread(0, 0, ThreadFuncLog, reinterpret_cast<void*>(this), 0, &threadID);
	if ( !hThread_ )
		HandleLoggerErrors("Не удалось запустить поток для логов (GetLastError() = %d)", GetLastError());
}

Logger::~Logger()
{
	// Дтор не должен выбрасывать исключений
	try
	{
		stop_ = true;
		DeleteCriticalSection( &enqueueCS_ );
		
		if (hThread_ && WaitForSingleObject(hThread_, 1000/*мс*/) == WAIT_TIMEOUT)
			TerminateThread(hThread_, EXIT_FAILURE);
	}
	catch(...)
	{
	}
}

bool Logger::Run()
{
	while ( !stop_ )
	{
		Log workEntry;
		bool hasEntries = false;
		// В критическую секцию помещена только работа с очередью.
		// Работа с БД из нее вынесена.
		EnterCriticalSection( &enqueueCS_ );
		try {

			if ( queue_.size() )
			{
				workEntry = queue_.back();
				queue_.pop_back();
				hasEntries = true;
			}

		} catch (...) { }
		LeaveCriticalSection( &enqueueCS_ );

		if ( hasEntries )
			try
			{
				TreatLogEntry( workEntry );
			}
			catch(...)
			{

			}
		else 
			Sleep( 50 );
	}
	return EXIT_SUCCESS;
}

void Logger::EnqueueLogEntry(const Log& log)
{
	EnterCriticalSection( &enqueueCS_ );
	try
	{
		queue_.push_front( log );
	}
	catch (...)
	{
	}
	LeaveCriticalSection( &enqueueCS_ );
}

void Logger::TreatLogEntry(const Log& log)
{
	// Диспетчеризация по устройству назначения
	if ( log.Device & ODS )
	{
		// переносим текст из std::stirng в массив чаров (чтобы избежать глюка)
		const int FULL_TEXT_BUF_LEN = 8192;
		char logText[ FULL_TEXT_BUF_LEN ] = { 0 };
		strncpy_s(logText, log.Text.str().c_str(), min(FULL_TEXT_BUF_LEN, log.Text.str().size()));

#ifdef _DEBUG
		OutputDebugString( logText );
		OutputDebugString( "\n" );
#endif
	}

	if ( log.Device & CON )
	{
		irr::gui::CNrpConsole* console = CNrpEngine::Instance().GetConsole();
		if( console )
			console->AppendMessage( nrp::StrToWide( log.Text.str() ).c_str() );
	}

	if( log.Device & NRPETL ) //этот тип надо помещать в список событий
	{
		irr::scene::INrpScene* tmpScene = CNrpEngine::Instance().GetCurrentScene();

		if( tmpScene )
		{
			irr::SEvent userEvent;
			userEvent.EventType = irr::EET_USER_EVENT;

			userEvent.UserEvent.UserData1 = EVENT_ADD_TO_LIST;
			userEvent.UserEvent.UserData2 = (int)&log;

			tmpScene->OnEvent( userEvent );
		}
	}
	// Складировать на постоянное сохранение отладочные сообщения 
	// системы - не лучшая идея (проверка флага Sev).
	// Если к БД подключиться не удалось, то тыкаться туда, очевидно 
	// тоже смысла нет (проверка флага ok_)
}

}//namespace nrp