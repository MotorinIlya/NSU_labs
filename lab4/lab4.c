#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#define MAX_STR 1000

typedef struct TStack
{
    unsigned char *Value;
    struct TStack *Next;
} *TStack;

typedef struct TIStack
{
    int Value;
    struct TIStack *Next;
} *TIStack;

TStack Create_stack (void);
void Push (TStack *s, unsigned char *a);
unsigned char *Pop (TStack *s);
bool Is_empty (TStack s);
unsigned char *Watch_last (TStack s);
TIStack Create_int_st (void);
void Push_int (TIStack *s, int a);
int Pop_int (TIStack *s);
bool Is_empty_int (TIStack s);
bool Is_operator (unsigned char c);
bool Verify (unsigned char **s, int len);
int Atoi (const unsigned char *s);
bool Is_digit (const unsigned char *c);
unsigned char **Create_str (int a);
unsigned char **Create_new_str (int a);
void Shift (unsigned char **s, int len);
int Scan_str (unsigned char **s);
void Squeeze_out (TStack *h, unsigned char **s, unsigned char **p, unsigned char c, int *len);
void Transfer_els (unsigned char **from, unsigned char **to);
int Inf_post (unsigned char **s_inf, unsigned char **s_post, int len_inf);
bool Count_post (int *z, unsigned char **s, int len);
void Free (unsigned char **s, int len);

int main ()
{
    unsigned char **s = Create_str(MAX_STR + 1);
    int len = Scan_str(s);
    if (!Verify(s, len))
    {
        printf("syntax error");
        Free(s, MAX_STR);
        return 0;
    }
    unsigned char **s_new = Create_new_str(len);
    int new_len = Inf_post(s, s_new, len);
    int a = 0;
    if (!Count_post(&a, s_new, new_len))
    {
        printf("division by zero");
        Free(s, MAX_STR);
        free(s_new);
        return 0;
    }
    printf("%d", a);
    Free(s, MAX_STR);
    free(s_new);
    return 0;
}

TStack Create_stack (void)
{
    return NULL;
}

void Push (TStack *s, unsigned char *a)
{
    TStack h = malloc(sizeof (struct TStack));
    assert(h != NULL);
    h -> Value = a;
    h -> Next = *s;
    *s = h;
}

unsigned char *Pop (TStack *s)
{
    unsigned char *a = (*s) -> Value;
    TStack h = *s;
    *s = (*s) -> Next;
    free(h);
    return a;
}

bool Is_empty (TStack s)
{
    if (s == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }

}

unsigned char *Watch_last (TStack s)
{
    return s -> Value;
}

TIStack Create_int_st (void)
{
    return NULL;
}

void Push_int (TIStack *s, int a)
{
    TIStack h = (TIStack) malloc(sizeof (struct TIStack));
    assert(h != NULL);
    h -> Value = a;
    h -> Next = *s;
    *s = h;
}

int Pop_int (TIStack *s)
{
    assert(!Is_empty_int(*s));
    int a;
    a = (*s) -> Value;
    TIStack h = *s;
    *s = (*s) -> Next;
    free(h);
    return a;
}

bool Is_empty_int (TIStack s)
{
    return s == NULL;
}

void Destroy_int (TIStack *s)
{
    while (*s != NULL)
    {
        Pop_int(s);
    }
}

bool Is_operator (unsigned char c)
{
    return (c >= '*' && c <= '/' && c != ',' && c != '.');
}

bool Is_bracket (unsigned char c)
{
    return (c == '(' || c == ')');
}

bool Verify (unsigned char **s, const int len) {
    TIStack h = Create_int_st();
    unsigned char last = 0;
    int x;
    bool there_is_numb = false;
    for (int i = 0; i < len; i++)
    {
        if (isdigit(*s[i]))
        {
            there_is_numb = true;
        }
        if (!(isdigit(*s[i]) || Is_bracket(*s[i]) || Is_operator(*s[i])))
        {
            Destroy_int(&h);
            return false;
        }
        if (*s[i] == '(')
        {
            Push_int(&h, i);
        }
        if (*s[i] == ')')
        {
            if (Is_empty_int(h))
            {
                return false;
            }
            else
            {
                x = Pop_int(&h);
                if (((i - x) % 2) == 1)
                {
                    Destroy_int(&h);
                    return false;
                }
            }
        }
        if (Is_operator(*s[i]) && Is_operator(last))
        {
            Destroy_int(&h);
            return false;
        }
        if (((i == 0) || (i == len - 1)) && Is_operator(*s[i]))
        {
            Destroy_int(&h);
            return false;
        }
        last = *s[i];
    }
    if (!there_is_numb)
    {
        Destroy_int(&h);
        return false;
    }
    if (!Is_empty_int(h))
    {
        Destroy_int(&h);
        return false;
    }
    Destroy_int((&h));
    return true;
}

