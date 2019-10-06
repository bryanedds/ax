#ifndef AX_IMPL_MATH_HPP
#define AX_IMPL_MATH_HPP

#include "assimp/color4.inl"
#include "assimp/vector2.inl"
#include "assimp/vector3.inl"

#include "prelude.hpp"

namespace ax
{
    using color = aiColor4t<uint8_t>;
    using v2 = aiVector2t<float>;
    using v3 = aiVector3t<float>;
    using v2i = aiVector2t<int>;
    using v3i = aiVector3t<int>;
    using box2 = std::pair<v2, v2>;
    using line2 = std::pair<v2, v2>;
    using triangle2 = std::tuple<v2, v2, v2>;
    using box3 = std::pair<v3, v3>;
    using line3 = std::pair<v3, v3>;
    using triangle3 = std::tuple<v3, v3, v3>;
}

#endif
