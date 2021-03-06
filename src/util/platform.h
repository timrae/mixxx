#ifndef MIXXX_UTIL_PLATFORM_H
#define MIXXX_UTIL_PLATFORM_H

#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
// Clang and GCC
#define M_ALIGN(x) __attribute__((aligned(x)))
#define M_RESTRICT __restrict__
#define M_PACK(statement) statement __attribute__((packed))
#define M_MUST_USE_RESULT __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
// MSVC
#define M_ALIGN(x) __declspec(align(x))
#define M_RESTRICT __restrict
#define M_PACK(statement)  __pragma( pack(push, 1) ) statement __pragma( pack(pop) )
// MSVC supports _Check_return_ but it's a prefix, not a suffix. We could do
// something like M_PACK but for now don't bother.
#define M_MUST_USE_RESULT
#else
#error We do not support your compiler. Please email mixxx-devel@lists.sourceforge.net and tell us about your use case.
#endif

#endif /* MIXXX_UTIL_PLATFORM_H */
