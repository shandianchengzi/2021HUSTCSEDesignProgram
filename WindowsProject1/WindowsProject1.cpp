// WindowsProject1.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "WindowsProject1.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
char szTitle[MAX_LOADSTRING];                  // 标题栏文本
char szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Dialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Tips(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Result(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursorFromFile("cursor.ico");
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
		CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, HWND_DESKTOP, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(hWnd, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int iDeltaPerLine, iAccumDelta,isStart=0,isShow=0;
	ULONG ulScrollLines;
	switch (message)
	{
	case WM_CREATE:
	{
		main_path();
		hdc = GetDC(hWnd);
		//创建逻辑字体
		hControl->hFont[0] = CreateFont(
			-15/*高度*/, -7/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
			DEFAULT_QUALITY,  //默认输出质量
			FF_DONTCARE,  //不指定字体族*/
			TEXT("微软雅黑")  //字体名
		);
		hControl->hBrush[0] = CreateSolidBrush(BGCOLOR3); //控件的背景颜色
		hControl->hDialog[0] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)Dialog);
		hControl->hDialog[2] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, (DLGPROC)Result);
		hControl->hDialog[3] = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, (DLGPROC)Setting);
		ShowWindow(hControl->hDialog[0], SW_HIDE);
		ShowWindow(hControl->hDialog[2], SW_HIDE);
		ShowWindow(hControl->hDialog[3], SW_HIDE);
		GetTextMetrics(hdc, &tm);
		GetClientRect(hWnd, &rect);
		cxChar = tm.tmAveCharWidth;/* 系统字体的平均长度 */
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;/* 系统字体的高度+一倍行距 */
		//AddControls(hWnd, hControl);
		hControl->hBitmap[0] = (HBITMAP)LoadImage(NULL, //应用程序实例
			"ap8gp-tp0uf.bmp", //路径
			IMAGE_BITMAP,
			pWidth, //如果不置0,则此参数指定bitmap的宽(位图被拉伸)
			pHeight, //如果不置0,则此参数指定bitmap的高(位图被拉伸)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		hControl->hBitmap[1] = (HBITMAP)LoadImage(NULL, //应用程序实例
			"2021.bmp", //路径
			IMAGE_BITMAP,
			1202, //如果不置0,则此参数指定bitmap的宽(位图被拉伸)
			679, //如果不置0,则此参数指定bitmap的高(位图被拉伸)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		hControl->hBitmap[2]=(HBITMAP)LoadImage(NULL, //应用程序实例
			"hide.bmp", //路径
			IMAGE_BITMAP,
			20, //如果不置0,则此参数指定bitmap的宽(位图被拉伸)
			20, //如果不置0,则此参数指定bitmap的高(位图被拉伸)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		hControl->hBitmap[3] = (HBITMAP)LoadImage(NULL, //应用程序实例
			"show.bmp", //路径
			IMAGE_BITMAP,
			20, //如果不置0,则此参数指定bitmap的宽(位图被拉伸)
			20, //如果不置0,则此参数指定bitmap的高(位图被拉伸)
			LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		hControl->hLab[4] = CreateWindow(TEXT("edit"), TEXT(""),
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
			700, 20, 250, 200,
			hWnd, (HMENU)ID_TEXTI, wincl.hInstance, NULL
		);
		SendMessage(hControl->hLab[4], WM_SETFONT, (WPARAM)hControl->hFont[0], NULL);
		AdjustRC(hControl->hLab[4], 10, 10, 5, 0);
		tips_input(hControl->hLab[4],1);
		ShowWindow(hControl->hLab[4], SW_HIDE);
		hControl->hBtn[1] = CreateWindow(TEXT("button"),//必须为：button
			TEXT("点击我，开始使用~"),//按钮上显示的字符
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			800, 300, 75, 30,			//x,y,宽,高
			hWnd, (HMENU)ID_OK,//绑定按钮ID
			wincl.hInstance, NULL);
		SendMessage(hControl->hBtn[1], BM_SETIMAGE, 0, (LPARAM)hControl->hBitmap[1]);
		hControl->hBtn[2] = CreateWindow(TEXT("button"),//必须为：button
			TEXT("隐藏"),//按钮上显示的字符
			WS_CHILD | WS_VISIBLE | BS_BITMAP,
			800, 20, 20, 20,			//x,y,宽,高
			hWnd, (HMENU)ID_HIDE,//绑定按钮ID
			wincl.hInstance, NULL);
		SendMessage(hControl->hBtn[2], BM_SETIMAGE, 0, (LPARAM)(hControl->hBitmap[2]));
		ShowWindow(hControl->hBtn[2], SW_HIDE);
		ReleaseDC(hWnd, hdc);
		//依次设置控件的字体
		// Save the width of the three columns
		iMaxWidth = 40 * cxChar + 22 * cxCaps;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
		if (ulScrollLines)
			iDeltaPerLine = WHEEL_DELTA / ulScrollLines;
		else
			iDeltaPerLine = 0;
		return 0;
	}

	case WM_CTLCOLORSTATIC:
	{	
		SetTextColor((HDC)wParam, BGCOLOR2);//控件文本的字体颜色
		SetBkColor((HDC)wParam, BGCOLOR);//控件字体的背景颜色，注意跟以上区分
		return (INT_PTR)hControl->hBrush[0];  //这里必须返回画刷句柄
	}
	case WM_MOUSEWHEEL:
		if (iDeltaPerLine == 0)
			break;
		iAccumDelta += (short)HIWORD(wParam);
		while(iAccumDelta>=iDeltaPerLine)
		{
			SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
			iAccumDelta -= iDeltaPerLine;
		}
		while (iAccumDelta <= -iDeltaPerLine)
		{
			SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
			iAccumDelta += iDeltaPerLine;
		}
		return 0;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			 DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_TIPS:
			ShowWindow(CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, Tips), SW_SHOWNA);
			break;
		case ID_OK:
			ShowWindow(hControl->hBtn[1], SW_HIDE);
			ShowWindow(hControl->hBtn[2], SW_SHOW);
			ShowWindow(hControl->hLab[4], SW_SHOW);
			isStart = 1,isShow=1;
			ShowWindow(hControl->hDialog[0], SW_SHOWNA);
			break;
		case ID_HIDE:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				if (isShow) {
					ShowWindow(hControl->hLab[4], SW_HIDE);
					SendMessage(hControl->hBtn[2], BM_SETIMAGE, 0, (LPARAM)(hControl->hBitmap[3]));
					isShow = 0;
				}
				else {
					ShowWindow(hControl->hLab[4], SW_SHOWNA);
					SendMessage(hControl->hBtn[2], BM_SETIMAGE, 0, (LPARAM)(hControl->hBitmap[2]));
					isShow = 1;
				}
			}
			break;
		}
		case ID_TEXTI:
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
			{
				HideCaret(GetDlgItem(hWnd, ID_TEXTI));
				break;
			}
			case EN_CHANGE:
				SetScrollPos(GetDlgItem(hWnd, ID_TEXTI), 0, 0, FALSE);
				break;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_SIZE:
	{
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		// Set vertical scroll bar range and page size
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		si.nMin = 0;
		si.nMax = pHeight / cyChar;
		si.nPage = cyClient / cyChar;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		if (si.nPos > si.nMax - (int)si.nPage + 1) 
		{
			GetScrollInfo(hWnd, SB_VERT, &si);
			iVertPos = si.nPos;
		}
		// Set horizontal scroll bar range and page size
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		si.nMin = 0;
		si.nMax = pWidth / cxChar;
		si.nPage = cxClient / cxChar;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		if (si.nPos > si.nMax - (int)si.nPage + 1)
		{
			GetScrollInfo(hWnd, SB_HORZ, &si);
			iHorzPos = si.nPos;
		}
		return 0;
	}

	case WM_VSCROLL:
	{
		// Get all the vertical scroll bar information
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd, SB_VERT, &si);
		// Save the position for comparison later on
		iVertPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case   SB_LINEDOWN:
			si.nPos += 1;
			break;
		case   SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case   SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case   SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//  by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
		GetScrollInfo(hWnd, SB_VERT, &si);
		// If the position has changed, scroll the window and update it
		if (si.nPos != iVertPos)
		{
			ScrollWindow(hWnd, 0, cyChar * (iVertPos - si.nPos),
				NULL, NULL);
			iVertPos = si.nPos;
			UpdateWindow(hWnd);
		}
		return 0;
		break;
	}
	case WM_HSCROLL:
	{
		// Get all the vertical scroll bar information
		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		// Save the position for comparison later on
		GetScrollInfo(hWnd, SB_HORZ, &si);
		iHorzPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case   SB_LINELEFT:
			si.nPos -= 1;
			break;
		case   SB_LINERIGHT:
			si.nPos += 1;
			break;
		case   SB_PAGELEFT:
			si.nPos -= si.nPage;
			break;
		case   SB_PAGERIGHT:
			si.nPos += si.nPage;
			break;
		case   SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}
		// Set the position and then retrieve it.  Due to adjustments
		//   by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
		GetScrollInfo(hWnd, SB_HORZ, &si);
		// If the position has changed, scroll the window
		if (si.nPos != iHorzPos)
		{
			ScrollWindow(hWnd, cxChar * (iHorzPos - si.nPos), 0,
				NULL, NULL);
			iHorzPos = si.nPos;
			UpdateWindow(hWnd);
		}
		return 0;
	}

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		if (hControl->hBitmap[0])
		{
			GetClientRect(hWnd, &rect);
			HDC hdcMem = CreateCompatibleDC(hdc); //创建内存DC
			SelectObject(hdcMem, hControl->hBitmap[0]); //把位图选进内存DC
			int xx = -iHorzPos;
			int yy = -iVertPos;
			BitBlt(hdc, xx * cxChar, yy * cyChar, pWidth, pHeight, hdcMem, 0, 0, SRCCOPY); //内存DC映射到屏幕DC
			DeleteDC(hdcMem); //删除内存DC
		}
		//SetBitmap();
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		MoveWindow(hControl->hLab[4], cxClient-280, 30, 250, 200, TRUE);
		MoveWindow(hControl->hBtn[2], cxClient-30, 0, 30, 30, TRUE);
		if(!isStart)MoveWindow(hControl->hBtn[1], 100, 100, cxClient-200, cyClient-200, TRUE);
		else MoveWindow(hControl->hBtn[1], cxClient - 130, cyClient-Btn_Height+20, Btn_Width, Btn_Height, TRUE);
		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_DESTROY:
		DeleteAClass(hControl->hBrush, 1);
		DeleteAClass(hControl->hFont, 2); //删除创建的字体
		DeleteAClass(hControl->hBitmap, 3); //释放位图所占Memory
		Free_Graph(&G);
		/*处理WM_DESTROY消息时删除之前我们创建的一切GDI对象*/
		PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK Dialog(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int isName = 1;
	int wmId = LOWORD(wParam);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		AddControls(hWnd, hControl);
		ShowWindow(GetDlgItem(hWnd, IDC_CHANGE1), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHANGE2), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHANGE4), SW_HIDE);
		ShowWindow(GetDlgItem(hWnd, IDC_CHANGE3), SW_HIDE);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
	{
		switch (wmId)
		{
		case ID_EDITB:
		case ID_EDITS:
		case ID_EDITB2:
		case ID_EDITS2:
		{	//printf("1");
			int idc = HIWORD(wParam);
			switch (idc)
			{
			case CBN_SETFOCUS:
			case CBN_SELCHANGE:
			case CBN_EDITCHANGE:
			{
				char tempstr[20];
				int nowSelect = 1;									/*nowSelect：当前正在选择的站点，1表示起点站，2表示终点站*/
				if (wmId == ID_EDITS||wmId==ID_EDITS2)nowSelect = 2;
				SetWindowText(GetDlgItem(hWnd, ID_TEXTI), "");		/*清空输出框内容*/
				if (idc == CBN_SELCHANGE)							/*当选项改变时*/
				{
					int in = SendMessage(GetDlgItem(hWnd, wmId), CB_GETCURSEL, 0, 0);
					if (wmId == ID_EDITB || wmId == ID_EDITS)
						SendMessage(GetDlgItem(hWnd, wmId), CB_GETLBTEXT, in, (LPARAM)tempstr);
					else {
						int line = SendMessage(hControl->hComBox[3+nowSelect*2], CB_GETCURSEL, 0, 0);
						strcpy_s(tempstr, 20, G->lineDetail[line].lineStations[in+1]);
					}
				}
				else if (idc == CBN_EDITCHANGE || idc == CBN_SETFOCUS)	/*当选项框的内容被编辑时*/
				{
					if (wmId == ID_EDITB || wmId == ID_EDITS)
						GetWindowText(GetDlgItem(hWnd, wmId), tempstr, 15);
					else {
						EditControlLimit(hWnd, LOWORD(wParam), (char*)"0123456789", 0);
						GetWindowText(GetDlgItem(hWnd, wmId), tempstr, 3);
						if (strlen(tempstr)) {
							int line = SendMessage(hControl->hComBox[3 + nowSelect * 2], CB_GETCURSEL, 0, 0);
							int sta=atoi(tempstr);
							if(sta<=G->lineDetail[line].lineCount)
								strcpy_s(tempstr, 20, G->lineDetail[line].lineStations[sta]);
						}
						else {
							strcpy_s(tempstr, 3, "");
						}
					}
				}
				tempstr[19] = 0;
				int iLength = strlen(tempstr);
				if (!iLength)
				{
					tips_stations(GetDlgItem(hWnd, ID_TEXTI), NULL, nowSelect);
					tips_stations(GetDlgItem(hWnd, ID_TEXTI), NULL, 5);
				}
				else
				{
					char ss[20][20];
					char* s[20] = { 0 };
					int j = 0, flag;
					for (int k = 0; k < G->numVertexes; k++)
					{
						flag = 0;
						for (int i = 0; i < iLength; i++)
						{
							if (tempstr[i] != G->adjList[k].data[i])
								flag = 1;
						}
						if (!flag)
						{
							strcpy_s(ss[j], 20, G->adjList[k].data);
							s[j] = ss[j];
							j++;
						}
					}
					tips_stations(GetDlgItem(hWnd, ID_TEXTI), NULL, nowSelect);
					int i = 0;
					while (s[i])
					{
						if (wmId == ID_EDITB || wmId == ID_EDITS) {
							tips_station(GetDlgItem(hWnd, ID_TEXTI), s[i]);
							tips_nearby(GetDlgItem(hWnd, ID_TEXTI), s[i]);
						}
						else
						{
							int line = SendMessage(hControl->hComBox[3 + nowSelect * 2], CB_GETCURSEL, 0, 0);
							changeclick(hWnd,nowSelect-1,line,LocateLineSta(G,line,s[0]));
						}
						i++;
					}
					if (!s[0])tips_stations(GetDlgItem(hWnd, ID_TEXTI), s, 6);
				}
				break;
			}
			}
			break;
		}
		case ID_EDITB1:
		case ID_EDITS1:
		{
			switch (HIWORD(wParam))
			{
			case CBN_SETFOCUS:
			case CBN_SELCHANGE:
			{
				int ID = 6,temp;
				char times[3] = { 0 };
				if (wmId == ID_EDITS1)ID = 8;
				int line = SendMessage(hControl->hComBox[ID - 1], CB_GETCURSEL, 0, 0);
				SendMessage(hControl->hComBox[ID], CB_RESETCONTENT, 0, 0);
				for (int k = 1;k <= G->lineDetail[line].lineCount;k++)
				{
					temp = k;
					sprintf_s(times, 3, "%d", temp);
					SendMessage(hControl->hComBox[ID], CB_ADDSTRING, 0, (LPARAM)times);
				}
				SendMessage(hControl->hComBox[ID], CB_SETCURSEL, 0, 0);                      /*默认站点：第一站*/
				SetWindowText(GetDlgItem(hWnd, ID_TEXTI), "");
				tips_line(GetDlgItem(hWnd, ID_TEXTI), line,ID/2-2);
				break;
			}
			}
			break;
		}
		
		case ID_EDITH:
		case ID_EDITM:
			switch (HIWORD(wParam))
			{
			case CBN_SELCHANGE:
			case CBN_EDITCHANGE:
				if (HIWORD(wParam) == CBN_EDITCHANGE)
				{
					EditControlLimit(hWnd, LOWORD(wParam), (char*)"0123456789", 0);
					time.hour = EditControlLimit(hWnd, ID_EDITH, (char*)"小时", 2);
				}
				else
				{
					time.hour = SendMessage(GetDlgItem(hWnd, ID_EDITH), CB_GETCURSEL, 0, 0)+6;
				}
				if (time.hour != -1)
				{
					if (HIWORD(wParam) == CBN_EDITCHANGE)
					{
						GetWindowText(hControl->hComBox[2], mTime, 2);
						time.minute = (float)EditControlLimit(hWnd, ID_EDITM, (char*)"分钟", 3);
					}
					else
					{
						time.minute = (float)SendMessage(GetDlgItem(hWnd, ID_EDITM), CB_GETCURSEL, 0, 0);
					}
					if (time.minute != -1)
					{
						SetWindowText(GetDlgItem(hWnd, ID_TEXTI),"" );
						tips_table(GetDlgItem(hWnd,ID_TEXTI),time);
					}
					return 0;
				}
				break;
			}
			break;
		case IDC_COMBO2:
			switch (HIWORD(wParam))
			{
			case CBN_EDITCHANGE:
				EditControlLimit(hWnd, LOWORD(wParam), (char*)"0123456789", 0);
				break;
			}
			break;
		case ID_TEXTI:
		{
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
			{
				HideCaret(GetDlgItem(hWnd, ID_TEXTI));
				break;
			}
			case EN_CHANGE:
				SetScrollPos(GetDlgItem(hWnd, ID_TEXTI), 0, 0, FALSE);
				break;
			}
			break;
		}
			
		case ID_OK:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
			{
				if (Checkinput(hWnd) == 0)
				{
					ShowWindow(hControl->hDialog[2], SW_SHOWNA);
					ShowResult(GetDlgItem(hControl->hDialog[2], ID_TEXTI1), 0);
					ShowResult(GetDlgItem(hControl->hDialog[2], ID_TEXTI2), 1);
					ShowResult(GetDlgItem(hControl->hDialog[2], ID_TEXTI3), 2);
					ShowResult(GetDlgItem(hControl->hDialog[2], ID_TEXTI4), 3);
				}
				break;
			}
			}
			break;
		}
		case ID_CHANGE:									/*切换输入模式*/
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				int i1 = SW_SHOWNA, i2 = SW_HIDE;
				if (howInput)
				{
					i1 = SW_HIDE, i2 = SW_SHOWNA;
				}
				ShowWindow(hControl->hComBox[5], i1);
				ShowWindow(hControl->hComBox[6], i1);
				ShowWindow(hControl->hComBox[7], i1);
				ShowWindow(hControl->hComBox[8], i1);
				ShowWindow(GetDlgItem(hWnd, IDC_UP1), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_UP2), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_DOWN1), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_DOWN2), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_CHANGE1), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_CHANGE2), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_CHANGE3), i1);
				ShowWindow(GetDlgItem(hWnd, IDC_CHANGE4), i1);
				ShowWindow(hControl->hComBox[0], i2);
				ShowWindow(hControl->hComBox[1], i2);
				howInput = !howInput;

				SetWindowText(GetDlgItem(hWnd, ID_TEXTI), "");
				tips_input(GetDlgItem(hWnd, ID_TEXTI), 2);
				if (howInput)tips_line(GetDlgItem(hWnd, ID_TEXTI),0, 0);
			}
			break;
		}
		case IDC_BUTTON1:								/*显示调整拥挤度对话框*/
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				ShowWindow(hControl->hDialog[3], SW_SHOWNA);
			}
			break;
		}
		case IDC_UP1:
		case IDC_UP2:
		case IDC_DOWN1:
		case IDC_DOWN2:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				int isUp = 0;
				if (wmId == IDC_UP1 || wmId == IDC_UP2)isUp = 1;
				int i = wmId - (isUp?IDC_UP1:IDC_DOWN1) + 1;
				int line = SendMessage(hControl->hComBox[i * 2 + 3], CB_GETCURSEL, 0, 0);
				char tempstr[3];
				GetWindowText(hControl->hComBox[i * 2 + 4], tempstr, 3);
				if (strlen(tempstr))
				{
					int sta = atoi(tempstr);
					if (isUp && sta == 1)
					{
						MessageBox(hWnd, "没有上一站了~", "小贴士", MB_ICONWARNING | MB_OK);
					}
					else if (!isUp && sta == G->lineDetail[line].lineCount)
					{
						MessageBox(hWnd, "没有下一站了~", "小贴士", MB_ICONWARNING | MB_OK);
					}
					else if (sta >= 1 && sta <= G->lineDetail[line].lineCount)
					{
						if (isUp)sta -= 1;
						else sta += 1;
						sprintf_s(tempstr, "%d", sta);
						SendMessage(hControl->hComBox[i * 2 + 4], WM_SETTEXT, 0, (LPARAM)tempstr);
						SetWindowText(GetDlgItem(hWnd, ID_TEXTI), "");
						tips_stations(GetDlgItem(hWnd, ID_TEXTI), NULL, i);
						changeclick(hWnd, i - 1, line, sta);
					}
				}
			}
			break;
		}
		case IDC_CHANGE1:
		case IDC_CHANGE2:
		case IDC_CHANGE3:
		case IDC_CHANGE4:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				int i = wmId - IDC_CHANGE1,who;
				char szBuffer[20];
				if (i < 2)who = 0;
				else {
					who = 1;
					i -= 2;
				}
				int line = transferPlan[who][i * 2],lineIndex=0;
				sprintf_s(szBuffer, "%d", line);
				SendMessage(hControl->hComBox[who*2+5], CB_SELECTSTRING, -1, (LPARAM)szBuffer);       /*设置线路*/
				/*重置一下站点号*/
				for (;lineIndex < linenum;lineIndex++)
				{
					if (line == G->lineDetail[lineIndex].lineIndex)
						break;
				}
				SendMessage(hControl->hComBox[who*2+6], CB_RESETCONTENT, 0, 0);
				for (int k = 1;k <= G->lineDetail[lineIndex].lineCount;k++)
				{
					int temp = k;
					char stas[3];
					sprintf_s(stas, 3, "%d", temp);
					SendMessage(hControl->hComBox[who*2+6], CB_ADDSTRING, 0, (LPARAM)stas);
				}
				int sta = transferPlan[who][i * 2 + 1];
				sprintf_s(szBuffer, "%d",sta );
				SendMessage(hControl->hComBox[who*2+6], CB_SELECTSTRING, -1, (LPARAM)szBuffer);      /*换乘站点*/
				SetWindowText(GetDlgItem(hWnd, ID_TEXTI), "");
				tips_stations(GetDlgItem(hWnd, ID_TEXTI), NULL, who + 1);
				changeclick(hWnd, who, lineIndex, sta);
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			// 如果执行了关闭
			// 退出对话框并返回数据
			ShowWindow(hControl->hBtn[1], SW_SHOWNA);
			EndDialog(hWnd, LOWORD(wParam));
			for (int j = 0; P[j]; j++)
				Free_Path(&P[j]);
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Tips(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		tips_input(GetDlgItem(hDlg, ID_TEXTT), 2);
		tips_table(GetDlgItem(hDlg, ID_TEXTT),time);
		tips_input(GetDlgItem(hDlg, ID_TEXTT), 1);
		SendMessage(GetDlgItem(hDlg, ID_TEXTT), WM_SETFONT, (WPARAM)hControl->hFont[0], NULL);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TEXTT:
		{
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
			{
				HideCaret(GetDlgItem(hDlg, ID_TEXTT));
				break;
			}
			case EN_CHANGE:
				SetScrollPos(GetDlgItem(hDlg, ID_TEXTT), 0, 0, FALSE);
				break;
			}
			break;
		}
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			// 如果执行了关闭
			// 退出对话框并返回数据
			EndDialog(hDlg, LOWORD(wParam));
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Result(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static HWND hwndTab;
	static int idTab = 0;
	int wmId = LOWORD(wParam);
	switch (message)
	{
	case WM_INITDIALOG:
		/*初始化用于切换结果展示的Tab控件*/
		hwndTab = GetDlgItem(hDlg, IDC_TAB2);
		ti.mask = TCIF_TEXT | TCIF_IMAGE;
		ti.iImage = -1;
		ti.pszText = g_achTemp;
		sprintf_s(g_achTemp, "综合评估");
		TabCtrl_InsertItem(hwndTab, 0, &ti);
		sprintf_s(g_achTemp, "时间短");
		TabCtrl_InsertItem(hwndTab, 1, &ti);
		sprintf_s(g_achTemp, "票价低");
		TabCtrl_InsertItem(hwndTab, 2, &ti);
		sprintf_s(g_achTemp, "拥挤少");
		TabCtrl_InsertItem(hwndTab, 3, &ti);
		ShowWindow(hwndTab, SW_NORMAL);
		/*显示第一个编辑框，其他的隐藏*/
		ShowWindow(GetDlgItem(hDlg, ID_TEXTI1), SW_SHOWNA);
		ShowWindow(GetDlgItem(hDlg, ID_TEXTI2), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, ID_TEXTI3), SW_HIDE);
		ShowWindow(GetDlgItem(hDlg, ID_TEXTI4), SW_HIDE);
		/*设置所有编辑框的字体*/
		SendMessage(GetDlgItem(hDlg, ID_TEXTI1), WM_SETFONT, (WPARAM)hControl->hFont[0], NULL);
		SendMessage(GetDlgItem(hDlg, ID_TEXTI2), WM_SETFONT, (WPARAM)hControl->hFont[0], NULL);
		SendMessage(GetDlgItem(hDlg, ID_TEXTI3), WM_SETFONT, (WPARAM)hControl->hFont[0], NULL);
		SendMessage(GetDlgItem(hDlg, ID_TEXTI4), WM_SETFONT, (WPARAM)hControl->hFont[0], NULL);
		/*调整所有编辑框的内边距*/
		AdjustRC(GetDlgItem(hDlg, ID_TEXTI1), 10, 10, 5, 0);
		AdjustRC(GetDlgItem(hDlg, ID_TEXTI2), 10, 10, 5, 0);
		AdjustRC(GetDlgItem(hDlg, ID_TEXTI3), 10, 10, 5, 0);
		AdjustRC(GetDlgItem(hDlg, ID_TEXTI4), 10, 10, 5, 0);
		return (INT_PTR)TRUE;
	case WM_NOTIFY:
	{
		if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
		{	/*当Tab控件被点击时，隐藏上一个编辑框，并显示当前切换的编辑框。注：编辑框的ID是连续的。*/
			ShowWindow(GetDlgItem(hDlg, ID_TEXTI1+idTab), SW_HIDE);
			idTab = TabCtrl_GetCurSel(hwndTab);
			ShowWindow(GetDlgItem(hDlg, ID_TEXTI1+idTab), SW_SHOWNA);
		}
		break;
	}
	case WM_COMMAND:
		switch (wmId)
		{
		case ID_TEXTI1:
		case ID_TEXTI2:
		case ID_TEXTI3:
		case ID_TEXTI4:
		{
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
			{
				HideCaret(GetDlgItem(hDlg, ID_TEXTT));
				break;
			}
			case EN_CHANGE:
				SetScrollPos(GetDlgItem(hDlg, ID_TEXTT), 0, 0, FALSE);
				break;
			}
			break;
		}
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			// 如果执行了关闭
			// 退出对话框并返回数据
			EndDialog(hDlg, LOWORD(wParam));
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK Setting(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static HWND hwndTab;
	static int idTab=0;
	switch (message)
	{
	case WM_INITDIALOG:
	{
		hwndTab = GetDlgItem(hDlg, IDC_TAB1);
		ti.mask = TCIF_TEXT | TCIF_IMAGE;
		ti.iImage = -1;
		ti.pszText = g_achTemp;
		for (int i = 0;i < linenum;i++)
		{
			sprintf_s(g_achTemp, "%d号线", G->lineDetail[i].lineIndex);
			TabCtrl_InsertItem(hwndTab, i, &ti);
		}
		ShowWindow(hwndTab, SW_NORMAL);
		HWND hWndSlider;
		for (int i = 0;hWndSlider = GetDlgItem(hDlg, IDC_SLIDER1 + i);i++)
		{
			SendMessage(hWndSlider, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(0, 99));//设置范围为0到99
			SendMessage(hWndSlider, TBM_SETPOS, (WPARAM)1, (LPARAM)(int)G->lineDetail[idTab].crowded[i]);//设置滑块初始位置
			char szBuffer[5];
			sprintf_s(szBuffer, "%02d％", G->lineDetail[idTab].crowded[i]);
			SendMessage(GetDlgItem(hDlg, IDC_EDIT1 + i), WM_SETTEXT, 0, (LPARAM)szBuffer);
		}
		return (INT_PTR)TRUE;
	}

	//处理tab的改变消息
	case WM_NOTIFY:
	{
		if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
		{
			idTab = TabCtrl_GetCurSel(hwndTab);
			HWND hWndSlider;
			for (int i = 0;hWndSlider = GetDlgItem(hDlg, IDC_SLIDER1 + i);i++)
			{
				SendMessage(hWndSlider, TBM_SETPOS, (WPARAM)1, (LPARAM)G->lineDetail[idTab].crowded[i]);
				char szBuffer[5];
				sprintf_s(szBuffer, "%02d％", G->lineDetail[idTab].crowded[i]);
				SendMessage(GetDlgItem(hDlg, IDC_EDIT1 + i), WM_SETTEXT, 0, (LPARAM)szBuffer);
			}
		}
		break;
	}

	//处理slider的改变消息
	case WM_HSCROLL:
	{
		HWND hWndSlider = (HWND)lParam;
		int i = GetDlgCtrlID(hWndSlider) - IDC_SLIDER1;				/*i：当前被更改的Slider的编号*/
		int nScrollCode = (int)LOWORD(wParam);						/* scroll bar value  */
		switch (nScrollCode)
		{
		case SB_THUMBTRACK:   //用户拖动滚动框。此消息被重复发送，直到用户释放鼠标按钮。
		{
			int value = SendMessage(hWndSlider, TBM_GETPOS, 0, 0);	/*滑块位置*/
			G->lineDetail[idTab].crowded[i] = value;				/*更新线路中记载的拥挤百分数*/
			/*将更新内容显示在编辑框中*/
			char szBuffer[5];
			sprintf_s(szBuffer, "%02d％", value);
			SendMessage(GetDlgItem(hDlg, IDC_EDIT1 + i), WM_SETTEXT, 0, (LPARAM)szBuffer);
			break;
		}
		}
		break;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_TEXTT:
		{
			switch (HIWORD(wParam))
			{
			case EN_SETFOCUS:
			{
				HideCaret(GetDlgItem(hDlg, ID_TEXTT));
				break;
			}
			case EN_CHANGE:
				SetScrollPos(GetDlgItem(hDlg, ID_TEXTT), 0, 0, FALSE);
				break;
			}
			break;
		}
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			// 如果执行了关闭
			// 退出对话框并返回数据
			EndDialog(hDlg, LOWORD(wParam));
		}
		return 0;
	}
	return (INT_PTR)FALSE;
}