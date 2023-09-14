#include "encode.h"

static int* Counting_char(TStream stream)
{
    int* count_char = calloc(sizeof(int), MAX_SYMBOL);
    if (!count_char)
    {
        return NULL;
    }
    unsigned char ascii_number;
    while ((Read_stream(&ascii_number, sizeof(unsigned char), 1, stream)) != EOF)
    {
        count_char[ascii_number]++;
    }
    return count_char;
}

static TNode* Unite_tree(TNode* first_tree, TNode* second_tree)
{
    TNode* united_tree = malloc(sizeof(TNode));
    if (!united_tree)
    {
        return NULL;
    }
    united_tree -> Frequency = first_tree -> Frequency + second_tree -> Frequency;
    united_tree -> Char = '\0';
    united_tree -> Left = first_tree;
    united_tree -> Right = second_tree;
    return united_tree;
}

void Encode(TStream stream, TCipherTable* cipher_table)
{
    int buffer_size = 0;
    unsigned long long int buffer = 0;
    unsigned char c;
    while (Read_stream(&c, sizeof(c), 1, stream) != EOF)
    {
        buffer = buffer << cipher_table[c].Bits;
        buffer = buffer | cipher_table[c].Code;
        buffer_size += cipher_table[c].Bits;
        while (buffer_size > 7)
        {
            buffer_size -= 8;
            unsigned char char_output = 0;
            char_output = buffer >> buffer_size;
            buffer = (buffer << (64 - buffer_size)) >> (64 - buffer_size);
            fwrite(&char_output, sizeof(unsigned char), 1, stream.Output);
        }
    }
    unsigned char char_output = buffer << (8 - buffer_size);
    fwrite(&char_output, sizeof(unsigned char), 1, stream.Output);
}

int Build_huffman_tree(TStream stream)
{
    int* count_char = Counting_char(stream);
    if (!count_char)
    {
        return EDOM;
    }
    TQueue* root_queue = Create_queue(count_char);
    if (!root_queue)
    {
        free(count_char);
        return EDOM;
    }
    while (Queue_is_full(root_queue))
    {
        TNode* firstTree = Extract_queue(&root_queue);
        TNode* secondTree = Extract_queue(&root_queue);
        TNode* united_tree = Unite_tree(firstTree, secondTree);
        if (!united_tree)
        {
            Free_queue(root_queue);
            free(count_char);
            return EDOM;
        }
        if (Insert_in_queue(&root_queue, united_tree) == EDOM)
        {
            Free_queue(root_queue);
            free(count_char);
            return EDOM;
        }
    }
    TNode* root_tree = Extract_queue(&root_queue);
    TCipherTable* cipher_table = malloc(sizeof(TCipherTable) * MAX_SYMBOL);
    if (!cipher_table)
    {
        Free_tree(root_tree);
        Free_queue(root_queue);
        free(count_char);
        return EDOM;
    }
    TCharQueue* char_queue = malloc(sizeof(TCharQueue));
    if (!char_queue)
    {
        Free_tree(root_tree);
        Free_queue(root_queue);
        free(count_char);
        free(cipher_table);
        return EDOM;
    }
    char_queue -> Next = NULL;
    Build_cipher_table(stream, cipher_table, root_tree, char_queue, count_char);
    Encode(stream, cipher_table);
    Free_tree(root_tree);
    if (root_queue)
    {
        Free_queue(root_queue);
    }
    free(count_char);
    free(cipher_table);
    free(char_queue);
    return 0;
}

