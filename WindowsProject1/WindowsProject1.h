#pragma once

#include"head.h"
#include"Resource.h"
#include"path.h"
#include<commctrl.h>
//#define BGCOLOR RGB(248,246,231)//米白色，文本背景颜色
#define BGCOLOR RGB(248,246,231)//米白色，文本背景颜色
//#define BGCOLOR2 RGB(0,122,255)//字体颜色
#define BGCOLOR2 RGB(0,51,153)//字体颜色
#define BGCOLOR3 RGB(248,246,231)//控件背景颜色
#define Btn_Width 460
#define Btn_Height 144
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("subway");
int WIDTH = 1000;
int HEIGHT = 660;
int pWidth = 6339/4;
int pHeight = 7180/4;
int iVertPos, iHorzPos;
HDC hdc;
PAINTSTRUCT ps;
RECT rect;
TEXTMETRIC tm;
TCITEM ti;
TCHAR g_achTemp[256];
SCROLLINFO si;
WNDCLASSEX wincl;        /* Data structure for the windowclass */
//声明变量
GraphAdjList* G;
PathInfo* P[999];
char bStation[50];
char eStation[50];
char hTime[3];
char mTime[3];
char cMax[3];
int crowded_Max;
int countt;
int cxChar, cxCaps, cxClient, cyChar, cyClient, iMaxWidth, ivex, jvex, when_up;
char transferTimes[2];
int* transferPlan[2];
int howInput = 1;
Time_2 time;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
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
    HWND hDialog[10];
    HWND hSlider[10];
} lhControl;
lhControl hc;
lhControl* hControl = &hc;
void FontAClass(HWND* hwnd, HFONT hFont)
{
    int i = 0;
    while (hwnd[i])
    {
        SendMessage(hwnd[i], WM_SETFONT, (WPARAM)hFont, NULL);
        i++;
    }
}
void AdjustRC(HWND hwnd, int left, int top, int right, int bottom)
{
    RECT rc;
    // Get the current control rectangle
    SendMessage(hwnd, EM_GETRECT, 0, (LPARAM)&rc);
    rc.left += left; // increase the left margin 
    rc.top += top; // increase the top margin
    rc.right -= right; // decrease the right margin
    rc.bottom -= bottom; // decrease the bottom margin (rectangle)
    // Set the rectangle
    SendMessage(hwnd, EM_SETRECT, 0, (LPARAM)&rc);
}
void append_edit(HWND hwnd, char* s)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)s);
}
void strcat1(char* s1, char* s2)
{
    strcat_s(_strrev(s1), 100, _strrev(s2));
    _strrev(s2);
    _strrev(s1);
}
int LocateLineSta(GraphAdjList* G,int line, char* str)
{
    for (int i = 1;i <= G->lineDetail[line].lineCount;i++)
    {
        if (!strcmp(G->lineDetail[line].lineStations[i],str))
        {
            return i;
        }
    }
    return -1;
}
void tips_station(HWND hwnd, char* str)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    char szBuffer[40] = { 0 }, szBuffer1[40] = { 0 }, szBuffer2[40] = { 0 }, sss[10] = "%*d.%*d,";
    char o1[100] = "%[^.]d", o2[100] = "%*d.%[^;,]d";
    hash_struct* temp = find_station(G->stations, str);
    sprintf_s(szBuffer, "　%03d.", temp->data.index+1);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)str);
    while (sscanf_s(temp->data.whose_where, o1, szBuffer1,40))
    {
        sscanf_s(temp->data.whose_where, o2, szBuffer2,40);
        sprintf_s(szBuffer, ", %d号线第%s站", G->lineDetail[atoi(szBuffer1)].lineIndex, szBuffer2);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        strcat1(o1, sss);
        strcat1(o2, sss);
    }
    if (find_station(G->stations, str)->data.isTransfer)
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)", 是换乘站");
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)";\r\n");
    SendMessage(hwnd, EM_SETSEL, 0, 0);
}
int* tips_sta_inline(HWND hwnd, int line,int sta,int special)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    char szBuffer[100];
    int* transferPlan = (int*)malloc(sizeof(int) * 4);
    hash_struct* temp = find_station(G->stations, G->lineDetail[line].lineStations[sta]);
    sprintf_s(szBuffer, "　%02d. %s，编号%03d。\r\n", sta,temp->name,temp->data.index);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
    int flag = 0, j = 0;
    for (EdgeNode* p = G->adjList[temp->data.index].firstedge; p; p = p->next) 
    {
        if (p->lineIndex != line)
        {
            flag = 1;
            if (j < 4)
            {
                transferPlan[j] = G->lineDetail[p->lineIndex].lineIndex;
                transferPlan[j + 1] = LocateLineSta(G, p->lineIndex, G->adjList[p->adjvex].data);
            }
            j += 2;
            sprintf_s(szBuffer, "　↑可换乘%d号线%02d%s。\r\n",
                G->lineDetail[p->lineIndex].lineIndex,LocateLineSta(G,p->lineIndex, G->adjList[p->adjvex].data),G->adjList[p->adjvex].data);
            SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        }
        else if (special)
        {
            sprintf_s(szBuffer, "　↑与之相连的站点有%02d%s。\r\n",
                LocateLineSta(G, p->lineIndex, G->adjList[p->adjvex].data), G->adjList[p->adjvex].data);
            SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        }
    }
    SendMessage(hwnd, EM_SETSEL, 0, 0);
    if (flag)return transferPlan;
    else 
    {
        free(transferPlan);
        return 0;
    }
}

