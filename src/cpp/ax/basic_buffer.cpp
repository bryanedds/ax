#include <memory>
#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <math.h>

#include "ax/basic_buffer.hpp"

#include "ax/prelude.hpp"
#include "ax/string.hpp"
#include "ax/basic_model.hpp"

namespace ax
{
    basic_buffer::basic_buffer() : basic_buffer(0, 0) { }

    basic_buffer::basic_buffer(int w, int h) : data(), width(w), height(h)
    {
        uint32_t nbytes = width * height * get_bytespp();
        data.reset(new uint8_t[nbytes]);
        std::memset(data.get(), 0, nbytes);
    }

    basic_buffer::basic_buffer(const basic_buffer& image)
    {
        width = image.width;
        height = image.height;
        uint32_t nbytes = width * height * get_bytespp();
        data.reset(new uint8_t[nbytes]);
        memcpy(data.get(), image.data.get(), nbytes);
    }

    basic_buffer& basic_buffer::operator=(const basic_buffer& image)
    {
        if (this != &image)
        {
            width = image.width;
            height = image.height;
            uint32_t nbytes = width * height * get_bytespp();
            data.reset(new uint8_t[nbytes]);
            memcpy(data.get(), image.data.get(), nbytes);
        }
        return *this;
    }

    ax::basic_pixel& basic_buffer::get_pixel_in_place(int x, int y)
    {
        if (x < 0 || y < 0 || x >= width || y >= height) throw std::out_of_range("basic_buffer point index out of range.");
        VAR* pixel = reinterpret_cast<ax::basic_pixel*>(data.get() + ((x + (height - y) * width) * get_bytespp()));
        return *pixel;
    }

    const ax::basic_pixel& basic_buffer::get_pixel(int x, int y) const
    {
        return const_cast<basic_buffer*>(this)->get_pixel_in_place(x, y);
    }

    bool basic_buffer::set_pixel(int x, int y, const ax::basic_pixel& pixel)
    {
        if (x < 0 || y < 0 || x >= width || y >= height) return false;
        VAR& pixel_in_place = get_pixel_in_place(x, y);
        pixel_in_place = pixel;
        return true;
    }

