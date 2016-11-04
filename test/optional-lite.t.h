// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/optional-lite
//
// This code is licensed under the MIT License (MIT).

#pragma once

#ifndef TEST_ANY_LITE_H_INCLUDED
#define TEST_ANY_LITE_H_INCLUDED

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

using ::nonstd::operator<<;

} // namespace lest

#endif // TEST_ANY_LITE_H_INCLUDED

// end of file
