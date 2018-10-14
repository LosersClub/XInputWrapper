using System;
using System.Runtime.InteropServices;

namespace XInputWrapper {
  class Imports {
    internal const string DLLName = "XInputInterface";

    [DllImport(DLLName)]
    public static extern uint XInputGetControllerState(uint userIndex, out ControllerState state);
    [DllImport(DLLName)]
    public static extern void XInputSetControllerState(uint userIndex, float leftMotorVib, float rightMotorVib);
    [DllImport(DLLName)]
    public static extern bool XInputControllerConnected(uint userIndex);
  }

  public enum Button {
    A = 0x1000,
    B = 0x2000,
    X = 0x4000,
    Y = 0x8000,
    Dpad_Up = 0x0001,
    Dpad_Right = 0x0008,
    Dpad_Down = 0x0002,
    Dpad_Left = 0x0004,
    Start = 0x0010,
    Back = 0x0020,
    Right_Bumper = 0x0200,
    Left_Bumper = 0x0100,
    Right_Thumbstick_Button = 0x0080,
    Left_Thumbstick_Button = 0x0040
  };

  public enum Side {
    Right,
    Left
  };

  [StructLayout(LayoutKind.Sequential)]
  public struct Thumbstick {
    float x;
    float y;

    public float X {
      get { return x; }
    }

    public float Y {
      get { return y; }
    }
  }

  [StructLayout(LayoutKind.Sequential)]
  public struct ControllerState {
    [MarshalAs(UnmanagedType.I1)]
    bool isConnected;
    ushort buttons;
    float rightTrigger;
    float leftTrigger;
    Thumbstick rightThumbstick;
    Thumbstick leftThumbstick;

    internal ControllerState(bool isConnected, ushort buttons, float rightTrigger, float leftTrigger,
      Thumbstick rightThumbstick, Thumbstick leftThumbstick) {
      this.isConnected = isConnected;
      this.buttons = buttons;
      this.rightTrigger = rightTrigger;
      this.leftTrigger = leftTrigger;
      this.rightThumbstick = rightThumbstick;
      this.leftThumbstick = leftThumbstick;
    }

    public bool IsConnected {
      get { return isConnected; }
    }

    public ushort Buttons {
      get { return buttons; }
    }

    public float RightTrigger {
      get { return rightTrigger; }
    }

    public float LeftTrigger {
      get { return leftTrigger; }
    }

    public Thumbstick RightThumbstick {
      get { return rightThumbstick; }
    }

    public Thumbstick LeftThumbstick {
      get { return leftThumbstick; }
    }

    public bool ButtonStatus(Button button) {
      return (buttons & (ushort)button) != 0;
    }

    public float TriggerStatus(Side trigger) {
      return trigger == Side.Left ? leftTrigger : rightTrigger;
    }

    public Thumbstick ThumbstickStatus(Side thumbstick) {
      return thumbstick == Side.Left ? leftThumbstick : rightThumbstick;
    }
  }

  public class ControllerInput {
    public static ControllerState GetState(uint userIndex) {
      ControllerState state;
      Imports.XInputGetControllerState(userIndex, out state);
      return state;
    }

    public static void SetState(uint userIndex, float leftMotorVib, float rightMotorVib) {
      Imports.XInputSetControllerState(userIndex, leftMotorVib, rightMotorVib);
    }

    public static bool ControllerConnected(uint userIndex) {
      return Imports.XInputControllerConnected(userIndex);
    }
  }
}