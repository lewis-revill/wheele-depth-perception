//===-------- Definitions.h - Type definitions -----------------*- C++ -*-===//
//
// Copyright © 2020 Lewis Revill
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt).
//
// SPDX-License-Identifier: BSL-1.0
//
//===----------------------------------------------------------------------===//
//
// This file defines types used in the search and depth modules.
//
//===----------------------------------------------------------------------===//

#ifndef WDP_DEFINITIONS_H
#define WDP_DEFINITIONS_H

#include <boost/gil/utilities.hpp>

namespace wdp {

typedef boost::gil::point2<std::size_t> Coordinates;
typedef boost::gil::point2<std::ptrdiff_t> Offset;
typedef boost::gil::point2<std::size_t> Radii;

} // namespace wdp

#endif // WDP_DEFINITIONS_H
