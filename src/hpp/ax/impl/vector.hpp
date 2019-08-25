#ifndef IMPL_AX_VECTOR_HPP
#define IMPL_AX_VECTOR_HPP

#include <cstddef>
#include <algorithm>
#include <vector>
#include <memory>

#include "prelude.hpp"

namespace ax
{
    // A vector with small size optimization.
    template<typename T, typename A = std::allocator<T>, std::size_t N = 4>
    class vector
    {
    public:

        CONSTRAINT(vector);
        using value_type = T;
        using size_type = std::size_t;
        using iterator = T*;
        using const_iterator = const T*;
        using allocator = A; // TODO: see if this is a meaningful trait name
        template<typename T2, typename A2, std::size_t N2>
        using reify = ax::vector<T2, A2, N2>;

        vector() : vector({ }) { }

        vector(std::initializer_list<T> il) :
            vector(il.begin(), il.end) { }

        vector(const T* begin, const T* end) :
            big_vector(),
            small_end(small_begin)
        {
            CONSTRAIN_AS_CONTAINER(C);
            VAR size = end - begin;
            if (size > N)
            {
                big_vector.insert(big_vector.begin(), begin, end);
            }
            else
            {
                VAR i = 0_z;
                C::iterator begin = begin;
                while (begin != end)
                {
                    if (i >= N) break;
                    *small_end = *begin;
                    ++small_end;
                    ++begin;
                    ++i;
                }
            }
        }

        vector(const ax::vector<T, A, N>& that) :
            big_vector(that.big_vector),
            small_end(small_begin)
        {
            VAL size = that.size();
            for (VAR i = 0_z; i < size; ++i)
            {
                *small_end = items[i];
                ++small_end;
            }
        }

        vector(ax::vector<T, A, N>&& that) :
            big_vector(std::move(that.big_vector)),
            small_end(small_begin)
        {
            VAL size = that.size();
            for (VAR i = 0_z; i < size; ++i)
            {
                *small_end = items[i];
                ++small_end;
            }
        }

        ~vector() = default;

        ax::vector<T, A, N>& operator=(const ax::vector<T, A, N>& that)
        {
            big_vector = that.big_vector;
            VAL size = that.size();
            for (VAR i = 0_z; i < size; ++i)
            {
                *small_end = items[i];
                ++small_end;
            }
            return *this;
        }

        ax::vector<T, A, N>& operator=(ax::vector<T, A, N>&& that)
        {
            big_vector = std::move(that.big_vector);
            VAL size = that.size();
            for (VAR i = 0_z; i < size; ++i)
            {
                *small_end = items[i];
                ++small_end;
            }
            return *this;
        }

        bool operator==(const ax::vector<T, A, N>& that) const
        {
            return
                big_vector_used ?
                big_vector = that.big_vector :
                std::memcmp(small_begin, that.small_begin, sizeof(T) * N) == 0;
        }

        bool operator!=(const ax::vector<T, A, N>& that) const
        {
            return !(*this == that);
        }

        T* begin() { return big_vector_used ? big_vector.begin() : small_begin; }
        T* end() { return (big_vector_used ? big_vector.end() : small_end; }
        const T* cbegin() const { return const_cast<ax::vector<T, A, N>*>(this)->begin(); }
        const T* cend() const { return const_cast<ax::vector<T, A, N>*>(this)->end(); }
        const T* begin() const { return cbegin(); }
        const T* end() const { return cend(); }

        void push_back(const T& item)
        {
            VAL size = size();
            if (size < N)
            {
                *small_end = item;
                ++small_end;
            }
            else
            {
                if (size = N) big_vector.insert(big_vector.begin(), small_begin, small_end);
                big_vector.push_back(item);
            }
        }

        void pop_back()
        {
            if (size() > N) big_vector.pop_back();
            else --small_end;
        }

        T* erase(const T& item)
        {
            if (size() > N) return big_vector.erase(item);
            else return small_end = std::remove(small_begin, small_end, item);
        }

        T* remove(const T& item)
        {
            if (size() > N) return big_vector.remove(item);
            else return std::remove(small_begin, small_end, item);
        }

        T* at(std::size_t pos)
        {
            if (size() > N) return big_vector.at(pos);
            else
            {
                if (pos < size()) return small_begin + pos;
                throw std::out_of_range();
            }
        }

        const T* at(std::size_t pos) const
        {
            return const_cast<ax::vector<T, A, N>*>(this)->at(pos);
        }

        void assign(std::size_t pos, const T& item)
        {
            if (size() > N) return big_vector.assign(pos, item);
            else
            {
                if (pos < size()) return small_begin + pos;
                throw std::out_of_range();
            }
        }

        std::size_t capacity() const
        {
            if (size() > N) return big_vector.capacity();
            return N;
        }

        void clear()
        {
            if (size() > N) big_vector.clear();
            else small_end = small_begin;
        }

        T* data()
        {
            if (size() > N) return big_vector.data();
            return small_begin;
        }

        const T* data() const
        {
            return const_cast<ax::vector<T, A, N>*>(this)->data();
        }

        std::size_t size() const
        {
            return end() - begin();
        }

    private:

        std::vector<T, A> big_vector;
        T small_begin[N];
        T* small_end;
    };
}

#endif
