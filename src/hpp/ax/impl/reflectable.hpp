#ifndef IMPL_AX_REFLECTABLE_HPP
#define IMPL_AX_REFLECTABLE_HPP

#include <cstddef>
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
	public:

		CONSTRAINT(reflectable);
		reflectable() = default;

    protected:

        ENABLE_CAST(reflectable, castable);
        const std::shared_ptr<type_t> type = register_type<reflectable>({});
        virtual std::shared_ptr<type_t> get_type() const;
        friend std::shared_ptr<type_t> get_type(const reflectable& source);
    };

    // Get the type of a value.
    std::shared_ptr<type_t> get_type(const reflectable& source);
}

#define ENABLE_REFLECTION_CUSTOM(t, s) \
    \
    ENABLE_CAST(t, s); \
    \
    struct enable_reflection_custom_macro_end

#define ENABLE_REFLECTION(t, s) \
    \
    ENABLE_CAST(t, s); \
    \
    const std::shared_ptr<ax::type_t> t##_type = ax::register_sub_type<s>(typeid(t), {}); \
    \
    std::shared_ptr<ax::type_t> get_type() const override \
    { \
        return t##_type; \
    }; \
    \
    struct enable_reflection_macro_end

namespace ax
{
    // A unit type enabled with reflection, allowing for more weakly-typed systems where needed.
    class unitr : public reflectable
    {
    protected: ENABLE_REFLECTION(unitr, ax::reflectable);
    public: CONSTRAINT(unitr);
    };
}

#endif
