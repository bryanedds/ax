// Copyright Edd Dawson 2008
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "../../hpp/blah/blah.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>

#if !defined(NDEBUG)
#include <typeinfo>
#endif

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined(_POSIX_VERSION) && !defined(BLAH_NO_THREAD_SUPPORT)
    #include <pthread.h>
#endif

namespace
{
    template<typename T, typename U>
    bool same_size(const T &lhs, const U &rhs) { return lhs.length() == rhs.length(); }
    bool same(const char *lhs, const char *rhs) { return std::strcmp(lhs, rhs) == 0; }

    std::deque<blah::filter> default_filters()
    {
        std::deque<blah::filter> filts;
        filts.push_back(blah::write_to(std::clog));
        filts.push_back(&blah::add_missing_newline);
        filts.push_back(&blah::prepend_details);

        return filts;
    }

    std::deque<blah::filter> filters(default_filters());

    struct logger : blah::logger
    {
        void log(const blah::entry &e, const std::string &text)
        {
            std::string mutable_text(text);
            std::deque<blah::filter>::const_reverse_iterator rb = filters.rbegin(), re = filters.rend();

            while (rb != re && (*rb)(e, mutable_text) == blah::carry_on)
                ++rb;
        }
    };

    logger lgr_;

    class counted_stringbuf : public std::stringbuf
    {
        public:
            counted_stringbuf() : std::stringbuf(std::ios_base::out), n_(1) { }
            std::size_t count() const { return n_; }
            void increment() { ++n_; }
            void decrement() { if (--n_ == 0) delete this; }

        private:
            std::size_t n_;
    };
}

namespace blah
{
    //------------------------------------------------------------------------------------------
    // thread_id_t
    //------------------------------------------------------------------------------------------
#if defined(_POSIX_VERSION) && !defined(BLAH_NO_THREAD_SUPPORT)
    struct stream_state
    {
        stream_state(std::ostream &out) : out(out), flags(out.flags()) { }
        ~stream_state() { out.flags(flags); }
        std::ostream &out;
        const std::ios_base::fmtflags flags;
    };

    int cmp(thread_id_t lhs, thread_id_t rhs) { return std::memcmp(&lhs.id, &rhs.id, sizeof lhs.id); }

    bool operator< (thread_id_t lhs, thread_id_t rhs) { return cmp(lhs, rhs) < 0; }
    bool operator> (thread_id_t lhs, thread_id_t rhs) { return cmp(lhs, rhs) > 0; }
    bool operator<= (thread_id_t lhs, thread_id_t rhs) { return cmp(lhs, rhs) <= 0; }
    bool operator>= (thread_id_t lhs, thread_id_t rhs) { return cmp(lhs, rhs) >= 0; }
    bool operator== (thread_id_t lhs, thread_id_t rhs) { return pthread_equal(lhs.id, rhs.id); }
    bool operator!= (thread_id_t lhs, thread_id_t rhs) { return !(lhs == rhs); }

    std::ostream &operator<< (std::ostream &out, thread_id_t id)
    {
    #if defined(BLAH_PTHREAD_T_IS_NOT_PRIMITIVE)
        stream_state save(out);

        const unsigned char *p = reinterpret_cast<const unsigned char *>(&id.id), 
                            *e = p + sizeof id.id;

        out << "0x" << std::hex;
        while (p != e) out << unsigned(*p++);
        return out;
    #else
        // If you get a compiler error here, define BLAH_PTHREAD_T_IS_NOT_PRIMITIVE
        return out << id.id;
    #endif
    }

#endif // _POSIX_VERSION

    //------------------------------------------------------------------------------------------
    // string
    //------------------------------------------------------------------------------------------

    string::string(const char *s) : s_(s), len_(std::strlen(s_)) { }

    string::operator const char *() const { return s_; }

    std::size_t string::length() const { return len_; }

    const char &string::operator[] (std::size_t i) const
    {
        assert(i < length());
        return s_[i];
    }

    bool operator== (const string &lhs, const string &rhs) { return same_size(lhs, rhs) && same(lhs, rhs); }
    bool operator== (const string &lhs, const std::string &rhs) { return same_size(lhs, rhs) && same(lhs, rhs.c_str()); }
    bool operator== (const std::string &lhs, const string &rhs) { return rhs == lhs; }
    bool operator== (const string &lhs, const char *rhs) { return same(lhs, rhs); }
    bool operator== (const char *lhs, const string &rhs) { return rhs == lhs; }

    bool operator!= (const string &lhs, const string &rhs) { return !(lhs == rhs); }
    bool operator!= (const string &lhs, const std::string &rhs) { return !(lhs == rhs); }
    bool operator!= (const std::string &lhs, const string &rhs) { return !(lhs == rhs); }
    bool operator!= (const string &lhs, const char *rhs) { return !(lhs == rhs); }
    bool operator!= (const char *lhs, const string &rhs) { return !(lhs == rhs); }

    bool operator< (const string &lhs, const string &rhs) { return std::strcmp(lhs, rhs) < 0; }
    bool operator> (const string &lhs, const string &rhs) { return std::strcmp(lhs, rhs) > 0; }


    //------------------------------------------------------------------------------------------
    // entry
    //------------------------------------------------------------------------------------------

