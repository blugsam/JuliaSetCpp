#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>

#define CUDA_CHECK(call) { gpuAssert((call), __FILE__, __LINE__); }

#define BLOCK_SIZE 16
#define REAL_MIN -1.5f
#define REAL_MAX 1.5f
#define IMAG_MIN -1.5f
#define IMAG_MAX 1.5f
#define MAX_ITER 300

inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "CUDA Error: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}

__device__ int julia(float x, float y, float cx, float cy)
{
    int i = 0;
    while (i < MAX_ITER && x * x + y * y < 4.0f)
    {
        float xtemp = x * x - y * y + cx;
        y = 2.0f * x * y + cy;
        x = xtemp;
        ++i;
    }
    return i;
}

__device__ void iterationToRGB(int iter, unsigned char* r, unsigned char* g, unsigned char* b)
{
    //paint points that have reached the value of infinity to black
    if (iter == MAX_ITER)
    {
        *r = 0;
        *g = 0;
        *b = 0;
        return;
    }

    float t = (float)iter / MAX_ITER;
    *r = (unsigned char)(9 * (1 - t) * t * t * t * 255);
    *g = (unsigned char)(15 * (1 - t) * (1 - t) * t * t * 255);
    *b = (unsigned char)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}

__global__ void juliaKernelRGB(unsigned char* output, int width, int height, float cx, float cy)
{
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height) return;

    float zoom = 3.0f;

    float real = (float)(x - width / 2) / (float)height * zoom;
    float imag = (float)(y - height / 2) / (float)height * zoom;

    int iter = julia(real, imag, cx, cy);

    unsigned char r, g, b;
    iterationToRGB(iter, &r, &g, &b);

    int idx = (y * width + x) * 3;
    output[idx] = r;
    output[idx + 1] = g;
    output[idx + 2] = b;
}

extern "C" void LaunchJuliaKernel(unsigned char* devPtr, int width, int height, float rc, float rs)
{
    dim3 threadsPerBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 numBlocks((width + threadsPerBlock.x - 1) / threadsPerBlock.x,
        (height + threadsPerBlock.y - 1) / threadsPerBlock.y);

    juliaKernelRGB << <numBlocks, threadsPerBlock >> > (devPtr, width, height, rc, rs);
}