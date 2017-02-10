
// This is a generated file. Do not edit!

#ifndef GLOPERATE_COMPILER_DETECTION_H
#define GLOPERATE_COMPILER_DETECTION_H

#ifdef __cplusplus
# define GLOPERATE_COMPILER_IS_Comeau 0
# define GLOPERATE_COMPILER_IS_Intel 0
# define GLOPERATE_COMPILER_IS_PathScale 0
# define GLOPERATE_COMPILER_IS_Embarcadero 0
# define GLOPERATE_COMPILER_IS_Borland 0
# define GLOPERATE_COMPILER_IS_Watcom 0
# define GLOPERATE_COMPILER_IS_OpenWatcom 0
# define GLOPERATE_COMPILER_IS_SunPro 0
# define GLOPERATE_COMPILER_IS_HP 0
# define GLOPERATE_COMPILER_IS_Compaq 0
# define GLOPERATE_COMPILER_IS_zOS 0
# define GLOPERATE_COMPILER_IS_XL 0
# define GLOPERATE_COMPILER_IS_VisualAge 0
# define GLOPERATE_COMPILER_IS_PGI 0
# define GLOPERATE_COMPILER_IS_Cray 0
# define GLOPERATE_COMPILER_IS_TI 0
# define GLOPERATE_COMPILER_IS_Fujitsu 0
# define GLOPERATE_COMPILER_IS_SCO 0
# define GLOPERATE_COMPILER_IS_AppleClang 0
# define GLOPERATE_COMPILER_IS_Clang 0
# define GLOPERATE_COMPILER_IS_GNU 0
# define GLOPERATE_COMPILER_IS_MSVC 0
# define GLOPERATE_COMPILER_IS_ADSP 0
# define GLOPERATE_COMPILER_IS_IAR 0
# define GLOPERATE_COMPILER_IS_ARMCC 0
# define GLOPERATE_COMPILER_IS_MIPSpro 0

#if defined(__COMO__)
# undef GLOPERATE_COMPILER_IS_Comeau
# define GLOPERATE_COMPILER_IS_Comeau 1

#elif defined(__INTEL_COMPILER) || defined(__ICC)
# undef GLOPERATE_COMPILER_IS_Intel
# define GLOPERATE_COMPILER_IS_Intel 1

#elif defined(__PATHCC__)
# undef GLOPERATE_COMPILER_IS_PathScale
# define GLOPERATE_COMPILER_IS_PathScale 1

#elif defined(__BORLANDC__) && defined(__CODEGEARC_VERSION__)
# undef GLOPERATE_COMPILER_IS_Embarcadero
# define GLOPERATE_COMPILER_IS_Embarcadero 1

#elif defined(__BORLANDC__)
# undef GLOPERATE_COMPILER_IS_Borland
# define GLOPERATE_COMPILER_IS_Borland 1

#elif defined(__WATCOMC__) && __WATCOMC__ < 1200
# undef GLOPERATE_COMPILER_IS_Watcom
# define GLOPERATE_COMPILER_IS_Watcom 1

#elif defined(__WATCOMC__)
# undef GLOPERATE_COMPILER_IS_OpenWatcom
# define GLOPERATE_COMPILER_IS_OpenWatcom 1

#elif defined(__SUNPRO_CC)
# undef GLOPERATE_COMPILER_IS_SunPro
# define GLOPERATE_COMPILER_IS_SunPro 1

#elif defined(__HP_aCC)
# undef GLOPERATE_COMPILER_IS_HP
# define GLOPERATE_COMPILER_IS_HP 1

#elif defined(__DECCXX)
# undef GLOPERATE_COMPILER_IS_Compaq
# define GLOPERATE_COMPILER_IS_Compaq 1

#elif defined(__IBMCPP__) && defined(__COMPILER_VER__)
# undef GLOPERATE_COMPILER_IS_zOS
# define GLOPERATE_COMPILER_IS_zOS 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ >= 800
# undef GLOPERATE_COMPILER_IS_XL
# define GLOPERATE_COMPILER_IS_XL 1

#elif defined(__IBMCPP__) && !defined(__COMPILER_VER__) && __IBMCPP__ < 800
# undef GLOPERATE_COMPILER_IS_VisualAge
# define GLOPERATE_COMPILER_IS_VisualAge 1

#elif defined(__PGI)
# undef GLOPERATE_COMPILER_IS_PGI
# define GLOPERATE_COMPILER_IS_PGI 1

#elif defined(_CRAYC)
# undef GLOPERATE_COMPILER_IS_Cray
# define GLOPERATE_COMPILER_IS_Cray 1

#elif defined(__TI_COMPILER_VERSION__)
# undef GLOPERATE_COMPILER_IS_TI
# define GLOPERATE_COMPILER_IS_TI 1

#elif defined(__FUJITSU) || defined(__FCC_VERSION) || defined(__fcc_version)
# undef GLOPERATE_COMPILER_IS_Fujitsu
# define GLOPERATE_COMPILER_IS_Fujitsu 1

#elif defined(__SCO_VERSION__)
# undef GLOPERATE_COMPILER_IS_SCO
# define GLOPERATE_COMPILER_IS_SCO 1

#elif defined(__clang__) && defined(__apple_build_version__)
# undef GLOPERATE_COMPILER_IS_AppleClang
# define GLOPERATE_COMPILER_IS_AppleClang 1

#elif defined(__clang__)
# undef GLOPERATE_COMPILER_IS_Clang
# define GLOPERATE_COMPILER_IS_Clang 1

