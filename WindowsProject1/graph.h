#include"readData.h"
#define MAX_VERTEX_NUM 240          /* ��󶥵��� */
#define visited 1
#define unvisited 0
#ifdef __cplusplus
extern "C" {
#endif
	typedef char* VertexType;           /* �������� */
	typedef float EdgeType;             /* ��Ȩ���� */

	typedef struct EdgeNode     /* �߱��� */
	{
		int adjvex;				/* �ڽӵ��򣬴洢�ý���Ӧ���±� */
		float distance;			/* ���ڴ洢�ߵĳ��� */
		int lineIndex;			/* ��ʶ�ñ�����������·��ע������·��Ŷ�����·���� */
		struct EdgeNode* next;  /* ����ָ����һ���ڽӵ� */
	} EdgeNode;

	typedef struct VertexNode   /* ������� */
	{
		char data[20];          /* �����򣬴洢�������� */
		EdgeNode* firstedge;	/* �߱�ͷָ�� */
	} VertexNode;

	typedef struct
	{
		VertexNode adjList[MAX_VERTEX_NUM];/* ����� */
		int numVertexes, numEdges;      /* ͼ�е�ǰ�������ͱ��� */
		hash_struct* stations;			/* �������ݽṹ-Hash�� */
		lineInfo* lineDetail;			/* �������ݽṹ-��·�ṹ */
	} GraphAdjList;

	/* ������������ */
	/* ��ʼ������ͼָ�� */
	GraphAdjList* Init_Graph();
	/* ��λ��㣬���ؽ�����;���޸ý�㣬����-1 */
	int LocateVex(GraphAdjList* G, VertexType Vex);
	/* ��λ�ߣ����ر�ָ��;���޸ñߣ�����0 */
	EdgeNode* LocateEdge(GraphAdjList* G, int ivex, int jvex);
	/* ����hash�������·��Ϣ����ͼ���ڽӱ�ṹ */
	Status CreateALGraph(GraphAdjList* G, FILE* fp);
	/* �ͷŽ���Graphʱ������ڴ� */
	Status Free_Graph(GraphAdjList** G);
#ifdef __cplusplus
}
#endif
