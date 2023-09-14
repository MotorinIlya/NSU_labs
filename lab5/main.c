#include "decode.h"
#include "encode.h"
#include "stream.h"
#include "structs.h"

int main()
{
    TStream stream = Create_stream("in.txt", "out.txt");
    unsigned char direction;
    Read_stream(&direction, sizeof(unsigned char), 1, stream);
    if (direction == 'c')
    {
        Cheak_file(stream);
        if (Build_huffman_tree(stream) == EDOM)
        {
            return EDOM;
        }
    }
    else
    {
        Cheak_file(stream);
        if (Decode(stream) == EDOM)
        {
            return EDOM;
        }
    }
}
