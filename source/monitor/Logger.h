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

// ����������-�������� ��������� (����� ��� ����������)
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

// ��������� ������ ��� ������ ������� �������� ��������� (����� ��� ����������)
typedef enum 
{
	INFO = 0,
	DEBUG,
	WARNING,
	FATAL
} SeverityLevel;

// ���� ������� ��������� ��������� (�������� � ����, ���������� �� ������� ��� � OutputDebugScreen)
const int 	ODS = 0x1,															//�������
			CON = 0x2,															//����������� � �������	
			NRPDB = 0x4,														//������ � ����
			NRPETL	= 0x8;														//������ � ������� �������				

// ��� ������� ����������. ����� �� ���������� ��������� ������ � ���� 'text' �������,
// � ������ �������� ������ ��������� �� �� Code.
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

// ��� �������� ���������� ���� ��������� ����� ����������� ���������� ������ ���������
// � ���� ������
struct MsgTerminator {};
// ��������� ����������� ���� �� ���� �������� ������ � ���. �������, � ������������� ����� �� 100 %
// ��������� ����� ��������� ��� ��������� :)
extern MsgTerminator term;

// �������� ����� ��� ������ � ������. �������������� �������������:
// Log(VIDEO, LAUNCH) << "������� ����� ����� ������" << 123 << "����� �������!" << term;
// Log(AUTH, LOGIN) << "������������ � ����� ������ ��� ������� �� ����������" << term;
// Log(DB) << "�� �����" << disk_name << "��������� �����" << term;
struct Log 
{
	// ����1
	Log();
	// ����2
	Log(SubSystem src, SeverityLevel sev = INFO, MessageCode code = NO_CODE, int dev = NRPDB|CON|ODS);
	// ���������� ����
	Log(const Log& other);
	Log& operator=(const Log& other);
	// �������� ����� ��� ������ � ���. ����������: ������� ���������� ����������� �������������.
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
	// ���-������ �����, ����� �������������� ������ ���������� ��� cch* ?
	// (� ����� �� �������� ������ ���� Log() << mysql_info() << term; )
	template<> Log& operator<< <const char*>(const char* t) 
	{		
		Text << (t ? t : "(null)") << " ";
		return *this;
	}	
	// ������������� �����, ��� ���� ����� Log ����, ����� ������� ��������� ���� � ����.
	template<> Log& operator<< <MsgTerminator>(MsgTerminator)
	{
		WriteLogEntry();
		return *this;
	}
	void WriteLogEntry();
	// ��� ����������, �� ������� ������ ���������
	SubSystem Source;
	// ������������ ��� ��������� (��� ������� �������)
	MessageCode Code;

	// ������� �������� ��� ����������� ����������������
	SeverityLevel Sev;
	// ����������(�), �� ������� ����� ���������� ���������
	int Device;
	
	std::stringstream Text;

	SYSTEMTIME createTime;			//����� ������������� ���������
};

// ����� ����� �������� � ����� ������ (� ���������, ��������� �� ������ � ������������� � �������)
// � ����� ������ � ������ ���������� ��������� ������ Log.
// TODO: ������� � ���� ����������
class Logger
{
public:
	// ��������� �����������������, ������������ �������� �������� 
	// ���� � ����� ����������� � ������������� �����
	// ������� ������� Instance ����� ���� ������� ����� �������������� 
	// ...
	// LogSubsystem& log = LogSubsystem::Instance() /*(��� ���-�� � main)*/
	// ...
	static Logger& Instance();
	// ������� �� ������� ���� � ����������� � ��� ����������� ������� ���
	// ����� (���� ��� ����� ������� �� ����)
	inline bool IsDBOk() const { return ok_; }
	// ��������� �������, ������� ����������, ��� ���� ������ � ���������� ������ Log
	// (� ���� ��������, �� ������ ����������)
	void EnqueueLogEntry(const Log& log);

	static void Stop(); 
private:
	Logger();
	~Logger();
	bool ok_;
	// ������������ ������� ���������.
	std::deque<Log> queue_;
	// ����������� ������ �� ���������� (� ����������) ��������� �� �������.
	CRITICAL_SECTION enqueueCS_;
	// ������ �� ����� (��������� ��� ���������� ��� ����������� ����������)
	HANDLE hThread_;
	// �������, ������� ����� ��������� � ��������� ������ ����� Logger::Run.
	friend DWORD WINAPI ThreadFuncLog( LPVOID lpParam );
	// ����������� ���� ��������� ����������� ���������.
	bool Run();
	// �������������� ���������, ������� ����� ������ � ���� ������
	void TreatLogEntry(const Log& log);
	// ������, ��������������� � ���, ��� ������ ����� ������� ��������� ��������� ������
	volatile bool stop_;
};

}  //namespace nrp