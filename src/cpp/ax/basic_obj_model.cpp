#include "../../hpp/ax/impl/basic_obj_model.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../../hpp/ax/impl/string.hpp"

namespace ax
{
    basic_obj_model::basic_obj_model(const char *file_name) :
        faces(),
        verts(),
        norms(),
        uvs(),
        stream_opt(),
        diffuse_map(),
        normal_map(),
        specular_map()
    {
        std::ifstream in(file_name, std::ifstream::in);
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
                    v3 v;
                    for (int i = 0; i < 3; i++) iss >> v[i];
                    verts.push_back(v);
                }
                else if (!line.compare(0, 3, "vn "))
                {
                    iss >> trash >> trash;
                    v3 n;
                    for (int i = 0; i < 3; i++) iss >> n[i];
                    norms.push_back(n);
                }
                else if (!line.compare(0, 3, "vt "))
                {
                    iss >> trash >> trash;
                    v2 uv;
                    for (int i = 0; i < 2; i++) iss >> uv[i];
                    uvs.push_back(uv);
                }
                else if (!line.compare(0, 2, "f "))
                {
                    std::vector<v3i> f;
                    v3i tmp;
                    iss >> trash;
                    while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
                    {
                        for (int i = 0; i < 3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                        f.push_back(tmp);
                    }
                    faces.push_back(f);
                }
            }

            // TODO: use the map_XX parses instead of hard-coding like this
            // https://en.wikipedia.org/wiki/Wavefront_.obj_file#Texture_maps
            load_texture(file_name, "_diffuse.tga", diffuse_map);
            load_texture(file_name, "_nm_tangent.tga", normal_map);
            load_texture(file_name, "_spec.tga", specular_map);
            return;
        }
        throw std::runtime_error("Invalid model file.");
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

    v3 basic_obj_model::vert(int i) const
    {
        return verts[i];
    }

    v3 basic_obj_model::vert(int iface, int nthvert) const
    {
        return verts[faces[iface][nthvert][0]];
    }

    void basic_obj_model::load_texture(std::string file_name, const char *suffix, tga_image &img)
    {
        std::string texfile(file_name);
        size_t dot = texfile.find_last_of(".");
        if (dot != std::string::npos)
        {
            texfile = texfile.substr(0, dot) + std::string(suffix);
        }
    }

    tga_color basic_obj_model::diffuse(v2 uvf) const
    {
        v2i uv(uvf[0] * diffuse_map.get_width(), uvf[1] * diffuse_map.get_height());
        return diffuse_map.get_point_tga(uv[0], uv[1]);
    }

    v3 basic_obj_model::normal(v2 uvf) const
    {
        v2i uv(uvf[0] * normal_map.get_width(), uvf[1] * normal_map.get_height());
        tga_color c = normal_map.get_point_tga(uv[0], uv[1]);
        v3 res;
        for (int i = 0; i < 3; i++)
        {
            res[2 - i] = (float)c.raw[i] / 255.f*2.f - 1.f;
        }
        return res;
    }

    v2 basic_obj_model::uv(int iface, int nthvert) const
    {
        return uvs[faces[iface][nthvert][1]];
    }

    float basic_obj_model::specular(v2 uvf) const
    {
        v2i uv(uvf[0] * specular_map.get_width(), uvf[1] * specular_map.get_height());
        return specular_map.get_point_tga(uv[0], uv[1]).raw[0] / 1.f;
    }

    v3 basic_obj_model::normal(int iface, int nthvert) const
    {
        int idx = faces[iface][nthvert][2];
        v3 v = norms[idx];
        return v.NormalizeSafe();
    }
}