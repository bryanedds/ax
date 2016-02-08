// Copyright 2011 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TEST_O_MATIC_HPP_2025_20082008
#define TEST_O_MATIC_HPP_2025_20082008

#if defined(_WIN32)
    #define TEST_O_MATIC_API extern "C" __declspec(dllexport)
#else
    #define TEST_O_MATIC_API extern "C" __attribute__((visibility("default")))
#endif

#define TEST_O_MATIC_VERSION 403
#define TEST_O_MATIC_VERSION_STRING "0.4.3"

// Use of unconditional branches is a common technique in C++ metaprogramming.
#if defined(_MSC_VER)
    #pragma warning(disable: 4127)
#endif

#include <cstddef>
#include <map>
#include <sstream>
#include <string>

namespace tom
{
    //! This type is used to store key/value pairs of data. It is used frequently, so a shorthand is useful.
    typedef std::map<std::string, std::string> strmap;

    //! A logger recieves events, triggered throughout the course of the tests.
    //! For example, the CHECK() macro supplied with the library triggers a "check" event and so on.
    class logger
    {
        public:
            virtual ~logger() { }
            virtual void record(const std::string &event, const strmap &data) = 0;
    };

    //! An implementation of the logger interface that records the number of successes and failures of individual checks, and
    //! the number of completed and aborted test cases.
    class simple_logger : public logger
    {
        public:
            //! If \a verbose is true, will produce output on successful checks as well as on failures.
            //! Any output produced will be written to \a out.
            simple_logger(std::ostream &out, bool verbose);

            void record(const std::string &event, const strmap &data);

            // Prints a summary of the tests recorded via the logger so far and returns the number of failed checks and aborted
            // test cases.
            int summary(std::ostream &out);


            std::size_t successes, failures;
            std::size_t complete, aborted;

        private:
            std::ostream &out;
            const bool verbose;
            std::string headline;
    };

    //! Specialise this template if your V isn't output streamable. The data class will make use of it to translate values to strings
    template<typename V>
    struct stringize
    {
        static std::string convert(const V &value)
        {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    };

    //! Use data("k1", v1)("k2", v2)("k3", v3)... as a shorthand to construct an object containing the given keys and values
    //! to be passed to logger::record_event(). The values can be of any output-stream-able type.
    struct data
    {
        template<typename V>
        data(const std::string &key, const V &value) { mp[key] = stringize<V>::convert(value); }
        data() { }

        operator const strmap &() const { return mp; }
        operator strmap &() { return mp; }

        template<typename V>
        data &operator() (const std::string &key, const V &value) { mp[key] = stringize<V>::convert(value); return *this; }

        strmap mp;
    };

    //! Holds all the data for a test. test objects are constructed at static initialisation. You shouldn't need to make any by
    //! explicitly calling the constructor (unless you write your own macro similar to TEST(), to define tests).
    struct test
    {
        test(const char *name, const char *file, std::size_t line, const char *date, const char *time, void (*function)(logger &));
        ~test();

        const char * const name;
        const char * const file;
        const std::size_t line;
        const char * const date;
        const char * const time;

        void (*const function)(logger &);
        test *next;
    };

    //! A runner provides a run() method to run a test. You can override it in a derived class.
    struct runner
    {
        virtual ~runner() { }
        virtual void run(const test &tst, logger &lgr);
    };

    namespace detail
    {
        template<typename T> struct to_const_ref { typedef const T &type; };
        template<typename T> struct to_const_ref<T &> { typedef const T &type; };

        class test_base_
        {
            logger &lgr_;

            protected:
                test_base_(logger &lgr) : lgr_(lgr) { }
                logger &get_local_tom_logger() { return lgr_; }
        };
    }

    // These functions can be dynamically loaded from shared libraries.
    // first_test() provides the first test in a linked list of tests.
    TEST_O_MATIC_API const test *first_test();
    TEST_O_MATIC_API void run_test(const test &tst, logger &lgr, runner &rnr);
    TEST_O_MATIC_API int run_tests();