    entry::entry(const string &log, const string &file, sz_t line, const string &function,
                 const string &date, const string &time) :
        log(log),
        file(file),
        line(line),
        function(function),
        date(date),
        time(time),
#if defined(BLAH_NO_THREAD_SUPPORT)
        thread('?')
#elif defined(_WIN32)
        thread(GetCurrentThreadId())
#elif defined(_POSIX_VERSION)
        thread(pthread_self())
#else
    #error "blah doesn't know how to represent thread identifiers on your platform. Define BLAH_NO_THREAD_SUPPORT if you don't care!"
#endif
    {
    }

    bool operator== (const entry &lhs, const entry &rhs)
    {
        return lhs.log == rhs.log &&
               lhs.file == rhs.file &&
               lhs.line == rhs.line &&
               lhs.thread == rhs.thread;
    }

    bool operator!= (const entry &lhs, const entry &rhs) { return !(lhs == rhs); }

    bool operator< (const entry &lhs, const entry &rhs)
    {
        if (lhs.log < rhs.log) return true;
        if (lhs.log > rhs.log) return false;
        if (lhs.file < rhs.file) return true;
        if (lhs.file > rhs.file) return false;
        if (lhs.line < rhs.line) return true;
        if (lhs.line > rhs.line) return false;

        return std::less<thread_id_t>()(lhs.thread, rhs.thread); 
    }


    //------------------------------------------------------------------------------------------
    // filter
    //------------------------------------------------------------------------------------------

    filter::filter() : f_(0) { }

    filter::~filter() { if (f_) f_->dec(); }

    filter::filter(const filter &other) : f_(other.f_) { if (f_) f_->inc(); }

    filter &filter::operator= (const filter &other)
    {
        filter tmp(other);
        std::swap(f_, tmp.f_);
        return *this;
    }

    progression filter::operator() (const entry &e, std::string &text) const { return f_ ? f_->filt(e, text) : carry_on; }

    filter::operator filter::bool_t () const { return f_ ? &filter::f_ : 0; }


    //------------------------------------------------------------------------------------------
    // library-supplied filters
    //------------------------------------------------------------------------------------------

    progression prepend_details(const entry &e, std::string &text)
    {
        using namespace std;

        const char close[] = "]: ";
        char buff[32] = "";

        // In general std::localtime isn't thread safe, so we look for common alternatives:
#if defined(_WIN32)
        SYSTEMTIME st;
        GetLocalTime(&st);
        const char *months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
        const char *month = months[st.wMonth - 1]; // (1 == Jan, 2 == Feb, ...)
        sprintf_s(buff, "[%02d %s %d %02d:%02d:%02d | ", (int)st.wDay, month, (int)st.wYear, (int)st.wHour, (int)st.wMinute, (int)st.wSecond);
#else
        time_t now = std::time(0);
        tm now_tm;
    #if defined(_POSIX_VERSION)
        localtime_r(&now, &now_tm);
    #else
        #error "localtime() may not be threadsafe on this platform. Comment-out this line if you know better or don't care!"
        now_tm = *localtime(&now);
    #endif
        strftime(buff, sizeof buff, "[%d %b %Y %R:%S | ", &now_tm);
#endif

        std::string new_text(buff);
        new_text.append(e.log, e.log.length());
        new_text.append(close, (sizeof close) - 1);
        new_text.append(text);

        text.swap(new_text);

        return carry_on;
    }

    progression add_missing_newline(const entry &, std::string &text)
    {
        if (text.empty() || *text.rbegin() != '\n') text.push_back('\n');
        return carry_on;
    }

    write_to::write_to(std::ostream &out) : out_(&out) { }

    progression write_to::operator() (const entry &, std::string &text)
    {
        *out_ << text;
        return carry_on;
    }


    //------------------------------------------------------------------------------------------
    // global logger
    //------------------------------------------------------------------------------------------

    logger &global_logger() { return lgr_; }

    void push_filter(const filter &filt) { if (filt) filters.push_back(filt); }

    filter pop_filter()
    {
        filter ret;
        if (!filters.empty())
        {
            ret = filters.back();
            filters.pop_back();
        }
        return ret;
    }


    //------------------------------------------------------------------------------------------
    // log_stream
    //------------------------------------------------------------------------------------------

    log_stream::log_stream(const string &name, const string &file, sz_t line, const string &func,
                           const string &date, const string &time,
                           logger &lgr) :
        std::ostream(new counted_stringbuf()),
        lgr_(&lgr),
        e_(name, file, line, func, date, time)
    {
    }

    log_stream::log_stream(const log_stream &other) :
        std::ios(other.rdbuf()), // virtual base
        std::ostream(other.rdbuf()),
        lgr_(other.lgr_),
        e_(other.e_)
    {
        assert(typeid(*rdbuf()) == typeid(counted_stringbuf));
        static_cast<counted_stringbuf *>(rdbuf())->increment();
    }

    log_stream::~log_stream()
    {
        assert(typeid(*rdbuf()) == typeid(counted_stringbuf));
        counted_stringbuf *buff = static_cast<counted_stringbuf *>(rdbuf());

        try { if (buff->count() == 1) lgr_->log(e_, buff->str()); }
        catch (...) { }

        buff->decrement(); // when it hits 0, it will delete itself
    }

} // close namespace blah
