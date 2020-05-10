#ifndef SCROLL_H_INCLUDED
#define SCROLL_H_INCLUDED


#include "Data.h"
#define MAX_SCROLL_POS 0xffff

/*
параметры полосы прокрутки
*/
typedef struct Scroll_t
{
    int iMaxWidth;      //максимальная ширина
    int iVscrollPos;    //позиция вертикального ползунка
    int iVscrollMax;    //макимальное значенине вертикального ползунка
    int iHscrollPos;    //позиция горизонтального ползунка
    int iHscrollMax;    //макимальное значенине горизонтального ползунка
    int coefScroll;     //коэффициент сжатия
} Scroll_t;

/*
Функция устанавливет в нужные значения параметры полсы прокрутки
Arguments:  (in)HWND
            (in/out)scroll - структура полосы прокрутки
            (in)workWind - структура представления
            (in)wind - параметры окна
*/
void SetScroll(HWND hwnd, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind);

/*
Функция обновляет значение вертикальной полосы прокрутки
Arguments:  (in)HWND
            (in/out)scroll - структура полосы прокрутки
            (in)workWind - структура представления
*/
void UpdateVScroll(Scroll_t* scroll, WorkWind_t* workWind);

/*
Функция обновляет допустимые значения полос прокрутки
Arguments:  (in)HWND
            (in/out)scroll - структура полосы прокрутки
            (in)workWind - структура представления
            (in)wind - параметры окна
*/
void UpdateScrollRange(HWND hwnd, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind);

/*
Функция реализует управление горизонтальной полосой прокрутки
Arguments:  (in)HWND
            (in)wParam
            (in/out)scroll - структура полосы прокрутки
            (in)wind - параметры окна
*/
void HScroll(HWND hwnd, WPARAM wParam, Scroll_t* scroll, WindParam_t* wind);

/*
Функция реализует управление вертикальной полосой прокрутки
Arguments:  (in)HWND
            (in)wParam
            (in/out)scroll - структура полосы прокрутки
            (in)workWind - структура представления
            (in)wind - параметры окна
*/
void VScroll(HWND hwnd, WPARAM wParam, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind);

/*
Функция обрабатывает нажатие клавиш
Arguments:  (in)HWND
            (in)wParam
*/
void Keydown(HWND hwnd, WPARAM wParam);

#endif // SCROLL_H_INCLUDED
