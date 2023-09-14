#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int Get_size (const unsigned char p[])
{
    int i = 0;
    while (p[i] != '\0')
    {
        i++;
    }
    return i;
}

void Compare_symbols (const unsigned char p[], const unsigned char w[], const int j, const int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d ", i + j + 1);
        if (p[i] != w[i])
        {
            return;
        }
    }
}

unsigned int Calc_exponent (int basis, int indicator)
{
    unsigned int res = 1;
    unsigned int mult = basis;
    while (true)
    {
        if (indicator % 2 == 1)
        {
            res *= mult;
        }
        indicator /= 2;
        if (indicator == 0)
        {
            return res;
        }
        mult *= mult;
    }
}

unsigned int Get_hash (const unsigned char p[], const int size)
{
    unsigned int hash = 0;
    for (int i = 0; i < size; i++)
    {
        hash += (((unsigned int)p[i]) % 3) * Calc_exponent(3, i);
    }
    return hash;
}

bool Shift (unsigned char* w, unsigned int *hash, unsigned int exp, unsigned int size)
{
    unsigned int c = fgetc(stdin);
    if ((int)c == EOF) {
        return true;
    }
    memmove(w, w + 1, size - 1);
    w[size - 1] = c;
    *hash = *hash / 3u + ((unsigned int)c % 3u) * exp;
    return false;
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

void Algorithm_Rabin_Karp (unsigned char p[], unsigned char w[])
{
    int size = Get_size(p);
    unsigned int hash_p = Get_hash(p, size);
    unsigned int hash_w = Get_hash(w, size);
    bool flag = false;
    int i = 0;
    unsigned int exp = Calc_exponent(3, Get_size(p) - 1);
    while (!flag)
    {
        if (hash_p != hash_w)
        {
            flag = Shift(w, &hash_w, exp, size);
            i++;
            continue;
        }
        Compare_symbols(p, w, i, size);
        flag = Shift(w, &hash_w, exp, size);
        i++;
    }

}

int main ()
{
    unsigned char pattern[18] = {0};
    int i = 0;
    while ((pattern[i] = getchar()) != '\n')
    {
        i++;
    }
    Delete_to_translation_string(pattern);
    int size = Get_size(pattern);
    printf("%u ", Get_hash(pattern, size));
    unsigned int size_window = i;
    unsigned char window[18] = {0};
    if (fread(&window, 1, size_window, stdin) == 0)
    {
        return 0;
    }
    Algorithm_Rabin_Karp(pattern, window);
    return 0;
}
