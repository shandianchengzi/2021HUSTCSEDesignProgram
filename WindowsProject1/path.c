#include"head.h"
#include"path.h"

int count = 0;
static Time_2 time_depart[10];

void Init_TimeDepart(Time_2 t[])
{
	t[0].hour = 6, t[0].minute = 0;
	t[1].hour = 7, t[1].minute = 30;
	t[2].hour = 9, t[2].minute = 0;
	t[3].hour = 9, t[3].minute = 30;
	t[4].hour = 15, t[4].minute = 0;
	t[5].hour = 16, t[5].minute = 30;
	t[6].hour = 18, t[6].minute = 30;
	t[7].hour = 19, t[7].minute = 0;
	t[8].hour = 22, t[8].minute = 0;
	t[9].hour = 23, t[9].minute = 0;
}

/*函数功能：使赶车时间与时刻表吻合
 *入口参数：赶车时间time
 *出口参数：赶车时间与时刻表之间的调整值k*/
float TimeTable(Time_2* time)
{                                       /*由于1h恰好包含20趟车，在时刻表中，不需要考虑小时的影响*/
	float k = (*time).minute;
	while (k > 3)
	{
		k -= 3;
	}
	k = 3 - k;							/*k是当前时间调整至下一趟发车时间的时间间隔*/
	time_add(time, k);					/*将当前选择时间调整至下一趟车的发车时间*/
	return k;                           /*返回调整的值*/
}

PathInfo* Init_Path()
{
	PathInfo* P = (PathInfo*)malloc(sizeof(PathInfo));
	if (!P)exit(1);
	P->money = 0;
	P->time = 0;
	P->true_time = 0;
	P->len = 0;
	P->transfer = 0;
	P->crowded = 0;
	memset(P->edge, 0, sizeof(EdgeNode*) * (MAX_VERTEX_NUM - 1));
	return P;
}

Status Free_Path(PathInfo** path)
{
	free(*path);
	*path = NULL;
	return 0;
}

int len_money(float len)
{
	if (len <= 0)
		return -1;
	if (len <= 4)
		return 2;
	if (len <= 8)
		return 3;
	if (len <= 12)
		return 4;
	if (len <= 18)
		return 5;
	if (len <= 24)
		return 6;
	if (len <= 32)
		return 7;
	if (len <= 40)
		return 8;
	if (len <= 50)
		return 9;
	return ((int)len - 50) / 20 + 10;
}

//如果b在时间区间内，则返回1，否则返回0；注：b等于a2时也返回0，视为不在区间内
int time_cmp(Time_2 a1, Time_2 a2, Time_2 b)
{
	if (a1.hour > b.hour)
		return 0;
	if (a1.hour < b.hour)
	{
		if (a2.hour < b.hour)
			return 0;
		if (a2.hour > b.hour)
		{
			return 1;
		}
		/*a2.hour==b.hour时*/
		if (a2.minute <= b.minute)
			return 0;
		else
			return 1;
	}
	/*a1.hour==b.hour时*/
	if (a1.minute > b.minute)
		return 0;
	else
		return 1;
}

int time_valid(Time_2 time)
{
	Time_2 a[2];
	a[0].hour = Time_Begin, a[0].minute = 0;
	a[1].hour = Time_End, a[1].minute = 0;
	return time_cmp(a[0], a[1], time);
}

/*函数功能：读取指定线路的指定时间点对应的拥挤百分数
 *入口参数：图G，其中存储了线路的拥挤百分数；线路序号lineIndex；时间点time
 *出口参数：整型的拥挤百分数；若输入出错，返回-1*/
int time_crowded(GraphAdjList* G, int lineIndex, Time_2 time)
{
	for (int i = 0;i < timepart;i++)
	{
		if (time_cmp(time_depart[i], time_depart[i + 1], time))
		{
			return G->lineDetail[lineIndex].crowded[i];
		}
	}
	return -1;
}

/*函数功能：利用拥挤百分数计算拥挤因子
 *入口参数：拥挤百分数crow；线路序号lineIndex
 *出口参数：浮点型的拥挤因子；注：若拥挤百分数不为0~100，视为异常情况，返回-1*/
float time_k(int crow, Time_2 time)
{
	Time_2 a[2];
	if (crow >= 0 || crow <= 20)
		return (float)k_relax;
	else if (crow <= 70)
		return (float)(k_crowded - k_relax) / 50 * (crow - 20) + k_relax;
	else if (crow < 100)
		return (float)k_crowded;
	return -1;
}

/*函数功能：为时间结构加几分钟；注：同样支持减功能，只要add为负数即可
 *入口参数：时间点结构time；浮点型加数add*/
