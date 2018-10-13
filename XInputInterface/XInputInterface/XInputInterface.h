#pragma once

#include <Xinput.h>
#include <math.h>
#include <Windows.h>

#ifdef XINPUT_EXPORTS
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

typedef struct _DPAD {
  bool UP;
  bool RIGHT;
  bool DOWN;
  bool LEFT;
}DPAD;

typedef struct _CONTROLLER_STATE {
  bool A;
  bool B;
  bool X;
  bool Y;
  bool START;
  bool BACK;
  bool RIGHT_BUMPER;
  bool LEFT_BUMPER;
  bool RIGHT_THUMBSTICK_BUTTON;
  bool LEFT_THUMBSTICK_BUTTON;
  DPAD DPAD;
  float RIGHT_TRIGGER;
  float LEFT_TRIGGER;
  THUMBSTICK RIGHT_THUMBSTICK;
  THUMBSTICK LEFT_THUMBSTICK;
} CONTROLLER_STATE;

extern "C" {
  XINPUT_EXPORTS DWORD XInputGetControllerState(DWORD userIndex, CONTROLLER_STATE* controllerState);
  XINPUT_EXPORTS void XInputSetControllerState(DWORD userIndex, float leftMotorVib, float rightMotorVib);
  XINPUT_EXPORTS bool XInputControllerConnected(DWORD userIndex);
  void buildControllerState(XINPUT_STATE* xinputState, CONTROLLER_STATE* controllerState);
  void applyThumbstickDeadZone(float* xCoord, float* yCoord, int deadZone);
  float applyTriggerDeadZone(byte trigger);
}