#include "Image.h"

int main()
{
    Image test("Gramado_72k.jpg");

    Image gray_test = test;
    gray_test.gray_scale();
    gray_test.write("graytest.jpg");

    return 0;
}