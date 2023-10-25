#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "stb_image.h"
#include "image.h"
#include <iostream>
using namespace std;

Image::Image(const char* filename) 
{
    if(read(filename))
    {
        cout << "Read " << filename << endl;
        size = h * w * channels;
    }
    else
    {
        cout << "Failed to read " << filename << endl;
    }
}

Image::Image(int w, int h, int channels) : w(w), h(h), channels(channels)
{
    size = w * h * channels;
    data = new uint8_t[size];
}

Image::Image(const Image& img) : Image(img.w, img.h, img.channels)
{
    memcpy(data, img.data, img.size);
}

Image::~Image() {
    stbi_image_free(data);
}

bool Image::read(const char* filename) 
{
    data = stbi_load(filename, &w, &h, &channels, 0);
    return data != NULL;
}
bool Image::write(const char* filename) 
{
    ImageType type = getFileType(filename);
    int success;
    switch(type)
    {
        case PNG:
            success = stbi_write_png(filename, w, h, channels, data,  w*channels);
            break;
        case JPG:
            success = stbi_write_jpg(filename, w, h, channels, data, 100);
            break;
        case BMP:
            success = stbi_write_bmp(filename, w, h, channels, data);
            break;
        case TGA:
            stbi_write_tga(filename, w, h, channels, data);
            break;

    }

    return success != 0;
}

ImageType Image::getFileType(const char* filename)
{
    // Get the file extension and return it
    const char* ext = strrchr(filename, '.');
    if(ext != nullptr)
    {
        return PNG;
    }
    else if(strcmp(ext, ".png") == 0)
    {
        return PNG;
    }
    else if(strcmp(ext, ".jpg") == 0)
    {
        return JPG;
    }
    else if(strcmp(ext, ".bmp") == 0)
    {
        return BMP;
    }
    else if(strcmp(ext, ".tga") == 0)
    {
        return TGA;
    }

    // If it's not a recognized extension, just use JPG
    return JPG;

}


void Image::gray_scale()
{
    if(channels < 3)
    {
        cout << "Image has less than 3 channels" << endl;
    }
    else
    {
        int i;
        for(i = 0; i < size; i += channels)
        {
            float gray = 0.299*data[i] + 0.587*data[i+1] + 0.114*data[i+2];
            memset(data + i, gray, 3);
        }
    }
}