#include"resource.h"
#include"path.h"
/* wParam的低字节位表示ID号，高字节位表示控件通知,
 * lParam中保存了控件的句柄 */

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("subway");
int WIDTH=1000;
int HEIGHT=650;
HDC hdc;
PAINTSTRUCT ps;
RECT rect;
TEXTMETRIC tm;
SCROLLINFO si;
WNDCLASSEX wincl;        /* Data structure for the windowclass */
//声明变量
GraphAdjList* G;
PathInfo* P[999];
char bStation[50];
char eStation[50];
char hTime[20];
char mTime[20];
int countt;
int cxChar,cxCaps,cxClient,cyChar,cyClient,iMaxWidth;
Time_2 time;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
int main_path();

typedef struct lhControl
{
    int ControlNum;
    HFONT hFont[10];  //逻辑字体
    HBRUSH hBrush[10];
    HWND hLab[40];  //静态文本框
    HWND hComBox[10];
    HWND hBtn[10];			//发送信息
    HBITMAP hBitmap[10];
} lhControl;
void append_edit(HWND hwnd,char*s)
{
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)s);
}
void strcat1(char*s1,char*s2)
{
    strcat_s(_strrev(s1),100,_strrev(s2));
    _strrev(s2);
    _strrev(s1);
}
void tips_station(HWND hwnd,char *str)
{
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    char szBuffer[40],szBuffer1[40],szBuffer2[40],sss[10]="%*d.%*d,";
    char o1[100]="%[^.]d",o2[100]="%*d.%[^;,]d";
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)str);
    hash_struct *temp=find_station(G->stations,str);
    while(sscanf_s(temp->data.whose_where,o1,szBuffer1))
    {
        sscanf_s(temp->data.whose_where,o2,szBuffer2);
        sprintf_s(szBuffer,"，%d号线第%s站",G->lineDetail[atoi(szBuffer1)].lineIndex,szBuffer2);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        strcat1(o1,sss);
        strcat1(o2,sss);
    }
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"；\r\n　");
    SendMessage(hwnd,EM_SETSEL,0,0);
}
void tips_stations(HWND hwnd,char*s[20],int now)
{
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    switch(now)
    {
    case 0:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"出发时间：\r\n　");
        break;
    case 1:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"\r\n - 起点站信息：\r\n　");
        break;
    case 2:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"\r\n - 终点站信息：\r\n　");
        break;
    case 3:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"（2）所有站点信息：\r\n　");
        break;
    case 4:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"推荐线路：\r\n　");
        break;
    case 5:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"不能为空喔~\r\n　");
        break;
    case 6:
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"没有这个站喔~\r\n　");
        break;
    }
    SendMessage(hwnd,EM_SETSEL,0,0);
    if(!s)return;
    int i=0;
    while(s[i])
    {
        tips_station(hwnd,s[i]);
        i++;
    }
}
void tips_input(HWND hwnd)
{
    SendMessage(hwnd,WM_SETTEXT,0,0);
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"（1）输入提示：\r\n　");
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"1. 起点站和终点站不能为空，且不能相同；\r\n　");
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"2. 时间只能输入整数喔~！\r\n　");
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)"3. 不记得名字了？换个输入方式试试吧~！\r\n　");
    char *stations[G->numVertexes+1];
    for(int k=0;k<G->numVertexes;k++)
    {
        stations[k]=G->adjList[k].data;
    }
    stations[G->numVertexes]=0;
    tips_stations(hwnd,stations,3);
}
Status Show_Path(HWND hwnd,GraphAdjList *G,PathInfo** P)
{
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    TCHAR szBuffer[40];
    int i;
    for(i=0; i<countt; i++)
    {
        sprintf(szBuffer,TEXT("路线%d："),
                        i+1);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        sprintf(szBuffer,TEXT("换乘%d次，"),
                        P[i]->transfer);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        sprintf(szBuffer,TEXT("耗时总计%.2f分钟，"),
                        P[i]->time);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        sprintf(szBuffer,TEXT("经过%d个站点，"),
                        P[i]->stationNum);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        sprintf(szBuffer,TEXT("总长%.2f千米，"),
                        P[i]->len);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        sprintf(szBuffer,TEXT("需花费%d元。\r\n　"),
                        P[i]->money);
        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
        int j=0,lastIndex=-1;
        while(P[i]->path[j]!=-1)
        {
            if(P[i]->path[j+1]!=-1)
            {
                int nowIndex=P[i]->edge[j]->lineIndex;
                if(lastIndex!=-1)
                {
                    if(lastIndex!=nowIndex)
                    {
                        sprintf(szBuffer,TEXT("%s【从%d号线换乘到%d号线】 → "),G->adjList[P[i]->path[j]].data,G->lineDetail[lastIndex].lineIndex,G->lineDetail[nowIndex].lineIndex);
                        SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
                        lastIndex=nowIndex;
                        j++;
                        continue;
                    }
                }
                lastIndex=nowIndex;
                sprintf_s(szBuffer,40,TEXT("%s → "),G->adjList[P[i]->path[j]].data);
                SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
            }
            else
            {
                sprintf_s(szBuffer,40,TEXT("%s。\r\n　"),G->adjList[P[i]->path[j]].data);
                SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
            }
            j++;
        }
        Free_Path(&P[i]);
    }
    SendMessage(hwnd,EM_SETSEL,0,0);
}
void tips_output(HWND hwnd)
{
    SetWindowTextW(hwnd,(LPCWSTR)"");
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    tips_stations(hwnd,NULL,0);
    char szBuffer[40];
    SendMessage(hwnd,EM_SETSEL,-2,-1);
    sprintf(szBuffer,"%02d:%02d\r\n期望路线：\r\n　%s->%s\r\n",time.hour,(int)(time.minute),bStation,eStation);
    SendMessage(hwnd,EM_REPLACESEL,1,(LPARAM)szBuffer);
    tips_stations(hwnd,NULL,4);
    Show_Path(hwnd,G,P);
    SendMessage(hwnd,EM_SCROLLCARET,1,2);
}
void FontAClass(HWND*hwnd,HFONT hFont)
{
    int i=0;
    while(hwnd[i])
    {
        SendMessage(hwnd[i], WM_SETFONT, (WPARAM)hFont, NULL);
        i++;
    }
}
void AddControls(HWND hwnd,lhControl *hControl)
{
    hControl->hBrush[0] = CreateSolidBrush(RGB(0X41, 0X96, 0X4F)); //翠绿色，控件的背景颜色
    //创建逻辑字体
    hControl->hFont[0] = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
                                    FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/,DEFAULT_CHARSET,
                                    OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                                    FF_DONTCARE, TEXT("微软雅黑")
                                   );
    hControl->hBitmap[0]=(HBITMAP)LoadImage(NULL, //应用程序实例
                                            "img/Wuhan_Metro_Diagram1.bmp", //路径
                                            IMAGE_BITMAP,
                                            750, //如果不置0,则此参数指定bitmap的宽(位图被拉伸)
                                            600, //如果不置0,则此参数指定bitmap的高(位图被拉伸)
                                            LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);
    hControl->hBtn[0]= CreateWindow(TEXT("button"),//必须为：button
                                    TEXT("OK"),//按钮上显示的字符
                                    WS_CHILD | WS_VISIBLE| BS_PUSHBUTTON,
                                    800, 300, 75, 30,			//x,y,宽,高
                                    hwnd, (HMENU)ID_OK,//绑定按钮ID
                                    wincl.hInstance, NULL);
    //创建静态文本框控件
    hControl->hLab[0]= CreateWindow(TEXT("static"), TEXT("起点站："),
                                    WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*内容垂直居中*/ | SS_CENTER /*水平居中*/,
                                    800, 10, 70, 26,
                                    hwnd, 0, wincl.hInstance, NULL
                                   );

    hControl->hLab[1]= CreateWindow(TEXT("static"), TEXT("终点站："),
                                    WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*内容垂直居中*/ | SS_CENTER /*水平居中*/,
                                    800, 10, 70, 26,
                                    hwnd, 0, wincl.hInstance, NULL
                                   );
    hControl->hLab[2]= CreateWindow(TEXT("edit"), TEXT(""),
                                    WS_CHILD | WS_VISIBLE | WS_VSCROLL |ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY,
                                    700, 350, 250, 200,
                                    hwnd, (HMENU)ID_TEXTI, wincl.hInstance, NULL
                                   );
    SetScrollInfo(hControl->hLab[2],SB_VERT,&si,TRUE);
    tips_input(hControl->hLab[2]);
    hControl->hComBox[0]= CreateWindow(TEXT("combobox"),
                                       NULL,
                                       WS_CHILD | WS_VISIBLE|WS_VSCROLL | CBS_SIMPLE | CBS_HASSTRINGS|CBS_AUTOHSCROLL /*水平滚动*/,
                                       750, 100, 90, cyChar*6,			//x,y,宽,高
                                       hwnd, (HMENU)ID_EDITB,//绑定按钮ID
                                       wincl.hInstance, NULL);
    hControl->hComBox[1] = CreateWindow(TEXT("combobox"),//必须为：button
                                        TEXT("三店站"),//按钮上显示的字符
                                        WS_CHILD | WS_VISIBLE|WS_VSCROLL | CBS_SIMPLE | CBS_HASSTRINGS|CBS_AUTOHSCROLL /*水平滚动*/,
                                        860, 100, 90, cyChar*6,			//x,y,宽,高
                                        hwnd, (HMENU)ID_EDITS,//绑定按钮ID
                                        wincl.hInstance, NULL);
    hControl->hComBox[2] = CreateWindow(TEXT("combobox"),//必须为：button
                                        TEXT("07"),//按钮上显示的字符
                                        WS_CHILD | WS_VISIBLE|WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS,
                                        840, 200, 50,
                                        cyChar*6,
                                        hwnd, (HMENU)ID_EDITH,//绑定按钮ID
                                        wincl.hInstance, NULL);
    hControl->hComBox[3] = CreateWindow(TEXT("combobox"),//必须为：button
                                        TEXT("00"),//按钮上显示的字符
                                        WS_CHILD | WS_VISIBLE|WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS,
                                        900, 200, 50,
                                        cyChar*6,
                                        hwnd, (HMENU)ID_EDITM,//绑定按钮ID
                                        wincl.hInstance, NULL);
    SendMessage(hControl->hComBox[3],WM_SETTEXT,0,(LPARAM)"00");
    hControl->hComBox[4] = CreateWindow(TEXT("combobox"),
                                        TEXT(""),
                                        WS_CHILD | WS_VISIBLE|WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
                                        860, 240, 50,
                                        cyChar*6,
                                        hwnd, (HMENU)ID_EDITT,//绑定按钮ID
                                        wincl.hInstance, NULL);
    SendMessage(hControl->hComBox[4],LB_SETCURSEL,0,(LPARAM)"2");
    char times[2]= {0};
    int k=0,temp;
    for(; k<=22-6; k++)
    {
        temp=k;
        sprintf(times,"%02d",temp+6);
        SendMessage(hControl->hComBox[2],CB_ADDSTRING,0,(LPARAM)times);
    }
    for(k=0; k<60; k++)
    {
        temp=k;
        sprintf(times,"%02d",temp);
        SendMessage(hControl->hComBox[3],CB_ADDSTRING,0,(LPARAM)times);
    }
    for(k=1; k<4; k++)
    {
        temp=k;
        sprintf(times,"%d",temp);
        SendMessage(hControl->hComBox[4],CB_ADDSTRING,0,(LPARAM)times);
    }
    for(k=0;k<G->numVertexes;k++)
    {
        char tem[20];
        strcpy(tem,G->adjList[k].data);
        SendMessage(hControl->hComBox[0],CB_ADDSTRING,0,(LPARAM)G->adjList[k].data);
        strcpy(G->adjList[k].data,tem);
        SendMessage(hControl->hComBox[1],CB_ADDSTRING,0,(LPARAM)tem);
    }
    SendMessage(hControl->hComBox[0],CB_SELECTSTRING,-1,(LPARAM)"径河站");
    SendMessage(hControl->hComBox[1],CB_SELECTSTRING,-1,(LPARAM)"三店站");
    SendMessage(hControl->hComBox[2],CB_SELECTSTRING,-1,(LPARAM)"07");
    FontAClass(hControl->hLab,hControl->hFont[0]);
    FontAClass(hControl->hComBox,hControl->hFont[0]);
    FontAClass(hControl->hBtn,hControl->hFont[0]);
}
void DeleteAClass(void*hwnd,int order)
{
    int i=0;
    switch(order)
    {
    case 1:
        while(((HBRUSH*)hwnd)[i])
        {
            DeleteObject(((HBRUSH*)hwnd)[i]);
            i++;
        }
    case 2:
        while(((HFONT*)hwnd)[i])
        {
            DeleteObject(((HFONT*)hwnd)[i]);
            i++;
        }
    case 3:
        while(((HBITMAP*)hwnd)[i])
        {
            DeleteObject(((HBITMAP*)hwnd)[i]);
        }
        i++;
    }
}

