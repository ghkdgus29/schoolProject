#include <stdio.h>
#include <cuda_runtime.h>                                                                  // cuda_ �Լ��� ����ϱ� ���� ������� ����


__global__ void                                                                         // host���� ȣ���ϰ�, device���� �����ϴ� �Լ��� __global__�� ����
vectorAdd(const float* A, const float* B, float* C, int numElements) {                  // ���� ���� �Լ�, �� ��Ը� ���� ������ �����ϴ� �Լ��̴�.
    int i = blockDim.x * blockIdx.x + threadIdx.x;                                  // i �� �ε�����, 1���� ���, 1���� �������� �ε����� ��Ÿ����.

    if (i < numElements) {                                                          // GPU�� �����Ͱ� �Ҵ�Ǿ��� ������ for���� �ƴ� if�� �ѹ����� 
        C[i] = A[i] + B[i];                                                         // ��Ը� �����͵��� ������ �ѹ��� �̷������. 
    }
}

int main(void) {
    cudaError_t err = cudaSuccess;                                                  // cuda �Լ��� �������θ� Ȯ���ϱ� ���� �����ڵ庯���� err�� ����, �켱 cudaSuccess (0)�� �־� �ʱ�ȭ�Ѵ�.

    int numElements = 50000;                                                        // ������ �� ������ ����
    size_t size = numElements * sizeof(float);                                      // size ���� ����, float Ÿ�� �����͸� 5���� �����Ҵ��ϱ� ���� size �̴�.
    printf("[Vector addition of %d elements]\n", numElements);                      // ���� ������ ����ɰ����� �˸��� ���

    float* h_A = (float*)malloc(size);                                              // ȣ��Ʈ �޸𸮿� h_A �迭 ���� �Ҵ� (float Ÿ�� 50000��)
    float* h_B = (float*)malloc(size);                                              // ȣ��Ʈ �޸𸮿� h_B �迭 ���� �Ҵ� (float Ÿ�� 50000��)
    float* h_C = (float*)malloc(size);                                              // ȣ��Ʈ �޸𸮿� h_C �迭 ���� �Ҵ� (float Ÿ�� 50000��)

    if (h_A == NULL || h_B == NULL || h_C == NULL) {                            // ���� ȣ��Ʈ �޸𸮿� �����Ҵ��� �����ϸ�
        fprintf(stderr, "Failed to allocate host vectors!\n");                  // ���� �ȳ� (���� �޼��� ���)
        exit(EXIT_FAILURE);                                                     // ���α׷� ����
    }

    for (int i = 0; i < numElements; ++i) {                                     // for���� ���� 5������ �迭��
        h_A[i] = rand() / (float)RAND_MAX;                                      // h_A �迭 ������ float Ÿ�� ���� ���� ����
        h_B[i] = rand() / (float)RAND_MAX;                                      // h_B �迭 ������ float Ÿ�� ���� ���� ����
    }

    float* d_A = NULL;                                                          // device �޸𸮿� �迭�� �Ҵ��ϱ� ���� �ʱ�ȭ
    err = cudaMalloc((void**)&d_A, size);                                       // d_A, �� device �޸𸮿� d_A �迭 ���� �Ҵ� , err�� �����ڵ� Ȯ�ο�

    if (err != cudaSuccess) {                                                                                   // ���� �ռ� ������ device �޸� �����Ҵ��� ������ ���
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));      // ���� �޼��� ��� (���� �ȳ�)
        exit(EXIT_FAILURE);                                                                                     // ���α׷� ����
    }   

    float* d_B = NULL;                                                         // device �޸𸮿� �迭�� �Ҵ��ϱ� ���� �ʱ�ȭ  
    err = cudaMalloc((void**)&d_B, size);                                      // d_B, �� device �޸𸮿� d_B �迭 ���� �Ҵ� , err�� �����ڵ� Ȯ�ο�

    if (err != cudaSuccess) {                                                                                   // ���� �ռ� ������ device �޸� �����Ҵ��� ������ ���
        fprintf(stderr, "Failed to allocate device vector B (error code %s)!\n", cudaGetErrorString(err));      // ���� �޼��� ��� (���� �ȳ�)
        exit(EXIT_FAILURE);                                                                                     // ���α׷� ����
    }

    float* d_C = NULL;                                                          // device �޸𸮿� �迭�� �Ҵ��ϱ� ���� �ʱ�ȭ  
    err = cudaMalloc((void**)&d_C, size);                                       // d_C, �� device �޸𸮿� d_C �迭 ���� �Ҵ� , err�� �����ڵ� Ȯ�ο�

    if (err != cudaSuccess) {                                                                                   // ���� �ռ� ������ device �޸� �����Ҵ��� ������ ���
        fprintf(stderr, "Failed to allocate device vector C (error code %s)!\n", cudaGetErrorString(err));      // ���� �޼��� ��� (���� �ȳ�)
        exit(EXIT_FAILURE);                                                                                     // ���α׷� ����
    }

    printf("Copy input data from the host memory to the CUDA device\n");                // host -> device �� ������ ������ �� ������ �ȳ�
    err = cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);                           // h_A �迭 �����͵��� d_A �迭�� ���� ���� , err �� �����ڵ� Ȯ�ο�

    if (err != cudaSuccess) {                                                                                               // �ռ� ������ host -> device ������ ������ ������ ���
        fprintf(stderr, "Failed to copy vector A from host to device (error code %s)!\n", cudaGetErrorString(err));         // ���� �޼��� ���
        exit(EXIT_FAILURE);                                                                                                 // ���α׷� ����
    }

    err = cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);                           // h_B �迭 �����͵��� d_B �迭�� ���� ���� , err �� �����ڵ� Ȯ�ο�

    if (err != cudaSuccess) {                                                                                               // �ռ� ������ host -> device ������ ������ ������ ���
        fprintf(stderr, "Failed to copy vector B from host to device (error code %s)!\n", cudaGetErrorString(err));         // ���� �޼��� ���
        exit(EXIT_FAILURE);                                                                                                 // ���α׷� ����
    }

    int threadsPerBlock = 256;                                                          // �� ���� �Ҵ�Ǵ� �������� ���� ���� (��⺰ ���� Ȯ�� �� ������ ���� ����)
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;          // �� ���� ������ ������ �������� �ʿ��� �� ���� ����                   
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);        // �Ҵ�� �� ������ �� �� ������ ���� Ȯ��
    vectorAdd << <blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, numElements);                     // GPU ���� vectorAdd �Լ� ����, 50000���� �����Ϳ� ���� C[i] = A[i]+B[i] �� ���� ������ �����Ѵ�.
    err = cudaGetLastError();                                                                           // GPU���� �Լ������� ������ �߻��ߴ��� Ȯ���ϱ� ���� �����ڵ� Ȯ�ο� err

    if (err != cudaSuccess) {                                                                                               // ���� �Լ� ������ ������ �߻��Ͽ��ٸ�
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));                   // ������ �߻��Ͽ����� ���
        exit(EXIT_FAILURE);                                                                                                 // ���α׷� ����
    }

    printf("Copy output data from the CUDA device to the host memory\n");                                               // device -> host �� ������ ������ �� ������ �ȳ�
    err = cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);                                                           // ���� ó���� �����, d_C �迭 �����͵��� h_C �迭�� �����Ѵ�. err�� �����ڵ�Ȯ�ο�

    if (err != cudaSuccess) {                                                                                               // ���� �ռ� ������ device -> host ������ ������ ������ ���
        fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));         // ���� �޼��� ���
        exit(EXIT_FAILURE);                                                                                                 // ���α׷� ����
    }

    for (int i = 0; i < numElements; ++i) {                                                                         // for���� ����Ͽ� ������ �˻�

        if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5) {                                                                // h_C (���� �����) �� h_A, h_B ������ ���̸� ���Ͽ�, �� ���̰� 10^-5 ���� ū ������ ���̸�   
            fprintf(stderr, "Result verification failed at element %d!\n", i);                                      // ������ ������������ �����Ͽ� ���� �޼��� ���
            exit(EXIT_FAILURE);                                                                                     // ���α׷� ����
        }
    }

    printf("Test PASSED\n");                                                                        // ������ �˻縦 ����ϸ� �׽�Ʈ�� ����� �Ϸ�Ǿ����� �ȳ�

    err = cudaFree(d_A);                                                                                        // device �޸𸮿� �����Ҵ�� �迭 d_A�� �������ش�. err�� �����ڵ�Ȯ�ο�

    if (err != cudaSuccess) {                                                                                            // ���� d_A �޸��� ������ �����ϸ�
        fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err));                   // ���� �޼��� ���
        exit(EXIT_FAILURE);                                                                                              // ���α׷� ����
    }

    err = cudaFree(d_B);                                                                                        // device �޸𸮿� �����Ҵ�� �迭 d_B�� �������ش�. err�� �����ڵ�Ȯ�ο�

    if (err != cudaSuccess) {                                                                                           // ���� d_B �޸��� ������ �����ϸ�
        fprintf(stderr, "Failed to free device vector B (error code %s)!\n", cudaGetErrorString(err));                  // ���� �޼��� ���
        exit(EXIT_FAILURE);                                                                                             // ���α׷� ����
    }   

    err = cudaFree(d_C);                                                                                        // device �޸𸮿� �����Ҵ�� �迭 d_C�� �������ش�. err�� �����ڵ�Ȯ�ο�

    if (err != cudaSuccess) {                                                                                           // ���� d_C �޸��� ������ �����ϸ�
        fprintf(stderr, "Failed to free device vector C (error code %s)!\n", cudaGetErrorString(err));                  // ���� �޼��� ���
        exit(EXIT_FAILURE);                                                                                             // ���α׷� ����
    }

    free(h_A);                                      // host �޸𸮿� �����Ҵ�� �迭 h_A ����
    free(h_B);                                      // host �޸𸮿� �����Ҵ�� �迭 h_B ����
    free(h_C);                                      // host �޸𸮿� �����Ҵ�� �迭 h_C ����

    printf("Done\n");                               // ��� ������ �Ϸ�Ǿ����� �ȳ�
    return 0;                                      
}


