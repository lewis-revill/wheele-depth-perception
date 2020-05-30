#include "WDPConfig.h"

#include "wdp/Depth.h"

#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>

#include <cmath>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>

#define STR(X) #X
#define XSTR(X) STR(X)

namespace bpo = boost::program_options;
namespace bg = boost::gil;

namespace fs = std::experimental::filesystem;

bool approxEqual(double a, double b) { return std::abs(a - b) < 25.0; }

int main(int argc, char **argv) {
  std::vector<std::string> Tests;

  // Declare program options.
  bpo::options_description Desc("Depth perception application options");
  Desc.add_options()("help", "produce help message")(
      "all", "run all tests in test directory")(
      "tests", bpo::value<std::vector<std::string>>(&Tests),
      "which tests to run");

  bpo::variables_map VM;
  bpo::store(bpo::command_line_parser(argc, argv).options(Desc).run(), VM);
  bpo::notify(VM);

  if (VM.count("help")) {
    std::cout << Desc << std::endl;
    return 1;
  }

  if (VM.count("all")) {
    if (VM.count("tests")) {
      std::cout << "All tests specified but --tests provided" << std::endl;
      return 1;
    }
    for (const auto &File : fs::directory_iterator(XSTR(WDP_TEST_DIR)))
      Tests.push_back(File.path().filename());
  } else if (!VM.count("tests")) {
    std::cout << "No tests specified - see help for details" << std::endl;
    return 1;
  }

  wdp::Radii NeighbourhoodRadii(15, 15);
  wdp::Offset SearchOffset(-250, 0);
  wdp::Radii SearchRadii(500, 250);

  wdp::SearchParameters SearchParams = {NeighbourhoodRadii, SearchOffset,
                                        SearchRadii, 1000000UL};

  wdp::DepthParameters DepthParams = {24.0, 0.00694, 100.0};

  for (const std::string &TestName : Tests) {
    auto LHSPath = fs::path(XSTR(WDP_DATA_DIR)) / TestName / "lhs.jpg";
    auto RHSPath = fs::path(XSTR(WDP_DATA_DIR)) / TestName / "rhs.jpg";
    bg::rgb8_image_t LHSImg;
    bg::rgb8_image_t RHSImg;
    bg::jpeg_read_image(LHSPath, LHSImg);
    bg::jpeg_read_image(RHSPath, RHSImg);

    auto TestPath = fs::path(XSTR(WDP_TEST_DIR)) / TestName;
    std::fstream TestFS(TestPath);

    std::size_t X, Y;
    double ExpectedDepth;
    while (TestFS >> X >> Y >> ExpectedDepth) {
      wdp::Coordinates C(X, Y);
      double Depth =
          wdp::getDepth(LHSImg, RHSImg, C, SearchParams, DepthParams);
      if (!approxEqual(Depth, ExpectedDepth))
        std::cout << "Failed depth check, expected " << ExpectedDepth << " at ["
                  << X << ", " << Y << "], got " << Depth << std::endl;
    }
  }

  return 0;
}
