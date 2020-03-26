#include <algorithm>
#include <functional>
#include <cmath>

#include "ax/math.hpp"

namespace ax
{
    ax::box2 get_bounds(const ax::triangle2& tri)
    {
        VAL left = std::min(
            std::get<0>(tri).x,
            std::min(std::get<1>(tri).x, std::get<2>(tri).x));
        VAL right = std::max(
            std::get<0>(tri).x,
            std::max(std::get<1>(tri).x, std::get<2>(tri).x));
        VAL bottom = std::min(
            std::get<0>(tri).y,
            std::min(std::get<1>(tri).y, std::get<2>(tri).y));
        VAL top = std::max(
            std::get<0>(tri).y,
            std::max(std::get<1>(tri).y, std::get<2>(tri).y));
        return
            {{ left, bottom },
             { right, top }};
    }

    float get_depth(const ax::v2& point, const ax::triangle2 tri)
    {
        VAL& coords = ax::get_barycentric_coords(point, tri);
        VAL depth =
            std::get<0>(tri)[2] * coords[0] +
            std::get<1>(tri)[2] * coords[1] +
            std::get<2>(tri)[2] * coords[2];
        return depth;
    }

    ax::box2 get_intersection(const ax::box2& box, const ax::box2& box2)
    {
        return
            {{std::min(box.first.x, box2.first.x),
              std::min(box.first.y, box2.first.y)},
             {std::max(box.second.x, box2.second.x),
              std::max(box.second.y, box2.second.y)}};
    }

    ax::v3 get_barycentric_coords(ax::v2 point, const ax::triangle2& tri)
    {
        VAL& a = ax::v3(std::get<2>(tri).x - std::get<0>(tri).x, std::get<1>(tri).x - std::get<0>(tri).x, std::get<0>(tri).x - point.x);
        VAL& b = ax::v3(std::get<2>(tri).y - std::get<0>(tri).y, std::get<1>(tri).y - std::get<0>(tri).y, std::get<0>(tri).y - point.y);
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

    bool get_in_bounds(const ax::v2& point, const ax::triangle2& tri)
    {
        VAL& coords = ax::get_barycentric_coords(point, tri);
        return coords.x >= 0 && coords.y >= 0 && coords.z >= 0;
    }
}
