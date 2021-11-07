#include <stdio.h>
#include <stdlib.h>

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

int int_pow(int value, int power) {
    //if (power == 0) return 1;
    int p = value;
    for (int i=2; i<=power; i++) {
        p *= value; 
    }
    return p;
}

int uint_digits(int value) {
    //if (value == 0) return 1;
    int digits = 1;
    int m = value;
    while (1) {
        m /= 10;
        if (m  == 0) return digits;
        else digits++;
    }
}

char digit_to_char(int digit) {
    return (char) digit + 48; //TODO test with '0'
}

void uint_to_str(int value, int buffer_len, char *buffer) {
    int digits = uint_digits(value);
    int zeros = buffer_len - digits;

    for (int i=0; i<zeros; i++) {
        *(buffer + i) = 48;
    }

    char *buf_end = buffer + buffer_len;

    // 2 -> just push this character and return.
    // 29873/10000=2 -> (29873%10000)/1000=9 -> (29873%1000)/100=8 ...
    // 50/10=5 -> if x/10 push this and next and return.

    if (digits == 1) {
        *(buf_end - 1) = digit_to_char(value);
        return;
    }

    int divisor = int_pow(10, digits - 1);
    int remainder = value / divisor;

    if (divisor == 10) {
        // Two digits.
        *(buf_end - 2) = digit_to_char(remainder);
        *(buf_end - 1) = digit_to_char(value % 10);
        return;
    }

    *(buf_end - digits) = digit_to_char(remainder);
    int c = 1;

    while (1) {
        int mod = value % divisor;
        divisor /= 10;
        remainder = mod / divisor;

         if (divisor == 10) {
             // Reached the last 2 digits of the number.
             *(buf_end - digits + c++) = digit_to_char(remainder);
             *(buf_end - digits + c++) = digit_to_char(value % 10);
             return;
        }else {
            *(buf_end - digits + c++) = digit_to_char(remainder);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return help();

    int rows = atoi(argv[1]);
    if (rows <= 2) return help();

    int max_k = (rows - 1) / 2;
    int max = 1;
    for (int k=1; k<=max_k; k++) {
        max = (int)(max * ((float)(rows - k) / k));
    }

    int digits = uint_digits(max);
    int spacing = digits + 2;

    // Length of the last line + 1
    int last_len = 2 + (rows - 1) * digits + spacing * (rows - 1);

    char *row_str = (char *) malloc(last_len); 
    char *number_str = (char *) malloc(digits);

    printf("last_len: %d\n", last_len);

    // Write the first line to row_str.
    int offset = (spacing + 1) * (rows - 1);
    for (int i=0; i<offset; i++) *(row_str + i) = ' ';
    for (int i=0; i<digits-1; i++) *(row_str + offset + i) = '0';
    *(row_str + offset + digits - 1) = '1';
    *(row_str + offset + digits) = 0;

    printf("%s\n", row_str);

    for (int n=1; n<rows; n++) {
        int offset = (spacing + 1) * (rows - 1 - n);
        int nk = 12;
        uint_to_str(nk, digits, number_str);

        for (int i=0; i<offset; i++) *(row_str + i) = ' ';
        for (int i=0; i<digits; i++) *(row_str + offset + i) = *(number_str + i);
        *(row_str + offset + digits) = 0;

        printf("%s\n", row_str);
    }


    //TODO free?

    return 0;
}
