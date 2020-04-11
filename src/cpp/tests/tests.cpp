#include <iostream>

#include "tom/tom.hpp"

#include "ax/ax.hpp"

namespace ax
{
    // A trivial type to demonstrate the castable type.
    class castable_a : public ax::castable
    {
    public: int i = 0;
    protected: ENABLE_CAST(ax::castable_a, ax::castable);
    };

    // A trivial type to demonstrate the castable type.
    class castable_b : public ax::castable_a
    {
    public: int j = 0;
    protected: ENABLE_CAST(ax::castable_b, ax::castable_a);
    };

    // A trivial program type to demonstrate the eventable program mixin.
    class eventable_test final : public ax::eventable<ax::eventable_test>
    {
    protected: ENABLE_CAST(ax::eventable_test, ax::eventable<ax::eventable_test>);
    };

    class reflectable_test final : public ax::reflectable
    {
    protected:

        ENABLE_REFLECTION_CUSTOM(ax::reflectable_test, ax::reflectable);

        const std::shared_ptr<ax::type> type = register_sub_type<ax::reflectable_test>(typeid(ax::reflectable),
        {
            { "bool_value",             register_field<bool>                            (offsetof(ax::reflectable_test, bool_value)) },
            { "int_value",              register_field<int>                             (offsetof(ax::reflectable_test, int_value)) },
            { "float_value",            register_field<float>                           (offsetof(ax::reflectable_test, float_value)) },
            { "name_value",             register_field<ax::name>                        (offsetof(ax::reflectable_test, name_value)) },
            { "address_value",          register_field<ax::address>                     (offsetof(ax::reflectable_test, address_value)) },
            { "vector_int_value",       register_field<std::vector<int>>                (offsetof(ax::reflectable_test, vector_int_value)) },
            { "vector_string_value",    register_field<std::vector<std::string>>        (offsetof(ax::reflectable_test, vector_string_value)) },
            { "shared_int_value",       register_field<std::shared_ptr<int>>            (offsetof(ax::reflectable_test, shared_int_value)) },
            { "pair_value",             register_field<ax::pair<int, int>>              (offsetof(ax::reflectable_test, pair_value)) },
            { "record_value",           register_field<ax::record<int, int, int>>       (offsetof(ax::reflectable_test, record_value)) },
            { "option_some_value",      register_field<ax::option<int>>                 (offsetof(ax::reflectable_test, option_some_value)) },
            { "option_none_value",      register_field<ax::option<int>>                 (offsetof(ax::reflectable_test, option_none_value)) },
            { "either_right_value",     register_field<ax::either<int, std::string>>    (offsetof(ax::reflectable_test, either_right_value)) },
            { "either_left_value",      register_field<ax::either<int, std::string>>    (offsetof(ax::reflectable_test, either_left_value)) },
            { "choice_value",           register_field<ax::choice<int, int, int>>       (offsetof(ax::reflectable_test, choice_value)) }
        });

        std::shared_ptr<ax::type> get_type() const override
        {
            return type;
        };

    public:

        int bool_value;
        int int_value;
        float float_value;
        ax::name name_value;
        ax::address address_value;
        std::vector<int> vector_int_value;
        std::vector<std::string> vector_string_value;
        std::shared_ptr<int> shared_int_value;
        ax::pair<int, int> pair_value;
        ax::record<int, int, int> record_value;
        ax::option<int> option_some_value;
        ax::option<int> option_none_value;
        ax::either<int, std::string> either_right_value;
        ax::either<int, std::string> either_left_value;
        ax::choice<int, int, int> choice_value;

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
            ax::name name_value,
            ax::address address_value,
            const std::vector<int>& vector_int_value,
            const std::vector<std::string>& vector_string_value,
            int shared_int_value,
            ax::pair<int, int> pair_value,
            ax::record<int, int, int> record_value,
            ax::option<int> option_some_value,
            ax::option<int> option_none_value,
            ax::either<int, std::string> either_right_value,
            ax::either<int, std::string> either_left_value,
            ax::choice<int, int, int> choice_value) :
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
        ax::castable_b b;
        b.i = 10;
        b.j = 20;
        const ax::castable_a& a = ax::cast<ax::castable_a>(b);
        CHECK(a.i == 10);
        const ax::castable& c = ax::cast<ax::castable>(b);
        const ax::castable_a& a2 = ax::cast<ax::castable_a>(c);
        CHECK(a2.i == 10);
        const ax::castable_b& b2 = ax::cast<ax::castable_b>(c);
        CHECK(b2.i == 10);
        CHECK(b2.j == 20);
    }

    TEST("shared castable works")
    {
        std::shared_ptr<ax::castable_b> b = std::make_shared<ax::castable_b>();
        b->i = 10;
        b->j = 20;
        std::shared_ptr<ax::castable_a> a = ax::cast<ax::castable_a>(b);
        CHECK(a->i == 10);
        std::shared_ptr<ax::castable> c = ax::cast<ax::castable>(b);
        std::shared_ptr<ax::castable_a> a2 = ax::cast<ax::castable_a>(c);
        CHECK(a2->i == 10);
        std::shared_ptr<ax::castable_b> b2 = cast<ax::castable_b>(c);
        CHECK(b2->i == 10);
        CHECK(b2->j == 20);
    }

    TEST("unique castable works")
    {
        std::unique_ptr<ax::castable_b> b = std::make_unique<ax::castable_b>();
        b->i = 10;
        b->j = 20;
        std::unique_ptr<ax::castable_a> a = ax::cast<ax::castable_a>(std::move(b));
        CHECK(a->i == 10);
        std::unique_ptr<ax::castable> c = ax::cast<ax::castable>(std::move(a));
        std::unique_ptr<ax::castable_a> a2 = ax::cast<ax::castable_a>(std::move(c));
        CHECK(a2->i == 10);
        std::unique_ptr<ax::castable_b> b2 = ax::cast<ax::castable_b>(std::move(a2));
        CHECK(b2->i == 10);
        CHECK(b2->j == 20);
    }

    TEST("events work")
    {
        VAR i = 0;
        ax::eventable_test program;
        VAL& event_address = ax::address("event");
        VAL addressable = std::make_shared<ax::addressable>(ax::address("addressable"));
        VAR handler = [&](VAL&, VAL&) { return ++i, true; };
        VAR unsubscriber = program.subscribe_event<std::string>(event_address, addressable, handler);
        program.publish_event("Event handled!"_s, event_address, addressable);
        unsubscriber(program);
        program.publish_event("Event unhandled."_s, event_address, addressable);
        CHECK(i == 1);
    }

    TEST("read and write value works")
    {
        ax::symbol symbol;
        ax::reflectable_test source(
            true, 5, 10.0f, "jim bob", ax::address("s/compton/la"),
            { 1, 3, 5 }, { "a", "bb", "ccc" }, 777,
            make_pair(50, 100), make_record(150, 200, 250),
            some(2), none<int>(), right<int, std::string>(4), left<int>("msg"_s), third<int, int, int>(3));
        ax::reflectable_test target;
        ax::write_value(source, symbol);
        ax::read_value(symbol, target);
        CHECK(target.bool_value);
        CHECK(target.int_value == 5);
        CHECK(target.float_value == 10.0f);
        CHECK(target.name_value == "jim bob");
        CHECK(target.address_value == ax::address("s/compton/la"));
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
        VAL& symbol = parse.get_success();
        ax::reflectable_test target;
        ax::read_value(symbol, target);
        CHECK(target.bool_value);
        CHECK(target.int_value == 5);
        CHECK(target.float_value == 10.0f);
        CHECK(target.name_value == "jim bob");
        CHECK(target.address_value == ax::address("s/compton/la"));
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
        ax::propertied p;
        p.attach("x", 0);
        CHECK(p.get<int>("x") == 0);
        p.set("x", 5);
        CHECK(p.get<int>("x") == 5);
    }

    TEST("misc")
    {
        std::cout << std::to_string(sizeof(std::string)) << std::endl;
        std::cout << std::to_string(sizeof(std::vector<std::size_t>)) << std::endl;
        std::cout << std::to_string(sizeof(ax::name)) << std::endl;
        std::cout << std::to_string(sizeof(ax::address)) << std::endl;
        std::cout << std::to_string(sizeof(ax::entity)) << std::endl;
    }

    TEST("main")
    {
        // open model
        VAL model_file_path = "../../data/model.obj";
        ax::basic_model model;
        model.try_read_from_obj(model_file_path);

        // create render target
        VAL width = 800;
        VAL height = 800;
        VAL image_file_path = "../../data/image.tga";
        ax::basic_buffer render_target(width, height);
        render_target.fill(ax::basic_pixel(std::numeric_limits<float>::lowest(), ax::zero<ax::v3>(), { 0, 0, 0, 255 }));

        // render model to target
        VAL& light = ax::v3(0.0f, 0.0f, 1.0f);
        ax::draw_textured_ortho(light, model, render_target);

        // write render target as flipped to file
        render_target.try_write_to_tga(image_file_path);
    }
}

int main(int, char*[])
{
    ax::register_common_type_descriptors();
    return tom::run_tests();
}
