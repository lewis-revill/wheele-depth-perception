//===-------- Depth.h - Depth Perception Algorithms -------------*- C++ -*-===//
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
// This file defines functions which implement extraction of depth information
// from stereo images.
//
//===----------------------------------------------------------------------===//

#ifndef WDP_DEPTH_H
#define WDP_DEPTH_H

#include "Definitions.h"
#include "Search.h"

namespace wdp {

struct DepthParameters {
  double FocalLength;
  double PixelScale;
  double CameraDisplacement;
};

template <typename LHSImage, typename RHSImage>
double getDepth(const LHSImage &LHSImg, const RHSImage &RHSImg, Coordinates C,
                const SearchParameters &SearchParams,
                const DepthParameters &DepthParams) {
  // LHSImg and RHSImg input arguments must be 2D images.
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<LHSImage>>();
  boost::function_requires<boost::gil::RandomAccess2DImageConcept<RHSImage>>();

  const Offset O = getOffset(LHSImg, RHSImg, C, SearchParams);

  double TrueXOffset = -O.x * DepthParams.PixelScale;
  return DepthParams.FocalLength *
         (DepthParams.CameraDisplacement / TrueXOffset);
}

} // namespace wdp

#endif // WDP_DEPTH_H
