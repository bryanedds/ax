#include "../../hpp/ax/impl/reflectable.hpp"

namespace ax
{
    std::shared_ptr<type_t> reflectable::get_type() const
    {
        return type;
    }

    std::shared_ptr<type_t> get_type(const reflectable& source)
    {
        return source.get_type();
    }
}
