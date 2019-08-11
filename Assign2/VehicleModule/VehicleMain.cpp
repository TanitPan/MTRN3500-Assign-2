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
		// Setting vehicle heartbeat to alive
		PMSMPtr->Heartbeats.Flags.Vehicle = 1;

		// Checking PM heartbeat via vehicle
		if (PMSMPtr->PMHeartbeats.Flags.Vehicle)
		{
			PMSMPtr->PMHeartbeats.Flags.Vehicle = 0;
			WaitCount = 0;
		}
		else
		{
			if (++WaitCount > WAIT_TIME)
			{
				PMSMPtr->Shutdown.Status = 0xFF;
			}
			
		}

		vehicle->ControlVehicle(XboxSMPtr->remoteSteering, XboxSMPtr->remoteSpeed, flag); // Need to add Xbox SM data to the first 2 parameters
		flag = !flag;
		Thread::Sleep(20);
	}

	Console::WriteLine("Vehicle Process terminated normally.");

	return 0;
}