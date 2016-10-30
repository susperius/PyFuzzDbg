PyFuzzDbg
=========

I decided to write an own python extension for the standard testing workflow in 
PyFuzz2, in order to avoid involving the whole WinDbg engine on every testcase.
This little extension just creates a new process with the DEBUG_PROCESS flag and
returns the exception code if an execption occured or just 0 if nothing happened
and the timer expired.

I created this extension using Visual Studio 2015 and the boost-python library. 
The project settings in the master branch are set to release. So you can import
the project into your VS2015 and compile it.
