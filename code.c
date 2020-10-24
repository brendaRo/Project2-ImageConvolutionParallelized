#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <regex.h>
#include <time.h>
//#include <omp.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

int readmatrix(size_t rows, size_t cols, double (*a)[cols], const char* filename)
{
    size_t i, j;
    FILE *pf;
    pf = fopen (filename, "r");
    if (pf == NULL)
        return 0;

    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < cols; j++)
            fscanf(pf, "%lf", &a[i][j]);
    }

    fclose (pf);
    return 1;
}


int main(int argc, char *argv[]){


	char *imgpath = argv[1];
	char *filterpath = argv[2];
	char *newimgpath = argv[3];
	char *filtsize = argv[4];

	int m;

	for(m = 0; m <= 9; m++){ //executions

        int i, j, k, m, n, h, w, b, colorc = 3, grayc = 1;
        double filter[atoi(filtsize)][atoi(filtsize)];// t0, t1, temps[9];
        readmatrix(atoi(filtsize), atoi(filtsize), filter, filterpath);
        //double filter[3][3]={0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};//creation of the filter matrix


        uint8_t* image = stbi_load(imgpath, &w, &h, &b, colorc); //image vector
        int img_size = w * h * colorc, wr = w-(atoi(filtsize)-1), hr = h-(atoi(filtsize)-1);
        int gray_img_size = w * h * grayc, red_img_s = hr * wr * grayc; //size of gray images
        unsigned char *gray_img = malloc(gray_img_size*10), *new_img_gray = malloc(gray_img_size*10), *reduced = malloc(red_img_s*10); //gray imgs + reduced



//transforming of RGB image to gray scale image

        for(unsigned char *p = image, *pg = gray_img; p != image + img_size; p += colorc, pg += grayc) {
            *pg = (uint8_t)((*p*0.21 + *(p + 1)*0.72 + *(p + 2)*0.07)); //luminosity
        }


//convolution of the gray image and the filter
        //t0 = omp_get_wtime();
        clock_t tic_c = clock();
        for (k = 0; k <= gray_img_size; k++){
                    unsigned char sum_loc = 0;
                    unsigned char *pg = new_img_gray;
                    unsigned char *p = gray_img;
                    int i, j;

                    for (i = atoi(filtsize)-1; i >= 0; i--){ //convolution for digital images

                        for (j = atoi(filtsize)-1; j >= 0; j--){

                            sum_loc += filter[i][j] * *(p + k +(atoi(filtsize)-(j+1)) + (w*(atoi(filtsize)-(i+1))));
                        }
                    }

                //save convolution result into the new image
                *(pg + k) = (uint8_t)sum_loc;
            }
        clock_t toc_c = clock();
        //t1 = omp_get_wtime();

//new image
            stbir_resize_uint8(new_img_gray , w , h , 0, reduced, wr, hr, 0, 1);
            stbi_write_jpg(newimgpath, wr, hr, grayc, reduced, 100);
//free memories
            stbi_image_free(image);
            free(gray_img);
            free(new_img_gray);
            free(reduced);


        //times
        //temps[m] = t1 - t0;

        printf("Total convolution time: %f seconds\n",(double)(toc_c-tic_c)/CLOCKS_PER_SEC);
	} //executions

	return 0;
}
