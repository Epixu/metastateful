#pragma once
#include <array>
#include <string_view>
#include <type_traits>
#include <concepts>
#include <bit>

#if defined(__GNUC__) and not defined(__clang__)
_Pragma("GCC diagnostic ignored \"-Wnon-template-friend\"")
_Pragma("GCC diagnostic ignored \"-Wsfinae-incomplete\"")
#endif



///
/// LITERAL IMPLEMENTATION
///
namespace CT
{
    template<class...T>
    concept Literal = (T::CTTI_Literal and ...);
}

template<size_t N = 0>
struct Literal {
    static constexpr bool   CTTI_Literal = true;
    static constexpr size_t ArraySize = N;

    using storage_type = ::std::array<char, N + 1>;
    storage_type data {};

    template<size_t M> requires (M <= N + 1)
    constexpr Literal(const char(&array)[M]) noexcept {
        for (size_t i = 0; i < M; i++)
            data[i] = array[i];
    }

    constexpr size_t size() const noexcept {
        auto ptr = data.data();
        const auto ptrEnd = ptr + N;
        while(ptr != ptrEnd and *ptr)
            ++ptr;
        return ptr - data.data();
    }
};

template<size_t N>
Literal(const char(&)[N]) -> Literal<::std::bit_ceil(N)>;

template<CT::Literal LHS, CT::Literal RHS>
constexpr bool operator == (const LHS& lhs, const RHS& rhs) {
    if (lhs.size() != rhs.size())
        return false;

    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs.data[i] != rhs.data[i])
            return false;
    }
    return true;
}



///
/// COMPILE-TIME IMMUTABLE VALUE LIST IMPLEMENTATION
///
template<auto...> struct Values;

template<>
struct Values<> {
    using FirstType = void;
    static constexpr bool Empty = true;
    static constexpr size_t Count = 0;

protected:
    template<auto...> friend struct Values;
    template<unsigned>
    static consteval auto AtInner() {
        static_assert(false, "Empty values");
    }

public:
    template<unsigned I>
    static constexpr auto At = AtInner<I>();
};


template<auto E1>
struct Values<E1> {
    using FirstType = decltype(E1);
    static constexpr auto First   = E1;
    static constexpr auto Last    = E1;
    static constexpr bool Empty   = false;
    static constexpr size_t Count = 1;

protected:
    template<auto...> friend struct Values;
    template<unsigned I>
    static consteval auto AtInner() {
        static_assert(I == 0, "Index is out of value list bounds");
        return E1;
    }

public:
    template<unsigned I>
    static constexpr auto At = AtInner<I>();
};

template<auto E1, auto E2, auto...EN>
struct Values<E1, E2, EN...> {
    using FirstType = decltype(E1);
    static constexpr auto First   = E1;
    static constexpr auto Second  = E2;
    static constexpr auto Last    = []<class V = Values<EN...>> {
        if constexpr (sizeof...(EN)) return V::Last;
        else return E2;
    }();
    static constexpr bool Empty   = false;
    static constexpr size_t Count = sizeof...(EN) + 2;

protected:
    template<auto...> friend struct Values;
    template<unsigned I>
    static consteval auto AtInner() {
        static_assert(I < Count, "Index is out of value list bounds");
             if constexpr (I == 0)    return E1;
        else if constexpr (I == 1)    return E2;
        else return Values<EN...>::template AtInner<I - 2>();
    }

public:
    template<unsigned I>
    static constexpr auto At = AtInner<I>();
};



///
/// COMPILE-TIME MUTABLE SET OF LITERALS IMPLEMENTATION
///
template<class ID, Literal...T>
struct StaticSet : Values<T...> {};

namespace Inner
{
    template<class ID, int>
    struct StaticSetAt {
        auto friend StaticSetGet(StaticSetAt);
    };
    
    template<class ID, int N, class T>
    struct StaticSetSet {
        auto friend StaticSetGet(StaticSetAt<ID, N>) {
            return T{};
        }
    };

    template<class ID, Literal T, template<class, Literal...> class TList, Literal...Ts>
    consteval auto StaticSetInsert(TList<ID, Ts...>) {
        static_assert(sizeof...(Ts) > 0);
        if constexpr (((Ts == T) or ...))
            return TList<ID, Ts...> {};
        else
            return TList<ID, Ts..., T> {};
    }
}

