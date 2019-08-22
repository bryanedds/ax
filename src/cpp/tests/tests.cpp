#include <istream>

#include "../../hpp/tom/tom.hpp"

#include "../../hpp/ax/impl/ax.hpp"

#include "../../hpp/ax/impl/system.hpp"

namespace ax
{
    // A trivial type to demonstrate the castable type.
    class castable_a : public castable
    {
    protected:
        ENABLE_CAST(castable_a, castable);
    public:
        int i = 0;
    };

    // A trivial type to demonstrate the castable type.
    class castable_b : public castable_a
    {
    protected:
        ENABLE_CAST(castable_b, castable_a);
    public:
        int j = 0;
    };

    // A trivial program type to demonstrate the eventable program mixin.
    class eventable_test : public eventable<eventable_test>
    {
    protected:
        ENABLE_CAST(eventable_test, eventable<eventable_test>);
    };

    class reflectable_test : public reflectable
    {
    protected:

        ENABLE_REFLECTION_CUSTOM(reflectable_test, reflectable);

        const std::shared_ptr<type_t> type = register_sub_type<reflectable_test>(typeid(reflectable),
        {
            { "bool_value",             register_field<bool>                        (offsetof(reflectable_test, bool_value)) },
            { "int_value",              register_field<int>                         (offsetof(reflectable_test, int_value)) },
            { "float_value",            register_field<float>                       (offsetof(reflectable_test, float_value)) },
            { "name_value",             register_field<name_t>                      (offsetof(reflectable_test, name_value)) },
            { "address_value",          register_field<address>                     (offsetof(reflectable_test, address_value)) },
            { "vector_int_value",       register_field<std::vector<int>>            (offsetof(reflectable_test, vector_int_value)) },
            { "vector_string_value",    register_field<std::vector<std::string>>    (offsetof(reflectable_test, vector_string_value)) },
            { "shared_int_value",       register_field<std::shared_ptr<int>>        (offsetof(reflectable_test, shared_int_value)) },
            { "pair_value",             register_field<pair<int, int>>              (offsetof(reflectable_test, pair_value)) },
            { "record_value",           register_field<record<int, int, int>>       (offsetof(reflectable_test, record_value)) },
            { "option_some_value",      register_field<option<int>>                 (offsetof(reflectable_test, option_some_value)) },
            { "option_none_value",      register_field<option<int>>                 (offsetof(reflectable_test, option_none_value)) },
            { "either_right_value",     register_field<either<int, std::string>>    (offsetof(reflectable_test, either_right_value)) },
            { "either_left_value",      register_field<either<int, std::string>>    (offsetof(reflectable_test, either_left_value)) },
            { "choice_value",           register_field<choice<int, int, int>>       (offsetof(reflectable_test, choice_value)) }
        });

        std::shared_ptr<type_t> get_type() const override
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
        std::shared_ptr<int> shared_int_value;
        pair<int, int> pair_value;
        record<int, int, int> record_value;
        option<int> option_some_value;
        option<int> option_none_value;
        either<int, std::string> either_right_value;
        either<int, std::string> either_left_value;
        choice<int, int, int> choice_value;

        reflectable_test() :
            bool_value(),
            int_value(),
            float_value(),
            name_value(),
            address_value(),
            vector_int_value(),
            vector_string_value(),
            shared_int_value(std::make_shared<int>()),
            pair_value(),
            record_value(),
            option_some_value(),
            option_none_value(),
            either_right_value(),
            either_left_value(),
            choice_value()
        { }

        reflectable_test(
            bool bool_value,
            int int_value,
            float float_value,
            name_t name_value,
            address address_value,
            const std::vector<int>& vector_int_value,
            const std::vector<std::string>& vector_string_value,
            int shared_int_value,
            pair<int, int> pair_value,
            record<int, int, int> record_value,
            option<int> option_some_value,
            option<int> option_none_value,
            either<int, std::string> either_right_value,
            either<int, std::string> either_left_value,
            choice<int, int, int> choice_value) :
            bool_value(bool_value),
            int_value(int_value),
            float_value(float_value),
            name_value(name_value),
            address_value(address_value),
            vector_int_value(vector_int_value),
            vector_string_value(vector_string_value),
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
        castable_b b{};
        b.i = 10;
        b.j = 20;
        const castable_a& a = cast<castable_a>(b);
        CHECK(a.i == 10);
        const castable& c = cast<castable>(b);
        const castable_a& a2 = cast<castable_a>(c);
        CHECK(a2.i == 10);
        const castable_b& b2 = cast<castable_b>(c);
        CHECK(b2.i == 10);
        CHECK(b2.j == 20);
    }

    TEST("shared castable works")
    {
        std::shared_ptr<castable_b> b = std::make_shared<castable_b>();
        b->i = 10;
        b->j = 20;
        std::shared_ptr<castable_a> a = cast<castable_a>(b);
        CHECK(a->i == 10);
        std::shared_ptr<castable> c = cast<castable>(b);
        std::shared_ptr<castable_a> a2 = cast<castable_a>(c);
        CHECK(a2->i == 10);
        std::shared_ptr<castable_b> b2 = cast<castable_b>(c);
        CHECK(b2->i == 10);
        CHECK(b2->j == 20);
    }

