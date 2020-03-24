#include <iostream>
#include <fstream>
#include <sstream>

#include "ax/basic_obj_model.hpp"

#include "ax/string.hpp"

namespace ax
{
    basic_obj_model::basic_obj_model(const char *file_path) :
        faces(),
        verts(),
        norms(),
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
                    verts.push_back(v);
                }
                else if (!line.compare(0, 3, "vn "))
                {
                    iss >> trash >> trash;
                    ax::v3 n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    norms.push_back(n);
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

    int basic_obj_model::nverts() const
    {
        return (int)verts.size();
    }

    int basic_obj_model::nfaces() const
    {
        return (int)faces.size();
    }

    std::vector<int> basic_obj_model::face(int idx) const
    {
        std::vector<int> face;
        for (int i = 0; i < (int)faces[idx].size(); i++) face.push_back(faces[idx][i][0]);
        return face;
    }

    ax::v3 basic_obj_model::vert(int i) const
    {
        return verts[i];
    }

    ax::v3 basic_obj_model::vert(int iface, int nthvert) const
    {
        return verts[faces[iface][nthvert][0]];
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

    ax::color basic_obj_model::diffuse(ax::v2 uvf) const
    {
        ax::v2i uv(
            static_cast<int>(uvf[0] * diffuse_map.get_width()),
            static_cast<int>(uvf[1] * diffuse_map.get_height()));
        return diffuse_map.get_cell(uv[0], uv[1]).color;
    }

    ax::v3 basic_obj_model::normal(ax::v2 uvf) const
    {
        ax::v2i uv(
            static_cast<int>(uvf[0] * normal_map.get_width()),
            static_cast<int>(uvf[1] * normal_map.get_height()));
        ax::color c = normal_map.get_cell(uv[0], uv[1]).color;
        ax::v3 res;
        for (int i = 0; i < 3; i++) res[2 - i] = (float)c[i] / 255.f * 2.f - 1.f;
        return res;
    }

    ax::v2 basic_obj_model::uv(int iface, int nthvert) const
    {
        return uvs[faces[iface][nthvert][1]];
    }

    float basic_obj_model::specular(ax::v2 uvf) const
    {
        ax::v2i uv(
            static_cast<int>(uvf[0] * specular_map.get_width()),
            static_cast<int>(uvf[1] * specular_map.get_height()));
        return specular_map.get_cell(uv[0], uv[1]).color[0] / 1.f;
    }

    ax::v3 basic_obj_model::normal(int iface, int nthvert) const
    {
        int idx = faces[iface][nthvert][2];
        ax::v3 v = norms[idx];
        return v.NormalizeSafe();
    }
}
