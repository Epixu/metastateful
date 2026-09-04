#pragma once
#include "metastateful.hpp"
#include <vector>
#include <string>
/// Override bool -> vector in particular
template<int UNIQUE> UniqueMorphismIdx(bool)
struct Morphism<bool, UNIQUE> {
    using From = bool;
    using To = std::vector<bool>;

    template<class TO>
    static constexpr TO Convert(bool const& from) {
        return std::vector<bool>{from};
    }
};

/// Map all integrals as convertible to vector using a concept
template<std::integral T, int UNIQUE> UniqueMorphismIdx(T)
struct Morphism<T, UNIQUE> {
    using Conceptual = T;
    using From = T;
    using To = std::vector<T>;

    template<class TO>
    static constexpr TO Convert(T const& from) {
        return std::vector<T>(1, from);
    }
};


#define morphism2_included

#if defined(morphism1_included)
    /*static_assert(Convertible<int,       std::string>);
    static_assert(Convertible<unsigned,  std::string>);
    static_assert(Convertible<int,       std::vector<int>>);
    static_assert(not Convertible<float, std::string>);*/

    static_assert(FindMorphism<Literal<256>, std::string>()   == 0);
    static_assert(FindMorphism<int,      std::string>()       == 0);
    // Exact order is undefined due to compiled differences, but this doesn't matter
    /*static_assert(FindMorphism<unsigned, std::string>()       == 1);
    static_assert(FindMorphism<bool,     std::string>()       == 1);*/
    static_assert(FindMorphism<unsigned, std::string>()       != -1);
    static_assert(FindMorphism<bool,     std::string>()       != -1);
    static_assert(FindMorphism<int,      std::vector<int>>()  == 1);
    //static_assert(FindMorphism<bool,     std::vector<bool>>() == 3);
    static_assert(FindMorphism<bool,     std::vector<bool>>() != -1);
#else
    /*static_assert(not Convertible<int,      std::string>);
    static_assert(not Convertible<unsigned, std::string>);
    static_assert(    Convertible<int,      std::vector<int>>);
    static_assert(not Convertible<float,    std::string>);*/

    static_assert(FindMorphism<Literal<256>, std::string>()   == -1);
    static_assert(FindMorphism<int,      std::string>()       == -1);
    static_assert(FindMorphism<unsigned, std::string>()       == -1);
    static_assert(FindMorphism<bool,     std::string>()       == -1);
    static_assert(FindMorphism<int,      std::vector<int>>()  ==  0);
    //static_assert(FindMorphism<bool,     std::vector<bool>>() ==  1);
    static_assert(FindMorphism<bool,     std::vector<bool>>() != -1);
#endif

static_assert(FindMorphism<float,    std::string>()        == -1);
static_assert(FindMorphism<int,      std::vector<float>>() == -1);
static_assert(FindMorphism<bool,     std::vector<float>>() == -1);
static_assert(FindMorphism<unsigned, std::vector<int>>()   == -1);
static_assert(FindMorphism<unsigned, std::vector<float>>() == -1);
static_assert(FindMorphism<float,    std::vector<int>>()   == -1);
static_assert(FindMorphism<float,    std::vector<float>>() == -1);
