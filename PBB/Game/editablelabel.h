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
#include "font.h"
#include "containercontrol.h"

/// <summary>
/// essentially behaves like a textbox with a single line. When the control is in edit mode, 
/// the user can type to add or remove text from the control in real-time. When not in edit mode,
/// the control functions like a regular label.
/// </summary>
public ref class EditableLabel : ContainerControl
{
private:
   

    // used as an index for the text array.
    unsigned int cursorPosition;

    // the maximum size the labels string can be.
    unsigned int maxLength;

    // the text to be rendered to the screen.
    array<unsigned char, 1> ^text;

    // determines whether the label is in edit mode (true) or "read only" mode (false).
    bool editMode;

    const unsigned int KEY_SPACE = 32;
    const unsigned int KEY_TILDE = 126;

    ::Font ^font;

    String ^navigateTo;

public:
    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="font"></param>
    /// <param name="length"></param>
    EditableLabel(int x, int y, int selectedIndex, String ^font, unsigned int length, MenuItemContainer ^parentContainer);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="selectedIndex"></param>
    /// <param name="font"></param>
    /// <param name="length"></param>
    /// <param name="navigateTo"></param>
    /// <param name="parentContainer"></param>
    EditableLabel(int x, int y, int selectedIndex, String ^font, unsigned int length, String ^navigateTo, MenuItemContainer ^parentContainer) :
        EditableLabel(x, y, selectedIndex, font, length, parentContainer)
    {
        if(nullptr == navigateTo)
        {
            throw gcnew ArgumentNullException("navigateTo");
        }
        
        this->navigateTo = navigateTo;
    }

    void ReceiveSceneArgs(array<Object ^, 1> ^args) override
    {
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="keyboardState"></param>
    /// <param name="mouseState"></param>
    void Update(Keys ^keyboardState, Mouse ^mouseState) override;

    /// <summary>
    /// 
    /// </summary>
    void Render() override;
};