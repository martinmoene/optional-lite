// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/optional-lite
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef TEST_OPTIONAL_LITE_H_INCLUDED
#define TEST_OPTIONAL_LITE_H_INCLUDED

#include "optional.hpp"
#include "lest_cpp03.hpp"

using namespace nonstd;

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace nonstd { namespace optional_lite {

// use oparator<< instead of to_string() overload;
// see  http://stackoverflow.com/a/10651752/437272

template< typename T >
inline std::ostream & operator<<( std::ostream & os, optional<T> const & v )
{
    using lest::to_string;
    return os << "[optional:" << (v ? to_string(*v) : "[empty]") << "]";
}

}}

namespace lest {

using ::nonstd::optional_lite::operator<<;

} // namespace lest

#endif // TEST_OPTIONAL_LITE_H_INCLUDED

// end of file
