#pragma once

#include <stdio.h>

enum Const {
	MAX_SYMBOL = 257,
	NOT_FOUND = -1,
	YES = 1,
	NO = 0,
	FROM_NO_CMD = 2,
	FROM_CMD = 3
};

typedef struct TStream {
	FILE* Input;
	FILE* Output;
} TStream;

typedef struct TNode {
	struct TNode* Left;
	struct TNode* Right;
	int Frequency;
	int Bits;
	unsigned long long int Code;
	unsigned char Char;
} TNode;

typedef struct TQueue {
	struct TNode* Node;
	struct TQueue* Next;
} TQueue;

typedef struct TCipherTable {
	unsigned long long int Code;
	int Bits;
} TCipherTable;

typedef struct TCharQueue {
	struct TCharQueue* Next;
	unsigned char Char;
} TCharQueue;

