#include <iostream>
#include <fstream>
#include <sstream>

#include "ax/basic_obj_model.hpp"

#include "ax/string.hpp"

namespace ax
{
    basic_obj_model::basic_obj_model(const char *file_path) :
        faces(),
        vertices(),
        normals(),
        uvs(),
        stream_opt(),
        diffuse_map(),
        normal_map(),
        specular_map()
    {
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
                    vertices.push_back(v);
                }
                else if (!line.compare(0, 3, "vn "))
                {
                    iss >> trash >> trash;
                    ax::v3 n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    normals.push_back(n);
                }
                else if (!line.compare(0, 3, "vt "))
                {
                    iss >> trash >> trash;
                    ax::v2 uv;
                    for (int i = 0; i < 2; i++) iss >> uv[i];
                    uvs.push_back(uv);
                }
                else if (!line.compare(0, 2, "f "))
                {
                    std::vector<ax::v3i> f;
                    int a, b, c;
                    iss >> trash;
                    while (iss >> a >> trash >> b >> trash >> c)
                    {
                        --a;
                        --b;
                        --c;
                        f.push_back(ax::v3i(a, b, c));
                    }
                    faces.push_back(f);
                }
            }

            // TODO: use the map_XX parses instead of hard-coding like this.
            // TODO: check results.
            // https://en.wikipedia.org/wiki/Wavefront_.obj_file#Texture_maps
			try_load_texture(file_path, "_diffuse.tga", diffuse_map);
            try_load_texture(file_path, "_nm_tangent.tga", normal_map);
            try_load_texture(file_path, "_spec.tga", specular_map);
            return;
        }
        throw std::runtime_error("Invalid model file '"_s + file_path + "'.");
    }

    basic_obj_model::~basic_obj_model() { }

    int basic_obj_model::get_vertex_count() const
    {
        return (int)vertices.size();
    }

    int basic_obj_model::get_face_count() const
    {
        return (int)faces.size();
    }

    std::vector<int> basic_obj_model::get_face(int idx) const
    {
        std::vector<int> face;
        for (int i = 0; i < (int)faces[idx].size(); i++) face.push_back(faces[idx][i][0]);
        return face;
    }

    ax::v3 basic_obj_model::get_vertex(int i) const
    {
        return vertices[i];
    }

    ax::v3 basic_obj_model::get_vertex(int iface, int nthvert) const
    {
        return vertices[faces[iface][nthvert][0]];
    }

    ax::v3 basic_obj_model::get_normal(int iface, int nthvert) const
    {
        int idx = faces[iface][nthvert][2];
        ax::v3 v = normals[idx];
        return v.NormalizeSafe();
    }

    ax::v2 basic_obj_model::get_uv(int iface, int nthvert) const
    {
        return uvs[faces[iface][nthvert][1]];
    }

    bool basic_obj_model::try_load_texture(std::string file_path, const char* suffix, basic_buffer& img)
    {
        std::string texfile(file_path);
        size_t dot = texfile.find_last_of(".");
        if (dot != std::string::npos)
        {
            texfile = texfile.substr(0, dot) + std::string(suffix);
            return img.read_from_tga_file(texfile.c_str());
        }
        return false;
    }
}
