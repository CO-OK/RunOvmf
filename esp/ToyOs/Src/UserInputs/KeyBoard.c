#include <KeyBoard.h>

bool isLeftShiftPressed;
bool isRightShiftPressed;

void HandleKeyboard(uint8_t scancode){

    switch (scancode){
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
        case Enter:
            NextLine();
            return;
        case Spacebar:
            putchar(' ');
            return;
        case BackSpace:
            Backspace();
           //GlobalRenderer->ClearChar();
           return;
    }

    char ascii = Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0){
        putchar(ascii);
        //GlobalRenderer->PutChar(ascii);
    }

}