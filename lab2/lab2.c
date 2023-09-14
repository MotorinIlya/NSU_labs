#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#define COUNT_DIGITS 10

typedef struct TA
{
    int *data;
    int size;
}TA;

typedef char Char_array[100];

TA Create_array (int s)
{
    int *p = malloc(s * sizeof(int));
    assert(p != NULL);
    struct TA a;
    a.data = p;
    a.size = s;
    return a;
}

int Get_size (TA *a)
{
    return a->size;
}

void Destroy (TA *a)
{
    free((*a).data);
    *a = (TA){NULL, 0};
}

int Get (const TA *a, int i)
{
    assert(i < (*a).size);
    return (*a).data[i];
}

void Set (const TA *a, int i, int x)
{
    assert(i < (*a).size);
    (*a).data[i] = x;
}

void Add (const TA *a, int i, int t)
{
    assert(i < (*a).size);
    (*a).data[i] = (*a).data[i] + t;
}

void Swap (TA *a, int i, int j)
{
    assert(i < (*a).size);
    assert(j < (*a).size);
    int t = (*a).data[i];
    (*a).data[i] = (*a).data[j];
    (*a).data[j] = t;
}

int Get_length (const char a[])
{
    int count = 0;
    while (a[count] != '\0')
    {
        count++;
    }
    return count;
}

int Calc_exponent (int basis_degree, int indicator_degree)
{
    int compositions = 1;
    for (int i = 0; i < indicator_degree; i++)
    {
        compositions *= basis_degree;
    }
    return compositions;
}

void Reverse_array_for_n_element (TA *a, int i)
{
    for (int j = Get_size(a) - 1; j > i; j--, i++)
    {
        Swap(a, i, j);
    }
}

TA Make_next_permutation (TA *sequence_of_number)
{
    for (int i = Get_size(sequence_of_number) - 2; i >= 0; i--) {
        if (Get(sequence_of_number, i) < Get(sequence_of_number, i + 1)) {
            for (int j = Get_size(sequence_of_number) - 1; j > i; j--) {
                if (Get(sequence_of_number, j) > Get(sequence_of_number, i)) {
                    Swap(sequence_of_number, i, j);
                    Reverse_array_for_n_element(sequence_of_number, i + 1);
                    return *sequence_of_number;
                }
            }
        }
    }
    return *sequence_of_number;
}

_Bool Verify_on_number_point (const Char_array p)
{
    for (int i = 0; i < Get_length(p); i++)
    {
        if (!(p[i] >= '0' && p[i] <= '9'))
        {
            return false;
        }
    }
    return true;
}

_Bool Verify_on_repeat_numbers (Char_array a)
{
    TA numbers = Create_array(COUNT_DIGITS);
    for (int i = 0; i < 10; i++)
    {
        Set(&numbers, i, 0);
    }
    for (int i = 0; i < Get_length(a); i++)
    {
        Add(&numbers, a[i] - '0', 1);
    }
    for (int i = 0; i < 10; i++)
    {
        if (Get(&numbers, i) > 1)
        {
            Destroy(&numbers);
            return false;
        }
    }
    Destroy(&numbers);
    return true;
}

_Bool Full_verify (Char_array a, Char_array p)
{
    if (Verify_on_number_point(a) && Verify_on_repeat_numbers(a) && Verify_on_number_point(p))
    {
        return true;
    }
    return false;
}

_Bool Verify_on_last_permutation (TA a)
{
    for (int i = 0; i < Get_size(&a) - 1; i++)
    {
        if (Get(&a, i) < Get(&a, i + 1))
        {
            return false;
        }

    }
    return true;
}

int Convert_to_number (Char_array p)
{
    int number = 0;
    for (int i = Get_length(p) - 1; i >= 0; i--)
    {
        number += (p[i] - '0') * Calc_exponent(10, Get_length(p) - i - 1);
    }
    return number;
}

void Delete_translation_symbol (char p[])
{
    int i = 0;
    while (p[i] != '\n')
    {
        i++;
    }
    p[i] = '\0';
}

int main ()
{
    Char_array number = {0};
    Char_array count_permutation = {0};
    if (fgets(number, 100, stdin) == NULL)
    {
        printf("bad input");
        return 0;
    }
    if (fgets(count_permutation, 100, stdin) == NULL)
    {
        printf("bad input");
        return 0;
    }
    Delete_translation_symbol(number);
    Delete_translation_symbol(count_permutation);
    if (!(Full_verify(number, count_permutation)))
    {
        printf("bad input");
        return 0;
    }
    int count = Convert_to_number(count_permutation);
    TA number_elements = Create_array(Get_length(number));
    for (int i = 0; i < Get_size(&number_elements); i++)
    {
        Set(&number_elements, i, number[i] - '0');
    }
    for (int i = 0; i < count; i++)
    {
        if (Verify_on_last_permutation(number_elements))
        {
            Destroy(&number_elements);
            return 0;
        }
        number_elements = Make_next_permutation(&number_elements);
        for (int j = 0; j < Get_size(&number_elements); j++)
        {
            printf("%d", Get(&number_elements, j));
        }
        printf("\n");
    }
    Destroy(&number_elements);
    return 0;
}
