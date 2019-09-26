#ifndef IMPL_AX_TGA_HPP
#define IMPL_AX_TGA_HPP

#include "math.hpp"

namespace ax
{
    enum tga_format
    {
        TGA_BGRA = 4
    };

    #pragma pack(push,1)
    struct tga_header
    {
        char idlength;
        char colormaptype;
        char datatypecode;
        short colormaporigin;
        short colormaplength;
        char colormapdepth;
        short x_origin;
        short y_origin;
        short width;
        short height;
        char bitsperpixel;
        char imagedescriptor;
    };
    #pragma pack(pop)
}

#endif
