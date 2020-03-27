#ifndef AX_BASIC_MODEL_HPP
#define AX_BASIC_MODEL_HPP

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
        ax::v3 position;
        ax::v3 normal;
        ax::v2 uv;
    };

    struct basic_model
    {
    public:

        basic_model();
        ~basic_model();
        
        std::vector<int> get_face(int index) const;
        ax::v3 get_position(int face_index, int vertex_index) const;
        ax::v3 get_position(int index) const;
        ax::v2 get_uv(int face_index, int vertex_index) const;
        ax::v2 get_uv(int index) const;
        ax::v3 get_normal(int face_index, int vertex_index) const;
        ax::v3 get_normal(int index) const;
        
        const std::vector<std::vector<ax::v3i>>& get_faces() const { return faces; }
        const std::vector<ax::v3>& get_positions() const { return positions; }
        const std::vector<ax::v2>& get_uvs() const { return uvs; }
        const std::vector<ax::v3>& get_normals() const { return normals; }
		const ax::basic_buffer& get_diffuse_map() const { return diffuse_map; }
		const ax::basic_buffer& get_normal_map() const { return normal_map; }
		const ax::basic_buffer& get_specular_map() const { return specular_map; }
        void load_from_obj(const char* file_path);
        void clear();

    private:

        bool try_load_texture(std::string file_path, const char *suffix, ax::basic_buffer &buffer);

        // Faces in terms of position / uv / normal.
        std::vector<std::vector<ax::v3i>> faces; 
        std::vector<ax::v3> positions;
        std::vector<ax::v2> uvs;
        std::vector<ax::v3> normals;
        ax::basic_buffer diffuse_map;
        ax::basic_buffer normal_map;
        ax::basic_buffer specular_map;
    };
}

#endif
