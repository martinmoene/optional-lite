// Copyright 2014 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// optional lite is inspired on std::Optional by Fernando Cacciola and Andrzej Krzemienski
// and on expected lite by Martin Moene.

#include "optional.hpp"

#include "lest_cpp03.hpp"

#define CASE( name ) lest_CASE( specification, name )

lest::test_specification specification;

using nonstd::optional;
using nonstd::nullopt;
using nonstd::bad_optional_access;
using nonstd::make_optional;

CASE( "A C++03 union can only contain POD types" )
{
    union U
    {
        char c;
//        waf x;
    };
}

/*
 * Positive tests:
 */

CASE( "A default constructed optional is empty" )
{
    optional<int> oi;
    EXPECT( !oi );
}

CASE( "An optional that is constructed disengaged explicitly is empty" )
{
    optional<int> oi( nullopt );
    EXPECT( !oi );
}

CASE( "An optional constructed with 42 contains 42" )
{
    optional<int> oi = 42;
    EXPECT( oi );
    EXPECT( *oi == 42 );
}

CASE( "An optional constructed from an empty optional is empty" )
{
    optional<int> oi1;
    optional<int> oi2( oi1 );
    EXPECT( !oi2 );
}

CASE( "An optional constructed from an non-empty optional obtains its value" )
{
    optional<int> oi1( 42 );
    optional<int> oi2( oi1 );
    EXPECT( oi2 );
    EXPECT( *oi2 == *oi1 );
}

CASE( "copy-assignment works for all permutations of engaged and disengaged optionals" )
{
    SETUP( "" ) {
        optional<int> d1;
        optional<int> d2;
        optional<int> e1( 123 );
        optional<int> e2( 987 );

    SECTION( "a disengaged optional assigned nullopt remains empty" ) {
        d1 = nullopt;
        EXPECT( !d1 );
    }
    SECTION( "a disengaged optional assigned an engaged optional obtains its value" ) {
        d1 = e1;
        EXPECT( !!d1 );
        EXPECT( *d1 == 123 );
    }
    SECTION( "an engaged optional assigned an engaged optional obtains its value" ) {
        e1 = e2;
        EXPECT( !!e1 );
        EXPECT( *e1 == 987 );
    }
    SECTION( "an engaged optional assigned nullopt becomes empty" ) {
        e1 = nullopt;
        EXPECT( !e1 );
    }
    SECTION( "a disengaged optional assigned a disengaged optional remains empty" ) {
        d1 = d2;
        EXPECT( !d1 );
    }}
}

CASE( "Member swap() swaps engage state and values" )
{
    SETUP( "" ) {
        optional<int> d1;
        optional<int> d2;
        optional<int> e1( 42 );
        optional<int> e2( 7 );

    SECTION( "swap disengaged with disengaged optional" ) {
        d1.swap( d2 );
        EXPECT( !d1 );
    }
    SECTION( "swap engaged with engaged optional" ) {
        e1.swap( e2 );
        EXPECT( !!e1  );
        EXPECT( !!e2 );
        EXPECT( *e1 == 7 );
        EXPECT( *e2 == 42 );
    }
    SECTION( "swap disengaged with engaged optional" ) {
        d1.swap( e1 );
        EXPECT( !!d1 );
        EXPECT(  !e1    );
        EXPECT( *d1 == 42 );
    }
    SECTION( "swap engaged with disengaged optional" ) {
        e1.swap( d1 );
        EXPECT( !!d1 );
        EXPECT(  !e1 );
        EXPECT( *d1 == 42 );
    }}
}

struct Integer { int x; Integer(int x) : x(x) {} };

CASE( "operator->() yields pointer to value" )
{
    SETUP( "" ) {
        optional<Integer> e( Integer( 42 ) );
    
    SECTION( "Operator->() yields pointer to value (const)" ) {
        EXPECT(  e->x == 42 );
    }
    SECTION( "Operator->() yields pointer to value (non-const)" ) {
        e->x = 7;
        EXPECT(  e->x == 7 );
    }}
}

CASE( "operator*() yields value" )
{
    SETUP( "" ) {
        optional<int> e( 42 );
    
    SECTION( "Operator*() yields value (const)" ) {
        EXPECT( *e == 42 );
    }
    SECTION( "Operator*() yields value (non-const)" ) {
        *e = 7;
        EXPECT( *e == 7 );
    }}
}

CASE( "value() yields value" )
{
    SETUP( "" ) {
        optional<int> e( 42 );
    
    SECTION( "value() yields value (const)" ) {
        EXPECT( e.value() == 42 );
    }
    SECTION( "value() yields value (non-const)" ) {
        e.value() = 7;
        EXPECT(  e.value() == 7 );
    }}
}

