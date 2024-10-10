cmake_minimum_required(VERSION 3.16)

# Project name and language
project(FFTBLAS C)

# Enable C99 standard (optional but recommended)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Find pkg-config to locate FFTW3
find_package(PkgConfig REQUIRED)

# Find FFTW3 using pkg-config
pkg_check_modules(FFTW REQUIRED fftw3)

# Find BLAS using CMake's FindBLAS module
find_package(BLAS REQUIRED)

# Include directories for FFTW3
include_directories(${FFTW_INCLUDE_DIRS})

# Add executable target
add_executable(fftblas fftblas.c)

# Link FFTW3 + BLAS
target_link_libraries(fftblas 
    ${FFTW_LIBRARIES} 
    ${BLAS_LIBRARIES} 
)
