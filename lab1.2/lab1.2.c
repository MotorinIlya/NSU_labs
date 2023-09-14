#include <stdio.h>
#include <stdbool.h>
#define MAX_LEN 18

int Get_size (const unsigned char p[])
{
    int i = 0;
    while (p[i] != '\0')
    {
        i++;
    }
    return i;
}

void Fill_table (const unsigned char p[], int t[], int size)
{
    int i = 1, j = 0;
    t[0] = 0;
    while (i < size)
    {
        if (p[i] != p[j])
        {
            if (j == 0)
            {
                t[i] = 0;
                i++;
            }
            else
            {
                j = t[j - 1];
            }
        }
        else
        {
            t[i] = j + 1;
            i++;
            j++;
        }
    }
}

int Delete_to_translation_string (unsigned char p[])
{
    int i = 0;
    while (p[i] != '\n')
    {
        i++;
    }
    p[i] = '\0';
    return i;
}

bool Shift (unsigned char w[],  const unsigned int shift)
{
    int i = 0;
    while (w[i + shift] != '\0')
    {
        w[i] = w[i + shift];
        i++;
    }
    return (fread(&w[i], 1, shift, stdin) < shift);
}

void Algorithm_Knut_Morris_Pratt (const unsigned char p[], unsigned char w[], const int t[], const int size)
{
    bool is_end = false;
    int k = 0, l = 0;
    int index = 1;
    while(!is_end)
    {
        if (Get_size(w) < size)
        {
            return;
        }
        if (size == 1)
        {
            if (p[l] == w[k])
            {
                printf("%d %d ", index, 1);
            }
            is_end = Shift(w, 1);
            index++;
            continue;
        }
        if (p[l] == w[k])
        {
            if (l == size - 1)
            {
                printf("%d %d ", index - size + 1, l + 1);
                l = t[l - 1];
            }
            if (k == size - 1)
            {
                is_end = Shift(w, size - l - 1);
                k = l;
                continue;
            }
            k++;
            l++;
            index++;
        }
        else
        {
            if (l == 0)
            {
                k++;
                if (k == size - 1)
                {
                    is_end = Shift(w, size - 1);
                    k = 0;
                }
                index++;
            }
            else
            {
                is_end = Shift(w, k - t[l - 1]);
                if (is_end)
                {
                    return;
                }
                printf("%d %d ", index - l, l);
                k = t[l - 1];
                l = t[l - 1];
            }
        }
    }
}

int main ()
{
    unsigned char pattern[MAX_LEN] = {0};
    int i = 0;
    while ((pattern[i] = getchar()) != '\n')
    {
        i++;
    }
    Delete_to_translation_string(pattern);
    int size = Get_size(pattern);
    if (size == 0)
    {
        return 0;
    }
    int table[size];
    Fill_table(pattern, table, size);
    for (int j = 0; j < size; j++)
    {
        printf("%d ", table[j]);
    }
    printf("\n");
    unsigned char window[MAX_LEN] = {0};
    if (fread(&window, 1, size, stdin) < (unsigned int)size)
    {
        return 0;
    }
    Algorithm_Knut_Morris_Pratt(pattern, window, table, size);
    return 0;
}
