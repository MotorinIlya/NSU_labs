#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct TAVLTree
{
    int Value;
    char Height;
    struct TAVLTree *Right;
    struct TAVLTree *Left;
} *TAVL;

int Height (TAVL t);
TAVL Give_memory (int count);
void Hitch (TAVL h, TAVL *t, int c, int ind);
TAVL Create_TAVL (TAVL t, int count);
int Max (int a, int b);
void Fix_height (TAVL *t);
int Balance_factor (TAVL *t);
TAVL Rotate_right (TAVL *t);
TAVL Rotate_left (TAVL *t);
void Balance_AVL (TAVL *t);
void Print_height (TAVL *t);
void Clean_pointers (TAVL *h, int count);

int main ()
{
    int count;
    if (scanf("%d", &count) == 0)
    {
        return 0;
    }
    TAVL h = Give_memory(count);
    TAVL t = Create_TAVL(h, count);
    Print_height(&t);
    Clean_pointers(&h, count);
    free(h);
    return 0;
}

int Height (TAVL t)
{
    return (t == NULL) ? 0 : (int) t -> Height;
}

TAVL Give_memory (int count)
{
    TAVL h = (TAVL) malloc(count * sizeof (struct TAVLTree));
    assert(h != NULL);
    for (int i = 0; i < count; i++)
    {
        h[i].Right = NULL;
        h[i].Left = NULL;
        h[i].Value = 0;
    }
    return h;
}

void Hitch (TAVL h, TAVL *t, int c, int ind)
{
    if (*t == NULL)
    {
        *t = h;
        (*t) -> Height = 1;
    }
    else if ((*t) -> Value > c)
    {
        Hitch(h, &((*t) -> Left), c, ind);
        (*t) -> Height = (char) (Max(Height((*t) -> Left), Height((*t) -> Right)) + 1);
        Balance_AVL(t);
    }
    else
    {
        Hitch(h, &((*t) -> Right), c, ind);
        (*t) -> Height = (char) (Max(Height((*t) -> Left), Height((*t) -> Right)) + 1);
        Balance_AVL(t);
    }
}

void Fix_height (TAVL *t)
{
    int lh = Height( (*t) -> Left);
    int rh = Height( (*t) -> Right);
    (*t) -> Height = (char) (Max(lh, rh) + 1);
}

int Balance_factor (TAVL *t)
{
    int lh = Height( (*t) -> Left);
    int rh = Height( (*t) -> Right);
    return rh - lh;
}

TAVL Rotate_right (TAVL *t)
{
    TAVL h = (*t) -> Left;
    (*t) -> Left  = h -> Right;
    h -> Right = (*t);
    Fix_height(t);
    Fix_height(&h);
    return h;
}

TAVL Rotate_left (TAVL *t)
{
    TAVL h = (*t) -> Right;
    (*t) -> Right = h -> Left;
    h -> Left = (*t);
    Fix_height(t);
    Fix_height(&h);
    return h;
}

void Balance_AVL (TAVL *t)
{
    if (Balance_factor(t) >= 2)
    {
        TAVL h = (*t) -> Right;
        if (Balance_factor(&h) < 0)
        {
            (*t) -> Right = Rotate_right(&h);
        }
        *t = Rotate_left(t);
    }
    else if (Balance_factor(t) <= -2)
    {
        TAVL h = (*t) -> Left;
        if (Balance_factor(&h) > 0)
        {
            (*t) -> Left = Rotate_left(&h);
        }
        *t = Rotate_right(t);
    }
}

TAVL Create_TAVL (TAVL t, int count)
{
    TAVL h = NULL;
    for (int i = 0; i < count; i++)
    {
        int c;
        if (scanf("%d", &c) == 0)
        {
            return NULL;
        }
        t[i].Value = c;
        Hitch(&t[i], &h, c, i);
    }
    return h;
}

int Max (int a, int b)
{
    return (a > b) ? a : b;
}

void Print_height (TAVL *t)
{
    printf("%d", Height(*t));
}

void Clean_pointers (TAVL *h, int count)
{
    for (int i = 0; i < count; i++)
    {
        (*h) -> Right = NULL;
        (*h) -> Left = NULL;
    }
}
