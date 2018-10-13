#include "XInputInterface.h"

// Allows us to wrap the XInputGetState call; this returns a pointer to
// the wrapping method
typedef DWORD(*XInputGetStatePtr)(DWORD userIndex, XINPUT_STATE* state);
// Allows us to wrap the XInputSetState call; this returns a pointer to
// the wrapping method
typedef DWORD(*XInputSetStatePtr)(DWORD userIndex, XINPUT_VIBRATION* vibration);

class XInputApiLoader {
private:
  bool apiLoaded;
  HINSTANCE apiHandle;

public:
  void loadApi() {
    if (!apiLoaded) {
      // Try version 1.4 first
      apiHandle = LoadLibrary(L"xinput1_4.dll");
      // Then, try version 1.3
      if (apiHandle == NULL) {
        apiHandle = LoadLibrary(L"xinput1_3.dll");
      }
      // Then, try version 9.1
      if (apiHandle == NULL) {
        apiHandle = LoadLibrary(L"xinput9_1_0.dll");
      }
      if (apiHandle != NULL) {
        // Bind XInput calls to our pointers
        getStateInterface = (XInputGetStatePtr)GetProcAddress(apiHandle, "XInputGetState");
        setStateInterface = (XInputSetStatePtr)GetProcAddress(apiHandle, "XInputSetState");
        apiLoaded = true;
      }
    }
  }

  XInputGetStatePtr getStateInterface;
  XInputSetStatePtr setStateInterface;
};

XInputApiLoader apiLoader;
DWORD previousPackets[XUSER_MAX_COUNT];
CONTROLLER_STATE previousControllerStates[XUSER_MAX_COUNT];

DWORD XInputGetControllerState(DWORD userIndex, CONTROLLER_STATE* controllerState) {
  apiLoader.loadApi();
  XINPUT_STATE xinputState;
  ZeroMemory(&xinputState, sizeof(XINPUT_STATE));
  if (apiLoader.getStateInterface != NULL) {
    DWORD errorCode = apiLoader.getStateInterface(userIndex, &xinputState);
    // The state changed
    if (previousPackets[userIndex] != xinputState.dwPacketNumber) {
      buildControllerState(&xinputState, controllerState);
      previousPackets[userIndex] = xinputState.dwPacketNumber;
      previousControllerStates[userIndex] = *controllerState;
    }
    else {
      *controllerState = previousControllerStates[userIndex];
    }
    return errorCode;

  }
  return ERROR_DEVICE_NOT_CONNECTED;
}

void XInputSetControllerState(DWORD userIndex, float leftMotorVib, float rightMotorVib) {
  apiLoader.loadApi();
  // Make sure the vibration floats are between 0.0 and 1.0
  leftMotorVib = leftMotorVib < 0.0f ? 0.0f : (leftMotorVib > 1.0f ? 1.0f : leftMotorVib);
  rightMotorVib = rightMotorVib < 0.0f ? 0.0f : (rightMotorVib > 1.0f ? 1.0f : rightMotorVib);
  XINPUT_VIBRATION vibration = {
    (WORD)(leftMotorVib * USHORT_MAX_VALUE),
    (WORD)(rightMotorVib * USHORT_MAX_VALUE)
  };
  apiLoader.setStateInterface(userIndex, &vibration);
}

bool XInputControllerConnected(DWORD userIndex) {
  XINPUT_STATE xinputState;
  ZeroMemory(&xinputState, sizeof(XINPUT_STATE));
  if (apiLoader.getStateInterface != NULL) {
    return apiLoader.getStateInterface(userIndex, &xinputState) != ERROR_DEVICE_NOT_CONNECTED;
  }
  return false;
}

void buildControllerState(XINPUT_STATE* xinputState, CONTROLLER_STATE* controllerState) {
  // Buttons
  controllerState->A = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_A;
  controllerState->B = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_B;
  controllerState->X = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_X;
  controllerState->Y = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_Y;
  controllerState->START = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_START;
  controllerState->BACK = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_BACK;
  controllerState->LEFT_BUMPER = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER;
  controllerState->RIGHT_BUMPER = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
  controllerState->LEFT_THUMBSTICK_BUTTON = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB;
  controllerState->RIGHT_THUMBSTICK_BUTTON = xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB;
  DPAD dpad = {
    xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP,
    xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT,
    xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN,
    xinputState->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT
  };
  controllerState->DPAD = dpad;
  float leftXCoord = (float)xinputState->Gamepad.sThumbLX;
  float leftYCoord = (float)xinputState->Gamepad.sThumbLY;
  applyThumbstickDeadZone(&leftXCoord, &leftYCoord, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
  // Thumbsticks
  THUMBSTICK leftThumbstick = {
    leftXCoord,
    leftYCoord
  };
  controllerState->LEFT_THUMBSTICK = leftThumbstick;
  float rightXCoord = (float)xinputState->Gamepad.sThumbRX;
  float rightYCoord = (float)xinputState->Gamepad.sThumbRY;
  applyThumbstickDeadZone(&rightXCoord, &rightYCoord, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
  THUMBSTICK rightThumbstick = {
    rightXCoord,
    rightYCoord
  };
  controllerState->RIGHT_THUMBSTICK = rightThumbstick;
  // Triggers
  controllerState->LEFT_TRIGGER = applyTriggerDeadZone(xinputState->Gamepad.bLeftTrigger);
  controllerState->RIGHT_TRIGGER = applyTriggerDeadZone(xinputState->Gamepad.bRightTrigger);

}

void applyThumbstickDeadZone(float* xCoord, float* yCoord, int deadZone) {
  float magnitude = sqrt((*xCoord)*(*xCoord) + (*yCoord)*(*yCoord));
  float normalizedX = (*xCoord) / magnitude;
  float normalizedY = (*yCoord) / magnitude;
  float normalizedMagnitude = 0;
  // if thumbstick is outside the dead zone
  if (magnitude > deadZone) {
    if (magnitude > THUMBSTICK_MAGNITUDE_MAX) {
      magnitude = THUMBSTICK_MAGNITUDE_MAX;
    }
    magnitude -= deadZone;
    normalizedMagnitude = magnitude / (THUMBSTICK_MAGNITUDE_MAX - deadZone);
  }
  *xCoord = normalizedX * normalizedMagnitude;
  *yCoord = normalizedY * normalizedMagnitude;
}

float applyTriggerDeadZone(byte trigger) {
  if (trigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
    trigger = 0;
  }
  return trigger / UBYTE_MAX_VALUE;
}