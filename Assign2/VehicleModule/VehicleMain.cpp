#include <SMObject.h>
#include <SMStructs.h>
#include <conio.h>
#define WAIT_TIME 100

using namespace System;
using namespace System::Threading;

int main()
{
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));

	PM* PMSMPtr = nullptr;
	int WaitCount;
	/*PMObj.SMCreate();
	if (PMObj.SMCreateError) {
		Console::WriteLine("Shared memory creation failed");
		return -1;
	}*/

	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed");
		return -2;
	}
	//Console::WriteLine(WaitCount);
	PMSMPtr = (PM*)PMObj.pData;
	PMSMPtr->Shutdown.Flags.Vehicle = 0;


	while (!PMSMPtr->Shutdown.Flags.Vehicle)
	{
		PMSMPtr->Heartbeats.Flags.Vehicle = 1;

		if (PMSMPtr->PMHeartbeats.Flags.Vehicle)
		{
			PMSMPtr->PMHeartbeats.Flags.Vehicle = 0;
			WaitCount = 0;
			//Console::WriteLine("{0,9:F6}", WaitCount);
		}
		else
		{
			if (++WaitCount > WAIT_TIME)
			{
				Console::WriteLine(WaitCount);
				//Console::WriteLine("GPS am dead");
				PMSMPtr->Shutdown.Status = 0xFF;
			}
			//Console::WriteLine("Waitcount: " + WaitCount);
		}

		//Console::WriteLine("{0,9:F3}", PMSMPtr->PMTimeStamp);
		PMSMPtr->PMTimeStamp = 0;
		//if (_kbhit()) break;
		Sleep(20);
	}

	//Console::ReadKey();
	Console::WriteLine("Vehicle terminated normally.");
	//Console::ReadKey();

	return 0;
}