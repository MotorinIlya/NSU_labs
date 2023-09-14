#include "decode.h"

static void Decode_useless_bits(TStream stream, int* useless_bits)
{
    Read_stream(useless_bits, sizeof(unsigned char), 1, stream);
}

static int Recurse_decode(TStream stream, TNode* node, unsigned char* buffer, int* buffer_size)
{
    if (!node -> Left)
    {
        TNode* new_node = malloc(sizeof(TNode));
        if (!new_node)
        {
            return EDOM;
        }
        new_node -> Left = NULL;
        new_node -> Right = NULL;
        node -> Left = new_node;
        if (*buffer_size == 0)
        {
            Read_stream(buffer, sizeof(unsigned char), 1, stream);
            (*buffer_size) = 8;
        }
        (*buffer_size)--;
        if (!(((*buffer) >> (*buffer_size)) & 1))
        {
            Recurse_decode(stream, node -> Left, buffer, buffer_size);
        }
    }
    if (!node -> Right)
    {
        TNode* new_node = malloc(sizeof(TNode));
        if (!new_node)
        {
            return EDOM;
        }
        new_node -> Left = NULL;
        new_node -> Right = NULL;
        node -> Right = new_node;
        if (*buffer_size == 0)
        {
            Read_stream(buffer, sizeof(unsigned char), 1, stream);
            (*buffer_size) = 8;
        }
        (*buffer_size)--;
        if (!(((*buffer) >> (*buffer_size)) & 1))
        {
            Recurse_decode(stream, node -> Right, buffer, buffer_size);
        }
    }
    return 0;
}

static void Initialization_decoded_tree(TStream stream, TNode* node, const int bits)
{
    if ((node -> Left == NULL) && (node -> Right == NULL))
    {
        Read_stream(&node -> Char, sizeof(unsigned char), 1, stream);
        node -> Bits = bits;
        return;
    }
    Initialization_decoded_tree(stream, node -> Left, bits + 1);
    Initialization_decoded_tree(stream, node -> Right, bits + 1);
}

static TNode* Decode_tree(TStream stream, int* useless_bits)
{
    TNode* head = malloc(sizeof(TNode));
    if (!head)
    {
        return NULL;
    }
    head -> Left = NULL;
    head -> Right = NULL;
    unsigned char buffer;
    Read_stream(&buffer, sizeof(unsigned char), 1, stream);
    int buffer_size = 8;
    if (!((buffer >> (buffer_size - 1)) & 1)) {
        buffer_size--;
        if (Recurse_decode(stream, head, &buffer, &buffer_size) == EDOM)
        {
            Free_tree(head);
            return NULL;
        }
        Initialization_decoded_tree(stream, head, 0);
    }
    else
    {
        Read_stream(&head -> Char, sizeof(unsigned char), 1, stream);
        head -> Bits = 1;
    }
    Decode_useless_bits(stream, useless_bits);
    return head;
}

TNode* Find_code_in_tree(TNode* tree, unsigned long long int buffer, int buffer_size)
{
    while (buffer_size > 0)
    {
        if (!tree -> Right && !tree -> Left)
        {
            return tree;
        }
        if (buffer >> (buffer_size - 1) & 1)
        {
            if (tree -> Right)
            {
                tree = tree -> Right;
                buffer_size--;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            if (tree -> Left)
            {
                tree = tree -> Left;
                buffer_size--;
            }
            else
            {
                return NULL;
            }
        }
    }
    if (!tree -> Right && !tree -> Left)
    {
        return tree;
    }
    return NULL;
}

int Decode(TStream stream)
{
    int useless_bits = 0;
    TNode* tree = Decode_tree(stream, &useless_bits);
    if (!tree)
    {
        return EDOM;
    }
    unsigned long long int buffer = 0;
    int buffer_size = 0;
    unsigned char input_char_f;
    unsigned char input_char_s;
    Read_stream(&input_char_f, sizeof(unsigned char), 1, stream);
    int exit = Read_stream(&input_char_s, sizeof(unsigned char), 1, stream);
    TNode* output_node;
    while (exit != EOF) {
        buffer = buffer << 8;
        buffer = buffer | input_char_f;
        buffer_size += 8;
        while ((output_node = Find_code_in_tree(tree, buffer, buffer_size)) != NULL && buffer_size >= 1)
        {
            fwrite(&output_node -> Char, sizeof(unsigned char), 1, stream.Output);
            buffer_size -= output_node->Bits;
        }
        input_char_f = input_char_s;
        exit = Read_stream(&input_char_s, sizeof(unsigned char), 1, stream);
    }
    buffer_size += 8;
    if (useless_bits == 0)
    {
        Free_tree(tree);
        return 0;
    }
    while (buffer_size != useless_bits)
    {
        buffer = buffer << 8;
        buffer = buffer | input_char_f;
        while ((output_node = Find_code_in_tree(tree, buffer, buffer_size)) != NULL && buffer_size >= 1)
        {
            fwrite(&output_node -> Char, sizeof(unsigned char), 1, stream.Output);
            buffer_size -= output_node -> Bits;
            if (buffer_size == useless_bits)
            {
                Free_tree(tree);
                return 0;
            }
        }
    }
    Free_tree(tree);
    return 0;
}
