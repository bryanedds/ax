// Copyright Edd Dawson 2008
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BLAH_HPP_0124_03122008
#define BLAH_HPP_0124_03122008

#define BLAH_VERSION_MAJOR 0
#define BLAH_VERSION_MINOR 2
#define BLAH_VERSION_FIX   1

#include <cstddef>
#include <string>
#include <ostream>

#if !defined(_WIN32)
    #include <unistd.h> // for _POSIX_VERSION
#endif

#if defined(_POSIX_VERSION) && !defined(BLAH_NO_THREAD_SUPPORT)
    #include <pthread.h>
#endif

namespace blah
{
    //! thread_id_t is the native thread identifier for a thread.
    //! To support other platforms in future, it must support equality and ordering comparisons 
    //! (<, ==, !=) and should be output-streamable

#if defined(BLAH_NO_THREAD_SUPPORT)
    // All thread identifiers will be '?'
    typedef char thread_id_t; 
#elif defined(_WIN32)
    // DWORD is a typedef for unsigned long, which is used here to avoid dragging in the massive
    // <windows.h>.
    // See http://msdn.microsoft.com/en-us/library/aa383751%28VS.85%29.aspx
    typedef unsigned long thread_id_t; 
#elif defined(_POSIX_VERSION)
    // Recent versions of POSIX provide leeway for implementations to define pthread_t as a compound type,
    // so we have to do a little work to get everything working uniformly.
    // If pthread_t is not a pointer or integral type on your system (which is rather uncommon) you'll
    // have to define BLAH_PTHREAD_T_IS_NOT_PRIMITIVE.
    struct thread_id_t 
    {
        thread_id_t(pthread_t id) : id(id) { }
        pthread_t id;
    };

    bool operator< (thread_id_t lhs, thread_id_t rhs);
    bool operator> (thread_id_t lhs, thread_id_t rhs);
    bool operator>= (thread_id_t lhs, thread_id_t rhs);
    bool operator<= (thread_id_t lhs, thread_id_t rhs);
    bool operator== (thread_id_t lhs, thread_id_t rhs);
    bool operator!= (thread_id_t lhs, thread_id_t rhs);

    std::ostream &operator<< (std::ostream &out, thread_id_t id);
#else
    #error "blah doesn't know how to represent thread identifiers on your platform. Define BLAH_NO_THREAD_SUPPORT if you don't care!"
#endif

    typedef std::size_t sz_t;

    //! This class is a wrapper around a const char *. It is used by blah to wrap string literals
    //! so that they can be compared with char*s using a natural syntax (==, !=, etc), which is a
    //! common thing to do inside a filter. They are also output streamable. std::string could be
    //! used, but this would imply a lot of allocation and unnecessary copying. I feel it is
    //! important to keep the logging system tight so it can be used without fears of inefficiency.
    class string
    {
        public:
            //! Use this to construct a string from a string literal
            template<sz_t N>
            string(const char (&s)[N]) : s_(s), len_(N - 1) { } // -1 for the '\0' in the array

            //! This is for the benefit of compilers that think string literals are character pointers.
            string(const char *s); 

            //! Returns a C-string representation (\0 terminated). There shouldn't be any lifetime issues
            //! if the string was constructed from a string literal.
            operator const char *() const;

            //! Returns the length of the string
            sz_t length() const;

            //! Returns the ith character in the string. i must be less than size()
            const char &operator[] (sz_t i) const;

        private:
            const char *s_;
            sz_t len_;
    };


    bool operator== (const string &lhs, const string &rhs);
    bool operator== (const string &lhs, const std::string &rhs);
    bool operator== (const std::string &lhs, const string &rhs);
    bool operator== (const string &lhs, const char *rhs);
    bool operator== (const char *lhs, const string &rhs);

    bool operator!= (const string &lhs, const string &rhs);
    bool operator!= (const string &lhs, const std::string &rhs);
    bool operator!= (const std::string &lhs, const string &rhs);
    bool operator!= (const char *lhs, const string &rhs);
    bool operator!= (const string &lhs, const char *rhs);

    bool operator< (const string &lhs, const string &rhs);
    bool operator> (const string &lhs, const string &rhs);

