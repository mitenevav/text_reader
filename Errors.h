#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include "Data.h"

/*
Функция выводит сообщение об ошибке
Arguments:  (in)HWND
            (in/out)errorCode - код ошибки
*/
void DisplayMessageBox(HWND hwnd, int errorCode);

/*
тип ошибки
*/
enum Error{
    NO_ERROR_e, NO_INPUT_FILE_e, NO_READ_FILE_e, MEMORY_SHORTAGE_e,
};


#endif // ERRORS_H_INCLUDED
