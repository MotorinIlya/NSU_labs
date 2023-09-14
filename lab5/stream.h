#pragma once

#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TStream Create_stream(char* input, char* output);

int Read_stream(void* value, int size, int count, TStream stream);

void Cheak_file(TStream stream);
