#ifndef AX_REFLECTABLE_HPP
#define AX_REFLECTABLE_HPP

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <memory>

#include "prelude.hpp"
#include "castable.hpp"
#include "type.hpp"

namespace ax
{
    // A mixin for enabling run-time reflection on a non-primitive type.
    //
    // NOTE: Sadly, the reflection system relies on undefined, yet portable behavior in that
    // offsetof is used to non-POD types (IE - types that use single implementation, non-virtual
    // inheritance and have private fields). This usage seems to be supported by the major
    // compilers however, so for now, I will continue using this approach.
    //
    // TODO: invoke run-time assertions to verify said behavior at the initialization of the
    // library.
    class reflectable : public castable
    {
    protected:

        enable_cast(reflectable, ax::castable);
        const std::shared_ptr<type_t> type = register_type<reflectable>({});
        virtual std::shared_ptr<type_t> get_type_impl() const;
        friend std::shared_ptr<type_t> get_type(const reflectable& source);

    public:
        
        constraint(reflectable);
        reflectable() = default;
    };

    // Get the type of a value.
    std::shared_ptr<type_t> get_type(const reflectable& source);
}

#endif
