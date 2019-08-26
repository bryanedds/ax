#include "../../hpp/ax/impl/reflectable.hpp"

namespace ax
{
    std::shared_ptr<ax::type> reflectable::get_type() const
    {
        return type;
    }

    std::shared_ptr<ax::type> get_type(const ax::reflectable& source)
    {
        return source.get_type();
    }
}
