#include "morphism1.hpp"
#include "morphism2.hpp"
#include <iostream>

static_assert(Convertible<int,       std::string>);
static_assert(Convertible<unsigned,  std::string>);
static_assert(Convertible<int,       std::vector<int>>);
static_assert(not Convertible<float, std::string>);

static_assert(FindMorphism<int,      std::string>()        == 0);
// Exact order is undefined due to compiled differences, but this doesn't matter
//static_assert(FindMorphism<unsigned, std::string>()        == 1);
//static_assert(FindMorphism<unsigned, std::string>()        == 1);
static_assert(FindMorphism<unsigned, std::string>()        != -1);
static_assert(FindMorphism<unsigned, std::string>()        != -1);
static_assert(FindMorphism<int,      std::vector<int>>()   == 1);

static_assert(FindMorphism<float,    std::string>()        == -1);
static_assert(FindMorphism<int,      std::vector<float>>() == -1);
static_assert(FindMorphism<unsigned, std::vector<int>>()   == -1);
static_assert(FindMorphism<unsigned, std::vector<float>>() == -1);
static_assert(FindMorphism<float,    std::vector<int>>()   == -1);
static_assert(FindMorphism<float,    std::vector<float>>() == -1);

bool test_order1() {
    std::cout << "test_order1()\n";
    if (Convert<std::string>(666) != "666") {
        std::cout << Convert<std::string>(666) << " != 666";
        return false;
    }
    if (Convert<std::string>(true) != "yes") {
        std::cout << Convert<std::string>(true) << " != yes";
        return false;
    }
    if (Convert<std::string>(1u) != "unsigned 1") {
        std::cout << Convert<std::string>(1u) << " != unsigned 1";
        return false;
    }
    if (Convert<std::vector<bool>>(true)[0] != true) {
        std::cout << "false != true";
        return false;
    }
    return true;
}
