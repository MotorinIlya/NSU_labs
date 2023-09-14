#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

enum colour{
    red = 1,
    black = 0
};

typedef struct RBTree
{
    int Value;
    enum colour Colour;
    struct RBTree *Right;
    struct RBTree *Left;
}* RB;


RB Give_memory (int count);
bool Is_leaf (RB h);
void Find_father (RB *t, RB *f, int x);
void Find_father_and_grandfather (RB *t, RB *f, RB *g, int x);
RB Rotate_right (RB *t);
RB Rotate_left (RB *t);
void Rotate_right_with_paint (RB *father, RB *grandfather);
void Rotate_left_with_paint (RB *father, RB *grandfather);
void Repaint (RB *grandfather, RB *father, RB *uncle);
void Hitch_father (RB *ggf, RB *father);
void Is_root_for_right (RB *t, RB *father, RB *grandfather);
void Is_root_for_left (RB *t, RB *father, RB *grandfather);
void Rotate_right_tree (RB *t, RB *h, RB *grandfather, RB *father);
void Rotate_left_tree (RB *t, RB *h, RB *grandfather, RB *father);
void RB_fixup (RB *t, RB *h, RB *leaf);
void Go_to_leaves (RB *x, RB *y, RB *h);
void Hitch (RB *t, RB *h, RB *y);
void RB_insert (RB *t, RB h, RB *leaf);
RB Create_RB_tree (RB t, int count);
int Height (RB t);
void Print_height (int c, int count);


int main ()
{
    int count;
    if (scanf("%d", &count) == 0)
    {
        return 0;
    }
    RB h = Give_memory(count);
    RB t = Create_RB_tree(h, count);
    Print_height(Height(t), count);
    free(h);
    return 0;
}

RB Give_memory (int count)
{
    RB h = (RB) malloc((count + 1) * sizeof (struct RBTree));
    assert(h != NULL);
    h[count].Colour = black;
    h[count].Right = NULL;
    h[count].Left = NULL;
    for (int i = 0; i < count; i++)
    {
        h[i].Right = &h[count];
        h[i].Left = &h[count];
        h[i].Value = 0;
        h[i].Colour = red;
    }
    return h;
}

bool Is_leaf (RB h)
{
    return h -> Right == NULL && h -> Left == NULL;
}

void Find_father_and_grandfather (RB *t, RB *f, RB *g, int x)
{
    RB h = *t;
    while (h -> Value != x)
    {
        *g = *f;
        *f = h;
        if (h -> Value > x)
        {
            h = h -> Left;
        }
        else
        {
            h = h -> Right;
        }
    }
}

void Find_father (RB *t, RB *f, int x)
{
    RB h = *t;
    while (h -> Value != x)
    {
        *f = h;
        if (h -> Value > x)
        {
            h = h -> Left;
        }
        else
        {
            h = h -> Right;
        }
    }
}

RB Rotate_right (RB *t)
{
    RB h = (*t) -> Left;
    (*t) -> Left  = h -> Right;
    h -> Right = (*t);
    return h;
}

RB Rotate_left (RB *t)
{
    RB h = (*t) -> Right;
    (*t) -> Right = h -> Left;
    h -> Left = (*t);
    return h;
}

void Rotate_right_with_paint (RB *father, RB *grandfather)
{
    *father = Rotate_right(grandfather);
    (*father) -> Colour = black;
    (*father) -> Right -> Colour = red;
}

void Rotate_left_with_paint (RB *father, RB *grandfather)
{
    *father = Rotate_left(grandfather);
    (*father) -> Colour = black;
    (*father) -> Left -> Colour = red;
}

void Repaint (RB *grandfather, RB *father, RB *uncle)
{
    (*grandfather) -> Colour = red;
    (*uncle) -> Colour = black;
    (*father) -> Colour = black;
}

void Hitch_father (RB *ggf, RB *father)
{
    if ((*ggf) -> Value <= (*father) -> Value)
    {
        (*ggf) -> Right = *father;
    }
    else
    {
        (*ggf) -> Left = *father;
    }
}

