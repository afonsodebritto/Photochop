#include <stdint.h>
#include <cstddef>

enum ImageType
{
    PNG, JPG, BMP, TGA
};

struct Image
{
    uint8_t* data = NULL;
    size_t size = 0;
    // Size of the image
    int w;
    int h;
    // How many channels the image has
    int channels;

    Image(const char* filename);
    Image(int w, int h, int channels);
    Image(const Image& img);
    ~Image();

    bool read(const char* filename);
    bool write(const char* filename);

    ImageType getFileType(const char* filename);

    void gray_scale();
};