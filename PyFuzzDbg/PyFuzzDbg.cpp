// PyFuzzDbg.cpp : Defines the exported functions for the DLL application.
//

#include "Debugger.h"
#include <boost\python.hpp>


BOOST_PYTHON_MODULE(PyFuzzDbg) {
	using namespace boost::python;
	scope().attr("EXCEPTION_DEBUG_EVENT") = EXCEPTION_DEBUG_EVENT;
	Debugger::export_Debugger();
}