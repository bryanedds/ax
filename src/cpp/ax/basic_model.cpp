#include <iostream>
#include <fstream>
#include <sstream>

#include "ax/basic_model.hpp"

#include "ax/string.hpp"

namespace ax
{
    basic_surface::basic_surface() :
        diffuse_map(),
        normal_map(),
        specular_map()
    { }

    basic_surface::~basic_surface() { }

    void basic_surface::clear()
    {
        diffuse_map.clear();
        normal_map.clear();
        specular_map.clear();
    }

    void basic_surface::load_from_obj(const char* file_path)
    {
        // TODO: use the map_XX parses instead of hard-coding like this.
        // https://en.wikipedia.org/wiki/Wavefront_.obj_file#Texture_maps
        try_load_buffer_from_tga(file_path, "_diffuse.tga", diffuse_map);
        try_load_buffer_from_tga(file_path, "_nm_tangent.tga", normal_map);
        try_load_buffer_from_tga(file_path, "_spec.tga", specular_map);
    }

    bool basic_surface::try_load_buffer_from_tga(std::string file_path, const char* suffix, basic_buffer& buffer)
    {
        std::string texfile(file_path);
        size_t dot = texfile.find_last_of(".");
        if (dot != std::string::npos)
        {
            texfile = texfile.substr(0, dot) + std::string(suffix);
            buffer.try_load_from_tga(texfile.c_str());
            return true;
        }
        return false;
    }

    basic_model::basic_model() :
        faces(),
        positions(),
        uvs(),
        normals(),
        surface()
    { }

    basic_model::~basic_model() { }

    std::vector<int> basic_model::get_face(int index) const
    {
        std::vector<int> face;
        for (int i = 0; i < (int)faces[index].size(); i++) face.push_back(faces[index][i][0]);
        return face;
    }

    ax::v3 basic_model::get_position(int face_index, int vertex_index) const
    {
        return positions[faces[face_index][vertex_index][0]];
    }

    ax::v3 basic_model::get_position(int index) const
    {
        return positions[index];
    }

    ax::v2 basic_model::get_uv(int face_index, int vertex_index) const
    {
        return uvs[faces[face_index][vertex_index][1]];
    }

    ax::v2 basic_model::get_uv(int index) const
    {
        return uvs[index];
    }

    ax::v3 basic_model::get_normal(int face_index, int vertex_index) const
    {
        int idx = faces[face_index][vertex_index][2];
        ax::v3 v = normals[idx];
        return v.NormalizeSafe();
    }

    ax::v3 basic_model::get_normal(int index) const
    {
        return normals[index];
    }

    ax::option<std::string> basic_model::try_load_from_obj(const char *file_path)
    {
        clear();
        std::ifstream in(file_path, std::ifstream::in);
        if (!in.fail())
        {
            std::string line;
            while (!in.eof())
            {
                std::getline(in, line);
                std::istringstream iss(line.c_str());
                char trash;
                if (!line.compare(0, 2, "v "))
                {
                    iss >> trash;
                    ax::v3 v;
                    for (int i = 0; i < 3; i++) iss >> v[i];
                    positions.push_back(v);
                }
                else if (!line.compare(0, 3, "vt "))
                {
                    iss >> trash >> trash;
                    ax::v2 uv;
                    for (int i = 0; i < 2; i++) iss >> uv[i];
                    uvs.push_back(uv);
                }
                else if (!line.compare(0, 3, "vn "))
                {
                    iss >> trash >> trash;
                    ax::v3 n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    normals.push_back(n);
                }
                else if (!line.compare(0, 2, "f "))
                {
                    std::vector<ax::v3i> f;
                    int position, uv, normal;
                    iss >> trash;
                    while (iss >> position >> trash >> uv >> trash >> normal) f.push_back(ax::v3i(position - 1, uv - 1, normal - 1));
                    faces.push_back(f);
                }
            }
            surface.load_from_obj(file_path);
            return ax::none<std::string>();
        }
        return ax::some("Invalid model file '"_s + file_path + "'.");
    }

    void basic_model::clear()
    {
        faces.clear();
        positions.clear();
        uvs.clear();
        normals.clear();
        surface.clear();
    }
}
