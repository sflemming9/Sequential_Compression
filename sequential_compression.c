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

Author: Sabrina Flemming

Assumptions:
- lower case letters only
- starting with a letter (not a number)
- input string is null terminated
- output_buffer should be null terminated before function return
- alphabet does not wrap around; begins with 'a' and ends with 'z' ex. input data such as
z10 is invalid

*/

#include <stdio.h>

/*  Libraries included for testing purposes */
#include <string.h>
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
static size_t checkNumValidity(char c, size_t num);
static size_t emptyString(char* input);
static char singleNumToChar(size_t num);
static void nullTermBuffer(char* output_buffer, size_t output_buffer_size, int buf_index);


void compress(char * string_to_compress, char * output_buffer, size_t output_buffer_size) {

    // Ensure input is not an empty string
    if (emptyString(string_to_compress)) {
        output_buffer[0] = '\0';
        printf("Error: Input string is empty.\n");
        return;
    }

    size_t buf_index = 0;
    size_t comp_index = 0;

    // Continue until buf_index = (output_buffer_size - 1); need room for null-terminator
    while (buf_index < (output_buffer_size - 1)) {

        char curr = string_to_compress[comp_index];
        comp_index++;

        if (curr == '\0') break;    // Break if we have reached the end of the input

        output_buffer[buf_index] = curr;
        buf_index++;

        char next = string_to_compress[comp_index];
        size_t count = 0;

        // While consequtive letters are found, increase count and continue to progress
        while (next == (curr + 1)) {
            count++;
            comp_index++;
            curr = next;
            next = string_to_compress[comp_index];
        }

        if (buf_index >= output_buffer_size) {
            printf("Error: output buffer size too small.\n");
            break;        
        }

        // If count is > 0, it can be a single or double digit
        if ((count > 0) && (count < 10)) {
            output_buffer[buf_index] = singleNumToChar(count);
            buf_index++;
        } else if (count >= 10) {
            output_buffer[buf_index] = singleNumToChar(count / 10);
            buf_index++;
            if (buf_index >= (output_buffer_size - 1)) {
                printf("Error: output buffer size too small.\n");
                break;
            }
            output_buffer[buf_index] = singleNumToChar(count % 10);
            buf_index++;
        }
    }

    // Null terminate output buffer
    nullTermBuffer(output_buffer, output_buffer_size, buf_index);
}

void decompress(char * string_to_decompress, char * output_buffer, size_t output_buffer_size) {

    size_t buf_index = 0;
    size_t decomp_index = 0;

    // Continue until buf_index = (output_buffer_size - 1); need room for null-terminator
    while (buf_index < (output_buffer_size - 1)) {

        char curr = string_to_decompress[decomp_index];
        decomp_index++;

        if (curr == '\0') break;

        // Check if curr is a letter or a number (number could be single or double digit)
        if (isLetter(curr)) {
            output_buffer[buf_index] = curr;
            buf_index++;
        } else if (isNumber(curr)) {
            char next = string_to_decompress[decomp_index];
            size_t num;

            // Check if next is a letter or a number; if a number, then the total number is double
            // digits
            if (isNumber(next)) {
                decomp_index++;
                num = charToDoubleNum(curr, next);
            } else {
                num = charToSingleNum(curr);
            }

            // Ensure data is valid
            if (checkNumValidity(output_buffer[buf_index - 1], num) == 0) {
                output_buffer[buf_index] = '\0';
                printf("Error: Data is corrupted.\n");
                return;
            }

            // Input the consequtive letters into the output_buffer
            buf_index = generate(output_buffer[buf_index - 1], num, output_buffer_size, buf_index, output_buffer);
        }
    }

    // Null terminate output buffer
    nullTermBuffer(output_buffer, output_buffer_size, buf_index);
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
    return (size_t)(c - ASCII_0);
}

static size_t charToDoubleNum(char c, char d) {
    return (size_t) (((c - ASCII_0) * 10) + (d - ASCII_0));
}

static size_t generate(char c, size_t num, size_t output_buffer_size, int buf_index, char* output_buffer) {
    char seq_ch = c;
    for (size_t i = 0; (i < num) && (buf_index < output_buffer_size); i++) {
        output_buffer[buf_index] = ++seq_ch;
        buf_index++;
    }
    return buf_index;
}

static size_t checkNumValidity(char c, size_t num) {
    if ((c + num) > ASCII_z) return 0;

    return 1;
}

static size_t emptyString(char* input) {
    return (input[0] == '\0');
}

static char singleNumToChar(size_t num) {
    return (char)(num + ASCII_0);
}

