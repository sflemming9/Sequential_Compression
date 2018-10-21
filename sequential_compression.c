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
- starting with a letter (for decompress)
- output_buffer should be null terminated before function return
- alphabet does not wrap around; begins with 'a' and ends with 'z'


*/

#include <stdio.h>
#include <assert.h>

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

    size_t buf_index = 0;
    size_t decomp_index = 0;

    while (buf_index < (output_buffer_size - 1)) {

        char curr = string_to_decompress[decomp_index];
        //printf("curr: %c\n", curr);
        decomp_index++;

        if (curr == '\0') break; // could be return?

        if (isLetter(curr)) {
            output_buffer[buf_index] = curr;
            buf_index++;
        } else if (isNumber(curr)) {

            char next = string_to_decompress[decomp_index];
            //printf("next char: %c\n", next);
            size_t num;

            if (isNumber(next)) {
                decomp_index++;
                num = charToDoubleNum(curr, next);
                // generate with the last inputted value in output_buffer as first letter and (curr*10 + next) as number of times
                //buf_index = generate(output_buffer[buf_index - 1], charToDoubleNum(curr, next), output_buffer_size, buf_index, output_buffer);
            } else {
                num = charToSingleNum(curr);
                // generate with the last inputted value in output_buffer as first letter and curr as number of times
                //buf_index = generate(output_buffer[buf_index - 1], charToSingleNum(curr), output_buffer_size, buf_index, output_buffer);
            }

            if (checkNumValidity(output_buffer[buf_index - 1], num) == 0) {
                printf("Error: Data is corrupted.\n");
                return;
            }

            buf_index = generate(output_buffer[buf_index - 1], num, output_buffer_size, buf_index, output_buffer);

        }
    }

    // Null terminate output buffer
    if (buf_index == output_buffer_size) {
        output_buffer[buf_index - 1] = '\0';
    } else {
        output_buffer[buf_index] = '\0';
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

static size_t charToSingleNum(char c) {
    return (size_t)(c - 0x30); 
}

static size_t charToDoubleNum(char c, char d) {
    return (size_t) (((c - 0x30) * 10) + (d - 0x30));
}

static size_t generate(char c, size_t num, size_t output_buffer_size, int buf_index, char* output_buffer) {
    char seq_ch = c;
    for (size_t i = 0; (i < num) && (buf_index < output_buffer_size); i++) {
        output_buffer[buf_index] = ++seq_ch;
        buf_index++;
    }
    printf("output in generate: %s\n", output_buffer);
    return buf_index;
    
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
    
    /* Testing */
    char test[] = "c3gj4";
    char result[20];
    decompress(test, result, sizeof(result));
    printf("Result = %s\n", result);

    // Testing of isLetter
    char ch = 'a';
    assert(isLetter(ch) == 1);

    ch = 'z';
    assert(isLetter(ch) == 1);

    ch = 'A';
    assert(isLetter(ch) == 0);

    ch = 'Z';
    assert(isLetter(ch) == 0);

    ch = '0';
    assert(isLetter(ch) == 0);

    ch = '9';
    assert(isLetter(ch) == 0);

    ch = '\0';
    assert(isLetter(ch) == 0);

    ch = '-';
    assert(isLetter(ch) == 0);

    printf("isLetter unit tests passed.\n");

    // Testing of isNumber
    ch = 'a';
    assert(isNumber(ch) == 0);

    ch = 'z';
    assert(isNumber(ch) == 0);

    ch = 'A';
    assert(isNumber(ch) == 0);

    ch = 'Z';
    assert(isNumber(ch) == 0);

    ch = '0';
    assert(isNumber(ch) == 1);

    ch = '9';
    assert(isNumber(ch) == 1);

    ch = '\0';
    assert(isNumber(ch) == 0);

    ch = '-';
    assert(isNumber(ch) == 0);

    printf("isNumber unit tests passed.\n");

    // Testing of charToSingleNum
    char num = '0';
    assert(charToSingleNum(num) == 0);

    num = '4';
    assert(charToSingleNum(num) == 4);

    num = '9';
    assert(charToSingleNum(num) == 9);

    printf("charToSingleNum unit tests passed.\n");

    // Testing of charToDoubleNum
    num = '0';
    char num2 = '0';
    assert(charToDoubleNum(num, num2) == 0);

    num = '0';
    num2 = '3';
    assert(charToDoubleNum(num, num2) == 3);

    num = '1';
    num2 = '0';
    assert(charToDoubleNum(num, num2) == 10);

    num = '9';
    num2 = '9';
    assert(charToDoubleNum(num, num2) == 99);

    num = '9';
    num2 = '0';
    assert(charToDoubleNum(num, num2) == 90);

    printf("charToDoubleNum unit tests passed.\n");
    return 0;
}

