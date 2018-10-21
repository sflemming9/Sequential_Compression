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

static char isLetter(char c);
static char isNumber(char c);
static size_t charToSingleNum(char c);
static size_t charToDoubleNum(char c, char d);
static size_t generate(char c, size_t num, size_t output_buffer_size, int buf_index, char* output_buffer);
static char checkNumValidity(char c, size_t num);
static char emptyString(char* input);
static char singleNumToChar(size_t num);
static void nullTermBuffer(char* output_buffer, size_t output_buffer_size, int buf_index);
static void logError(char* errorMsg);

/* Testing Functions */
static void testCompress();
static void testDecompress();
static void testIsLetter();
static void testIsNumber();
static void testCharToSingleNum();
static void testCharToDoubleNum();
static void testCheckNumValidity();
static void testSingleNumToChar();


void compress(char * string_to_compress, char * output_buffer, size_t output_buffer_size) {

    // Ensure input is not an empty string
    if (emptyString(string_to_compress)) {
        output_buffer[0] = '\0';
        logError("Input string is empty");
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
            logError("Output buffer size too small.");
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
                logError("Output buffer size too small.");
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
                logError("Data is corrupted.");
                return;
            }

            // Input the consecutive letters into the output_buffer
            buf_index = generate(output_buffer[buf_index - 1], num, output_buffer_size, buf_index, output_buffer);
        }
    }

    // Null terminate output buffer
    nullTermBuffer(output_buffer, output_buffer_size, buf_index);
}

/*  This function returns a 1 if inputted char c is a letter, and 0 otherwise. */
static char isLetter(char c) {
    if ((c >= ASCII_a) && (c <= ASCII_z)) return 1;

    return 0;
}

/*  This function returns a 1 if inputted char c is a number (0-9), and 0 otherwise. */
static char isNumber(char c) {
    if ((c >= ASCII_0) && (c <= ASCII_9)) return 1;

    return 0;
}

/*  This function returns the number value of the inputted char c. It is assumed c is between '0'
 *  and '9'. */
static size_t charToSingleNum(char c) {
    return (size_t)(c - ASCII_0);
}

/*  This function returns the two digit number value of the input 'cd'. It is assumed both c
 *  and d are between '0' and '9'. */
static size_t charToDoubleNum(char c, char d) {
    return (size_t) (((c - ASCII_0) * 10) + (d - ASCII_0));
}

/*  This function inputs 'num' sequential letters into the output buffer without exceeding the
 *  output buffer size. The updated buf_index is returned. */
static size_t generate(char c, size_t num, size_t output_buffer_size, int buf_index, char* output_buffer) {
    char seq_ch = c;
    for (size_t i = 0; (i < num) && (buf_index < output_buffer_size); i++) {
        output_buffer[buf_index] = ++seq_ch;
        buf_index++;
    }
    return buf_index;
}

/*  This function returns a 0 for invalid data and a 1 otherwise. Invalid data is characterized
 *  by a number following a letter, where the letter does not have 'num' letters between it
 *  and the end of the alphabet. Ex. c10 is valid, w10 is invalid. */
static char checkNumValidity(char c, size_t num) {
    if ((c + num) > ASCII_z) return 0;

    return 1;
}

/*  This function returns a 1 if the inputted string is empty, and 0 otherwise. */
static char emptyString(char* input) {
    return (input[0] == '\0');
}

/*  This function converts the inputted number to its character ascii value. Ex. turn 2 into '2'. */
static char singleNumToChar(size_t num) {
    return (char)(num + ASCII_0);
}

/*  This function null terminates the output buffer. If the output buffer size is greater than the
 *  current buf_index, the null terminator is inputted at buf_index (end of string). If the
 *  buf_index equals the output buffer size, the last character in the output buffer is overwritten
 *  with a null terminator so as to not exceed the output buffer size. */
static void nullTermBuffer(char* output_buffer, size_t output_buffer_size, int buf_index) {

    // Null terminate output buffer
    if (buf_index == output_buffer_size) {
        output_buffer[buf_index - 1] = '\0';
    } else {
        output_buffer[buf_index] = '\0';
    }
}

/*  This function is used for printing error messages. */
static void logError(char* errorMsg) {
    //printf("Error: %s\n", errorMsg);  /* Commented out for clean execution. */
}

int main() {

    char decompress_this[] = "c3j3d8js3bsj2k4bo3k";
    char compress_this[] = "ytuvwxyghijcdhijklmbcdefklmnopqopqrscdefghicdefghhijhijqrstuvwxmnovghwssthijklmnopfghijklmnrstuqrstuv";

    printf("to compress: %s \n", compress_this);
    //Implement line below
    char compress_result[100];
    compress(compress_this, compress_result, sizeof(compress_result));
    printf("Compressed output: %s \n", compress_result);

    printf("to decompress: %s \n", decompress_this);
    //Implement line below
    char decompress_result[100];
    decompress(decompress_this, decompress_result, sizeof(decompress_result));
    printf("Decompressed output: %s \n", decompress_result);

    /* Testing */
    testCompress();
    testDecompress();
    testIsLetter();
    testIsNumber();
    testCharToSingleNum();
    testCharToDoubleNum();
    testCheckNumValidity();
    testSingleNumToChar();

    return 0;
}

