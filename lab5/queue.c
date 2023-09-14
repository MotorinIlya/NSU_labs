#include "queue.h"

int Queue_is_full (TQueue* root_queue)
{
    return (root_queue -> Next ? YES : NO);
}

TNode* Extract_queue (TQueue** root_queue)
{
    TQueue* queue = *root_queue;
    if (!queue -> Next)
    {
        TNode* node = queue -> Node;
        free(queue);
        *root_queue = NULL;
        return node;
    }
    while (queue -> Next -> Next != NULL)
    {
        queue = queue -> Next;
    }
    TNode* node = queue -> Next -> Node;
    free(queue -> Next);
    queue -> Next = NULL;
    return node;
}

void Push (TCharQueue* char_queue, const unsigned char symbol)
{
    while (char_queue -> Next)
    {
        char_queue = char_queue -> Next;
    }
    char_queue -> Next = malloc(sizeof(TCharQueue));
    char_queue -> Next -> Char = symbol;
    char_queue -> Next -> Next = NULL;
}

TQueue* Create_queue (int* count_char)
{
    TQueue* root_queue = NULL;
    for (int i = 0; i < MAX_SYMBOL; i++)
    {
        if (count_char[i] == 0)
        {
            continue;
        }
        TNode* new_node = Initialization_new_node(i, count_char[i]);
        if (Insert_in_queue(&root_queue, new_node) == EDOM)
        {
            if (root_queue)
            {
                Free_queue(root_queue);
            }
            return NULL;
        }
    }
    return root_queue;
}

TNode* Initialization_new_node(const unsigned char symbol, const int frequency)
{
	TNode* new_node = malloc(sizeof(TNode));
	if (!new_node)
    {
		return NULL;
	}
    new_node -> Char = symbol;
    new_node -> Frequency = frequency;
    new_node -> Left = NULL;
    new_node -> Right = NULL;
	return new_node;
}

static void Add_object (TQueue** root_queue, TQueue* new_queue, TQueue* next, TNode* new_node)
{
    new_queue -> Next = next;
    new_queue -> Node = new_node;
    *root_queue = new_queue;
}

int Insert_in_queue(TQueue** root_queue, TNode* new_node)
{
    TQueue* queue = *root_queue;
    TQueue* new_queue = malloc(sizeof(TQueue));
    if (!new_queue)
    {
        return EDOM;
    }
    int correct_frequency = new_node -> Frequency;
    if (!(*root_queue))
    {
        Add_object(root_queue, new_queue, NULL, new_node);
    }
    else if ((*root_queue) -> Node -> Frequency < correct_frequency)
    {
        Add_object(root_queue, new_queue, *root_queue, new_node);
    }
    else
    {
        while (queue -> Next != NULL && queue -> Next -> Node -> Frequency > correct_frequency)
        {
            queue = queue -> Next;
        }
        if (queue -> Next)
        {
            Add_object(&(queue -> Next), new_queue, queue -> Next, new_node);
        }
        else
        {
            Add_object(&(queue -> Next), new_queue, NULL, new_node);
        }
    }
    return 0;
}

void Free_queue(TQueue* queue)
{
    if (queue -> Next)
    {
        Free_queue(queue -> Next);
    }
    if (queue -> Node)
    {
        Free_tree(queue -> Node);
    }
    free(queue);
}

void Free_tree (TNode* node)
{
    if (node -> Left)
    {
        Free_tree(node -> Left);
    }
    if (node -> Right)
    {
        Free_tree(node -> Right);
    }
    free(node);
}