#ifndef __TEST__


double test(void (*f)(float **,
	                  float const *const *const,
	                  float const *const *const,
	                  size_t, size_t, size_t), 
	        float **C, float** A, float **B, size_t nrow_A, size_t ncol_A, size_t ncol_B);

#endif // __TEST__
