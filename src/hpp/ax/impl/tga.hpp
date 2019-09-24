// NOTE: file referenced from https://github.com/ssloy/tinyrenderer/blob/909fe20934ba5334144d2c748805690a1fa4c89f/tgaimage.h

#ifndef IMPL_AX_TGA_HPP
#define IMPL_AX_TGA_HPP

#include <fstream>

#include "math.hpp"

namespace ax
{
    enum tga_format
    {
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
            struct { uint8_t b, g, r, a; };
            uint8_t raw[4];
            uint32_t val;
        };

        tga_color() : tga_color(0) { }
        tga_color(uint8_t R, uint8_t G, uint8_t B, uint8_t A) : b(B), g(G), r(R), a(A) { }
        tga_color(int v) : val(v) { }
        tga_color(const tga_color &color) : val(color.val) { }

        // NOTE: this looks particularly dangerous...
        // TODO: get rid of this.
        tga_color(const uint8_t *p) : val(0)
        {
            for (int i = 0; i < RGBA; ++i) raw[i] = p[i];
        }

        tga_color& operator=(const tga_color &color)
        {
            if (this != &color) val = color.val;
            return *this;
        }
    };

    struct tga_image
    {
    public:

        tga_image();
        tga_image(int w, int h);
        tga_image(const tga_image& img);
        ~tga_image();

        tga_image& operator=(const tga_image& img);

        inline int get_bytespp() const { return bytespp; }
        inline int get_width() const { return width; }
        inline int get_height() const { return height; }
        inline const uint8_t* get_buffer() const { return data; }
        inline uint8_t* get_buffer() { return data; }
        ax::color get_point(int x, int y) const;
        bool set_point(int x, int y, const ax::color& color);
        void draw_line(int x, int y, int x2, int y2, const ax::color& color);
        void clear();

        bool read_tga_file(const char *filename);
        bool write_tga_file(const char *filename) const;

    private:

        uint8_t* data;
        int width;
        int height;
        int bytespp;
    };
}

#endif