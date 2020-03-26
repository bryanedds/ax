#include <iostream>
#include <fstream>
#include <functional>
#include <string>
#include <math.h>

#include "ax/basic_buffer.hpp"

#include "ax/prelude.hpp"
#include "ax/basic_obj_model.hpp"

namespace ax
{
    basic_buffer::basic_buffer() : basic_buffer(0, 0) { }

    basic_buffer::basic_buffer(int w, int h) : data(), width(w), height(h)
    {
        uint32_t nbytes = width * height * get_bytespp();
        data = new uint8_t[nbytes];
        std::memset(data, 0, nbytes);
    }

    basic_buffer::basic_buffer(const basic_buffer& img)
    {
        width = img.width;
        height = img.height;
        uint32_t nbytes = width * height * get_bytespp();
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
            uint32_t nbytes = width * height * get_bytespp();
            data = new uint8_t[nbytes];
            memcpy(data, img.data, nbytes);
        }
        return *this;
    }

    ax::basic_pixel& basic_buffer::get_pixel_in_place(int x, int y)
    {
        if (x < 0 || y < 0 || x >= width || y >= height) throw std::out_of_range("basic_buffer point index out of range.");
        VAR* pixel = reinterpret_cast<ax::basic_pixel*>(data + ((x + (height - y) * width) * get_bytespp()));
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

    void basic_buffer::flood(const ax::basic_pixel& pixel)
    {
		VAL bytespp = get_bytespp();
        VAL length = width * height * bytespp;
        for (VAR i = 0; i < length; i += bytespp)
        {
            VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data[i]);
            pixel_in_place = pixel;
        }
    }

	void basic_buffer::flip()
	{
		// TODO: make this allocation exception-safe!
		VAL line_size = sizeof(ax::basic_pixel) * width;
		VAL size = height * line_size;
		VAR* line = new char[line_size];
		for (int j = 0; j < height / 2; ++j)
		{
			VAL cursor = j * line_size;
			std::memcpy(line, &data[cursor], line_size);
			std::memcpy(&data[cursor], &data[size - line_size - cursor], line_size);
			std::memcpy(&data[size - line_size - cursor], line, line_size);
		}
		delete[] line;
	}

    ax::color basic_buffer::sample_diffuse(const ax::v2& position) const
    {
        VAL& positionI = ax::v2i(static_cast<int>(position.x * width), static_cast<int>(position.y * height));
        VAL& color = get_pixel(positionI.x, positionI.y).color;
		return color;
    }

    ax::v3 basic_buffer::sample_normal(const ax::v2& position) const
    {
        VAL& positionI = ax::v2i(static_cast<int>(position.x * width), static_cast<int>(position.y * height));
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
		VAL& positionI = ax::v2i(static_cast<int>(position.x * width), static_cast<int>(position.y * height));
        VAL& specular = get_pixel(positionI.x, positionI.y).color.r / 1.0f;
        return specular;
    }

    // TODO: clean up this code. it's terrible and not exception-safe!
    bool basic_buffer::read_from_tga_file(const char* file_path)
    {
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
        VAL inbytespp = header.bitsperpixel >> 3;
        if (width <= 0 || height <= 0 || (inbytespp != 1 && inbytespp != 3 && inbytespp != 4))
        {
            in.close();
            std::cerr << "bad width, height, or bpp value\n";
            return false;
        }
        data = new uint8_t[width * height * get_bytespp()];
        if (header.datatypecode == 2 || header.datatypecode == 3)
        {
			for (VAR j = 0; j < height; ++j)
			{
				for (VAR i = 0; i < width; ++i)
				{
					VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data[j * width + i]);
					pixel_in_place.depth = std::numeric_limits<float>::lowest();
					pixel_in_place.normal = ax::zero<ax::v3>();
					in.read((char*)&pixel_in_place.color, sizeof(ax::color));
					if (inbytespp < 3) pixel_in_place.color.g = pixel_in_place.color.b = pixel_in_place.color.r;
					if (inbytespp < 4) pixel_in_place.color.a = 255;
					std::swap(pixel_in_place.color.r, pixel_in_place.color.b);
				}
			}
            if (!in.good())
            {
                in.close();
                std::cerr << "an error occured while reading the data\n";
                return false;
            }
        }
        else if (header.datatypecode == 10 || header.datatypecode == 11)
        {
			if (!load_rle_data(inbytespp, in))
			{
				in.close();
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
        }
        else
        {
            in.close();
            std::cerr << "unknown file format " << (int)header.datatypecode << "\n";
            return false;
        }
		flip();
        std::cerr << width << "x" << height << "/" << inbytespp * 8 << "\n";
        in.close();
        return true;
    }

	// TODO: clean up this code. it's terrible and not exception-safe!
	bool basic_buffer::write_to_tga_file(const char *file_path, bool flip) const
    {
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
        header.bitsperpixel = static_cast<char>(32);
        header.width = static_cast<short>(width);
        header.height = static_cast<short>(height);
        header.datatypecode = 2;
        header.imagedescriptor = flip ? 0x20 : 0x0;
        out.write((char*)&header, sizeof(header));
        if (!out.good())
        {
            out.close();
            std::cerr << "can't dump the tga file\n";
            return false;
        }
		VAL bytespp = get_bytespp();
        VAL nbytes = width * height * bytespp;
        for (VAR i = 0; i < nbytes; i += bytespp)
        {
            VAR* r = data + i + 16;
            VAR* g = data + i + 17;
            VAR* b = data + i + 18;
            VAR* a = data + i + 19;
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

	// TODO: clean up this code.
	bool basic_buffer::load_rle_data(int inbytespp, std::ifstream& in)
	{
		VAL dataLength = width * height * get_bytespp();
		for (VAR i = 0; i < dataLength; i)
		{
			VAR chunkheader = static_cast<unsigned char>(in.get());
			if (!in.good())
			{
				std::cerr << "an error occured while reading the data\n";
				return false;
			}
			if (chunkheader < 128)
			{
				++chunkheader;
				for (int j = 0; j < chunkheader; ++j)
				{
					ax::color color;
					in.read((char*)&color, inbytespp);
					if (inbytespp < 3) color.g = color.b = color.r;
					if (inbytespp < 4) color.a = 255;
					std::swap(color.r, color.b);
					if (!in.good())
					{
						std::cerr << "an error occured while reading the header\n";
						return false;
					}
					else
					{
						VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data[i]);
						pixel_in_place.depth = std::numeric_limits<float>::lowest();
						pixel_in_place.normal = ax::zero<ax::v3>();
						pixel_in_place.color = color;
						i += get_bytespp();
					}
				}
			}
			else
			{
				chunkheader -= 127;
				ax::color color;
				in.read((char*)&color, inbytespp);
				if (inbytespp < 3) color.g = color.b = color.r;
				if (inbytespp < 4) color.a = 255;
				std::swap(color.r, color.b);
				if (!in.good())
				{
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
				else
				{
					for (int j = 0; j < chunkheader; ++j)
					{
						VAR& pixel_in_place = reinterpret_cast<ax::basic_pixel&>(data[i]);
						pixel_in_place.depth = std::numeric_limits<float>::lowest();
						pixel_in_place.normal = ax::zero<ax::v3>();
						pixel_in_place.color = color;
						i += get_bytespp();
					}
				}
			}
		}
		return true;
	}
}
