#ifndef IMPL_AX_RECORD_HPP
#define IMPL_AX_RECORD_HPP

#include <cstddef>
#include <stdexcept>

#include "prelude.hpp"

namespace ax
{
    // TODO: build record types up to record7.
    template<typename First, typename Second, typename Third>
    class record
    {
    private:

        First first;
        Second second;
        Third third;

    protected:

        virtual const char* get_first_name() const { return "first"; }
        virtual const char* get_second_name() const { return "second"; }
        virtual const char* get_third_name() const { return "third"; }

        template<typename A, typename B, typename C>
        friend const A& fst(const record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend const B& snd(const record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend const C& thd(const record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend A& fst(record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend B& snd(record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend C& thd(record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend const char* get_first_name(const record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend const char* get_second_name(const record<A, B, C>& rcd);

        template<typename A, typename B, typename C>
        friend const char* get_third_name(const record<A, B, C>& rcd);

    public:

        CONSTRAINT(record);
        using first_type = First;
        using second_type = Second;
        using third_type = Third;
        template<typename A, typename B, typename C>
        using reify = record<A, B, C>;
        
        record() { } // NOTE: do not change this to = default as that makes MSVC think it is deleted when inherited!
        record(const record&) = default;
        record(record&&) = default;
        record& operator=(const record&) = default;
        record& operator=(record&&) = default;
        ~record() = default;

        record(const First& first, const Second& second, const Third& third) : first(first), second(second), third(third) { }
        record(First&& first_mvb, Second&& second_mvb, Third&& third_mvb) : first(first_mvb), second(second_mvb), third(third_mvb) { }
    };

    template<typename First, typename Second, typename Third>
    const First& fst(const record<First, Second, Third>& rcd)
    {
        return rcd.first;
    }

    template<typename First, typename Second, typename Third>
    const Second& snd(const record<First, Second, Third>& rcd)
    {
        return rcd.second;
    }

    template<typename First, typename Second, typename Third>
    const Third& thd(const record<First, Second, Third>& rcd)
    {
        return rcd.third;
    }

    template<typename First, typename Second, typename Third>
    First& fst(record<First, Second, Third>& rcd)
    {
        return rcd.first;
    }

    template<typename First, typename Second, typename Third>
    Second& snd(record<First, Second, Third>& rcd)
    {
        return rcd.second;
    }

    template<typename First, typename Second, typename Third>
    Third& thd(record<First, Second, Third>& rcd)
    {
        return rcd.third;
    }

    template<typename First, typename Second, typename Third>
    const char* get_first_name(const record<First, Second, Third>& rcd)
    {
        return rcd.get_first_name();
    }

    template<typename First, typename Second, typename Third>
    const char* get_second_name(const record<First, Second, Third>& rcd)
    {
        return rcd.get_second_name();
    }

    template<typename First, typename Second, typename Third>
    const char* get_third_name(const record<First, Second, Third>& rcd)
    {
        return rcd.get_third_name();
    }

    template<typename First, typename Second, typename Third>
    record<First, Second, Third> make_record(const First& first, const Second& second, const Third& third)
    {
        return record<First, Second, Third>(first, second, third);
    }

    template<typename First, typename Second, typename Third>
    record<First, Second, Third> make_record(First&& first, Second&& second, Third&& third)
    {
        return record<First, Second, Third>(first, second, third);
    }
}

#define PRODUCT_TYPE3(T, Ft, Fn, St, Sn, Tt, Tn) \
    class T : public ::ax::record<Ft, St, Tt> \
    { \
    protected: \
    \
        const char* get_first_name() const override { return #Sn; } \
        const char* get_second_name() const override { return #Fn; } \
        const char* get_third_name() const override { return #Tn; } \
    \
    public: \
    \
        CONSTRAINT(T); \
        using ::ax::record<Ft, St, Tt>::record; \
    }; \
    \
    inline const T::first_type& get_##Fn(const T& rcd) \
    { \
        return fst(rcd); \
    } \
    \
    inline const T::second_type& get_##Sn(const T& rcd) \
    { \
        return snd(rcd); \
    } \
    \
    inline const T::third_type& get_##Tn(const T& rcd) \
    { \
        return thd(rcd); \
    } \
    \
    inline T::first_type& get_##Fn(T& rcd) \
    { \
        return fst(rcd); \
    } \
    \
    inline T::second_type& get_##Sn(T& rcd) \
    { \
        return snd(rcd); \
    } \
    \
    inline T::third_type& get_##Tn(T& rcd) \
    { \
        return thd(rcd); \
    } \
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
