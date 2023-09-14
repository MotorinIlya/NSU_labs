#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct T_array
{
    int *data;
    int size;
} T_array;

T_array Create (int s)
{
    int *p = malloc(s * sizeof(int));
    assert(p != NULL);
    struct T_array a;
    a.data = p;
    a.size = s;
    return a;
}

int Get_size (T_array *a)
{
    return a->size;
}

void Destroy (T_array *a)
{
    free(a -> data);
    *a = (T_array){NULL, 0};
}

int Get (T_array *a, int i)
{
    assert(i < Get_size(a));
    return a -> data[i];
}

void Set (T_array *a, int i, int x)
{
    assert(i < Get_size(a));
    a->data[i] = x;
}

void Swap (T_array *a, int i, int j)
{
    int tmp = Get(a, i);
    Set(a, i, Get(a, j));
    Set(a, j, tmp);
}

int Get_max_element (int a, int b)
{
    if (a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int Get_index_max_element (T_array *a, int i, int j)
{
    if (Get(a, i) >= Get(a, j))
    {
        return i;
    }
    else
    {
        return j;
    }
}

void Bring_element_on_other_position (T_array *elements, int max_position, int real_position)
{
    int i = real_position;
    int j = i;
    while (i <= max_position)
    {
        if (i * 2 + 2 <= max_position)
        {
            if (Get(elements, i) < Get_max_element(Get(elements, i * 2 + 1), Get(elements, i * 2 + 2)))
            {
                int p = Get_index_max_element(elements, i * 2 + 1, i * 2 + 2);
                Swap(elements, i, Get_index_max_element(elements, i * 2 + 1, i * 2 + 2));
                i = p;
            }
        }
        else if (i * 2 + 1 <= max_position)
        {
            if (Get(elements, i) < Get(elements, i * 2 + 1))
            {
                Swap(elements, i, i * 2 + 1);
                i = i * 2 + 1;
            }
        }
        if (j == i)
        {
            break;
        }
        j = i;
    }
}

void Find_max_element (T_array *elements, int count_elements)
{
    for (int i = count_elements - 1; i > 0; i--)
    {
        int j;
        if (i % 2 == 0)
        {
            j = i / 2 - 1;
        }
        else
        {
            j = (i - 1) / 2;
        }
        if (Get(elements, i) > Get(elements, j))
        {
            Swap(elements, i, j);
        }
        Bring_element_on_other_position(elements, count_elements - 1, i);
    }
}

void Heat_sort (T_array *elements, int count_elements)
{
    Find_max_element(elements, count_elements);
    int i = count_elements - 1;
    while (i > 0)
    {
        Swap(elements, 0, i);
        --i;
        Bring_element_on_other_position(elements, i, 0);
    }
}

int main ()
{
    int count_elements;
    if (scanf("%d", &count_elements) != 1)
    {
        return 0;
    }
    T_array elements = Create(count_elements);
    for (int i = 0; i < count_elements; i++)
    {
        int element;
        if (scanf("%d", &element) != 1)
        {
            return 0;
        }
        Set(&elements, i, element);
    }
    Heat_sort(&elements, count_elements);
    for (int i = 0; i < count_elements; i++)
    {
        printf("%d ", Get(&elements, i));
    }
    Destroy(&elements);
    return 0;
}