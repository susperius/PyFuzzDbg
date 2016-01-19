#include "ProcessList.h"

ProcessList::ProcessList() {
	start_ptr = NULL;
	end_ptr = NULL;
}

ProcessList::~ProcessList() {
	LPLIST_ITEM actual_item = start_ptr;
	while (start_ptr->next != NULL) {
		start_ptr = start_ptr->next;
		delete actual_item;
		actual_item = start_ptr;
	}
	delete start_ptr;
}

void ProcessList::add_item(DWORD proc_id, HANDLE proc_handle) {
	LPLIST_ITEM entry = new LIST_ITEM();
	entry->proc_id = proc_id;
	entry->proc_handle = proc_handle;
	if (start_ptr == NULL) {
		start_ptr = entry;
		end_ptr = entry;
	}
	else {
		end_ptr->next = entry;
		end_ptr = entry;
	}
}

bool ProcessList::del_item(DWORD proc_id) {
	bool return_value = false;
	LPLIST_ITEM item = start_ptr;
	if (item->proc_id == proc_id) {
		//first item
		start_ptr = item->next;
		delete item;
		return true;
	}
	else {
		while (item->next != NULL) {
			if (item->next->proc_id == proc_id) {
				LPLIST_ITEM del_item = item->next;
				if (end_ptr == del_item) {
					end_ptr = item;
				}
				item->next = del_item->next;
				delete del_item;
				return true;
			}
			else {
				item = item->next;
			}
		}
	}
	return false;
}

HANDLE ProcessList::get_first_handle() {
	if (start_ptr != NULL) {
		LPLIST_ITEM item = start_ptr;
		HANDLE return_value = item->proc_handle;
		start_ptr = item->next;
		delete item;
		return return_value;
	}
	return NULL;
	
}