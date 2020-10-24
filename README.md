# Project2-ImageConvolutionParallelized

The code convolutes an image with a filter. For this, you need to have saved in the same folder as the code, your image and your filter in .txt format. If you want, you can use any of the 3 images we have already (lena1, edificio1, paisaje1) or you can upload your image and pass it to the code. For the filter, we have 3 options of 3x3 matrices, but you can use any size of filter and values by changing the filter path and size and creating a filter in .txt format. 
For the code to be able to read the image and write it modified, we use two libraries that are open source, STB_IMAGE_IMPLEMENTATION and STB_IMAGE_WRITE_IMPLEMENTATION. The image is saved in form of a vector and is treated like that through the whole code. The image is transformed to gray-scale with luminosity method. 
For the treatment of the borders, we just cropped the pixels of the borders, letting the resultant image smaller than the original one. As a result the convoluted image will be saved in JPEG format in the source folder.
This code prints the time it takes for the convolution process.
We implemented the code in a parallelized form using Open MP. You can pass different number of threads and schedule to the code.
To run the program from the command prompt you have to give the following arguments: number of executions, number of threads, schedule, imgae path, filter path, new image path, size of the filter.
