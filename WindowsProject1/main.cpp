#include"head.h"
#include"main.h"

#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#define NUMLINES countt*9


int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    main_path();
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;        /* 窗口类的名称 */
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);     /* 图标 */
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);   /* 小图标 */
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);       /* 鼠标指针 */
    wincl.lpszMenuName = NULL;                          /* No menu */
    wincl.cbClsExtra = 0;                               /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                               /* structure or the window instance */
    wincl.hbrBackground = CreateSolidBrush (BGCOLOR);          /* 窗口客户区的背景填充色 */

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("地铁转乘系统"),  /* Title Text */
               WS_OVERLAPPEDWINDOW|WS_VSCROLL, /* 窗口样式：层叠overlapped窗口+垂直滚动条 */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               WIDTH,               /* 宽 */
               HEIGHT,              /* 高 */
               HWND_DESKTOP,        /* 父窗口句柄 */
               NULL,                /* 窗口菜单句柄 */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);         /* 重新绘制窗口 */
    /* 消息循环. It will run until GetMessage() returns 0 */
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
    static lhControl hc;
    static lhControl *hControl=&hc;
    int x,iVertPos,iHorzPos,iPaintBeg,iPaintEnd;

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
    {
        hdc = GetDC (hwnd) ;
        GetTextMetrics (hdc, &tm) ;
        GetClientRect (hwnd, &rect) ;
        cxChar = tm.tmAveCharWidth ;/* 系统字体的平均长度 */
        cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
        cyChar = tm.tmHeight + tm.tmExternalLeading ;/* 系统字体的高度+一倍行距 */
        AddControls(hwnd,hControl);
        ReleaseDC (hwnd, hdc) ;
        //依次设置控件的字体
        // Save the width of the three columns
        iMaxWidth = 40 * cxChar + 22 * cxCaps ;
        return 0 ;
    }
    case WM_TIMER:
    {
        switch(wParam)
        {
        case TIMER_TOAST:
        {
            SendMessage(hwnd,WM_CLOSE,0,0);
        }
        }
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        //SetTextColor((HDC)wParam, RGB(255,255,255));//控件文本的字体颜色
        SetBkColor((HDC)wParam, BGCOLOR);//控件字体的背景颜色，注意跟以上区分
        return (INT_PTR)hControl->hBrush[0];  //这里必须返回画刷句柄
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_EDITB:
        case ID_EDITS:
            //printf("1");
            switch(HIWORD(wParam))
            {
            case CBN_EDITCHANGE:
            case CBN_SELENDOK:
                {
                    char tempstr[20];
                    SetWindowText(GetDlgItem(hwnd,ID_TEXTI),"");
                    int nowID=1;
                    if(LOWORD(wParam)==ID_EDITS)nowID=2;
                    //SendMessage(GetDlgItem(hwnd,ID_TEXTI),CB_GETDROPPEDSTATE,)
                    GetWindowText(hControl->hComBox[nowID-1],tempstr, 20);
                    int iLength=strlen(tempstr);
                    if(!iLength)
                    {
                        tips_stations(GetDlgItem(hwnd,ID_TEXTI),NULL,nowID);
                        tips_stations(GetDlgItem(hwnd,ID_TEXTI),NULL,5);
                    }
                    else
                    {
                        char ss[20][20];
                        char *s[20]={0};
                        int j=0,flag;
                        for(int k=0;k<G->numVertexes;k++)
                        {
                            flag=0;
                            for(int i=0;i<iLength;i++)
                            {
                                if(tempstr[i]!=G->adjList[k].data[i])
                                    flag=1;
                            }
                            if(!flag)
                            {
                                strcpy(ss[j],G->adjList[k].data);
                                s[j]=ss[j];
                                j++;
                            }
                        }

                        tips_stations(GetDlgItem(hwnd,ID_TEXTI),s,nowID);
                        if(!s[0])tips_stations(GetDlgItem(hwnd,ID_TEXTI),s,6);
                    }
                }
                break;
            }
            break;
        case ID_EDITH:
        case ID_EDITM:
        case ID_EDITT:
            EditControlLimit(hwnd,LOWORD(wParam),"0123456789",0);
            break;
        case ID_TEXTI:
            switch(HIWORD(wParam))
            {
            case EN_SETFOCUS:
            {
                HideCaret(GetDlgItem(hwnd,ID_TEXTI));
                break;
            }
            case EN_CHANGE:
                SetScrollPos(GetDlgItem(hwnd,ID_TEXTI),0,0,FALSE);
                break;
            }
            break;
        case ID_OK:
        {
            switch(HIWORD(wParam))
            {
            case BN_CLICKED:
            {
                int ivex,jvex;
                GetWindowText(hControl->hComBox[0], bStation, 30);
                ivex=EditControlLimit(hwnd,ID_EDITB,"起点站",1);
                if(ivex!=-1)
                {
                    GetWindowText(hControl->hComBox[1], eStation, 30);
                    jvex=EditControlLimit(hwnd,ID_EDITS,"终点站",1);
                    if(jvex!=-1)
                    {
                        if(ivex==jvex)
                        {
                            MessageBox(hwnd,"起点站和终点站不可以相同~","小贴士",MB_ICONWARNING|MB_OK);
                            break;
                        }
                        GetWindowText(hControl->hComBox[1], hTime, 2);
                        time.hour=EditControlLimit(hwnd,ID_EDITH,"小时",2);
                        if(time.hour!=-1)
                        {
                            GetWindowText(hControl->hComBox[2], mTime, 2);
                            time.minute=(float)EditControlLimit(hwnd,ID_EDITM,"分钟",3);
                            if(time.minute!=-1)
                            {
                                printf("%s->%s，发车时间：%d:%f\n",bStation,eStation,time.hour,time.minute);
                                countt=get_All(G,P,ivex,jvex,time,2);
                                tips_output(GetDlgItem(hwnd,ID_TEXTI));
                            }
                        }
                        //Show_Path(G,P);
                    }
                }
                break;
            }
            }
        }

        }
    }
    //SendMessage(wndHandle, WM_CLOSE, 0, 0);
    case WM_SIZE:
    {
        cxClient = LOWORD (lParam) ;
        cyClient = HIWORD (lParam) ;
        // Set vertical scroll bar range and page size
        si.cbSize     = sizeof (si) ;
        si.fMask      = SIF_RANGE | SIF_PAGE ;
        si.nMin       = 0 ;
        si.nMax       = NUMLINES - 1 ;
        si.nPage      = cyClient / cyChar ;
        SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
        // Set horizontal scroll bar range and page size
        si.cbSize     = sizeof (si) ;
        si.fMask      = SIF_RANGE | SIF_PAGE ;
        si.nMin       = 0 ;
        si.nMax       = 2 + iMaxWidth / cxChar ;
        si.nPage      = cxClient / cxChar ;
        SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
        return 0 ;
    }

    case WM_VSCROLL:
    {
        // Get all the vertical scroll bar information
        si.cbSize     = sizeof (si) ;
        si.fMask      = SIF_ALL ;
        GetScrollInfo (hwnd, SB_VERT, &si) ;
        // Save the position for comparison later on
        iVertPos = si.nPos ;
        switch (LOWORD (wParam))
        {
        case   SB_TOP:
            si.nPos = si.nMin ;
            break ;
        case   SB_BOTTOM:
            si.nPos = si.nMax ;
            break ;
        case SB_LINEUP:
            si.nPos -= 1 ;
            break ;
        case   SB_LINEDOWN:
            si.nPos += 1 ;
            break ;
        case   SB_PAGEUP:
            si.nPos -= si.nPage ;
            break ;
        case   SB_PAGEDOWN:
            si.nPos += si.nPage ;
            break ;
        case   SB_THUMBTRACK:
            si.nPos = si.nTrackPos ;
            break ;
        default:
            break ;
        }
        // Set the position and then retrieve it.  Due to adjustments
        //  by Windows it may not be the same as the value set.
        si.fMask = SIF_POS ;
        SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
        GetScrollInfo (hwnd, SB_VERT, &si) ;
        // If the position has changed, scroll the window and update it
        if (si.nPos != iVertPos)
        {
            ScrollWindow (hwnd, 0, cyChar * (iVertPos - si.nPos),
                          NULL, NULL) ;
            UpdateWindow (hwnd) ;
        }
        return 0 ;
    }
    case WM_HSCROLL:
    {
        // Get all the vertical scroll bar information
        si.cbSize = sizeof (si) ;
        si.fMask  = SIF_ALL ;
        // Save the position for comparison later on
        GetScrollInfo (hwnd, SB_HORZ, &si) ;
        iHorzPos = si.nPos ;
        switch (LOWORD (wParam))
        {
        case   SB_LINELEFT:
            si.nPos -= 1 ;
            break ;
        case   SB_LINERIGHT:
            si.nPos += 1 ;
            break ;
        case   SB_PAGELEFT:
            si.nPos -= si.nPage ;
            break ;
        case   SB_PAGERIGHT:
            si.nPos += si.nPage ;
            break ;
        case   SB_THUMBPOSITION:
            si.nPos = si.nTrackPos ;
            break ;
        default :
            break ;
        }
        // Set the position and then retrieve it.  Due to adjustments
        //   by Windows it may not be the same as the value set.
        si.fMask = SIF_POS ;
        SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
        GetScrollInfo (hwnd, SB_HORZ, &si) ;
        // If the position has changed, scroll the window
        if (si.nPos != iHorzPos)
        {
            ScrollWindow (hwnd, cxChar * (iHorzPos - si.nPos), 0,
                          NULL, NULL) ;
        }
        return 0 ;
    }

    case WM_PAINT:
    {
        int cxChar,cxCaps,cxClient,cyChar,cyClient,iMaxWidth;
        hdc=BeginPaint(hwnd,&ps);
        if(hControl->hBitmap[0])
        {
            GetClientRect (hwnd, &rect) ;
            HDC hdcMem=CreateCompatibleDC(hdc); //创建内存DC
            SelectObject(hdcMem,hControl->hBitmap[0]); //把位图选进内存DC
            BitBlt(hdc,0,0,1000,600,hdcMem,0,0,SRCCOPY); //内存DC映射到屏幕DC
            DeleteDC(hdcMem) ; //删除内存DC
        }


        // Get vertical scroll bar position
        si.cbSize=sizeof(si);
        si.fMask=SIF_POS;
        GetScrollInfo(hwnd,SB_VERT,&si);
        iVertPos=si.nPos;
        //Get horizontal scroll bar position
        GetScrollInfo(hwnd,SB_HORZ,&si);
        iHorzPos=si.nPos;
        // Find painting limits
        //iPaintBeg=max(0,iVertPos+ps.rcPaint.top/cyChar);
        //iPaintEnd=min(countt*4-1,iVertPos+ps.rcPaint.bottom/cyChar);
        TCHAR szBuffer [40];
        // 获取格式化的字符串长度，并显示int cxChar,cxCaps,cxClient,cyChar,cyClient,iMaxWidth;

        //Free_Graph(&G);
        EndPaint(hwnd,&ps);
        return 0;
    }

    case WM_DESTROY:
        DeleteAClass(hControl->hBrush,1);
        DeleteAClass(hControl->hFont,2); //删除创建的字体
        DeleteAClass(hControl->hBitmap,3); //释放位图所占Memory
        Free_Graph(&G);
        for(int j=0; j<countt; j++)
        {
            Free_Path(&P[j]);
        }
        free(hControl);
        /*处理WM_DESTROY消息时删除之前我们创建的一切GDI对象*/

        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
