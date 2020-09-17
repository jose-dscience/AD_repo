#include "matrix.h"
#include "strassen.h"
#include "strassen_optimized.h"


void strassen_matrix_multiplication_op(float **C, 
                                    float const *const *const A,
                                    float const *const *const B, 
                                    size_t nrow_A, size_t ncol_A, size_t ncol_B) 
{
    size_t nrow_B = ncol_A;
    size_t nrow_C = nrow_A;
    size_t ncol_C = ncol_B;
    
    float **D = allocate_matrix(nrow_A, ncol_B); //auxiliary matrix to save temporal values
    
    float ***S = (float***)malloc(sizeof(float **) * 10);
    for(size_t i=0; i<10; i++)
        S[i] = allocate_matrix(nrow_A, ncol_B);
    
    float ***P = (float***)malloc(sizeof(float **) * 7);
    for(size_t i=0; i<7; i++)
        P[i] = allocate_matrix(nrow_A, ncol_B);
    
    strassen_aux_generalized_op(C, A, B,
                                D, S, P,
                             0, 0, nrow_A, ncol_A,
                             0, 0, nrow_B, ncol_B,
                             0, 0);
    
    for(size_t i=0; i<10; i++)
        deallocate_matrix(S[i], nrow_A);
    free(S);
    
    for(size_t i=0; i<7; i++)
        deallocate_matrix(P[i], nrow_A);
    free(P);
    
    deallocate_matrix(D, nrow_A);
}

void strassen_aux_generalized_op(float **C, float const *const *const A, float const *const *const B,
                   float** D, float*** S, float*** P,
                   const size_t row_start_A, const size_t col_start_A, const size_t nrow_A, const size_t ncol_A,
                   const size_t row_start_B, const size_t col_start_B, const size_t nrow_B, const size_t ncol_B,
                   const size_t row_start_C, const size_t col_start_C)
{
    if((nrow_A*ncol_A == 0) || (nrow_B*ncol_B == 0))
        return;
    
    //search size of the maximum square block able to perform a multiplication
    size_t npivot = nrow_A;
    npivot = (npivot > ncol_A)? ncol_A : npivot;
    npivot = (npivot > ncol_B)? ncol_B : npivot;
    
    //Calculation of C11 using pure and gener. strassen algorithm
    strassen_aux_op(C, A, B, S, P, row_start_A, col_start_A, row_start_B, col_start_B, row_start_C, col_start_C, npivot); 
    
    strassen_aux_generalized_op(D, A, B, 
                                D, S, P,
                                row_start_A, col_start_A+npivot, npivot, ncol_A-npivot, //A12
                                row_start_B+npivot, col_start_B, nrow_B-npivot, npivot,  //B21
                                row_start_C, col_start_C);
    
    add_matrix_blocks(C, (const float* const* const)C, (const float* const* const)D,
                           row_start_C, col_start_C,
                           row_start_C, col_start_C,
                           row_start_C, col_start_C,
                           npivot, npivot);
    
    //Calculation of C12 using gener. strassen algorithm
    strassen_aux_generalized_op(C, A, B, 
                                D, S, P,
                                row_start_A, col_start_A, npivot, npivot, //A11
                                row_start_B, col_start_B+npivot, npivot, ncol_B-npivot,  //B12
                                row_start_C, row_start_C+npivot); //C12
    strassen_aux_generalized_op(D, A, B,
                                D, S, P,
                                row_start_A, col_start_A+npivot, npivot, ncol_A-npivot, //A12
                                row_start_B+npivot, col_start_B+npivot, nrow_B-npivot, ncol_B-npivot,  //B22
                                row_start_C, row_start_C+npivot);
    add_matrix_blocks(C, (const float* const* const)C, (const float* const* const)D,
                           row_start_C, col_start_C+npivot,    //C12
                           row_start_C, col_start_C+npivot,
                           row_start_C, col_start_C+npivot,    //C12
                           npivot, ncol_B-npivot);
    
    //Calculation of C21 using gener. strassen algorithm
    
    strassen_aux_generalized_op(C, A, B, 
                                D, S, P,
                                row_start_A+npivot, col_start_A, nrow_A-npivot, npivot, //A21
                                row_start_B, col_start_B, npivot, npivot,  //B11
                                row_start_C+npivot, col_start_C); //C21
    strassen_aux_generalized_op(D, A, B, 
                                D, S, P,
                                row_start_A+npivot, col_start_A+npivot, nrow_A-npivot, ncol_A-npivot, //A22
                                row_start_B+npivot, col_start_B, nrow_B-npivot, npivot,  //B21
                                row_start_C+npivot, col_start_C);
    add_matrix_blocks(C, (const float* const* const)C, (const float* const* const)D,
                           row_start_C+npivot, col_start_C,    //C21
                           row_start_C+npivot, col_start_C,
                           row_start_C+npivot, col_start_C,    //C21
                           nrow_A-npivot, npivot);
    
    //Calculation of C22 using gener. strassen algorithm
    strassen_aux_generalized_op(C, A, B, 
                             D, S, P,
                                row_start_A+npivot, col_start_A, nrow_A-npivot, npivot, //A21
                                row_start_B, col_start_B+npivot, npivot, ncol_B-npivot,  //B12
                                row_start_C+npivot, row_start_C+npivot); //C22
    strassen_aux_generalized_op(D, A, B,
                             D, S, P,
                                row_start_A+npivot, col_start_A+npivot, nrow_A-npivot, ncol_A-npivot, //A22
                                row_start_B+npivot, col_start_B+npivot, nrow_B-npivot, ncol_B-npivot,  //B22
                                row_start_C+npivot, row_start_C+npivot);
    add_matrix_blocks(C, (const float* const* const)C, (const float* const* const)D,
                           row_start_C+npivot, col_start_C+npivot,    //C22
                           row_start_C+npivot, col_start_C+npivot,
                           row_start_C+npivot, col_start_C+npivot,    //C22
                           nrow_A-npivot, ncol_B-npivot);
}