CASE( "value() throws bad_optional_access at disengaged access" )
{
    EXPECT_THROWS_AS( optional<int>().value(), bad_optional_access );
}

CASE( "value_or() yields value or default" )
{
    SETUP( "" ) {
        optional<int> d;
        optional<int> e( 42 );
    
    SECTION( "value_or( 7 ) yields value for non-empty optional" ) {
        EXPECT( e.value_or( 7 ) == 42 );
    }
    SECTION( "value_or( 7 ) yields default for empty optional" ) {
        EXPECT(  d.value_or( 7 ) == 7 );
    }}
}

CASE( "Global swap() swaps engage state and values" )
{
#if optional_COMPILER_IS_VC6
    using ::nonstd::swap;
#endif    

    SETUP( "" ) {
        optional<int> d1;
        optional<int> d2;
        optional<int> e1( 42 );
        optional<int> e2( 7 );

    SECTION( "swap disengaged with disengaged optional" ) {
        swap( d1, d2 );
        EXPECT( !d1 );
    }
    SECTION( "swap engaged with engaged optional" ) {
        swap( e1, e2 );
        EXPECT( !!e1  );
        EXPECT( !!e2 );
        EXPECT( *e1 == 7 );
        EXPECT( *e2 == 42 );
    }
    SECTION( "swap disengaged with engaged optional" ) {
        swap( d1, e1 );
        EXPECT( !!d1 );
        EXPECT(  !e1    );
        EXPECT( *d1 == 42 );
    }
    SECTION( "swap engaged with disengaged optional" ) {
        swap( e1, d1 );
        EXPECT( !!d1 );
        EXPECT(  !e1 );
        EXPECT( *d1 == 42 );
    }}
}

CASE( "make_optional() creates optional" )
{
    EXPECT( *make_optional( 42 ) == 42 );
}

CASE( "Relational operators" )
{
    SETUP( "" ) {
        optional<int> d;
        optional<int> e1( 6 );
        optional<int> e2( 7 );
    
    SECTION( "engaged    == engaged"    ) { EXPECT(   e1 == e1  ); }
    SECTION( "engaged    == disengaged" ) { EXPECT( !(e1 == d ) ); }
    SECTION( "disengaged == engaged"    ) { EXPECT( !(d  == e1) ); }

    SECTION( "engaged    != engaged"    ) { EXPECT(   e1 != e2  ); }
    SECTION( "engaged    != disengaged" ) { EXPECT(   e1 != d   ); }
    SECTION( "disengaged != engaged"    ) { EXPECT(   d  != e2  ); }

    SECTION( "engaged    <  engaged"    ) { EXPECT(   e1 <  e2  ); }
    SECTION( "engaged    <  disengaged" ) { EXPECT( !(e1 <  d ) ); }
    SECTION( "disengaged <  engaged"    ) { EXPECT(   d  <  e2  ); }

    SECTION( "engaged    <= engaged"    ) { EXPECT(   e1 <= e1  ); }
    SECTION( "engaged    <= engaged"    ) { EXPECT(   e1 <= e2  ); }
    SECTION( "engaged    <= disengaged" ) { EXPECT( !(e1 <= d ) ); }
    SECTION( "disengaged <= engaged"    ) { EXPECT(   d  <= e2  ); }

    SECTION( "engaged    >  engaged"    ) { EXPECT(   e2 >  e1  ); }
    SECTION( "engaged    >  disengaged" ) { EXPECT(   e2 >  d   ); }
    SECTION( "disengaged >  engaged"    ) { EXPECT( !(d  >  e1) ); }

    SECTION( "engaged    >= engaged"    ) { EXPECT(   e1 >= e1  ); }
    SECTION( "engaged    >= engaged"    ) { EXPECT(   e2 >= e1  ); }
    SECTION( "engaged    >= disengaged" ) { EXPECT(   e2 >= d   ); }
    SECTION( "disengaged >= engaged"    ) { EXPECT( !(d  >= e1) ); }

    SECTION( "disengaged == nullopt"    ) { EXPECT(  (d       == nullopt) ); }
    SECTION( "nullopt    == disengaged" ) { EXPECT(  (nullopt == d      ) ); }
    SECTION( "engaged    == nullopt"    ) { EXPECT(  (e1      != nullopt) ); }
    SECTION( "nullopt    == engaged"    ) { EXPECT(  (nullopt != e1     ) ); }
    SECTION( "disengaged == nullopt"    ) { EXPECT( !(d       <  nullopt) ); }
    SECTION( "nullopt    == disengaged" ) { EXPECT( !(nullopt <  d      ) ); }
    SECTION( "disengaged == nullopt"    ) { EXPECT(  (d       <= nullopt) ); }
    SECTION( "nullopt    == disengaged" ) { EXPECT(  (nullopt <= d      ) ); }
    SECTION( "disengaged == nullopt"    ) { EXPECT( !(d       >  nullopt) ); }
    SECTION( "nullopt    == disengaged" ) { EXPECT( !(nullopt >  d      ) ); }
    SECTION( "disengaged == nullopt"    ) { EXPECT(  (d       >= nullopt) ); }
    SECTION( "nullopt    == disengaged" ) { EXPECT(  (nullopt >= d      ) ); }

    SECTION( "engaged    == value"      ) { EXPECT( e1 == 6  ); }
    SECTION( "value     == engaged"     ) { EXPECT(  6 == e1 ); }
    SECTION( "engaged   != value"       ) { EXPECT( e1 != 7  ); }
    SECTION( "value     != engaged"     ) { EXPECT(  6 != e2 ); }
    SECTION( "engaged   <  value"       ) { EXPECT( e1 <  7  ); }
    SECTION( "value     <  engaged"     ) { EXPECT(  6 <  e2 ); }
    SECTION( "engaged   <= value"       ) { EXPECT( e1 <= 7  ); }
    SECTION( "value     <= engaged"     ) { EXPECT(  6 <= e2 ); }
    SECTION( "engaged   >  value"       ) { EXPECT( e2 >  6  ); }
    SECTION( "value     >  engaged"     ) { EXPECT(  7 >  e1 ); }
    SECTION( "engaged   >= value"       ) { EXPECT( e2 >= 6  ); }
    SECTION( "value     >= engaged"     ) { EXPECT(  7 >= e1 ); }
    }
}

