using System;
using System.Runtime.InteropServices;

class Imports
{
  internal const string DLLName = "XInputInterface";

  [DllImport(DLLName)]
  public static extern uint XInputGetControllerState(uint userIndex, out ControllerState state);
  [DllImport(DLLName)]
  public static extern void XInputSetControllerState(uint userIndex, float leftMotorVib, float rightMotorVib);
  [DllImport(DLLName)]
  public static extern bool XInputControllerConnected(uint userIndex);
}

public struct Thumbstick {
  float x;
  float y;
}

public struct DPad {
  bool up;
  bool right;
  bool down;
  bool left;
}

public struct ControllerState {
  bool isConnected;
  bool a;
  bool b;
  bool x;
  bool y;
  bool start;
  bool back;
  bool rightBumper;
  bool leftBumper;
  bool rightThumbstickButton;
  bool leftThumbstickButton;
  DPad dPad;
  float rightTrigger;
  float leftTrigger;
  Thumbstick rightThumbstick;
  Thumbstick leftThumbstick;
}

public class ControllerState {
  public static ControllerState GetState(uint userIndex) {
    ControllerState state;
    return Imports.XInputGetControllerState(userIndex, out state);
  }

  public static void SetState(uint userIndex, float leftMotorVib, float rightMotorVib) {
    Imports.XInputSetControllerState(userIndex, leftMotorVib, rightMotorVib);
  }

  public static bool ControllerConnected(uint userIndex) {
    return Imports.XInputControllerConnected(userIndex);
  }
}
