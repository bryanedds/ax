// NOTE: file referenced from https://github.com/ssloy/tinyrenderer/blob/909fe20934ba5334144d2c748805690a1fa4c89f/tgaimage.h

#ifndef IMPL_AX_TGA_HPP
#define IMPL_AX_TGA_HPP

#include <fstream>

namespace ax
{
    enum tga_format
    {
        GRAYSCALE = 1,
        RGB = 3,
        RGBA = 4
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

    struct tga_color
    {
        union
        {
            struct { unsigned char b, g, r, a; };
            unsigned char raw[4];
            unsigned int val;
        };

        int bytespp;

        tga_color() : val(0), bytespp(1) { }
        tga_color(const tga_color &c) : val(c.val), bytespp(c.bytespp) { }
        tga_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A) : b(B), g(G), r(R), a(A), bytespp(4) { }
        tga_color(int v, int bpp) : val(v), bytespp(bpp) { }

        tga_color(const unsigned char *p, int bpp) : val(0), bytespp(bpp)
        {
            for (int i = 0; i < bpp; ++i) raw[i] = p[i];
        }

        tga_color& operator=(const tga_color &c)
        {
            if (this != &c)
            {
                bytespp = c.bytespp;
                val = c.val;
            }
            return *this;
        }
    };

    struct tga_image
    {
    public:

        tga_image();
        tga_image(int w, int h, int bpp);
        tga_image(const tga_image& img);
        ~tga_image();

        tga_image& operator=(const tga_image& img);

        inline int get_bytespp() { return bytespp; }
        inline int get_width() { return width; }
        inline int get_height() { return height; }
        inline unsigned char* get_buffer() { return data; }
        tga_color get_point(int x, int y);
        bool set_point(int x, int y, const tga_color& c);
        void draw_line(int x, int y, int x2, int y2, const tga_color& c);
        void clear();

        bool read_tga_file(const char *filename);
        bool write_tga_file(const char *filename);

    private:

        unsigned char* data;
        int width;
        int height;
        int bytespp;
    };
}

#endif