    TEST("unique castable works")
    {
        std::unique_ptr<castable_b> b = std::make_unique<castable_b>();
        b->i = 10;
        b->j = 20;
        std::unique_ptr<castable_a> a = cast<castable_a>(std::move(b));
        CHECK(a->i == 10);
        std::unique_ptr<castable> c = cast<castable>(std::move(a));
        std::unique_ptr<castable_a> a2 = cast<castable_a>(std::move(c));
        CHECK(a2->i == 10);
        std::unique_ptr<castable_b> b2 = cast<castable_b>(std::move(a2));
        CHECK(b2->i == 10);
        CHECK(b2->j == 20);
    }

    TEST("events work")
    {
        VAR i = 0;
        ax::eventable_test program{};
        VAL& event_address = address("event");
		VAL participant = std::make_shared<ax::addressable>(address("participant"));
        VAR handler = [&](VAL&, VAL&) { return ++i, true; };
        VAR unsubscriber = program.subscribe_event<std::string>(event_address, participant, handler);
        program.publish_event("Event handled!"_s, event_address, participant);
        unsubscriber(program);
        program.publish_event("Event unhandled."_s, event_address, participant);
        CHECK(i == 1);
    }

    TEST("read and write value works")
    {
        symbol symbol{};
        reflectable_test source(
            true, 5, 10.0f, "jim bob", address("s/compton/la"),
            { 1, 3, 5 }, { "a", "bb", "ccc" }, 777,
            make_pair(50, 100), make_record(150, 200, 250),
            some(2), none<int>(), right<int, std::string>(4), left<int>("msg"_s), third<int, int, int>(3));
        reflectable_test target{};
        write_value(source, symbol);
        read_value(symbol, target);
        CHECK(target.bool_value);
        CHECK(target.int_value == 5);
        CHECK(target.float_value == 10.0f);
        CHECK(target.name_value == "jim bob");
        CHECK(target.address_value == address("s/compton/la"));
        CHECK(target.vector_int_value == std::vector<int>({ 1, 3, 5 }));
        CHECK(target.vector_string_value == std::vector<std::string>({ "a", "bb", "ccc" }));
        CHECK(*target.shared_int_value == 777);
        CHECK(target.pair_value.fst() == 50);
        CHECK(target.pair_value.snd() == 100);
        CHECK(target.record_value.fst() == 150);
        CHECK(target.record_value.snd() == 200);
        CHECK(target.record_value.thd() == 250);
        CHECK(*target.option_some_value == 2);
        CHECK(target.option_none_value.is_none());
        CHECK(*target.either_right_value == 4);
        CHECK(~target.either_left_value == "msg");
        CHECK(target.choice_value.get_third() == 3);
    }

    TEST("parser works")
    {
        VAL& str =
            "[true 5 10.0 \
              \"jim bob\" \
              \"s/compton/la\" \
              [1 3 5] \
              [\"a\" \"bb\" \"ccc\"] \
              777 \
              [50 100] \
              [150 200 250] \
              [some 2] \
              none \
              [right 4] \
              [left \"msg\"] \
              [  third  3  ]  ]"; // a little extra whitespace to try to throw off the parser
        VAL& parse = parse_symbol(str);
        VAL& symbol = get_parse_success(parse);
        reflectable_test target{};
        read_value(symbol, target);
        CHECK(target.bool_value);
        CHECK(target.int_value == 5);
        CHECK(target.float_value == 10.0f);
        CHECK(target.name_value == "jim bob");
        CHECK(target.address_value == address("s/compton/la"));
        CHECK(target.vector_int_value == std::vector<int>({ 1, 3, 5 }));
        CHECK(target.vector_string_value == std::vector<std::string>({ "a", "bb", "ccc" }));
        CHECK(*target.shared_int_value == 777);
        CHECK(target.pair_value.fst() == 50);
        CHECK(target.pair_value.snd() == 100);
        CHECK(target.record_value.fst() == 150);
        CHECK(target.record_value.snd() == 200);
        CHECK(target.record_value.thd() == 250);
        CHECK(*target.option_some_value == 2);
        CHECK(target.option_none_value.is_none());
        CHECK(*target.either_right_value == 4);
        CHECK(~target.either_left_value == "msg");
        CHECK(target.choice_value.get_third() == 3);
    }

    TEST("properties work")
    {
        propertied p{};
        attach(p, "x", 0);
        CHECK(get<int>(p, "x") == 0);
        set(p, "x", 5);
        CHECK(get<int>(p, "x") == 5);
    }

    TEST("logging works")
    {
        // TODO: implement
    }
}

int main(int, char*[])
{
    ax::register_common_type_descriptors();
    return tom::run_tests();
}
