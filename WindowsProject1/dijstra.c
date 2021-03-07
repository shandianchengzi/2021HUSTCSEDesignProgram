#include"head.h"
#include"path.h"
/* ÿ��ѡ��һ��Ϊ��������s[]�ľ�����СȨֵ�Ľ�� */
int Choose(float *d, int *s,int n)
{
    int i,minpos;
    float min=FLT_MAX;                              /* ��min��ʼ��Ϊfloat�����ֵ */
    minpos=-1;
    for(i=0; i<n; i++)
    {
        if(d[i]<min&&!s[i])
        {
            min=d[i];
            minpos=i;
        }
    }
    return minpos;
}
//Dijstra�㷨��·��
Status Dijkstra(int v,float *d,int *path,GraphAdjList *G)
{
    int num=G->numVertexes;
    if(v<0||v>num-1)
    {
        return ERROR;
    }
    int i,k,s[num];
    EdgeNode *p=G->adjList[v].firstedge;              /* ����ָ�� */
    for(i=0; i<num ; i++)                   /* ��ʼ��s,d,path���� */
    {
        s[i]=0;
        path[i]=-1;
        d[i]=FLT_MAX;
    }
    while(p)
    {
        d[p->adjvex]=p->distance;
        if(p->adjvex!=v&&d[p->adjvex]<FLT_MAX)
        {
            path[p->adjvex]=v;
        }
        p=p->next;
    }
    s[v]=1;
    d[v]=0;

    for(i=1; i<num ; i++)
    {
        k=Choose(d,s,num);                  /* ��ȡd��s��ӦȨ����С�Ľ������ */
        if(k==-1)                           /* �ж��Ƿ�ѡ������Ч��� */
        {
            continue;
        }
        s[k]=1;
        p=G->adjList[k].firstedge;
        if(p==NULL)
        {
            continue ;
        }
        while(p)
        {
            if(!s[p->adjvex]&&d[k]+p->distance<d[p->adjvex])//����d��path
            {
                d[p->adjvex]=d[k]+p->distance;
                path[p->adjvex]=k;
            }
            p=p->next;
        }
    }
    return OK;
}
Status get_shortPath(GraphAdjList *G,int* pathAll,int v,int u)
{
    if(u==v)
    {
        printf("��ʼվ������ֹվ�㲻Ӧ����ͬ��~\n");
        return 1;
    }
    int path[G->numVertexes];
    float d[G->numVertexes];
    Dijkstra(u,d,path,G);
    if(path[v]==-1)
    {
        return -1;
    }
    int pathNum=0;
    while (path[v]!=-1)
    {
        pathAll[pathNum++]=v;
        v=path[v];
    }
    pathAll[pathNum++]=u;
    pathAll[pathNum]=-1;
    return 0;
}
