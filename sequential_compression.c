/*
Alphabet Compression

We've encountered a dataset that contains a lot of sub-sequences of monotonically increasing letters:
Example sub-sequences: ab uvwx ijk

Implement a compression and decompression scheme that replaces 
sequential sub-strings with the starting letter and the number of 
letters that follow in the sequence:

Example Compress: 

Input:
abcdghijkjklmnghijkabgabcdegaj

Output:
a3g4j4g4a1ga4gaj

visual aid:
a[bcd] g[hijk] j[klmn] g[hijk] a[b] g a[bcde] g a j

Decompression is the inverse of the above.

Please implement: 
1. The compress function
2. The decompress function

Use gcc on your machine or an online c tool such as:
https://repl.it/languages/c


*/

#include <stdio.h>


void compress(char * string_to_compress, char * output_buffer, size_t output_buffer_size)
{
    /* 1. Implement */
}

void decompress(char * string_to_decompress, char * output_buffer, size_t output_buffer_size)
{
    /* 2. Implement */
}

int main()
{
    char decompress_this[] = "c3j3d8js3bsj2k4bo3k";
    char compress_this[] = "ytuvwxyghijcdhijklmbcdefklmnopqopqrscdefghicdefghhijhijqrstuvwxmnovghwssthijklmnopfghijklmnrstuqrstuv";
    
    printf("to compress: %s \n", compress_this);
    //Implement line below
    //printf("Compressed output: %s \n", xxxx);

    printf("to decompress: %s \n", decompress_this);
    //Implement line below
    //printf("Decompressed output: %s \n", xxxx );



    return 0;
}

