#ifndef AlgheLib_cppex_base_h__
#define AlgheLib_cppex_base_h__

// Compiler start
#if defined(_MSC_VER)
#define CPPEX_COMPILER_VC
#elif defined(__GNUC__)
#define CPPEX_COMPILER_GCC
#else
#error "Compiler not suported! Please edit the file and add your compiler"
#endif
// Compiler end


// Operating system start
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
#define CPPEX_LINUX
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define CPPEX_WINDOWS
#else
#error "Operating system not suported! Please edit the file and add your operating system"
#endif
// Operating system end


// Debug and Release
#ifdef _DEBUG
#define CPPEX_DEBUG
#else
#define CPPEX_RELEASE
#endif

// Compiler settings
#if defined(_MT) || defined(__MT__)
#define CPPEX_MULTI_THREAD
#endif

#if defined(_DLL)
#define CPPEX_DLL
#endif

#ifdef CPPEX_MULTI_THREAD
# ifdef CPPEX_DEBUG
#  ifdef CPPEX_DLL
#   define CPPEX_STATIC_LIB_SUFIX "-mdd"
#  else
#   define CPPEX_STATIC_LIB_SUFIX "-mtd"
#  endif
# else
#  ifdef CPPEX_DLL
#   define CPPEX_STATIC_LIB_SUFIX "-md"
#  else
#   define CPPEX_STATIC_LIB_SUFIX "-mt"
#  endif
# endif
#endif

#ifndef CPPEX_STATIC_LIB_SUFIX
#define CPPEX_STATIC_LIB_SUFIX ""
#endif

#define CPPEX_STATIC_LIB(l) l CPPEX_STATIC_LIB_SUFIX ".lib"



// DLL exports start
#if defined(CPPEX_WINDOWS) && defined(ALGHELIB_EXPORTS)
# define ALGHELIB_C_API extern "C" __declspec(dllexport)
# define ALGHELIB_API __declspec(dllexport)
# define ALGHELIB_DECL __stdcall
#elif defined(CPPEX_WINDOWS) && defined(ALGHELIB_IMPORTS)
# define ALGHELIB_C_API extern "C" __declspec(dllimport)
# define ALGHELIB_API __declspec(dllimport)
# define ALGHELIB_DECL __stdcall
#endif

#ifndef ALGHELIB_C_API
#define ALGHELIB_C_API
#endif
#ifndef ALGHELIB_API
#define ALGHELIB_API
#endif
#ifndef ALGHELIB_DECL
#define ALGHELIB_DECL
#endif

#define ALGHE_EXPORT_C
#define ALGHE_EXPORT_CPP
// DLL exports end


// type definition for API classes with no implementation
#ifdef CPPEX_WINDOWS
#define API_INTERFACE struct __declspec(novtable)
#else
#define API_INTERFACE struct
#endif


// Common type definitions start
#ifndef i8
typedef signed char            i8;
typedef unsigned char          u8;

typedef signed short           i16;
typedef unsigned short         u16;

typedef signed long            i32;
typedef unsigned long          u32;

#if defined(CPPEX_WINDOWS)
 typedef __int64               i64;
 typedef unsigned __int64      u64;
#elif defined(CPPEX_LINUX)
# if defined(__LP64__)
   typedef signed long         i64;
   typedef unsigned long       u64;
# else
   typedef signed long long    i64;
   typedef unsigned long long  u64;
# endif
#endif

#endif
// Common type definitions end


// Helper macros start
#define ALGHE_SUCCESSFUL(x)    ((x) >= 0)
#define ALGHE_FAILED(x)        ((x) < 0)

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);             \
	void operator=(const TypeName&)
#endif
// Helper macros end


// Common function helpers start
#ifndef ARRAY_SIZE
# define ARRAY_SIZE(a) (sizeof(a) / sizeof(*(a)))
#endif

#ifndef sizeof_field
#define sizeof_field(type, field) sizeof(((type *) 0)->field)
#endif

// Do nothing
#define NULL_STMT ((void) 0)

// shorter version of UNREFERENCED_PARAMETER
#define REF(arg)    (arg)

// Run-time assertion
#ifdef CPPEX_DEBUG
#define ASSERT(x) assert(x)
#define ASSERTFAST(x) ASSERT(x)
#else
#define ASSERT(x) ((void) 0)
#define ASSERTFAST(x) __assume(x)
#endif

// Compile-time assertion 
#define CCASSERT(predicate) _x_CCASSERT_LINE(predicate, __LINE__)
#define _x_CCASSERT_LINE(predicate, line) typedef char constraint_violated_on_line_##line[2*((predicate)!=0)-1];

// Output WARNINGS and NOTES during compilation
#define STRING2_(x) #x
#define STRING_(x) STRING2_(x)
#define NOTE(msg) __pragma( message (__FILE__ "[" STRING_(__LINE__) "] : NOTE: " msg) )
#define WARN(msg) __pragma( message (__FILE__ "[" STRING_(__LINE__) "] : WARN: " msg) )

// Check if some bits are set in a value
#ifndef HasFlag
#define HasFlag(x,flag) (((x) & (flag)) == (x))
#endif

// Swap two values
template <typename T>
inline void swap(T& a, T& b) {
    T temp(a);
    a = b;
    b = temp;
}


// Common includes start
#include <stddef.h>
#include <string>
// Common includes end


#endif // end header guard
