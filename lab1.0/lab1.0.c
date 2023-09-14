#include <stdio.h>
#include <stdbool.h>
#define MAX_LEN 17

int Get_size (const char *a)
{
    int i = 0;
    while (a[i] != '\0')
    {
        i++;
    }
    return i;
}

int Delete_to_translation_string (char p[])
{
    int i = 0;
    while (p[i] != '\n')
    {
        i++;
    }
    p[i] = '\0';
    return i;
}

bool Is_original (const char *p, int j)
{
    for (int i = Get_size(p) - 2; i > j; i--)
    {
        if (p[i] == p[j])
        {
            return false;
        }
    }
    return true;
}

int Find_index_symbol (const char *p, int j, int size)
{
    for (int i = size - 2; i > j; i--)
    {
        if (p[i] == p[j])
        {
            return i;
        }
    }
    if ((j == 0) && (p[size - 1] == p[j]))
    {
        return size - 1;
    }
    return size;
}

void Make_table (const char p[], int t[], int size)
{
    int index = 1;
    for (int i = size - 2; i >= 0 ; i--)
    {
        if (Is_original(p, i)) {
            t[i] = index;
            index++;
        }
        else
        {
            t[i] = Find_index_symbol(p, i, size);
        }
    }
    t[size - 1] = Find_index_symbol(p, 0, size);
}

unsigned int Find_table_index (const char p[], const char w[], const int t[], int size)
{
    char sym = w[size - 1];
    for (int i = size - 1; i >= 0; i--)
    {
        if (sym == p[i])
        {
            return t[i];
        }
    }
    return size;
}

void Compare_symbols (const char p[],  const char w[], const unsigned int *index, int size)
{
    for (int i =  size - 1; i >= 0; i--)
    {
        printf("%u ", *index + i);
        if (p[i] != w[i])
        {
            break;
        }
    }
}

bool Shift (char w[],  const unsigned int shift)
{
    int i = 0;
    while (w[i + shift] != '\0')
    {
        w[i] = w[i + shift];
        i++;
    }
    return (fread(&w[i], 1, shift, stdin) != shift);
}

void Algorithm_Boyer_Mur (const char p[], char w[], const int t[], int size)
{
    unsigned int index = 1;
    while (true)
    {
        Compare_symbols(p, w, &index, size);
        unsigned int shift = Find_table_index(p, w, t, size);
        index += shift;
        bool is_end = Shift(w, shift);
        if (is_end)
        {
            return;
        }
    }
}

int main ()
{
    char pattern[MAX_LEN + 1] = {0};
    if (fgets(pattern, MAX_LEN + 1, stdin) == NULL)
    {
        return 0;
    }
    int size_window = Delete_to_translation_string(pattern);
    char window[MAX_LEN + 1] = {0};
    if (fgets(window, size_window + 1,stdin) == NULL)
    {
        return 0;
    }
    int size = Get_size(pattern);
    int table[MAX_LEN - 1] = {0};
    Make_table(pattern, table, size);
    printf("\n");
    Algorithm_Boyer_Mur(pattern, window, table, size);
    return 0;
}