/*  This function tests decompressing strings */
static void testDecompress() {
    // Testing of decompress function
    char test[] = "c3gj4";
    char result[3];     // Buffer size too small
    decompress(test, result, sizeof(result));
    assert(strcmp(result, "cd") == 0);

    char test1[] = "c3gj4";
    char result1[20];
    decompress(test1, result1, sizeof(result1));
    assert(strcmp(result1, "cdefgjklmn") == 0);

    char test2[] = "c20";
    char result2[6];    // Buffer size too small
    decompress(test2, result2, sizeof(result2));
    assert(strcmp(result2, "cdefg") == 0);

    char test3[] = "c20";
    char result3[100];
    decompress(test3, result3, sizeof(result3));
    assert(strcmp(result3, "cdefghijklmnopqrstuvw") == 0);

    char test4[] = "c20j4a8z3";     // Invalid data
    char result4[100];
    decompress(test4, result4, sizeof(result4));

    char test5[] = "casdflkui";
    char result5[7];    // Buffer too small
    decompress(test5, result5, sizeof(result5));
    assert(strcmp(result5, "casdfl") == 0);

    char test6[] = "casdflkui";
    char result6[15];
    decompress(test6, result6, sizeof(result6));
    assert(strcmp(result6, "casdflkui") == 0);

    char test7[] = "w9";    // Invalid data
    char result7[15];
    decompress(test7, result7, sizeof(result7));

    char test8[] = "";  // Empty string
    char result8[15];
    decompress(test8, result8, sizeof(result8));
    assert(strcmp(result8, "") == 0);

    char test9[] = "a3g4j4g4a1ga4gaj";
    char result9[100];
    decompress(test9, result9, sizeof(result9));
    assert(strcmp(result9, "abcdghijkjklmnghijkabgabcdegaj") == 0);
}

/*  This function tests compressing strings */
static void testCompress() {
    // Testing of compress function
    char ctest[] = "abc";
    char cresult[5];
    compress(ctest, cresult, sizeof(cresult));
    assert(strcmp(cresult, "a2") == 0);

    char ctest1[] = "abcjklmn";
    char cresult1[100];
    compress(ctest1, cresult1, sizeof(cresult1));
    assert(strcmp(cresult1, "a2j4") == 0);

    char ctest2[] = "abcjlmnoab";
    char cresult2[100];
    compress(ctest2, cresult2, sizeof(cresult2));
    assert(strcmp(cresult2, "a2jl3a1") == 0);

    char ctest3[] = "abcjlmnoabcdefghijklmnopqrs";
    char cresult3[100];
    compress(ctest3, cresult3, sizeof(cresult3));
    assert(strcmp(cresult3, "a2jl3a18") == 0);

    char ctest4[] = "abcjklmn";
    char cresult4[4];   // Buffer size too small
    compress(ctest4, cresult4, sizeof(cresult4));
    assert(strcmp(cresult4, "a2j") == 0);

    char ctest5[] = "abcjlmnoabcdefghijklmnopqrs";
    char cresult5[8];   // Buffer size too small
    compress(ctest5, cresult5, sizeof(cresult5));
    assert(strcmp(cresult5, "a2jl3a1") == 0);

    char ctest6[] = "aidjencls";
    char cresult6[5];   // Buffer size too small
    compress(ctest6, cresult6, sizeof(cresult6));
    assert(strcmp(cresult6, "aidj") == 0);

    char ctest7[] = "";     // Empty string
    char cresult7[5];
    compress(ctest7, cresult7, sizeof(cresult7));

    char ctest8[] = "abcdghijkjklmnghijkabgabcdegaj";
    char cresult8[100];
    compress(ctest8, cresult8, sizeof(cresult8));
    assert(strcmp(cresult8, "a3g4j4g4a1ga4gaj") == 0);
}

/*  This function is used for testing isLetter */
static void testIsLetter() {

    // Testing of isLetter
    char ch_lower = 'a';
    char ch_upper = 'A';
    for(int i = 0; i < 26; i++) {
        // Test isLetter is correct for all upper and lower case characters
        assert(isLetter(ch_lower) == 1);
        ch_lower++;

        assert(isLetter(ch_upper) == 0);
        ch_upper++;
    }

    char ch = '0';
    assert(isLetter(ch) == 0);

    ch = '9';
    assert(isLetter(ch) == 0);

    ch = '\0';
    assert(isLetter(ch) == 0);

    ch = '-';
    assert(isLetter(ch) == 0);
}

/*  This function is used for testing isNumber */
static void testIsNumber() {
    // Testing of isNumber
    char ch = 'a';
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

}

/*  This function is used for testing charToSingleNum */
static void testCharToSingleNum() {

    // Testing of charToSingleNum
    char num = '0';
    assert(charToSingleNum(num) == 0);

    num = '4';
    assert(charToSingleNum(num) == 4);

    num = '9';
    assert(charToSingleNum(num) == 9);
}

/*  This function is used for testing charToDoubleNum */
static void testCharToDoubleNum() {

    // Testing of charToDoubleNum
    char num = '0';
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
}

/*  This function is used for testing checkNumValidity */
static void testCheckNumValidity() {

    // Testing checkNumValidity
    char ch = 'x';
    int num = 1;
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
}

/*  This function is used for testing singleNumToChar */
static void testSingleNumToChar() {

    // Testing singleNumToChar
    int num = 8;
    assert(singleNumToChar(num) == '8');

    num = 0;
    assert(singleNumToChar(num) == '0');
}
