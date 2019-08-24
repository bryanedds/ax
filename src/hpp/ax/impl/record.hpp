#ifndef IMPL_AX_RECORD_HPP
#define IMPL_AX_RECORD_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    template<typename F, typename S, typename T>
    class record
    {
    public:

        CONSTRAINT(record);
        using first_type = F;
        using second_type = S;
        using third_type = T;
        template<typename A, typename B, typename C>
        using reify = record<A, B, C>;

        record() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        record(const record&) = default;
        record(record&&) = default;
        record(const F& first, const S& second, const T& third) : first(first), second(second), third(third) { }
        record(F&& first, S&& second, T&& third) : first(first), second(second), third(third) { }
        virtual ~record() = default;

        record& operator=(const record&) = default;
        record& operator=(record&&) = default;

        const F& fst() const { return first; }
        const S& snd() const { return second; }
        const T& thd() const { return third; }

        F& fst() { return first; }
        S& snd() { return second; }
        T& thd() { return third; }

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }

    private:

        F first;
        S second;
        T third;
    };

    template<typename F, typename S, typename T>
    const F& fst(const record<F, S, T>& rcd) { return rcd.fst(); }

    template<typename F, typename S, typename T>
    const S& snd(const record<F, S, T>& rcd) { return rcd.snd(); }

    template<typename F, typename S, typename T>
    const T& thd(const record<F, S, T>& rcd) { return rcd.thd(); }

    template<typename F, typename S, typename T>
    F& fst(record<F, S, T>& rcd) { return rcd.fst(); }

    template<typename F, typename S, typename T>
    S& snd(record<F, S, T>& rcd) { return rcd.snd(); }

    template<typename F, typename S, typename T>
    T& thd(record<F, S, T>& rcd) { return rcd.thd(); }

    template<typename F, typename S, typename T>
    record<F, S, T> make_record(const F& first, const S& second, const T& third)
    {
        return record<F, S, T>(first, second, third);
    }

    template<typename F, typename S, typename T>
    record<F, S, T> make_record(F&& first, S&& second, T&& third)
    {
        return record<F, S, T>(first, second, third);
    }
}

#define PRODUCT_TYPE3(T, Ft, Fn, St, Sn, Tt, Tn) \
    class T : public ::ax::record<Ft, St, Tt> \
    { \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::record<Ft, St, Tt>::record; \
        \
        inline const T::first_type& get_##Fn(const T& rcd) { return fst(rcd); } \
        inline const T::second_type& get_##Sn(const T& rcd) { return snd(rcd); } \
        inline const T::third_type& get_##Tn(const T& rcd) { return thd(rcd); } \
        \
        inline T::first_type& get_##Fn(T& rcd) { return fst(rcd); } \
        inline T::second_type& get_##Sn(T& rcd) { return snd(rcd); } \
        inline T::third_type& get_##Tn(T& rcd) { return thd(rcd); } \
        \
        const char* get_first_name() const override { return #Fn; } \
        const char* get_second_name() const override { return #Sn; } \
        const char* get_third_name() const override { return #Tn; } \
    }; \
    \
    T make_##T(const T::first_type& first, const T::second_type& second, const T::third_type& third) \
    { \
        return T(first, second, third); \
    } \
    \
    T make_##T(T::first_type&& first, T::second_type&& second, T::third_type&& third) \
    { \
        return T(first, second, third); \
    } \
    \
    using T##_product_type3 = void

#endif
