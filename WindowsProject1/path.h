#include"graph.h"
#define Time_Go 2
#define Time_Stop 1
#define Time_Walk 3.5
#define k_relax 0.6
#define k_crowded 1.5
#define Time_Begin 6
#define Time_End 23
#ifdef __cplusplus
extern "C" {
#endif
	typedef struct
	{
		int path[MAX_VERTEX_NUM];
		EdgeNode* edge[MAX_VERTEX_NUM - 1];     /* ��¼·����Ӧ�ıߵ�ָ�� */
		int money;                              /* ��·����Ʊ�� */
		float time;                             /* ��·�����ۺ�����ʱ�� */
		float true_time;                        /* ��·������ʵ����ʱ�� */
		float len;                              /* ��·������� */
		int transfer;                           /* ���˴��� */
		int stationNum;                         /* ������վ������ */
		float crowded;                          /* ��·����ƽ��ӵ���̶�(0~100) */
	} PathInfo;

	typedef struct
	{
		int hour;
		float minute;
	} Time_2;

	/* ��ʼ��Path���� */
	PathInfo* Init_Path();
	Status Free_Path(PathInfo** path);
	float TimeTable(Time_2* time);
	int Path_Sort_Weight(PathInfo* P[], int c);
	int Path_Sort_Time(PathInfo* P[], int c);
	int Path_Sort_Money(PathInfo* P[], int c);
	int Path_Sort_Crowded(PathInfo* P[], int c);
	/* �������ܣ������ͨͼ�ļ�·������dfs) */
	/* ��ڲ�������ͨͼG��·������P�����վ���i���յ�վ���j������ʱ��time����󻻳˴���c�����ӵ���̶�cm */
	/* ���ڲ������ҵ���·������ */
	int get_All(GraphAdjList* G, PathInfo* P[], int i, int j, Time_2 time, int c, int cm);
	void time_add(Time_2* time, float add);

#ifdef __cplusplus
}
#endif
