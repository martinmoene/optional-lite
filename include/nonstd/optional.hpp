//
// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/optional-lite
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once

#ifndef NONSTD_OPTIONAL_LITE_HPP
#define NONSTD_OPTIONAL_LITE_HPP

#include <cassert>
#include <stdexcept>
#include <utility>

#define  optional_lite_VERSION "2.x.x"

#if ( __cplusplus >= 201103L )
# define optional_CPP11_OR_GREATER  1
#endif

#if optional_CPP11_OR_GREATER
# include <type_traits>
#endif

#ifndef  optional_FEATURE_MAX_ALIGN_HACK
# define optional_FEATURE_MAX_ALIGN_HACK  0
#endif

#ifndef optional_FEATURE_ALIGN_AS
// used in #if defined(), so no default...
#endif

#ifndef  optional_FEATURE_ALIGN_AS_FALLBACK
# define optional_FEATURE_ALIGN_AS_FALLBACK  double
#endif

namespace nonstd { namespace optional_lite {

/// class optional

template< typename T >
class optional;

namespace detail {

#if optional_FEATURE_MAX_ALIGN_HACK

// Max align, use most restricted type for alignment:

#define optional_UNIQUE(  name )       optional_UNIQUE2( name, __LINE__ )
#define optional_UNIQUE2( name, line ) optional_UNIQUE3( name, line )
#define optional_UNIQUE3( name, line ) name ## line

#define optional_ALIGN_TYPE( type ) \
    type optional_UNIQUE( _t ); struct_t< type > optional_UNIQUE( _st )

template< typename T >
struct struct_t { T _; };

union max_align_t
{
    optional_ALIGN_TYPE( char );
    optional_ALIGN_TYPE( short int );
    optional_ALIGN_TYPE( int );
    optional_ALIGN_TYPE( long int  );
    optional_ALIGN_TYPE( float  );
    optional_ALIGN_TYPE( double );
    optional_ALIGN_TYPE( long double );
    optional_ALIGN_TYPE( char * );
    optional_ALIGN_TYPE( short int * );
    optional_ALIGN_TYPE( int *  );
    optional_ALIGN_TYPE( long int * );
    optional_ALIGN_TYPE( float * );
    optional_ALIGN_TYPE( double * );
    optional_ALIGN_TYPE( long double * );
    optional_ALIGN_TYPE( void * );

#ifdef HAVE_LONG_LONG
    optional_ALIGN_TYPE( long long );
#endif

    struct Unknown;

    Unknown ( * optional_UNIQUE(_) )( Unknown );
    Unknown * Unknown::* optional_UNIQUE(_);
    Unknown ( Unknown::* optional_UNIQUE(_) )( Unknown );

    struct_t< Unknown ( * )( Unknown)         > optional_UNIQUE(_);
    struct_t< Unknown * Unknown::*            > optional_UNIQUE(_);
    struct_t< Unknown ( Unknown::* )(Unknown) > optional_UNIQUE(_);
};

#undef optional_UNIQUE
#undef optional_UNIQUE2
#undef optional_UNIQUE3

#undef optional_ALIGN_TYPE

#elif defined( optional_FEATURE_ALIGN_AS ) // optional_FEATURE_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define optional_ALIGN_AS( unused ) \
    optional_FEATURE_ALIGN_AS

#else // optional_FEATURE_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define optional_ALIGN_AS( to_align ) \
    typename type_of_size< alignment_types, alignment_of< to_align >::value >::type

template < bool condition, typename Then, typename Else > struct select;
template < typename Then, typename Else > struct select< true , Then, Else > { typedef Then type; };
template < typename Then, typename Else > struct select< false, Then, Else > { typedef Else type; };

template <typename T>
struct alignment_of;

template <typename T>
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{
    enum { value = A < S ? A : S };
};

template< typename T >
struct alignment_of
{
    enum { value = alignment_logic<
        sizeof( alignment_of_hack<T> ) - sizeof(T), sizeof(T) >::value, };
};

struct nulltype{};

template< typename Head, typename Tail >
struct typelist
{
    typedef Head head;
    typedef Tail tail;
};

template< typename List, size_t N >
struct type_of_size
{
    typedef typename select<
        N == sizeof( typename List::head ),
            typename List::head,
            typename type_of_size<typename List::tail, N >::type >::type type;
};

template< size_t N >
struct type_of_size< nulltype, N >
{
    typedef optional_FEATURE_ALIGN_AS_FALLBACK type;
};

template< typename T>
struct struct_t { T _; };

#define optional_ALIGN_TYPE( type ) \
    typelist< type , typelist< struct_t< type >

struct Unknown;

typedef
    optional_ALIGN_TYPE( char ),
    optional_ALIGN_TYPE( short ),
    optional_ALIGN_TYPE( int ),
    optional_ALIGN_TYPE( long ),
    optional_ALIGN_TYPE( float ),
    optional_ALIGN_TYPE( double ),
    optional_ALIGN_TYPE( long double ),

