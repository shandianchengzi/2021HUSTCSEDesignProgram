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

/*�������ܣ�ʹ�ϳ�ʱ����ʱ�̱��Ǻ�
 *��ڲ������ϳ�ʱ��time
 *���ڲ������ϳ�ʱ����ʱ�̱�֮��ĵ���ֵk*/
float TimeTable(Time_2* time)
{                                       /*����1hǡ�ð���20�˳�����ʱ�̱��У�����Ҫ����Сʱ��Ӱ��*/
	float k = (*time).minute;
	while (k > 3)
	{
		k -= 3;
	}
	k = 3 - k;							/*k�ǵ�ǰʱ���������һ�˷���ʱ���ʱ����*/
	time_add(time, k);					/*����ǰѡ��ʱ���������һ�˳��ķ���ʱ��*/
	return k;                           /*���ص�����ֵ*/
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

//���b��ʱ�������ڣ��򷵻�1�����򷵻�0��ע��b����a2ʱҲ����0����Ϊ����������
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
		/*a2.hour==b.hourʱ*/
		if (a2.minute <= b.minute)
			return 0;
		else
			return 1;
	}
	/*a1.hour==b.hourʱ*/
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

/*�������ܣ���ȡָ����·��ָ��ʱ����Ӧ��ӵ���ٷ���
 *��ڲ�����ͼG�����д洢����·��ӵ���ٷ�������·���lineIndex��ʱ���time
 *���ڲ��������͵�ӵ���ٷ������������������-1*/
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

/*�������ܣ�����ӵ���ٷ�������ӵ������
 *��ڲ�����ӵ���ٷ���crow����·���lineIndex
 *���ڲ����������͵�ӵ�����ӣ�ע����ӵ���ٷ�����Ϊ0~100����Ϊ�쳣���������-1*/
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

/*�������ܣ�Ϊʱ��ṹ�Ӽ����ӣ�ע��ͬ��֧�ּ����ܣ�ֻҪaddΪ��������
 *��ڲ�����ʱ���ṹtime�������ͼ���add*/
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

//path���������飬��-1��β
Status Cal_TimeAll(GraphAdjList* G, PathInfo* P, Time_2 time, int cm)
{
	int i = 0;                    /*ѭ������*/
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
				/*���ϵȳ�ʱ��*/
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

int flag = 0;							/* ��־̽Ѱ����0 */
int top = 0;
int Visited[MAX_VERTEX_NUM] = { 0 };    /* ��¼����Ƿ񱻷��� */

/* �������ܣ�DFS����ͼG������Ҫ��ļ�·�������洢��·������P��
 * ��ڲ�����ͼG��·������P�����վ���i���յ�վ���j������ʱ��time����󻻳˴���changeTimes�����ӵ���̶�cm */
void pathdfs(GraphAdjList* G, PathInfo** P, int i, int j, Time_2 time, int changeTimes, int cm)
{
	/* ������i��ջ */
	Visited[i] = visited;							
	P[count]->path[top++] = i;							

	/*��ջ��������������Ԫ��ʱ����ʼ��¼ջ��·����Ӧ����·*/
	if (top > 1)
	{
		/*�õ�������Ӧ�ıߵ�ָ�룬���뵽·���ı���*/
		P[count]->edge[top - 2] = LocateEdge(G, P[count]->path[top - 2], P[count]->path[top - 1]);

		/* �����ظ�������վ-��ɽ�㳡վ�����ļٻ��ˣ����ڽ�������������ظ��ߣ��ʴ˷�����Ч */
		EdgeNode* m = P[count]->edge[top - 2]->next;
		while (m && m->adjvex != P[count]->path[top - 1])	/* Ѱ���Ƿ����ظ��� */
			m = m->next;
		if (m)/* ������ظ��ߣ����ж��߸��ظ����Ƿ����õ�ǰ���ڲ��ҵ�·�������� */
			if (top > 2 && P[count]->edge[top - 3]->lineIndex == m->lineIndex)
				/* ����ܣ���ѡ���������ظ��ı� */
				P[count]->edge[top - 2] = m;

		/* ����¼��·�ߴ���һ��ʱ���ɿ�ʼ�ж��Ƿ���ֻ��� */
		if (top > 2 && P[count]->edge[top - 3]->lineIndex != P[count]->edge[top - 2]->lineIndex)
		{
			P[count]->transfer++;

			/* �����˵Ĵ�������������ƴ���ʱ��������� */
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

	/* ���ڶ���i������j��·������flag=1��������·���洢���� */
	if (!flag && P[count]->path[top - 1] == j)
		flag = 1;
	if (flag)
	{
		/* Ϊ����ı߽縳ֵ���Ա�������б߽��� */
		P[count]->edge[top - 1] = NULL;
		P[count]->path[top - 1] = j;
		P[count]->path[top] = -1;
		P[count]->stationNum = top;

		/* �жϲ��ҵ�����·�Ƿ��ǺϺ�Ҫ�����· */
		/* ������������-1��˵�������ʱ�䳬��������Ӫʱ�䣬���߾����˳������ӵ���ȵ�·�� */
		if (Cal_TimeAll(G, P[count], time, cm) != -1)
		{
			/* ��һ������·����Ʊ�ۺ������ */
			Cal_MoneyLen(G, P[count]);		
			/* ����·����̺��ۺ�����ʱ�䣬�ų�һ����·����·���ظ�ԭ��δ֪��*/
			int k = 0;
			for (;k < count;k++)
				if (P[count]->len == P[k]->len && P[count]->time == P[k]->time)
					break;
			/* ����ǰ·�������ظ�·�������½�����ʼ��·���ṹ�� */
			if (k == count)
			{
				count++;
				P[count] = Init_Path();
				/* Ϊ��һ���ṹ�帴��վ�㡢·���ͻ���״̬ */
				for (int s = 0; s < top && P[count - 1]->path[s] != j; s++)
				{
					P[count]->path[s] = P[count - 1]->path[s];
					if (P[count - 1]->edge[k])
						P[count]->edge[s] = P[count - 1]->edge[s];
				}
				P[count]->transfer = P[count - 1]->transfer;
			}
		}
		/* ����һ��·���ǲ������·������ԭ�������ĸ���������� */
		else
		{
			P[count]->time = 0;
			P[count]->true_time = 0;
			P[count]->crowded = 0;
		}
		flag = 0;/* ��ԭflag״̬ */
	}

	/* �����ڶ���i������j��·��ʱ�����Ҹý����ڽӽ�� */
	else
		for (EdgeNode* p = G->adjList[i].firstedge; p; p = p->next)
		{
			if (Visited[p->adjvex] == unvisited)
				pathdfs(G, P, p->adjvex, j, time, changeTimes, cm);
		}

	/* ������ݣ�������i��ջ */
	/* Visited[i]��Ϊunvisited���´ο�ͨ������iѰ���������ܵ���j��·�� */
	Visited[i] = unvisited;
	top--;

	/* �жϸû���վ���Ƿ��ǻ��˵�վ�㣬����ǣ��򽫻�������һ */
	if (top > 1 && P[count]->edge[top - 2] && P[count]->edge[top - 1])
		if (P[count]->edge[top - 2]->lineIndex != P[count]->edge[top - 1]->lineIndex)
			P[count]->transfer--;
}

/*c����·����*/
int Path_Sort_Weight(PathInfo* P[], int c) {
	int i = 0, j;
	for (;i < 3 && i < c;i++) {
		for (j = i + 1;j < c;j++) {
			/*���ۺ�ָ������ʱ�������Ƽ�ӵ�����ٵ���·*/
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
