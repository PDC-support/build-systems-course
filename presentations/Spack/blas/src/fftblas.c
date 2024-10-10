#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fftw3.h>
#include <cblas.h>

#define N0 1000  // Number of rows
#define N1 100   // Number of columns

int main() {
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Allocate memory for two complex input arrays
    fftw_complex *input1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N0 * N1);
    fftw_complex *input2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N0 * N1);

    if (input1 == NULL || input2 == NULL) {
        fprintf(stderr, "Error allocating memory for input arrays.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the input arrays with random numbers between 0.0 and 1.0
    for (long i = 0; i < N0 * N1; i++) {
        input1[i][0] = (double)rand() / RAND_MAX; // Real part
        input1[i][1] = (double)rand() / RAND_MAX; // Imaginary part

        input2[i][0] = (double)rand() / RAND_MAX; // Real part
        input2[i][1] = (double)rand() / RAND_MAX; // Imaginary part
    }

    // Allocate memory for the FFT output arrays
    fftw_complex *fft1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N0 * N1);
    fftw_complex *fft2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N0 * N1);

    if (fft1 == NULL || fft2 == NULL) {
        fprintf(stderr, "Error allocating memory for FFT output arrays.\n");
        fftw_free(input1);
        fftw_free(input2);
        exit(EXIT_FAILURE);
    }

    // Create FFTW plans for 2D complex-to-complex FFTs
    fftw_plan plan1 = fftw_plan_dft_2d(N0, N1, input1, fft1, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan plan2 = fftw_plan_dft_2d(N0, N1, input2, fft2, FFTW_FORWARD, FFTW_ESTIMATE);

    if (plan1 == NULL || plan2 == NULL) {
        fprintf(stderr, "Error creating FFTW plans.\n");
        fftw_free(input1);
        fftw_free(input2);
        fftw_free(fft1);
        fftw_free(fft2);
        exit(EXIT_FAILURE);
    }

    // Execute the FFTs
    fftw_execute(plan1);
    fftw_execute(plan2);

    // Allocate memory for the result FFT
    fftw_complex *fft_result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N0 * N1);
    if (fft_result == NULL) {
        fprintf(stderr, "Error allocating memory for FFT result array.\n");
        fftw_free(input1);
        fftw_free(input2);
        fftw_free(fft1);
        fftw_free(fft2);
        fftw_destroy_plan(plan1);
        fftw_destroy_plan(plan2);
        exit(EXIT_FAILURE);
    }

    // Initialize fft_result to zero
    for (long i = 0; i < N0 * N1; i++) {
        fft_result[i][0] = 0.0; // Real part
        fft_result[i][1] = 0.0; // Imaginary part
    }

    // Perform matrix multiplication using BLAS
    // fft1 is treated as matrix A (N0 x N1)
    // fft2 is treated as matrix B (N1 x N1)
    // Result fft_result will be matrix C (N0 x N1)

    // For matrix multiplication A (N0 x N1) * B (N1 x N1) = C (N0 x N1)
    // Ensure that fft2 is appropriately sized. In this example, we treat it as square for simplicity.

    // Define alpha and beta as complex numbers
    double alpha[2] = {1.0, 0.0}; // alpha = 1 + 0i
    double beta[2] = {0.0, 0.0};  // beta = 0 + 0i

    // Perform the multiplication
    // CBLAS expects the matrices to be in row-major order if CblasRowMajor is specified
    // Each complex number is represented by two doubles: {real, imag}

    // Set dimensions
    int M = N0;    // Rows of A and C
    int K = N1;    // Columns of A and rows of B
    int N = N1;    // Columns of B and C

    // Leading dimensions
    int lda = K;    // Leading dimension of A
    int ldb = N;    // Leading dimension of B
    int ldc = N;    // Leading dimension of C

    // Perform C = alpha * A * B + beta * C
    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                M, N, K,
                alpha, (const void*)fft1, lda,
                (const void*)fft2, ldb,
                beta, (void*)fft_result, ldc);

    // (Optional) Print a few elements of the result for verification
    printf("Sample of FFT Result Matrix Multiplication:\n");
    for (int i = 0; i < 5 && i < N0; i++) {
        for (int j = 0; j < 5 && j < N1; j++) {
            printf("fft_result[%d][%d] = %f + %fi\n", i, j,
                   fft_result[i * N1 + j][0],
                   fft_result[i * N1 + j][1]);
        }
    }

    // Clean up: Free allocated memory and destroy FFTW plans
    fftw_destroy_plan(plan1);
    fftw_destroy_plan(plan2);
    fftw_free(input1);
    fftw_free(input2);
    fftw_free(fft1);
    fftw_free(fft2);
    fftw_free(fft_result);

    return 0;
}
