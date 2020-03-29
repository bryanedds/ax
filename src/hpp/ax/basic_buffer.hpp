#ifndef AX_BASIC_BUFFER_HPP
#define AX_BASIC_BUFFER_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "option.hpp"
#include "tga.hpp"

namespace ax
{
    struct basic_pixel
    {
    public:

        basic_pixel(float depth, const ax::v3& normal, const ax::color& color) : depth(depth), normal(normal), color(color) { }
        basic_pixel() = default;
        ~basic_pixel() = default;
        basic_pixel& operator=(const basic_pixel&) = default;

        float depth;
        ax::v3 normal;
        ax::color color;
    };

    struct basic_buffer
    {
    public:

        basic_buffer();
        basic_buffer(int w, int h);
        basic_buffer(const basic_buffer& image);
        ~basic_buffer() = default;
        basic_buffer& operator=(const basic_buffer& image);

        int get_width() const { return width; };
        int get_height() const { return height; };
        constexpr int get_bytespp() const { return sizeof(ax::basic_pixel); }
        ax::basic_pixel& get_pixel_in_place(int x, int y);
        const ax::basic_pixel& get_pixel(int x, int y) const;
        bool set_pixel(int x, int y, const ax::basic_pixel& pixel);
        void fill(const ax::basic_pixel& pixel);
        void flip_vertical();
        void clear();

        ax::color sample_diffuse(const ax::v2& position) const;
        ax::v3 sample_normal(const ax::v2& position) const;
        float sample_specular(const ax::v2& position) const;

        ax::option<std::string> try_write_to_tga(const char* filename) const;
        ax::option<std::string> try_read_from_tga(const char* filename);

    private:

        ax::option<std::string> try_read_data_raw(int inbytespp, std::ifstream& in);
        ax::option<std::string> try_read_data_rle(int inbytespp, std::ifstream& in);
        void color_from_tga(int inbytespp, ax::color& color) const;
        void color_to_tga(ax::color& color) const;

        std::unique_ptr<uint8_t> data;
        int width;
        int height;
    };
}

#endif
