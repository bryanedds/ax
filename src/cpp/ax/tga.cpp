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
    tga_image::tga_image() : data(nullptr), width(0), height(0), bytespp(0) { }

    tga_image::tga_image(int w, int h, int bpp) : data(nullptr), width(w), height(h), bytespp(bpp)
    {
        unsigned long nbytes = width * height*bytespp;
        data = new unsigned char[nbytes];
        std::memset(data, 0, nbytes);
    }

    tga_image::tga_image(const tga_image& img)
    {
        width = img.width;
        height = img.height;
        bytespp = img.bytespp;
        unsigned long nbytes = width * height * bytespp;
        data = new unsigned char[nbytes];
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
            unsigned long nbytes = width * height*bytespp;
            data = new unsigned char[nbytes];
            memcpy(data, img.data, nbytes);
        }
        return *this;
    }

    tga_color tga_image::get_point(int x, int y)
    {
        if (!data || x < 0 || y < 0 || x >= width || y >= height) return tga_color();
        return tga_color(data + (x + y * width) * bytespp, bytespp);
    }

    bool tga_image::set_point(int x, int y, const tga_color& c)
    {
        if (!data || x < 0 || y < 0 || x >= width || y >= height) return false;
        memcpy(data + (x + y * width)*bytespp, c.raw, bytespp);
        return true;
    }

    void tga_image::draw_line(int x, int y, int x2, int y2, const tga_color& c)
    {
        // local functions
        struct local
        {
            static void set_point_normal(int x, int y, const tga_color& c, tga_image& image) { image.set_point(x, y, c); }
            static void set_point_swapped(int x, int y, const tga_color& c, tga_image& image) { image.set_point(y, x, c); }
        };

        // determine steepness
        VAL steep = std::abs(x - x2) < std::abs(y - y2);

        // transpose
        if (steep)
        {
            std::swap(x, y);
            std::swap(x2, y2);
        }

        // invert
        if (x > x2)
        {
            std::swap(x, x2);
            std::swap(y, y2);
        }

        // draw loop
        VAL x_delta = x2 - x;
        VAL y_delta = y2 - y;
        VAL error_delta = std::abs(y_delta) * 2;
        VAL error_heading = y2 > y ? 1 : -1;
        VAL set_point_local = steep ? &local::set_point_swapped : &local::set_point_normal;
        for (VAR error = 0, i = x, j = y; i < x2; ++i)
        {
            set_point_local(i, j, c, *this);
            error += error_delta;
            if (error > x_delta)
            {
                j += error_heading;
                error -= x_delta * 2;
            }
        }
    }

    void tga_image::clear()
    {
        std::memset(data, 0, width * height * bytespp);
    }

    bool tga_image::read_tga_file(const char *filename)
    {
        if (data) delete[] data;
        data = nullptr;
        std::ifstream in;
        in.open(filename, std::ios::binary);
        if (!in.is_open())
        {
            std::cerr << "can't open file " << filename << "\n";
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
        if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA))
        {
            in.close();
            std::cerr << "bad bpp (or width/height) value\n";
            return false;
        }
        unsigned long nbytes = bytespp * width*height;
        data = new unsigned char[nbytes];
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

    bool tga_image::write_tga_file(const char *filename)
    {
        unsigned char developer_area_ref[4] = { 0, 0, 0, 0 };
        unsigned char extension_area_ref[4] = { 0, 0, 0, 0 };
        unsigned char footer[18] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };
        std::ofstream out;
        out.open(filename, std::ios::binary);
        if (!out.is_open())
        {
            std::cerr << "can't open file " << filename << "\n";
            out.close();
            return false;
        }
        tga_header header;
        std::memset((void*)&header, 0, sizeof(header));
        header.bitsperpixel = static_cast<char>(bytespp << 3);
        header.width = static_cast<short>(width);
        header.height = static_cast<short>(height);
        header.datatypecode = bytespp == GRAYSCALE ? 3 : 2;
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
