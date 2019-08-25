#ifndef IMPL_AX_TYPE_DESCRIPTORS_HPP
#define IMPL_AX_TYPE_DESCRIPTORS_HPP

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "prelude.hpp"
#include "string.hpp"
#include "vector.hpp"
#include "pair.hpp"
#include "record.hpp"
#include "record4.hpp"
#include "record5.hpp"
#include "option.hpp"
#include "either.hpp"
#include "choice.hpp"
#include "choice4.hpp"
#include "choice5.hpp"
#include "symbol.hpp"
#include "type_descriptor.hpp"

namespace ax
{
    // A type descriptor for boolean values.
    class bool_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for integer values.
    class int_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for 32-bit integer values.
    class int32_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for 64-bit integer values.
    class int64_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for float values.
    class float_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for double values.
    class double_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for std::size_t values.
    class size_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for std::string values.
    class string_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for ax::name_t values.
    class name_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for ax::address values.
    class address_descriptor : public ax::type_descriptor
    {
    public:
        void inspect_value(const void* source_ptr, void* target_ptr) const override;
        void inject_value(const void* source_ptr, void* target_ptr) const override;
        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override;
        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override;
    };

    // A type descriptor for std::vector<T> values.
    template<typename T>
    class std_vector_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<std::vector<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<std::vector<T>>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            VAR* vector_ptr = static_cast<std::vector<T>*>(target_ptr);
            source_symbol.match(
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& symbols)
            {
                vector_ptr->clear();
                VAL& type_descriptor = ax::get_type_descriptor<T>();
                for (VAL& symbol : symbols)
                {
                    T elem_mvb{};
                    type_descriptor->read_value(symbol, static_cast<void*>(&elem_mvb));
                    vector_ptr->emplace_back(std::move(elem_mvb));
                }
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            VAL* vector_ptr = static_cast<const std::vector<T>*>(source_ptr);
            std::vector<ax::symbol> symbols{};
            for (VAL& elem : *vector_ptr)
            {
                ax::symbol symbol_mvb{};
                VAL& type_descriptor = ax::get_type_descriptor<T>();
                type_descriptor->write_value(static_cast<const void*>(&elem), symbol_mvb);
                symbols.emplace_back(std::move(symbol_mvb));
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    template<typename T>
    class std_unordered_set_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<std::unordered_set<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<std::unordered_set<T>>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            VAR* set_ptr = static_cast<std::unordered_set<T>*>(target_ptr);
            source_symbol.match(
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& symbols)
            {
                set_ptr->clear();
                VAL& type_descriptor = ax::get_type_descriptor<T>();
                for (VAL& symbol : symbols)
                {
                    T elem_mvb{};
                    type_descriptor->read_value(symbol, static_cast<void*>(&elem_mvb));
                    set_ptr->insert(std::move(elem_mvb));
                }
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            VAL* set_ptr = static_cast<const std::unordered_set<T>*>(source_ptr);
            std::vector<ax::symbol> symbols{};
            for (VAL& elem : *set_ptr)
            {
                ax::symbol symbol_mvb{};
                VAL& type_descriptor = ax::get_type_descriptor<T>();
                type_descriptor->write_value(static_cast<const void*>(&elem), symbol_mvb);
                symbols.emplace_back(std::move(symbol_mvb));
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    template<typename K, typename V>
    class std_unordered_map_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<std::unordered_map<K, V>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<std::unordered_map<K, V>>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            VAR* map_ptr = static_cast<std::unordered_map<K, V>*>(target_ptr);
            source_symbol.match(
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
            [&](VAL& symbols)
            {
                map_ptr->clear();
                VAL& key_type_descriptor = ax::get_type_descriptor<K>();
                VAL& value_type_descriptor = ax::get_type_descriptor<V>();
                for (VAL& symbol : symbols)
                {
                    symbol.match(
                    [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                    [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                    [&](VAL& symbols)
                    {
                        // ensure correct symbol structure
                        if (symbols.size() != 2 ||
                            !symbols[0].is_atom() ||
                            !symbols[1].is_atom())
                            throw std::invalid_argument("Expected two symbol leaves.");

                        // read value
                        K key{};
                        V value{};
                        key_type_descriptor->read_value(symbols[0], static_cast<void*>(&key));
                        value_type_descriptor->read_value(symbols[1], static_cast<void*>(&value));
                        VAR insertion = map_ptr->emplace(key, value);
                        if (!insertion.second) insertion.first->second = value;
                    });
                }
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            VAL* map_ptr = static_cast<const std::unordered_map<K, V>*>(source_ptr);
            VAL& key_type_descriptor = ax::get_type_descriptor<K>();
            VAL& value_type_descriptor = ax::get_type_descriptor<V>();
            std::vector<ax::symbol> symbols{};
            for (VAL& kvp : *map_ptr)
            {
                symbols_t symbol_kvp_mvb({ ax::symbol{}, ax::symbol{} });
                key_type_descriptor->write_value(static_cast<const void*>(&kvp.first), symbol_kvp_mvb[0]);
                value_type_descriptor->write_value(static_cast<const void*>(&kvp.second), symbol_kvp_mvb[1]);
                symbols.push_back(ax::symbols(std::move(symbol_kvp_mvb)));
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    // A type descriptor for ax::vector<T> values.
    template<typename T>
    class ax_vector_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<ax::vector<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<ax::vector<T>>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            VAR* vector_ptr = static_cast<ax::vector<T>*>(target_ptr);
            source_symbol.match(
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [](VAL&) { throw std::invalid_argument("Expected symbols value."); },
                [&](VAL& symbols)
            {
                vector_ptr->clear();
                VAL& type_descriptor = ax::get_type_descriptor<T>();
                for (VAL& symbol : symbols)
                {
                    T elem_mvb{};
                    type_descriptor->read_value(symbol, static_cast<void*>(&elem_mvb));
                    vector_ptr->push_back(elem_mvb); // implement ax::vector::emplace_back to std::move this in
                }
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            VAL* vector_ptr = static_cast<const ax::vector<T>*>(source_ptr);
            std::vector<ax::symbol> symbols{};
            for (VAL& elem : *vector_ptr)
            {
                ax::symbol symbol_mvb{};
                VAL& type_descriptor = get_type_descriptor<T>();
                type_descriptor->write_value(static_cast<const void*>(&elem), symbol_mvb);
                symbols.push_back(symbol_mvb); // implement ax::vector::emplace_back to std::move this in
            }
            target_symbol = ax::symbols(symbols);
        }
    };

    template<typename T>
    class shared_ptr_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_shared_ptr<std::shared_ptr<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_shared_ptr<std::shared_ptr<T>>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            VAR value_ptr = std::make_shared<T>();
            VAR* shared_ptr_ptr = static_cast<std::shared_ptr<T>*>(target_ptr);
            VAL& type_descriptor = get_type_descriptor<T>();
            type_descriptor->read_value(source_symbol, static_cast<void*>(value_ptr.get()));
            *shared_ptr_ptr = value_ptr;
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            VAL* shared_ptr_ptr = static_cast<const std::shared_ptr<const T>*>(source_ptr);
            VAL& type_descriptor = ax::get_type_descriptor<T>();
            type_descriptor->write_value(static_cast<const void*>(shared_ptr_ptr->get()), target_symbol);
        }
    };

    // TODO: std_pair_descriptor?

    template<typename P>
    class ax_pair_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(P, pair);
            ax::assign_value<P>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(P, pair);
            ax::assign_value<P>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(P, pair);
            VAR* pair_ptr = static_cast<P*>(target_ptr);
            source_symbol.match(
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
                VAL& first_type_descriptor = ax::get_type_descriptor<typename P::first_type>();
                VAL& second_type_descriptor = ax::get_type_descriptor<typename P::second_type>();
                first_type_descriptor->read_value(source_symbols[0], &first_value_mvb);
                second_type_descriptor->read_value(source_symbols[1], &second_value_mvb);
                *pair_ptr = P(std::move(first_value_mvb), std::move(second_value_mvb));
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(P, pair);
            VAL* pair_ptr = static_cast<const P*>(source_ptr);
            ax::symbols_t symbols_mvb{};
            symbols_mvb.resize(2_z);
            VAL& first_type_descriptor = ax::get_type_descriptor<typename P::first_type>();
            VAL& second_type_descriptor = ax::get_type_descriptor<typename P::second_type>();
            first_type_descriptor->write_value(static_cast<const void*>(&ax::fst(*pair_ptr)), symbols_mvb[0]);
            second_type_descriptor->write_value(static_cast<const void*>(&ax::snd(*pair_ptr)), symbols_mvb[1]);
            target_symbol = ax::symbols(std::move(symbols_mvb));
        }
    };

    template<typename R>
    class record_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record);
            ax::assign_value<R>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record);
            ax::assign_value<R>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(R, record);
            VAR* record_ptr = static_cast<R*>(target_ptr);
            source_symbol.match(
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
                VAL& first_type_descriptor = ax::get_type_descriptor<typename R::first_type>();
                VAL& second_type_descriptor = ax::get_type_descriptor<typename R::second_type>();
                VAL& third_type_descriptor = ax::get_type_descriptor<typename R::third_type>();
                first_type_descriptor->read_value(source_symbols[0], &first_value_mvb);
                second_type_descriptor->read_value(source_symbols[1], &second_value_mvb);
                third_type_descriptor->read_value(source_symbols[2], &third_value_mvb);
                *record_ptr = R(std::move(first_value_mvb), std::move(second_value_mvb), std::move(third_value_mvb));
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(R, record);
            VAL* record_ptr = static_cast<const R*>(source_ptr);
            ax::symbols_t symbols_mvb{};
            symbols_mvb.resize(3_z);
            VAL& first_type_descriptor = ax::get_type_descriptor<typename R::first_type>();
            VAL& second_type_descriptor = ax::get_type_descriptor<typename R::second_type>();
            VAL& third_type_descriptor = ax::get_type_descriptor<typename R::third_type>();
            first_type_descriptor->write_value(static_cast<const void*>(&ax::fst(*record_ptr)), symbols_mvb[0]);
            second_type_descriptor->write_value(static_cast<const void*>(&ax::snd(*record_ptr)), symbols_mvb[1]);
            third_type_descriptor->write_value(static_cast<const void*>(&ax::thd(*record_ptr)), symbols_mvb[2]);
            target_symbol = ax::symbols(std::move(symbols_mvb));
        }
    };

    template<typename R>
    class record4_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record4);
            ax::assign_value<R>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record4);
            ax::assign_value<R>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(R, record4);
            VAR* record_ptr = static_cast<R*>(target_ptr);
            source_symbol.match(
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
                VAL& first_type_descriptor = ax::get_type_descriptor<typename R::first_type>();
                VAL& second_type_descriptor = ax::get_type_descriptor<typename R::second_type>();
                VAL& third_type_descriptor = ax::get_type_descriptor<typename R::third_type>();
                VAL& fourth_type_descriptor = ax::get_type_descriptor<typename R::fourth_type>();
                first_type_descriptor->read_value(source_symbols[0], &first_value_mvb);
                second_type_descriptor->read_value(source_symbols[1], &second_value_mvb);
                third_type_descriptor->read_value(source_symbols[2], &third_value_mvb);
                fourth_type_descriptor->read_value(source_symbols[3], &fourth_value_mvb);
                *record_ptr = R(std::move(first_value_mvb), std::move(second_value_mvb), std::move(third_value_mvb), std::move(fourth_value_mvb));
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(R, record4);
            VAL* record_ptr = static_cast<const R*>(source_ptr);
            ax::symbols_t symbols_mvb{};
            symbols_mvb.resize(4_z);
            VAL& first_type_descriptor = ax::get_type_descriptor<typename R::first_type>();
            VAL& second_type_descriptor = ax::get_type_descriptor<typename R::second_type>();
            VAL& third_type_descriptor = ax::get_type_descriptor<typename R::third_type>();
            VAL& fourth_type_descriptor = ax::get_type_descriptor<typename R::fourth_type>();
            first_type_descriptor->write_value(static_cast<const void*>(&ax::fst(*record_ptr)), symbols_mvb[0]);
            second_type_descriptor->write_value(static_cast<const void*>(&ax::snd(*record_ptr)), symbols_mvb[1]);
            third_type_descriptor->write_value(static_cast<const void*>(&ax::thd(*record_ptr)), symbols_mvb[2]);
            fourth_type_descriptor->write_value(static_cast<const void*>(&ax::frth(*record_ptr)), symbols_mvb[3]);
            target_symbol = ax::symbols(std::move(symbols_mvb));
        }
    };

