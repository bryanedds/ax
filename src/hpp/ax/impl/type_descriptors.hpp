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

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for integer values.
    class int_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for 32-bit integer values.
    class int32_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for 64-bit integer values.
    class int64_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for float values.
    class float_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for double values.
    class double_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for std::size_t values.
    class size_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for std::string values.
    class string_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for ax::name_t values.
    class name_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for ax::address values.
    class address_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, symbol& target_symbol) const override;
    };

    // A type descriptor for std::vector<T> values.
    template<typename T>
    class vector_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::vector<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::vector<T>>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            VAR* vector_ptr = static_cast<std::vector<T>*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& symbols)
            {
                vector_ptr->clear();
                VAL& type_descriptor = get_type_descriptor<T>();
                for (VAL& symbol : symbols)
                {
                    T elem_mvb{};
                    read_value_vptr(*type_descriptor, symbol, static_cast<void*>(&elem_mvb));
                    vector_ptr->emplace_back(std::move(elem_mvb));
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            VAL* vector_ptr = static_cast<const std::vector<T>*>(source_ptr);
            std::vector<symbol> symbols{};
            for (VAL& elem : *vector_ptr)
            {
                symbol symbol_mvb{};
                VAL& type_descriptor = get_type_descriptor<T>();
                write_value_vptr(*type_descriptor, static_cast<const void*>(&elem), symbol_mvb);
                symbols.emplace_back(std::move(symbol_mvb));
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    template<typename T>
    class unordered_set_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_set<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_set<T>>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            VAR* set_ptr = static_cast<std::unordered_set<T>*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& symbols)
            {
                set_ptr->clear();
                VAL& type_descriptor = get_type_descriptor<T>();
                for (VAL& symbol : symbols)
                {
                    T elem_mvb{};
                    read_value_vptr(*type_descriptor, symbol, static_cast<void*>(&elem_mvb));
                    set_ptr->insert(std::move(elem_mvb));
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            VAL* set_ptr = static_cast<const std::unordered_set<T>*>(source_ptr);
            std::vector<symbol> symbols{};
            for (VAL& elem : *set_ptr)
            {
                symbol symbol_mvb{};
                VAL& type_descriptor = get_type_descriptor<T>();
                write_value_vptr(*type_descriptor, static_cast<const void*>(&elem), symbol_mvb);
                symbols.emplace_back(std::move(symbol_mvb));
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    template<typename K, typename V>
    class unordered_map_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_map<K, V>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<std::unordered_map<K, V>>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            VAR* map_ptr = static_cast<std::unordered_map<K, V>*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& symbols)
            {
                map_ptr->clear();
                VAL& key_type_descriptor = get_type_descriptor<K>();
                VAL& value_type_descriptor = get_type_descriptor<V>();
                for (VAL& symbol : symbols)
                {
                    match3(symbol,
                    [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                    [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                    [&](VAL& symbols)
                    {
                        // ensure correct symbol structure
                        if (symbols.size() != 2 ||
                            !is_atom(symbols[0]) ||
                            !is_atom(symbols[1]))
                            throw std::invalid_argument("Expected two symbol leaves.");

                        // read value
                        K key{};
                        V value{};
                        read_value_vptr(*key_type_descriptor, symbols[0], static_cast<void*>(&key));
                        read_value_vptr(*value_type_descriptor, symbols[1], static_cast<void*>(&value));
                        VAR insertion = map_ptr->emplace(key, value);
                        if (!insertion.second) insertion.first->second = value;
                    });
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            VAL* map_ptr = static_cast<const std::unordered_map<K, V>*>(source_ptr);
            VAL& key_type_descriptor = get_type_descriptor<K>();
            VAL& value_type_descriptor = get_type_descriptor<V>();
            std::vector<symbol> symbols{};
            for (VAL& kvp : *map_ptr)
            {
                symbols_t symbol_kvp_mvb({ symbol{}, symbol{} });
                write_value_vptr(*key_type_descriptor, static_cast<const void*>(&kvp.first), symbol_kvp_mvb[0]);
                write_value_vptr(*value_type_descriptor, static_cast<const void*>(&kvp.second), symbol_kvp_mvb[1]);
                symbols.emplace_back(ax::symbols(std::move(symbol_kvp_mvb)));
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    template<typename T>
    class unique_ptr_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_unique_ptr_vptr<std::unique_ptr<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_unique_ptr_vptr<std::unique_ptr<T>>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            T value_mvb{};
            VAR* unique_ptr_ptr = static_cast<std::unique_ptr<T>*>(target_ptr);
            VAL& type_descriptor = get_type_descriptor<T>();
            read_value_vptr(*type_descriptor, source_symbol, static_cast<void*>(&value_mvb));
            unique_ptr_ptr->reset(new T(std::move(value_mvb)));
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            VAL* unique_ptr_ptr = static_cast<const std::unique_ptr<const T>*>(source_ptr);
            VAL& type_descriptor = get_type_descriptor<T>();
            write_value_vptr(*type_descriptor, static_cast<const void*>(unique_ptr_ptr->get()), target_symbol);
        }
    };

    template<typename T>
    class shared_ptr_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_shared_ptr_vptr<std::shared_ptr<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_shared_ptr_vptr<std::shared_ptr<T>>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            T value_mvb{};
            VAR* shared_ptr_ptr = static_cast<std::shared_ptr<T>*>(target_ptr);
            VAL& type_descriptor = get_type_descriptor<T>();
            read_value_vptr(*type_descriptor, source_symbol, static_cast<void*>(&value_mvb));
            shared_ptr_ptr->reset(new T(std::move(value_mvb)));
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            VAL* shared_ptr_ptr = static_cast<const std::shared_ptr<const T>*>(source_ptr);
            VAL& type_descriptor = get_type_descriptor<T>();
            write_value_vptr(*type_descriptor, static_cast<const void*>(shared_ptr_ptr->get()), target_symbol);
        }
    };

    template<typename P>
    class pair_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(P, pair);
            assign_value_vptr<P>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(P, pair);
            assign_value_vptr<P>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(P, pair);
            VAR* pair_ptr = static_cast<P*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 2)
                    throw std::invalid_argument("Expected two symbols.");

                // populate target pair
                typename P::first_type first_value_mvb{};
                typename P::second_type second_value_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename P::first_type>();
                VAL& second_type_descriptor = get_type_descriptor<typename P::second_type>();
                read_value_vptr(*first_type_descriptor, source_symbols[0], &first_value_mvb);
                read_value_vptr(*second_type_descriptor, source_symbols[1], &second_value_mvb);
                *pair_ptr = P(std::move(first_value_mvb), std::move(second_value_mvb));
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(P, pair);
            VAL* pair_ptr = static_cast<const P*>(source_ptr);
            symbols_t symbols_mvb{};
            symbols_mvb.resize(2_z);
            VAL& first_type_descriptor = get_type_descriptor<typename P::first_type>();
            VAL& second_type_descriptor = get_type_descriptor<typename P::second_type>();
            write_value_vptr(*first_type_descriptor, static_cast<const void*>(&fst(*pair_ptr)), symbols_mvb[0]);
            write_value_vptr(*second_type_descriptor, static_cast<const void*>(&snd(*pair_ptr)), symbols_mvb[1]);
            target_symbol = symbols(std::move(symbols_mvb));
        }
    };

    template<typename R>
    class record_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(R, record);
            VAR* record_ptr = static_cast<R*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 3)
                    throw std::invalid_argument("Expected three symbols.");

                // populate target record
                typename R::first_type first_value_mvb{};
                typename R::second_type second_value_mvb{};
                typename R::third_type third_value_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename R::first_type>();
                VAL& second_type_descriptor = get_type_descriptor<typename R::second_type>();
                VAL& third_type_descriptor = get_type_descriptor<typename R::third_type>();
                read_value_vptr(*first_type_descriptor, source_symbols[0], &first_value_mvb);
                read_value_vptr(*second_type_descriptor, source_symbols[1], &second_value_mvb);
                read_value_vptr(*third_type_descriptor, source_symbols[2], &third_value_mvb);
                *record_ptr = R(std::move(first_value_mvb), std::move(second_value_mvb), std::move(third_value_mvb));
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(R, record);
            VAL* record_ptr = static_cast<const R*>(source_ptr);
            symbols_t symbols_mvb{};
            symbols_mvb.resize(3_z);
            VAL& first_type_descriptor = get_type_descriptor<typename R::first_type>();
            VAL& second_type_descriptor = get_type_descriptor<typename R::second_type>();
            VAL& third_type_descriptor = get_type_descriptor<typename R::third_type>();
            write_value_vptr(*first_type_descriptor, static_cast<const void*>(&fst(*record_ptr)), symbols_mvb[0]);
            write_value_vptr(*second_type_descriptor, static_cast<const void*>(&snd(*record_ptr)), symbols_mvb[1]);
            write_value_vptr(*third_type_descriptor, static_cast<const void*>(&thd(*record_ptr)), symbols_mvb[2]);
            target_symbol = symbols(std::move(symbols_mvb));
        }
    };

    template<typename R>
    class record4_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record4);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record4);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(R, record4);
            VAR* record_ptr = static_cast<R*>(target_ptr);
            match3(source_symbol,
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 4)
                    throw std::invalid_argument("Expected four symbols.");

                // populate target record
                typename R::first_type first_value_mvb{};
                typename R::second_type second_value_mvb{};
                typename R::third_type third_value_mvb{};
                typename R::fourth_type fourth_value_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename R::first_type>();
                VAL& second_type_descriptor = get_type_descriptor<typename R::second_type>();
                VAL& third_type_descriptor = get_type_descriptor<typename R::third_type>();
                VAL& fourth_type_descriptor = get_type_descriptor<typename R::fourth_type>();
                read_value_vptr(*first_type_descriptor, source_symbols[0], &first_value_mvb);
                read_value_vptr(*second_type_descriptor, source_symbols[1], &second_value_mvb);
                read_value_vptr(*third_type_descriptor, source_symbols[2], &third_value_mvb);
                read_value_vptr(*fourth_type_descriptor, source_symbols[3], &fourth_value_mvb);
                *record_ptr = R(std::move(first_value_mvb), std::move(second_value_mvb), std::move(third_value_mvb), std::move(fourth_value_mvb));
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(R, record4);
            VAL* record_ptr = static_cast<const R*>(source_ptr);
            symbols_t symbols_mvb{};
            symbols_mvb.resize(4_z);
            VAL& first_type_descriptor = get_type_descriptor<typename R::first_type>();
            VAL& second_type_descriptor = get_type_descriptor<typename R::second_type>();
            VAL& third_type_descriptor = get_type_descriptor<typename R::third_type>();
            VAL& fourth_type_descriptor = get_type_descriptor<typename R::fourth_type>();
            write_value_vptr(*first_type_descriptor, static_cast<const void*>(&fst(*record_ptr)), symbols_mvb[0]);
            write_value_vptr(*second_type_descriptor, static_cast<const void*>(&snd(*record_ptr)), symbols_mvb[1]);
            write_value_vptr(*third_type_descriptor, static_cast<const void*>(&thd(*record_ptr)), symbols_mvb[2]);
            write_value_vptr(*fourth_type_descriptor, static_cast<const void*>(&fourth(*record_ptr)), symbols_mvb[3]);
            target_symbol = symbols(std::move(symbols_mvb));
        }
    };

    template<typename R>
    class record5_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record5);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record5);
            assign_value_vptr<R>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(R, record5);
            VAR* record_ptr = static_cast<R*>(target_ptr);
            match3(source_symbol,
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 5)
                    throw std::invalid_argument("Expected four symbols.");

                // populate target record
                typename R::first_type first_value_mvb{};
                typename R::second_type second_value_mvb{};
                typename R::third_type third_value_mvb{};
                typename R::fourth_type fourth_value_mvb{};
                typename R::fifth_type fifth_value_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename R::first_type>();
                VAL& second_type_descriptor = get_type_descriptor<typename R::second_type>();
                VAL& third_type_descriptor = get_type_descriptor<typename R::third_type>();
                VAL& fourth_type_descriptor = get_type_descriptor<typename R::fourth_type>();
                VAL& fifth_type_descriptor = get_type_descriptor<typename R::fifth_type>();
                read_value_vptr(*first_type_descriptor, source_symbols[0], &first_value_mvb);
                read_value_vptr(*second_type_descriptor, source_symbols[1], &second_value_mvb);
                read_value_vptr(*third_type_descriptor, source_symbols[2], &third_value_mvb);
                read_value_vptr(*fourth_type_descriptor, source_symbols[3], &fourth_value_mvb);
                read_value_vptr(*fifth_type_descriptor, source_symbols[4], &fifth_value_mvb);
                *record_ptr = R(
                    std::move(first_value_mvb),
                    std::move(second_value_mvb),
                    std::move(third_value_mvb),
                    std::move(fourth_value_mvb),
                    std::move(fifth_value_mvb));
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(R, record5);
            VAL* record_ptr = static_cast<const R*>(source_ptr);
            symbols_t symbols_mvb{};
            symbols_mvb.resize(5_z);
            VAL& first_type_descriptor = get_type_descriptor<typename R::first_type>();
            VAL& second_type_descriptor = get_type_descriptor<typename R::second_type>();
            VAL& third_type_descriptor = get_type_descriptor<typename R::third_type>();
            VAL& fourth_type_descriptor = get_type_descriptor<typename R::fourth_type>();
            VAL& fifth_type_descriptor = get_type_descriptor<typename R::fifth_type>();
            write_value_vptr(*first_type_descriptor, static_cast<const void*>(&fst(*record_ptr)), symbols_mvb[0]);
            write_value_vptr(*second_type_descriptor, static_cast<const void*>(&snd(*record_ptr)), symbols_mvb[1]);
            write_value_vptr(*third_type_descriptor, static_cast<const void*>(&thd(*record_ptr)), symbols_mvb[2]);
            write_value_vptr(*fourth_type_descriptor, static_cast<const void*>(&fourth(*record_ptr)), symbols_mvb[3]);
            write_value_vptr(*fifth_type_descriptor, static_cast<const void*>(&fifth(*record_ptr)), symbols_mvb[4]);
            target_symbol = symbols(std::move(symbols_mvb));
        }
    };

    template<typename T>
    class option_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<option<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            assign_value_vptr<option<T>>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            VAR* option_ptr = static_cast<option<T>*>(target_ptr);
            VAL& type_descriptor = get_type_descriptor<T>();
            match3(source_symbol,
            [&](VAL& atom)
            {
                if (atom != "none") throw std::invalid_argument("Expected the atom 'none'.");
                *option_ptr = none<T>();
            },
            [&](VAL&)
            {
                throw std::invalid_argument("Expected atom or symbols value.");
            },
            [&](VAL& symbols)
            {
                if (symbols.size() != 2 ||
                    !is_atom(symbols[0]) ||
                    get_atom(symbols[0]) != "some")
                    throw std::invalid_argument("Expected two symbols, with the first being the atom 'some'.");
                T some_value_mvb{};
                read_value_vptr(*type_descriptor, symbols[1], &some_value_mvb);
                *option_ptr = some(std::move(some_value_mvb));
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            VAL* option_ptr = static_cast<const option<T>*>(source_ptr);
            match_opt(*option_ptr,
            [&](VAL& some_value)
            {
                symbol symbol_value_mvb{};
                VAL& type_descriptor = get_type_descriptor<T>();
                write_value_vptr(*type_descriptor, static_cast<const void*>(&some_value), symbol_value_mvb);
                target_symbol = symbols({ atom("some"), std::move(symbol_value_mvb) });
            },
            [&]() { target_symbol = atom("none"); });
        }
    };

    template<typename E>
    class either_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(E, either);
            assign_value_vptr<E>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(E, either);
            assign_value_vptr<E>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(E, either);
            VAR* either_ptr = static_cast<E*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& source_symbols)
            {
                // validate symbol count
                if (source_symbols.size() != 2)
                    throw std::invalid_argument("Expected two symbols.");

                // validate correct symbol name usage
                VAL& symbol_name = source_symbols[0];
                VAL& symbol_value = source_symbols[1];
                VAL& right_name = get_right_name(*either_ptr);
                VAL& left_name = get_left_name(*either_ptr);
                if (!is_atom(symbol_name))
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target either
                VAL& either_name = get_atom(symbol_name);
                if (either_name == right_name)
                {
                    typename E::right_type right_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename E::right_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &right_value_mvb);
                    *either_ptr = E(std::move(right_value_mvb), false);
                }
                else if (either_name == left_name)
                {
                    typename E::left_type left_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename E::left_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &left_value_mvb);
                    *either_ptr = E(std::move(left_value_mvb), false, false);
                }
                else
                {
                    throw std::invalid_argument(
                        "Expected appropriate either constructor name of either '"_s +
                        right_name + "' or '" +
                        left_name + "'.");
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(E, either);
            VAL* either_ptr = static_cast<const E*>(source_ptr);
            match(*either_ptr,
            [&](VAL& right_value)
            {
                symbol symbol_mvb{};
                VAL& right_type_descriptor = get_type_descriptor<typename E::right_type>();
                write_value_vptr(*right_type_descriptor, static_cast<const void*>(&right_value), symbol_mvb);
                target_symbol = symbols({ atom(get_right_name(*either_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& left_value)
            {
                symbol symbol_mvb{};
                VAL& left_type_descriptor = get_type_descriptor<typename E::left_type>();
                write_value_vptr(*left_type_descriptor, static_cast<const void*>(&left_value), symbol_mvb);
                target_symbol = symbols({ atom(get_left_name(*either_ptr)), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(C, choice);
            VAR* choice_ptr = static_cast<C*>(target_ptr);
            match3(source_symbol,
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 2)
                    throw std::invalid_argument("Expected two symbols.");

                // validate correct symbol name usage
                VAL& symbol_name = source_symbols[0];
                VAL& symbol_value = source_symbols[1];
                VAL& first_name = get_first_name(*choice_ptr);
                VAL& second_name = get_second_name(*choice_ptr);
                VAL& third_name = get_third_name(*choice_ptr);
                if (!is_atom(symbol_name))
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target choice
                VAL& choice_name = get_atom(symbol_name);
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::first_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb), false);
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::second_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false, false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::third_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &third_value_mvb);
                    *choice_ptr = C(std::move(third_value_mvb), false, false, false);
                }
                else
                {
                    throw std::invalid_argument(
                        "Expected appropriate choice constructor name of either '"_s +
                        first_name + "' or '" +
                        second_name + "' or '" +
                        third_name + "'.");
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(C, choice);
            VAL* choice_ptr = static_cast<const C*>(source_ptr);
            match3(*choice_ptr,
            [&](VAL& first_value)
            {
                symbol symbol_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename C::first_type>();
                write_value_vptr(*first_type_descriptor, static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = symbols({ atom(get_first_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& second_value)
            {
                symbol symbol_mvb{};
                VAL& second_type_descriptor = get_type_descriptor<typename C::second_type>();
                write_value_vptr(*second_type_descriptor, static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = symbols({ atom(get_second_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& third_value)
            {
                symbol symbol_mvb{};
                VAL& third_type_descriptor = get_type_descriptor<typename C::third_type>();
                write_value_vptr(*third_type_descriptor, static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = symbols({ atom(get_third_name(*choice_ptr)), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice4_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice4);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice4);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(C, choice4);
            VAR* choice_ptr = static_cast<C*>(target_ptr);
            match3(source_symbol,
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 2)
                    throw std::invalid_argument("Expected two symbols.");

                // validate correct symbol name usage
                VAL& symbol_name = source_symbols[0];
                VAL& symbol_value = source_symbols[1];
                VAL& first_name = get_first_name(*choice_ptr);
                VAL& second_name = get_second_name(*choice_ptr);
                VAL& third_name = get_third_name(*choice_ptr);
                VAL& fourth_name = get_fourth_name(*choice_ptr);
                if (!is_atom(symbol_name))
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target choice
                VAL& choice_name = get_atom(symbol_name);
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::first_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb), false);
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::second_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false, false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::third_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &third_value_mvb);
                    *choice_ptr = C(std::move(third_value_mvb), false, false, false);
                }
                else if (choice_name == fourth_name)
                {
                    typename C::fourth_type fourth_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::fourth_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &fourth_value_mvb);
                    *choice_ptr = C(std::move(fourth_value_mvb), false, false, false, false);
                }
                else
                {
                    throw std::invalid_argument(
                        "Expected appropriate choice constructor name of either '"_s +
                        first_name + "' or '" +
                        second_name + "' or '" +
                        third_name + "' or '" +
                        fourth_name + "'.");
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(C, choice4);
            VAL* choice_ptr = static_cast<const C*>(source_ptr);
            match4(*choice_ptr,
            [&](VAL& first_value)
            {
                symbol symbol_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename C::first_type>();
                write_value_vptr(*first_type_descriptor, static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = symbols({ atom(get_first_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& second_value)
            {
                symbol symbol_mvb{};
                VAL& second_type_descriptor = get_type_descriptor<typename C::second_type>();
                write_value_vptr(*second_type_descriptor, static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = symbols({ atom(get_second_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& third_value)
            {
                symbol symbol_mvb{};
                VAL& third_type_descriptor = get_type_descriptor<typename C::third_type>();
                write_value_vptr(*third_type_descriptor, static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = symbols({ atom(get_third_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& fourth_value)
            {
                symbol symbol_mvb{};
                VAL& fourth_type_descriptor = get_type_descriptor<typename C::fourth_type>();
                write_value_vptr(*fourth_type_descriptor, static_cast<const void*>(&fourth_value), symbol_mvb);
                target_symbol = symbols({ atom(get_fourth_name(*choice_ptr)), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice5_descriptor : public type_descriptor
    {
    protected:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice5);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice5);
            assign_value_vptr<C>(source_ptr, target_ptr);
        }

        void read_value(const symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(C, choice5);
            VAR* choice_ptr = static_cast<C*>(target_ptr);
            match3(source_symbol,
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [&](VAL& source_symbols)
            {
                // validate symbols count
                if (source_symbols.size() != 2)
                    throw std::invalid_argument("Expected two symbols.");

                // validate correct symbol name usage
                VAL& symbol_name = source_symbols[0];
                VAL& symbol_value = source_symbols[1];
                VAL& first_name = get_first_name(*choice_ptr);
                VAL& second_name = get_second_name(*choice_ptr);
                VAL& third_name = get_third_name(*choice_ptr);
                VAL& fourth_name = get_fourth_name(*choice_ptr);
                VAL& fifth_name = get_fifth_name(*choice_ptr);
                if (!is_atom(symbol_name))
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target choice
                VAL& choice_name = get_atom(symbol_name);
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::first_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb), false);
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::second_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false, false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::third_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &third_value_mvb);
                    *choice_ptr = C(std::move(third_value_mvb), false, false, false);
                }
                else if (choice_name == fourth_name)
                {
                    typename C::fourth_type fourth_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::fourth_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &fourth_value_mvb);
                    *choice_ptr = C(std::move(fourth_value_mvb), false, false, false, false);
                }
                else if (choice_name == fifth_name)
                {
                    typename C::fifth_type fifth_value_mvb{};
                    VAL& type_descriptor = get_type_descriptor<typename C::fifth_type>();
                    read_value_vptr(*type_descriptor, symbol_value, &fifth_value_mvb);
                    *choice_ptr = C(std::move(fifth_value_mvb), false, false, false, false);
                }
                else
                {
                    throw std::invalid_argument(
                        "Expected appropriate choice constructor name of either '"_s +
                        first_name + "' or '" +
                        second_name + "' or '" +
                        third_name + "' or '" +
                        fourth_name + "' or '" +
                        fifth_name + "'.");
                }
            });
        }

        void write_value(const void* source_ptr, symbol& target_symbol) const override
        {
            CONSTRAIN(C, choice5);
            VAL* choice_ptr = static_cast<const C*>(source_ptr);
            match5(*choice_ptr,
            [&](VAL& first_value)
            {
                symbol symbol_mvb{};
                VAL& first_type_descriptor = get_type_descriptor<typename C::first_type>();
                write_value_vptr(*first_type_descriptor, static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = symbols({ atom(get_first_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& second_value)
            {
                symbol symbol_mvb{};
                VAL& second_type_descriptor = get_type_descriptor<typename C::second_type>();
                write_value_vptr(*second_type_descriptor, static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = symbols({ atom(get_second_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& third_value)
            {
                symbol symbol_mvb{};
                VAL& third_type_descriptor = get_type_descriptor<typename C::third_type>();
                write_value_vptr(*third_type_descriptor, static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = symbols({ atom(get_third_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& fourth_value)
            {
                symbol symbol_mvb{};
                VAL& fourth_type_descriptor = get_type_descriptor<typename C::fourth_type>();
                write_value_vptr(*fourth_type_descriptor, static_cast<const void*>(&fourth_value), symbol_mvb);
                target_symbol = symbols({ atom(get_fourth_name(*choice_ptr)), std::move(symbol_mvb) });
            },
            [&](VAL& fifth_value)
            {
                symbol symbol_mvb{};
                VAL& fifth_type_descriptor = get_type_descriptor<typename C::fifth_type>();
                write_value_vptr(*fifth_type_descriptor, static_cast<const void*>(&fifth_value), symbol_mvb);
                target_symbol = symbols({ atom(get_fifth_name(*choice_ptr)), std::move(symbol_mvb) });
            });
        }
    };
}

#endif