    optional_ALIGN_TYPE( char *),
    optional_ALIGN_TYPE( short * ),
    optional_ALIGN_TYPE( int * ),
    optional_ALIGN_TYPE( long * ),
    optional_ALIGN_TYPE( float * ),
    optional_ALIGN_TYPE( double * ),
    optional_ALIGN_TYPE( long double * ),

    optional_ALIGN_TYPE( Unknown ( * )( Unknown ) ),
    optional_ALIGN_TYPE( Unknown * Unknown::*     ),
    optional_ALIGN_TYPE( Unknown ( Unknown::* )( Unknown ) ),

    nulltype
    > > > > > > >    > > > > > > >
    > > > > > > >    > > > > > > >
    > > > > > >
    alignment_types;

#undef optional_ALIGN_TYPE

#endif // optional_FEATURE_MAX_ALIGN_HACK

/// C++03 constructed union to hold value.

template< typename T >
union storage_t
{
private:
    friend class optional<T>;

    typedef T value_type;

    storage_t() {}

    storage_t( value_type const & v )
    {
        construct_value( v );
    }

    void construct_value( value_type const & v )
    {
        ::new( value_ptr() ) value_type( v );
    }

    void destruct_value()
    {
        // Note: VC6 requires the use of the
        // template parameter T (cannot use value_type).
        value_ptr()->~T();
    }

    value_type * value_ptr() const
    {
        return as( (value_type*)0 );
    }

    value_type * value_ptr()
    {
        return as( (value_type*)0 );
    }

    value_type const & value() const
    {
        return * value_ptr();
    }

    value_type & value()
    {
        return * value_ptr();
    }

#if optional_CPP11_OR_GREATER

    using aligned_storage_t = typename std::aligned_storage< sizeof(value_type), alignof(value_type) >::type;
    aligned_storage_t buffer;

#elif optional_FEATURE_MAX_ALIGN_HACK

    typedef struct { unsigned char data[ sizeof(value_type) ]; } aligned_storage_t;

    max_align_t hack;
    aligned_storage_t buffer;

#else
    typedef optional_ALIGN_AS(value_type) align_as_type;

    typedef struct { align_as_type data[ 1 + ( sizeof(value_type) - 1 ) / sizeof(align_as_type) ]; } aligned_storage_t;
    aligned_storage_t buffer;

#   undef optional_ALIGN_AS

#endif // optional_FEATURE_MAX_ALIGN_HACK

    // Note: VC6 cannot handle as<T>():

    template <typename U>
    U * as( U* ) const
    {
        return reinterpret_cast<U*>( const_cast<aligned_storage_t *>( &buffer ) );
    }
};

} // namespace detail

/// disengaged state tag

struct nullopt_t
{
    struct init{};
    nullopt_t( init ) {}
};

// extra parenthesis to prevent the most vexing parse:
const nullopt_t nullopt( ( nullopt_t::init() ) );

/// optional access error

class bad_optional_access : public std::logic_error
{
public:
  explicit bad_optional_access( char const * const what_arg )
  : logic_error( what_arg ) {}
};

/// optional

template< typename T>
class optional
{
private:
    typedef void (optional::*safe_bool)() const;

public:
    typedef T value_type;

    optional()
    : has_value( false )
     ,contained()
    {}

    optional( nullopt_t )
    : has_value( false )
    , contained()
    {}

    optional( value_type const & value )
    : has_value( true )
    , contained( value )
    {}

    optional( optional const & other )
    : has_value( other.initialized() )
    {
        if ( other.initialized() )
            contained.construct_value( other.contained.value() );
    }

    ~optional()
    {
        if ( initialized() )
            contained.destruct_value();
    }

    operator safe_bool() const
    {
        return initialized() ? &optional::this_type_does_not_support_comparisons : 0;
    }

    optional & operator=( nullopt_t )
    {
        clear();
        return *this;
    }

    optional & operator=( optional const & rhs )
    {
        if      ( initialized() == true  && rhs.initialized() == false ) clear();
        else if ( initialized() == false && rhs.initialized() == true  ) initialize( *rhs );
        else if ( initialized() == true  && rhs.initialized() == true  ) contained.value() = *rhs;
        return *this;
    }

    void swap( optional & other )
    {
        using std::swap;
        if      ( initialized() == true  && other.initialized() == true  ) { swap( **this, *other ); }
        else if ( initialized() == false && other.initialized() == true  ) { initialize( *other ); other.clear(); }
        else if ( initialized() == true  && other.initialized() == false ) { other.initialize( **this ); clear(); }
    }

    // observers

    value_type const * operator ->() const
    {
        assert( initialized() );
        return contained.value_ptr();
    }

    value_type * operator ->()
    {
        assert( initialized() );
        return contained.value_ptr();
    }