void tips_stations(HWND hwnd, char* s[500], int now)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    switch (now)
    {
    case 0:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"上车时间：\r\n");
        break;
    case 1:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)" - 起点站信息：\r\n");
        break;
    case 2:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)" - 终点站信息：\r\n");
        break;
    case 3:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"所有站点信息：\r\n");
        break;
    case 4:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"推荐线路：\r\n");
        break;
    case 5:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"不能为空喔~\r\n");
        break;
    case 6:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"没有这个站喔~\r\n");
        break;
    case 7:
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"==========================\r\n");
        break;
    }
    SendMessage(hwnd, EM_SETSEL, 0, 0);
    if (!s)return;
    int i = 0;
    while (s[i])
    {
        tips_station(hwnd, s[i]);
        i++;
    }
}

void tips_nearby(HWND hwnd, char* str)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    char szBuffer[100];
    sprintf_s(szBuffer, "　与%s相连的站点有：", str);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
    int i = LocateVex(G, str);
    for (EdgeNode* p = G->adjList[i].firstedge; p; p = p->next)
    {
        sprintf_s(szBuffer, "%03d%s(%d号线)", LocateVex(G, G->adjList[p->adjvex].data), G->adjList[p->adjvex].data, G->lineDetail[p->lineIndex].lineIndex);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        if (!(p->next))SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"。\r\n");
        else SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"; ");
    }
    tips_stations(hwnd, NULL, 7);
    SendMessage(hwnd, EM_SETSEL, 0, 0);
}

