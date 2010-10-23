#pragma once
#include <sstream>
#include <deque>
#include <windows.h>
#include "StrConversation.h"

namespace nrp 
{

typedef enum
{
	EVENT_ADD_TO_LIST=1	
} SUserEventID;

// Подсистема-источник сообщения (нужно для фильтрации)
typedef enum 
{
	AUTH = 0,		
	LOG,
	VIDEO,
	AIM,		
	SOUND,
	HW,
	NETWORK,
	DB,
	CONF,
	SCRIPT,
	PLUGIN,
	UNKNOWN
} SubSystem;

// Наксолько важным для работы системы является сообщение (нужно для фильтрации)
typedef enum 
{
	INFO = 0,
	DEBUG,
	WARNING,
	FATAL
} SeverityLevel;

// Куда следует направить сообщение (записать в базу, напечатать на консоли или в OutputDebugScreen)
const int 	ODS = 0x1,															//отладка
			CON = 0x2,															//отображение в консоли	
			NRPDB = 0x4,														//запись в базу
			NRPETL	= 0x8;														//запись в таблицу событий				

// Для быстрой фильтрации. Чтобы не заниматься парсингом текста в поле 'text' таблицы,
// а быстро выбирать нужные сообщения по их Code.
typedef enum 
{
	NO_CODE = 0,
	LAUNCH = 1,
	QUIT,
	LOGIN,
	LOGOUT,
	CONFIG_HW,
	CONFIG_USR,
	FAILURE_HW,
	MANAGE_TAGRETS,
	START_STOP,
	PACKETS,
	VIEW_LOGS,
	VIDW_TARGETS,
	STORAGE,
	CLEAN,
	VIEW_TRACE_WND,
	VIEW_PLOTS_WND
} MessageCode;

// При передаче экземпляра этой структуры будет происходить физическая запись сообщения
// в базу данных
struct MsgTerminator {};
// Экземпляр терминатора один на всех желающих писать в лог. Впрочем, в многопоточной среде он 100 %
// безопасен ввиду идеальной его рализации :)
extern MsgTerminator term;

// Основной класс для работы с логами. Предполагаемое использование:
// Log(VIDEO, LAUNCH) << "Запущен режим точек трассы" << 123 << "Слава роботам!" << term;
// Log(AUTH, LOGIN) << "Пользователя с таким именем или паролем не существует" << term;
// Log(DB) << "На диске" << disk_name << "кончилось место" << term;
struct Log 
{
	// ктор1
	Log();
	// ктор2
	Log(SubSystem src, SeverityLevel sev = INFO, MessageCode code = NO_CODE, int dev = NRPDB|CON|ODS);
	// копирующий ктор
	Log(const Log& other);
	Log& operator=(const Log& other);
	// Основной метод для вывода в лог. Примечание: пробелы элементами вставляются автоматически.
	template<typename T> Log& operator<< (T t) 
	{
		Text << t << " ";
		return *this;
	}

	template<> Log& operator<< <int>(int t)
	{
		Text << nrp::IntToStr(t);
		return *this;
	}
	// Кто-нибудь знает, зачем переопределять шаблон специально для cch* ?
	// (А иначе не работает запись типа Log() << mysql_info() << term; )
	template<> Log& operator<< <const char*>(const char* t) 
	{		
		Text << (t ? t : "(null)") << " ";
		return *this;
	}	
	// Перегруженный метод, для того чтобы Log знал, когда следует задампить себя в базу.
	template<> Log& operator<< <MsgTerminator>(MsgTerminator)
	{
		WriteLogEntry();
		return *this;
	}
	void WriteLogEntry();
	// Для подсистемы, из которой пришло ссобщение
	SubSystem Source;
	// Опциональный код сообщения (для быстрой выборки)
	MessageCode Code;

	// Уровень важности для дальнейшего функционирования
	SeverityLevel Sev;
	// Устройства(о), на который будет отправлено сообщение
	int Device;
	
	std::stringstream Text;

	SYSTEMTIME createTime;			//время возникновения сообщения
};

// Класс умеет общаться с базой данных (в частности, загружать ее конфиг с пользователем и паролем)
// и умеет писать в нужные устройства экземпляр класса Log.
// TODO: сделать в виде синглетона
class Logger
{
public:
	// Синглетон потокоНЕбезопасен, потенциально возможно создание 
	// двух и более экземпляров в многопоточной среде
	// Поэтому функцию Instance нужно явно вызвать перед использованием 
	// ...
	// LogSubsystem& log = LogSubsystem::Instance() /*(Это где-то в main)*/
	// ...
	static Logger& Instance();
	// Удалось ли открыть базу и подготовить в ней необходимую таблицу для
	// логов (если там такой таблицы не было)
	inline bool IsDBOk() const { return ok_; }
	// Служебная функция, которая разберется, что надо делать с экземпяром класса Log
	// (в базу записать, на экране отобразить)
	void EnqueueLogEntry(const Log& log);

	static void Stop(); 
private:
	Logger();
	~Logger();
	bool ok_;
	// Динамическая очередь элементов.
	std::deque<Log> queue_;
	// Критическая секция на добавление (и извлечение) элементов из очереди.
	CRITICAL_SECTION enqueueCS_;
	// Ссылка на поток (требуется его сохранение для корректного завершения)
	HANDLE hThread_;
	// Функция, которая будет выполнять в отдельном потоке метод Logger::Run.
	friend DWORD WINAPI ThreadFuncLog( LPVOID lpParam );
	// Бесконечный цикл обработки поступающих сообщений.
	bool Run();
	// Низкоуровневая процедура, которая умеет писать в базу данных
	void TreatLogEntry(const Log& log);
	// Флажок, сигнализирующий о том, что потоку логов следует корректно завершить работу
	volatile bool stop_;
};

}  //namespace nrp