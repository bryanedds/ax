#ifndef AX_BASIC_BUFFER_HPP
#define AX_BASIC_BUFFER_HPP

#include "prelude.hpp"
#include "math.hpp"
#include "tga.hpp"

namespace ax
{
    struct basic_cell
    {
    public:
        basic_cell(float depth, const ax::v3& normal, const ax::color& color) : depth(depth), normal(normal), color(color) { }
        basic_cell() = default;
        ~basic_cell() = default;
        basic_cell& operator=(const basic_cell&) = default;
		float depth;
		ax::v3 normal;
        ax::color color; // RGBA
    };

    struct basic_buffer
    {
    public:

        basic_buffer();
        basic_buffer(int w, int h);
        basic_buffer(const basic_buffer& img);
        ~basic_buffer();

        basic_buffer& operator=(const basic_buffer& img);

        constexpr int get_bytespp() const { return sizeof(ax::basic_cell); }
        int get_width() const { return width; };
        int get_height() const { return height; };
        ax::basic_cell get_cell(int x, int y) const;
        bool set_point(int x, int y, const ax::basic_cell& cell);
        void flood(const ax::basic_cell& cell);

        bool read_from_tga_file(const char *filename);
        bool write_to_tga_file(const char *filename) const;

    private:

        uint8_t* data;
        int width;
        int height;
    };
}

#endif
