#include <iostream>
#include <vector>
#include "../str.hpp"

int main() {
    str a = str('a');
    str b = str('b');
    std::vector<str> v{a, b};
    str sep = str(',');
    str j = sep.join(v);
    std::cout << j.len() << "\n"; // simple use
    return 0;
}