template<class ID, Literal T, int N = 0, auto unique = []{}>
consteval bool StaticSetInsert() {
    if constexpr (requires { StaticSetGet(Inner::StaticSetAt<ID, N>{}); }) {
        if constexpr (decltype(StaticSetGet(Inner::StaticSetAt<ID, N>{}))::template At<N> == T)
        return false;
        else 
        return StaticSetInsert<ID, T, N+1, unique>();
    }
    else if constexpr (N == 0) {
        (void) Inner::StaticSetSet<ID, N, StaticSet<ID, T>>{};
        return true;
    }
    else {
        (void) Inner::StaticSetSet<ID, N, decltype(
            Inner::StaticSetInsert<ID, T>(StaticSetGet(
                Inner::StaticSetAt<ID, N-1>{}
            ))
        )>{};
        return true;
    }
}

template<class ID, auto unique = []{}, int N = 0>
consteval auto GetStaticSet() {
    if constexpr (requires { StaticSetGet(Inner::StaticSetAt<ID, N>{}); })
        return GetStaticSet<ID, unique, N+1>();
    else if constexpr (N == 0)
        return StaticSet<ID>{};
    else
        return StaticSetGet(Inner::StaticSetAt<ID, N-1>{});
}

template<class ID, auto unique = []{}, int N = 0>
consteval int GetStaticSetSize() {
    if constexpr (requires { StaticSetGet(Inner::StaticSetAt<ID, N>{}); })
        return GetStaticSetSize<ID, unique, N+1>();
    else
        return N;
}

template<class ID, Literal T, int N = 0, auto unique = []{}>
consteval int GetStaticSetIndex() {
    if constexpr (requires { StaticSetGet(Inner::StaticSetAt<ID, N>{}); }) {
        if constexpr (decltype(StaticSetGet(Inner::StaticSetAt<ID, N>{}))::template At<N> == T)
            return N;
        else 
            return GetStaticSetIndex<ID, T, N+1, unique>();
    }
    else if constexpr (N == 0) {
        (void) Inner::StaticSetSet<ID, N, StaticSet<ID, T>>{};
        return 0;
    }
    else {
        (void) Inner::StaticSetSet<ID, N, decltype(
            Inner::StaticSetInsert<ID, T>(StaticSetGet(
                Inner::StaticSetAt<ID, N-1>{}
            ))
        )>{};
        return N;
    }
}



///
/// Build compile-time type traits
///
template<class FROM, int UNIQUE>
struct Morphism;

#define str(x) #x
#define str2(x) str(x)
#define HERE() __FILE__ ":" str2(__LINE__)
#define UniqueMorphismIdx(T) requires (UNIQUE == GetStaticSetIndex<T, HERE()>())

///
/// Read compile-time type traits
///
//template<class...T>
//concept Complete = (sizeof...(T) > 0) and ((sizeof(T) == sizeof(T)) and ...);

template<class FROM, class TO, int PROGRESS = 0, auto U = []{}>
consteval int FindMorphism() {
    //using I = uid<PROGRESS>;
    using M = Morphism<FROM, PROGRESS>;
    // if constexpr (Complete<M>) won't work, 
    // because Complete concept gets cached. 
    // For this to not fuck everything up due to ordering,
    // each call to FindMorphism needs to be unique
    if constexpr (requires { M{}; }) {
        if constexpr (std::same_as<TO, typename M::To> /*and std::same_as<FROM, typename M::From>*/) {
            // Prioritize concrete specializations
            // over conceptual ones
            if constexpr (requires { typename M::Conceptual; }) {
                constexpr int concrete = FindMorphism<FROM, TO, PROGRESS + 1, U>();
                if constexpr (concrete == -1)
                    return PROGRESS;
                else
                    return concrete;
            }
            else return PROGRESS;
        }
        else return FindMorphism<FROM, TO, PROGRESS + 1, U>();
    }
    else return -1;
}

template<class TO, class FROM>
constexpr auto Convert(FROM const& from) -> TO {
    constexpr int found = FindMorphism<FROM, TO, 0>();
    static_assert(found >= 0, "FROM can't be converted to TO - add Morphism specialization");

    using M = Morphism<FROM, found>;
    static_assert(requires { {M::template Convert<TO>(from)} -> ::std::same_as<TO>; },
        "Missing conversion method");

    return M::template Convert<TO>(from);
}

bool test_order1();
bool test_order2();

// Won't work unless performed after all includes
// Concepts have to be atomic - can't change values inbetween uses
template<class FROM, class TO>
concept Convertible = FindMorphism<FROM, TO>() != -1;