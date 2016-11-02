#include "any.hpp"
#include "optional.hpp"

#include <cassert>
#include <string>

using namespace nonstd;

int main()
{
    std::string hello = "hello, world";
    {
        any var;

        var =  'v' ; assert( any_cast<char>( var ) == 'v' );
        var =   7  ; assert( any_cast<int >( var ) ==  7  );
        var =  42L ; assert( any_cast<long>( var ) == 42L );
        var = hello; assert( any_cast<std::string>( var ) == hello );
    }{
        optional< int > var;

//        var =  'v' ; assert( get<  0 >( var ) == 'v' );
//        var =  'v' ; assert( get<char>( var ) == 'v' );
//        var =   7  ; assert( get<int >( var ) ==  7  );
//        var =  42L ; assert( get<long>( var ) == 42L );
//        var = hello; assert( get<std::string>( var ) == hello );
    }
}

// cl -nologo -EHsc -I../include/nonstd -I../../any-lite/include/nonstd 04-any-optional.cpp && 04-any-optional
// g++ -Wall -I../include/nonstd -I../../any-lite/include/nonstd -o 04-any-optional 04-any-optional.cpp && 04-any-optional
