#include "../hpp/tom/tom.hpp"

#include "../hpp/ax/ax.hpp"

int main(int, char*[])
{
    ax::register_common_type_descriptors();
    return tom::run_tests();
}