    struct entry
    {
        entry(const string &log, const string &file, sz_t line, const string &function,
              const string &date, const string &time);

        string log;         //!< The name of the log
        string file;        //!< The source file in which the logging occurred
        sz_t line;          //!< The line of the source file in which the logging occurred
        string function;    //!< The name of the function in which the logging occurred
        string date;        //!< This is the date of *compilation*
        string time;        //!< This is the time of *compilation*
        thread_id_t thread; //!< The thread in which the logging occurred (integral id or pointer)
    };

    //! Comparisons are based on the log, file, line and thread members (which uniquely identify an
    //! entry unless somebody logs something twice on a single line, which would be weird).
    bool operator== (const entry &lhs, const entry &rhs);
    bool operator!= (const entry &lhs, const entry &rhs);
    bool operator< (const entry &lhs, const entry &rhs);

    //! These are the possible return codes for filters
    enum progression { stop = 0, carry_on = 1 };

    //! A filter is a functor that takes an entry object and a mutable string and returns a progression
    //! constant, indicating whether any subsequent filters should be applied. The call may modify the
    //! string passed in. The same string will be passed to any subsequent filters.
    //! The global logger object maintains a stack of filters through which an entry object and some
    //! associated text are sent.
    class filter
    {
        private:
            struct filter_base
            {
                filter_base() : n_(1) { }
                virtual ~filter_base() { }
                virtual progression filt(const entry &, std::string &) = 0;
                void dec() { if (--n_ == 0) delete this; }
                void inc() { ++n_; }
                sz_t n_;
            };

            template<typename F>
            struct filter_impl : filter_base
            {
                filter_impl(const F &f) : f_(f) { }
                progression filt(const entry &e, std::string &text) { return f_(e, text); }
                F f_;
            };

            typedef filter_base *filter::* bool_t;

        public:
            //! Constructs a filter from the given functor, which must be copyable and callable with the
            //! following signature:
            //! \code
            //! progression (const entry &, std::string &);
            //! \endcode
            template<typename F>
            filter(F fn) : f_(new filter_impl<F>(fn)) { }

            //! Constructs an empty filter. Calling it does nothing. You can test if a filter is empty by using
            //! it's conversion-to-boolean operator:
            //! \code
            //! filter f;
            //! if (f) { /* not empty */ }
            //! \endcode
            filter();

            ~filter();

            //! Copy constructor. Doesn't throw under any circumstances.
            filter(const filter &other);

            //! Copy assignment operator. Doesn't throw under any circumstances
            filter &operator= (const filter &other);

            //! Call the underlying functor with the given arguments, unless this filter is empty, in which case
            //! this does nothing and returns carry_on.
            progression operator() (const entry &e, std::string &text) const;

            //! Returns an object that is convertible to bool. If true, then the filter is not empty
            operator bool_t () const;

        private:
           filter_base *f_;
    };

    //! This function may be used as a filter to prepend the current date and time and log's name to each
    //! piece of text that is logged. Something like this, where 'err' is the name of the log:
    //! \code
    //! [30th Feb 2008 18:00 | err]: logged text goes here
    //! \endcode
    //! Always returns carry_on.
    progression prepend_details(const entry &e, std::string &text);

    //! The function may be used as a filter to make sure that anything logged ends in a newline.
    //! Always returns carry_on.
    progression add_missing_newline(const entry &e, std::string &text);

    //! This functor may be used as a filter (typically at the bottom of the stack) to write logged data to
    //! the given stream.
    //! NOTE: that if two different filters write to the same stream in different threads, you will have to
    //! make sure that the particular C++ stream is thread-safe on your system.
    class write_to
    {
        public:
            write_to(std::ostream &out);

            //! Writes to the stream given at construction time. Always returns carry_on.
            progression operator() (const entry &e, std::string &text);

        private:
            std::ostream *out_;
    };

    //! The base class for a logger.
    class logger
    {
        public:
            logger() { }
            virtual ~logger() { }
            virtual void log(const entry &e, const std::string &text) = 0;

        private:
            // Copying is explicitly forbidden to prevent slicing
            logger(const logger &);
            logger &operator=(const logger &);
    };

