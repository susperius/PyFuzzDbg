#include "ProcessList.h"

#ifdef UNICODE
#define tcout std::wcout
#define FACTOR 2
#else
#define tcout std::cout
#define FACTOR 1
#endif

ProcessList::ProcessList() {
	start_ptr = NULL;
	end_ptr = NULL;
}

void ProcessList::add_item(DWORD proc_id, HANDLE proc_handle) {
	LIST_ITEM entry = { proc_id, proc_handle, NULL };
	if (start_ptr == NULL) {
		start_ptr = &entry;
		end_ptr = &entry;
	}
	else {
		end_ptr->next = &entry;
		end_ptr = &entry;
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
	tcout << "Trying to receive first process" << std::endl;
	if (start_ptr != NULL) {
		// producing crashes ....
		LPLIST_ITEM item = start_ptr;
		HANDLE return_value = item->proc_handle;
		start_ptr = item->next;
		//delete item;
		return return_value;
	}
	return NULL;
	
}