void time_add(Time_2* time, float add)
{
	time->minute += add;
	while (time->minute >= 60)
	{
		time->hour++;
		time->minute -= 60;
	}
	while (time->minute < 0)
	{
		time->hour--;
		time->minute += 60;
	}
}

//path是数字数组，以-1结尾
Status Cal_TimeAll(GraphAdjList* G, PathInfo* P, Time_2 time, int cm)
{
	int i = 0;                    /*循环参数*/
	for (;P->edge[i];i++)
	{
		int crow = time_crowded(G, P->edge[i]->lineIndex, time);
		if (crow > cm)return -1;
		float w = Time_Go * (time_k(crow, time));
		P->time += w;
		P->true_time += Time_Go;
		P->crowded += crow;
		time_add(&time, Time_Go);
		if (P->edge[i + 1])
		{
			if (P->edge[i]->lineIndex == P->edge[i + 1]->lineIndex)
			{
				int crow = time_crowded(G, P->edge[i + 1]->lineIndex, time);
				if (crow > cm)return -1;
				w = Time_Stop * (time_k(crow, time));
				P->time += w;
				P->true_time += Time_Stop;
				time_add(&time, Time_Stop);
			}
			else
			{
				P->time += Time_Walk;
				P->true_time += Time_Walk;
				time_add(&time, Time_Walk);
				/*加上等车时间*/
				float k = TimeTable(&time);
				P->time += k;
				P->true_time += k;
				time_add(&time, k);
			}
		}
	}
	P->crowded = P->crowded * 1.0 / i;
	if (time_valid(time))
		return 0;
	else return -1;
}

Status Cal_MoneyLen(GraphAdjList* G, PathInfo* P)
{
	int i = 0;
	while (P->edge[i]) {
		P->len += P->edge[i]->distance;
		i++;
	}
	P->money = len_money(P->len);
	if (P->money != -1)return OK;
	else return -1;
}

int flag = 0;							/* 标志探寻点置0 */
int top = 0;
int Visited[MAX_VERTEX_NUM] = { 0 };    /* 记录结点是否被访问 */

/* 函数功能：DFS遍历图G求解符合要求的简单路径，并存储到路径数组P中
 * 入口参数：图G，路径数组P，起点站编号i，终点站编号j，发车时间time，最大换乘次数changeTimes，最大拥挤程度cm */
void pathdfs(GraphAdjList* G, PathInfo** P, int i, int j, Time_2 time, int changeTimes, int cm)
{
	/* 将顶点i入栈 */
	Visited[i] = visited;							
	P[count]->path[top++] = i;							

	/*当栈中有两个或以上元素时，开始记录栈的路径对应的线路*/
	if (top > 1)
	{
		/*得到顶点间对应的边的指针，加入到路径的边中*/
		P[count]->edge[top - 2] = LocateEdge(G, P[count]->path[top - 2], P[count]->path[top - 1]);

		/* 修正重复边中南站-洪山广场站带来的假换乘，由于结点间中最多两条重复边，故此方法有效 */
		EdgeNode* m = P[count]->edge[top - 2]->next;
		while (m && m->adjvex != P[count]->path[top - 1])	/* 寻找是否有重复边 */
			m = m->next;
		if (m)/* 如果有重复边，则判断走该重复边是否能让当前正在查找的路径不换乘 */
			if (top > 2 && P[count]->edge[top - 3]->lineIndex == m->lineIndex)
				/* 如果能，则选择走这条重复的边 */
				P[count]->edge[top - 2] = m;

		/* 当记录的路线大于一条时，可开始判断是否出现换乘 */
		if (top > 2 && P[count]->edge[top - 3]->lineIndex != P[count]->edge[top - 2]->lineIndex)
		{
			P[count]->transfer++;

			/* 当换乘的次数超过最大限制次数时，顶点回溯 */
			if (P[count]->transfer > changeTimes)
			{
				P[count]->path[top - 1] = 0;
				Visited[i] = unvisited;
				P[count]->transfer--;
				top--;
				return;
			}
		}
	}

	/* 存在顶点i到顶点j的路径，令flag=1，并进行路径存储操作 */
	if (!flag && P[count]->path[top - 1] == j)
		flag = 1;
	if (flag)
	{
		/* 为数组的边界赋值，以便后续进行边界检测 */
		P[count]->edge[top - 1] = NULL;
		P[count]->path[top - 1] = j;
		P[count]->path[top] = -1;
		P[count]->stationNum = top;

		/* 判断查找到的线路是否是合乎要求的线路 */
		/* 若计算结果等于-1，说明结果的时间超过地铁运营时间，或者经过了超过最大拥挤度的路段 */
		if (Cal_TimeAll(G, P[count], time, cm) != -1)
		{
			/* 进一步计算路径的票价和里程数 */
			Cal_MoneyLen(G, P[count]);		
			/* 利用路径里程和综合评估时间，排除一样的路径（路径重复原因未知）*/
			int k = 0;
			for (;k < count;k++)
				if (P[count]->len == P[k]->len && P[count]->time == P[k]->time)
					break;
			/* 若当前路径并非重复路径，则新建并初始化路径结构体 */
			if (k == count)
			{
				count++;
				P[count] = Init_Path();
				/* 为下一个结构体复制站点、路径和换乘状态 */
				for (int s = 0; s < top && P[count - 1]->path[s] != j; s++)
				{
					P[count]->path[s] = P[count - 1]->path[s];
					if (P[count - 1]->edge[k])
						P[count]->edge[s] = P[count - 1]->edge[s];
				}
				P[count]->transfer = P[count - 1]->transfer;
			}
		}
		/* 若上一条路径是不合理的路径，则还原参与计算的各项计算内容 */
		else
		{
			P[count]->time = 0;
			P[count]->true_time = 0;
			P[count]->crowded = 0;
		}
		flag = 0;/* 还原flag状态 */
	}

	/* 不存在顶点i到顶点j的路径时，查找该结点的邻接结点 */
	else
		for (EdgeNode* p = G->adjList[i].firstedge; p; p = p->next)
		{
			if (Visited[p->adjvex] == unvisited)
				pathdfs(G, P, p->adjvex, j, time, changeTimes, cm);
		}

	/* 顶点回溯，将顶点i出栈 */
	/* Visited[i]置为unvisited，下次可通过顶点i寻找其他可能到达j的路径 */
	Visited[i] = unvisited;
	top--;

	/* 判断该回溯站点是否是换乘的站点，如果是，则将换乘数减一 */
	if (top > 1 && P[count]->edge[top - 2] && P[count]->edge[top - 1])
		if (P[count]->edge[top - 2]->lineIndex != P[count]->edge[top - 1]->lineIndex)
			P[count]->transfer--;
}

