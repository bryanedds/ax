#include <istream>

#include "../../hpp/tom/tom.hpp"

#include "../../hpp/ax/ax.hpp"

namespace ax
{
    // A trivial program type to demonstrate the eventable program mixin.
    class eventable_test : public eventable<eventable_test>
    {
    protected:
        enable_cast(eventable_test, eventable<eventable_test>);
    };

    class inspectable_test : public inspectable
    {
    protected:

        enable_cast(inspectable_test, inspectable);

        const std::shared_ptr<type_t> type = register_sub_type<inspectable_test>(typeid(inspectable),
        {
            { "bool_value",             register_field<bool>                        (offsetof(inspectable_test, bool_value)) },
            { "int_value",              register_field<int>                         (offsetof(inspectable_test, int_value)) },
            { "float_value",            register_field<float>                       (offsetof(inspectable_test, float_value)) },
            { "name_value",             register_field<name_t>                      (offsetof(inspectable_test, name_value)) },
            { "address_value",          register_field<address>                     (offsetof(inspectable_test, address_value)) },
            { "vector_int_value",       register_field<std::vector<int>>            (offsetof(inspectable_test, vector_int_value)) },
            { "vector_string_value",    register_field<std::vector<std::string>>    (offsetof(inspectable_test, vector_string_value)) },
            { "unique_int_value",       register_field<std::unique_ptr<int>>        (offsetof(inspectable_test, unique_int_value)) },
            { "shared_int_value",       register_field<std::shared_ptr<int>>        (offsetof(inspectable_test, shared_int_value)) },
            { "pair_value",             register_field<pair<int, int>>              (offsetof(inspectable_test, pair_value)) },
            { "record_value",           register_field<record<int, int, int>>       (offsetof(inspectable_test, record_value)) },
            { "option_some_value",      register_field<option<int>>                 (offsetof(inspectable_test, option_some_value)) },
            { "option_none_value",      register_field<option<int>>                 (offsetof(inspectable_test, option_none_value)) },
            { "either_right_value",     register_field<either<std::string, int>>    (offsetof(inspectable_test, either_right_value)) },
            { "either_left_value",      register_field<either<std::string, int>>    (offsetof(inspectable_test, either_left_value)) },
            { "choice_value",           register_field<choice<int, int, int>>       (offsetof(inspectable_test, choice_value)) }
        });

        std::shared_ptr<type_t> get_type_impl() const override
        {
            return type;
        };

    public:

        int bool_value;
        int int_value;
        float float_value;
        name_t name_value;
        address address_value;
        std::vector<int> vector_int_value;
        std::vector<std::string> vector_string_value;
        std::unique_ptr<int> unique_int_value;
        std::shared_ptr<int> shared_int_value;
        pair<int, int> pair_value;
        record<int, int, int> record_value;
        option<int> option_some_value;
        option<int> option_none_value;
        either<std::string, int> either_right_value;
        either<std::string, int> either_left_value;
        choice<int, int, int> choice_value;

        inspectable_test() :
            bool_value(),
            int_value(),
            float_value(),
            name_value(),
            address_value(),
            vector_int_value(),
            vector_string_value(),
            unique_int_value(std::make_unique<int>()),
            shared_int_value(std::make_shared<int>()),
            pair_value(),
            record_value(),
            option_some_value(),
            option_none_value(),
            either_right_value(),
            either_left_value(),
            choice_value()
        { }

        inspectable_test(
            bool bool_value,
            int int_value,
            float float_value,
            name_t name_value,
            address address_value,
            const std::vector<int>& vector_int_value,
            const std::vector<std::string>& vector_string_value,
            int unique_int_value,
            int shared_int_value,
            pair<int, int> pair_value,
            record<int, int, int> record_value,
            option<int> option_some_value,
            option<int> option_none_value,
            either<std::string, int> either_right_value,
            either<std::string, int> either_left_value,
            choice<int, int, int> choice_value) :
            bool_value(bool_value),
            int_value(int_value),
            float_value(float_value),
            name_value(name_value),
            address_value(address_value),
            vector_int_value(vector_int_value),
            vector_string_value(vector_string_value),
            unique_int_value(std::make_unique<int>(unique_int_value)),
            shared_int_value(std::make_shared<int>(shared_int_value)),
            pair_value(pair_value),
            record_value(record_value),
            option_some_value(option_some_value),
            option_none_value(option_none_value),
            either_right_value(either_right_value),
            either_left_value(either_left_value),
            choice_value(choice_value)
        { }
    };

