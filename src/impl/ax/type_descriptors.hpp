#ifndef IMPL_AX_TYPE_DESCRIPTORS_HPP
#define IMPL_AX_TYPE_DESCRIPTORS_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "prelude.hpp"
#include "string.hpp"
#include "pair.hpp"
#include "record.hpp"
#include "option.hpp"
#include "either.hpp"
#include "choice.hpp"
#include "symbol.hpp"
#include "type_descriptor.hpp"

namespace ax
{
    // A type descriptor for boolean values.
    class bool_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for integer values.
    class int_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for 32-bit integer values.
    class int32_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for 64-bit integer values.
    class int64_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for float values.
    class float_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for double values.
    class double_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for std::size_t values.
    class size_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for std::string values.
    class string_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for ax::name_t values.
    class name_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for ax::address values.
    class address_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override;
        void inject_value_impl(const void* source_ptr, void* target_ptr) const override;
        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override;
        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for std::vector<T> values.
    template<typename T>
    class vector_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::vector<T>>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::vector<T>>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            var* vector_ptr = static_cast<std::vector<T>*>(target_ptr);
            match2(source_symbol,
            [&](val& symbol_tree)
            {
                vector_ptr->clear();
                val& type_descriptor = get_type_descriptor<T>();
                for (val& symbol : symbol_tree)
                {
                    T elem_mvb{};
                    read_value_vptr(*type_descriptor, symbol, static_cast<void*>(&elem_mvb));
                    vector_ptr->emplace_back(std::move(elem_mvb));
                }
            },
            [](val&) { throw std::invalid_argument("Expected symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            val* vector_ptr = static_cast<const std::vector<T>*>(source_ptr);
            std::vector<symbol> symbol_tree{};
            for (val& elem : *vector_ptr)
            {
                symbol symbol_mvb{};
                val& type_descriptor = get_type_descriptor<T>();
                write_value_vptr(*type_descriptor, static_cast<const void*>(&elem), symbol_mvb);
                symbol_tree.emplace_back(std::move(symbol_mvb));
            }
            target_symbol = ax::symbol_tree(symbol_tree);
        }
    };

    template<typename T>
    class unordered_set_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_set<T>>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_set<T>>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            var* set_ptr = static_cast<std::unordered_set<T>*>(target_ptr);
            match2(source_symbol,
            [&](val& symbol_tree)
            {
                set_ptr->clear();
                val& type_descriptor = get_type_descriptor<T>();
                for (val& symbol : symbol_tree)
                {
                    T elem_mvb{};
                    read_value_vptr(*type_descriptor, symbol, static_cast<void*>(&elem_mvb));
                    set_ptr->insert(std::move(elem_mvb));
                }
            },
            [](val&) { throw std::invalid_argument("Expected symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            val* set_ptr = static_cast<const std::unordered_set<T>*>(source_ptr);
            std::vector<symbol> symbol_tree{};
            for (val& elem : *set_ptr)
            {
                symbol symbol_mvb{};
                val& type_descriptor = get_type_descriptor<T>();
                write_value_vptr(*type_descriptor, static_cast<const void*>(&elem), symbol_mvb);
                symbol_tree.emplace_back(std::move(symbol_mvb));
            }
            target_symbol = ax::symbol_tree(symbol_tree);
        }
    };

    template<typename K, typename V>
    class unordered_map_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_map<K, V>>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_map<K, V>>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            var* map_ptr = static_cast<std::unordered_map<K, V>*>(target_ptr);
            match2(source_symbol,
            [&](val& symbol_tree)
            {
                map_ptr->clear();
                val& key_type_descriptor = get_type_descriptor<K>();
                val& value_type_descriptor = get_type_descriptor<V>();
                for (val& symbol : symbol_tree)
                {
                    match2(symbol,
                    [&](val& symbol_tree)
                    {
                        // ensure correct symbol structure
                        if (symbol_tree.size() != 2 ||
                            !is_symbol_leaf(symbol_tree[0]) ||
                            !is_symbol_leaf(symbol_tree[1]))
                            throw std::invalid_argument("Expected two symbol leaves.");

                        // read value
                        K key{};
                        V value{};
                        read_value_vptr(*key_type_descriptor, symbol_tree[0], static_cast<void*>(&key));
                        read_value_vptr(*value_type_descriptor, symbol_tree[1], static_cast<void*>(&value));
                        var insertion = map_ptr->emplace(key, value);
                        if (!insertion.second) insertion.first->second = value;
                    },
                    [](val&) { throw std::invalid_argument("Expected symbol tree."); });
                }
            },
            [](val&) { throw std::invalid_argument("Expected symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            val* map_ptr = static_cast<const std::unordered_map<K, V>*>(source_ptr);
            val& key_type_descriptor = get_type_descriptor<K>();
            val& value_type_descriptor = get_type_descriptor<V>();
            std::vector<symbol> symbol_tree{};
            for (val& kvp : *map_ptr)
            {
                symbol::right_type symbol_kvp_mvb({ symbol{}, symbol{} });
                write_value_vptr(*key_type_descriptor, static_cast<const void*>(&kvp.first), symbol_kvp_mvb[0]);
                write_value_vptr(*value_type_descriptor, static_cast<const void*>(&kvp.second), symbol_kvp_mvb[1]);
                symbol_tree.emplace_back(ax::symbol_tree(std::move(symbol_kvp_mvb)));
            }
            target_symbol = ax::symbol_tree(symbol_tree);
        }
    };

    template<typename T>
    class unique_ptr_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_unique_ptr_vptr<std::unique_ptr<T>>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_unique_ptr_vptr<std::unique_ptr<T>>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            T value_mvb{};
            var* unique_ptr_ptr = static_cast<std::unique_ptr<T>*>(target_ptr);
            val& type_descriptor = get_type_descriptor<T>();
            read_value_vptr(*type_descriptor, source_symbol, static_cast<void*>(&value_mvb));
            unique_ptr_ptr->reset(new T(std::move(value_mvb)));
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            val* unique_ptr_ptr = static_cast<const std::unique_ptr<const T>*>(source_ptr);
            val& type_descriptor = get_type_descriptor<T>();
            write_value_vptr(*type_descriptor, static_cast<const void*>(unique_ptr_ptr->get()), target_symbol);
        }
    };

    template<typename T>
    class shared_ptr_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_shared_ptr_vptr<std::shared_ptr<T>>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_shared_ptr_vptr<std::shared_ptr<T>>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            T value_mvb{};
            var* shared_ptr_ptr = static_cast<std::shared_ptr<T>*>(target_ptr);
            val& type_descriptor = get_type_descriptor<T>();
            read_value_vptr(*type_descriptor, source_symbol, static_cast<void*>(&value_mvb));
            shared_ptr_ptr->reset(new T(std::move(value_mvb)));
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            val* shared_ptr_ptr = static_cast<const std::shared_ptr<const T>*>(source_ptr);
            val& type_descriptor = get_type_descriptor<T>();
            write_value_vptr(*type_descriptor, static_cast<const void*>(shared_ptr_ptr->get()), target_symbol);
        }
    };

    template<typename P>
    class pair_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(P, pair);
            assign_value_vptr<P>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(P, pair);
            assign_value_vptr<P>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            constrain(P, pair);
            var* pair_ptr = static_cast<P*>(target_ptr);
            match2(source_symbol,
            [&](val& source_tree)
            {
                // validate source tree size
                if (source_tree.size() != 2)
                    throw std::invalid_argument("Expected value tree with two children");

                // populate target pair
                typename P::first_type first_value_mvb{};
                typename P::second_type second_value_mvb{};
                val& first_type_descriptor = get_type_descriptor<typename P::first_type>();
                val& second_type_descriptor = get_type_descriptor<typename P::second_type>();
                read_value_vptr(*first_type_descriptor, source_tree[0], &first_value_mvb);
                read_value_vptr(*second_type_descriptor, source_tree[1], &second_value_mvb);
                *pair_ptr = P(std::move(first_value_mvb), std::move(second_value_mvb));
            },
            [&](val&) { throw std::invalid_argument("Expected source symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            constrain(P, pair);
            val* pair_ptr = static_cast<const P*>(source_ptr);
            symbol::right_type symbol_tree_mvb{};
            symbol_tree_mvb.resize(2_z);
            val& first_type_descriptor = get_type_descriptor<typename P::first_type>();
            val& second_type_descriptor = get_type_descriptor<typename P::second_type>();
            write_value_vptr(*first_type_descriptor, static_cast<const void*>(&fst(*pair_ptr)), symbol_tree_mvb[0]);
            write_value_vptr(*second_type_descriptor, static_cast<const void*>(&snd(*pair_ptr)), symbol_tree_mvb[1]);
            target_symbol = symbol_tree(std::move(symbol_tree_mvb));
        }
    };

    template<typename R>
    class record_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(R, record);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(R, record);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            constrain(R, record);
            var* record_ptr = static_cast<R*>(target_ptr);
            match2(source_symbol,
            [&](val& source_tree)
            {
                // validate source tree size
                if (source_tree.size() != 3)
                    throw std::invalid_argument("Expected value tree with three children");

                // populate target record
                typename R::first_type first_value_mvb{};
                typename R::second_type second_value_mvb{};
                typename R::third_type third_value_mvb{};
                val& first_type_descriptor = get_type_descriptor<typename R::first_type>();
                val& second_type_descriptor = get_type_descriptor<typename R::second_type>();
                val& third_type_descriptor = get_type_descriptor<typename R::third_type>();
                read_value_vptr(*first_type_descriptor, source_tree[0], &first_value_mvb);
                read_value_vptr(*second_type_descriptor, source_tree[1], &second_value_mvb);
                read_value_vptr(*third_type_descriptor, source_tree[2], &third_value_mvb);
                *record_ptr = R(std::move(first_value_mvb), std::move(second_value_mvb), std::move(third_value_mvb));
            },
            [&](val&) { throw std::invalid_argument("Expected source symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            constrain(R, record);
            val* record_ptr = static_cast<const R*>(source_ptr);
            symbol::right_type symbol_tree_mvb{};
            symbol_tree_mvb.resize(3_z);
            val& first_type_descriptor = get_type_descriptor<typename R::first_type>();
            val& second_type_descriptor = get_type_descriptor<typename R::second_type>();
            val& third_type_descriptor = get_type_descriptor<typename R::third_type>();
            write_value_vptr(*first_type_descriptor, static_cast<const void*>(&fst(*record_ptr)), symbol_tree_mvb[0]);
            write_value_vptr(*second_type_descriptor, static_cast<const void*>(&snd(*record_ptr)), symbol_tree_mvb[1]);
            write_value_vptr(*third_type_descriptor, static_cast<const void*>(&thd(*record_ptr)), symbol_tree_mvb[2]);
            target_symbol = symbol_tree(std::move(symbol_tree_mvb));
        }
    };

    template<typename T>
    class option_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<option<T>>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<option<T>>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            var* option_ptr = static_cast<option<T>*>(target_ptr);
            val& type_descriptor = get_type_descriptor<T>();
            match2(source_symbol,
            [&](val& symbol_tree)
            {
                if (symbol_tree.size() != 2 ||
                    !is_symbol_leaf(symbol_tree[0]) ||
                    get_symbol_leaf(symbol_tree[0]) != "some")
                    throw std::invalid_argument("Expected symbol tree with two children, with the first being a symbol leaf of 'some'");
                T some_value_mvb{};
                read_value_vptr(*type_descriptor, symbol_tree[1], &some_value_mvb);
                *option_ptr = some(std::move(some_value_mvb));
            },
            [&](val& symbol_leaf)
            {
                if (symbol_leaf != "none") throw std::invalid_argument("Expected symbol 'none'.");
                *option_ptr = none<T>();
            });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            val* option_ptr = static_cast<const option<T>*>(source_ptr);
            match(*option_ptr,
            [&](val& some_value)
            {
                symbol symbol_value_mvb{};
                val& type_descriptor = get_type_descriptor<T>();
                write_value_vptr(*type_descriptor, static_cast<const void*>(&some_value), symbol_value_mvb);
                target_symbol = symbol_tree({ symbol_leaf("some"), std::move(symbol_value_mvb) });
            },
            [&]() { target_symbol = symbol_leaf("none"); });
        }
    };

    template<typename E>
    class either_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(E, either);
            assign_value_vptr<E>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(E, either);
            assign_value_vptr<E>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            constrain(E, either);
            var* either_ptr = static_cast<E*>(target_ptr);
            match2(source_symbol,
            [&](val& source_tree)
            {
                // validate source tree size
                if (source_tree.size() != 2)
                    throw std::invalid_argument("Expected source symbol tree with two children");

                // validate correct symbol name usage
                val& symbol_name = source_tree[0];
                val& symbol_value = source_tree[1];
                val& right_name = get_right_name(*either_ptr);
                val& left_name = get_left_name(*either_ptr);
                if (!is_symbol_leaf(symbol_name))
                    throw std::invalid_argument("Expected source symbol tree with valid leaf names");

                // populate target either
                val& either_name = get_symbol_leaf(symbol_name);
                if (either_name == right_name)
                {
                    typename E::right_type right_value_mvb{};
                    val& type_descriptor = get_type_descriptor<typename E::right_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &right_value_mvb);
                    *either_ptr = E(std::move(right_value_mvb));
                }
                else if (either_name == left_name)
                {
                    typename E::left_type left_value_mvb{};
                    val& type_descriptor = get_type_descriptor<typename E::left_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &left_value_mvb);
                    *either_ptr = E(std::move(left_value_mvb), false);
                }
                else
                {
                    throw std::invalid_argument(
                        "Expected appropriate either constructor name of either '"_s +
                        left_name + "' or '" +
                        right_name + "'.");
                }
            },
            [&](val&) { throw std::invalid_argument("Expected source symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            constrain(E, either);
            val* either_ptr = static_cast<const E*>(source_ptr);
            match2(*either_ptr,
            [&](val& right_value)
            {
                symbol symbol_mvb{};
                val& right_type_descriptor = get_type_descriptor<typename E::right_type>();
                write_value_vptr(*right_type_descriptor, static_cast<const void*>(&right_value), symbol_mvb);
                target_symbol = symbol_tree({ symbol_leaf(get_right_name(*either_ptr)), std::move(symbol_mvb) });
            },
            [&](val& left_value)
            {
                symbol symbol_mvb{};
                val& left_type_descriptor = get_type_descriptor<typename E::left_type>();
                write_value_vptr(*left_type_descriptor, static_cast<const void*>(&left_value), symbol_mvb);
                target_symbol = symbol_tree({ symbol_leaf(get_left_name(*either_ptr)), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice_descriptor : public type_descriptor
    {
    protected:

        void inspect_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(C, choice);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void inject_value_impl(const void* source_ptr, void* target_ptr) const override
        {
            constrain(C, choice);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void read_value_impl(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            constrain(C, choice);
            var* choice_ptr = static_cast<C*>(target_ptr);
            match2(source_symbol,
            [&](val& source_tree)
            {
                // validate source tree size
                if (source_tree.size() != 2)
                    throw std::invalid_argument("Expected source symbol tree with two children");

                // validate correct symbol name usage
                val& symbol_name = source_tree[0];
                val& symbol_value = source_tree[1];
                val& first_name = get_first_name(*choice_ptr);
                val& second_name = get_second_name(*choice_ptr);
                val& third_name = get_third_name(*choice_ptr);
                if (!is_symbol_leaf(symbol_name))
                    throw std::invalid_argument("Expected source symbol tree with valid leaf names");

                // populate target choice
                val& choice_name = get_symbol_leaf(symbol_name);
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    val& type_descriptor = get_type_descriptor<typename C::first_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb));
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    val& type_descriptor = get_type_descriptor<typename C::second_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    val& type_descriptor = get_type_descriptor<typename C::third_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &third_value_mvb);
                    *choice_ptr = C(std::move(third_value_mvb), false, false);
                }
                else
                {
                    throw std::invalid_argument(
                        "Expected appropriate choice constructor name of either '"_s +
                        first_name + "' or '" +
                        second_name + "' or '" +
                        third_name + "'.");
                }
            },
            [&](val&) { throw std::invalid_argument("Expected source symbol tree."); });
        }

        void write_value_impl(const void* source_ptr, symbol& target_symbol) const override
        {
            constrain(C, choice);
            val* choice_ptr = static_cast<const C*>(source_ptr);
            match3(*choice_ptr,
            [&](val& first_value)
            {
                symbol symbol_mvb{};
                val& first_type_descriptor = get_type_descriptor<typename C::first_type>();
                write_value_vptr(*first_type_descriptor, static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = symbol_tree({ symbol_leaf(get_first_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](val& second_value)
            {
                symbol symbol_mvb{};
                val& second_type_descriptor = get_type_descriptor<typename C::second_type>();
                write_value_vptr(*second_type_descriptor, static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = symbol_tree({ symbol_leaf(get_second_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](val& third_value)
            {
                symbol symbol_mvb{};
                val& third_type_descriptor = get_type_descriptor<typename C::third_type>();
                write_value_vptr(*third_type_descriptor, static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = symbol_tree({ symbol_leaf(get_third_name(*choice_ptr)), std::move(symbol_mvb) });
            });
        }
    };
}

#endif