/*c是线路总数*/
int Path_Sort_Weight(PathInfo* P[], int c) {
	int i = 0, j;
	for (;i < 3 && i < c;i++) {
		for (j = i + 1;j < c;j++) {
			/*当综合指数相差不大时，优先推荐拥挤更少的线路*/
			if ((P[i]->time > P[j]->time + 0.001)
				|| ((P[i]->time > P[j]->time - 0.001) && P[i]->crowded > P[j]->crowded))
			{
				PathInfo* temp = P[j];
				P[j] = P[i];
				P[i] = temp;
			}
		}
	}
	if (i < 3)return i;
	else return 3;
}
int Path_Sort_Time(PathInfo* P[], int c) {
	int i = 0, j;
	for (;i < 3 && i < c;i++) {
		for (j = i + 1;j < c;j++) {
			if ((P[i])->true_time > (P[j])->true_time)
			{
				PathInfo* temp = P[j];
				P[j] = P[i];
				P[i] = temp;
			}
		}
	}
	if (i < 3)return i;
	else return 3;
}
int Path_Sort_Money(PathInfo* P[], int c) {
	int i = 0, j;
	for (;i < 3 && i < c;i++) {
		for (j = i + 1;j < c;j++) {
			if ((P[i])->money > (P[j])->money)
			{
				PathInfo* temp = P[j];
				P[j] = P[i];
				P[i] = temp;
			}
		}
	}
	if (i < 3)return i;
	else return 3;
}
int Path_Sort_Crowded(PathInfo* P[], int c) {
	int i = 0, j;
	for (;i < 3 && i < c;i++) {
		for (j = i + 1;j < c;j++) {
			if ((P[i])->crowded > (P[j])->crowded)
			{
				PathInfo* temp = P[j];
				P[j] = P[i];
				P[i] = temp;
			}
		}
	}
	if (i < 3)return i;
	else return 3;
}
int get_All(GraphAdjList* G, PathInfo* P[], int i, int j, Time_2 time, int c, int cm) {
	Init_TimeDepart(time_depart);
	P[count] = Init_Path();
	pathdfs(G, P, i, j, time, c, cm);
	int l = count;
	if (P[count])free(P[count]);
	P[count] = 0;
	count = 0;
	return l;
	/*Path_Sort_Time(P, count);
	for (l = 0;l < count;l++)
	{
		if (l >= 3)
		{
			Free_Path(&P[l]);
		}
	}
	count = 0;
	if (l < 3)return l;
	else return 3;*/
}
