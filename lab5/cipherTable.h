#include "structs.h"
#include "queue.h"
#include "stream.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void Build_cipher_table(
        TStream stream,
        TCipherTable* cipher_table,
        TNode* root_tree,
        TCharQueue* char_queue,
        int* count_char_table
        );
