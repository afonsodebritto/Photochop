#include <stdint.h>

enum ImageType
{
    PNG, JPG, BMP, TGA
};

struct Image
{
    unit8_t* data = NULL;
    size_t size = 0;
    // Size of the image
    int width;
    int height;
    // How many channels the image has
    int channels;

    Image(const char* filename);
    Image(int width, int height, int channels);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType(const char* filename);
}