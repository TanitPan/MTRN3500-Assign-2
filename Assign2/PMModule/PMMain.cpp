#include <SMObject.h>
#include <SMStructs.h>
#include <conio.h>
#include <iostream>
#include <TlHelp32.h>
#define NUM_PROCESS 5

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
	TEXT("VehicleModule.exe"),
	TEXT("XboxModule.exe"),
	TEXT("DisplayModule.exe"),

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

// Process startup routine
int startUp(int i)
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
	Sleep(100); 
}

// Shutdown routine
void shutDown()
{
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
}

int main()
{
	// Declaring SM Object
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));
	SMObject LaserObj(_TEXT("LaserObj"), sizeof(Laser));
	SMObject GPSObj(_TEXT("GPSObj"), sizeof(GPS));
	SMObject VehicleObj(_TEXT("VehicleObj"), sizeof(VehicleSM));
	SMObject XboxObj(_TEXT("XboxObj"), sizeof(Remote));
	
	
	//SMObject 
	PM* PMSMPtr = nullptr;
	Remote* XboxSMPtr = nullptr;
	

	PMObj.SMCreate();
	if (PMObj.SMCreateError) {
		Console::WriteLine("Shared memory creation failed");
		return -1;
	}

	LaserObj.SMCreate();
	if (LaserObj.SMCreateError) {
		Console::WriteLine("Shared memory creation failed");
		return -1;
	}

	GPSObj.SMCreate();
	if (GPSObj.SMCreateError) {
		Console::WriteLine("Shared memory creation failed");
		return -1;
	}

	VehicleObj.SMCreate();
	XboxObj.SMCreate();
	

	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed");
		return -2;
	}
	XboxObj.SMAccess();

	
	PMSMPtr = (PM*)PMObj.pData;
	XboxSMPtr = (Remote*)XboxObj.pData;

	PMSMPtr->Shutdown.Flags.PM = 0;
	PMSMPtr->Heartbeats.Status = 0x00;
	
	// Change number of loops to match .exe output files
	// Starting the processes
	for (int i = 0; i < NUM_PROCESS; i++)
	{
		if (startUp(i) == -1)
		{
			return -1;
		}
	}


	while (!PMSMPtr->Shutdown.Flags.PM)
	{
		Sleep(250);
		// Setting PM heartbeat to be alive
		PMSMPtr->PMHeartbeats.Status = 0xFF;
		
		// Checking other modules heartbeat
		if (PMSMPtr->Heartbeats.Flags.GPS)
		{
			PMSMPtr->Heartbeats.Flags.GPS = 0;
		}
		else
		{
			// Non-critical process startup
			startUp(0);
		}

		if (PMSMPtr->Heartbeats.Flags.Laser)
		{
			PMSMPtr->Heartbeats.Flags.Laser = 0;
		}
		else
		{
			Console::WriteLine("Laser in PM check die");
			PMSMPtr->Shutdown.Status = 0xFF;
		}


		if (PMSMPtr->Heartbeats.Flags.Vehicle)
		{
			PMSMPtr->Heartbeats.Flags.Vehicle = 0;
		}
		else
		{
			Console::WriteLine("Vehicle in PM check die");
			PMSMPtr->Shutdown.Status = 0xFF;
		}

		if (PMSMPtr->Heartbeats.Flags.Xbox)
		{
			PMSMPtr->Heartbeats.Flags.Xbox = 0;
		}
		else
		{
			Console::WriteLine("Xbox in PM check die");
			PMSMPtr->Shutdown.Status = 0xFF;
		}
		if (PMSMPtr->Heartbeats.Flags.DisplayGL)
		{
			PMSMPtr->Heartbeats.Flags.DisplayGL = 0;
		}
		else
		{
			PMSMPtr->Shutdown.Status = 0xFF;
	
		}

		// Printing each process heartbeat in PM module
		Sleep(10);
		Console::WriteLine("[GPS Heartbeat " + PMSMPtr->Heartbeats.Flags.GPS);
		Console::WriteLine("Laser Heartbeat " + PMSMPtr->Heartbeats.Flags.Laser);
		Console::WriteLine("Xbox Heartbeat " + PMSMPtr->Heartbeats.Flags.Xbox);
		Console::WriteLine("Vehicle Heartbeat " + PMSMPtr->Heartbeats.Flags.Vehicle + "]\n");


		// Shutdown routine if the Xbox key is pressed or keyboard hit
		if (_kbhit() || XboxSMPtr->routineShutdown)
		{
			PMSMPtr->Shutdown.Status = 0xFF;
			shutDown();
			
		}
		
	}
	
	//Console::ReadKey();
	Console::WriteLine("Process Mangement terminated normally.");
	Console::ReadKey();

	return 0;
}