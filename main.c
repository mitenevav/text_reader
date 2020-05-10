#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
#include "Data.h"
#include "MyMenu.h"
#include "Scroll.h"
#include <commdlg.h>
#include "Errors.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{


    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "MyMenu";                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground =(HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               CW_USEDEFAULT,       /* The programs width */
               CW_USEDEFAULT,       /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               lpszArgument                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}




/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    RECT rect;
    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    static Data_t data;
    static WorkWind_t workWind;
    int y, iPaintBeg, iPaintEnd;
    static Scroll_t scroll;
    static WindParam_t wind;
    int err;


    switch(message)
    {
    case WM_CREATE:
    {
        CREATESTRUCT*pCS=(CREATESTRUCT*) lParam;
        char* file =(char*) pCS->lpCreateParams;

        hdc = GetDC(hwnd);
        SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));

        GetTextMetrics(hdc, &tm);
        wind.cxChar = tm.tmAveCharWidth;
        wind.cyChar = tm.tmHeight + tm.tmExternalLeading;

        GetClientRect(hwnd, &rect);
        wind.cxClient = rect.right;
        wind.cyClient = rect.bottom;


        ReleaseDC(hwnd, hdc);

        err=processFile(file, &data);
        if(err)
        {
            DisplayMessageBox(hwnd, err);
            return -1;
        }

        err=initModelOfData(&data, &workWind);
        if(err)
        {
            DisplayMessageBox(hwnd, err);
            return -1;
        }

        return 0;
    }
    case WM_SIZE :
    {
        wind.cxClient = LOWORD(lParam);
        wind.cyClient = HIWORD(lParam);

        if(workWind.wrap==WRAP)
        {
            workWind.firstCh=workWind.lengthLine[workWind.iFirstLine];
            err=makeWrap(&data, &workWind, &wind);
            if(err)
            {
                DisplayMessageBox(hwnd, err);
                return -1;
            }
        }


        SetScroll(hwnd, &scroll, &workWind, &wind);


        return 0;
    }

    case WM_VSCROLL :
        VScroll(hwnd, wParam, &scroll, &workWind, &wind);
        return 0;

    case WM_HSCROLL :
        HScroll(hwnd, wParam, &scroll, &wind);
        return 0;

    case WM_KEYDOWN :
        Keydown(hwnd, wParam);
        return 0;

    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        int i;
        int windWidth = wind.cxClient / wind.cxChar + 1;
        long iFirstColumn;


        iPaintBeg = max(0, workWind.iFirstLine + ps.rcPaint.top / wind.cyChar - 1);
        iPaintEnd = min(workWind.numOfLines, workWind.iFirstLine + ps.rcPaint.bottom / wind.cyChar + 1);

        for(i=iPaintBeg; i<iPaintEnd; i++)
        {
            if( (workWind.lengthLine[i+1]-workWind.lengthLine[i]) > scroll.iHscrollPos)
            {
                y = wind.cyChar *( i - workWind.iFirstLine );
                iFirstColumn = workWind.lengthLine[i]+scroll.iHscrollPos;
                TextOut(hdc, 0, y, workWind.str+iFirstColumn, min(windWidth, max(0, workWind.lengthLine[i+1]-iFirstColumn)));
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }


    case WM_COMMAND :
    {
        HMENU hMenu;

        switch(LOWORD(wParam))
        {
        case IDM_OPEN :
        {

            OPENFILENAME ofn;
            char buff[260];

            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hwnd;
            ofn.lpstrFile = buff;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(buff);
            ofn.lpstrFilter = "Text\0*.TXT\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn)==TRUE)
            {

                FreeData(&data);
                FreeWorkModel(&workWind);

                err=processFile(ofn.lpstrFile, &data);

                if(err)
                {
                    DisplayMessageBox(hwnd, err);
                    return -1;
                }

                err=initModelOfData(&data, &workWind);
                if(err)
                {
                    DisplayMessageBox(hwnd, err);
                    return -1;
                }
                GetClientRect(hwnd, &rect);
                InvalidateRect(hwnd, &rect, TRUE);
                UpdateWindow(hwnd);

                SetScroll(hwnd, &scroll, &workWind, &wind);

                hMenu=GetMenu(hwnd);
                CheckMenuItem(hMenu, IDM_NOWRAP, MF_CHECKED);
                CheckMenuItem(hMenu, IDM_WRAP, MF_UNCHECKED);
                EnableMenuItem(hMenu, IDM_NOWRAP, MF_DISABLED);
                EnableMenuItem(hMenu, IDM_WRAP, MF_ENABLED);
            }

            return 0;
        }
        case IDM_EXIT :
            SendMessage(hwnd, WM_CLOSE, 0, 0L);
            return 0;

        case IDM_WRAP :

            workWind.wrap=WRAP;
            workWind.firstCh=workWind.lengthLine[workWind.iFirstLine];
            err=makeWrap(&data, &workWind, &wind);
            if(err)
            {
                DisplayMessageBox(hwnd, err);
                return -1;
            }
            SetScroll(hwnd, &scroll, &workWind, &wind);


            GetClientRect(hwnd, &rect);
            InvalidateRect(hwnd, &rect, TRUE);
            UpdateWindow(hwnd);

            hMenu=GetMenu(hwnd);
            CheckMenuItem(hMenu, IDM_WRAP, MF_CHECKED);
            CheckMenuItem(hMenu, IDM_NOWRAP, MF_UNCHECKED);
            EnableMenuItem(hMenu, IDM_WRAP, MF_DISABLED);
            EnableMenuItem(hMenu, IDM_NOWRAP, MF_ENABLED);
            return 0;

        case IDM_NOWRAP :

            workWind.wrap=NOWRAP;
            workWind.firstCh=workWind.lengthLine[workWind.iFirstLine];

            err=makeModelOfData(&data, &workWind);
            if(err)
            {
                DisplayMessageBox(hwnd, err);
                return -1;
            }

            SetScroll(hwnd, &scroll, &workWind, &wind);

            GetClientRect(hwnd, &rect);
            InvalidateRect(hwnd, &rect, TRUE);
            UpdateWindow(hwnd);
            hMenu=GetMenu(hwnd);
            CheckMenuItem(hMenu, IDM_NOWRAP, MF_CHECKED);
            CheckMenuItem(hMenu, IDM_WRAP, MF_UNCHECKED);
            EnableMenuItem(hMenu, IDM_NOWRAP, MF_DISABLED);
            EnableMenuItem(hMenu, IDM_WRAP, MF_ENABLED);
            return 0;


        case IDM_ABOUT :
            MessageBox(hwnd,
                       "Это читалка, она показывает текст!\nЕсть два режима: с версткой и без верстки.\nВы можете открыть любой файл  расширения .txt",
                       "About", MB_OK | MB_ICONINFORMATION);
            return 0;
            return 0;
        }
        break;
    }
    case WM_DESTROY:
        FreeWorkModel(&workWind);
        FreeData(&data);
        PostQuitMessage(0);

        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}
