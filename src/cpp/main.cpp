#include "../dpp/tom/tom.hpp"

#include "../dpp/ax/ax.hpp"

int main(int, char*[])
{
    ax::register_common_type_descriptors();
    return tom::run_tests();
}
