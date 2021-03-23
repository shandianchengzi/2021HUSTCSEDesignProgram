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
	e = (EdgeNode*)malloc(sizeof(EdgeNode));  /* ���ڴ�����ռ䣬���ɱ߱��� */
	if (!e)
		exit(1);
	e->adjvex = j;                            /* �ڽ����Ϊj */
	e->distance = distance;
	e->next = G->adjList[i].firstedge;        /* ��eָ��ǰ���ĵ�һ���� */
	e->lineIndex = line;
	G->adjList[i].firstedge = e;              /* ��e��Ϊ��ǰ���ĵ�һ���� */

	e = (EdgeNode*)malloc(sizeof(EdgeNode));  /* ���ڴ�����ռ䣬���ɱ߱��� */
	if (!e)
		exit(1);
	e->adjvex = i;                            /* �ڽ����Ϊi */
	e->distance = distance;
	e->next = G->adjList[j].firstedge;        /* ��eָ��ǰ���ĵ�һ���� */
	e->lineIndex = line;
	G->adjList[j].firstedge = e;              /* ��e��Ϊ��ǰ���ĵ�һ���� */

	return 0;
}

Status CreateALGraph(GraphAdjList* G, FILE* fp)
{
	read_Data(fp, &(G->stations), G->lineDetail);

	/* ��hash���ж�ȡ������Ϣ����������� */
	hash_struct* s;
	for (s = G->stations; s != NULL; s = s->hh.next)            
	{
		strcpy_s(G->adjList[G->numVertexes].data, 20, s->name); /* ���붥����Ϣ */
		G->adjList[G->numVertexes].firstedge = NULL;			/* ���߱���Ϊ�ձ� */
		G->numVertexes++;
	}

	/* ����·��Ϣ�н����߱� */
	for (int i = 0; i < linenum; i++)                               
	{
		for (int j = 1; j < G->lineDetail[i].lineCount; j++)
		{
			if (InsertEdge(G, LocateVex(G, G->lineDetail[i].lineStations[j]), LocateVex(G, G->lineDetail[i].lineStations[j + 1]), G->lineDetail[i].lineAverage, i))
				return -1;										/* ������ʧ�ܣ�����-1 */
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
