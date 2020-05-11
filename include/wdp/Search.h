//===-------- Search.h - Block Searching Algorithms -------------*- C++ -*-===//
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
// This file defines functions which implement block search over image frames.
//
//===----------------------------------------------------------------------===//

#ifndef WDP_SEARCH_H
#define WDP_SEARCH_H

#include <boost/concept_check.hpp>
#include <boost/gil/image.hpp>

#include <cstddef>

namespace wdp {

/// Calculate the sum of squared differences between the pixels in two blocks.
template <typename LHSView, typename RHSView>
std::size_t sumOfSquaredDifferences(const LHSView &LHS, const RHSView &RHS) {
  // Input arguments must be two compatible 2D image views.
  boost::function_requires<
      boost::gil::RandomAccess2DImageViewConcept<LHSView>>();
  boost::function_requires<
      boost::gil::RandomAccess2DImageViewConcept<RHSView>>();

  boost::function_requires<
      boost::gil::ViewsCompatibleConcept<LHSView, RHSView>>();

  // Can only operate on views of equal dimensions.
  assert(LHS.dimensions() == RHS.dimensions());

  // TODO: Consider parallelisation.
  std::size_t Sum = 0;
  for (std::size_t Y = 0; Y < LHS.height(); ++Y) {
    typename LHSView::x_iterator LHSIt = LHS.row_begin(Y);
    typename RHSView::x_iterator RHSIt = RHS.row_begin(Y);
    for (std::size_t X = 0; X < LHS.width(); ++X) {
      for (std::size_t C = 0; C < boost::gil::num_channels<LHSView>::value;
           ++C) {
        const auto Difference = LHSIt[X][C] - RHSIt[X][C];
        Sum += Difference * Difference;
      }
    }
  }
  return Sum;
}

/// Get a block of pixels with given radii surrounding the given coordinates.
template <typename Image, typename Coordinates, typename Radii>
const typename Image::view_t getBlock(const Image &Img, const Coordinates &C,
                                      const Radii &R) {
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<Image>>();
  boost::function_requires<boost::gil::Point2DConcept<Coordinates>>();
  boost::function_requires<boost::gil::Point2DConcept<Radii>>();

  typedef typename Image::view_t View;
  typedef typename Image::point_t Point;
  typedef typename View::xy_locator Locator;

  // Create the dimensions of the block.
  const Point Dimensions(R.x * 2 + 1, R.y * 2 + 1);

  // Get a locator for the beginning of the block.
  const Point Begin(C.x - R.x, C.y - R.y);
  const Locator &Loc = Img._view.xy_at(Begin);

  // Construct a new view into the image.
  return View(Dimensions, Loc);
}

} // namespace wdp

#endif // WDP_SEARCH_H
