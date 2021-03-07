#include"head.h"
#include"path.h"
/* 每次选择一个为加入数组s[]的具有最小权值的结点 */
int Choose(float *d, int *s,int n)
{
    int i,minpos;
    float min=FLT_MAX;                              /* 将min初始化为float的最大值 */
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
//Dijstra算法求路径
Status Dijkstra(int v,float *d,int *path,GraphAdjList *G)
{
    int num=G->numVertexes;
    if(v<0||v>num-1)
    {
        return ERROR;
    }
    int i,k,s[num];
    EdgeNode *p=G->adjList[v].firstedge;              /* 工作指针 */
    for(i=0; i<num ; i++)                   /* 初始化s,d,path数组 */
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
        k=Choose(d,s,num);                  /* 获取d，s对应权重最小的结点的序号 */
        if(k==-1)                           /* 判断是否选择了有效结点 */
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
            if(!s[p->adjvex]&&d[k]+p->distance<d[p->adjvex])//更新d和path
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
        printf("起始站点与终止站点不应该相同呦~\n");
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
