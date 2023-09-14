#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TObject
{
    int Price;
    int Weight;
} TO;

typedef struct TStack
{
    int Value;
    struct TStack *Next;
} *TS;

TS Create_stack (void);
void Push (TS *h, int x);
int Pop (TS *h);
bool Is_empty (TS *t);
int Weight (TO *t);
int Price (TO *t);
int Max (int a, int b);
int *Give_memory (int count);
int **Give_memory_for_matrix (int count, int weight);
TO *Give_memory_for_objects (int count);
void Scan_objects (TO *t, int count);
void Generation_matrix (TO *t, int **x, int count, int weight);
void Find_ans (TO *t, int **x, TS *ans, int count, int weight);
void Print_objects (TO *t, TS *ans);
void Free (TO *t, int **x, int count);


int main ()
{
    int weight, count;
    if (scanf("%d %d", &count, &weight) != 2)
    {
        return 0;
    }
    TO *objects = Give_memory_for_objects(count);
    Scan_objects(objects, count);
    int **x = Give_memory_for_matrix(count, weight);
    Generation_matrix(objects, x, count, weight);
    printf("%d\n", x[count][weight]);
    TS ans = Create_stack();
    Find_ans(objects, x, &ans, count, weight);
    Print_objects(objects, &ans);
    Free(objects, x, count);
    return 0;
}

TS Create_stack ()
{
    return NULL;
}

void Push (TS *h, int x)
{
    TS t = (TS) malloc(sizeof (struct TStack));
    t -> Next = *h;
    t -> Value = x;
    *h = t;
}

int Pop (TS *h)
{
    TS p = *h;
    int x = (*h) -> Value;
    *h = (*h) -> Next;
    free(p);
    return x;
}

bool Is_empty (TS *t)
{
    return (*t == NULL);
}

int **Give_memory_for_matrix (int count, int weight)
{
    int **x = (int **) malloc((count + 1) * sizeof (int *));
    for (int i = 0; i < count + 1; i++)
    {
        x[i] = Give_memory(weight + 1);
    }
    return x;
}

int Weight (TO *t)
{
    return t -> Weight;
}

int Price (TO *t)
{
    return t -> Price;
}

TO *Give_memory_for_objects (int count)
{
    TO *x = (TO *) malloc(count * sizeof (TO));
    return x;
}

int *Give_memory (int count)
{
    int *x = malloc(count * sizeof (int));
    return x;
}

void Scan_objects (TO *t, int count)
{
    for (int i = 0; i < count; i++)
    {
        int weight, price;
        if (scanf("%d %d", &weight, &price) != 2)
        {
            return;
        }
        t[i].Weight = weight;
        t[i].Price = price;
    }
}

int Max (int a, int b)
{
    return a * (a > b) + b * (a <= b);
}

void Generation_matrix (TO *t, int **x, int count, int weight)
{
    for (int i = 0; i < count + 1; i++)
    {
        x[i][0] = 0;
    }
    for (int i = 0; i < weight + 1; i++)
    {
        x[0][i] = 0;
    }
    for (int i = 1; i < count + 1; i++)
    {
        for (int j = 1; j < weight + 1; j++)
        {
            if (j >= t[i - 1].Weight)
            {
                x[i][j] = Max(x[i - 1][j], x[i - 1][j - t[i - 1].Weight] + t[i - 1].Price);
            }
            else
            {
                x[i][j] = x[i - 1][j];
            }
        }
    }
}

void Find_ans (TO *t, int **x, TS *ans, int count, int weight)
{
    if (!x[count][weight])
    {
        return;
    }
    if (x[count - 1][weight] == x[count][weight])
    {
        Find_ans(t, x, ans, count - 1, weight);
    }
    else
    {
        Find_ans(t, x,  ans, count - 1, weight - Weight(&t[count - 1]));
        Push(ans, count);
    }
}

void Print_objects (TO *t, TS *ans)
{
    if (Is_empty(ans))
    {
        return;
    }
    else
    {
        int x = Pop(ans);
        Print_objects(t, ans);
        printf("%d %d\n", Weight(&t[x - 1]), Price(&t[x - 1]));
    }
}

void Free (TO *t, int **x, int count)
{
    free(t);
    for (int i = 0; i < count + 1; i++)
    {
        free(x[i]);
    }
    free(x);
}