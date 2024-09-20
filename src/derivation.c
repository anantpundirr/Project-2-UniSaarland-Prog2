#include "derivation.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "convolution.h"
#include "image.h"

void gradient_magnitude(float *result, const float *d_x, const float *d_y, int w, int h) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int i = y * w + x; 

        float dx = d_x[i];
        float dy = d_y[i];

        result[i] = sqrt(dx * dx + dy * dy);
        }
    }
}

const float sobel_x[9] = {1, 0, -1, 2, 0, -2, 1, 0, -1};
const float sobel_y[9] = {1, 2, 1, 0, 0, 0, -1, -2, -1};

void derivation_x_direction(float *result, const float *img, int w, int h) {
    assert(img != NULL && result != NULL);
    convolve(result, img, w, h, sobel_x, 3, 3);
}

void derivation_y_direction(float *result, const float *img, int w, int h) {
    assert(img != NULL && result != NULL);
    convolve(result, img, w, h, sobel_y, 3, 3);
}



