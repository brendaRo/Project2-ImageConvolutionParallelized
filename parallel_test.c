#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <regex.h>
#include <time.h>
#include <omp.h>
#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"


int readmatrix(size_t rows, size_t cols, double (*a)[cols], const char* filename)
{
    size_t v, q;
    FILE *pf;
    pf = fopen (filename, "r");
    if (pf == NULL)
        return 0;

    for( v = 0; v < rows; v++)
    {
        for( q = 0; q < cols; q++)
            fscanf(pf, "%lf", &a[v][q]);
    }

    fclose (pf);
    return 1;
}


int main(int argc, char *argv[]){



  //char *argv [] =  {"1","1", "4","dynamic", "lena1.jpeg", "filter_gauss.txt", "lena1_gauss_lum.jpeg", "3"};
  //char *argv [] =  {"1","1","4","edificio1.jpeg", "filter_gauss.txt", "edificio_gauss_lum.jpeg", "3"};
  //char *argv [] =  {"1","9","1","dynamic","paisaje1.jpeg", "filter_gauss.txt", "paisaje_gauss_lum.jpeg", "3"};


    char *iterations = argv[1];
    char *threads = argv[2];
    char *sched = argv[3];
	char *imgpath = argv[4];
	char *filterpath = argv[5];
	char *newimgpath = argv[6];
	char *filtsize = argv[7];


	int m, g;

    for(g = 1; g <= atoi(threads); g++){ //threads
        for(m = 0; m <= atoi(iterations);m++){  //executions

            omp_set_num_threads(g);
            int i, j, k, m, n, h, w, b, colorc = 3, grayc = 1;
            FILE *fil;
            double filter[3][3]={0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625}, t0, t1, temps[9];
             //creation of the filter matrix


            uint8_t* image = stbi_load(imgpath, &w, &h, &b, colorc); //image vector
            int img_size = w * h * colorc, wr = w-(atoi(filtsize)-1), hr = h-(atoi(filtsize)-1);
            int gray_img_size = w * h * grayc, red_img_s = hr * wr * grayc; //size of gray images
            unsigned char *gray_img = malloc(gray_img_size*10), *new_img_gray = malloc(gray_img_size*10), *reduced = malloc(red_img_s*10); //gray imgs + reduced


    //transforming of RGB image to gray scale image

            for(n = 0; n <=  gray_img_size; n++) {
                unsigned char *pg = gray_img;
                unsigned char *p = image;

                *(pg + n) = (uint8_t)((*(p+ n*colorc)*0.21 + *(p + n*colorc + 1)*0.72 + *(p + n*colorc + 2)*0.07)); //luminosity
            }


    //convolution of the gray image and the filter
            t0 = omp_get_wtime();


            #pragma omp parallel for shared(new_img_gray, gray_img_size, gray_img, filter, w, filtsize) schedule(dynamic)

            for (k=0; k<= gray_img_size; k++){
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

            t1 = omp_get_wtime();

            stbir_resize_uint8(new_img_gray , w , h , 0, reduced, wr, hr, 0, 1);
            stbi_write_jpg(newimgpath, wr, hr, grayc, reduced, 100);

    //free images
            stbi_image_free(image);
            free(gray_img);
            free(new_img_gray);
            free(reduced);


    //time
            printf("Total_convolution_time %d:_%f_seconds\n", g, (double)(t1-t0));
        } //exe
    } //threads

	return 0;
}