void tips_input(HWND hwnd,int order)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    if (order == 2)
    {
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"输入提示：\r\n　");
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"1. 起点站和终点站不能为空，且不能相同；\r\n　");
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"2. 时间只能输入整数喔~！\r\n　");
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"3. 输入模式有两种：1)输入站点名字; 2)输入站点是第几条线路的第几号站。\r\n　");
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"4. 不记得站点名字？看看地图吧~！\r\n");
        tips_stations(hwnd, NULL, 7);
    }
    else if (order == 1)
    {
        char* stations[500];
        for (int k = 0; k < G->numVertexes; k++)
        {
            stations[k] = G->adjList[k].data;
        }
        stations[(G->numVertexes)] = 0;
        tips_stations(hwnd, stations, 3);
    }
}
void tips_line(HWND hwnd, int line,int which)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    char szBuffer[100];
    if(which)tips_stations(hwnd, NULL, which);
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    sprintf_s(szBuffer, "　第%d号线的信息如下：\r\n　满员%d人，长度%.2f千米，%d站点。\r\n", G->lineDetail[line].lineIndex, G->lineDetail[line].lineFull, G->lineDetail[line].lineLength, G->lineDetail[line].lineCount);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
    int i = 1;
    while (i<=G->lineDetail[line].lineCount)
    {
        tips_sta_inline(hwnd, line,i,0);
        i++;
    }
    SendMessage(hwnd, EM_SCROLLCARET, 1, 2);
}
void tips_table(HWND hwnd, Time_2 time)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"|　到　站　|　发　车　|\r\n");
    Time_2 out;
    Time_2 in;
    out.minute = time.minute - ((int)(time.minute)) % 3;
    out.hour = time.hour;
    for (int i = 0;i < 3;i++)
    {
        in = out;
        if (out.minute == 0)
        {
            in.minute = 59;
            in.hour--;
        }
        else
        {
            in.minute--;
        }
        char szBuffer[100];
        if (out.hour != 6 || out.minute != 0)
        {
            if (out.hour != 23 || out.minute != 0)
            {
                sprintf_s(szBuffer, "|　%02d: %02d　|　%02d: %02d　|\r\n", in.hour, (int)(in.minute), out.hour, (int)(out.minute));
                SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
            }
            else
            {
                sprintf_s(szBuffer, "|　%02d: %02d　|　　　　　|\r\n", in.hour, (int)(in.minute));
                SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
                break;
            }
        }
        else
        {
            sprintf_s(szBuffer, "|　　　　　|　%02d: %02d　|\r\n", out.hour, (int)(out.minute));
            SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        }
        time_add(&out, 3);
    }
}
void Show_Path(HWND hwnd, GraphAdjList* G, PathInfo** P,int countt)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    if (!countt)
    {
        tips_stations(hwnd, NULL, 7);
        SendMessage(hwnd, EM_SETSEL, -2, -1);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"竟然没有可以推荐的路线。\r\n换些条件试试吧~！");
        SendMessage(hwnd, EM_SETSEL, 0, 0);
        return;
    }
    char szBuffer[100];
    int i;
    for (i = 0; i < countt; i++)
    {
        sprintf_s(szBuffer, "==========推荐路线%d==========\r\n　",
            i + 1);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        Time_2 timeOrigin=time;
        int h = (int)(P[i]->true_time) / 60;
        int m = (int)(P[i]->true_time - 60 * h);
        time_add(&timeOrigin,P[i]->true_time );
        sprintf_s(szBuffer, "用时%.02d小时%.02d分钟，\r\n　预计到达时间：%02d:%02d。\r\n　",h,
            m,timeOrigin.hour,(int)timeOrigin.minute);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        sprintf_s(szBuffer, "拥挤平均程度%.2f％,综合指数%.2f。\r\n　",
            P[i]->crowded,P[i]->time);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        sprintf_s(szBuffer, "票价%d元，换乘%d次，总里程%.2f公里。\r\n",
            P[i]->money, P[i]->transfer, P[i]->len);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        int j = 0, lastIndex = -1;
        sprintf_s(szBuffer, "　经过%d个站点：\r\n",
            P[i]->stationNum);
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
        int nowIndex=0;
        while (P[i]->path[j] != -1)
        {
            if (P[i]->path[j + 1] != -1)
            {
                nowIndex = P[i]->edge[j]->lineIndex;
                if (lastIndex != -1)
                {
                    if (lastIndex != nowIndex)
                    {
                        sprintf_s(szBuffer, "%02d%s\r\n　%d号线: %02d%s → ",
                            LocateLineSta(G, nowIndex, G->adjList[P[i]->path[j]].data),G->adjList[P[i]->path[j]].data,G->lineDetail[nowIndex].lineIndex, LocateLineSta(G, nowIndex, G->adjList[P[i]->path[j]].data), G->adjList[P[i]->path[j]].data);
                        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
                        lastIndex = nowIndex;
                        j++;
                        continue;
                    }
                }
                else
                {
                    sprintf_s(szBuffer, "　%d号线: ", G->lineDetail[nowIndex].lineIndex);
                    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
                }
                lastIndex = nowIndex;
                sprintf_s(szBuffer, "%02d%s → ", LocateLineSta(G, nowIndex, G->adjList[P[i]->path[j]].data), G->adjList[P[i]->path[j]].data);
                SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
            }
            else
            {
                sprintf_s(szBuffer,"%02d%s。\r\n\r\n", LocateLineSta(G, nowIndex, G->adjList[P[i]->path[j]].data),G->adjList[P[i]->path[j]].data);
                SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
            }
            j++;
        }
        //Free_Path(&P[i]);
    }
    SendMessage(hwnd, EM_SETSEL, 0, 0);
}
void tips_output(HWND hwnd,int when_up,char *times)
{
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"　");
    if(when_up==3)
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"当前地铁正停靠站内，但上车时间紧迫，建议等待2分钟乘坐下一趟~\r\n");
    else if(when_up==2)
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"下一趟地铁还有1分钟就要到站啦~请尽快前往乘车地点。\r\n");
    else if(when_up==1)
        SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"当前地铁正停靠站内，距离发车时间还有1分钟~请尽快前往乘车地点。\r\n");
    tips_stations(hwnd, NULL, 7);
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    tips_stations(hwnd, NULL, 0);
    char szBuffer[100];
    SendMessage(hwnd, EM_SETSEL, -2, -1);
    sprintf_s(szBuffer, "%02d:%02d\r\n最大换乘次数：\r\n　%s\r\n期望路线：\r\n　%s → %s\r\n", time.hour, (int)(time.minute), times, bStation, eStation);
    SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)szBuffer);
    //SendMessage(hwnd, EM_REPLACESEL, 1, (LPARAM)"\r\n");
    SendMessage(hwnd, EM_SCROLLCARET, 1, 2);
}
void AddControls(HWND hwnd, lhControl* hControl)
{   
    /*记录Dialog控件句柄并分组存放，以便之后统一字体*/
    hControl->hLab[0] = GetDlgItem(hwnd, ID_TEXTI);
    hControl->hComBox[0] = GetDlgItem(hwnd, ID_EDITB);
    hControl->hComBox[1] = GetDlgItem(hwnd, ID_EDITS);
    hControl->hComBox[2] = GetDlgItem(hwnd, ID_EDITH);
    hControl->hComBox[3] = GetDlgItem(hwnd, ID_EDITM);
    hControl->hComBox[4] = GetDlgItem(hwnd, ID_EDITT);
    hControl->hComBox[5] = GetDlgItem(hwnd, ID_EDITB1);
    hControl->hComBox[6] = GetDlgItem(hwnd, ID_EDITB2);
    hControl->hComBox[7] = GetDlgItem(hwnd, ID_EDITS1);
    hControl->hComBox[8] = GetDlgItem(hwnd, ID_EDITS2);
    hControl->hComBox[9] = GetDlgItem(hwnd, IDC_COMBO2);
    /*填充Combobox的选项*/
    char times[3] = { 0 };
    int k = 0, temp;
    for (; k <= 22 - 6; k++)
    {
        temp = k;
        sprintf_s(times,3, "%02d", temp + 6);
        SendMessage(hControl->hComBox[2], CB_ADDSTRING, 0, (LPARAM)times);
    }
    for (k = 0; k < 60; k++)
    {
        temp = k;
        sprintf_s(times,3, "%02d", temp);
        SendMessage(hControl->hComBox[3], CB_ADDSTRING, 0, (LPARAM)times);
    }
    for (k = 1; k <= 4; k++)
    {
        temp = k;
        sprintf_s(times,3, "%d", temp);
        SendMessage(hControl->hComBox[4], CB_ADDSTRING, 0, (LPARAM)times);
    }
    for (k = 0; k < G->numVertexes; k++)
    {
        char tem[20];
        strcpy_s(tem, 20,G->adjList[k].data);
        SendMessage(hControl->hComBox[0], CB_ADDSTRING, 0, (LPARAM)G->adjList[k].data);
        strcpy_s(G->adjList[k].data,20, tem);
        SendMessage(hControl->hComBox[1], CB_ADDSTRING, 0, (LPARAM)tem);
    }
    for (k = 0; k<linenum; k++)
    {
        sprintf_s(times, 3, "%d", G->lineDetail[k].lineIndex);
        SendMessage(hControl->hComBox[5], CB_ADDSTRING, 0, (LPARAM)times);
        sprintf_s(times, 3, "%d", G->lineDetail[k].lineIndex);
        SendMessage(hControl->hComBox[7], CB_ADDSTRING, 0, (LPARAM)times);
    }
    for (k = 1;k <= G->lineDetail[0].lineCount;k++)
    {
        temp = k;
        sprintf_s(times, 3, "%d", temp);
        SendMessage(hControl->hComBox[6], CB_ADDSTRING, 0, (LPARAM)times);
        sprintf_s(times, 3, "%d", temp);
        SendMessage(hControl->hComBox[8], CB_ADDSTRING, 0, (LPARAM)times);
    }
    for (k = 0; k <= 99; k++)
    {
        temp = k;
        sprintf_s(times, 3, "%02d", temp);
        SendMessage(hControl->hComBox[9], CB_ADDSTRING, 0, (LPARAM)times);
    }

    /*为选择框设置默认选项：当前时间7:00，换乘2次或以下，1号线1号站(径河站)->1号线2号站(三店站)，输入最大长度15，最大拥挤度99*/
    SendMessage(hControl->hComBox[2], CB_SELECTSTRING, -1, (LPARAM)"07");       /*小时：07*/
    SendMessage(hControl->hComBox[3], CB_SELECTSTRING, -1, (LPARAM)"00");       /*分钟：00*/
    SendMessage(hControl->hComBox[4], CB_SETCURSEL, 1, 0);                      /*换乘次数：2*/
    SendMessage(hControl->hComBox[0], CB_SELECTSTRING, -1, (LPARAM)"径河站");   /*站点：径河站*/
    SendMessage(hControl->hComBox[1], CB_SELECTSTRING, -1, (LPARAM)"三店站");   /*站点：三店站*/
    SendMessage(hControl->hComBox[5], CB_SETCURSEL, 0, 0);                      /*线路：一号线*/
    SendMessage(hControl->hComBox[6], CB_SETCURSEL, 0, 0);                      /*站点：第一站*/
    SendMessage(hControl->hComBox[7], CB_SETCURSEL, 0, 0);                      /*线路：一号线*/
    SendMessage(hControl->hComBox[8], CB_SETCURSEL, 1, 0);                      /*站点：第二站*/
    SendMessage(hControl->hComBox[0], CB_LIMITTEXT, 15, 0);                     /*限制输入长度为15字符*/
    SendMessage(hControl->hComBox[1], CB_LIMITTEXT, 15, 0);                     /*限制输入长度为15字符*/
    SendMessage(hControl->hComBox[9], CB_SELECTSTRING, -1, (LPARAM)"99");       /*最大拥挤度限制为99*/
    /*为指定控件组设置字体*/
    FontAClass(hControl->hLab, hControl->hFont[0]);
    FontAClass(hControl->hComBox, hControl->hFont[0]);
    /*隐藏输入模式2对应的控件*/
    ShowWindow(hControl->hComBox[0], SW_HIDE);
    ShowWindow(hControl->hComBox[1], SW_HIDE);
    /*调整输出框的内边距*/
    AdjustRC(GetDlgItem(hwnd, ID_TEXTI), 10, 10, 10, 0);
    /*为输出框增加默认内容*/
    tips_input(GetDlgItem(hwnd, ID_TEXTI),2);
    tips_table(GetDlgItem(hwnd, ID_TEXTI), time);
    tips_line(GetDlgItem(hwnd, ID_TEXTI), 0, 0);
}
int main_path()
{
    FILE* fp;
    int err = fopen_s(&fp, "./data.txt", "r");
    if (err) {
        printf("数据文件丢失!");
        exit(1);
    }
    printf("正在读入站点和线路信息……\n");
    G = Init_Graph();
    if (!CreateALGraph(G, fp))
    {
        time.hour = 7;
        time.minute = 0;
        printf("读入成功！\n");
    }
    return 0;
}
void DeleteAClass(void* hwnd, int order)
{
    int i = 0;
    switch (order)
    {
    case 1:
        while (((HBRUSH*)hwnd)[i])
        {
            DeleteObject(((HBRUSH*)hwnd)[i]);
            i++;
        }
        break;
    case 2:
        while (((HFONT*)hwnd)[i])
        {
            DeleteObject(((HFONT*)hwnd)[i]);
            i++;
        }
        break;
    case 3:
        while (((HBITMAP*)hwnd)[i])
        {
            DeleteObject(((HBITMAP*)hwnd)[i]);
            i++;
        }
        break;
    }
}
/*限制输入内容为合法内容*/
int EditControlLimit(HWND hWnd, int iResouceID, char* szLimit, int order)
{
    int iLength=SendMessage(GetDlgItem(hWnd, iResouceID), WM_GETTEXTLENGTH, 0, 0);//获得编辑框的长度
    char szContent[500] = { 0 };	//分配缓冲区，存储编辑框内容
    switch (order)
    {
    case 0:/*控制输入内容为合法字符串中的字符*/
    {
        if (iLength != 0)
        {
            int iPos = -1;	//记录非限定字符位置
            SendMessage(GetDlgItem(hWnd, iResouceID), WM_GETTEXT, iLength + 1, (LPARAM)szContent);	//获得编辑框内容
            //printf("编辑框长度：%d\n",iLength);
            //判断缓冲区中是否包含非限定字符
            for (int i = 0; i < iLength; i++)
            {
                //判断是包含非限定字符，如果包含，则记录非限定字符位置
                for (int j = 0; j < (int)strlen(szLimit); j++)
                {
                    iPos = i;
                    if (szContent[i] == szLimit[j])
                    {
                        iPos = -1;
                        break;
                    }
                }
                //如果存在非限定字符
                if (iPos != -1)
                {
                    //将非限定字符后的所有字符，向前依次复制
                    for (int k = i; k <= iLength; k++)
                    {
                        szContent[k] = szContent[k + 1];
                    }
                    //重设编辑框内容以及光标位置
                    SendMessage(GetDlgItem(hWnd, iResouceID), WM_SETTEXT, 0, (LPARAM)szContent);
                    SendMessage(GetDlgItem(hWnd, iResouceID), CB_SETEDITSEL, 0, (LPARAM)(iPos|(iPos<<16))); /*选择编辑框中的内容，低16位代表选择的起始位置，高16位代表结束位置*/
                    break;
                }
            }
        }
        break;
    }

    case 1:/*判断输入是否为数据库中的内容*/
    {
        if (iLength == 0)
        {
            sprintf_s(szContent, "%s不能为空~", szLimit);
            MessageBox(hWnd, szContent,"小贴士", MB_ICONWARNING | MB_OK);
            //Toast(hWnd,szContent);
        }
        else
        {
            SendMessage(GetDlgItem(hWnd, iResouceID), WM_GETTEXT, iLength + 1, (LPARAM)szContent);	//获得编辑框内容
            int temp = LocateVex(G, szContent);
            if (temp != -1)
                return temp;
            else
            {
                sprintf_s(szContent, "该%s不存在~", szLimit);
                MessageBox(hWnd,  szContent,  "小贴士", MB_ICONWARNING | MB_OK);
            }
        }
        break;
    }
    }
    return -1;
}

