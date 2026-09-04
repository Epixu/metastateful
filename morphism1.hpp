#pragma once
#include "metastateful.hpp"
#include <vector>
#include <string>

/// Literal -> string
template<CT::Literal L, int UNIQUE> UniqueMorphismIdx(L)
struct Morphism<L, UNIQUE> {
    using Conceptual = L;
    using From = L;
    using To = std::string;

    template<class TO>
    static constexpr TO Convert(L const& from) {
        return {from.data};
    }
};
/// Override unsigned -> string in particular
template<int UNIQUE> UniqueMorphismIdx(unsigned)
struct Morphism<unsigned, UNIQUE> {
    using From = unsigned;
    using To = std::string;

    template<class TO>
    static constexpr TO Convert(unsigned const& from) {
        return "unsigned " + std::to_string(from);
    }
};

/// Override bool -> string in particular
template<int UNIQUE> UniqueMorphismIdx(bool)
struct Morphism<bool, UNIQUE> {
    using From = bool;
    using To = std::string;

    template<class TO>
    static constexpr TO Convert(bool const& from) {
        return from ? "yes" : "no";
    }
};

/// Map all integrals as convertible to string using a concept
template<std::integral T, int UNIQUE> UniqueMorphismIdx(T)
struct Morphism<T, UNIQUE> {
    using Conceptual = T;
    using From = T;
    using To = std::string;

    template<class TO>
    static constexpr TO Convert(T const& from) {
        return std::to_string(from);
    }
};



#define morphism1_included

#if defined(morphism2_included)
    /*static_assert(Convertible<int,       std::string>);
    static_assert(Convertible<unsigned,  std::string>);
    static_assert(Convertible<int,       std::vector<int>>);
    static_assert(not Convertible<float, std::string>);*/

    static_assert(FindMorphism<int,      std::string>()       == 1);
    //static_assert(FindMorphism<unsigned, std::string>()       == 2);
    static_assert(FindMorphism<unsigned, std::string>()       != -1);
    //static_assert(FindMorphism<bool,     std::string>()       == 3);
    static_assert(FindMorphism<bool,     std::string>()       != -1);
    static_assert(FindMorphism<int,      std::vector<int>>()  == 0);
    //static_assert(FindMorphism<bool,     std::vector<bool>>() == 1);
    static_assert(FindMorphism<bool,     std::vector<bool>>() != -1);
#else
    /*static_assert(Convertible<int,       std::string>);
    static_assert(Convertible<unsigned,  std::string>);
    static_assert(not Convertible<int,   std::vector<int>>);
    static_assert(not Convertible<float, std::string>);*/

    // Exact order is undefined due to compiled differences, but this doesn't matter
    /*static_assert(FindMorphism<unsigned, std::string>()       == 1);
    static_assert(FindMorphism<bool,     std::string>()       == 1);*/
    static_assert(FindMorphism<int,      std::string>()       == 0);
    static_assert(FindMorphism<unsigned, std::string>()       != -1);
    static_assert(FindMorphism<bool,     std::string>()       != -1);
    static_assert(FindMorphism<int,      std::vector<int>>()  == -1);
    static_assert(FindMorphism<bool,     std::vector<bool>>() == -1);
#endif

static_assert(FindMorphism<Literal<256>, std::string>()    == 0);
static_assert(FindMorphism<float,    std::string>()        == -1);
static_assert(FindMorphism<int,      std::vector<float>>() == -1);
static_assert(FindMorphism<bool,     std::vector<float>>() == -1);
static_assert(FindMorphism<unsigned, std::vector<int>>()   == -1);
static_assert(FindMorphism<unsigned, std::vector<float>>() == -1);
static_assert(FindMorphism<float,    std::vector<int>>()   == -1);
static_assert(FindMorphism<float,    std::vector<float>>() == -1);