void strassen_aux_op(float** C, float const *const *const A, float const *const *const B,
                   float*** S, float*** P,
                   const size_t row_start_A, const size_t col_start_A,
                   const size_t row_start_B, const size_t col_start_B,
                   const size_t row_start_C, const size_t col_start_C, const size_t n)
{
    if(n <= 1)
    {
        naive_aux(C, A, B, row_start_A, col_start_A, row_start_B, col_start_B, row_start_C, col_start_C, n, n, n);
        return;
    }
    
    size_t n2 = n/2;
    
    //S1 = B12 - B22
    sub_matrix_blocks(S[0], B, B,
                   row_start_B, col_start_B+n2,
                   row_start_B+n2, col_start_B+n2,
                   0, 0, 
                   n2, n2);
    
    //P1 = A11 x S1
    strassen_aux_op(P[0], A, (const float* const* const)S[0],
                   S, P,
                   row_start_A, col_start_A,
                   0, 0,
                   0, 0, 
                   n2);
    
    //S2 = A11 + A12
    add_matrix_blocks(S[1], A, A,
                   row_start_A, col_start_A,
                   row_start_A, col_start_A+n2,
                   0, 0, 
                   n2, n2);
    
    //P2 = S2 X B22
    strassen_aux_op(P[1], (const float* const* const)S[1], B,
                   S, P,
                   0, 0,
                   row_start_B+n2, col_start_B+n2,
                   0, 0, 
                   n2);
    
    //S3 = A21 + A22
    add_matrix_blocks(S[2], A, A,
                   row_start_A+n2, col_start_A,
                   row_start_A+n2, col_start_A+n2,
                   0, 0, 
                   n2, n2);
    
    //P3 = S3 X B11
    strassen_aux_op(P[2], (const float* const* const)S[2], B,
                   S, P,
                   0, 0,
                   row_start_B, col_start_B,
                   0, 0, 
                   n2);
    
    //S4 = B21 - B11
    sub_matrix_blocks(S[3], B, B,
                   row_start_B+n2, col_start_B,
                   row_start_B, col_start_B,
                   0, 0, 
                   n2, n2);
    
    //P4 = A22 X S4
    strassen_aux_op(P[3], A, (const float* const* const)S[3],
                   S, P,
                   row_start_A+n2, col_start_A+n2,
                   0, 0,
                   0, 0, 
                   n2);
    
    //S5 = A11 + A22
    add_matrix_blocks(S[4], A, A,
                   row_start_A, col_start_A,
                   row_start_A+n2, col_start_A+n2,
                   0, 0,
                   n2, n2);
    
    //S6 = B11 + B22
    add_matrix_blocks(S[5], B, B,
                   row_start_B, col_start_B,
                   row_start_B+n2, col_start_B+n2,
                   0, 0, 
                   n2, n2);
    
    //P5 = S5 X S6
    strassen_aux_op(P[4], (const float* const* const)S[4], (const float* const* const)S[5],
                   S, P,
                   0, 0,
                   0, 0,
                   0, 0, 
                   n2);
    
    //S7 = A12 - A22
    sub_matrix_blocks(S[6], A, A,
                   row_start_A, col_start_A+n2,
                   row_start_A+n2, col_start_A+n2,
                   0, 0, 
                   n2, n2);
    
    //S8 = B21 + B22
    add_matrix_blocks(S[7], B, B,
                   row_start_B+n2, col_start_B+0,
                   row_start_B+n2, col_start_B+n2,
                   0, 0, 
                   n2, n2);
    
    //P6 = S7 X S8
    strassen_aux_op(P[5], (const float* const* const)S[6], (const float* const* const)S[7],
                   S, P,
                   0, 0,
                   0, 0,
                   0, 0, 
                   n2);
    
    //S9 = A11 - A21
    sub_matrix_blocks(S[8], A, A,
                   row_start_A, col_start_A,
                   row_start_A+n2, col_start_A,
                   0, 0, 
                   n2, n2);
    
    //S10 = B11 + B12
    add_matrix_blocks(S[9], B, B,
                   row_start_B, col_start_B,
                   row_start_B, col_start_B+n2,
                   0, 0, 
                   n2, n2);
    
    //P7 = S9 X S10
    strassen_aux_op(P[6], (const float* const* const)S[8], (const float* const* const)S[9],
                   S, P,
                   0, 0,
                   0, 0,
                   0, 0, 
                   n2);
    
    //C11 = P5 + P4 - P2 + P6
    add_matrix_blocks(C, (const float* const* const)P[4], (const float* const* const)P[3],
                   0, 0,
                   0, 0,
                   row_start_C, col_start_C, 
                   n2, n2);
    
    sub_matrix_blocks(C, (const float* const* const)C, (const float* const* const)P[1],
                   row_start_C+0, col_start_C+0,
                   0, 0,
                   row_start_C+0, col_start_C+0, 
                   n2, n2);
    
    add_matrix_blocks(C, (const float* const* const)C, (const float* const* const)P[5],
                   row_start_C+0, col_start_C+0,
                   0, 0,
                   row_start_C+0, col_start_C+0, 
                   n2, n2);
    
    //C12 = P1 + P2
    add_matrix_blocks(C, (const float* const* const)P[0], (const float* const* const)P[1],
                   0, 0,
                   0, 0,
                   row_start_C+0, col_start_C+n2, 
                   n2, n2);
    
    //C21 = P3 + P4
    add_matrix_blocks(C, (const float* const* const)P[2], (const float* const* const)P[3],
                   0, 0,
                   0, 0,
                   row_start_C+n2, col_start_C+0, 
                   n2, n2);
    
    //C22 = P5 + P1 - P3 - P7
    add_matrix_blocks(C, (const float* const* const)P[4], (const float* const* const)P[0],
                   0, 0,
                   0, 0,
                   row_start_C+n2, col_start_C+n2, 
                   n2, n2);
    
    sub_matrix_blocks(C, (const float* const* const)C, (const float* const* const)P[2],
                   row_start_C+n2, col_start_C+n2,
                   0, 0,
                   row_start_C+n2, col_start_C+n2, 
                   n2, n2);
    
    sub_matrix_blocks(C, (const float* const* const)C, (const float* const* const)P[6],
                   row_start_C+n2, col_start_C+n2,
                   0, 0,
                   row_start_C+n2, col_start_C+n2, 
                   n2, n2);
}
