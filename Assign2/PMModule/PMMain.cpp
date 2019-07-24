#include <SMObject.h>
#include <SMStructs.h>
#include <conio.h>
#include <iostream>
#include <TlHelp32.h>
#define NUM_PROCESS 1


using namespace System;
using namespace System::Threading;




//The following file gives some useful code fragments 
//for process management. You should fill out the main function with the following steps:
//1) start all processes
//2) while the PM shutdown flag is not set, keep looping through the heartbeat process

//defining start up sequence
// Start up sequence
TCHAR* Units[10] = //
{
	TEXT("GPSModule.exe"),
	TEXT("LaserModule.exe"), // Should change to laser
	TEXT("XBox.exe"),
	TEXT("VehicleControl.exe"),
	TEXT("OpenGL.exe"),

};

// Module execution based variable declarations
STARTUPINFO s[10];
PROCESS_INFORMATION p[10];


//Is provess running function
bool IsProcessRunning(const char* processName)
{
	bool exists = false;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
			if (!_stricmp(entry.szExeFile, processName))
				exists = true;

	CloseHandle(snapshot);
	return exists;
}



int main()
{
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));

	PM* PMSMPtr = nullptr;

	PMObj.SMCreate();
	if (PMObj.SMCreateError) {
		Console::WriteLine("Shared memory creation failed");
		return -1;
	}

	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed");
		return -2;
	}
	
	PMSMPtr = (PM*)PMObj.pData;
	PMSMPtr->Shutdown.Flags.PM = 0;
	PMSMPtr->Heartbeats.Status = 0x00;
	
	// Change number of loops to match .exe output files
	// Starting the processes
	for (int i = 0; i < NUM_PROCESS; i++)
	{
		// Check if each process is running
		if (!IsProcessRunning(Units[i]))
		{
			ZeroMemory(&s[i], sizeof(s[i]));
			s[i].cb = sizeof(s[i]);
			ZeroMemory(&p[i], sizeof(p[i]));
			// Start the child processes.

			if (!CreateProcess(NULL,   // No module name (use command line)
				Units[i],        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				CREATE_NEW_CONSOLE,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory
				&s[i],            // Pointer to STARTUPINFO structure
				&p[i])           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("%s failed (%d).\n", Units[i], GetLastError());
				//_getch(); can be changed to _kbhit()
				_kbhit();
				return -1;
			}
		}
		std::cout << "Started: " << Units[i] << std::endl;
		Sleep(1000);
	}



	while (!PMSMPtr->Shutdown.Flags.PM)
	{
		Sleep(200);
		PMSMPtr->PMHeartbeats.Flags.GPS = 1;
		PMSMPtr->PMHeartbeats.Flags.Laser = 1;
		
		
		if (PMSMPtr->Heartbeats.Flags.GPS)
		{
			PMSMPtr->Heartbeats.Flags.GPS = 0;
		}
		else
		{
			PMSMPtr->Shutdown.Status = 0xFF;
		}

		Sleep(10);
		Console::WriteLine(PMSMPtr->Heartbeats.Flags.GPS);
		

		/*if (PMSMPtr->Heartbeats.Flags.Laser)
		{
			PMSMPtr->Heartbeats.Flags.Laser = 0;
		}
		else
		{
			PMSMPtr->Shutdown.Status = 0xFF;
		}*/

		//PMSMPtr->PMTimeStamp = 300;
		//if (_kbhit()) break;
		
		if (_kbhit())
		{
			PMSMPtr->Shutdown.Status = 0xFF;
			bool allShutdown = false;
			while (!allShutdown)
			{
				for (int i = 0; i < NUM_PROCESS; i++) 
				{
					if (!IsProcessRunning(Units[i]))
					{
						allShutdown = true;
					}
					else
					{
						allShutdown = false;
						break;
					}
				}
			}
			/*PMSMPtr->Shutdown.Status = 0xFE;
			Sleep(2500);
			PMSMPtr->Shutdown.Flags.PM = 1;*/
		}
		
	}
	
	Console::ReadKey();
	Console::WriteLine("Process Mangement terminated normally.");
	Console::ReadKey();

	return 0;
}