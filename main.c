#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//                      01
//                   01    01
//                01    02    01
//                    . . .
// 
//       01    05    10    10    05    01
//    01    06    15    20    15    06    01  
// 01    07    21    35    35    21    07    01
//
// - n is the current row (starts at 0)
// - k is the current column (starts at 0)
//
// - N is the index of the last row
// - M is biggest number on last row
// - L is length of numbers
// - s is the spacing between numbers
// - S is the offset at the start of a row
//
// -> (n,k) = (n,k-1) * ((n+1-k)/k)
// -> M = ((N/2)-1) * ((N+1-(N/2)/(N/2))
// -> L = log10(M) + 1 or divide by 10 until we reach 0
// -> s = L + 2
// -> S = (L+1) * (N-n)
//
// https://en.wikipedia.org/wiki/Pascal%27s_triangle

int help() {
    printf( "\033[31mSpecify the number of rows:\n"
            "\033[32m./a.out    10\033[0m\n"
            "./a.out    rows (at least 3)\n");

    return 1;
}

int int_digits(int value) {
    int digits = 1;
    int m = value;
    while (1) {
        m /= 10;
        if (m  == 0) break;
        else digits++;
    }
}

int uint_to_str(int value, char *buffer, int buffer_len) {
    memset(buffer, 0, buffer_len);

    signed char digits[buffer_len];
    memset(digits, -1, buffer_len);

    int digits = int_digits(value);

    if (digits == 1) {
        //TODO
        //return
    }

    int n = 1;
    while (1) {
        // 592
        int value_divisor = n * 10;
        int digit_divisor = (digits - n - 1) * 10; //can be 0
        // 592/10=59 592/100=5 592/1000=0
        // 506/10=50 506/100=5 
        // 5/10=0
        // 50/10=5
        int value_remainder = value / value_divisor;

    }
    
    for (int i=0; i<buffer_len; i++) {
        if (*(buffer+1) == 0) return i + 1;
    }

    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) return help();

    int rows = atoi(argv[1]);
    if (rows <= 2) return help();

    int max_k = (rows - 1) / 2;
    int max = (max_k - 1) * ((rows - max_k) / max_k);
    int digits = int_digits(max);
    int spacing = digits + 2;

    char *row_str; 
    int max_len = 1 + (rows - 1) * digits + spacing * (rows - 1);
    memset(row_str, 0, max_len);
    char *number_str;

    for (int i=0; i<rows; i++) {
        int offset = (spacing + 1) * (rows - 1 - i);
        printf("%s\n", row_str);
    }

    return 0;
}
