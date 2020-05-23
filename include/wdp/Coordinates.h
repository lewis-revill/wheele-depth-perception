//===-------- Coordinates.h - Coordinate Types and Functions ----*- C++ -*-===//
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
// This file defines types and functions surrounding coordinates used for
// indexing images.
//
//===----------------------------------------------------------------------===//

#ifndef WDP_COORDINATES_H
#define WDP_COORDINATES_H

#include <boost/concept_check.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/utilities.hpp>

#include <cstddef>

namespace wdp {

typedef boost::gil::point2<std::size_t> Coordinates;
typedef boost::gil::point2<std::ptrdiff_t> Offset;
typedef boost::gil::point2<std::size_t> Radii;

/// Get the offset from the centre of the image of the given coordinates.
template <typename Image>
Offset getCentreOffset(const Image &Img, Coordinates C) {
  // Input argument must be a 2D image.
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<Image>>();

  return Offset(C.x - Img.width() / 2, C.y - Img.height() / 2);
}

/// Get the coordinates at the given offset from the centre of the image.
template <typename Image>
Coordinates getCoordinates(const Image &Img, Offset CentreOffset) {
  // Input argument must be a 2D image.
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<Image>>();

  return Coordinates(Img.width() / 2 + CentreOffset.x,
                     Img.height() / 2 + CentreOffset.y);
}

} // namespace wdp

#endif // WDP_COORDINATES_H