    //! Returns the global logger object, used by default. The global logger maintains a stack of filters
    //! through which log entries are passed. The initial stack contains (top to bottom):
    //! - prepend_details
    //! - add_missing_newline
    //! - write_to(std::clog)
    //!
    //! To clear the initial stack:
    //! \code
    //! while (pop_filter()) { }
    //! \endcode
    logger &global_logger();

    //! Pushes a filter on top of the filter stack of the global logger. Filters added later are applied first
    //! when logging. If filt is empty it is not added to the stack.
    void push_filter(const filter &filt);

    //! Pops a filter off of the filter stack of the global logger. An empty filter is returned if the stack is empty
    filter pop_filter();

    //! This fake stream does nothing. It's used in the expansion of the logging macros when transient logs are disabled.
    //! Even with a "while (false)" infront of a stream insertion expression, some compilers won't optimize-out the
    //! operation if a real stream is used.
    struct null_stream
    {
        template<typename T>
        null_stream &operator<< (const T &) { return *this; }

        operator bool () const { return true; }
    };

    //! A temporary log_stream is created inside the logging macros. When destroyed, the content written to
    //! the stream is written to the logger given to the constructor.
    class log_stream : public std::ostream
    {
        public:
            //! Creates a stream that will write to the given logger when destroyed. The details passed here
            //! are bundled in to an entry object which is passed to that logger's log function.
            log_stream(const string &name, const string &file, sz_t line, const string &func,
                       const string &date, const string &time,
                       logger &lgr);

            //! Creates a stream whose buffer is shared with other.
            log_stream(const log_stream &other);

            //! Writes everything written to this stream during its lifetime to the logger passed on construction.
            ~log_stream();

            //! Returns *this. Used to avoid a problem with using rvalues in the blah macros
            log_stream &get_this() { return *this; }

        private:
            log_stream &operator= (const log_stream &); // not allowed

            logger *lgr_;
            entry e_;
    };
}

#ifdef __BORLANDC__
    #define BLAH_FUNCTION __FUNC__
#else
    #define BLAH_FUNCTION __FUNCTION__
#endif

#define BLAH_TOK_JOIN2(a, b) a ## b
#define BLAH_TOK_JOIN(a, b) BLAH_TOK_JOIN2(a, b)

#ifdef __GNUC__
    #define BLAH_NEVER_DO __attribute__((unused)) const bool BLAH_TOK_JOIN(blah_uNuSeD_VaR_, __LINE__) = true ||
#else
    #define BLAH_NEVER_DO true ||
#endif

#define BLAH_PERMANENT_LOG_WITH(logname, logger) \
    blah::log_stream(logname, __FILE__, __LINE__, BLAH_FUNCTION, __DATE__, __TIME__, logger).get_this()

//! To define a log, MSG, that is always active, even when BLAH_DISABLE_TRANSIENT_LOGS is defined, do:
//! \code
//! #define MSG BLAH_PERMANENT_LOG("msg")
//!
//! // then in your code:
//! MSG << "Hello, log!";
//! \endcode
#define BLAH_PERMANENT_LOG(logname) BLAH_PERMANENT_LOG_WITH(logname, blah::global_logger())

//! To define a log, DBG, that is not compiled in to the code when BLAH_DISABLE_TRANSIENT_LOGS is defined, do:
//! \code
//! #define DBG BLAH_TRANSIENT_LOG("dbg")
//!
//! // then in your code:
//! DBG << "Hello, log!";
//! \endcode
#if defined(BLAH_DISABLE_TRANSIENT_LOGS)
    #define BLAH_TRANSIENT_LOG(logname) BLAH_NEVER_DO blah::null_stream()
    #define BLAH_TRANSIENT_LOG_WITH(logname, logger) BLAH_TRANSIENT_LOG(logname)
#else
    #define BLAH_TRANSIENT_LOG(logname) BLAH_PERMANENT_LOG(logname)
    #define BLAH_TRANSIENT_LOG_WITH(logname, logger) BLAH_PERMANENT_LOG_WITH(logname, logger)
#endif

#endif // BLAH_HPP_0124_03122008
