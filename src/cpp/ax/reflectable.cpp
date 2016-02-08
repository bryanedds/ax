#include "../../impl/ax/reflectable.hpp"

namespace ax
{
    std::shared_ptr<type_t> reflectable::get_type_impl() const
    {
        return type;
    }

    std::shared_ptr<type_t> get_type(const reflectable& source)
    {
        return source.get_type_impl();
    }
}
