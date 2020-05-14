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

/// Calculate the sum of squared differences between the pixels in two
/// neighbourhoods of equal size.
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

/// Get the neighbourhood of pixels which surround the given coordinates within
/// the given radii.
template <typename Image, typename Coordinates, typename Radii>
const typename Image::view_t
getNeighbourhood(const Image &Img, const Coordinates NeighbourhoodCoords,
                 const Radii NeighbourhoodRadii) {
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<Image>>();
  boost::function_requires<boost::gil::Point2DConcept<Coordinates>>();
  boost::function_requires<boost::gil::Point2DConcept<Radii>>();

  // Check that the neighbourhood would be within the boundaries of the image.
  assert(NeighbourhoodCoords.x - NeighbourhoodRadii.x >= 0);
  assert(NeighbourhoodCoords.y - NeighbourhoodRadii.y >= 0);
  assert(NeighbourhoodCoords.x + NeighbourhoodRadii.x < Img.width());
  assert(NeighbourhoodCoords.y + NeighbourhoodRadii.y < Img.height());

  typedef typename Image::view_t View;
  typedef typename Image::point_t Point;
  typedef typename View::xy_locator Locator;

  // Calculate the dimensions of the neighbourhood.
  const Point Dimensions(NeighbourhoodRadii.x * 2 + 1,
                         NeighbourhoodRadii.y * 2 + 1);

  // Get a locator for the beginning of the neighbourhood.
  const Point Begin(NeighbourhoodCoords.x - NeighbourhoodRadii.x,
                    NeighbourhoodCoords.y - NeighbourhoodRadii.y);
  const Locator &Loc = Img._view.xy_at(Begin);

  // Construct a new view into the image.
  return View(Dimensions, Loc);
}

// Find the neighbourhood in the RHS image which best matches a neighbourhood -
// defined by the given coordinates and radii - in the LHS image. Search within
// the given search radii.
template <typename LHSImage, typename RHSImage, typename Coordinates,
          typename Radii>
const Coordinates findBestMatch(const LHSImage &LHSImg, const RHSImage &RHSImg,
                                const Coordinates NeighbourhoodCoords,
                                const Radii NeighbourhoodRadii,
                                const Radii SearchRadii) {
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<LHSImage>>();
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<RHSImage>>();
  boost::function_requires<boost::gil::Point2DConcept<Coordinates>>();
  boost::function_requires<boost::gil::Point2DConcept<Radii>>();

  const auto LHS =
      getNeighbourhood(LHSImg, NeighbourhoodCoords, NeighbourhoodRadii);

  // Calculate the bounds of the search.
  const Coordinates Begin(NeighbourhoodCoords.x - SearchRadii.x,
                          NeighbourhoodCoords.y - SearchRadii.y);
  const Coordinates End(NeighbourhoodCoords.x + SearchRadii.x,
                        NeighbourhoodCoords.y + SearchRadii.y);

  // Search through neighbourhoods in the given search radii to find the lowest
  // sum of squared differences to the LHS neighbourhood.
  Coordinates MinCoords(NeighbourhoodCoords);
  std::size_t MinSSD = (std::size_t)-1;
  for (auto Y = Begin.y; Y <= End.y; ++Y) {
    for (auto X = Begin.x; X <= End.x; ++X) {
      Coordinates RHSCoords(X, Y);
      const auto RHS = getNeighbourhood(RHSImg, RHSCoords, NeighbourhoodRadii);
      std::size_t SSD = sumOfSquaredDifferences(LHS, RHS);
      if (SSD < MinSSD) {
        MinCoords = RHSCoords;
        MinSSD = SSD;
      }
    }
  }

  return MinCoords;
}

} // namespace wdp

#endif // WDP_SEARCH_H