    namespace
    {
        const char * const result_names[3] = { "fail", "ok", "aborted" };

    } // anonyous

} // close namespace tom

//! Provided so that CHECK/REQUIRE/etc may be called outside of TEST or TESTFIX
tom::simple_logger &get_local_tom_logger();


#if !defined(TEST_O_MATIC_USE_VARIADIC_MACROS)
    #if defined(_MSC_VER) && _MSC_VER >= 1400 || \
        defined(__BORLANDC__) && __BORLANDC__ >= 0x581 || \
        defined(__SUNPRO_CC) && __SUNPRO_CC > 0x530 || \
        defined(__DMC__) && __DMC__ >= 0x850
        #define TEST_O_MATIC_USE_VARIADIC_MACROS
    #endif
    #if defined(__GNUC__) && __GNUC__ >= 3
        #define TEST_O_MATIC_USE_VARIADIC_MACROS
    #endif
#endif

#if defined(TEST_O_MATIC_USE_VARIADIC_MACROS) && defined(__GNUC__) && __GNUC__ >= 3
    // This pragma quiets the pre-processor warning about variadic macros.
    // I wish there was a better way (GCC 4 has -Wno-variadic-macros, but GCC 3 doesn't).
    #pragma GCC system_header
#endif

#if defined(TEST_O_MATIC_DONT_USE_VARIADIC_MACROS) // override auto-detection
    #undef TEST_O_MATIC_USE_VARIADIC_MACROS
#endif

#define TEST_O_MATIC_TOKEN_JOIN_IMPL(a, b) a ## b
#define TEST_O_MATIC_TOKEN_JOIN(a, b) TEST_O_MATIC_TOKEN_JOIN_IMPL(a, b)

// --------------------------
// TEST and TESTFIX macros
// --------------------------
#define TEST_O_MATIC_STRUCT_NAME TEST_O_MATIC_TOKEN_JOIN(test_struct_on_line_, __LINE__)
#define TEST_O_MATIC_OBJ_NAME TEST_O_MATIC_TOKEN_JOIN(test_object_on_line_, __LINE__)

#define TEST_O_MATIC_TEST_DEFINITION_BEGIN \
    namespace { namespace test_o_matic_defs_ { \
        struct TEST_O_MATIC_STRUCT_NAME : ::tom::detail::test_base_

#define TEST_O_MATIC_TEST_DEFINITION_END(strliteral) \
        { \
            TEST_O_MATIC_STRUCT_NAME(::tom::logger &lgr) : ::tom::detail::test_base_(lgr) { } \
            void tom_run_(); \
            static void tom_func_(::tom::logger &lgr) { TEST_O_MATIC_STRUCT_NAME t(lgr); t.tom_run_(); } \
        }; \
        ::tom::test \
            TEST_O_MATIC_OBJ_NAME(strliteral, __FILE__, __LINE__, __DATE__, __TIME__, TEST_O_MATIC_STRUCT_NAME::tom_func_); \
    } } \
    void test_o_matic_defs_::TEST_O_MATIC_STRUCT_NAME::tom_run_()

#define TEST(strliteral) TEST_O_MATIC_TEST_DEFINITION_BEGIN TEST_O_MATIC_TEST_DEFINITION_END(strliteral)

#if defined(TEST_O_MATIC_USE_VARIADIC_MACROS)
    #define TESTFIX(strliteral, ...) TEST_O_MATIC_TEST_DEFINITION_BEGIN, __VA_ARGS__ TEST_O_MATIC_TEST_DEFINITION_END(strliteral)
#else
    #define TESTFIX(strliteral, fixclass) TEST_O_MATIC_TEST_DEFINITION_BEGIN, fixclass TEST_O_MATIC_TEST_DEFINITION_END(strliteral)
#endif

// Core data items for each check
#define TEST_O_MATIC_HEADER(expr_literal, result_index) \
    ::tom::data("line", __LINE__) \
                        ("file", __FILE__) \
                        ("statement", expr_literal) \
                        ("result", ::tom::result_names[(result_index)])

// --------------------------
// CHECK macro
// --------------------------
#define TEST_O_MATIC_CHECK_BEGIN \
    do \
    { \
        try \
        { \
            const int tom_ok_ = !!(

#define TEST_O_MATIC_CHECK_END(expr_literal) \
                                    ); \
            get_local_tom_logger().record("check", TEST_O_MATIC_HEADER(expr_literal, tom_ok_)); \
        } \
        catch (...) \
        { \
            get_local_tom_logger().record("check", TEST_O_MATIC_HEADER(expr_literal, 0)("threw", "an exception unexpectedly")); \
        } \
    } \
    while(false)

#if defined(TEST_O_MATIC_USE_VARIADIC_MACROS)
    #define CHECK(...) TEST_O_MATIC_CHECK_BEGIN __VA_ARGS__ TEST_O_MATIC_CHECK_END(#__VA_ARGS__)
#else
    #define CHECK(cond) TEST_O_MATIC_CHECK_BEGIN cond TEST_O_MATIC_CHECK_END(#cond)
#endif

// --------------------------
// REQUIRE macro
// --------------------------
#define TEST_O_MATIC_REQUIRE_BEGIN \
    do \
    { \
        int tom_ok_ = 0; \
        try \
        { \
            tom_ok_ = !!(

#define TEST_O_MATIC_REQUIRE_END(expr_literal) \
                            ); \
            get_local_tom_logger().record("check", TEST_O_MATIC_HEADER(expr_literal, tom_ok_)); \
        } \
        catch (...) \
        { \
            get_local_tom_logger().record("check", TEST_O_MATIC_HEADER(expr_literal, false)("threw", "an exception unexpectedly")); \
        } \
        if (!tom_ok_) \
            throw TEST_O_MATIC_HEADER(expr_literal, 2)("reason", "REQUIRE()ment '" expr_literal "' not met"); \
    } \
    while (false)

#if defined(TEST_O_MATIC_USE_VARIADIC_MACROS)
    #define REQUIRE(...) TEST_O_MATIC_REQUIRE_BEGIN __VA_ARGS__ TEST_O_MATIC_REQUIRE_END(#__VA_ARGS__)
#else
    #define REQUIRE(cond) TEST_O_MATIC_REQUIRE_BEGIN cond TEST_O_MATIC_REQUIRE_END(#cond)
#endif

// --------------------------
// TRY macro
// --------------------------
#define TEST_O_MATIC_TRY_BEGIN \
    do \
    { \
        try \
        {

#define TEST_O_MATIC_TRY_END(expr_literal) \
            ; \
            get_local_tom_logger().record("try", TEST_O_MATIC_HEADER(expr_literal, 1)("threw", "nothing")); \
        } \
        catch (...) \
        { \
            get_local_tom_logger().record("try", TEST_O_MATIC_HEADER(expr_literal, 0)("threw", "an exception")); \
        } \
    } \
    while (false)

#if defined(TEST_O_MATIC_USE_VARIADIC_MACROS)
    #define TRY(...) TEST_O_MATIC_TRY_BEGIN __VA_ARGS__ TEST_O_MATIC_TRY_END(#__VA_ARGS__)
#else
    #define TRY(statement) TEST_O_MATIC_TRY_BEGIN statement TEST_O_MATIC_TRY_END(#statement)
#endif

// --------------------------
// THROWS macro
// --------------------------
#define TEST_O_MATIC_THROWS_BEGIN \
    do \
    { \
        try \
        { \

#define TEST_O_MATIC_THROWS_MIDDLE(expr_literal) \
            ; \
            get_local_tom_logger().record("throws", TEST_O_MATIC_HEADER(expr_literal, 0)("threw", "nothing")); \
        } \
        catch (::tom::detail::to_const_ref<

#define TEST_O_MATIC_THROWS_END(expr_literal, type_literal) \
                                                        >::type) \
        { \
            get_local_tom_logger().record("throws", TEST_O_MATIC_HEADER(expr_literal, 1)("threw", "an exception of type '" type_literal "'")); \
        } \
        catch (...) \
        { \
            get_local_tom_logger().record("throws", TEST_O_MATIC_HEADER(expr_literal, 0)("threw", "an exception of type other than '" type_literal "'")); \
        } \
    } \
    while (false)

#if defined(TEST_O_MATIC_USE_VARIADIC_MACROS)
    #define THROWS(expression, ...) \
        TEST_O_MATIC_THROWS_BEGIN expression TEST_O_MATIC_THROWS_MIDDLE(#expression) __VA_ARGS__ TEST_O_MATIC_THROWS_END(#expression, #__VA_ARGS__)
#else
    #define THROWS(expression, extype) \
        TEST_O_MATIC_THROWS_BEGIN expression TEST_O_MATIC_THROWS_MIDDLE(#expression) extype TEST_O_MATIC_THROWS_END(#expression, #extype)
#endif

// --------------------------
// ABORT() macro
// --------------------------
#define ABORT() \
    throw ::tom::data("line", __LINE__)("file", __FILE__)("reason", "test ABORT()ed")("result", "aborted");

#endif // TEST_O_MATIC_HPP_2025_20082008
