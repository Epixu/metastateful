# metastateful
Stateful metaprogramming exploit that is 100% C++23 standard compatible.
Enables compile-time reflection and other huge implications.

--------

Tested on GCC 14.2, GCC 16.2, Clang 21, Clang 22, MSVC v143. Surprisingly non-flaky.

[![Tests](https://github.com/Epixu/metastateful/actions/workflows/ci.yml/badge.svg)](https://github.com/Epixu/metastateful/actions/workflows/ci.yml)

> [!WARNING]
> Stateful metaprogramming is disputed in C++ standardization circles, and despite this being very stable at the moment, the future is uncertain.
> The controversial parts mostly involve:
>  - friend injection (not as controversial really, as ADL is not likely to change)
>  - making function instantiations unique by using a uniquely generated lambda type via `[]{}`. This allows for seemingly the same functions to return different results at different places, even if `consteval`. I find this an amazing feature, and seems to be 100% compliant. Only drawback is that it offloads some cognitive overhead to the code writer/reader, as they have to be aware of the changing compiler state. The benefits outweight the costs, in my opinion.
--------

> [!IMPORTANT]
> Wrapping `FindMorphism` in a concept will negate the uniqueness of each call, as concepts are memoized (cached). If you plan on using concepts that check for conversion, make sure you have included all relevant morphism specializations before using the concept for the first time, or just stuck with directly calling `FindMorphism` wherever needed.

You can fork and modify CI if you want to test on more configurations.


# Details
Minimally required infrastructure and boilerplate is in [metastateful.hpp](metastateful.hpp).
This repo demonstrates only the reflection of morphisms (a.k.a. converting functions between two types), but the principle can be extended for any amount/kind of type traits. 

Reflection is done by partial specialization of `struct Morphism` in [morphism1.hpp](morphism1.hpp) and [morphism2.hpp](morphism2.hpp).

Naturally, the order of reflection depends on the order of the includes. This uncertainty is tested in the two translation units: [order1.cpp](order1.cpp) and [order2.cpp](order2.cpp).
The order is irrelevant, however missing an include will naturally alter the translation unit's state, altering `FindMorphism`'s behavior.
There are nitty-gritty details that were taken care of, involving compiler caching and [specialization ordering](https://stackoverflow.com/questions/66740897/why-does-clang-prefer-the-primary-template-over-the-specialization-from-c17) specifics per compiler.

The result is surprisingly stable.

All that remains is the [main.cpp](main.cpp) which invokes the reflected converters to confirm, that the right converters were picked. This did caught a couple of early mistakes, as some of the morphisms overlap and override each other. Some are concept-based, while other are concrete specializations. This is very handy for reducing code duplication, as you can generalize morphisms pretty easily, so I was very keen on making it work.
