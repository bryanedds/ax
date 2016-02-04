#include "../../hpp/ax/inspectable.hpp"

namespace ax
{
    std::shared_ptr<type_t> inspectable::get_type_impl() const
    {
        return type;
    }

    std::shared_ptr<type_t> get_type(const inspectable& source)
    {
        return source.get_type_impl();
    }
}
