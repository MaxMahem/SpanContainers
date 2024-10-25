#pragma once

#ifndef SC_USE_EXCEPTIONS
#define SC_USE_EXCEPTIONS true
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1900 && _MSC_FULL_VER >= 190023918 && _MSC_VER < 2000
//Other compilers make empty bases occupy 0, MSVC doesn't always do that without the following line:
  #define SC_EMPTY_BASES __declspec(empty_bases)
#else
  #define SC_EMPTY_BASES
#endif

// VS2019 v16.10 and later (_MSC_FULL_VER >= 192829913 for VS 2019 v16.9)
#if _MSC_VER >= 1929 && _MSC_VER < 2000 
  #define SC_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
  #define SC_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif