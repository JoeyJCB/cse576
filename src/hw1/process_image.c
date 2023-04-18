#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    int image_height = im.h, image_width = im.w, image_channel = im.c, p;
    float pixel;

    if (x >= image_width) x = image_width - 1;
    if (y >= image_height) y = image_height - 1;
    if (c >= image_channel) c = image_channel - 1;

    p = c*image_height*image_width + y*image_width + x;
    pixel = im.data[p];

    return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    int image_height = im.h, image_width = im.w, image_channel = im.c;
    if (x < image_width && y < image_height && c < image_channel){
        int p = c*image_height*image_width + y*image_width + x;
        im.data[p] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    int width = im.w, height = im.h;
    float red, green, blue;
    for (int i=0; i<width*height; i++){
        red = im.data[i];
        green = im.data[i + width * height];
        blue = im.data[i + 2 * width * height];
        gray.data[i] = 0.299 * red + 0.587 * green + 0.114 * blue;
        // printf("%f,", gray.data[i]);
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    int width = im.w, height = im.h;
    for (int i=0; i<width*height;i++){
        im.data[i + c * width*height] += v;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for(int i=0;i<im.h*im.w*im.c;i++){
        if (im.data[i]<0) im.data[i] = 0;
        if (im.data[i]>1) im.data[i] = 1;
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    int width = im.w, height = im.h;
    float r, g, b, cmin, cmax, delta, h, s, v;

    for (int i = 0; i < width * height; i++) {
        r = im.data[i];
        g = im.data[i + width * height];
        b = im.data[i + width * height * 2];
        
        // Calculate value (brightness)
        cmax = three_way_max(r,g,b);
        cmin = three_way_min(r,g,b);
        v = cmax;
        
        // Calculate saturation
        if (cmax == 0) {
            s = 0;
        } else {
            s = (cmax - cmin) / cmax;
        }
        
        // Calculate hue
        if (cmax == cmin) {
            h = 0;
        } else if (cmax == r) {
            h = fmod((g - b) / (cmax - cmin) + 6, 6);
        } else if (cmax == g) {
            h = (b - r) / (cmax - cmin) + 2;
        } else {
            h = (r - g) / (cmax - cmin) + 4;
        }
        
        h /= 6; // Convert hue to range [0,1]
        
        // Update pixel values in image data array
        im.data[i] = h;
        im.data[i + width * height] = s;
        im.data[i + width * height * 2] = v;
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    int width = im.w, height = im.h;
    float h, s, v, c, x, m, r, g, b;

    for (int i = 0; i < width * height; i++) {
        h = im.data[i];
        s = im.data[i + width * height];
        v = im.data[i + width * height * 2];
        
        c = v * s;
        x = c * (1 - fabs(fmod(h * 6, 2) - 1));
        m = v - c;
        
        if (h < 1.0 / 6) {
            r = c;
            g = x;
            b = 0;
        } else if (h < 2.0 / 6) {
            r = x;
            g = c;
            b = 0;
        } else if (h < 3.0 / 6) {
            r = 0;
            g = c;
            b = x;
        } else if (h < 4.0 / 6) {
            r = 0;
            g = x;
            b = c;
        } else if (h < 5.0 / 6) {
            r = x;
            g = 0;
            b = c;
        } else {
            r = c;
            g = 0;
            b = x;
        }
        
        r += m;
        g += m;
        b += m;
        
        // Update pixel values in image data array
        im.data[i] = r;
        im.data[i + width * height] = g;
        im.data[i + width * height * 2] = b;
    }
}