    value_type const & operator *() const
    {
        assert( initialized() );
        return contained.value();
    }

    value_type & operator *()
    {
        assert( initialized() );
        return contained.value();
    }

    value_type const & value() const
    {
        if ( ! initialized() )
            throw bad_optional_access( "accessing value of disengaged optional (const)" );

        return contained.value();
    }

    value_type & value()
    {
        if ( ! initialized() )
            throw bad_optional_access( "accessing value of disengaged optional (non-const)" );

        return contained.value();
    }

    value_type value_or( value_type const & default_value ) const
    {
        return initialized() ? contained.value() : default_value;
    }

private:
    void this_type_does_not_support_comparisons() const {}

    bool initialized() const
    {
        return has_value;
    }

    template< typename V >
    void initialize( V const & value)
    {
        assert( ! initialized()  );
        contained.construct_value( value );
        has_value = true;
    }

    void clear()
    {
        if ( initialized() )
            contained.destruct_value();

        has_value = false;
    }

private:
    bool has_value;
    detail::storage_t< value_type > contained;

};

// Relational operators

template< typename T > bool operator==( optional<T> const & x, optional<T> const & y )
{
    return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template< typename T > bool operator!=( optional<T> const & x, optional<T> const & y )
{
    return !(x == y);
}

template< typename T > bool operator<( optional<T> const & x, optional<T> const & y )
{
    return (!y) ? false : (!x) ? true : *x < *y;
}

template< typename T > bool operator>( optional<T> const & x, optional<T> const & y )
{
    return (y < x);
}

template< typename T > bool operator<=( optional<T> const & x, optional<T> const & y )
{
    return !(y < x);
}

template< typename T > bool operator>=( optional<T> const & x, optional<T> const & y )
{
    return !(x < y);
}

// Comparison with nullopt

template< typename T > bool operator==( optional<T> const & x, nullopt_t )
{
    return (!x);
}

template< typename T > bool operator==( nullopt_t, optional<T> const & x )
{
    return (!x);
}

template< typename T > bool operator!=( optional<T> const & x, nullopt_t )
{
    return bool(x);
}

template< typename T > bool operator!=( nullopt_t, optional<T> const & x )
{
    return bool(x);
}

template< typename T > bool operator<( optional<T> const &, nullopt_t )
{
    return false;
}

template< typename T > bool operator<( nullopt_t, optional<T> const & x )
{
    return bool(x);
}

template< typename T > bool operator<=( optional<T> const & x, nullopt_t )
{
    return (!x);
}

template< typename T > bool operator<=( nullopt_t, optional<T> const & )
{
    return true;
}

template< typename T > bool operator>( optional<T> const & x, nullopt_t )
{
    return bool(x);
}

template< typename T > bool operator>( nullopt_t, optional<T> const & )
{
    return false;
}

template< typename T > bool operator>=( optional<T> const &, nullopt_t )
{
    return true;
}

template< typename T > bool operator>=( nullopt_t, optional<T> const & x )
{
    return (!x);
}

// Comparison with T

template< typename T > bool operator==( optional<T> const & x, const T& v )
{
    return bool(x) ? *x == v : false;
}

template< typename T > bool operator==( T const & v, optional<T> const & x )
{
    return bool(x) ? v == *x : false;
}

template< typename T > bool operator!=( optional<T> const & x, const T& v )
{
    return bool(x) ? *x != v : true;
}

template< typename T > bool operator!=( T const & v, optional<T> const & x )
{
    return bool(x) ? v != *x : true;
}

template< typename T > bool operator<( optional<T> const & x, const T& v )
{
    return bool(x) ? *x < v : true;
}

template< typename T > bool operator<( T const & v, optional<T> const & x )
{
    return bool(x) ? v < *x : false;
}

template< typename T > bool operator<=( optional<T> const & x, const T& v )
{
    return bool(x) ? *x <= v : true;
}

template< typename T > bool operator<=( T const & v, optional<T> const & x )
{
    return bool(x) ? v <= *x : false;
}

template< typename T > bool operator>( optional<T> const & x, const T& v )
{
    return bool(x) ? *x > v : false;
}

template< typename T > bool operator>( T const & v, optional<T> const & x )
{
    return bool(x) ? v > *x : true;
}

template< typename T > bool operator>=( optional<T> const & x, const T& v )
{
    return bool(x) ? *x >= v : false;
}

template< typename T > bool operator>=( T const & v, optional<T> const & x )
{
    return bool(x) ? v >= *x : true;
}

// Specialized algorithms

template< typename T >
void swap( optional<T> & x, optional<T> & y )
{
    x.swap( y );
}

template< typename T >
optional<T> make_optional( T const & v )
{
    return optional<T>( v );
}

} // namespace optional

using namespace optional_lite;

} // namespace nonstd

#endif // NONSTD_OPTIONAL_LITE_HPP