static void nullTermBuffer(char* output_buffer, size_t output_buffer_size, int buf_index) {

    // Null terminate output buffer
    if (buf_index == output_buffer_size) {
        output_buffer[buf_index - 1] = '\0';
    } else {
        output_buffer[buf_index] = '\0';
    }
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

    // Testing of compress function
    char ctest[] = "abc";
    char cresult[5];
    compress(ctest, cresult, sizeof(cresult));
    assert(strcmp(cresult, "a2") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest, cresult, sizeof(cresult));

    char ctest1[] = "abcjklmn";
    char cresult1[100];
    compress(ctest1, cresult1, sizeof(cresult1));
//    printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest1, cresult1, sizeof(cresult1));
    assert(strcmp(cresult1, "a2j4") == 0);

    char ctest2[] = "abcjlmnoab";
    char cresult2[100];
    compress(ctest2, cresult2, sizeof(cresult2));
    assert(strcmp(cresult2, "a2jl3a1") == 0);
//    printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest2, cresult2, sizeof(cresult2));

    char ctest3[] = "abcjlmnoabcdefghijklmnopqrs";
    char cresult3[100];
    compress(ctest3, cresult3, sizeof(cresult3));
    assert(strcmp(cresult3, "a2jl3a18") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest3, cresult3, sizeof(cresult3));

    char ctest4[] = "abcjklmn";
    char cresult4[4];
    compress(ctest4, cresult4, sizeof(cresult4));
    assert(strcmp(cresult4, "a2j") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest4, cresult4, sizeof(cresult4));

    char ctest5[] = "abcjlmnoabcdefghijklmnopqrs";
    char cresult5[8];
    compress(ctest5, cresult5, sizeof(cresult5));
    assert(strcmp(cresult5, "a2jl3a1") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest5, cresult5, sizeof(cresult5));

    char ctest6[] = "aidjencls";
    char cresult6[5];
    compress(ctest6, cresult6, sizeof(cresult6));
    assert(strcmp(cresult6, "aidj") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest6, cresult6, sizeof(cresult6));

    char ctest7[] = "";
    char cresult7[5];
    compress(ctest7, cresult7, sizeof(cresult7));
//    printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", ctest7, cresult7, sizeof(cresult7));

    // Testing of decompress function
    char test[] = "c3gj4";
    char result[3];
    decompress(test, result, sizeof(result));
//    printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test, result, sizeof(result));
    assert(strcmp(result, "cd") == 0);

    char test1[] = "c3gj4";
    char result1[20];
    decompress(test1, result1, sizeof(result1));
    assert(strcmp(result1, "cdefgjklmn") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test1, result1, sizeof(result1));

    char test2[] = "c20";
    char result2[6];
    decompress(test2, result2, sizeof(result2));
    assert(strcmp(result2, "cdefg") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test2, result2, sizeof(result2));

    char test3[] = "c20";
    char result3[100];
    decompress(test3, result3, sizeof(result3));
    assert(strcmp(result3, "cdefghijklmnopqrstuvw") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test3, result3, sizeof(result3));

    char test4[] = "c20j4a8z3";
    char result4[100];
    decompress(test4, result4, sizeof(result4));
//    printf("Testtest4= result4,, Result = %s, sizeof(result) = %zu\n\n", test4, result4, sizeof(result4));

    char test5[] = "casdflkui";
    char result5[7];
    decompress(test5, result5, sizeof(result5));
    assert(strcmp(result5, "casdfl") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test5, result5, sizeof(result5));

    char test6[] = "casdflkui";
    char result6[15];
    decompress(test6, result6, sizeof(result6));
    assert(strcmp(result6, "casdflkui") == 0);
    //printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test6, result6, sizeof(result6));

    char test7[] = "w9";
    char result7[15];
    decompress(test7, result7, sizeof(result7));
//    printf("Test = result7,, Result = %s, sizeof(result) = %zu\n\n", test7, result7, sizeof(result7));

    char test8[] = "";
    char result8[15];
    decompress(test8, result8, sizeof(result8));
    assert(strcmp(result8, "") == 0);
//    printf("Test = %s, Result = %s, sizeof(result) = %zu\n\n", test8, result8, sizeof(result8));

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

    // Testing checkNumValidity
    ch = 'x';
    num = 1;
    assert(checkNumValidity(ch, num) == 1);

    ch = 'x';
    num = 10;
    assert(checkNumValidity(ch, num) == 0);

    ch = 'a';
    num = 100;
    assert(checkNumValidity(ch, num) == 0);

    ch = 'a';
    num = 10;
    assert(checkNumValidity(ch, num) == 1);

    ch = 'z';
    num = 1;
    assert(checkNumValidity(ch, num) == 0);

    printf("checkNumValidity unit tests passed.\n");

    // Testing singleNumToChar
    num = 8;
    assert(singleNumToChar(num) == '8');

    num = 0;
    assert(singleNumToChar(num) == '0');

    printf("singleNumToChar unit tests passed.\n");

    return 0;
}