void Is_root_for_right (RB *t, RB *father, RB *grandfather)
{
    if (*grandfather == *t)
    {
        Rotate_left_with_paint(father, grandfather);
        *t = *father;
    }
    else
    {
        RB ggf = *t;
        Find_father(t, &ggf, (*grandfather) -> Value);
        Rotate_left_with_paint(father, grandfather);
        Hitch_father(&ggf, father);
    }
}

void Rotate_right_tree (RB *t, RB *h, RB *grandfather, RB *father)
{
    if (*h == (*father) -> Right)
    {
        Is_root_for_right(t, father, grandfather);
    }
    else
    {
        RB x = Rotate_right(father);
        *h = x;
        (*grandfather) -> Right = x;
        (*h) = (*h) -> Right;
        Is_root_for_right(t, father, grandfather);
    }
}

void Is_root_for_left (RB *t, RB *father, RB *grandfather)
{
    if (*grandfather == *t)
    {
        Rotate_right_with_paint(father, grandfather);
        *t = *father;
    }
    else
    {
        RB ggf = *t;
        Find_father(t, &ggf, (*grandfather) -> Value);
        Rotate_right_with_paint(father, grandfather);
        Hitch_father(&ggf, father);
    }
}

void Rotate_left_tree (RB *t, RB *h, RB *grandfather, RB *father)
{
    if (*h == (*father) -> Left)
    {
        Is_root_for_left(t, father, grandfather);
    }
    else
    {
        RB x = Rotate_left(father);
        *h = x;
        (*grandfather) -> Left = x;
        (*h) = (*h) -> Left;
        Is_root_for_left(t, father, grandfather);
    }
}

void RB_fixup (RB *t, RB *h, RB *leaf)
{
    RB father = *leaf;
    RB grandfather = *leaf;
    Find_father_and_grandfather(t, &father, &grandfather, (*h) -> Value);
    if (*t == *h)
    {
        (*h) -> Colour = black;
        return;
    }
    else if (father -> Colour == black)
    {
        return;
    }
    else
    {
        if (father == grandfather -> Left)
        {
            RB uncle = grandfather -> Right;
            if (uncle -> Colour == red)
            {
                Repaint(&grandfather, &father, &uncle);
                RB_fixup(t, &grandfather, leaf);
            }
            else
            {
                Rotate_left_tree(t, h, &grandfather, &father);
            }
        }
        else
        {
            RB uncle = grandfather -> Left;
            if (uncle -> Colour == red)
            {
                Repaint(&grandfather, &father, &uncle);
                RB_fixup(t, &grandfather, leaf);
            }
            else
            {
                Rotate_right_tree(t, h, &grandfather, &father);
            }
        }
    }
}

void Go_to_leaves (RB *x, RB *y, RB *h)
{
    while (!Is_leaf(*x))
    {
        *y = *x;
        if ((*h) -> Value  < (*x) -> Value)
        {
            *x = (*x) -> Left;
        }
        else
        {
            *x = (*x) -> Right;
        }
    }
}

void Hitch (RB *t, RB *h, RB *y)
{
    if (*y == NULL)
    {
        *t = *h;
    }
    else if ((*h) -> Value >= (*y) -> Value)
    {
        (*y) -> Right = *h;
    }
    else
    {
        (*y) -> Left = *h;
    }
}

void RB_insert (RB *t, RB h, RB *leaf)
{
    RB y = NULL;
    RB x = *t;
    Go_to_leaves(&x, &y, &h);
    Hitch(t, &h, &y);
    RB_fixup(t, &h, leaf);
}

RB Create_RB_tree (RB t, int count)
{
    RB leaf = &t[count];
    RB h = leaf;
    for (int i = 0; i < count; i++)
    {
        int c;
        if (scanf("%d", &c) != 1)
        {
            return NULL;
        }
        t[i].Value = c;
        RB_insert(&h, &t[i], &leaf);
    }
    return h;
}

int Height (RB t)
{
    if (Is_leaf(t))
    {
        return 0;
    }
    else
    {
        int c = Height(t -> Left) + (t -> Colour == black);
        return c;
    }
}

void Print_height (int c, int count)
{
    if (count)
    {
        printf("%d", c + 1);
    }
    else
    {
        printf("%d", 0);
    }
}