/*
 * Negative tests:
 */

/*
 * Tests that print information:
 */

struct S{ S(){} };

#if !optional_FEATURE_MAX_ALIGN_HACK

#define optional_OUTPUT_ALIGNMENT_OF( type ) \
    "alignment_of<" #type ">: " <<  \
     alignment_of<type>::value  << "\n" <<

CASE("Show alignment of various types" 
     "[.]" )
{
#if optional_CPP11_OR_GREATER  
    using std::alignment_of;
#elif optional_COMPILER_IS_VC6
    using namespace ::nonstd::optional_detail;
#else
    using ::nonstd::optional_detail::alignment_of;
#endif    
    std::cout << 
        optional_OUTPUT_ALIGNMENT_OF( char )
        optional_OUTPUT_ALIGNMENT_OF( short )
        optional_OUTPUT_ALIGNMENT_OF( int )
        optional_OUTPUT_ALIGNMENT_OF( long )
        optional_OUTPUT_ALIGNMENT_OF( float )
        optional_OUTPUT_ALIGNMENT_OF( double )
        optional_OUTPUT_ALIGNMENT_OF( long double )
        optional_OUTPUT_ALIGNMENT_OF( S )
         "";
}
#undef optional_OUTPUT_ALIGNMENT_OF
#endif

#define optional_OUTPUT_SIZEOF( type ) \
    "sizeof( optional<" #type "> ): " << \
     sizeof( optional<   type>   )    << " (" << sizeof(type) << ")\n" <<

CASE("Show sizeof various optionals" 
     "[.]" )
{
    std::cout << 
        "sizeof( nonstd::optional_detail::storage_t<char> ): " << 
         sizeof( nonstd::optional_detail::storage_t<char> )    << "\n" << 
         optional_OUTPUT_SIZEOF( char )
         optional_OUTPUT_SIZEOF( short )
         optional_OUTPUT_SIZEOF( int )
         optional_OUTPUT_SIZEOF( long )
         optional_OUTPUT_SIZEOF( float )
         optional_OUTPUT_SIZEOF( double )
         optional_OUTPUT_SIZEOF( long double )
         optional_OUTPUT_SIZEOF( S )
         "";
}
#undef optional_OUTPUT_SIZEOF

/*
 * Test driver:
 */

int main( int argc, char * argv[] )
{
    return lest::run( specification, argc, argv );
}

#if 0
cl -nologo -W3 -EHsc optional.t.cpp && optional.t
cl -nologo -Wall -EHsc optional.t.cpp && optional.t

g++ -Wall -Wextra -std=c++03 -o optional.t.exe optional.t.cpp && optional.t
g++ -Wall -Wextra -std=c++11 -o optional.t.exe optional.t.cpp && optional.t
g++ -Wall -Wextra -std=c++1y -o optional.t.exe optional.t.cpp && optional.t

#endif // 0
