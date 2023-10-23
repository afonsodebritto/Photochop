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
            size = height * width * channels;
        }
        else
        {
            cout << "Failed to read " << filename << endl;
        }
   }

    Image::Image(int width, int height, int channels) : width(width), height(height), channels(channels)
    {
        size = width * height * channels;
        data = new uint8_t[size];
    }

    Image::Image(const Image& img) : Image(img.width, img.height, img.channels)
    {
        memcpy(data, img.data, img.size);
    }

    Image::~Image() {
        stbi_image_free(data);
    }

    bool Image::read(const char* filename) 
    {
        data = stbi_load(filename, &width, &height, &channels, 0);
        return data != NULL;
    }
    bool Image::write(const char* filename) 
    {
        ImageType type = getFileType(filename);
        switch(type)
        {
            
        }
    }

    ImageType Image::getFileType(const char* filename)
    {
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

        return JPG;

    }