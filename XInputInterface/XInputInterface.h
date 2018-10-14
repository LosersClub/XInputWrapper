#pragma once

#include <Windows.h>
#include <Xinput.h>
#include <math.h>

#ifdef XINPUTINTERFACE_EXPORTS
#define XINPUT_EXPORTS __declspec(dllexport)
#else
#define XINPUT_EXPORTS __declspec(dllimport)
#endif

#define USHORT_MAX_VALUE 65535
#define UBYTE_MAX_VALUE 255
#define THUMBSTICK_MAGNITUDE_MAX 32767

typedef struct _THUMBSTICK {
  float X;
  float Y;
} THUMBSTICK;

typedef struct _CONTROLLER_STATE {
  bool IS_CONNECTED;
  WORD BUTTONS;
  float RIGHT_TRIGGER;
  float LEFT_TRIGGER;
  THUMBSTICK RIGHT_THUMBSTICK;
  THUMBSTICK LEFT_THUMBSTICK;
} CONTROLLER_STATE;

extern "C" {
  XINPUT_EXPORTS DWORD XInputGetControllerState(DWORD userIndex, CONTROLLER_STATE* controllerState);
  XINPUT_EXPORTS void XInputSetControllerState(DWORD userIndex, float leftMotorVib, float rightMotorVib);
  XINPUT_EXPORTS bool XInputControllerConnected(DWORD userIndex);
  void buildControllerState(XINPUT_STATE* xinputState, CONTROLLER_STATE* controllerState, DWORD errorCode);
  void applyThumbstickDeadZone(float* xCoord, float* yCoord, int deadZone);
  float applyTriggerDeadZone(byte trigger);
}