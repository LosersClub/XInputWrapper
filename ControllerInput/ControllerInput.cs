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
    A,
    B,
    X,
    Y,
    Dpad_Up,
    Dpad_Right,
    Dpad_Down,
    Dpad_Left,
    Start,
    Back,
    Right_Bumper,
    Left_Bumper,
    Right_Thumbstick_Button,
    Left_Thumbstick_Button
  };

  // Thumbsticks
  public enum Analog {
    Right,
    Left
  };

  public enum Trigger {
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
  public struct DPad {
    bool up;
    bool right;
    bool down;
    bool left;

    public bool Up
    {
      get { return up; }
    }

    public bool Right
    {
      get { return right; }
    }

    public bool Down
    {
      get { return down; }
    }

    public bool Left
    {
      get { return up; }
    }
  }

  [StructLayout(LayoutKind.Sequential)]
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

    internal ControllerState(bool isConnected, bool a, bool b, bool x, bool y,
      bool start, bool back, bool rightBumper, bool leftBumper, bool rightThumbstickButton,
      bool leftThumbstickButton, DPad dPad, float rightTrigger, float leftTrigger,
      Thumbstick rightThumbstick, Thumbstick leftThumbstick)
    {
      this.isConnected = isConnected;
      this.a = a;
      this.b = b;
      this.x = x;
      this.y = y;
      this.start = start;
      this.back = back;
      this.rightBumper = rightBumper;
      this.leftBumper = leftBumper;
      this.rightThumbstickButton = rightThumbstickButton;
      this.leftThumbstickButton = leftThumbstickButton;
      this.dPad = dPad;
      this.rightTrigger = rightTrigger;
      this.leftTrigger = leftTrigger;
      this.rightThumbstick = rightThumbstick;
      this.leftThumbstick = leftThumbstick;
    }

    public bool IsConnected
    {
      get { return isConnected; }
    }

    public bool A
    {
      get { return a; }
    }

    public bool B
    {
      get { return b; }
    }

    public bool X
    {
      get { return x; }
    }

    public bool Y
    {
      get { return y; }
    }

    public bool Start
    {
      get { return start; }
    }

    public bool Back
    {
      get { return back; }
    }

    public bool RightBumper
    {
      get { return rightBumper; }
    }

    public bool LeftBumper
    {
      get { return leftBumper; }
    }

    public bool RightThumbstickButton
    {
      get { return rightThumbstickButton; }
    }

    public bool LeftThumbstickButton
    {
      get { return leftThumbstickButton; }
    }

    public DPad DPad
    {
      get { return dPad; }
    }

    public float RightTrigger
    {
      get { return rightTrigger; }
    }

    public float LeftTrigger
    {
      get { return leftTrigger; }
    }

    public Thumbstick RightThumbstick
    {
      get { return rightThumbstick; }
    }

    public Thumbstick LeftThumbstick
    {
      get { return leftThumbstick; }
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

    public static bool ButtonStatus(ControllerState state, Button button) {
      switch (button) {
        case Button.A:
          return state.A;
        case Button.B:
          return state.B;
        case Button.X:
          return state.X;
        case Button.Y:
          return state.Y;
        case Button.Dpad_Up:
          return state.DPad.Up;
        case Button.Dpad_Right:
          return state.DPad.Right;
        case Button.Dpad_Down:
          return state.DPad.Down;
        case Button.Dpad_Left:
          return state.DPad.Left;
        case Button.Start:
          return state.Start;
        case Button.Back:
          return state.Back;
        case Button.Right_Bumper:
          return state.RightBumper;
        case Button.Left_Bumper:
          return state.LeftBumper;
        case Button.Right_Thumbstick_Button:
          return state.RightThumbstickButton;
        // Left_Thumbstick_Button; default required
        default:
          return state.LeftThumbstickButton;
      }
    }

    public static float TriggerStatus(ControllerState state, Trigger trigger) {
      return trigger == Trigger.Left ? state.LeftTrigger : state.RightTrigger;
    }

    public static Thumbstick ThumbstickStatus(ControllerState state, Analog thumbstick) {
      return thumbstick == Analog.Left ? state.LeftThumbstick : state.RightThumbstick;
    }
  }
}