#include"head.h"
#include"graph.h"

GraphAdjList* Init_Graph()
{
	GraphAdjList* g;
	g = (GraphAdjList*)malloc(sizeof(GraphAdjList));
	if (!g)
		exit(1);
	g->numVertexes = 0;
	g->numEdges = 0;
	g->stations = NULL;
	g->lineDetail = Init_lineInfo();
	return g;
}

int LocateVex(GraphAdjList* G, VertexType Vex)
{
	hash_struct* s;
	s = find_station(G->stations, Vex);
	if (s)
		return s->data.index;
	return -1;
}

EdgeNode* LocateEdge(GraphAdjList* G, int ivex, int jvex)
{
	EdgeNode* s = G->adjList[ivex].firstedge;
	while (s && s->adjvex != jvex)
	{
		s = s->next;
	}
	if (s)
	{
		return s;
	}
	return 0;
}

Status InsertEdge(GraphAdjList* G, int i, int j, float distance, int line)
{
	EdgeNode* e;
	e = (EdgeNode*)malloc(sizeof(EdgeNode));  /* 向内存申请空间，生成边表结点 */
	if (!e)
		exit(1);
	e->adjvex = j;                            /* 邻接序号为j */
	e->distance = distance;
	e->next = G->adjList[i].firstedge;        /* 令e指向当前结点的第一条边 */
	e->lineIndex = line;
	G->adjList[i].firstedge = e;              /* 令e成为当前结点的第一条边 */

	e = (EdgeNode*)malloc(sizeof(EdgeNode));  /* 向内存申请空间，生成边表结点 */
	if (!e)
		exit(1);
	e->adjvex = i;                            /* 邻接序号为i */
	e->distance = distance;
	e->next = G->adjList[j].firstedge;        /* 令e指向当前结点的第一条边 */
	e->lineIndex = line;
	G->adjList[j].firstedge = e;              /* 令e成为当前结点的第一条边 */

	return 0;
}

Status CreateALGraph(GraphAdjList* G, FILE* fp)
{
	read_Data(fp, &(G->stations), G->lineDetail);

	/* 从hash表中读取顶点信息，建立顶点表 */
	hash_struct* s;
	for (s = G->stations; s != NULL; s = s->hh.next)            
	{
		strcpy_s(G->adjList[G->numVertexes].data, 20, s->name); /* 读入顶点信息 */
		G->adjList[G->numVertexes].firstedge = NULL;			/* 将边表置为空表 */
		G->numVertexes++;
	}

	/* 从线路信息中建立边表 */
	for (int i = 0; i < linenum; i++)                               
	{
		for (int j = 1; j < G->lineDetail[i].lineCount; j++)
		{
			if (InsertEdge(G, LocateVex(G, G->lineDetail[i].lineStations[j]), LocateVex(G, G->lineDetail[i].lineStations[j + 1]), G->lineDetail[i].lineAverage, i))
				return -1;										/* 若插入失败，返回-1 */
			G->numEdges++;
		}
	}
	return 0;
}

Status Free_Graph(GraphAdjList** G)
{
	free((*G)->lineDetail);
	hash_struct* s = (*G)->stations;
	while (s != NULL)
	{
		hash_struct* s2 = s->hh.next;
		delete_station(&((*G)->stations), s);
		s = s2;
	}
	free(*G);
	*G = NULL;
	return 0;
}
