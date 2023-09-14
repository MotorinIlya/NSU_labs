#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#define MAX_TOP 2000

enum colour {
    white = 1,
    gray = 2,
    black = 3
};

enum error {
    good = 0,
    bad = -1
};

unsigned char Get_bit (unsigned char c, int position);
unsigned char Change_bit (unsigned char c, int position);
int* Create_sort (int cnt_top);
void Add_top (int* sort, int a, int index);
enum colour* Create_graph_colour (int cnt_top);
unsigned char** Create_graph (void);
void Scan_top (int* cnt_top, int* cnt_ways, enum error* status);
void Scan_graph (unsigned char** graph, int cnt_top, int cnt_ways, enum error* status);
bool Top_sort (unsigned char** graph, enum colour* graph_colour, int* sort, int cnt_top, int now, int* index);
void Verify (int cnt_top, int cnt_ways, enum error* status);
void Print_sort (int* sort, int cnt_top);
void Free(unsigned char** graph, int* sort, enum colour* graph_colour);

int main ()
{
    int cnt_top = 0;
    int cnt_ways = 0;
    enum error status = good;
    Scan_top(&cnt_top, &cnt_ways, &status);
    Verify(cnt_top, cnt_ways, &status);
    int *sort = Create_sort(cnt_top);
    enum colour *vertex_colour = Create_graph_colour(cnt_top);
    unsigned char **graph = Create_graph();
    if (status == good)
    {
        Scan_graph(graph, cnt_top, cnt_ways, &status);
    }
    if (status == good)
    {
        int index_sort = 0;
        for (int i = 0; i < cnt_top; i++)
        {
            if (!Top_sort(graph, vertex_colour, sort, cnt_top, i, &index_sort))
            {
                printf("impossible to sort");
                status = bad;
                break;
            }
        }
    }
    if (status == good)
    {
        Print_sort(sort, cnt_top);
    }
    Free(graph, sort, vertex_colour);
    return 0;
}

unsigned char Get_bit (unsigned char c, const int position)
{
    c <<= position;
    c &= 0x80;
    return (c == 128);
}

unsigned char Change_bit (unsigned char c, const int position)
{
    c = c | (int)(1 << (7 - position));
    return c;
}

int* Create_sort (const int cnt_top)
{
    int* a = malloc(cnt_top * sizeof (int));
    assert(a != NULL);
    return a;
}

void Add_top (int* sort, const int a, const int index)
{
    sort[index] = a;
}

enum colour* Create_graph_colour (const int cnt_top)
{
    enum colour* graph_colour = calloc(cnt_top, sizeof (int));
    assert(graph_colour != NULL);
    return graph_colour;
}

unsigned char** Create_graph (void)
{
    unsigned char** graph = (unsigned char**) malloc(MAX_TOP * sizeof (unsigned char*));
    assert(graph != NULL);
    for (int i = 0; i < MAX_TOP; ++i)
    {
        graph[i] = (unsigned char*) calloc(MAX_TOP/8, sizeof (unsigned char));
        assert(graph[i] != NULL);
    }
    return graph;
}

void Scan_top (int* cnt_top, int* cnt_ways, enum error* status)
{
    if (scanf("%d\n%d\n", cnt_top, cnt_ways) != 2)
    {
        printf("bad number of lines");
        *status = bad;
        return;
    }
}

void Scan_graph (unsigned char** graph, const int cnt_top, const int cnt_ways, enum error* status)
{
    for (int i = 0; i < cnt_ways; ++i)
    {
        int from, where;
        if (scanf("%d %d\n", &from, &where) != 2)
        {
            printf("bad number of lines");
            *status = bad;
            return;
        }
        if ((from <= 0 || from > cnt_top) || (where <= 0 || where > cnt_top))
        {
            printf("bad vertex");
            *status = bad;
            return;
        }
        graph[from - 1][(where - 1) / 8] = Change_bit(graph[from - 1][(where - 1) / 8], (where - 1) % 8);
    }
}

bool Top_sort (unsigned char** graph, enum colour *vertex_colour, int* sort, const int cnt_top, const int now, int* index)
{
    if (vertex_colour[now] == black)
    {
        return true;
    }
    vertex_colour[now] = gray;
    for (int i = 0; i < MAX_TOP; ++i)
    {
        if (Get_bit(graph[now][i/8], i % 8))
        {
            if (vertex_colour[i] == gray)
            {
                return false;
            }
            if (!Top_sort(graph, vertex_colour, sort, cnt_top, i, index))
            {
                return false;
            }
        }
    }
    Add_top(sort, now, *index);
    *index += 1;
    vertex_colour[now] = black;
    return true;
}

void Verify (const int cnt_top, const int cnt_ways, enum error* status)
{
    if (cnt_top < 0 || cnt_top > 2000)
    {
        if (*status == 0)
        {
            *status = bad;
            printf("bad number of vertices");
        }
    }
    else if (cnt_ways < 0 || cnt_ways > (cnt_top * (cnt_top + 1) / 2))
    {
        if (*status == 0)
        {
            *status = bad;
            printf("bad number of edges");
        }
    }
}

void Print_sort (int* sort, const int cnt_top)
{
    for (int i = cnt_top - 1; i >= 0; i--)
    {
        printf("%d ", sort[i] + 1);
    }
}

void Free(unsigned char** graph, int* sort, enum colour* graph_colour)
{
    free(sort);
    free(graph_colour);
    for (int i = 0; i < MAX_TOP; i++)
    {
        free(graph[i]);
    }
    free(graph);
}
