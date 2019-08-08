#ifndef XBOX_CONTROLLER_H
#define XBOX_CONTROLLER_H

#ifdef _WIN32
  #include <Windows.h>
#endif

#include "XInputWrapper.h"
//Definitions for setting vibration
constexpr auto bUP = 0x0001;
constexpr auto bDOWN = 0x0002;
constexpr auto bLEFT = 0x0004;
constexpr auto bRIGHT = 0x0008;
constexpr auto bSTART = 0x0010;
constexpr auto bBACK =  0x0020;
constexpr auto bLT = 0x0040;
constexpr auto bRT = 0x0080;
constexpr auto bLS = 0x0100;
constexpr auto bRS = 0x0200;
constexpr auto bA = 0x1000;
constexpr auto bB = 0x2000;
constexpr auto bX = 0x4000;
constexpr auto bY = 0x8000;
/*
 * For MTRN2500 Students - These methods and their respective signatures must not be changed. If 
 * they are and human intervention is required then marks will be deducted. You are more than
 * welcome to add private member variables and private methods to the provided classes, or create
 * your own underlying classes to provide the requested functionality as long as all of the
 * functionality for the XBoxController can be accessed using the methods in the provided classes.
 */

namespace GamePad {
  class Coordinate {
    public:
      Coordinate(SHORT x, SHORT y) : x(x), y(y) {}
      ~Coordinate() = default;

      SHORT GetX() { return this->x; }
      SHORT GetY() { return this->y; }
      void SetX(SHORT x) { this->x = x; }
      void SetY(SHORT y) { this->y = y; }

    private:
      SHORT x;
      SHORT y;
  };

  class XBoxController {
    public:
      XBoxController(XInputWrapper* xinput, DWORD id);
      ~XBoxController() = default;

      // methods which report information relating to the specific controller
      DWORD GetControllerId();

      // methods which report particular aspects of the controller status
      bool IsConnected();
      bool PressedA();
      bool PressedB();
      bool PressedX();
      bool PressedY();
      bool PressedLeftShoulder();
      bool PressedRightShoulder();
      bool PressedLeftDpad();
      bool PressedRightDpad();
      bool PressedUpDpad();
      bool PressedDownDpad();
      bool PressedStart();
      bool PressedBack();
      bool PressedLeftThumb();
      bool PressedRightThumb();
	  BYTE LeftTriggerLocation();
      BYTE RightTriggerLocation();

      GamePad::Coordinate LeftThumbLocation();
      GamePad::Coordinate RightThumbLocation();
	  DWORD VibrateButton;

      // methods which set values that modify the behaviour of the controller
      void Vibrate(WORD left, WORD right);
      void SetDeadzone(unsigned int radius);
	  
	  // additional methods
	  // Get current buttons pressed
	  WORD ButtonPressed();
	  // Set buttons pressed that should cause vibration
	  void SetVibrateButton(DWORD buttonPressed);
	  

    private:
      // NOTE: All XInput function calls must be made through the XInputWrapper class
      XInputWrapper* xinput;
	  DWORD id;
	  // additional variable for DeadZone radius
	  unsigned int Deadzone;
  };
};
//NON-MEMBER FUNCTIONS
// starts console output and return TRUE or FALSE to check if all controllers are disconnected while the 'while'
// loop is active. If return FALSE, exits while loop in the main function
bool StartConsole(GamePad::XBoxController player);

// Check controller connections at the start. Takes in XBoxController object and a counter. 
// Counts to 3 if all controllers are connnected
int CheckConnection(GamePad::XBoxController &player, int &instance);

#endif // XBOX_CONTROLLER_H
