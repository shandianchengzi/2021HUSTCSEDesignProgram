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
		EdgeNode* edge[MAX_VERTEX_NUM - 1];     /* 记录路径对应的边的指针 */
		int money;                              /* 该路径的票价 */
		float time;                             /* 该路径的综合评估时间 */
		float true_time;                        /* 该路径的真实花费时间 */
		float len;                              /* 该路径的里程 */
		int transfer;                           /* 换乘次数 */
		int stationNum;                         /* 经过的站点数量 */
		float crowded;                          /* 该路径的平均拥挤程度(0~100) */
	} PathInfo;

	typedef struct
	{
		int hour;
		float minute;
	} Time_2;

	/* 初始化Path数组 */
	PathInfo* Init_Path();
	Status Free_Path(PathInfo** path);
	float TimeTable(Time_2* time);
	int Path_Sort_Weight(PathInfo* P[], int c);
	int Path_Sort_Time(PathInfo* P[], int c);
	int Path_Sort_Money(PathInfo* P[], int c);
	int Path_Sort_Crowded(PathInfo* P[], int c);
	/* 函数功能：求解连通图的简单路径（类dfs) */
	/* 入口参数：连通图G，路径数组P，起点站编号i，终点站编号j，出发时间time，最大换乘次数c，最大拥挤程度cm */
	/* 出口参数：找到的路的数量 */
	int get_All(GraphAdjList* G, PathInfo* P[], int i, int j, Time_2 time, int c, int cm);
	void time_add(Time_2* time, float add);

#ifdef __cplusplus
}
#endif
