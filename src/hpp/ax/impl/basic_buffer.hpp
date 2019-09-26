#ifndef AX_BASIC_BUFFER_HPP
#define AX_BASIC_BUFFER_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "tga.hpp"

namespace ax
{
    enum buffer_format
    {
        BUFFER_RGBA = 4
    };

    struct basic_buffer
    {
    public:

        basic_buffer();
        basic_buffer(int w, int h);
        basic_buffer(const basic_buffer& img);
        ~basic_buffer();

        basic_buffer& operator=(const basic_buffer& img);

        inline int get_bytespp() const { return bytespp; }
        int get_width() const { return width; };
        int get_height() const { return height; };
        ax::color get_point(int x, int y) const;
        bool set_point(int x, int y, const ax::color& color);
        void clear(const ax::color& color);

        bool read_from_tga_file(const char *filename);
        bool write_to_tga_file(const char *filename) const;

    private:

        uint8_t* data;
        int width;
        int height;
        int bytespp;
    };
}

#endif
