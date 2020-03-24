#ifndef AX_BASIC_OBJ_MODEL_HPP
#define AX_BASIC_OBJ_MODEL_HPP

#include <vector>
#include <string>
#include <iosfwd>

#include "math.hpp"
#include "option.hpp"
#include "basic_buffer.hpp"

namespace ax
{
    struct basic_vertex
    {
        ax::v3 vertex;
        ax::v3 normal;
        ax::v2 uv;
    };

    struct basic_obj_model
    {
    public:

        basic_obj_model(const char *file_path);
        ~basic_obj_model();

        int get_vertex_count() const;
        int get_face_count() const;
        ax::v3 get_normal(int iface, int nthvert) const;
        ax::v3 get_normal(ax::v2 uv) const;
        ax::v3 get_vertex(int i) const;
        ax::v3 get_vertex(int iface, int nthvert) const;
        ax::v2 get_uv(int iface, int nthvert) const;
        ax::color get_color_diffuse(ax::v2 uv) const;
        float get_specularity(ax::v2 uv) const;
        std::vector<int> get_face(int idx) const;

    private:

        bool try_load_texture(std::string file_path, const char *suffix, ax::basic_buffer &img);

        std::vector<std::vector<ax::v3i>> faces; // NOTE: this v3i means vertex/uv/normal. TODO: make it mean vertex/normal/uv instead.
        std::vector<ax::v3> vertices;
        std::vector<ax::v3> normals;
        std::vector<ax::v2> uvs;
        ax::option<std::vector<ax::basic_vertex>> stream_opt;
        ax::basic_buffer diffuse_map;
        ax::basic_buffer normal_map;
        ax::basic_buffer specular_map;
    };
}

#endif
