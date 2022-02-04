#include <stdio.h>
#include <cuda_runtime.h>                                                                  // cuda_ 함수를 사용하기 위한 헤더파일 선언


__global__ void                                                                         // host에서 호출하고, device에서 실행하는 함수는 __global__로 선언
vectorAdd(const float* A, const float* B, float* C, int numElements) {                  // 벡터 덧셈 함수, 즉 대규모 병렬 덧셈을 수행하는 함수이다.
    int i = blockDim.x * blockIdx.x + threadIdx.x;                                  // i 는 인덱스로, 1차원 블록, 1차원 스레드의 인덱스를 나타낸다.

    if (i < numElements) {                                                          // GPU에 데이터가 할당되었기 때문에 for문이 아닌 if문 한번으로 
        C[i] = A[i] + B[i];                                                         // 대규모 데이터들의 덧셈이 한번에 이루어진다. 
    }
}

int main(void) {
    cudaError_t err = cudaSuccess;                                                  // cuda 함수의 에러여부를 확인하기 위한 에러코드변수를 err로 선언, 우선 cudaSuccess (0)을 넣어 초기화한다.

    int numElements = 50000;                                                        // 덧셈을 할 데이터 개수
    size_t size = numElements * sizeof(float);                                      // size 변수 선언, float 타입 데이터를 5만개 동적할당하기 위한 size 이다.
    printf("[Vector addition of %d elements]\n", numElements);                      // 벡터 덧셈이 진행될것임을 알리는 출력

    float* h_A = (float*)malloc(size);                                              // 호스트 메모리에 h_A 배열 동적 할당 (float 타입 50000개)
    float* h_B = (float*)malloc(size);                                              // 호스트 메모리에 h_B 배열 동적 할당 (float 타입 50000개)
    float* h_C = (float*)malloc(size);                                              // 호스트 메모리에 h_C 배열 동적 할당 (float 타입 50000개)

    if (h_A == NULL || h_B == NULL || h_C == NULL) {                            // 만약 호스트 메모리에 동적할당이 실패하면
        fprintf(stderr, "Failed to allocate host vectors!\n");                  // 실패 안내 (에러 메세지 출력)
        exit(EXIT_FAILURE);                                                     // 프로그램 종료
    }

    for (int i = 0; i < numElements; ++i) {                                     // for문을 돌며 5만개의 배열에
        h_A[i] = rand() / (float)RAND_MAX;                                      // h_A 배열 각각에 float 타입 랜덤 난수 대입
        h_B[i] = rand() / (float)RAND_MAX;                                      // h_B 배열 각각에 float 타입 랜덤 난수 대입
    }

    float* d_A = NULL;                                                          // device 메모리에 배열을 할당하기 위한 초기화
    err = cudaMalloc((void**)&d_A, size);                                       // d_A, 즉 device 메모리에 d_A 배열 동적 할당 , err은 에러코드 확인용

    if (err != cudaSuccess) {                                                                                   // 만약 앞서 수행한 device 메모리 동적할당이 실패한 경우
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));      // 에러 메세지 출력 (실패 안내)
        exit(EXIT_FAILURE);                                                                                     // 프로그램 종료
    }   

    float* d_B = NULL;                                                         // device 메모리에 배열을 할당하기 위한 초기화  
    err = cudaMalloc((void**)&d_B, size);                                      // d_B, 즉 device 메모리에 d_B 배열 동적 할당 , err은 에러코드 확인용

    if (err != cudaSuccess) {                                                                                   // 만약 앞서 수행한 device 메모리 동적할당이 실패한 경우
        fprintf(stderr, "Failed to allocate device vector B (error code %s)!\n", cudaGetErrorString(err));      // 에러 메세지 출력 (실패 안내)
        exit(EXIT_FAILURE);                                                                                     // 프로그램 종료
    }

    float* d_C = NULL;                                                          // device 메모리에 배열을 할당하기 위한 초기화  
    err = cudaMalloc((void**)&d_C, size);                                       // d_C, 즉 device 메모리에 d_C 배열 동적 할당 , err은 에러코드 확인용

    if (err != cudaSuccess) {                                                                                   // 만약 앞서 수행한 device 메모리 동적할당이 실패한 경우
        fprintf(stderr, "Failed to allocate device vector C (error code %s)!\n", cudaGetErrorString(err));      // 에러 메세지 출력 (실패 안내)
        exit(EXIT_FAILURE);                                                                                     // 프로그램 종료
    }

    printf("Copy input data from the host memory to the CUDA device\n");                // host -> device 로 데이터 전송을 할 것임을 안내
    err = cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);                           // h_A 배열 데이터들을 d_A 배열로 전부 전송 , err 은 에러코드 확인용

    if (err != cudaSuccess) {                                                                                               // 앞서 수행한 host -> device 데이터 전송이 실패할 경우
        fprintf(stderr, "Failed to copy vector A from host to device (error code %s)!\n", cudaGetErrorString(err));         // 에러 메세지 출력
        exit(EXIT_FAILURE);                                                                                                 // 프로그램 종료
    }

    err = cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);                           // h_B 배열 데이터들을 d_B 배열로 전부 전송 , err 은 에러코드 확인용

    if (err != cudaSuccess) {                                                                                               // 앞서 수행한 host -> device 데이터 전송이 실패할 경우
        fprintf(stderr, "Failed to copy vector B from host to device (error code %s)!\n", cudaGetErrorString(err));         // 에러 메세지 출력
        exit(EXIT_FAILURE);                                                                                                 // 프로그램 종료
    }

    int threadsPerBlock = 256;                                                          // 한 블럭에 할당되는 스레드의 개수 설정 (기기별 스펙 확인 후 정해진 값을 기입)
    int blocksPerGrid = (numElements + threadsPerBlock - 1) / threadsPerBlock;          // 한 블럭당 스레드 개수를 바탕으로 필요한 블럭 개수 설정                   
    printf("CUDA kernel launch with %d blocks of %d threads\n", blocksPerGrid, threadsPerBlock);        // 할당된 블럭 개수와 블럭 당 스레드 개수 확인
    vectorAdd << <blocksPerGrid, threadsPerBlock >> > (d_A, d_B, d_C, numElements);                     // GPU 에서 vectorAdd 함수 실행, 50000개의 데이터에 대해 C[i] = A[i]+B[i] 의 병렬 연산을 수행한다.
    err = cudaGetLastError();                                                                           // GPU에서 함수실행중 에러가 발생했는지 확인하기 위한 에러코드 확인용 err

    if (err != cudaSuccess) {                                                                                               // 만약 함수 실행중 에러가 발생하였다면
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));                   // 에러가 발생하였음을 출력
        exit(EXIT_FAILURE);                                                                                                 // 프로그램 종료
    }

    printf("Copy output data from the CUDA device to the host memory\n");                                               // device -> host 로 데이터 전송을 할 것임을 안내
    err = cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);                                                           // 덧셈 처리의 결과물, d_C 배열 데이터들을 h_C 배열에 전달한다. err은 에러코드확인용

    if (err != cudaSuccess) {                                                                                               // 만약 앞서 수행한 device -> host 데이터 전송이 실패할 경우
        fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));         // 에러 메세지 출력
        exit(EXIT_FAILURE);                                                                                                 // 프로그램 종료
    }

    for (int i = 0; i < numElements; ++i) {                                                                         // for문을 사용하여 오차율 검사

        if (fabs(h_A[i] + h_B[i] - h_C[i]) > 1e-5) {                                                                // h_C (덧셈 결과물) 와 h_A, h_B 덧셈의 차이를 비교하여, 그 차이가 10^-5 보다 큰 오차를 보이면   
            fprintf(stderr, "Result verification failed at element %d!\n", i);                                      // 덧셈이 실패했음으로 간주하여 에러 메세지 출력
            exit(EXIT_FAILURE);                                                                                     // 프로그램 종료
        }
    }

    printf("Test PASSED\n");                                                                        // 오차율 검사를 통과하면 테스트가 제대로 완료되었음을 안내

    err = cudaFree(d_A);                                                                                        // device 메모리에 동적할당된 배열 d_A를 해제해준다. err은 에러코드확인용

    if (err != cudaSuccess) {                                                                                            // 만약 d_A 메모리의 해제에 실패하면
        fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err));                   // 에러 메세지 출력
        exit(EXIT_FAILURE);                                                                                              // 프로그램 종료
    }

    err = cudaFree(d_B);                                                                                        // device 메모리에 동적할당된 배열 d_B를 해제해준다. err은 에러코드확인용

    if (err != cudaSuccess) {                                                                                           // 만약 d_B 메모리의 해제에 실패하면
        fprintf(stderr, "Failed to free device vector B (error code %s)!\n", cudaGetErrorString(err));                  // 에러 메세지 출력
        exit(EXIT_FAILURE);                                                                                             // 프로그램 종료
    }   

    err = cudaFree(d_C);                                                                                        // device 메모리에 동적할당된 배열 d_C를 해제해준다. err은 에러코드확인용

    if (err != cudaSuccess) {                                                                                           // 만약 d_C 메모리의 해제에 실패하면
        fprintf(stderr, "Failed to free device vector C (error code %s)!\n", cudaGetErrorString(err));                  // 에러 메세지 출력
        exit(EXIT_FAILURE);                                                                                             // 프로그램 종료
    }

    free(h_A);                                      // host 메모리에 동적할당된 배열 h_A 해제
    free(h_B);                                      // host 메모리에 동적할당된 배열 h_B 해제
    free(h_C);                                      // host 메모리에 동적할당된 배열 h_C 해제

    printf("Done\n");                               // 모든 동작이 완료되었음을 안내
    return 0;                                      
}


