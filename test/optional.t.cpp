// Copyright 2014-2016 by Martin Moene
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// optional lite is inspired on std::optional by Fernando Cacciola and Andrzej Krzemienski
// and on expected lite by Martin Moene.

#include "optional-lite.t.h"

using nonstd::optional;
using nonstd::nullopt;
using nonstd::bad_optional_access;
using nonstd::make_optional;

struct nonpod { nonpod(){} };

CASE( "union: A C++03 union can only contain POD types" )
{
    union U
    {
        char c;
#if optional_CPP11_OR_GREATER
        nonpod np;
#endif
    };
}

//
// Positive tests:
//

CASE( "optional: A default constructed optional is empty" )
{
    optional<int> oi;
    EXPECT( !oi );
}

CASE( "optional: An optional that is constructed disengaged explicitly is empty" )
{
    optional<int> oi( nullopt );
    EXPECT( !oi );
}

CASE( "optional: An optional constructed with 42 contains 42" )
{
    optional<int> oi = 42;
    EXPECT(  oi );
    EXPECT( *oi == 42 );
}

CASE( "optional: An optional constructed from an empty optional is empty" )
{
    optional<int> oi1;
    optional<int> oi2( oi1 );
    EXPECT( !oi2 );
}

CASE( "optional: An optional constructed from an non-empty optional obtains its value" )
{
    optional<int> oi1( 42 );
    optional<int> oi2( oi1 );
    EXPECT(  oi2 );
    EXPECT( *oi2 == *oi1 );
}

CASE( "optional: copy-assignment works for all permutations of engaged and disengaged optionals" )
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
        EXPECT(  d1 );
        EXPECT( *d1 == 123 );
    }
    SECTION( "an engaged optional assigned an engaged optional obtains its value" ) {
        e1 = e2;
        EXPECT(  e1 );
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

CASE( "optional: Member swap() swaps engage state and values" )
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
        EXPECT(  e1  );
        EXPECT(  e2 );
        EXPECT( *e1 == 7 );
        EXPECT( *e2 == 42 );
    }
    SECTION( "swap disengaged with engaged optional" ) {
        d1.swap( e1 );
        EXPECT(  d1 );
        EXPECT( !e1 );
        EXPECT( *d1 == 42 );
    }
    SECTION( "swap engaged with disengaged optional" ) {
        e1.swap( d1 );
        EXPECT(  d1 );
        EXPECT( !e1 );
        EXPECT( *d1 == 42 );
    }}
}

struct Integer { int x; Integer(int x) : x(x) {} };

CASE( "optional: operator->() yields pointer to value" )
{
    SETUP( "" ) {
        optional<Integer> e( Integer( 42 ) );

    SECTION( "operator->() yields pointer to value (const)" ) {
        EXPECT(  e->x == 42 );
    }
    SECTION( "operator->() yields pointer to value (non-const)" ) {
        e->x = 7;
        EXPECT(  e->x == 7 );
    }}
}

CASE( "optional: operator*() yields value" )
{
    SETUP( "" ) {
        optional<int> e( 42 );

    SECTION( "operator*() yields value (const)" ) {
        EXPECT( *e == 42 );
    }
    SECTION( "operator*() yields value (non-const)" ) {
        *e = 7;
        EXPECT( *e == 7 );
    }}
}

CASE( "optional: value() yields value" )
{
    SETUP( "" ) {
        optional<int> e( 42 );

    SECTION( "value() yields value (const)" ) {
        EXPECT( e.value() == 42 );
    }
    SECTION( "value() yields value (non-const)" ) {
        e.value() = 7;
        EXPECT( e.value() == 7 );
    }}
}

CASE( "optional: value() throws bad_optional_access at disengaged access" )
{
    EXPECT_THROWS_AS( optional<int>().value(), bad_optional_access );
}

CASE( "optional: value_or() yields value or default" )
{
    SETUP( "" ) {
        optional<int> d;
        optional<int> e( 42 );

    SECTION( "value_or( 7 ) yields value for non-empty optional" ) {
        EXPECT( e.value_or( 7 ) == 42 );
    }
    SECTION( "value_or( 7 ) yields default for empty optional" ) {
        EXPECT( d.value_or( 7 ) == 7 );
    }}
}

CASE( "optional: Global swap() swaps engage state and values" )
{
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
        EXPECT(  e1  );
        EXPECT(  e2 );
        EXPECT( *e1 == 7 );
        EXPECT( *e2 == 42 );
    }
    SECTION( "swap disengaged with engaged optional" ) {
        swap( d1, e1 );
        EXPECT(  d1 );
        EXPECT( !e1 );
        EXPECT( *d1 == 42 );
    }
    SECTION( "swap engaged with disengaged optional" ) {
        swap( e1, d1 );
        EXPECT(  d1 );
        EXPECT( !e1 );
        EXPECT( *d1 == 42 );
    }}
}

CASE( "optional: make_optional() creates optional" )
{
    EXPECT( *make_optional( 42 ) == 42 );
}

CASE( "optional: Relational operators" )
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

//
// Negative tests:
//

//
// Tests that print information:
//

struct S{ S(){} };

#if !optional_FEATURE_MAX_ALIGN_HACK

#define optional_OUTPUT_ALIGNMENT_OF( type ) \
    "alignment_of<" #type ">: " <<  \
     alignment_of<type>::value  << "\n" <<

CASE("alignment_of: Show alignment of various types"
     "[.]" )
{
#if optional_CPP11_OR_GREATER
    using std::alignment_of;
#else
    using ::nonstd::optional_lite::detail::alignment_of;
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

CASE("storage_t: Show sizeof various optionals"
     "[.]" )
{
    std::cout <<
        "sizeof( nonstd::optional_lite::detail::storage_t<char> ): " <<
         sizeof( nonstd::optional_lite::detail::storage_t<char> )    << "\n" <<
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

//
// Issues:
//

CASE( "optional: isocpp-lib: CH 3, p0032r2 -- let's not have too clever tags" "[.issue #1]" )
{
    EXPECT( false );
//    optional< optional< optional<int> > > o (
//        in_place<int>,
//#if 0
//        in_place<int>,
//#else
////        nonstd_lite_in_place_type_t(int),
//        static_cast< nonstd::in_place_t >( in_place<int> ),
//#endif
//        nullopt
//    );
//
//    EXPECT(       o );
//    EXPECT(      *o );
//    EXPECT_NOT( **o );
}

// end of file
