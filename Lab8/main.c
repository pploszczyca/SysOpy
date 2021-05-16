#include "pgma_io.h"
#include <pthread.h>
#include <sys/time.h>

#define MAX_PIXEL_VALUE 255

// Structure for ASCII PGM image 
typedef struct matrix_data {
    int *imageMatrix;
    int xsize;
    int ysize;
    int maxg;
} matrix_data;

// Arguments for thread functions
typedef struct processing_data {
    pthread_t thread_id;
    int start;
    int end;
    matrix_data *image;
} processing_data;

int calculateNewPixel(int pixel){
    return MAX_PIXEL_VALUE - pixel;
}

double calculate_time(struct timeval tv1, struct timeval tv2){
    return (double) (tv2.tv_usec - tv1.tv_usec) +(double) ((tv2.tv_sec - tv1.tv_sec)*1000000);
}

void *numbers_image_processing(void *arg){
    processing_data data = *(processing_data*)arg;
    int index;

    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);       // Start time

    for(int y = 0; y < data.image->ysize ; y++){
        for(int x = 0; x < data.image->xsize; x++){
            index = y*data.image->xsize+x;

            if(data.start <= data.image->imageMatrix[index] && data.image->imageMatrix[index] <= data.end){
                data.image->imageMatrix[index] = calculateNewPixel(data.image->imageMatrix[index]);
            }
        }
    }

    gettimeofday(&tv2, NULL);       // End time
    printf("    Thread number: %2d Time: %7g μs\n", data.thread_id, calculate_time(tv1, tv2));

    free(arg);
}

void *blocks_image_processing(void *arg){
    processing_data data = *(processing_data*)arg;
    int index;

    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);       // Start time

    for(int y = 0; y < data.image->ysize ; y++){
        for(int x = data.start; x <= data.end; x++){
            index = y*data.image->xsize+x;
            data.image->imageMatrix[index] = calculateNewPixel(data.image->imageMatrix[index]);
        }
    }

    gettimeofday(&tv2, NULL);       // End time
    printf("    THREAD NUMBER: %2d TIME: %7g μs\n", data.thread_id, calculate_time(tv1, tv2));

    free(arg);
}

int main(int argc, char *argv[]){
    int nOfThreads;
    char *typeOfDivision, *inputFilePath, *outputFilePath;
    pthread_t *threads;
    matrix_data imageData;
    processing_data *data;
    struct timeval  tv1, tv2;

    if(argc != 5){
        printf("Bad arguments\n");
        exit(1);
    }

    nOfThreads = atoi(argv[1]);
    typeOfDivision = argv[2];
    inputFilePath = argv[3];
    outputFilePath = argv[4];

    threads = calloc(nOfThreads, sizeof(pthread_t));

    pgma_read(inputFilePath, &imageData.xsize, &imageData.ysize, &imageData.maxg, &imageData.imageMatrix);

    gettimeofday(&tv1, NULL);       // Start time
    for(int k = 0; k < nOfThreads; k++){
        data = malloc(sizeof(processing_data));
        data->image = &imageData;
        data->thread_id = k;

        if(strcmp(typeOfDivision, "numbers") == 0){
            data->start = floor(k*(MAX_PIXEL_VALUE/nOfThreads));
            data->end = k+1!=nOfThreads ? floor((k+1)*(MAX_PIXEL_VALUE/nOfThreads))-1: MAX_PIXEL_VALUE;

            pthread_create(&threads[k], NULL, numbers_image_processing, data);
        } else if (strcmp(typeOfDivision, "block") == 0){
            data->start = (k)*ceil(imageData.xsize/nOfThreads);
            data->end = k+1!=nOfThreads ? (k+1)*ceil(imageData.xsize/nOfThreads)-1: imageData.xsize-1;

            pthread_create(&threads[k], NULL, blocks_image_processing, data);
        }
    }

    for(int i = 0; i < nOfThreads; i++){
        pthread_join(threads[i], NULL);
    }
    gettimeofday(&tv2, NULL);       // End time

    printf("    TOTAL PROGRAM TIME: %11g μs\n", calculate_time(tv1, tv2));

    pgma_write(outputFilePath, "Result" ,imageData.xsize, imageData.ysize, imageData.maxg, imageData.imageMatrix);

    free(threads);
    free(imageData.imageMatrix);
    return 0;
}