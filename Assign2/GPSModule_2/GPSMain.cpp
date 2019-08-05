
#include "GPSRef.h"
#include <SMObject.h>
#include <SMStructs.h>
#define WAIT_TIME 200 // may need some modification later
//#using <System.dll>

//using namespace System;
//using namespace System::Threading;
//using namespace System::Net;
//using namespace System::Text;
//using namespace System::Net::Sockets;




int main()
{
	SMObject PMObj(_TEXT("PMObj"), sizeof(PM));
	SMObject GPSObj(_TEXT("GPSObj"), sizeof(GPS));

	PM* PMSMPtr = nullptr;
	GPS* GPSSMPtr = nullptr;

	int WaitCount = 0;


	PMObj.SMAccess();
	if (PMObj.SMAccessError) {
		Console::WriteLine("Shared memory access failed");
		return -2;
	}

	//GPSObj.SMAccess();
	//if (GPSObj.SMAccessError) {
	//	Console::WriteLine("Shared memory access failed LaserObj");
	//	Console::ReadKey();
	//	return -2;
	//}


	//Console::ReadKey();
	Console::WriteLine("GPS terminated normally.");
	//Console::ReadKey();

	return 0;
}