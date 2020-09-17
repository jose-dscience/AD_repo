#ifndef __MATRIX__
#include <stdlib.h>

void naive_aux(float **C, 
                float const *const *const A, 
                float const *const *const B,
                const size_t row_start_A, const size_t col_start_A,
                const size_t row_start_B, const size_t col_start_B,
                const size_t row_start_C, const size_t col_start_C, 
                const size_t nrow_A, const size_t ncol_A, const size_t ncol_B);

void naive_matrix_multiplication(float **C, float const *const *const A,
                                float const *const *const B,
                                const size_t nrow_A, const size_t ncol_A, const size_t ncol_B);

int same_matrix(float const *const *const A, float const *const *const B,
                const size_t rows, const size_t cols);

float **allocate_matrix(const size_t rows, const size_t cols);
void deallocate_matrix(float **A, const size_t rows);


float **allocate_random_matrix(const size_t rows, const size_t cols);
#endif //__MATRIX__
