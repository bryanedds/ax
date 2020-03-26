#include <algorithm>
#include <functional>
#include <cmath>

#include "ax/math.hpp"

namespace ax
{
    ax::box2 get_bounds(const ax::triangle2& triangle)
    {
        VAL left = std::min(
            std::get<0>(triangle).x,
            std::min(std::get<1>(triangle).x, std::get<2>(triangle).x));
        VAL right = std::max(
            std::get<0>(triangle).x,
            std::max(std::get<1>(triangle).x, std::get<2>(triangle).x));
        VAL bottom = std::min(
            std::get<0>(triangle).y,
            std::min(std::get<1>(triangle).y, std::get<2>(triangle).y));
        VAL top = std::max(
            std::get<0>(triangle).y,
            std::max(std::get<1>(triangle).y, std::get<2>(triangle).y));
        return
            {{ left, bottom },
             { right, top }};
    }

    float get_depth(const ax::v2& point, const ax::triangle2 triangle)
    {
        VAL& coords = ax::get_barycentric_coords(point, triangle);
        VAL depth =
            std::get<0>(triangle).y * coords.x +
            std::get<1>(triangle).y * coords.y +
            std::get<2>(triangle).y * coords.z;
        return depth;
    }

    ax::v2 get_interpolation(const ax::v2& point, const ax::triangle2 triangle)
    {
        VAL& coords = ax::get_barycentric_coords(point, triangle);
        VAL interpolation =
            std::get<0>(triangle) * coords.x +
            std::get<1>(triangle) * coords.y +
            std::get<2>(triangle) * coords.z;
        return interpolation;
    }

    ax::box2 get_intersection(const ax::box2& box, const ax::box2& box2)
    {
        return
            {{std::min(box.first.x, box2.first.x),
              std::min(box.first.y, box2.first.y)},
             {std::max(box.second.x, box2.second.x),
              std::max(box.second.y, box2.second.y)}};
    }

    ax::v3 get_barycentric_coords(ax::v2 point, const ax::triangle2& triangle)
    {
        VAL& a = ax::v3(std::get<2>(triangle).x - std::get<0>(triangle).x, std::get<1>(triangle).x - std::get<0>(triangle).x, std::get<0>(triangle).x - point.x);
        VAL& b = ax::v3(std::get<2>(triangle).y - std::get<0>(triangle).y, std::get<1>(triangle).y - std::get<0>(triangle).y, std::get<0>(triangle).y - point.y);
        VAL& u = a ^ b;
        if (std::abs(u.z) < 1.0f)
        {
            // we have a degenerate triangle
            // NOTE: I don't have high confidence in the math here being correct. It was converted from
            // integer math and I suspect I thereby lost something.
            return ax::v3(-1.0f, 1.0f, 1.0f);
        }
        return ax::v3(
            1.0f - (u.x + u.y) / u.z,
            u.y / u.z,
            u.x / u.z);
    }

    bool get_in_bounds(const ax::v2& point, const ax::triangle2& triangle)
    {
        VAL& coords = ax::get_barycentric_coords(point, triangle);
        return coords.x >= 0 && coords.y >= 0 && coords.z >= 0;
    }
}
