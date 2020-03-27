#ifndef AX_BASIC_BUFFER_HPP
#define AX_BASIC_BUFFER_HPP

#include "prelude.hpp"
#include "math.hpp"
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
        ~basic_buffer();
        basic_buffer& operator=(const basic_buffer& image);

        int get_width() const { return width; };
        int get_height() const { return height; };
        constexpr int get_bytespp() const { return sizeof(ax::basic_pixel); }
        ax::basic_pixel& get_pixel_in_place(int x, int y);
        const ax::basic_pixel& get_pixel(int x, int y) const;
        bool set_pixel(int x, int y, const ax::basic_pixel& pixel);
        void fill(const ax::basic_pixel& pixel);
		void flip_horizontal();
        void clear();

        ax::color sample_diffuse(const ax::v2& position) const;
        ax::v3 sample_normal(const ax::v2& position) const;
        float sample_specular(const ax::v2& position) const;

        bool load_from_tga(const char *filename);
        bool save_to_tga(const char *filename);

    private:

		bool load_rle_data(int inbytespp, std::ifstream& in);

        uint8_t* data; // TODO: P1: use a smart ptr here.
        int width;
        int height;
    };
}

#endif
