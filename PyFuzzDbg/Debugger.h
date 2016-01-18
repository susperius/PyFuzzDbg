#pragma once
#include <Windows.h>
#include <tchar.h>
#include <string>
#include <cassert>
#include <iostream>
#include <atlconv.h>
#include <stdint.h>
#include <ctime>

#include <boost/python.hpp>

#include "ProcessList.h"

#define STRING_SIZE 1000

class Debugger {
private:
	STARTUPINFO startup_info;
	PROCESS_INFORMATION proc_info;
	DEBUG_EVENT dbg_event;
	TCHAR app_name[STRING_SIZE];
	time_t sleep_time;
	ProcessList proc_list;

	bool start_process();
	void kill_all_processes();
public:
	Debugger(uint32_t sleep_time);
	uint32_t Debugger::start_test();
	void Debugger::set_app_name(std::wstring application_name);
	
	static void export_Debugger(); // export the class to python
};