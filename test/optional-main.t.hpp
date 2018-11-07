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

#include <iosfwd>
namespace lest { template<typename T> std::ostream & operator<<( std::ostream & os, nonstd::optional<T> const & v ); }

#include "lest_cpp03.hpp"

using namespace nonstd;

#define CASE( name ) lest_CASE( specification(), name )

extern lest::tests & specification();

namespace lest {

template< typename T >
inline std::ostream & operator<<( std::ostream & os, nonstd::optional<T> const & v )
{
    using lest::to_string;
    return os << "[optional:" << (v ? to_string(*v) : "[empty]") << "]";
}

} // namespace lest

#endif // TEST_OPTIONAL_LITE_H_INCLUDED

// end of file
