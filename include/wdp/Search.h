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
#include <boost/gil/gil_all.hpp>
#include <cstddef>

namespace wdp {

/// Calculate the sum of squared differences between the pixels in two blocks.
template<typename LHSView, typename RHSView>
std::size_t SumOfSquaredDifferences(const LHSView &LHS, const RHSView &RHS) {
  // Input arguments must be two compatible 2D image views.
  boost::function_requires<boost::gil::RandomAccess2DImageViewConcept<LHSView>>();
  boost::function_requires<boost::gil::RandomAccess2DImageViewConcept<RHSView>>();

  boost::function_requires<boost::gil::ViewsCompatibleConcept<LHSView, RHSView>>();

  // Can only operate on views of equal dimensions.
  assert(LHS.dimensions() == RHS.dimensions());

  // TODO: Consider parallelisation.
  std::size_t Sum = 0;
  for (std::ptrdiff_t Y = 0; Y < LHS.height(); ++Y) {
    auto LHSIt = LHS.row_begin(Y);
    auto RHSIt = RHS.row_begin(Y);
    for (std::ptrdiff_t X = 0; X < LHS.width(); ++X) {
      auto Difference = LHSIt[X] - RHSIt[X];
      Sum += Difference * Difference;
    }
  }
  return Sum;
}

} // namespace wdc

#endif // WDP_SEARCH_H
