#include "WDPConfig.h"

#include "wdp/Depth.h"

#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include <iostream>

#define STR(X) #X
#define XSTR(X) STR(X)

namespace bpo = boost::program_options;
namespace bg = boost::gil;

int main(int argc, char **argv) {
  std::string ImageDir;
  std::size_t X;
  std::size_t Y;
  double CameraDisplacement;

  // Declare program options.
  bpo::options_description Desc("Depth perception application options");
  Desc.add_options()
      ("help", "produce help message")
      ("imagedir", bpo::value<std::string>(&ImageDir),
       "name of the directory within 'data/' containing stereo images, named "
       "lhs.jpg and rhs.jpg")
      ("x", bpo::value<std::size_t>(&X),
       "x coordinate of pixel for which depth should be determined")
      ("y", bpo::value<std::size_t>(&Y),
       "y coordinate of pixel for which depth should be determined")
      ("displacement",
       bpo::value<double>(&CameraDisplacement)->default_value(100.0),
       "Displacement between the two images");

  bpo::variables_map VM;
  bpo::store(bpo::command_line_parser(argc, argv).options(Desc).run(), VM);
  bpo::notify(VM);

  if (VM.count("help")) {
    std::cout << Desc << std::endl;
    return 1;
  }

  if (!VM.count("imagedir")) {
    std::cout << "Image directory not set - see help for details" << std::endl;
    return 1;
  }

  if (!VM.count("x")) {
    std::cout << "X coordinate not set - see help for details" << std::endl;
    return 1;
  }

  if (!VM.count("y")) {
    std::cout << "Y coordinate not set - see help for details" << std::endl;
    return 1;
  }

  std::string LHSFilename =
      std::string(XSTR(WDP_DATA_DIR)).append(ImageDir).append("/lhs.jpg");
  std::string RHSFilename =
      std::string(XSTR(WDP_DATA_DIR)).append(ImageDir).append("/rhs.jpg");
  bg::rgb8_image_t LHSImg;
  bg::rgb8_image_t RHSImg;
  bg::jpeg_read_image(LHSFilename, LHSImg);
  bg::jpeg_read_image(RHSFilename, RHSImg);

  wdp::Coordinates C(X, Y);

  wdp::Radii NeighbourhoodRadii(25, 25);
  wdp::Offset SearchOffset(-20, 0);
  wdp::Radii SearchRadii(1000, 1000);

  wdp::SearchParameters SearchParams = {NeighbourhoodRadii, SearchOffset,
                                        SearchRadii, 25000UL};

  wdp::DepthParameters DepthParams = {125.0, 0.05, CameraDisplacement};

  auto Depth = wdp::getDepth(LHSImg, RHSImg, C, SearchParams, DepthParams);

  std::cout << Depth << std::endl;
  return 0;
}
