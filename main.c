//                      01
//                   01    01
//                01    02    01
//                    . . .
// 
//       01    05    10    10    05    01
//    01    06    15    20    15    06    01  
// 01    07    21    35    35    21    07    01
//
// - n is the current row
// - k is the current column
//
// - N is the last row
// - M is biggest number on last row
// - L is length of numbers
// - s is the spacing between numbers
// - S is the offset at the start of a row
//
// -> M = ((N/2)-1) * ((N+1-(N/2)/(N/2))
// -> L = log10(M) + 1 or divide by 10 until we reach 0
// -> s = L + 2
// -> S = (L+1) * (N-n)
//
// https://en.wikipedia.org/wiki/Pascal%27s_triangle#Calculating_a_row_or_diagonal_by_itself
