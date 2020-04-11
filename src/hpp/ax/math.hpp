#ifndef AX_MATH_HPP
#define AX_MATH_HPP

#include "assimp/color4.inl"
#include "assimp/vector2.inl"
#include "assimp/vector3.inl"
#include "assimp/quaternion.inl"
#include "assimp/matrix3x3.inl"
#include "assimp/matrix4x4.inl"

#include "prelude.hpp"

namespace ax
{
    using color = aiColor4t<uint8_t>;
    using v2 = aiVector2t<float>;
    using v3 = aiVector3t<float>;
    using v2i = aiVector2t<int>;
    using v3i = aiVector3t<int>;
    using box2 = std::pair<v2, v2>;
    using box2i = std::pair<v2i, v2i>;
    using line2 = std::pair<v2, v2>;
    using triangle2 = std::tuple<v2, v2, v2>;
    using box3 = std::pair<v3, v3>;
    using line3 = std::pair<v3, v3>;
    using triangle3 = std::tuple<v3, v3, v3>;
    using quaternion = aiQuaterniont<float>;
    using matrix3 = aiMatrix3x3t<float>;
    using matrix4 = aiMatrix4x4t<float>;

    ax::v2 get_ortho(const ax::v3& vector);
    ax::line2 get_ortho(const ax::line3& line);
    ax::triangle2 get_ortho(const ax::triangle3& triangle);
    float get_depth(const ax::v2& point, const ax::triangle3& triangle);
    ax::v2 get_interpolation(const ax::v2& point, const ax::triangle2& uvs);
    ax::v3 get_normal(const ax::triangle3& triangle);
    ax::v3 get_tangent(const ax::triangle3& triangle);
    ax::box2 get_bounds(const ax::triangle2& triangle);
    ax::box2 get_intersection(const ax::box2& box, const ax::box2& box2);
    ax::v3 get_barycentric_coords(ax::v2 point, const ax::triangle2& triangle);
    bool get_in_bounds(const ax::v2& point, const ax::triangle2& triangle);
    bool get_in_bounds(const ax::v2& point, const ax::box2& box);
    bool get_in_bounds(const ax::v2i& point, const ax::box2i& box);
    bool get_in_bounds(const ax::v2& point, const ax::v2& size);
    bool get_in_bounds(const ax::v2i& point, const ax::v2i& size);
}

#endif
