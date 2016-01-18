#include "Debugger.h"


#ifdef UNICODE
#define tcout std::wcout
#define FACTOR 2
#else
#define tcout std::cout
#define FACTOR 1
#endif

Debugger::Debugger(uint32_t sleep_time) {
	ZeroMemory(&startup_info, sizeof(startup_info));
	ZeroMemory(&proc_info, sizeof(proc_info));
	ZeroMemory(&dbg_event, sizeof(dbg_event));
	//assert(application_name.size() > 1000);
	this->sleep_time = (time_t)sleep_time;
}

bool Debugger::start_process() {
	tcout << "Starting Process with app_name: " << std::endl << app_name << std::endl;
	bool proc_creation_successfull = CreateProcess(NULL, // Module name
		app_name, // command line
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
	DWORD return_code = 0;
	DWORD exit_code = STILL_ACTIVE;
	if (!this->start_process()) {
		return 0;
	}
	time_t start = time(0);
	time_t running = start;
	EXCEPTION_DEBUG_INFO exception_debug_info;	
	while (((running - start) < sleep_time) && (exit_code == STILL_ACTIVE)) {
		WaitForDebugEvent(&dbg_event, 200);
		if (dbg_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
			exception_debug_info = dbg_event.u.Exception;
			if (exception_debug_info.dwFirstChance != 0) {
				ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
			}
			else {
				ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
				kill_all_processes();
				return_code = exception_debug_info.ExceptionRecord.ExceptionCode;				
			}
		}
		else if (dbg_event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
			tcout << "Removing process from proc_list" << std::endl;
			proc_list.del_item(dbg_event.dwProcessId);
			ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_CONTINUE);
		}
		else if (dbg_event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
			tcout << "Adding new Process to proc_list" << std::endl;
			proc_list.add_item(dbg_event.dwProcessId, dbg_event.u.CreateProcessInfo.hProcess);
			ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_CONTINUE);
		}
		else {
			ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_CONTINUE);
		}
		GetExitCodeProcess(proc_info.hProcess, &exit_code);
		running = time(0);
	}
	kill_all_processes();
	return return_code;
}

void Debugger::set_app_name(std::wstring application_name) {
	ZeroMemory(&startup_info, sizeof(startup_info));
	ZeroMemory(&proc_info, sizeof(proc_info));
	ZeroMemory(&dbg_event, sizeof(dbg_event));
	proc_list = ProcessList();
	std::copy(application_name.begin(), application_name.end(), app_name);
}


void Debugger::kill_all_processes() {
	tcout << "Kill all processes called!" << std::endl;
	HANDLE proc_handle = proc_list.get_first_handle();
	tcout << "Got first handle" << std::endl;
	while (proc_handle != NULL) {
		tcout << "TerminateProcess" << std::endl;
		TerminateProcess(proc_handle, DBG_TERMINATE_PROCESS);
		tcout << "Need next process" << std::endl;
		proc_handle = proc_list.get_first_handle();
	}
}

void Debugger::export_Debugger() {
	using namespace boost::python;
	class_ < Debugger >("Debugger", init<uint32_t>())
		.def("start_test", &Debugger::start_test)
		.def("set_app_name", &Debugger::set_app_name)
		;
}