int EditControlRange(HWND hWnd, int iResouceID, char* szLimit, int mi,int ma)
{
    int iLength = SendMessage(GetDlgItem(hWnd, iResouceID), WM_GETTEXTLENGTH, 0, 0);//获得编辑框的长度
    char szContent[20] = { 0 };	//分配缓冲区，存储编辑框内容
    if (iLength == 0)
    {
        sprintf_s(szContent, "%s不能为空~", szLimit);
        MessageBox(hWnd, szContent, "小贴士", MB_ICONWARNING | MB_OK);
    }
    else
    {
        SendMessage(GetDlgItem(hWnd, iResouceID), WM_GETTEXT, iLength + 1, (LPARAM)szContent);	//获得编辑框内容
        int temp = atoi(szContent);
        if (temp >= mi && temp <= ma)
        {
            return temp;
        }
        else
        {
            sprintf_s(szContent, "%s格式不对喔~", szLimit);
            MessageBox(hWnd, szContent, "小贴士", MB_ICONWARNING | MB_OK);
            return -1;
        }
    }
    return -1;
}

int Checkinput(HWND hwnd)
{
    if (!howInput)
    {
        GetWindowText(hControl->hComBox[0], bStation, 30);
        ivex = EditControlLimit(hwnd, ID_EDITB, (char*)"起点站", 1);
    }
    else
    {
        int line = SendMessage(GetDlgItem(hwnd, ID_EDITB1), CB_GETCURSEL, 0, 0);
        char tempstr[3];
        GetWindowText(GetDlgItem(hwnd, ID_EDITB2), tempstr, 3);
        int sta = atoi(tempstr);
        if (sta <= G->lineDetail[line].lineCount)
        {
            strcpy_s(bStation, 20, G->lineDetail[line].lineStations[sta]);
            ivex = LocateVex(G, bStation);
        }
        else
        {
            MessageBox(hwnd, "起点站编号有问题喔~", "小贴士", MB_ICONWARNING | MB_OK);
            return -1;
        }
    }
    if (ivex != -1)
    {
        if (!howInput)
        {
            GetWindowText(hControl->hComBox[1], eStation, 30);
            jvex = EditControlLimit(hwnd, ID_EDITS, (char*)"终点站", 1);
        }
        else
        {
			int line = SendMessage(GetDlgItem(hwnd, ID_EDITS1), CB_GETCURSEL, 0, 0);
			char tempstr[3];
			GetWindowText(GetDlgItem(hwnd, ID_EDITS2), tempstr, 3);
			int sta = atoi(tempstr);
			if (sta <= G->lineDetail[line].lineCount)
			{
				strcpy_s(eStation, 20, G->lineDetail[line].lineStations[sta]);
				jvex = LocateVex(G, eStation);
			}
			else
			{
				MessageBox(hwnd, "终点站编号有问题喔~", "小贴士", MB_ICONWARNING | MB_OK);
				return -1;
			}
		}
        if (jvex != -1)
        {
            if (ivex == jvex)
            {
                MessageBox(hwnd, "起点站和终点站不可以相同~", "小贴士", MB_ICONWARNING | MB_OK);
                return -1;
            }
            GetWindowText(GetDlgItem(hwnd, ID_EDITH), hTime, 2);
            time.hour = EditControlRange(hwnd, ID_EDITH, (char*)"小时", 6,22);
            if (time.hour != -1)
            {
                GetWindowText(hControl->hComBox[2], mTime, 2);
                time.minute = (float)EditControlRange(hwnd, ID_EDITM, (char*)"分钟", 0,59);
                if (time.minute != -1)
                {
                    GetWindowText(hControl->hComBox[9], cMax, 2);
                    crowded_Max= EditControlRange(hwnd, IDC_COMBO2, (char*)"最大拥挤程度", 0,99);
                    if (crowded_Max != -1)
                    {
                        GetWindowText(hControl->hComBox[4], transferTimes, 2);
                        countt = get_All(G, P, ivex, jvex, time, int(transferTimes[0] - '0'),crowded_Max);
                        when_up = (int)TimeTable(&time);
                    }
                }
                return 0;
            }
            //Show_Path(G,P);
        }
    }
    return -1;
}
void ShowResult(HWND OutputEdit,int order)
{
    /*0.综合推荐；1.时间短；2.票价低；3.拥挤少*/
    int number = 0;
    switch (order) 
    {
    case 0: 
    {
        SetWindowText(OutputEdit, "");
        SendMessage(OutputEdit, EM_REPLACESEL, 1, (LPARAM)"综合评估推荐路线：\r\n");
        tips_output(OutputEdit, when_up, transferTimes);
        number = Path_Sort_Weight(P, countt);
        break;
    }
    case 1:
    {
        SetWindowText(OutputEdit, "");
        SendMessage(OutputEdit, EM_REPLACESEL, 1, (LPARAM)"时间短推荐路线：\r\n");
        tips_output(OutputEdit, when_up, transferTimes);
        number = Path_Sort_Time(P, countt);
        break;
    }
    case 2:
    {
        SetWindowText(OutputEdit, "");
        SendMessage(OutputEdit, EM_REPLACESEL, 1, (LPARAM)"票价低推荐路线：\r\n");
        tips_output(OutputEdit, when_up, transferTimes);
        number = Path_Sort_Money(P, countt);
        break;
    }
    case 3:
    {
        SetWindowText(OutputEdit, "");
        SendMessage(OutputEdit, EM_REPLACESEL, 1, (LPARAM)"拥挤少推荐路线：\r\n");
        tips_output(OutputEdit, when_up, transferTimes);
        number = Path_Sort_Crowded(P, countt);
        break;
    }
    }
    Show_Path(OutputEdit, G, P, number);
    SendMessage(OutputEdit, EM_SCROLLCARET, 1, 2);
}
void changeclick(HWND hWnd,int who,int lineIndex,int sta)
{
    free(transferPlan[who]);
    transferPlan[who] = tips_sta_inline(GetDlgItem(hWnd, ID_TEXTI), lineIndex, sta,1);
    if (transferPlan[who])
    {
        ShowWindow(GetDlgItem(hWnd, IDC_CHANGE1 + 2 * who), SW_SHOWNA);
        ShowWindow(GetDlgItem(hWnd, IDC_CHANGE1 + 2 * who + 1), SW_SHOWNA);
    }
    else
    {
        ShowWindow(GetDlgItem(hWnd, IDC_CHANGE1 + 2 * who), SW_HIDE);
        ShowWindow(GetDlgItem(hWnd, IDC_CHANGE1 + 2 * who + 1), SW_HIDE);
    }
}