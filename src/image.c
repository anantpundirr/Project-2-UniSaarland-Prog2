#include <stdio.h>
#include <stdlib.h>

void apply_threshold(float *img, int w, int h, int T) {
    // Calculate the total number of pixels
    int total_pixels = w * h;

    // Loop through each pixel
    for (int i = 0; i < total_pixels; i++) {

        if (img[i] > T) {
            // Set the pixel to white (255)
            img[i] = 255.0f;
        } else {
            // Set the pixel to black (0)
            img[i] = 0.0f;
        }
    }
}
#include <float.h> 

void scale_image(float *result, const float *img, int w, int h) {
    int total_pixels = w * h;
    float min = FLT_MAX, max = -FLT_MAX;

    for (int i = 0; i < total_pixels; i++) {
        if (img[i] < min) min = img[i];
        if (img[i] > max) max = img[i];
    }

    if (min == max) {
        memset(result, 0, total_pixels * sizeof(float));
        return;
    }

    float range = max - min;
    for (int i = 0; i < total_pixels; i++) {
        result[i] = ((img[i] - min) / range) * 255;
    }
}

float get_pixel_value(const float *img, int w, int h, int x, int y) {
    // Mirror the coordinates at the boundaries
    if (x < 0) x = -x-1;
    if (y < 0) y = -y-1;
    if (x >= w) x = 2 * w - x - 1;
    if (y >= h) y = 2 * h - y - 1;
    
    return img[y * w + x];
}

float *array_init(int size) {
    return (float *)malloc(size * sizeof(float));
}

void array_destroy(float *m) {
    free(m);
}

void skip_comments(FILE *fp) {
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch));
    if (ch == '#') {
        if (fgets(line, sizeof(line), fp) == NULL) return; // Handle error if fgets fails
        skip_comments(fp); // Recursively skip subsequent comments
    } else {
        ungetc(ch, fp); // Put back the last non-comment, non-whitespace character
    }
}

float* read_image_from_file(const char *filename, int *w, int *h) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "File not found: %s\n", filename);
        return NULL;
    }

    char header[3];
    int maxval;
    skip_comments(file);
    if (fscanf(file, "%2s", header) != 1 || strcmp(header, "P2") != 0) {
        fprintf(stderr, "Invalid or unsupported image format\n");
        fclose(file);
        return NULL;
    }

    skip_comments(file);
    if (fscanf(file, "%d %d", w, h) != 2 || *w <= 0 || *h <= 0) {
        fprintf(stderr, "Invalid image dimensions\n");
        fclose(file);
        return NULL;
    }

    skip_comments(file);
    if (fscanf(file, "%d", &maxval) != 1 || maxval != 255) {
        fprintf(stderr, "Invalid maximum grayscale value\n");
        fclose(file);
        return NULL;
    }

    int num_pixels = *w * *h;
    float *img = array_init(num_pixels);
    if (!img) {
        fclose(file);
        return NULL;
    }

    int pixel, pixel_count = 0;
    for (int i = 0; i < num_pixels; i++) {
        if (fscanf(file, "%d", &pixel) != 1 || pixel < 0 || pixel > 255) {
            fprintf(stderr, "Pixel values must be within the range 0-255\n");
            fclose(file);
            array_destroy(img);
            return NULL;
        }
        img[i] = (float)pixel;
        pixel_count++;
    }

    // Check for correct number of pixels
    if (pixel_count != num_pixels) {
        fprintf(stderr, "Incorrect number of pixels: expected %d, got %d\n", num_pixels, pixel_count);
        fclose(file);
        array_destroy(img);
        return NULL;
    }

    // Check for any extraneous data after the expected number of pixels
    if (fscanf(file, "%d", &pixel) == 1) {
        fprintf(stderr, "File contains extraneous data beyond the expected number of pixels\n");
        fclose(file);
        array_destroy(img);
        return NULL;
    }

    fclose(file);
    return img;
}

void write_image_to_file(const float *img, int w, int h, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Unable to open file for writing: %s\n", filename);
        return;
    }

    fprintf(file, "P2\n%d %d\n255\n", w, h);
    for (int i = 0; i < w * h; i++) {
        if (i % w == 0 && i != 0)
            fprintf(file, "\n");
        fprintf(file, "%d ", (int)(img[i] + 0.5));
    }
    fprintf(file, "\n");
    fclose(file);
}









