#ifndef AX_BUFFER_HPP
#define AX_BUFFER_HPP

#include "prelude.hpp"
#include "math.hpp"

namespace ax
{
    class buffer
    {
    public:

        CONSTRAINT(buffer);
        virtual int get_width() const = 0;
        virtual int get_height() const = 0;
        virtual ax::color get_point(int x, int y) const = 0;
        virtual bool set_point(int x, int y, const ax::color& color) = 0;
        virtual void clear(const ax::color& color) = 0;
    };
}

#endif
