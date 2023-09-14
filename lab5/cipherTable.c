#include "cipherTable.h"

static void Recurse_build_cipher_table(
        FILE* output,
        TCipherTable* cipher_table,
        TNode* node,
        TCharQueue* char_queue,
        unsigned long long int code,
        int bits,
        unsigned char* buffer,
        int* size_buffer,
        unsigned long long int* count_char,
        const int* count_char_table
        )
{
    if (node -> Right == NULL && node -> Left == NULL)
    {
        cipher_table[node -> Char].Code = code;
        if (bits != 0)
        {
            cipher_table[node -> Char].Bits = bits;
            *count_char += bits * count_char_table[node -> Char];
        }
        else
        {
            cipher_table[node -> Char].Bits = 1;
            *count_char += 1 * count_char_table[node -> Char];
        }
        node -> Bits = bits;
        *buffer = (*buffer) << 1;
        *buffer = (*buffer) | 1;
        (*size_buffer)++;
        if (*size_buffer == 8)
        {
            fwrite(buffer, sizeof(unsigned char), 1, output);
            *buffer = 0;
            *size_buffer = 0;
        }
        Push(char_queue, node -> Char);
        return;
    }
    *buffer = (*buffer) << 1;
    *buffer = (*buffer) | 0;
    (*size_buffer)++;
    if (*size_buffer == 8)
    {
        fwrite(buffer, sizeof(unsigned char), 1, output);
        *buffer = 0;
        *size_buffer = 0;
    }
    if (node -> Left != NULL)
    {
        Recurse_build_cipher_table(
                output,
                cipher_table,
                node -> Left,
                char_queue,
                (code << 1),
                bits + 1, buffer,
                size_buffer,
                count_char,
                count_char_table
                );
    }
    if (node -> Right != NULL)
    {
        Recurse_build_cipher_table(
                output,
                cipher_table,
                node -> Right,
                char_queue,
                (code << 1) + 1,
                bits + 1,
                buffer,
                size_buffer,
                count_char,
                count_char_table
                );
    }
}

void Print_alf(TStream stream, TCharQueue* char_queue)
{
    char_queue = char_queue -> Next;
    while (char_queue)
    {
        fwrite(&char_queue -> Char, sizeof(unsigned char), 1, stream.Output);
        TCharQueue* prev = char_queue;
        char_queue = char_queue -> Next;
        free(prev);
    }
}

static void Print_useless_bits(TStream stream, const unsigned long long int count_char)
{
    unsigned char use_less_bits = (8 - count_char % 8) % 8;
    fwrite(&use_less_bits, sizeof(unsigned char), 1, stream.Output);
}


void Build_cipher_table(
        TStream stream,
        TCipherTable* cipher_table,
        TNode* root_tree,
        TCharQueue* char_queue,
        int* count_char_table
        )
{
    int size_buffer = 0;
    unsigned char buffer = 0;
    unsigned long long int count_char = 0;
    Recurse_build_cipher_table(
            stream.Output,
            cipher_table,
            root_tree,
            char_queue,
            0,
            0,
            &buffer,
            &size_buffer,
            &count_char,
            count_char_table
            );
    if (size_buffer != 0)
    {
        buffer = buffer << (8 - size_buffer);
        fwrite(&buffer, sizeof(unsigned char), 1, stream.Output);
    }
    Print_alf(stream, char_queue);
    rewind(stream.Input);
    Print_useless_bits(stream, count_char);
    Read_stream(&buffer, sizeof(unsigned char), 1, stream);
}