#include <SMObject.h>
#include <SMStructs.h>
#include <conio.h>

#include "VehicleRef.h"
#define WAIT_TIME 100

using namespace System;
using namespace System::Threading;

int main()
{
	// Setup Shared Memory object
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));
	SMObject VehicleObj(_TEXT("VehicleObj"), sizeof(VehicleSM));
	SMObject XboxObj(_TEXT("XboxObj"), sizeof(Remote));

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

	VehicleObj.SMAccess();
	XboxObj.SMAccess();

	PM* PMSMPtr = (PM*)PMObj.pData;
	VehicleSM* VehSMPtr = (VehicleSM*)VehicleObj.pData;
	Remote* XboxSMPtr = (Remote*)XboxObj.pData;

	// Client setup
	int WaitCount = 0;
	unsigned int flag = 0;
	VehicleRef^ vehicle = gcnew VehicleRef("192.168.1.200", 25000);
	vehicle->Connect();
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
				PMSMPtr->Shutdown.Status = 0xFF;
			}
			Console::WriteLine("Waitcount: " + WaitCount);
			Console::WriteLine("Vehicle is dead");
		}

		
		vehicle->ControlVehicle(XboxSMPtr->remoteSteering, XboxSMPtr->remoteSpeed, flag); // Need to add Xbox SM data to the first 2 parameters
		flag = !flag;
		//Console::WriteLine("{0,9:F3}", PMSMPtr->PMTimeStamp);
		//MSMPtr->PMTimeStamp = 0;
		//if (_kbhit()) break;
		Thread::Sleep(20);
	}

	//Console::ReadKey();
	Console::WriteLine("Vehicle Process terminated normally.");
	//Console::ReadKey();

	return 0;
}