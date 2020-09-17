#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

void naive_aux(float **C, 
                float const *const *const A, 
                float const *const *const B,
                const size_t row_start_A, const size_t col_start_A,
                const size_t row_start_B, const size_t col_start_B,
                const size_t row_start_C, const size_t col_start_C, 
                const size_t nrow_A, const size_t ncol_A, const size_t ncol_B)
{
    const size_t nrow_B = ncol_A;
    const size_t nrow_C = nrow_A;
    const size_t ncol_C = ncol_B;
    
    for (size_t i=0; i<nrow_C; i++)
    {
        for (size_t j=0; j<ncol_C; j++)
        {
            double sum = 0.0;
            for (size_t k=0; k<ncol_A; k++)
            {
                sum += 
                A[i+row_start_A][k+col_start_A]*
                B[k+row_start_B][j+col_start_B];
            }
            C[i+row_start_C][j+col_start_C] = sum;
        }
    }
}


void naive_matrix_multiplication(float **C, float const *const *const A,
                                float const *const *const B,
                                const size_t nrow_A, const size_t ncol_A, const size_t ncol_B) 
{
    naive_aux(C, A, B, 0, 0, 0, 0, 0, 0, nrow_A, ncol_A, ncol_B);
}

int same_matrix(float const *const *const A, float const *const *const B,
                const size_t rows, const size_t cols) {
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      if (A[i][j] != B[i][j]) {
        return 0;
      }
    }
  }

  return 1;
}

float **allocate_matrix(const size_t rows, const size_t cols) {
  float **M = (float **)calloc(rows, sizeof(float *));

  for (size_t i = 0; i < rows; i++) {
    M[i] = (float *)calloc(cols, sizeof(float));
  }

  return M;
}

void deallocate_matrix(float **A, const size_t rows) {
  for (size_t i = 0; i < rows; i++) {
    free(A[i]);
  }

  free(A);
}

float **allocate_random_matrix(const size_t rows, const size_t cols) {
  
  float **A = allocate_matrix(rows, cols);
  
  srand(10);
  for (size_t i = 0; i < rows; i++) {
    for (size_t j = 0; j < cols; j++) {
      A[i][j] = (rand() - RAND_MAX / 2) % 5;
    }
  }

  return A;
}

