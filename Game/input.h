// Copyright(c) 2018 Seth Ballantyne <seth.ballantyne@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files(the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons
// to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies or
// substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
// FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
#pragma once

#include <windows.h>
#include <dinput.h>
#include "keys.h"
#include "logmanager.h"
#include "mouse.h"
#include "exceptions.h"

using namespace System;

/// <summary>
/// Wrapper around DirectInput. Input::Initalise() must be called before any of its other methods can be called.
/// In order to release the resources allocated by Initialise(), call Input::Shutdown(). Once Shutdown has been
/// called, no methods can be used until the class is initialised again.
/// </summary>
public ref class Input abstract sealed
{
private:
    static LPDIRECTINPUT8 lpDI;

    static LPDIRECTINPUTDEVICE8 lpDIKeyboard;
    static LPDIRECTINPUTDEVICE8 lpDIMouse;

    // If a key is present in this table, it can be bound by the user to a particular action 
	// in the game. the int argument is the DIK_* scan code, the string is the text description 
	// of the key as it'll appear within the game when binding the key.
    static Dictionary<int, String ^> ^lookupTable = gcnew Dictionary<int, String ^>();

    // the previous state of the keyboard is stored because it's required when
    // checking to see if a key has been pressed: previous state to see if it was down the last time
    // the keyboard state was retrieved and the current state to see if it's now up.
    static array<UCHAR, 1> ^previousKeyboardState;
    static array<UCHAR, 1> ^currentKeyboardState;

    static HKL keyboardLayout;

    /// <summary>
    /// Requests that DirectInput have access to the specified device.
    /// </summary>
    /// <param name="lpDIDevice">the device to request access to.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException"></exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNotInitializedException">the object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    static void AcquireDevice(LPDIRECTINPUTDEVICE8 lpDIDevice);

    /// <summary>
    /// Puts the keyboard into a useable state.
    /// </summary>
    /// <param name="hWnd">handle of the window that will be capturing input.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectINput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputAcquiredException">the operation cannot be performed while the device is acquired.</exception>
    /// <exception cref="DirectInputDeviceNotRegisteredException">the device or device instance is not registered with DirectInput.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNoInterfaceException">the object does not support the specified interface.</exception>
    /// <exception cref="DirectInputNotInitializedException">this object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    /// <exception cref="OutOfMemoryException">not enough memory available to complete the operation.</exception>
    static void InitKeyboard(HWND hWnd);

    /// <summary>
    /// Populates the table with string descriptions of specific keys.
    /// 
    /// </summary>
    static void InitLookupTable()
    {
		// fix for issue #56: ESCAPE is reserved for bringing up the main menu during gameplay.
        //lookupTable->Add(DIK_ESCAPE, "ESCAPE")
		lookupTable->Add(0, "LEFT MOUSE BUTTON");
		lookupTable->Add(1, "RIGHT MOUSE BUTTON");
        lookupTable->Add(DIK_1, "1");
        lookupTable->Add(DIK_2, "2");
        lookupTable->Add(DIK_3,"3");
        lookupTable->Add(DIK_4, "4");
        lookupTable->Add(DIK_5, "5");
        lookupTable->Add(DIK_6, "6");
        lookupTable->Add(DIK_7, "7");
        lookupTable->Add(DIK_8, "8");
        lookupTable->Add(DIK_9, "9");
        lookupTable->Add(DIK_0, "0");
        lookupTable->Add(DIK_MINUS, "-");
        lookupTable->Add(DIK_EQUALS, "=");
        lookupTable->Add(DIK_BACK, "BACKSPACE");
        lookupTable->Add(DIK_TAB, "TAB");
        lookupTable->Add(DIK_Q, "Q");
        lookupTable->Add(DIK_W,  "W");
        lookupTable->Add(DIK_E, "E");
        lookupTable->Add(DIK_R, "R");
        lookupTable->Add(DIK_T, "T");
        lookupTable->Add(DIK_Y, "Y");
        lookupTable->Add(DIK_U, "U");
        lookupTable->Add(DIK_I, "I");
        lookupTable->Add(DIK_O, "O");
        lookupTable->Add(DIK_P, "P"); 
        lookupTable->Add(DIK_LBRACKET, "[");
        lookupTable->Add(DIK_RBRACKET, "]");
        lookupTable->Add(DIK_RETURN, "ENTER"); // Enter on main keyboard
        lookupTable->Add(DIK_LCONTROL, "LEFT CTRL");
        lookupTable->Add(DIK_A, "A");
        lookupTable->Add(DIK_S, "S");
        lookupTable->Add(DIK_D, "D");
        lookupTable->Add(DIK_F, "F");
        lookupTable->Add(DIK_G, "G");
        lookupTable->Add(DIK_H, "H");
        lookupTable->Add(DIK_J, "J");
        lookupTable->Add(DIK_K, "K");
        lookupTable->Add(DIK_L, "L");
        lookupTable->Add(DIK_SEMICOLON, ";");
        lookupTable->Add(DIK_APOSTROPHE, "\'");
        lookupTable->Add(DIK_GRAVE, "`");
        lookupTable->Add(DIK_LSHIFT, "LEFT SHIFT");
        lookupTable->Add(DIK_BACKSLASH, "\\");
        lookupTable->Add(DIK_Z, "Z");
        lookupTable->Add(DIK_X, "X");
        lookupTable->Add(DIK_C, "C");
        lookupTable->Add(DIK_V, "V");
        lookupTable->Add(DIK_B, "B");
        lookupTable->Add(DIK_N, "N");
        lookupTable->Add(DIK_M, "M");
        lookupTable->Add(DIK_COMMA, ",");
        lookupTable->Add(DIK_PERIOD, "."); //.on main keyboard
        lookupTable->Add(DIK_SLASH, "/"); // / on main keyboard
        lookupTable->Add(DIK_RSHIFT, "RIGHT SHIFT");
        lookupTable->Add(DIK_MULTIPLY, "*"); // * on numeric keypad
        lookupTable->Add(DIK_LMENU, "LEFT ALT"); //left Alt
        lookupTable->Add(DIK_SPACE, "SPACE");
        lookupTable->Add(DIK_CAPITAL, "CAPS LOCK");
        lookupTable->Add(DIK_F1, "F1");
        lookupTable->Add(DIK_F2, "F2");
        lookupTable->Add(DIK_F3, "F3");
        lookupTable->Add(DIK_F4, "F4");
        lookupTable->Add(DIK_F5, "F5");
        lookupTable->Add(DIK_F6, "F6");
        lookupTable->Add(DIK_F7, "F7");
        lookupTable->Add(DIK_F8, "F8");
        lookupTable->Add(DIK_F9, "F9");
        lookupTable->Add(DIK_F10, "F10");
        lookupTable->Add(DIK_NUMLOCK, "NUM LOCK");
        lookupTable->Add(DIK_NUMPAD7, "NUMPAD 7");
        lookupTable->Add(DIK_NUMPAD8, "NUMPAD 8");
        lookupTable->Add(DIK_NUMPAD9, "NUMPAD 9");
        lookupTable->Add(DIK_SUBTRACT, "NUMPAD -"); // - on numeric keypad
        lookupTable->Add(DIK_NUMPAD4, "NUMPAD 4");
        lookupTable->Add(DIK_NUMPAD5, "NUMPAD 5");
        lookupTable->Add(DIK_NUMPAD6, "NUMPAD 6");
        lookupTable->Add(DIK_ADD, "NUMPAD +"); // + on numeric keypad
        lookupTable->Add(DIK_NUMPAD1, "NUMPAD 1");
        lookupTable->Add(DIK_NUMPAD2, "NUMPAD 2");
        lookupTable->Add(DIK_NUMPAD3, "NUMPAD 3");
        lookupTable->Add(DIK_NUMPAD0, "NUMPAD 0");
        lookupTable->Add(DIK_DECIMAL, "NUMPAD ."); //.on numeric keypad
        lookupTable->Add(DIK_F11, "F11");
        lookupTable->Add(DIK_F12, "F12");
        lookupTable->Add(DIK_NUMPADENTER, "NUMPAD ENTER"); //Enter on numeric keypad
        lookupTable->Add(DIK_DIVIDE, "NUMPAD /"); // / on numeric keypad
        lookupTable->Add(DIK_RMENU, "RIGHT ALT"); //right Alt
        lookupTable->Add(DIK_PAUSE, "PAUSE");
        lookupTable->Add(DIK_HOME, "HOME"); //Home on arrow keypad
        lookupTable->Add(DIK_UP, "UP"); //up arrow
        lookupTable->Add(DIK_PRIOR, "PAGE UP");
        lookupTable->Add(DIK_LEFT, "LEFT"); //left arrow
        lookupTable->Add(DIK_RIGHT, "RIGHT"); //right arrow
        lookupTable->Add(DIK_END, "END");
        lookupTable->Add(DIK_DOWN, "DOWN"); //down arrow
        lookupTable->Add(DIK_NEXT, "PAGE DOWN");
        lookupTable->Add(DIK_INSERT, "INSERT");
        lookupTable->Add(DIK_DELETE, "DELETE");
        
    }

    /// <summary>
    /// Puts the mouse into a useable state.
    /// </summary>
    /// <param name="hWnd">handle of the window that will be capturing input.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectINput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputAcquiredException">the operation cannot be performed while the device is acquired.</exception>
    /// <exception cref="DirectInputDeviceNotRegisteredException">the device or device instance is not registered with DirectInput.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNoInterfaceException">the object does not support the specified interface.</exception>
    /// <exception cref="DirectInputNotInitializedException">this object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    /// <exception cref="System::OutOfMemoryException">not enough memory available to complete the operation.</exception>
    static void InitMouse(HWND hWnd);

    /// <summary>
    /// Releases the sppecified device. Once this is called, the device is unavailable for use.
    /// </summary>
    /// <param name="device">the DirectInput device to release.</param>
    static void SafeRelease(LPDIRECTINPUTDEVICE8 device)
    {
        if(device != NULL)
        {
            device->Unacquire();
            device->Release();
            device = NULL;
        }
    }
public:

    /// <summary>
    /// Converts the specified key in the form of a DIK_* scancode to an ASCII value.
    /// </summary>
    /// <param name="dikCode">the scancode of the key.</param>
    /// <returns><b>0</b> if the specified scancode can't be converted, otherwise the ASCII value of the key.</returns>
    /// <remarks>DIKtoASCII queries the keyboard and takes the shift keys into account when converting to ASCII so on a standard US Keyboard,
    /// specifying DIK_2 with the shift key not pressed down will return '2', while
    /// specifying the same value with the shift key pressed will return '@'.</remarks>
    static unsigned int DIKToASCII(unsigned int dikCode)
    {
        UCHAR keyboardState[256];
        USHORT result[2] = { 0, 0 }; 

        // I'm cheating, when passing it to ToAsciiEx(), it'll take into 
        // account whether shift is pressed, returning the correct ascii code.
        // Using this->currentKeyboardState means extra work, as it doesn't take shift into account.
        // (and somewhat shows that DirectInput is redundant for run-of-the-mill keyboard input).
        if(!GetKeyboardState(keyboardState))
        {
            return 0;
        }
        
        int scanCode = MapVirtualKeyEx(dikCode, MAPVK_VSC_TO_VK, keyboardLayout);
        ToAsciiEx(scanCode, dikCode, keyboardState, result, 0, keyboardLayout);

        return result[0];
    }

    /// <summary>
    /// Returns a string description of the specified DIK_* key code.
    /// </summary>
    /// <param name="dikCode">the DIK_* code of the desired description.</param>
    /// <returns>the description of the specified DIK_* code, or <b>null</b> if the keycode isn't present
    /// within the lookup table.</returns>
    static String ^GetDIKAsString(unsigned char dikCode)
    {
        try
        {
            return lookupTable[dikCode];
        }
        catch(...)
        {
            return nullptr;
        }
    }

    /// <summary>
    /// Initialises DirectInput and puts the mouse and keyboard into a useable state. Initialise() must be called before
    /// any other methods in the Input class. Once you're done with the Input class, call Shutdown() to release the resources
    /// allocated by Initialise().
    /// </summary>
    /// <param name="hInstance">the instance handle of the application.</param>
    /// <param name="hWnd">the handle of the window that'll be capturing input.</param>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectINput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputAcquiredException">the operation cannot be performed while the device is acquired.</exception>
    /// <exception cref="DirectInputDeviceNotRegisteredException">the device or device instance is not registered with DirectInput.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNoInterfaceException">the object does not support the specified interface.</exception>
    /// <exception cref="DirectInputNotInitializedException">this object has not been initialized.</exception>
    /// <exception cref="DirectInputOldVersionException">DirectX 8 required.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    /// <exception cref="System::OutOfMemoryException">not enough memory available to complete the operation.</exception>
    static void Initialise(HINSTANCE hInstance, HWND hWnd);

    /// <summary>
    /// Captures the current state of the keys on the keyboard.
    /// </summary>
    /// <returns></returns>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectInput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputDeviceLostException">access to the input device has been lost. It must be reacquired.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed to the function, or the object is an invalid state.</exception>
    /// <exception cref="DirectInputDeviceNotAcquiredException">attempting to use the device when it hasn't been acquired.</exception>
    /// <exception cref="DirectInputNotInitializedException">object hasn't been initialised.</exception>
    static Keys ^ReadKeyboard();

    /// <summary>
    /// Captures the current state of the mouse and its buttons.
    /// </summary>
    /// <returns></returns>
    /// <exception cref="System::Runtime::InteropServices::COMException">DirectInput returned an unspecified COM+ error.</exception>
    /// <exception cref="DirectInputDeviceLostException">access to the input device has been lost. It must be reacquired.</exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed to the function, or the object is an invalid state.</exception>
    /// <exception cref="DirectInputDeviceNotAcquiredException">attempting to use the device when it hasn't been acquired.</exception>
    /// <exception cref="DirectInputNotInitializedException">object hasn't been initialised.</exception>
    static Mouse ^ReadMouse();

    static void Release()
    {
        SafeRelease(lpDIKeyboard);
        SafeRelease(lpDIMouse);

        if(lpDI != NULL)
        {
            lpDI->Release();
            lpDI = NULL;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <exception cref="System::Runtime::InteropServices::COMException"></exception>
    /// <exception cref="DirectInputInvalidParameterException">an invalid parameter was passed or the object wasn't in a callable state (null).</exception>
    /// <exception cref="DirectInputNotInitializedException">the object has not been initialized.</exception>
    /// <exception cref="DirectInputOtherApplicationHasPriorityException">another application has a higher priority level, preventing this call from succeeding.</exception>
    static void Restore()
    {
        try
        {
            AcquireDevice(lpDIKeyboard);
            AcquireDevice(lpDIMouse);
        }
        catch(...)
        {
            throw;
        }
    }

    /// <summary>
    /// Shutsdown DirectInput and releases the resources allocated by Initialise(). Once this method is called,
    /// no methods belonging to Input can be called until it's initialised again.
    /// </summary>
    static void Shutdown()
    {
        Release();
    }
};