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

// Count digits in a positive integer.
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

// Convert single digit integer to character.
char digit_to_char(int digit) {
    return (char) digit + '0';
}

// Put character n times after cursor.
void write_n(char **cursor, char c, int n) {
    for (int i=0; i<n; i++) {
        **cursor = c;
        (*cursor)++;
    }
}

void write_color(char **cursor, int nk) {
    **cursor = '\033';
    *(*cursor + 1) = '[';
    *(*cursor + 2) = '9';
    *(*cursor + 3) = '1' + (char)nk % 7;
    *(*cursor + 4) = 'm';
    *cursor += 5;
}

// End str with 0, print row_str to cursor, reset cursor.
void end_row(char *row_str, char **cursor) {
    **cursor = 0;
    printf("%s\n", row_str);
    *cursor = row_str;
}

int line_offset(int digits, int rows, int n) {
    int mul = rows - 1 - n;
    return digits <= 2 ? digits * mul : digits + (1 + digits / 2) * mul;
}

// Convert positive integer to string.
void uint_to_str(int value, int buffer_len, char *buffer) {
    char *cursor = buffer;
    int digits = uint_digits(value);
    int zeros = buffer_len - digits;

    for (int i=0; i<zeros; i++) {
        *cursor = '0';
        cursor++;
    }

    // 2 -> just push this character and return.
    // 29873/10000=2 -> (29873%10000)/1000=9 -> (29873%1000)/100=8 ...
    // 50/10=5 -> if x/10 push this and next and return.

    if (digits == 1) {
        *cursor = digit_to_char(value);
        return;
    }

    int divisor = int_pow(10, digits - 1);
    int remainder = value / divisor;

    if (digits == 2) {
        *cursor = digit_to_char(remainder);
        *(cursor + 1) = digit_to_char(value % 10);
        return;
    }

    *cursor = digit_to_char(remainder);
    cursor++;

    while (1) {
        int mod = value % divisor;
        divisor /= 10;
        remainder = mod / divisor;

         if (divisor == 10) {
             // Reached the last 2 digits of the number.
             *cursor = digit_to_char(remainder);
             *(cursor + 1) = digit_to_char(value % 10);
             return;
        }else {
            *cursor = digit_to_char(remainder);
            cursor++;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return help();

    int rows = atoi(argv[1]);
    if (rows <= 2) return help();

    int cell_count = (rows * (rows + 1)) / 2;
    int *cells = malloc(cell_count * sizeof(int));
    int *cur_cell = cells;
    *cur_cell = 1;
    cur_cell++;

    for (int n=1; n<rows; n++) {
        for (int k=0; k<n+1; k++) {
            int ul = k == 0 ? 0 : *(cur_cell - n - 1);
            int ur = k == n ? 0 : *(cur_cell - n);
            *cur_cell = ul + ur;
            cur_cell++;
        }
    }

    int max = *(cells + cell_count - 1 - ((rows - 1) / 2));
    int digits = uint_digits(max);
    int spacing = digits < 2 ? digits : (digits & 1 ? 1 : 2);

    // Length of the last line (numbers + spacing + colors + \0)
    int last_len = 2 + ((rows - 1) * digits) + (spacing * (rows - 1)) + (rows * 5);

    char *row_str = (char *) malloc(last_len); 
    char *cursor = row_str;
    char *number_str = (char *) malloc(digits);

    // Write the first line to row_str.
    int offset = line_offset(digits, rows, 0);
    write_color(&cursor, 0); 
    write_n(&cursor, ' ', offset);
    write_n(&cursor, '0', digits - 1);
    *(cursor++) = '1';

    // Print and reset cursor.
    end_row(row_str, &cursor);

    cur_cell = cells + 1;

    // Calculate and print all the other rows.
    for (int n=1; n<rows; n++) {
        // Offset at the start of the line.
        offset = line_offset(digits, rows, n);
        write_n(&cursor, ' ', offset);

        // Calculate and convert to string the current entry.
        int nk = *cur_cell;
        cur_cell++;
        uint_to_str(nk, digits, number_str);
        write_color(&cursor, nk); 

        // Write the first number's characters to row.
        for (int i=0; i<digits; i++) {
            *(cursor++) = *(number_str + i);
        }

        // All entries in the row after the first.
        for (int k=1; k<=n; k++) {
            write_n(&cursor, ' ', spacing);
            
            int nk = *cur_cell;
            cur_cell++;
            uint_to_str(nk, digits, number_str);
            write_color(&cursor, nk); 

            for (int d=0; d<digits; d++) {
                *(cursor++) = *(number_str + d);
            }
        }

        end_row(row_str, &cursor);
    }

    printf("\033[0m");

    // Free allocated string (number_str is included in row_str).
    free(cells);
    free(row_str);

    return 0;
}
