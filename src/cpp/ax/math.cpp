#include <algorithm>
#include <functional>
#include <cmath>

#include "ax/math.hpp"

namespace ax
{
    ax::v2 get_ortho(const ax::v3& vector)
    {
        return ax::v2(vector.x, vector.y);
    }

    ax::line2 get_ortho(const ax::line3& line)
    {
        VAL& line_ortho = ax::line2(
            ax::v2(std::get<0>(line).x, std::get<0>(line).y),
            ax::v2(std::get<1>(line).x, std::get<1>(line).y));
        return line_ortho;
    }

    ax::triangle2 get_ortho(const ax::triangle3& triangle)
    {
        VAL& triangle_ortho = ax::triangle2(
            ax::v2(std::get<0>(triangle).x, std::get<0>(triangle).y),
            ax::v2(std::get<1>(triangle).x, std::get<1>(triangle).y),
            ax::v2(std::get<2>(triangle).x, std::get<2>(triangle).y));
        return triangle_ortho;
    }

    float get_depth(const ax::v2& point, const ax::triangle3& triangle)
    {
        VAL& triangle_ortho = ax::get_ortho(triangle);
        VAL& coords = ax::get_barycentric_coords(point, triangle_ortho);
        VAL depth =
            std::get<0>(triangle).z * coords.x +
            std::get<1>(triangle).z * coords.y +
            std::get<2>(triangle).z * coords.z;
        return depth;
    }

    ax::v2 get_interpolation(const ax::v2& point, const ax::triangle2& triangle)
    {
        VAL& coords = ax::get_barycentric_coords(point, triangle);
        VAL interpolation =
            std::get<0>(triangle) * coords.x +
            std::get<1>(triangle) * coords.y +
            std::get<2>(triangle) * coords.z;
        return interpolation;
    }

    ax::v3 get_normal(const ax::triangle3& triangle)
    {
        VAR cross_product = (std::get<1>(triangle) - std::get<0>(triangle)) ^ (std::get<2>(triangle) - std::get<0>(triangle));
		return cross_product.NormalizeSafe();
    }
    
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

    bool get_in_bounds(const ax::v2& point, const ax::box2& box)
    {
        VAL result =
            point.x >= std::get<0>(box).x && point.y >= std::get<0>(box).y &&
            point.x <  std::get<1>(box).x && point.y <  std::get<1>(box).y;
        return result;
    }

    bool get_in_bounds(const ax::v2i& point, const ax::box2i& box)
    {
        VAL result =
            point.x >= std::get<0>(box).x && point.y >= std::get<0>(box).y &&
            point.x <  std::get<1>(box).x && point.y <  std::get<1>(box).y;
        return result;
    }

    bool get_in_bounds(const ax::v2& point, const ax::v2& size)
    {
        return ax::get_in_bounds(point, ax::box2(ax::zero<ax::v2>(), size));
    }

    bool get_in_bounds(const ax::v2i& point, const ax::v2i& size)
    {
        return ax::get_in_bounds(point, ax::box2i(ax::zero<ax::v2i>(), size));
    }
}
