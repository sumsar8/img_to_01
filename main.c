#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"
#include <stdlib.h>


#define WIDTH 128
#define HEIGHT 64

int main(){
    int width, height, channels;
    unsigned char *input_image = stbi_load("img.png", &width, &height, &channels, 0);
    if (!input_image) {
        fprintf(stderr, "Failed to load image\n");
        return 1;
    }

    // Calculate the resize factors
    int max_width = WIDTH;
    int max_height = HEIGHT;

    float width_scale = (float)max_width / width;
    float height_scale = (float)max_height / height;
    float scale = (width_scale < height_scale) ? width_scale : height_scale; // Choose the smaller scale

    int new_width = (int)(width * scale);
    int new_height = (int)(height * scale);

    // Allocate memory for the resized image
    unsigned char *resized_image = (unsigned char *)malloc(new_width * new_height * channels);
    if (!resized_image) {
        fprintf(stderr, "Failed to allocate memory for resized image\n");
        stbi_image_free(input_image);
        return 1;
    }

    // Resize the image
    if (!stbir_resize_uint8_linear(input_image, width, height, 0,
                            resized_image, new_width, new_height, 0,
                            channels)) {
        fprintf(stderr, "Failed to resize image\n");
        free(resized_image);
        stbi_image_free(input_image);
        return 1;
    }

    // Save the resized image (optional)
    if (!stbi_write_png("output.png", new_width, new_height, channels, resized_image, new_width * channels)) {
        fprintf(stderr, "Failed to save resized image\n");
    } else {
        printf("Resized image saved to 'output.png'\n");
    }

    // Free memory
    free(resized_image);
    stbi_image_free(input_image);
    int w, h, chan;
    unsigned char *img = stbi_load("output.png", &w, &h, &chan, 0);
    int array[HEIGHT][WIDTH] = {0};
    int cutoff = 300;
    if(img){
        int totalvalue = 0;
        for(int i = 0; i < new_height; i++){
            for(int j = 0; j < new_width; j++){
                unsigned char *pixel = img + (i * w + j) * chan;
                totalvalue = pixel[0] + pixel[1] + pixel[2];
                if(totalvalue > cutoff){
                    array[i][j] = 1;
                }else{
                    array[i][j] = 0;
                }
            }
        }
        
        stbi_image_free(img);
    }
    FILE *fptr;
    fptr = fopen("out.txt", "w");
    char snum[2];
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            itoa(array[i][j], snum, 10);
            fprintf(fptr, snum);
        }
        fprintf(fptr, "\n");
    }
    
    fclose(fptr);

    return 0;
}