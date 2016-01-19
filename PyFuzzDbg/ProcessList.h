#pragma once
#include <Windows.h>
#include <iostream>

typedef struct _list_item {
	DWORD proc_id;
	HANDLE proc_handle;
	_list_item *next;
}LIST_ITEM, *LPLIST_ITEM;

class ProcessList {
private:
	LPLIST_ITEM start_ptr, end_ptr;
public:
	ProcessList();
	void add_item(DWORD proc_id, HANDLE proc_handle);
	bool del_item(DWORD proc_id);
	HANDLE get_first_handle();
};