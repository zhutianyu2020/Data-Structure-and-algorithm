#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#define INFINITY 32767
#define MAX_VEX 20
#define QUEUE_SIZE (MAX_VERTEX+1)
#define DataType char  /* vertext's info  */
int* visited; /* Node: visited flag with dynamic array, good idea ! */
/* init queue for bfs */
struct _node                         //边节点
{
    int v_num;                      //该边所指向节点的位置 
    struct _node* next;            //指向下一条边的指针
};
typedef struct _node node, * pnode;
struct _queue                           //队列
{
    pnode front; 
    pnode rear;
};
typedef struct _queue queue, * pqueue;

struct _graph
{
    DataType* vexs;                 //顶点表
    int arcs[MAX_VEX][MAX_VEX];     //邻接矩阵
    int vexnum, arcnum;
};
typedef struct _graph graph, * pgraph;

/* operation of queue */            //队列操作
queue init_queue()      //创建队列                 
{
    queue qu;
    qu.front = qu.rear = (pnode)malloc(sizeof(node));
    if (qu.front == NULL)
        exit(1);
    qu.rear->next = NULL;
    return qu;
}
int en_queue(pqueue pqu, int v_num)
{
    pnode pn;
    pn = (pnode)malloc(sizeof(node));
    if (pqu->front == NULL)
        exit(1);
    pn->v_num = v_num;
    pn->next = NULL;
    pqu->rear->next = pn;
    pqu->rear = pqu->rear->next;
}
int isempty_queue(pqueue pqu)     //判断队列是否为空
{
    if (pqu->front == pqu->rear)
        return 1;
    else
        return 0;
}
int de_queue(pqueue pqu)
{
    pnode pn;
    int d;
    if (isempty_queue(pqu))
        return -1;
    pn = pqu->front;
    d = pn->v_num;
    pqu->front = pn->next;
    free(pn);
    return d;
}
int locate(graph g, DataType data)                //确定顶点在图中的位置
{
    int i;
    for (i = 0; i < g.vexnum; i++)
        if (g.vexs[i] == data)
            return i;
    return -1;
}
graph create_graph()          //创建无项网
{
    int i, j, w, s1, s2;
    DataType ch1, ch2, tmp;
    graph g;
    printf("请输入总顶点数和总边数:");
    scanf("%d %d", &g.vexnum, &g.arcnum);
    tmp = getchar();
    g.vexs = (DataType*)malloc(sizeof(DataType));
    if (g.vexs == NULL)
        exit(1);
    printf("请输入%d个顶点的值\n", g.vexnum);
    for (i = 0; i < g.vexnum; i++)
    {
        printf("第%d个顶点:", i);
        scanf("%c", &g.vexs[i]);
        tmp = getchar();
        //visited[i]=0;
    }
    for (i = 0; i < g.vexnum; i++)
        for (j = 0; j < g.vexnum; j++)
            g.arcs[i][j] = INFINITY;
    for (i = 0; i < g.arcnum; i++)
    {   
        printf("请输入第%d条边依附的两个顶点及权值:\n", i);
        scanf("%c %c %d", &ch1, &ch2, &w);
        tmp = getchar();
        s1 = locate(g, ch1);        //确定ch1顶点在图中的位置
        s2 = locate(g, ch2);        //确定ch2顶点在图中的位置
        g.arcs[s1][s2] = g.arcs[s2][s1] = w; //置<ch1,ch2>的对称弧<ch2,ch1>
    }
    return g;
}
int firstvex_graph(graph g, int k)//返回顶点k的第一个邻接顶点的索引，失败则返回-1
{
    int i;
    if (k >= 0 && k < g.vexnum)
        for (i = 0; i < g.vexnum; i++)
            if (g.arcs[k][i] != INFINITY)
                return i;
    return -1;
}
int nextvex_graph(graph g, int i, int j)//返回顶点i相对与j的下一个邻接顶点的索引，失败则返回-1
{
    int k;
    if (i >= 0 && i < g.vexnum && j >= 0 && j < g.vexnum) {
        for (k = j + 1; k < g.vexnum; k++)
            if (g.arcs[i][k] != INFINITY)
                return k;
      }
    return -1;
}
 void dfs(graph g, int k)  //深度优先搜素
{
    int i;
    if (k == -1)
    {
        for (i = 0; i < g.vexnum; i++)
            if (!visited[i])
                dfs(g, i);
    }
    else
    {
        visited[k] = 1;
        printf("%c ", g.vexs[k]);
        for (i = firstvex_graph(g, k); i >= 0; i = nextvex_graph(g, k, i))
            if (!visited[i])
                dfs(g, i);
    }
}
void bfs(graph g)       //广度优先搜索
{
    int i, j, k;
    queue qu;
    qu = init_queue();
    for (i = 0; i < g.vexnum; i++)
        if (!visited[i])
        {
            visited[i] = 1;
            printf("%c ", g.vexs[i]);
            en_queue(&qu, i);
            while (!isempty_queue(&qu))
            {
                k = de_queue(&qu);
                for (j = firstvex_graph(g, k); j >= 0; j = nextvex_graph(g, k, j))
                    if (!visited[j])
                    {
                        visited[j] = 1;
                        printf("%c ", g.vexs[j]);
                        en_queue(&qu, j);
                    }
            }
        }
}

int main ()
{
    int i = 0;
    graph g;
    while (1) {
    printf("\t菜单\n");
    printf("1.邻接矩阵法创建无向网\n");
    printf("2.深度优先搜索\n");
    printf("3.广度优先搜索\n");
    printf("4.退出程序\n\n");
    printf("请选择:");
    int choose=0;
    scanf(" %d", &choose);
    switch (choose)
    {
        case 1:
            g = create_graph();
            printf("创建成功\n");
            visited = (int*)malloc(g.vexnum * sizeof(int));
            break;
        case 2:
            for (i = 0; i < g.vexnum; i++)
                visited[i] = 0;
            printf("深度优先搜索结果:\n");
            dfs(g,-1);
            printf("\n\n");
            break;
        case 3:
            for (i = 0; i < g.vexnum; i++)
                visited[i] = 0;
            printf("广度优先搜索结果:\n");
            bfs(g);
            printf("\n\n");
            break;
        case 4:
            exit(0);
            break;
    default:
        break;
    }
    }
}

/*int main()
{
   // int i;
    //graph g;
   // visited = (int*)malloc(g.vexnum * sizeof(int));

    
    printf("/n/n dfs:");

    dfs(g, -1);
    for (i = 0; i < g.vexnum; i++)
        visited[i] = 0;
    printf("/n bfs:");
    bfs(g);

    if (visited)
        free(visited);
    printf("/n");

    system("PAUSE");
}*/
