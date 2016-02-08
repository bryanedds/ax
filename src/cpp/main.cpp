#include "../impl/tom/tom.hpp"

#include "../impl/ax/ax.hpp"

int main(int, char*[])
{
    ax::register_common_type_descriptors();
    return tom::run_tests();
}