int Atoi (const unsigned char *s)
{
    int n = 0;
    for (int i = 0; isdigit(s[i]); i++)
    {
        n = 10 * n + (s[i] - '0');
    }
    return n;
}

bool Is_digit (const unsigned char *c)
{
    for (int i = 0; c[i] != '\0'; i++)
    {
        if (!isdigit(c[i]))
        {
            return false;
        }
    }
    return true;
}

unsigned char **Create_str (int a)
{
    unsigned char **s = (unsigned char**) malloc(a * sizeof (unsigned char*));
    assert(s != NULL);
    for (int i = 0; i < a; i++)
    {
        s[i] = (unsigned char*) calloc(100, sizeof (unsigned char));
        assert(s[i] != NULL);
    }
    return s;
}

unsigned char **Create_new_str (int a)
{
    unsigned char **s = (unsigned char**) malloc(a * sizeof (unsigned char*));
    assert(s != NULL);
    return s;
}

void Shift (unsigned char **s, int len)
{
    int i;
    free(s[0]);
    for (i = 0; i < len; i++)
    {
        s[i] = s[i+1];
    }
}

int Scan_str (unsigned char **s)
{
    int len = 0;
    unsigned char c;
    unsigned char p = 'x';
    int ind = 0;
    while ((c = getchar()) != '\n')
    {
        if (isdigit(c))
        {
            if (!isdigit(p))
            {
                ind = 0;
                len++;
            }
            s[len][ind++] = c;
        }
        else
        {
            ind = 0;
            s[++len][ind] = c;
        }
        p = c;
    }
    Shift(s, MAX_STR);
    return len;
}

void Squeeze_out (TStack* h, unsigned char **s, unsigned char **p, unsigned char c, int *len)
{
    if (c == ')')
    {
        while (*Watch_last(*h) != '(')
        {
            s[(*len)++] = Pop(h);
        }
        Pop(h);
    }
    else if (c == '+' || c == '-')
    {
        while (!Is_empty(*h) && (Watch_last(*h)[0] == '+' || Watch_last(*h)[0] == '-' || Watch_last(*h)[0] == '*' || Watch_last(*h)[0] == '/'))
        {
            s[*len] = Pop(h);
            (*len)++;
        }
        Push(h, *p);
    }
    else if (c == '*' || c == '/')
    {
        while (!Is_empty(*h) && (Watch_last(*h)[0] == '*' || Watch_last(*h)[0] == '/'))
        {
            s[*len] = Pop(h);
            (*len)++;
        }
        Push(h, *p);
    }
}

void Transfer_els (unsigned char **from, unsigned char **to)
{
    *to = *from;
}

int Inf_post (unsigned char **s_inf, unsigned char **s_post, int len_inf)
{
    TStack h = Create_stack();
    int len_post = 0;
    for (int i = 0; i < len_inf; i++)
    {
        if (Is_digit(s_inf[i]))
        {
            Transfer_els(&s_inf[i], &s_post[len_post]);
            len_post++;
        }
        else if (Is_empty(h) || s_inf[i][0] == '(')
        {
            Push(&h, s_inf[i]);
        }
        else
        {
            switch (s_inf[i][0]) {
                case ')' : Squeeze_out(&h, s_post, &s_inf[i], ')', &len_post);
                    break;
                case '*' : Squeeze_out(&h, s_post, &s_inf[i], '*', &len_post);
                    break;
                case '/' : Squeeze_out(&h, s_post, &s_inf[i], '/', &len_post);
                    break;
                case '+' : Squeeze_out(&h, s_post, &s_inf[i], '+', &len_post);
                    break;
                case '-' : Squeeze_out(&h, s_post, &s_inf[i], '-', &len_post);
                    break;
            }
        }
    }
    while (!Is_empty(h))
    {
        s_post[len_post] = Pop(&h);
        len_post++;
    }
    return len_post;
}

bool Count_post (int *z, unsigned char **s, int len)
{
    TIStack h = Create_int_st();
    for (int i = 0; i < len; i++)
    {
        if (Is_digit(s[i]))
        {
            Push_int(&h, Atoi(s[i]));
        }
        else
        {
            int a = Pop_int(&h);
            int b = Pop_int(&h);
            switch (s[i][0]) {
                case '+' : *z = a + b;
                    break;
                case '-' : *z = b - a;
                    break;
                case '*' : *z = a * b;
                    break;
                case '/' :
                    if (a == 0)
                    {
                        Destroy_int(&h);
                        return false;
                    }
                    *z = b / a;
                    break;
            }
            Push_int(&h, *z);
        }
    }
    *z = Pop_int(&h);
    Destroy_int(&h);
    return true;
}

void Free (unsigned char **s, int len)
{
    for (int i = 0; i < len; i++)
    {
        free(s[i]);
    }
    free(s);
}
