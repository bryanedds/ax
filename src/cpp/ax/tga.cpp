// NOTE: file referenced from https://github.com/ssloy/tinyrenderer/blob/909fe20934ba5334144d2c748805690a1fa4c89f/tgaimage.cpp

#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <math.h>

#include "../../hpp/ax/impl/prelude.hpp"
#include "../../hpp/ax/impl/tga.hpp"

namespace ax
{
    tga_image::tga_image() : tga_image(0, 0) { }

    tga_image::tga_image(int w, int h) : data(), width(w), height(h), bytespp(BGRA)
    {
        uint32_t nbytes = width * height * bytespp;
        data = new uint8_t[nbytes];
        std::memset(data, 0, nbytes);
    }

    tga_image::tga_image(const tga_image& img)
    {
        width = img.width;
        height = img.height;
        bytespp = img.bytespp;
        uint32_t nbytes = width * height * bytespp;
        data = new uint8_t[nbytes];
        memcpy(data, img.data, nbytes);
    }

    tga_image::~tga_image()
    {
        if (data) delete[] data;
    }

    tga_image& tga_image::operator=(const tga_image& img)
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

    color tga_image::get_point(int x, int y) const
    {
        if (x < 0 || y < 0 || x >= width || y >= height) throw std::out_of_range("tga_image point index out of range.");
        VAL tga_color = ax::tga_color(data + (x + (height - y) * width) * bytespp);
        return ax::color(tga_color.r, tga_color.g, tga_color.b, tga_color.a);
    }

    bool tga_image::set_point(int x, int y, const ax::color& color)
    {
        if (x < 0 || y < 0 || x >= width || y >= height) return false;
        VAL tga_color = ax::tga_color(color.b, color.g, color.r, color.a);
        memcpy(data + (x + (height - y) * width) * bytespp, tga_color.raw, bytespp);
        return true;
    }

    void tga_image::clear(const ax::color& color)
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

    bool tga_image::read_tga_file(const char* file_name)
    {
        if (data) delete[] data;
        data = nullptr;
        std::ifstream in;
        in.open(file_name, std::ios::binary);
        if (!in.is_open())
        {
            std::cerr << "can't open file " << file_name << "\n";
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
        if (width <= 0 || height <= 0 || bytespp != BGRA)
        {
            in.close();
            std::cerr << "bad bpp (or width/height) value\n";
            return false;
        }
        uint32_t nbytes = bytespp * width * height;
        data = new uint8_t[nbytes];
        if (3 == header.datatypecode || 2 == header.datatypecode)
        {
            in.read((char*)data, nbytes);
            if (!in.good())
            {
                in.close();
                std::cerr << "an error occured while reading the data\n";
                return false;
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

    bool tga_image::write_tga_file(const char *file_name) const
    {
        uint8_t developer_area_ref[4] = { 0, 0, 0, 0 };
        uint8_t extension_area_ref[4] = { 0, 0, 0, 0 };
        uint8_t footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
        std::ofstream out;
        out.open(file_name, std::ios::binary);
        if (!out.is_open())
        {
            std::cerr << "can't open file " << file_name << "\n";
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
        out.write((char*)data, width*height*bytespp);
        if (!out.good())
        {
            std::cerr << "can't unload raw data\n";
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
