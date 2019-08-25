#include "../../hpp/ax/impl/reflectable.hpp"

namespace ax
{
    std::shared_ptr<ax::type_t> reflectable::get_type() const
    {
        return type;
    }

    std::shared_ptr<ax::type_t> get_type(const ax::reflectable& source)
    {
        return source.get_type();
    }
}
