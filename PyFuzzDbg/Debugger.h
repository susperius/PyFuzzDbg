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

#define STRING_SIZE 1000

class Debugger {
private:
	STARTUPINFO startup_info;
	PROCESS_INFORMATION proc_info;
	DEBUG_EVENT dbg_event;
	TCHAR app_name[STRING_SIZE];
	time_t sleep_time;
	bool start_process();
public:
	Debugger(std::wstring application_name, uint32_t sleep_time);
	uint32_t Debugger::start_test();
	void Debugger::set_app_name(std::wstring application_name);
	
	static void export_Debugger(); // export the class to python
};