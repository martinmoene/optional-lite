#include "any.hpp"
#include "optional.hpp"
#include "variant.hpp"

#include <cassert>
#include <string>

using namespace nonstd;

int main()
{
    std::string hello = "hello, world";
    {
        optional< int > var;

        assert( ! var );

        var =   7  ; assert( *var         ==  7  );
        var =   7  ; assert(  var.value() ==  7  );
    }{
        any var;
        
        assert( ! var.has_value() );

        var =  'v' ; assert( any_cast<char>( var ) == 'v' );
        var =   7  ; assert( any_cast<int >( var ) ==  7  );
        var =  42L ; assert( any_cast<long>( var ) == 42L );
        var = hello; assert( any_cast<std::string>( var ) == hello );
    }{
        variant< char, int, long, std::string > var;

        assert( ! var.valueless_by_exception() );

                     assert( get<  0 >( var ) == char() );
        var =  'v' ; assert( get<char>( var ) == 'v'    );
        var =   7  ; assert( get<int >( var ) ==  7     );
        var =  42L ; assert( get<long>( var ) == 42L    );
        var = hello; assert( get<std::string>( var ) == hello );
    }
}

// cl -nologo -EHsc -I../../any-lite/include/nonstd -I../../optional-lite/include/nonstd -I../../variant-lite/include/nonstd 04-any-optional-variant.cpp && 04-any-optional-variant
// g++ -Wall -I../../any-lite/include/nonstd -I../../optional-lite/include/nonstd -I../../variant-lite/include/nonstd -o 04-any-optional-variant 04-any-optional-variant.cpp && 04-any-optional-variant
