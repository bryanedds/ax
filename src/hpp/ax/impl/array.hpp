#ifndef IMPL_AX_ARRAY_HPP
#define IMPL_AX_ARRAY_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <memory>

#include "prelude.hpp"
#include "string.hpp"
#include "name.hpp"

namespace ax
{
    // An array with small size optimization.
    template<typename T, std::size_t N>
    class array_t
    {
    public:

        CONSTRAINT(array_t);
        using value_type = T;
        using size_type = std::size_t;
        template<typename T2, std::size_t N2>
        using reify = array_t<T2, N2>;

        array_t() : array_t({ }) { }

        array_t(std::initializer_list<T> il) :
            array_t(std::vector<T>(il)) { }

        array_t(const std::vector<T>& items)
        {
            VAR size = items.size();
            if (size > N)
            {
                big_array.reset(new T[items.size()]);
                for (VAR i = 0_z; i < size; ++i) big_array.get()[i] = items[i]; // TODO: use memcpy
                this->size = size;
            }
            else
            {
                VAR i = 0_z;
                for (VAR& item : items)
                {
                    if (i >= N) break;
                    small_array[i] = item;
                    ++i;
                }
                this->size = i - 1;
            }
        }

        array_t(const array_t<T, N>& that) :
            big_array(that.big_array.get()),
            small_array{ },
            size(that.size)
        {
            for (VAR i = 0_z; i < N; ++i) small_array[i] = that.small_array[i]; // TODO: use memcpy
        }

        array_t(array_t<T, N>&& that) :
            big_array(std::move(that.big_array)),
            small_array{ },
            size(that.size)
        {
            for (VAR i = 0_z; i < N; ++i) small_array[i] = that.small_array[i]; // TODO: use memcpy
        }

        ~array_t() = default;

        array_t<T, N>& operator=(const array_t<T, N>& that)
        {
            if (that.big_array)
            {
                T* big_array_copy = new T[that.size];
                for (VAR i = 0_z; i < that.size; ++i) big_array.get()[i] = that.big_array.get()[i]; // TODO: use memcpy
                big_array.reset(big_array_copy);
            }
            else
            {
                for (VAR i = 0_z; i < N; ++i) small_array[i] = that.small_array[i]; // TODO: use memcpy
            }
            size = that.size;
            return *this;
        }

        array_t<T, N>& operator=(array_t<T, N>&& that)
        {
            big_array.reset(that.big_array.release());
            for (VAR i = 0_z; i < N; ++i) small_array[i] = that.small_array[i]; // TODO: use memcpy
            size = that.size;
            return *this;
        }

        bool operator==(const array_t<T, N>& that) const
        {
            return
                big_array.get() == that.big_array.get() &&
                small_array == that.small_array &&
                size == that.size;
        }

        bool operator!=(const array_t<T, N>& that) const
        {
            return !(*this == that);
        }

        T* begin() { return big_array ? big_array.get() : small_array; }
        T* end() { return (big_array ? big_array.get() : small_array) + size; }
        const T* cbegin() const { return const_cast<array_t<T, N>*>(this)->begin(); }
        const T* cend() const { return const_cast<array_t<T, N>*>(this)->end(); }
        const T* begin() const { return cbegin(); }
        const T* end() const { return cend(); }

    private:

        std::unique_ptr<T> big_array;
        T small_array[N];
        std::size_t size;
    };
}

#endif
