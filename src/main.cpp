#include <vector>
#include <vector>
#include <iostream>
#include <algorithm>

int main() {
    std::vector<int> v{3, 1, 2};
    std::sort(v.begin(), v.end());
    for (int i : v) std::cout << i << " ";
}

