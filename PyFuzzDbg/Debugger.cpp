#include "Debugger.h"


#ifdef UNICODE
#define tcout std::wcout
#define FACTOR 2
#else
#define tcout std::cout
#define FACTOR 1
#endif

Debugger::Debugger(std::wstring application_name, std::wstring command_line, uint32_t sleep_time) {
	ZeroMemory(&startup_info, sizeof(startup_info));
	ZeroMemory(&proc_info, sizeof(proc_info));
	ZeroMemory(&dbg_event, sizeof(dbg_event));
	//assert(application_name.size() > 1000);
	std::copy(application_name.begin(), application_name.end(), app_name);
	std::copy(command_line.begin(), command_line.end(), cmd_line);
	this->sleep_time = (time_t)sleep_time;
}

bool Debugger::start_process() {
	tcout << "Starting Process with cmd_line: " << std::endl << app_name << std::endl;
	bool proc_creation_successfull = CreateProcess(app_name, // Module name
		cmd_line, // command line
		NULL,
		NULL,
		FALSE,
		DEBUG_PROCESS, // creation flags
		NULL,
		NULL,
		&startup_info,
		&proc_info);
	if (!proc_creation_successfull) {
		tcout << "Process creation failed: " << GetLastError() << std::endl;
		tcout << "Application name: " << app_name << std::endl;
	}
	return proc_creation_successfull;
}

uint32_t Debugger::start_test() {
	if (!this->start_process()) {
		return 0;
	}
	time_t start = time(0);
	time_t running = start;
	EXCEPTION_DEBUG_INFO exception_debug_info;
	while ((running - start) < sleep_time) {
		WaitForDebugEvent(&dbg_event, INFINITE);
		if (dbg_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
			exception_debug_info = dbg_event.u.Exception;
			if (exception_debug_info.dwFirstChance != 0) {
				ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
			}
			else {
				return exception_debug_info.ExceptionRecord.ExceptionCode;
			}
		}
		else {
			ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
		}
		running = time(0);
	}
	return 0;
}

void Debugger::export_Debugger() {
	using namespace boost::python;
	class_ < Debugger >("Debugger", init<std::wstring, std::wstring, uint32_t>())
		.def("start_test", &Debugger::start_test)
		;
}