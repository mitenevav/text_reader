#include "Scroll.h"
#include <windows.h>
#include <math.h>
#include "Errors.h"

void SetScroll(HWND hwnd, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind)
{
    UpdateScrollRange(hwnd, scroll, workWind, wind);

    UpdateVScroll(scroll, workWind);


    SetScrollPos(hwnd, SB_VERT, scroll->iVscrollPos, TRUE);


    SetScrollPos(hwnd, SB_HORZ, scroll->iHscrollPos, TRUE);

}

void UpdateVScroll(Scroll_t* scroll, WorkWind_t* workWind)
{
    long i;
    i=FindFirstLine(workWind);
    scroll->iVscrollPos = i / scroll->coefScroll;
    workWind->iFirstLine=i;
}

void UpdateScrollRange(HWND hwnd, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind)
{
    long iMaxWidth = maxLineLength(workWind);

    int cyClient,cyChar, cxClient,cxChar;

    cyClient=wind->cyClient;
    cxClient=wind->cxClient;
    cxChar=wind->cxChar;
    cyChar=wind->cyChar;
    if(workWind->numOfLines - cyClient/cyChar + 1 > MAX_SCROLL_POS)
    {
        scroll->coefScroll = ceil(((long double)(workWind->numOfLines - cyClient/cyChar + 1) / MAX_SCROLL_POS));

    }
    else
    {
        //scroll->iVscrollMax = max(0, workWind->numOfLines - cyClient/cyChar + 1);
        scroll->coefScroll=1;
    }

    scroll->iVscrollMax = max(0, ceil((long double)(workWind->numOfLines - cyClient/cyChar + 1) / scroll->coefScroll));

    scroll->iHscrollMax = max(0, iMaxWidth - cxClient/cxChar);

    scroll->iVscrollPos = min(scroll->iVscrollPos, scroll->iVscrollMax);

    scroll->iHscrollPos = min(scroll->iHscrollPos, scroll->iHscrollMax);

    SetScrollRange(hwnd, SB_VERT, 0, scroll->iVscrollMax, FALSE);

    SetScrollRange(hwnd, SB_HORZ, 0, scroll->iHscrollMax, FALSE);
}

void HScroll(HWND hwnd, WPARAM wParam, Scroll_t* scroll, WindParam_t* wind)
{
    int iHscrollInc;
    switch(LOWORD(wParam))
    {
    case SB_LINEUP :
        iHscrollInc = -1;
        break;

    case SB_LINEDOWN :
        iHscrollInc = 1;
        break;

    case SB_PAGEUP :
        iHscrollInc = -8;
        break;

    case SB_PAGEDOWN :
        iHscrollInc = 8;
        break;

    case SB_THUMBTRACK :
        iHscrollInc = HIWORD(wParam) - scroll->iHscrollPos;
        break;

    default :
        iHscrollInc = 0;
    }

    iHscrollInc = max(-scroll->iHscrollPos, min(iHscrollInc, scroll->iHscrollMax - scroll->iHscrollPos));

    if (iHscrollInc != 0)
    {
        scroll->iHscrollPos += iHscrollInc;
        ScrollWindow(hwnd, -wind->cxChar * iHscrollInc, 0, NULL, NULL);
        SetScrollPos(hwnd, SB_HORZ, scroll->iHscrollPos, TRUE);

        UpdateWindow(hwnd);
    }
}
void VScroll(HWND hwnd, WPARAM wParam, Scroll_t* scroll, WorkWind_t* workWind, WindParam_t* wind)
{
    int iVscrollInc;
    switch(LOWORD(wParam))
    {
    case SB_TOP :
        iVscrollInc = -scroll->iVscrollPos;
        break;

    case SB_BOTTOM :
        iVscrollInc = scroll->iVscrollMax - scroll->iVscrollPos;
        break;

    case SB_LINEUP :
        iVscrollInc = -1;
        break;

    case SB_LINEDOWN :
        iVscrollInc = 1;
        break;

    case SB_PAGEUP :
        iVscrollInc = min(-1, -wind->cyClient / wind->cyChar);
        break;

    case SB_PAGEDOWN :
        iVscrollInc = max(1, wind->cyClient / wind->cyChar);
        break;

    case SB_THUMBTRACK :
        iVscrollInc = HIWORD(wParam) - scroll->iVscrollPos;
        break;

    default :
        iVscrollInc = 0;
    }

    iVscrollInc = max(-scroll->iVscrollPos, min(iVscrollInc, scroll->iVscrollMax - scroll->iVscrollPos));

    if (iVscrollInc != 0)
    {
        scroll->iVscrollPos += iVscrollInc;
        ScrollWindow(hwnd, 0, -wind->cyChar * (scroll->iVscrollPos*scroll->coefScroll - workWind->iFirstLine), NULL, NULL);
        SetScrollPos(hwnd, SB_VERT, scroll->iVscrollPos, TRUE);

        workWind->iFirstLine= scroll->iVscrollPos * scroll->coefScroll;
        workWind->firstCh=workWind->lengthLine[workWind->iFirstLine];

        UpdateWindow(hwnd);
    }
}
void Keydown(HWND hwnd, WPARAM wParam)
{
    switch(wParam)
    {
    case VK_HOME :
        SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0L);
        break;
    case VK_END :
        SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0L);
        break;
    case VK_PRIOR :
        SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
        break;
    case VK_NEXT :
        SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
        break;
    case VK_UP :
        SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
        break;
    case VK_DOWN :
        SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
        break;
    case VK_LEFT :
        SendMessage(hwnd, WM_HSCROLL, SB_PAGEUP, 0L);
        break;
    case VK_RIGHT :
        SendMessage(hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L);
        break;
    }
}
