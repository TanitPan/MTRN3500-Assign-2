#include "VehicleRef.h"

VehicleRef::~VehicleRef()
{
	Stream->Close();
	Client->Close();
}

VehicleRef::VehicleRef(System::String^ ipaddress, int port)
{
	IPAddress = ipaddress;
	PortNumber = port;
}

bool VehicleRef::Connect()
{
	
	// Instantiations
	Client = gcnew TcpClient(IPAddress, PortNumber);
	SendData = gcnew array<unsigned char>(40);

	// Configurations, set the read/write timeouts & buffer size
	Client->NoDelay = true;
	Client->ReceiveTimeout = 2000;//ms The number is given by Tutor 2000
	Client->SendTimeout = 500;//ms  The number is given by Tutor 2000
	Client->ReceiveBufferSize = 1024; // used to be 1024
	Client->SendBufferSize = 1024;


	// Get the network stream object associated with client so we 
	// can use it to read and write
	Stream = Client->GetStream();

	//Authentication
	System::String^ authen = gcnew System::String("5224642\n");
	SendData = System::Text::Encoding::ASCII->GetBytes(authen);
	Stream->Write(SendData, 0 ,SendData->Length);
	SendData = gcnew array<unsigned char>(50);

	return true;
}

void VehicleRef::ControlVehicle(double steering, double speed, unsigned int flag)
{
	//System::String^ SendCommand = gcnew System::String("# <+40> <+1> <1> #");
	System::String^ SendCommand = gcnew System::String("# " +steering.ToString("f3")+" "+speed.ToString("f3")+" "+flag+" #");

	Console::WriteLine(SendCommand);
	SendData = System::Text::Encoding::ASCII->GetBytes(SendCommand);
	Stream->WriteByte(0x02);
	Stream->Write(SendData, 0, SendData->Length);
	Stream->WriteByte(0x03);
	System::Threading::Thread::Sleep(120);
}

double VehicleRef::GetSpeed()
{
	return this->speed;
}

double VehicleRef::GetSteering()
{
	return this->steering;
}
