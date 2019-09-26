#include "../../hpp/ax/impl/basic_buffer.hpp"

#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <math.h>

#include "../../hpp/ax/impl/prelude.hpp"
#include "../../hpp/ax/impl/basic_obj_model.hpp"

namespace ax
{
    basic_buffer::basic_buffer() : basic_buffer(0, 0) { }

    basic_buffer::basic_buffer(int w, int h) : data(), width(w), height(h), bytespp(BUFFER_RGBA)
    {
        uint32_t nbytes = width * height * bytespp;
        data = new uint8_t[nbytes];
        std::memset(data, 0, nbytes);
    }

    basic_buffer::basic_buffer(const basic_buffer& img)
    {
        width = img.width;
        height = img.height;
        bytespp = img.bytespp;
        uint32_t nbytes = width * height * bytespp;
        data = new uint8_t[nbytes];
        memcpy(data, img.data, nbytes);
    }

    basic_buffer::~basic_buffer()
    {
        if (data) delete[] data;
    }

    basic_buffer& basic_buffer::operator=(const basic_buffer& img)
    {
        if (this != &img)
        {
            if (data) delete[] data;
            width = img.width;
            height = img.height;
            bytespp = img.bytespp;
            uint32_t nbytes = width * height * bytespp;
            data = new uint8_t[nbytes];
            memcpy(data, img.data, nbytes);
        }
        return *this;
    }

    ax::color basic_buffer::get_point(int x, int y) const
    {
        if (x < 0 || y < 0 || x >= width || y >= height) throw std::out_of_range("basic_buffer point index out of range.");
        VAL* color = reinterpret_cast<ax::color*>(data + ((x + (height - y) * width) * bytespp));
        return *color;
    }

    bool basic_buffer::set_point(int x, int y, const ax::color& color)
    {
        if (x < 0 || y < 0 || x >= width || y >= height) return false;
        memcpy(data + (x + (height - y) * width) * bytespp, &color, bytespp);
        return true;
    }

    void basic_buffer::clear(const ax::color& color)
    {
        VAL length = width * height * bytespp;
        for (VAR i = 0; i < length; i += bytespp)
        {
            data[i] = color.b;
            data[i+1] = color.g;
            data[i+2] = color.r;
            data[i+3] = color.a;
        }
    }

    bool basic_buffer::read_from_tga_file(const char* file_path)
    {
        // TODO: clean up this code. it's terrible and not exception-safe!
        if (data) delete[] data;
        data = nullptr;
        std::ifstream in;
        in.open(file_path, std::ios::binary);
        if (!in.is_open())
        {
            std::cerr << "can't open file " << file_path << "\n";
            in.close();
            return false;
        }
        tga_header header;
        in.read((char*)&header, sizeof(header));
        if (!in.good())
        {
            in.close();
            std::cerr << "an error occured while reading the header\n";
            return false;
        }
        width = header.width;
        height = header.height;
        bytespp = header.bitsperpixel >> 3;
        if (width <= 0 || height <= 0 || bytespp != TGA_BGRA)
        {
            in.close();
            std::cerr << "bad bpp (or width/height) value\n";
            return false;
        }
        uint32_t nbytes = width * height * bytespp;
        data = new uint8_t[nbytes]; // TODO: make this a local var then swap contents with field on success
        if (3 == header.datatypecode || 2 == header.datatypecode)
        {
            in.read((char*)data, nbytes);
            if (!in.good())
            {
                in.close();
                std::cerr << "an error occured while reading the data\n";
                return false;
            }
            for (VAR i = 0u; i < nbytes; i += bytespp)
            {
                VAR* zero = data + i;
                VAR* two = data + i + 2;
                std::swap(*zero, *two);
            }
        }
        else if (10 == header.datatypecode || 11 == header.datatypecode)
        {
            in.close();
            std::cerr << "rle not supported\n";
            return false;
        }
        else
        {
            in.close();
            std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
            return false;
        }
        std::cerr << width << "x" << height << "/" << bytespp * 8 << "\n";
        in.close();
        return true;
    }

    bool basic_buffer::write_to_tga_file(const char *file_path) const
    {
        // TODO: clean up this code. it's terrible.
        uint8_t developer_area_ref[4] = { 0, 0, 0, 0 };
        uint8_t extension_area_ref[4] = { 0, 0, 0, 0 };
        uint8_t footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
        std::ofstream out;
        out.open(file_path, std::ios::binary);
        if (!out.is_open())
        {
            std::cerr << "can't open file " << file_path << "\n";
            out.close();
            return false;
        }
        tga_header header;
        std::memset((void*)&header, 0, sizeof(header));
        header.bitsperpixel = static_cast<char>(bytespp << 3);
        header.width = static_cast<short>(width);
        header.height = static_cast<short>(height);
        header.datatypecode = 2;
        header.imagedescriptor = 0x20; // top-left origin
        out.write((char*)&header, sizeof(header));
        if (!out.good())
        {
            out.close();
            std::cerr << "can't dump the tga file\n";
            return false;
        }
        VAL nbytes = width * height * bytespp;
        for (VAR i = 0; i < nbytes; i += bytespp)
        {
            VAR* r = data + i;
            VAR* g = data + i + 1;
            VAR* b = data + i + 2;
            VAR* a = data + i + 3;
            out.write((char*)b, 1);
            out.write((char*)g, 1);
            out.write((char*)r, 1);
            out.write((char*)a, 1);
        }
        if (!out.good())
        {
            std::cerr << "can't dump raw data\n";
            out.close();
            return false;
        }
        out.write((char*)developer_area_ref, sizeof(developer_area_ref));
        if (!out.good())
        {
            std::cerr << "can't dump the tga file\n";
            out.close();
            return false;
        }
        out.write((char*)extension_area_ref, sizeof(extension_area_ref));
        if (!out.good())
        {
            std::cerr << "can't dump the tga file\n";
            out.close();
            return false;
        }
        out.write((char*)footer, sizeof(footer));
        if (!out.good())
        {
            std::cerr << "can't dump the tga file\n";
            out.close();
            return false;
        }
        out.close();
        return true;
    }
}
