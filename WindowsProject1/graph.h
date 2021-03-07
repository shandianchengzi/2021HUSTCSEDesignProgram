#include"readData.h"
#define MAX_VERTEX_NUM 240          /* 最大顶点数 */
#define visited 1
#define unvisited 0
#ifdef __cplusplus
extern "C" {
#endif
typedef char* VertexType;           /* 顶点类型 */
typedef float EdgeType;             /* 边权类型 */

typedef struct EdgeNode             /* 边表结点 */
{
    int adjvex;                     /* 邻接点域，存储该结点对应的下标 */
    EdgeType distance;              /* 用于存储边的长度 */
    struct EdgeNode *next;          /* 链域，指向下一个邻接点 */
    int lineIndex;                  /* 标识该边属于哪条线路，注：是线路序号而非线路名字 */
} EdgeNode;

typedef struct VertexNode           /* 顶点表结点 */
{
    char data[20];                  /* 顶点域，存储顶点信息 */
    EdgeNode *firstedge;            /* 边表头指针 */
    int index;                      /* 顶点编号 */
} VertexNode;

typedef struct
{
    VertexNode adjList[MAX_VERTEX_NUM];
    int Visited[MAX_VERTEX_NUM];    /* 记录结点是否被访问 */
    int numVertexes,numEdges;       /* 图中当前顶点数和边数 */
    hash_struct *stations;
    lineInfo* lineDetail;
} GraphAdjList;

/* 基本函数定义 */
/* 初始化定义图指针 */
GraphAdjList* Init_Graph();
/* 定位结点，返回结点序号;若无该结点，返回-1 */
int LocateVex(GraphAdjList *G,VertexType Vex);
/* 定位边，返回边指针;若无该边，返回0 */
EdgeNode *LocateEdge(GraphAdjList *G,int ivex,int jvex);
/* 依据hash结点表和线路信息建立图的邻接表结构 */
Status CreateALGraph(GraphAdjList *G,FILE *fp);

Status Free_Graph(GraphAdjList **G);
#ifdef __cplusplus
}
#endif
