#include"readData.h"
#define MAX_VERTEX_NUM 240          /* 最大顶点数 */
#define visited 1
#define unvisited 0
#ifdef __cplusplus
extern "C" {
#endif
	typedef char* VertexType;           /* 顶点类型 */
	typedef float EdgeType;             /* 边权类型 */

	typedef struct EdgeNode     /* 边表结点 */
	{
		int adjvex;				/* 邻接点域，存储该结点对应的下标 */
		float distance;			/* 用于存储边的长度 */
		int lineIndex;			/* 标识该边属于哪条线路，注：是线路序号而非线路名字 */
		struct EdgeNode* next;  /* 链域，指向下一个邻接点 */
	} EdgeNode;

	typedef struct VertexNode   /* 顶点表结点 */
	{
		char data[20];          /* 顶点域，存储顶点名称 */
		EdgeNode* firstedge;	/* 边表头指针 */
	} VertexNode;

	typedef struct
	{
		VertexNode adjList[MAX_VERTEX_NUM];/* 顶点表 */
		int numVertexes, numEdges;      /* 图中当前顶点数和边数 */
		hash_struct* stations;			/* 辅助数据结构-Hash表 */
		lineInfo* lineDetail;			/* 辅助数据结构-线路结构 */
	} GraphAdjList;

	/* 基本函数定义 */
	/* 初始化定义图指针 */
	GraphAdjList* Init_Graph();
	/* 定位结点，返回结点序号;若无该结点，返回-1 */
	int LocateVex(GraphAdjList* G, VertexType Vex);
	/* 定位边，返回边指针;若无该边，返回0 */
	EdgeNode* LocateEdge(GraphAdjList* G, int ivex, int jvex);
	/* 依据hash结点表和线路信息建立图的邻接表结构 */
	Status CreateALGraph(GraphAdjList* G, FILE* fp);
	/* 释放建立Graph时分配的内存 */
	Status Free_Graph(GraphAdjList** G);
#ifdef __cplusplus
}
#endif
