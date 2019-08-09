#include <SMStructs.h>
#include <SMObject.h>
#include <conio.h>
#include "XBoxController.h"


#define WAIT_TIME 100 // 300
#define MIN_JOY_STICK -32768.0
#define MAX_JOY_STICK 32767.0

using namespace System;
using namespace System::Threading;

int main()
{
	// Setup Shared Memory object
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));
	SMObject XboxObj(_TEXT("XboxObj"), sizeof(Remote));

	PMObj.SMAccess();
	XboxObj.SMAccess();
	
	PM* PMSMPtr = (PM*)PMObj.pData;
	Remote* XboxSMPtr = (Remote*)XboxObj.pData;

	XInputWrapper xinput;
	GamePad::XBoxController controller(&xinput, 0);


	int WaitCount = 0;
	PMSMPtr->Shutdown.Flags.Xbox = 0;

	while (!PMSMPtr->Shutdown.Flags.Xbox)
	{
		PMSMPtr->Heartbeats.Flags.Xbox = 1;

		if (PMSMPtr->PMHeartbeats.Flags.Xbox)
		{
			PMSMPtr->PMHeartbeats.Flags.Xbox = 0;
			WaitCount = 0;
		}
		else
		{
			if (++WaitCount > WAIT_TIME)
			{
				Console::WriteLine("Xbox Die");
				PMSMPtr->Shutdown.Status = 0xFF;
			}
			Console::WriteLine("Waitcount: " + WaitCount);
			
		}
		//Console::WriteLine(controller.PressedA());

		controller.SetDeadzone(5000);
		// Scaling speed to -1 +1 and Steering to -40 +40
		double speed = (controller.RightThumbLocation().GetY() - MIN_JOY_STICK)/ (MAX_JOY_STICK - MIN_JOY_STICK) * (1 - (-1)) + (-1);
		double steering = (controller.LeftThumbLocation().GetX() - MIN_JOY_STICK) / (MAX_JOY_STICK - MIN_JOY_STICK) * (40 - (-40)) + (-40);
		XboxSMPtr->remoteSpeed = speed;
		XboxSMPtr->remoteSteering = steering;
		XboxSMPtr->routineShutdown = controller.PressedB();

		if (!controller.IsConnected())
		{
			XboxSMPtr->remoteSpeed = 0;
			XboxSMPtr->remoteSteering = 0;
			Console::WriteLine("Disconnected");
		}

		Thread::Sleep(20);

	}

	Console::WriteLine("Xbox Process terminated normally");
	//Console::ReadKey();
	return 0;
}