    void basic_buffer::fill(const ax::basic_pixel& pixel)
    {
        VAL bytespp = get_bytespp();
        VAL length = width * height * bytespp;
        for (VAR i = 0; i < length; i += bytespp)
        {
            VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data.get()[i]);
            pixel_in_place = pixel;
        }
    }

    void basic_buffer::flip_vertical()
    {
        VAL line_size = sizeof(ax::basic_pixel) * width;
        VAL full_size = height * line_size;
        VAL& line = std::unique_ptr<char[]>(new char[line_size]);
        for (int j = 0; j < height / 2; ++j)
        {
            VAL cursor = j * line_size;
            std::memcpy(line.get(), &data.get()[cursor], line_size);
            std::memcpy(&data.get()[cursor], &data.get()[full_size - line_size - cursor], line_size);
            std::memcpy(&data.get()[full_size - line_size - cursor], line.get(), line_size);
        }
    }

    void basic_buffer::clear()
    {
        data.reset(new uint8_t[0]);
        width = 0;
        height = 0;
    }

    ax::color basic_buffer::sample_diffuse(const ax::v2& position) const
    {
        if (width < 1 || height < 1) return ax::color(255, 0, 0, 255);
        VAL& positionI = ax::v2i(static_cast<int>(position.x * width), static_cast<int>(position.y * height));
        if (!ax::get_in_bounds(positionI, ax::v2i(width, height))) return ax::color(255, 0, 0, 255);
        VAL& color = get_pixel(positionI.x, positionI.y).color;
        return color;
    }

    ax::v3 basic_buffer::sample_normal(const ax::v2& position) const
    {
        if (width < 1 || height < 1) return ax::v3(0.0f, 0.0f, -1.0f); // TODO: make sure this is forward
        VAL& positionI = ax::v2i(static_cast<int>(position.x * width), static_cast<int>(position.y * height));
        if (!ax::get_in_bounds(positionI, ax::v2i(width, height))) return ax::v3(0.0f, 0.0f, -1.0f); // TODO: make sure this is forward
        VAL& color = get_pixel(positionI.x, positionI.y).color;
        ax::v3 normal;
        for (int i = 0; i < 3; i++)
        {
            VAL scalar = static_cast<float>(color[i]) / 255.f * 2.0f - 1.0f;
            normal[2 - i] = scalar;
        }
        return normal;
    }

    float basic_buffer::sample_specular(const ax::v2& position) const
    {
        if (width < 1 || height < 1) return 0.5f;
        VAL& positionI = ax::v2i(static_cast<int>(position.x * width), static_cast<int>(position.y * height));
        if (!ax::get_in_bounds(positionI, ax::v2i(width, height))) return 0.5f;
        VAL& specular = get_pixel(positionI.x, positionI.y).color.r / 1.0f;
        return specular;
    }

    ax::option<std::string> basic_buffer::try_read_from_tga(const char* file_path)
    {
        // open tga file
        std::ifstream in;
        in.open(file_path, std::ios::binary);
        if (!in.is_open()) return ax::some("Can't open tga file "_s + file_path + ".");
        
        // read tga header
        tga_header header;
        in.read(reinterpret_cast<char*>(&header), sizeof(header));
        if (!in.good()) return ax::some("An error occured while reading tga header."_s);
        
        // read metadata
        VAL width = header.width;
        VAL height = header.height;
        VAL inbytespp = header.bitsperpixel >> 3;
        if (width <= 0 || height <= 0 || (inbytespp != 1 && inbytespp != 3 && inbytespp != 4)) return ax::some("Bad width, height, or bpp value."_s);
        
        // reset fields
        this->width = width;
        this->height = height;
        this->data.reset(new uint8_t[width * height * get_bytespp()]);

        // read content
        if (header.datatypecode == 2 || header.datatypecode == 3)
        {
            VAL& error_opt = try_read_data_raw(inbytespp, in);
            if (error_opt) return error_opt;
        }
        else if (header.datatypecode == 10 || header.datatypecode == 11)
        {
            VAL& error_opt = try_read_data_rle(inbytespp, in);
            if (error_opt) return error_opt;
        }
        else return ax::some("Unknown tga data type code "_s + header.datatypecode + ".");

        // always flip after read due to buffer layout expectations (upside-down)
        flip_vertical();

        // fin
        return ax::none<std::string>();
    }

    ax::option<std::string> basic_buffer::try_write_to_tga(const char *file_path) const
    {
        // flip self because buffer always has image upside-down
        const_cast<basic_buffer*>(this)->flip_vertical();

        // open file for writing
        std::ofstream out;
        out.open(file_path, std::ios::binary);
        if (!out.is_open())
        {
            const_cast<basic_buffer*>(this)->flip_vertical();
            return ax::some("Can't open tga file "_s + file_path + " for saving an ax::basic_buffer.");
        }

        // write header
        tga_header header;
        std::memset(reinterpret_cast<char*>(&header), 0, sizeof(header));
        header.bitsperpixel = static_cast<char>(32);
        header.width = static_cast<short>(width);
        header.height = static_cast<short>(height);
        header.datatypecode = 2;
        out.write(reinterpret_cast<char*>(&header), sizeof(header));        
        if (!out.good())
        {
            const_cast<basic_buffer*>(this)->flip_vertical();
            return ax::some("Failed to write tga header."_s);
        }

        // write content
        VAL bytespp = get_bytespp();
        VAL nbytes = width * height * bytespp;
        for (VAR i = 0; i < nbytes; i += bytespp)
        {
            ax::color color;
            std::memcpy(&color, data.get() + i + 16, sizeof(ax::color));
            color_to_tga(color);
            out.write(reinterpret_cast<char*>(&color), sizeof(ax::color));
            if (!out.good())
            {
                const_cast<basic_buffer*>(this)->flip_vertical();
                return ax::some("Failed to write to ax::basic_buffer to "_s + file_path + ".");
            }
        }

        // write dev area ref
        uint8_t developer_area_ref[4] = { 0, 0, 0, 0 };
        out.write(reinterpret_cast<char*>(developer_area_ref), sizeof(developer_area_ref));
        if (!out.good())
        {
            const_cast<basic_buffer*>(this)->flip_vertical();
            return ax::some("Failed to write to ax::basic_buffer to "_s + file_path + ".");
        }

        // write extension area ref
        uint8_t extension_area_ref[4] = { 0, 0, 0, 0 };
        out.write(reinterpret_cast<char*>(extension_area_ref), sizeof(extension_area_ref));
        if (!out.good())
        {
            const_cast<basic_buffer*>(this)->flip_vertical();
            return ax::some("Failed to write to ax::basic_buffer to "_s + file_path + ".");
        }

        // write footer
        uint8_t footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
        out.write(reinterpret_cast<char*>(footer), sizeof(footer));
        if (!out.good())
        {
            const_cast<basic_buffer*>(this)->flip_vertical();
            return ax::some("Failed to write to ax::basic_buffer to "_s + file_path + ".");
        }

        // unflip
        const_cast<basic_buffer*>(this)->flip_vertical();

        // success
        return ax::none<std::string>();
    }

    ax::option<std::string> basic_buffer::try_read_data_raw(int inbytespp, std::ifstream& in)
    {
        for (VAR j = 0; j < height; ++j)
        {
            for (VAR i = 0; i < width; ++i)
            {
                ax::color color;
                in.read(reinterpret_cast<char*>(&color), inbytespp);
                if (!in.good()) return ax::some("An error occured while reading tga data."_s);
                color_from_tga(inbytespp, color);
                VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data.get()[j * width + i]);   
                pixel_in_place = ax::basic_pixel(std::numeric_limits<float>::lowest(), ax::zero<ax::v3>(), color);
            }
        }
        return ax::none<std::string>();
    }

    ax::option<std::string> basic_buffer::try_read_data_rle(int inbytespp, std::ifstream& in)
    {
        VAL dataLength = width * height * get_bytespp();
        for (VAR i = 0; i < dataLength; i)
        {
            VAR chunkheader = static_cast<uint8_t>(in.get());
            if (!in.good()) return ax::some("An error occured while reading tga rle data."_s);
            if (chunkheader < 128)
            {
                ++chunkheader;
                for (int j = 0; j < chunkheader; ++j)
                {
                    ax::color color;
                    in.read(reinterpret_cast<char*>(&color), inbytespp);
                    if (!in.good()) return ax::some("An error occured while reading tga rle color."_s);
                    color_from_tga(inbytespp, color);
                    VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data.get()[i]);
                    pixel_in_place = ax::basic_pixel(std::numeric_limits<float>::lowest(), ax::zero<ax::v3>(), color);
                    i += get_bytespp();
                }
            }
            else
            {
                chunkheader -= 127;
                ax::color color;
                in.read(reinterpret_cast<char*>(&color), inbytespp);
                if (!in.good()) return ax::some("An error occured while reading tga rle color."_s);
                color_from_tga(inbytespp, color);
                for (int j = 0; j < chunkheader; ++j)
                {
                    VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data.get()[i]);
                    pixel_in_place = ax::basic_pixel(std::numeric_limits<float>::lowest(), ax::zero<ax::v3>(), color);
                    i += get_bytespp();
                }
            }
        }
        return ax::none<std::string>();
    }

    void basic_buffer::color_from_tga(int inbytespp, ax::color& color) const
    {
        if (inbytespp < 3) color.g = color.b = color.r;
        if (inbytespp < 4) color.a = 255;
        std::swap(color.r, color.b);
    }

    void basic_buffer::color_to_tga(ax::color& color) const
    {
        std::swap(color.r, color.b);
    }
}
