#pragma once

#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TNode* Initialization_new_node(unsigned char symbol, int frequency);

void Push (TCharQueue* char_queue, unsigned char symbol);

TNode* Extract_queue (TQueue** root_queue);

int Queue_is_full (TQueue* root_queue);

int Insert_in_queue(TQueue** root_queue, TNode* new_node);

void Free_queue(TQueue* queue);

TQueue* Create_queue (int* count_char);

void Free_tree (TNode* node);
