#ifndef IMPL_AX_BASIC_OBJ_MODEL_HPP
#define IMPL_AX_BASIC_OBJ_MODEL_HPP

#include <vector>
#include <string>
#include <iosfwd>

#include "math.hpp"
#include "option.hpp"
#include "tga.hpp"

namespace ax
{
    struct basic_vert
    {
        ax::v3 vert;
        ax::v3 norm;
        ax::v2 uv;
    };

    struct basic_obj_model
    {
    public:

        basic_obj_model(const char *file_name);
        ~basic_obj_model();

        int nverts() const;
        int nfaces() const;
        ax::v3 normal(int iface, int nthvert) const;
        ax::v3 normal(ax::v2 uv) const;
        ax::v3 vert(int i) const;
        ax::v3 vert(int iface, int nthvert) const;
        ax::v2 uv(int iface, int nthvert) const;
        ax::color diffuse(ax::v2 uv) const;
        float specular(ax::v2 uv) const;
        std::vector<int> face(int idx) const;

    private:

        bool try_load_texture(std::string file_name, const char *suffix, ax::tga_image &img);

        std::vector<std::vector<ax::v3i>> faces; // NOTE: this v3i means vertex/uv/normal. TODO: make it mean vertex/normal/uv instead.
        std::vector<ax::v3> verts;
        std::vector<ax::v3> norms;
        std::vector<ax::v2> uvs;
        ax::option<std::vector<ax::basic_vert>> stream_opt;
        ax::tga_image diffuse_map;
        ax::tga_image normal_map;
        ax::tga_image specular_map;
    };
}

#endif
