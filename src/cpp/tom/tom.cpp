// Copyright 2008 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "../../hpp/tom/tom.hpp"

#include <iostream>
#include <iomanip> // endl
#include <algorithm>
#include <cctype>

#ifdef __BORLANDC__
#pragma warn -8091 // "template argument InputIterator passed to 'find_if' is a output iterator: input iterator required"
#pragma warn -8013 // "Possible use of 'ns' before definition in function trim(const string &)"
#endif

namespace
{
    using namespace std;
    using namespace tom;

    struct not_space { bool operator() (char ch) const { return !::isspace(ch); } };

    string trim(const string &s)
    {
        not_space ns;
        string::const_iterator open = find_if(s.begin(), s.end(), ns);
        string::const_iterator close = find_if(s.rbegin(), s.rend(), ns).base();

        return (open < close) ? string(open, close) : s;
    }

    std::ostream &result_colon(std::ostream &out, const std::string &okfail)
    {
        return out << okfail << ':' << setw(5 - okfail.size()) << ' ';
    }

} // close anonymous namespace

namespace tom
{
    simple_logger::simple_logger(std::ostream &out, bool verbose) :
        successes(0),
        failures(0),
        complete(0),
        aborted(0),
        out(out),
        verbose(verbose)
    {
    }

    void simple_logger::record(const std::string &event, const strmap &dat)
    {
        strmap mp(dat);
        const string res(mp["result"]);
        const bool ok = (event == "pre_test") || (res == "ok");

        if (!ok) 
        {
            if (!headline.empty()) out << headline;
            headline.clear();
        }

        if (event == "check")
        {
            if (verbose || !ok)
            {
                result_colon(out, res) << "'" << trim(mp["statement"]) << "'";
                if (!mp["threw"].empty()) out << " threw " << mp["threw"];
                out << ", line " << mp["line"] << endl;
            }
            ++(ok ? successes : failures);
        }
        else if (event == "throws" || event == "try")
        {
            if (verbose || !ok)
                result_colon(out, res) << "'" << trim(mp["statement"]) << "' threw " << mp["threw"] <<
                                          (ok ? " as expected" : " unexpectedly") << ", line " << mp["line"] << endl;
            ++(ok ? successes : failures);
        }
        else if (event == "pre_test")
        {
            std::ostringstream oss;
            (verbose ? out : static_cast<ostream &>(oss)) << '\n' << mp["name"] << ", " << mp["file"] << ':' << mp["line"]  << endl;
            if (!verbose) headline = oss.str();
        }
        else if (event == "post_test")
        {
            if (!ok) out << "8< ---- test cut short: " << mp["reason"] << " ----\n";
            ++(ok ? complete : aborted);
            headline.clear();
        }
    }

    int simple_logger::summary(std::ostream &out_stream)
    {
        out_stream << "\nsuccesses: " << successes << '\n';
        out_stream << "failures:  " << failures << '\n';

        if (aborted) out_stream << "aborted:   " << aborted << '\n';
        out_stream << std::endl;

        return static_cast<int>(failures + aborted);
    }

    void runner::run(const test &t, logger &lgr)
    {
        try
        {
            lgr.record("pre_test", data("name", t.name)("line", t.line)("file", t.file)("time", t.time)("date", t.date));
            t.function(lgr);
            lgr.record("post_test", data("result", "ok"));
        }
        catch (const data &dat)
        {
            lgr.record("post_test", dat);
        }
        catch (...)
        {
            lgr.record("post_test", data("result", "aborted")("reason", "an uncaught exception ended the test"));
        }
    }

    namespace
    {
        test *&root()
        {
            static test *root = 0;
            return root;
        }
    }

    test::test(const char *name, const char *file, std::size_t line, const char *date, const char *time, void (*function)(logger &)) :
        name(name),
        file(file),
        line(line),
        date(date),
        time(time),
        function(function),
        next(0)
    {
        if (!root()) root() = this;
        else
        {
            test *t = root();
            while (t->next) t = t->next;
            t->next = this;
        }
    }

    test::~test()
    {
        if (root() == this) root() = next;
        else
        {
            test *t = root();
            while (t->next != this) t = t->next;
            t->next = next;
        }
    }

    const test *first_test() { return root(); }

    void run_test(const test &tst, logger &lgr, runner &rnr) { rnr.run(tst, lgr); }

    int run_tests()
    {
        runner rnr;
        simple_logger lgr(std::cout, true);
        for (const test *t = first_test(); t; t = t->next) run_test(*t, lgr, rnr);
        return lgr.summary(std::cout);
    }

} // close namespace tom

tom::simple_logger &get_local_tom_logger()
{
    static tom::simple_logger lgr(std::cout, false);
    return lgr;
}
