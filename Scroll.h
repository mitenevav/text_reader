#ifndef SCROLL_H_INCLUDED
#define SCROLL_H_INCLUDED


#include "Data.h"
#define MAX_SCROLL_POS 0xffff

/*
��������� ������ ���������
*/
typedef struct Scroll_t
{
    int iMaxWidth;      //������������ ������
    int iVscrollPos;    //������� ������������� ��������
    int iVscrollMax;    //����������� ��������� ������������� ��������
    int iHscrollPos;    //������� ��������������� ��������
    int iHscrollMax;    //����������� ��������� ��������������� ��������
    int coefScroll;     //����������� ������
} Scroll_t;

/*
������� ������������ � ������ �������� ��������� ����� ���������
Arguments:  (in)HWND
            (in/out)scroll - ��������� ������ ���������
            (in)workWind - ��������� �������������
            (in)wind - ��������� ����
*/
void SetScroll(HWND hwnd, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind);

/*
������� ��������� �������� ������������ ������ ���������
Arguments:  (in)HWND
            (in/out)scroll - ��������� ������ ���������
            (in)workWind - ��������� �������������
*/
void UpdateVScroll(Scroll_t* scroll, WorkWind_t* workWind);

/*
������� ��������� ���������� �������� ����� ���������
Arguments:  (in)HWND
            (in/out)scroll - ��������� ������ ���������
            (in)workWind - ��������� �������������
            (in)wind - ��������� ����
*/
void UpdateScrollRange(HWND hwnd, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind);

/*
������� ��������� ���������� �������������� ������� ���������
Arguments:  (in)HWND
            (in)wParam
            (in/out)scroll - ��������� ������ ���������
            (in)wind - ��������� ����
*/
void HScroll(HWND hwnd, WPARAM wParam, Scroll_t* scroll, WindParam_t* wind);

/*
������� ��������� ���������� ������������ ������� ���������
Arguments:  (in)HWND
            (in)wParam
            (in/out)scroll - ��������� ������ ���������
            (in)workWind - ��������� �������������
            (in)wind - ��������� ����
*/
void VScroll(HWND hwnd, WPARAM wParam, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind);

/*
������� ������������ ������� ������
Arguments:  (in)HWND
            (in)wParam
*/
void Keydown(HWND hwnd, WPARAM wParam);

#endif // SCROLL_H_INCLUDED
