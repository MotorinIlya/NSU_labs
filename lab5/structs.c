#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Item{
    int Size;
    int Weight;
    struct texture;
    struct model;
};




typedef struct T_array
{
    int *data;
    int size;
} T_array;

struct TElements{
    char Value;
    struct TElements* Next;
};

typedef struct TElements* TS;

T_array Create (int s)
{
    int *p = malloc(s * sizeof(int));
    assert(p != NULL);
    T_array a;
    a.data = p;
    a.size = s;
    return a;
}

int Get_size (T_array a)
{
    return a.size;
}

void Destroy (T_array *a)
{
    free((*a).data);
    *a = (T_array){NULL, 0};
}

void Set (T_array *a, int i, int x)
{
    assert(i < (*a).size);
    (*a).data[i] = x;
}

TS CreateTS ()
{
    TS s;
    return NULL;
}

bool IsEmpty (const TS* s)
{
    return (*s == NULL);
}

char Pop (TS* s)
{
    TS t = (*s) -> Next;
    char res = (*s) -> Value;
    *s = t;
    return res;
}

void DestroyTS (TS* s)
{
    while (!IsEmpty(s))
    {
        Pop(s);
    }
}

void Push (TS* s, char x)
{
    TS t = malloc(sizeof(**s));
    t -> Value = x;
    t -> Next = *s;
    *s = t;
}

typedef struct T_tree{
    int Value;
    struct T_tree *Left;
    struct T_tree *Right;
}*TT;

TT Make_leaf (int x)
{
    TT l = malloc(sizeof(TT));
    l -> Value = x;
    l -> Left = NULL;
    l -> Right = NULL;
    return l;
}

TT Make_tree (TT l, TT r, int x)
{
    TT t = malloc(sizeof(TT));
    t -> Left = l;
    t -> Right = r;
    t -> Value = x;
    return t;
}



int main ()
{
    return 0;
}

