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
a3 g4 j4 g4 a1 g a4 g a j

visual aid:
a[bcd] g[hijk] j[klmn] g[hijk] a[b] g a[bcde] g a j

Decompression is the inverse of the above.

Please implement: 
1. The compress function
2. The decompress function

Use gcc on your machine or an online c tool such as:
https://repl.it/languages/c

Assumptions:
- lower case letters only
- starting with a letter


*/

#include <stdio.h>

#define ASCII_a 0x61
#define ASCII_z 0x7a
#define ASCII_0 0x30
#define ASCII_9 0x39

static unsigned int isLetter(char c);
static unsigned int isNumber(char c);
static size_t charToSingleNum(char c);
static size_t charToDoubleNum(char c, char d);
static size_t generate(char c, size_t num, size_t output_buffer_size, int buf_index, char* output_buffer);


void compress(char * string_to_compress, char * output_buffer, size_t output_buffer_size) {
    /* 1. Implement */
}

void decompress(char * string_to_decompress, char * output_buffer, size_t output_buffer_size) {
    /* 2. Implement */

    int buf_index = 0;
    int decomp_index = 0;


    while (buf_index < output_buffer_size) {

        char curr = string_to_decompress[decomp_index];
        decomp_index++;
        
        if (curr == '\0') break; // could be return?

        if (isLetter(curr)) {
            output_buffer[buf_index] = curr;
            buf_index++;
        } else if (isNumber(curr)) {
            
            decomp_index++;
            char next = string_to_decompress[decomp_index];

            if (isLetter(next)) {
                // generate with the last inputted value in output_buffer as first letter and curr as number of times
            } else if (isNumber(next)) {
                // generate with the last inputted value in output_buffer as first letter and (curr*10 + next) as number of times 
            }

            if (next == '\0') break; // could be return?




        }

    }

    // TODO Never go past the length of the buffer
    //      always stop at the null terminator
    //      empty string?
    //      add '\0' to end of output buffer
}

static unsigned int isLetter(char c) {
    if ((c >= ASCII_a) && (c <= ASCII_z)) return 1;

    return 0;
}

static unsigned int isNumber(char c) {
    if ((c >= ASCII_0) && (c <= ASCII_9)) return 1;

    return 0;
}

static unsigned int charToSingleNum(char c) {
    return (unsigned int)(c - 30); 
}

static unsigned int charToDoubleNum(char c, char d) {
    return (unsigned int) (((c - 30) * 10) + (d - 30));
}

static void generate(char c, int num) {
    
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