/*限制输入内容为合法内容*/
int EditControlLimit(HWND hWnd,int iResouceID,char *szLimit,int order)
{
    int iLength=SendMessage(GetDlgItem(hWnd,iResouceID),WM_GETTEXTLENGTH,0,0);//获得编辑框的长度
    char szContent[iLength+1];	//分配缓冲区，存储编辑框内容
    switch(order)
    {
    case 0:/*控制输入内容为合法字符串中的字符*/
    {
        if(iLength!=0)
        {
            int iPos=-1;	//记录非限定字符位置
            SendMessage(GetDlgItem(hWnd,iResouceID),WM_GETTEXT,iLength+1,(LPARAM)szContent);	//获得编辑框内容
            //printf("编辑框长度：%d\n",iLength);
            //判断缓冲区中是否包含非限定字符
            for(int i=0; i<iLength; i++)
            {
                //判断是包含非限定字符，如果包含，则记录非限定字符位置
                for(int j=0; j<strlen(szLimit); j++)
                {
                    iPos=i;
                    if(szContent[i]==szLimit[j])
                    {
                        iPos=-1;
                        break;
                    }
                }
                //如果存在非限定字符
                if(iPos!=-1)
                {
                    //将非限定字符后的所有字符，向前依次复制
                    for(int k=i; k<=iLength; k++)
                    {
                        szContent[k]=szContent[k+1];
                    }
                    //重设编辑框内容以及光标位置
                    SendMessage(GetDlgItem(hWnd,iResouceID),WM_SETTEXT,0,(LPARAM)szContent);
                    SendMessage(GetDlgItem(hWnd,iResouceID),EM_SETSEL,iPos,iPos);
                    break;
                }
            }
        }
        break;
    }

    case 1:/*判断输入是否为数据库中的内容*/
    {
        if(iLength==0)
        {
            sprintf(szContent,"%s不能为空~",szLimit);
            MessageBox(hWnd,szContent,"小贴士",MB_ICONWARNING|MB_OK);
            //Toast(hWnd,szContent);
        }
        else
        {
            SendMessage(GetDlgItem(hWnd,iResouceID),WM_GETTEXT,iLength+1,(LPARAM)szContent);	//获得编辑框内容
            int temp=LocateVex(G,szContent);
            if(temp!=-1)
                return temp;
            else
            {
                sprintf(szContent,"该%s不存在~",szLimit);
                MessageBox(hWnd,szContent,"小贴士",MB_ICONWARNING|MB_OK);
            }
        }
        break;
    }

    case 2:
    {
        if(iLength==0)
        {
            sprintf(szContent,"%s不能为空~",szLimit);
            MessageBox(hWnd,szContent,"小贴士",MB_ICONWARNING|MB_OK);
        }
        else
        {
            SendMessage(GetDlgItem(hWnd,iResouceID),WM_GETTEXT,iLength+1,(LPARAM)szContent);	//获得编辑框内容
            int temp=atoi(szContent);
            if(temp>5&&temp<23)
            {
                return temp;
            }
            else
            {
                sprintf(szContent,"%s格式不对喔~",szLimit);
                MessageBox(hWnd,szContent,"小贴士",MB_ICONWARNING|MB_OK);
                return -1;
            }
        }
        break;
    }
    case 3:
    {

        if(iLength==0)
        {
            sprintf(szContent,"%s不能为空~",szLimit);
            MessageBox(hWnd,szContent,"小贴士",MB_ICONWARNING|MB_OK);
        }
        else
        {
            SendMessage(GetDlgItem(hWnd,iResouceID),WM_GETTEXT,iLength+1,(LPARAM)szContent);	//获得编辑框内容
            int temp=atoi(szContent);
            if(temp>=0&&temp<60)
            {
                return temp;
            }
            else
            {
                sprintf(szContent,"%s格式不对喔~",szLimit);
                MessageBox(hWnd,szContent,"小贴士",MB_ICONWARNING|MB_OK);
                return -1;
            }
        }
        break;
    }
    }
    return -1;
}
/*
iPos=iLength-1;
for(int j=0;j<strlen(szLimit);j++)
{
    if(szContent[iLength-1]==szLimit[j])
    {
        iPos=-1;
        break;
    }
}
if(iPos!=-1)
{
    szContent[iPos]=0;
    SendMessage(GetDlgItem(hWnd,iResouceID),WM_SETTEXT,0,(LPARAM)szContent);
    SendMessage(GetDlgItem(hWnd,iResouceID),EM_SETSEL,iPos,iPos);
}*/
int main_path()
{
    FILE* fp;
    int err=fopen_s(&fp,"./data.txt", "r");
    if (err) {
        printf("数据文件丢失!");
        exit(1);
    }
    printf("正在读入站点和线路信息……\n");
    G=Init_Graph();
    if(!CreateALGraph(G,fp))
    {
        time.hour=7;
        time.minute=0;
        printf("读入成功！\n");
    }
    return 0;
}