    TEST("hash works")
    {
        CHECK(ax::get_hash(0) == std::hash<int>()(0));
    }

    TEST("castable works")
    {
        // TODO: implement
    }

    TEST("events work")
    {
        var i = 0;
        ax::eventable_test program{};
        val& event_address = address("event");
        val& participant = std::make_shared<ax::addressable>("participant");
        var handler = [&](val&, val&) { return ++i, true; };
        var unsubscriber = ax::subscribe_event<std::string>(program, event_address, participant, handler);
        ax::publish_event(program, "Event handled!"_s, event_address, participant);
        unsubscriber(program);
        ax::publish_event(program, "Event unhandled."_s, event_address, participant);
        CHECK(i == 1);
    }

    TEST("read and write value works")
    {
        symbol symbol{};
        inspectable_test source(
            true, 5, 10.0f, "jim bob", address("s/compton/la"),
            { 1, 3, 5 }, { "a", "bb", "ccc" }, 666, 777,
            make_pair(50, 100), make_record(150, 200, 250),
            some(2), none<int>(), right<std::string>(4), left<std::string, int>("msg"), third<int, int>(3));
        inspectable_test target{};
        write_value(source, symbol);
        read_value(symbol, target);
        CHECK(target.bool_value);
        CHECK(target.int_value == 5);
        CHECK(target.float_value == 10.0f);
        CHECK(target.name_value == "jim bob");
        CHECK(target.address_value == address("s/compton/la"));
        CHECK(target.vector_int_value == std::vector<int>({ 1, 3, 5 }));
        CHECK(target.vector_string_value == std::vector<std::string>({ "a", "bb", "ccc" }));
        CHECK(*target.unique_int_value == 666);
        CHECK(*target.shared_int_value == 777);
        CHECK(fst(target.pair_value) == 50);
        CHECK(snd(target.pair_value) == 100);
        CHECK(fst(target.record_value) == 150);
        CHECK(snd(target.record_value) == 200);
        CHECK(thd(target.record_value) == 250);
        CHECK(*target.option_some_value == 2);
        CHECK(is_none(target.option_none_value));
        CHECK(*target.either_right_value == 4);
        CHECK(~target.either_left_value == "msg");
        CHECK(get_third(target.choice_value) == 3);
    }

    TEST("parser works")
    {
        val& str =
            "[true 5 10.0 \
              \"jim bob\" \
              \"s/compton/la\" \
              [1 3 5] \
              [\"a\" \"bb\" \"ccc\"] \
              666 \
              777 \
              [50 100] \
              [150 200 250] \
              [some 2] \
              none \
              [right 4] \
              [left \"msg\"] \
              [  third  3  ]  ]"; // a little extra whitespace to try to throw off parser
        std::stringstream sstr(str);
        sstr << std::noskipws; // apparently avoids skipping whitespace
        std::istream_iterator<char> iter(sstr);
        val& parse = parse_symbol_from_stream(iter, std::istream_iterator<char>());
        val& symbol = get_parse_success(parse);
        inspectable_test target{};
        read_value(symbol, target);
        CHECK(target.bool_value);
        CHECK(target.int_value == 5);
        CHECK(target.float_value == 10.0f);
        CHECK(target.name_value == "jim bob");
        CHECK(target.address_value == address("s/compton/la"));
        CHECK(target.vector_int_value == std::vector<int>({ 1, 3, 5 }));
        CHECK(target.vector_string_value == std::vector<std::string>({ "a", "bb", "ccc" }));
        CHECK(*target.unique_int_value == 666);
        CHECK(*target.shared_int_value == 777);
        CHECK(fst(target.pair_value) == 50);
        CHECK(snd(target.pair_value) == 100);
        CHECK(fst(target.record_value) == 150);
        CHECK(snd(target.record_value) == 200);
        CHECK(thd(target.record_value) == 250);
        CHECK(*target.option_some_value == 2);
        CHECK(is_none(target.option_none_value));
        CHECK(*target.either_right_value == 4);
        CHECK(~target.either_left_value == "msg");
        CHECK(get_third(target.choice_value) == 3);
    }

    TEST("properties work")
    {
        // TODO: implement
    }
}
