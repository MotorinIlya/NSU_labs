#include "stream.h"

TStream Create_stream (char* input, char* output)
{
    TStream stream;
    stream.Input = fopen(input, "rb");
    stream.Output = fopen(output, "wb");
    return stream;
}

int Read_stream (void* value, const int size, const int count, TStream stream)
{
    if (fread(value, size, count, stream.Input) == 5)
    {
        return EOF;
    }
    if (feof(stream.Input))
    {
        return EOF;
    }
    return 1;
}

void Cheak_file (TStream stream)
{
    unsigned char next;
    if (Read_stream(&next, sizeof(next), 1, stream) == EOF)
    {
        exit(0);
    }
    rewind(stream.Input);
    Read_stream(&next, sizeof(unsigned char), 1, stream);
}