    template<typename R>
    class record5_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record5);
            ax::assign_value<R>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(R, record5);
            ax::assign_value<R>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(R, record5);
            VAR* record_ptr = static_cast<R*>(target_ptr);
            source_symbol.match(
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
                VAL& first_type_descriptor = ax::get_type_descriptor<typename R::first_type>();
                VAL& second_type_descriptor = ax::get_type_descriptor<typename R::second_type>();
                VAL& third_type_descriptor = ax::get_type_descriptor<typename R::third_type>();
                VAL& fourth_type_descriptor = ax::get_type_descriptor<typename R::fourth_type>();
                VAL& fifth_type_descriptor = ax::get_type_descriptor<typename R::fifth_type>();
                first_type_descriptor->read_value(source_symbols[0], &first_value_mvb);
                second_type_descriptor->read_value(source_symbols[1], &second_value_mvb);
                third_type_descriptor->read_value(source_symbols[2], &third_value_mvb);
                fourth_type_descriptor->read_value(source_symbols[3], &fourth_value_mvb);
                fifth_type_descriptor->read_value(source_symbols[4], &fifth_value_mvb);
                *record_ptr = R(
                    std::move(first_value_mvb),
                    std::move(second_value_mvb),
                    std::move(third_value_mvb),
                    std::move(fourth_value_mvb),
                    std::move(fifth_value_mvb));
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(R, record5);
            VAL* record_ptr = static_cast<const R*>(source_ptr);
            ax::symbols_t symbols_mvb{};
            symbols_mvb.resize(5_z);
            VAL& first_type_descriptor = ax::get_type_descriptor<typename R::first_type>();
            VAL& second_type_descriptor = ax::get_type_descriptor<typename R::second_type>();
            VAL& third_type_descriptor = ax::get_type_descriptor<typename R::third_type>();
            VAL& fourth_type_descriptor = ax::get_type_descriptor<typename R::fourth_type>();
            VAL& fifth_type_descriptor = ax::get_type_descriptor<typename R::fifth_type>();
            first_type_descriptor->write_value(static_cast<const void*>(&ax::fst(*record_ptr)), symbols_mvb[0]);
            second_type_descriptor->write_value(static_cast<const void*>(&ax::snd(*record_ptr)), symbols_mvb[1]);
            third_type_descriptor->write_value(static_cast<const void*>(&ax::thd(*record_ptr)), symbols_mvb[2]);
            fourth_type_descriptor->write_value(static_cast<const void*>(&ax::frth(*record_ptr)), symbols_mvb[3]);
            fifth_type_descriptor->write_value(static_cast<const void*>(&ax::ffth(*record_ptr)), symbols_mvb[4]);
            target_symbol = ax::symbols(std::move(symbols_mvb));
        }
    };

    template<typename T>
    class option_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<ax::option<T>>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            ax::assign_value<ax::option<T>>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            VAR* option_ptr = static_cast<ax::option<T>*>(target_ptr);
            VAL& type_descriptor = ax::get_type_descriptor<T>();
            source_symbol.match(
            [&](VAL& atom)
            {
                if (atom != "none") throw std::invalid_argument("Expected the atom 'none'.");
                *option_ptr = ax::none<T>();
            },
            [&](VAL&)
            {
                throw std::invalid_argument("Expected atom or symbols value.");
            },
            [&](VAL& symbols)
            {
                if (symbols.size() != 2 ||
                    !symbols[0].is_atom() ||
                    symbols[0].get_atom() != "some")
                    throw std::invalid_argument("Expected two symbols, with the first being the atom 'some'.");
                T some_value_mvb{};
                type_descriptor->read_value(symbols[1], &some_value_mvb);
                *option_ptr = ax::some(std::move(some_value_mvb));
            });
        }

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            VAL* option_ptr = static_cast<const ax::option<T>*>(source_ptr);
            option_ptr->match(
            [&](VAL& some_value)
            {
                ax::symbol symbol_value_mvb{};
                VAL& type_descriptor = ax::get_type_descriptor<T>();
                type_descriptor->write_value(static_cast<const void*>(&some_value), symbol_value_mvb);
                target_symbol = ax::symbols({ ax::atom("some"), std::move(symbol_value_mvb) });
            },
            [&]() { target_symbol = ax::atom("none"); });
        }
    };

    template<typename E>
    class either_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(E, either);
            assign_value<E>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(E, either);
            assign_value<E>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(E, either);
            VAR* either_ptr = static_cast<E*>(target_ptr);
            source_symbol.match(
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
                VAL& right_name = either_ptr->get_right_name();
                VAL& left_name = either_ptr->get_left_name();
                if (!symbol_name.is_atom())
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target either
                VAL& either_name = symbol_name.get_atom();
                if (either_name == right_name)
                {
                    typename E::right_type right_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename E::right_type>();
                    type_descriptor->read_value(symbol_value, &right_value_mvb);
                    *either_ptr = E(std::move(right_value_mvb), false);
                }
                else if (either_name == left_name)
                {
                    typename E::left_type left_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename E::left_type>();
                    type_descriptor->read_value(symbol_value, &left_value_mvb);
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

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(E, either);
            VAL* either_ptr = static_cast<const E*>(source_ptr);
            either_ptr->match(
            [&](VAL& right_value)
            {
                ax::symbol symbol_mvb{};
                VAL& right_type_descriptor = ax::get_type_descriptor<typename E::right_type>();
                right_type_descriptor->write_value(static_cast<const void*>(&right_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(either_ptr->get_right_name()), std::move(symbol_mvb) });
            },
            [&](VAL& left_value)
            {
                ax::symbol symbol_mvb{};
                VAL& left_type_descriptor = ax::get_type_descriptor<typename E::left_type>();
                left_type_descriptor->write_value(static_cast<const void*>(&left_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(either_ptr->get_left_name()), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice);
            ax::assign_value<C>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice);
            ax::assign_value<C>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(C, choice);
            VAR* choice_ptr = static_cast<C*>(target_ptr);
            source_symbol.match(
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
                VAL& first_name = choice_ptr->get_first_name();
                VAL& second_name = choice_ptr->get_second_name();
                VAL& third_name = choice_ptr->get_third_name();
                if (!symbol_name.is_atom())
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target choice
                VAL& choice_name = symbol_name.get_atom();
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::first_type>();
                    type_descriptor->read_value(symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb), false);
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::second_type>();
                    type_descriptor->read_value(symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false, false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::third_type>();
                    type_descriptor->read_value(symbol_value, &third_value_mvb);
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

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(C, choice);
            VAL* choice_ptr = static_cast<const C*>(source_ptr);
            choice_ptr->match(
            [&](VAL& first_value)
            {
                ax::symbol symbol_mvb{};
                VAL& first_type_descriptor = ax::get_type_descriptor<typename C::first_type>();
                first_type_descriptor->write_value(static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_first_name()), std::move(symbol_mvb) });
            },
            [&](VAL& second_value)
            {
                ax::symbol symbol_mvb{};
                VAL& second_type_descriptor = ax::get_type_descriptor<typename C::second_type>();
                second_type_descriptor->write_value(static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_second_name()), std::move(symbol_mvb) });
            },
            [&](VAL& third_value)
            {
                ax::symbol symbol_mvb{};
                VAL& third_type_descriptor = ax::get_type_descriptor<typename C::third_type>();
                third_type_descriptor->write_value(static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_third_name()), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice4_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice4);
            ax::assign_value<C>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice4);
            ax::assign_value<C>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(C, choice4);
            VAR* choice_ptr = static_cast<C*>(target_ptr);
            source_symbol.match(
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
                VAL& first_name = choice_ptr->get_first_name();
                VAL& second_name = choice_ptr->get_second_name();
                VAL& third_name = choice_ptr->get_third_name();
                VAL& fourth_name = choice_ptr->get_fourth_name();
                if (!symbol_name.is_atom())
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target choice
                VAL& choice_name = symbol_name.get_atom();
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::first_type>();
                    type_descriptor->read_value(symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb), false);
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::second_type>();
                    type_descriptor->read_value(symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false, false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::third_type>();
                    type_descriptor->read_value(symbol_value, &third_value_mvb);
                    *choice_ptr = C(std::move(third_value_mvb), false, false, false);
                }
                else if (choice_name == fourth_name)
                {
                    typename C::fourth_type fourth_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::fourth_type>();
                    type_descriptor->read_value(symbol_value, &fourth_value_mvb);
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

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(C, choice4);
            VAL* choice_ptr = static_cast<const C*>(source_ptr);
            choice_ptr->match(
            [&](VAL& first_value)
            {
                ax::symbol symbol_mvb{};
                VAL& first_type_descriptor = ax::get_type_descriptor<typename C::first_type>();
                first_type_descriptor->write_value(static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_first_name()), std::move(symbol_mvb) });
            },
            [&](VAL& second_value)
            {
                ax::symbol symbol_mvb{};
                VAL& second_type_descriptor = ax::get_type_descriptor<typename C::second_type>();
                second_type_descriptor->write_value(static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_second_name()), std::move(symbol_mvb) });
            },
            [&](VAL& third_value)
            {
                ax::symbol symbol_mvb{};
                VAL& third_type_descriptor = ax::get_type_descriptor<typename C::third_type>();
                third_type_descriptor->write_value(static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_third_name()), std::move(symbol_mvb) });
            },
            [&](VAL& fourth_value)
            {
                ax::symbol symbol_mvb{};
                VAL& fourth_type_descriptor = ax::get_type_descriptor<typename C::fourth_type>();
                fourth_type_descriptor->write_value(static_cast<const void*>(&fourth_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_fourth_name()), std::move(symbol_mvb) });
            });
        }
    };

    template<typename C>
    class choice5_descriptor : public ax::type_descriptor
    {
    public:

        void inspect_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice5);
            ax::assign_value<C>(source_ptr, target_ptr);
        }

        void inject_value(const void* source_ptr, void* target_ptr) const override
        {
            CONSTRAIN(C, choice5);
            ax::assign_value<C>(source_ptr, target_ptr);
        }

        void read_value(const ax::symbol& source_symbol, void* target_ptr) const override
        {
            // read target value from source symbol
            CONSTRAIN(C, choice5);
            VAR* choice_ptr = static_cast<C*>(target_ptr);
            source_symbol.match(
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
                VAL& first_name = choice_ptr->get_first_name();
                VAL& second_name = choice_ptr->get_second_name();
                VAL& third_name = choice_ptr->get_third_name();
                VAL& fourth_name = choice_ptr->get_fourth_name();
                VAL& fifth_name = choice_ptr->get_fifth_name();
                if (!symbol_name.is_atom())
                    throw std::invalid_argument("Expected two symbols with valid atom names.");

                // populate target choice
                VAL& choice_name = symbol_name.get_atom();
                if (choice_name == first_name)
                {
                    typename C::first_type first_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::first_type>();
                    type_descriptor->read_value(symbol_value, &first_value_mvb);
                    *choice_ptr = C(std::move(first_value_mvb), false);
                }
                else if (choice_name == second_name)
                {
                    typename C::second_type second_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::second_type>();
                    type_descriptor->read_value(symbol_value, &second_value_mvb);
                    *choice_ptr = C(std::move(second_value_mvb), false, false);
                }
                else if (choice_name == third_name)
                {
                    typename C::third_type third_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::third_type>();
                    type_descriptor->read_value(symbol_value, &third_value_mvb);
                    *choice_ptr = C(std::move(third_value_mvb), false, false, false);
                }
                else if (choice_name == fourth_name)
                {
                    typename C::fourth_type fourth_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::fourth_type>();
                    type_descriptor->read_value(symbol_value, &fourth_value_mvb);
                    *choice_ptr = C(std::move(fourth_value_mvb), false, false, false, false);
                }
                else if (choice_name == fifth_name)
                {
                    typename C::fifth_type fifth_value_mvb{};
                    VAL& type_descriptor = ax::get_type_descriptor<typename C::fifth_type>();
                    type_descriptor->read_value(symbol_value, &fifth_value_mvb);
                    *choice_ptr = C(std::move(fifth_value_mvb), false, false, false, false, false);
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

        void write_value(const void* source_ptr, ax::symbol& target_symbol) const override
        {
            CONSTRAIN(C, choice5);
            VAL* choice_ptr = static_cast<const C*>(source_ptr);
            choice_ptr->match(
            [&](VAL& first_value)
            {
                ax::symbol symbol_mvb{};
                VAL& first_type_descriptor = ax::get_type_descriptor<typename C::first_type>();
                first_type_descriptor->write_value(static_cast<const void*>(&first_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_first_name()), std::move(symbol_mvb) });
            },
            [&](VAL& second_value)
            {
                ax::symbol symbol_mvb{};
                VAL& second_type_descriptor = ax::get_type_descriptor<typename C::second_type>();
                second_type_descriptor->write_value(static_cast<const void*>(&second_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_second_name()), std::move(symbol_mvb) });
            },
            [&](VAL& third_value)
            {
                ax::symbol symbol_mvb{};
                VAL& third_type_descriptor = ax::get_type_descriptor<typename C::third_type>();
                third_type_descriptor->write_value(static_cast<const void*>(&third_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_third_name()), std::move(symbol_mvb) });
            },
            [&](VAL& fourth_value)
            {
                ax::symbol symbol_mvb{};
                VAL& fourth_type_descriptor = ax::get_type_descriptor<typename C::fourth_type>();
                fourth_type_descriptor->write_value(static_cast<const void*>(&fourth_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_fourth_name()), std::move(symbol_mvb) });
            },
            [&](VAL& fifth_value)
            {
                ax::symbol symbol_mvb{};
                VAL& fifth_type_descriptor = ax::get_type_descriptor<typename C::fifth_type>();
                fifth_type_descriptor->write_value(static_cast<const void*>(&fifth_value), symbol_mvb);
                target_symbol = ax::symbols({ ax::atom(choice_ptr->get_fifth_name()), std::move(symbol_mvb) });
            });
        }
    };
}

#endif
