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
    struct vector
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

        vector() :
            small_begin{},
            small_end(small_begin),
            big_vector()
        { }

        vector(std::initializer_list<T> il) :
            vector(il.begin(), il.end())
        { }

        vector(const T* begin, const T* end)
        {
            VAR size = static_cast<std::size_t>(end - begin);
            if (size > N)
            {
                small_end = nullptr;
                big_vector.insert(big_vector.begin(), begin, end);
            }
            else
            {
                small_end = small_begin;
                for (VAR i = 0_z; i < N && begin != end; ++i, ++begin)
                {
                    *small_end = *begin;
                    ++small_end;
                }
            }
        }

        vector(const ax::vector<T, A, N>& that)
        {
            if (that.big())
            {
                small_end = nullptr;
                big_vector = that.big_vector;
            }
            else
            {
                small_end = small_begin;
                VAL size = that.size();
                for (VAR i = 0_z; i < size; ++i)
                {
                    *small_end = that.small_begin[i];
                    ++small_end;
                }
            }
        }

        vector(ax::vector<T, A, N>&& that)
        {
            if (that.big())
            {
                small_end = nullptr;
                big_vector = std::move(that.big_vector);
            }
            else
            {
                small_end = small_begin;
                VAL size = that.size();
                for (VAR i = 0_z; i < size; ++i)
                {
                    *small_end = that.small_begin[i];
                    ++small_end;
                }
            }
        }

        ~vector() = default;

        ax::vector<T, A, N>& operator=(const ax::vector<T, A, N>& that)
        {
            big_vector = that.big_vector;
            VAL size = that.size();
            for (VAR i = 0_z; i < size; ++i)
            {
                *small_end = that.small_begin[i];
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
                *small_end = that.small_begin[i];
                ++small_end;
            }
            return *this;
        }

        bool operator==(const ax::vector<T, A, N>& that) const
        {
            VAL size = this->size();
            if (size > N)
            {
                return big_vector == that.big_vector;
            }
            else
            {
                VAR result = true;
                for (VAR i = 0_z; result && i < size; ++i)
                {
                    result = small_begin[i] == that.small_begin[i];
                }
                return result;
            }
        }

        bool operator!=(const ax::vector<T, A, N>& that) const
        {
            return !(*this == that);
        }

        T* begin() { return big() ? big_vector.data() : small_begin; }
        T* end() { return big() ? big_vector.data() + big_vector.size() : small_end; }
        const T* cbegin() const { return const_cast<ax::vector<T, A, N>*>(this)->begin(); }
        const T* cend() const { return const_cast<ax::vector<T, A, N>*>(this)->end(); }
        const T* begin() const { return cbegin(); }
        const T* end() const { return cend(); }

        void push_back(const T& item)
        {
            if (big())
            {
                big_vector.push_back(item);
            }
            else if (size() == N)
            {
                big_vector.insert(big_vector.begin(), small_begin, small_end);
                big_vector.push_back(item);
                small_end = nullptr;
            }
            else
            {
                new (small_end) T(item);
                ++small_end;
            }
        }

        void pop_back()
        {
            if (big())
            {
                big_vector.pop_back();
                return;
            }
            else if (size() != 0)
            {
                (small_end - 1)->~T();
                --small_end;
                return;
            }
            else throw std::range_error("Cannot pop_back empty ax::vector.");
        }

        T* erase(const T* iter)
        {
            if (big())
            {
                VAL pos = iter - big_vector.data();
                VAL& iter = big_vector.begin() + pos;
                return &*big_vector.erase(iter);
            }
            else
            {
                small_end = std::remove(small_begin, small_end, *iter);
                return small_end;
            }
        }

        T& at(std::size_t pos)
        {
            if (big()) return big_vector.at(pos);
            if (pos < size()) return *(small_begin + pos);
            throw std::out_of_range("ax::vector indexed out of range.");
        }

        const T& at(std::size_t pos) const
        {
            return const_cast<ax::vector<T, A, N>*>(this)->at(pos);
        }

        void assign(std::size_t pos, const T& item)
        {
            if (big()) return big_vector.assign(pos, item);
            if (pos < size()) return small_begin + pos;
            throw std::out_of_range("ax::vector indexed out of range.");
        }

        T& front()
        {
            if (big()) return big_vector.front();
            if (size() > 0) return *small_begin;
            throw std::out_of_range("ax::vector indexed out of range.");
        }

        T& back()
        {
            if (big()) return big_vector.back();
            if (size() > 0) return *(small_end - 1);
            throw std::out_of_range("ax::vector indexed out of range.");
        }

        std::size_t capacity() const
        {
            if (big()) return big_vector.capacity();
            return N;
        }

        void clear()
        {
            if (big()) big_vector.clear();
            else small_end = small_begin;
        }

        T* data()
        {
            if (big()) return big_vector.data();
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

        inline bool big() const
        {
            return !small_end;
        }

    private:

        T small_begin[N];
        T* small_end;
        std::vector<T, A> big_vector;
    };
}

#endif
