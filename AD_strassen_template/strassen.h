#ifndef __STRASSEN__

void strassen_matrix_multiplication(float **C, float const *const *const A,
                                    float const *const *const B, size_t nrow_A, size_t ncol_A, size_t ncol_B);

void sub_matrix_blocks(float **C, 
                   float const *const *const A, 
                   float const *const *const B,
                   const size_t row_start_A, const size_t col_start_A,
                   const size_t row_start_B, const size_t col_start_B,
                   const size_t row_start_C, const size_t col_start_C, 
                   const size_t nrows, const size_t ncols);

void add_matrix_blocks(float **C, 
                   float const *const *const A, 
                   float const *const *const B,
                   const size_t row_start_A, const size_t col_start_A,
                   const size_t row_start_B, const size_t col_start_B,
                   const size_t row_start_C, const size_t col_start_C, 
                   const size_t nrows, const size_t ncols);

void strassen_aux(float **C, 
                   float const *const *const A, 
                   float const *const *const B,
                   const size_t row_start_A, const size_t col_start_A,
                   const size_t row_start_B, const size_t col_start_B,
                   const size_t row_start_C, const size_t col_start_C, const size_t n);

void strassen_aux_generalized(float **C, 
                   float const *const *const A, 
                   float const *const *const B,
                   const size_t row_start_A, const size_t col_start_A, const size_t nrow_A, const size_t ncol_A,
                   const size_t row_start_B, const size_t col_start_B, const size_t nrow_B, const size_t ncol_B,
                   const size_t row_start_C, const size_t col_start_C);


#endif //__STRASSEN__
