#include "Errors.h"
#include <string.h>
void DisplayMessageBox(HWND hwnd, int errorCode)
{
    char buff [30];
    switch(errorCode)
    {
    case NO_INPUT_FILE_e:
        strcpy(buff,"File not found!");
        break;
    case NO_READ_FILE_e:
        strcpy(buff,"File not read!");
        break;
    case MEMORY_SHORTAGE_e:
        strcpy(buff,"Not enough memory!");
        break;
    default:
        strcpy(buff,"Unexpected error!");

    }
    MessageBox(hwnd, buff, "Error Details", MB_ICONERROR | MB_OK);
}
