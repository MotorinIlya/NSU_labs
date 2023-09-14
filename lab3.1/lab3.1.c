#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

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

int Transposition (T_array *array, int first, int last)
{
    int pivot = Get(array, (first + last)/2);
    int i = first, j = last;
    while (true)
    {
        while (Get(array, i) < pivot)
        {
            i++;
        }
        while (Get(array, j) > pivot)
        {
            j--;
        }
        if (i >= j)
        {
            return j;
        }
        Swap(array, i, j);
        i++;
        j--;
    }
}

void Quick_sort (T_array *array, int first, int last)
{
    if (first < last)
    {
        int middle = Transposition(array, first, last);
        Quick_sort(array, first, middle);
        Quick_sort(array, middle + 1, last);
    }
}

int main ()
{
    int count_elements;
    if (scanf("%d", &count_elements) != 1)
    {
        printf("bad input");
        return 0;
    }
    T_array elements = Create(count_elements);
    for (int i = 0; i < count_elements; i++)
    {
        int a;
        if (scanf("%d", &a) != 1)
        {
            printf("bad input");
            return 0;
        }
        Set(&elements, i, a);
    }
    Quick_sort(&elements, 0, Get_size(&elements) - 1);
    for (int i = 0; i < Get_size(&elements); i++)
    {
        printf("%d ", Get(&elements, i));
    }
    Destroy(&elements);
    return 0;
}