#elif defined(__GNUC__)
# undef GLOPERATE_COMPILER_IS_GNU
# define GLOPERATE_COMPILER_IS_GNU 1

#elif defined(_MSC_VER)
# undef GLOPERATE_COMPILER_IS_MSVC
# define GLOPERATE_COMPILER_IS_MSVC 1

#elif defined(__VISUALDSPVERSION__) || defined(__ADSPBLACKFIN__) || defined(__ADSPTS__) || defined(__ADSP21000__)
# undef GLOPERATE_COMPILER_IS_ADSP
# define GLOPERATE_COMPILER_IS_ADSP 1

#elif defined(__IAR_SYSTEMS_ICC__ ) || defined(__IAR_SYSTEMS_ICC)
# undef GLOPERATE_COMPILER_IS_IAR
# define GLOPERATE_COMPILER_IS_IAR 1

#elif defined(__ARMCC_VERSION)
# undef GLOPERATE_COMPILER_IS_ARMCC
# define GLOPERATE_COMPILER_IS_ARMCC 1

#elif defined(_SGI_COMPILER_VERSION) || defined(_COMPILER_VERSION)
# undef GLOPERATE_COMPILER_IS_MIPSpro
# define GLOPERATE_COMPILER_IS_MIPSpro 1


#endif

#  if GLOPERATE_COMPILER_IS_AppleClang

#    if !(((__clang_major__ * 100) + __clang_minor__) >= 400)
#      error Unsupported compiler version
#    endif

# define GLOPERATE_COMPILER_VERSION_MAJOR (__clang_major__)
# define GLOPERATE_COMPILER_VERSION_MINOR (__clang_minor__)
# define GLOPERATE_COMPILER_VERSION_PATCH (__clang_patchlevel__)
# if defined(_MSC_VER)
   /* _MSC_VER = VVRR */
#  define GLOPERATE_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#  define GLOPERATE_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
# endif
# define GLOPERATE_COMPILER_VERSION_TWEAK (__apple_build_version__)

#    if ((__clang_major__ * 100) + __clang_minor__) >= 400 && __has_feature(cxx_noexcept)
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 1
#    else
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 0
#    endif

#  elif GLOPERATE_COMPILER_IS_Clang

#    if !(((__clang_major__ * 100) + __clang_minor__) >= 304)
#      error Unsupported compiler version
#    endif

# define GLOPERATE_COMPILER_VERSION_MAJOR (__clang_major__)
# define GLOPERATE_COMPILER_VERSION_MINOR (__clang_minor__)
# define GLOPERATE_COMPILER_VERSION_PATCH (__clang_patchlevel__)
# if defined(_MSC_VER)
   /* _MSC_VER = VVRR */
#  define GLOPERATE_SIMULATE_VERSION_MAJOR (_MSC_VER / 100)
#  define GLOPERATE_SIMULATE_VERSION_MINOR (_MSC_VER % 100)
# endif

#    if ((__clang_major__ * 100) + __clang_minor__) >= 304 && __has_feature(cxx_noexcept)
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 1
#    else
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 0
#    endif

#  elif GLOPERATE_COMPILER_IS_GNU

#    if !((__GNUC__ * 100 + __GNUC_MINOR__) >= 404)
#      error Unsupported compiler version
#    endif

# define GLOPERATE_COMPILER_VERSION_MAJOR (__GNUC__)
# if defined(__GNUC_MINOR__)
#  define GLOPERATE_COMPILER_VERSION_MINOR (__GNUC_MINOR__)
# endif
# if defined(__GNUC_PATCHLEVEL__)
#  define GLOPERATE_COMPILER_VERSION_PATCH (__GNUC_PATCHLEVEL__)
# endif

#    if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && (__cplusplus >= 201103L || (defined(__GXX_EXPERIMENTAL_CXX0X__) && __GXX_EXPERIMENTAL_CXX0X__))
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 1
#    else
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 0
#    endif

#  elif GLOPERATE_COMPILER_IS_MSVC

#    if !(_MSC_VER >= 1600)
#      error Unsupported compiler version
#    endif

  /* _MSC_VER = VVRR */
# define GLOPERATE_COMPILER_VERSION_MAJOR (_MSC_VER / 100)
# define GLOPERATE_COMPILER_VERSION_MINOR (_MSC_VER % 100)
# if defined(_MSC_FULL_VER)
#  if _MSC_VER >= 1400
    /* _MSC_FULL_VER = VVRRPPPPP */
#   define GLOPERATE_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 100000)
#  else
    /* _MSC_FULL_VER = VVRRPPPP */
#   define GLOPERATE_COMPILER_VERSION_PATCH (_MSC_FULL_VER % 10000)
#  endif
# endif
# if defined(_MSC_BUILD)
#  define GLOPERATE_COMPILER_VERSION_TWEAK (_MSC_BUILD)
# endif

#    if _MSC_VER >= 1900
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 1
#    else
#      define GLOPERATE_COMPILER_CXX_NOEXCEPT 0
#    endif

#  else
#    error Unsupported compiler
#  endif

#  if GLOPERATE_COMPILER_CXX_NOEXCEPT
#    define GLOPERATE_NOEXCEPT noexcept
#    define GLOPERATE_NOEXCEPT_EXPR(X) noexcept(X)
#  else
#    define GLOPERATE_NOEXCEPT
#    define GLOPERATE_NOEXCEPT_EXPR(X)
#  endif

#endif

#endif
