#include "convolution.h"

#include <stdlib.h>

#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

void convolve(float *result, const float *img, int w, int h, const float *matrix, int w_m, int h_m) {
    int a = w_m / 2;
    int b = h_m / 2;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float sum = 0.0;
            for (int j = 0; j < h_m; j++) {
                for (int i = 0; i < w_m; i++) {
                    int px = x - a + i;
                    int py = y - b + j;
                    sum += matrix[j * w_m + i] * get_pixel_value(img, w, h, px, py);
                }
            }
            result[y * w + x] = sum;
        }
    }
}