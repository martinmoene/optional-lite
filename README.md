optional lite - nullable objects for C++03 &emsp; ![Build Status](https://travis-ci.org/martinmoene/optional-lite.png?branch=master)
============================================

*optional lite* is a single-header header-only library to represent optional (nullable) objects and pass them by value. The library is a variant of std::optional [1,2] for use with C++03 and later and Visual C++ 6 (VC6).

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Synopsis](#synopsis)
- [Comparison of std::optional, optional lite and Boost.Optional](#comparison-of-std-optional-optional-lite-and-boost-optional)
- [Reported to work with](#reported-to-work-with)
- [Implementation notes](#implementation-notes)
- [Notes and references](#notes-and-references)


Example usage
-------------

```C++
#include "optional.hpp"

#include <cstdlib>
#include <iostream>

using nonstd::optional;
using nonstd::nullopt;

optional<int> to_int( char const * const text )
{
    char * pos = NULL;
    const int value = strtol( text, &pos, 0 );

    return pos == text ? nullopt : optional<int>( value );
}

int main( int argc, char * argv[] )
{
    char * text = argc > 1 ? argv[1] : "42";

    optional<int> oi = to_int( text );

    if ( oi ) std::cout << "'" << text << "' is " << *oi;
    else      std::cout << "'" << text << "' isn't a number";
}
```
### Compile and run
```
prompt>g++ -Wall -Wextra -std=c++03 -I.. -o to_int.exe to_int.cpp && to_int x1
'x1' isn't a number
```

In a nutshell
---------------
**optional lite** is a single-header header-only library to represent optional (nullable) objects and pass them by value. The library is a variant of [std::optional](https://github.com/akrzemi1/Optional) [1,2] for use with C++03 and later and with Visual C++ 6 (VC6). In turn, std::optional is inspired on [Boost.Optional](http://www.boost.org/doc/libs/1_49_0/libs/optional/doc/html/index.html) [3].

**Features and properties of optional lite** are default and explicit construction of an empty optional, construction and assignment from a value that is convertible to the underlying type, copy-construction and copy-assignment from another optional of the same type, testing for the presence of a value, operators for unchecked access to the value (pointer or reference), value() and value_or() for checked access to the value, relational operators, swap() and make_optional() to create an optional of the proper type.  

**Not provided** are reference-type optionals, and C++11 capabilities such as move semantics and in-place construction. *optional lite* doesn't handle overloaded *address of* operators.

For more examples, see [this answer on StackOverflow](http://stackoverflow.com/a/16861022) [4] and the [quick start guide](http://www.boost.org/doc/libs/1_57_0/libs/optional/doc/html/boost_optional/quick_start.html) [5] of Boost.Optional (note that its interface differs from *optional lite*).


Dependencies
------------
*optional lite* has no other dependencies than the C++ standard library.


Installation
------------

*optional lite* is a single-file header-only library. Put `optional.hpp` directly into the project source tree or somewhere reachable from your project.


Synopsis
--------

**Contents**
[Types in namespace nonstd](#types-in-namespace-nonstd)
[Interface of *optional lite*](#interface-of-optional-lite)
[Algorithms for *optional lite*](#algorithms-for-optional-lite)
[Macros to control alignment](#macros-to-control-alignment)

### Types in namespace nonstd

| Purpose         | Type | Remark |
|-----------------|------|--------|
| To be, or not   | template< typename T ><br>class optional; |&nbsp;|
| Disengaging     | struct nullopt_t;            |&nbsp;  |
| &nbsp;          | nullopt_t nullopt;           | object |
| Error reporting | class bad_optional_access;   |&nbsp;  |

### Interface of *optional lite*

| Kind         | Method | Result |
|--------------------------------|
| Construction | optional()            | a nulled object  |
| &nbsp;       | optional( nullopt_t ) | a nulled object 
| &nbsp;       | optional( value_type const & value )         | initialized to value
| &nbsp;       | optional( optional const & other )           | initialized to value of other
| Destruction  | ~optional()           | destruct current content, if any
| Assignment   | optional & operator=( nullopt_t )            | null the object;<br>destruct current content, if any |
| &nbsp;       | optional & operator=( optional const & rhs ) | assign the value of other;<br>destruct current content, if any |
| Swap         | void swap( optional & other )                | swap with other |
| State        | operator bool() const | true if content is present (safe bool idiom)
| Content      | value_type const * operator ->() const       | pointer to current content (const);<br>must contain value
| &nbsp;       | value_type * operator ->()            | pointer to current content (non-const);<br>must contain value |
| &nbsp;       | value_type const & operator *() const | the current content (const ref);<br>must contain value |
| &nbsp;       | value_type & operator *()             | the current content (non-const ref);<br>must contain value |
| &nbsp;       | value_type const & value() const      | the current content (const ref);<br>throws bad_optional_access if nulled |
| &nbsp;       | value_type & value()                  | the current content (non-const ref);<br>throws bad_optional_access if nulled |
| &nbsp;       | value_type value_or( value_type const & default_value ) | the value, or default_value if nulled<br>value_type must be copy-constructible |

### Algorithms for *optional lite*

| Kind                     | Function |
|-------------------------------------|
| Relational operators     | &nbsp;   | 
| ==                       | template< typename T ><br>bool operator==( optional<T> const & x, optional<T> const & y )
| !=                       | template< typename T ><br>bool operator!=( optional<T> const & x, optional<T> const & y )
| <                        | template< typename T ><br>bool operator<( optional<T> const & x, optional<T> const & y )
| >                        | template< typename T ><br>bool operator>( optional<T> const & x, optional<T> const & y )
| <=                       | template< typename T ><br>bool operator<=( optional<T> const & x, optional<T> const & y )
| >=                       | template< typename T ><br>bool operator>=( optional<T> const & x, optional<T> const & y )
| Comparison with nullopt  | &nbsp;   | 
| ==                       | template< typename T ><br>bool operator==( optional<T> const & x, nullopt_t )
| &nbsp;                   | template< typename T ><br>bool operator==( nullopt_t, optional<T> const & x )
| !=                       | template< typename T ><br>bool operator!=( optional<T> const & x, nullopt_t )
| &nbsp;                   | template< typename T ><br>bool operator!=( nullopt_t, optional<T> const & x )
| <                        | template< typename T ><br>bool operator<( optional<T> const &, nullopt_t )
| &nbsp;                   | template< typename T ><br>bool operator<( nullopt_t, optional<T> const & x )
| <=                       | template< typename T ><br>bool operator<=( optional<T> const & x, nullopt_t )
| &nbsp;                   | template< typename T ><br>bool operator<=( nullopt_t, optional<T> const & )
| >                        | template< typename T ><br>bool operator>( optional<T> const & x, nullopt_t )
| &nbsp;                   | template< typename T ><br>bool operator>( nullopt_t, optional<T> const & )
| >=                       | template< typename T ><br>bool operator>=( optional<T> const &, nullopt_t )
| &nbsp;                   | template< typename T ><br>bool operator>=( nullopt_t, optional<T> const & x )
| Comparison with T        | &nbsp;   | 
| ==                       | template< typename T ><br>bool operator==( optional<T> const & x, const T& v )
| &nbsp;                   | template< typename T ><br>bool operator==( T const & v, optional<T> const & x )
| !=                       | template< typename T ><br>bool operator!=( optional<T> const & x, const T& v )
| &nbsp;                   | template< typename T ><br>bool operator!=( T const & v, optional<T> const & x )
| <                        | template< typename T ><br>bool operator<( optional<T> const & x, const T& v )
| &nbsp;                   | template< typename T ><br>bool operator<( T const & v, optional<T> const & x )
| <=                       | template< typename T ><br>bool operator<=( optional<T> const & x, const T& v )
| &nbsp;                   | template< typename T ><br>bool operator<=( T const & v, optional<T> const & x )
| >                        | template< typename T ><br>bool operator>( optional<T> const & x, const T& v )
| &nbsp;                   | template< typename T ><br>bool operator>( T const & v, optional<T> const & x )
| >=                       | template< typename T ><br>bool operator>=( optional<T> const & x, const T& v )
| &nbsp;                   | template< typename T ><br>bool operator>=( T const & v, optional<T> const & x )
| Specialized algorithms   | &nbsp;   | 
| swap                     | template< typename T ><br>void swap( optional<T> & x, optional<T> & y )
| create                   | template< typename T ><br>optional<T> make_optional( T const & v )


### Macros to control alignment

If *optional lite* is compiled as C++11 or later, C++11 alignment facilities are used for storage of the underlying object. When compiled with C++03, *optional lite* tries to determine proper alignment itself. If this doesn't work out, you can control alignment via the following macros. See also section [Implementation notes](#implementation-notes).

-D<b>optional_FEATURE_MAX_ALIGN_HACK</b>=0  
Define this to 1 to use the *max align hack* for alignment. Default is 0.

-D<b>optional_FEATURE_ALIGN_AS</b>=*pod-type*  
Define this to the *pod-type* you want to align to (no default).

-D<b>optional_FEATURE_ALIGN_AS_FALLBACK</b>=*pod-type*  
Define this to the *pod-type* to use for alignment if the algorithm of *optional lite* cannot find a suitable POD type to use for alignment. Default is double.


Comparison of std::optional, optional lite and Boost.Optional
-------------------------------------------------------------

*optional lite* is inspired on std::optional, which in turn is inspired on Boost.Optional. Here are the significant differences.

| Aspect                            | optional              | optional lite | Boost.Optional |
|-----------------------------------|-----------------------|---------------|----------------|
| Move semantics                    | yes                   | no            | no             |
| noexcept                          | yes                   | no            | no             |
| Hash support                      | yes                   | no            | no             |
| Throwing value accessor           | yes                   | yes           | no             |
| Literal type	                    | partially             | no            | no             |
| In-place construction	            | emplace, tag in_place | no            | utility in_place_factory |
| Disengaged state tag	            | nullopt	            | nullopt       | none           |
| optional references               | no (optionally)       | no            | yes            |
| Conversion from optional\<U\> to optional\<T\>       | no | no            | yes            |
| Duplicated interface functions 1) | no                    | no            | yes            |
| Explicit convert to ptr (get_ptr)	| no                    | no            | yes            |

1) is_initialized(), reset(), get().


Reported to work with
---------------------

*optional lite* is reported to work with the following compilers: 
- Visual C++ 6 SP5 (VS6), VC10, (VS2010), VC11 (VS2012), VC12 (VS2013), ...
- GNUC 4.8.1 with -std=c++98, -std=c++03, -std=c++11, -std=c++1y 
- clang


Implementation notes
--------------------

### Object allocation and alignment

*optional lite* reserves POD-type storage for an object of the underlying type inside a union to prevent unwanted construction and uses placement new to construct the object when required. Using non-placement new (malloc) to  obtain storage, ensures that the memory is properly aligned for the object's type, whereas that's not the case with placement new.

If you access data that's not properly aligned, it 1) may take longer than when it is properly aligned (on x86 processors), or 2) it may terminate the program immediately (many other processors).

Although the C++ standard does not guarantee that all user-defined types have the alignment of some POD type, in practice it's likely they do [6, part 2].

If *optional lite* is compiled as C++11 or later, C++11 alignment facilities are used for storage of the underlying object. When compiling with C++03, *optional lite* tries to determine proper alignment using meta programming. If this doesn't work out, you can control alignment via three macros. 

*optional lite* uses the following rules for alignment:

1. If the program compiles as C++11 or later, C++11 alignment facilities  are used.

2. If you define -D<b>optional_FEATURE_MAX_ALIGN_HACK</b>=1 the underlying type is aligned as the most restricted type in `struct max_align_t`. This potentially wastes many bytes per optional if the actually required alignment is much less, e.g. 24 bytes used instead of the 2 bytes required.

3. If you define -D<b>optional_FEATURE_ALIGN_AS</b>=*type* the underlying type is aligned as *type*. It's your obligation to specify a type with proper alignment.

4. If you define -D<b>optional_FEATURE_ALIGN_AS_FALLBACK</b>=*type* the fallback type for alignment of rule 5 below becomes *type*. It's your obligation to specify a type with proper alignment.

5. At default, *optional lite* tries to find a POD type with the same alignment as the underlying type. 

	The algorithm for alignment of 5. is:
	- Determine the alignment A of the underlying type using `alignment_of<>`.
	- Find a POD type from the list `alignment_types` with exactly alignment A.
	- If no such POD type is found, use a type with a relatively strict alignment requirement such as double;<br>this type is specified in  `optional_FEATURE_ALIGN_AS_FALLBACK` (default double).

Note that the algorithm of 5. differs from the one Andrei Alexandrescu uses in [6, part 2].

The class template `alignment_of<>` is gleaned from [Boost.TypeTraits, alignment_of](http://www.boost.org/doc/libs/1_57_0/libs/type_traits/doc/html/boost_typetraits/reference/alignment_of.html) [9]. The storage type `storage_t<>` is adapted from the one I created for [spike-expected, expected lite](https://github.com/martinmoene/spike-expected) [11].

For more information on constructed unions and alignment, see [6-10].


Notes and references
--------------------

[1] Fernando Cacciola, Andrzej Krzemieński. [A proposal to add a utility class to represent optional objects (Revision 5)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3793.html).

[2] Andrzej Krzemieński. [optional (nullable) objects for C++14](https://github.com/akrzemi1/Optional). Reference implementation on GetHub.

[3] Fernando Cacciola. [Boost.Optional library](http://www.boost.org/doc/libs/1_49_0/libs/optional/doc/html/index.html).

[4] StackOverflow. [How should one use std::optional?](http://stackoverflow.com/a/16861022). Answer by Timothy Shields. 31 May 2013.

[5] Fernando Cacciola. [Boost.Optional Quick start guide](http://www.boost.org/doc/libs/1_57_0/libs/optional/doc/html/boost_optional/quick_start.html).

[6] Andrei Alexandrescu. [Generic<Programming>: Discriminated Unions 1](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2004/alexandr/alexandr.htm), [part2](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2006/alexandr/alexandr.htm), [part 3](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2008/alexandr/alexandr.htm). April 2002. 

[7] Herb Sutter. [Style Case Study #3: Construction Unions](http://www.gotw.ca/gotw/085.htm). GotW #85. 2009

[8] Kevin T. Manley. [Using Constructed Types in C++ Unions](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/0208/manley/manley.htm). C/C++ Users Journal, 20(8), August 2002.

[9] StackOverflow. [Determining maximum possible alignment in C++](http://stackoverflow.com/a/3126992).

[10] [Boost.TypeTraits, alignment_of](http://www.boost.org/doc/libs/1_57_0/libs/type_traits/doc/html/boost_typetraits/reference/alignment_of.html) ( [code](http://www.boost.org/doc/libs/1_57_0/boost/type_traits/alignment_of.hpp) ).

[11] Martin Moene. [spike-expected](https://github.com/martinmoene/spike-expected) ([expected-lite.hpp](https://github.com/martinmoene/spike-expected/blob/master/exception_ptr_lite.hpp)).
