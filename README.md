### WHEEL.E Depth Perception

#### Background

Depth perception is a task required for the obstacle perception subsystem of the WHEEL.E autonomous wheelchair. This task was originally intended to be performed using the Intel Realsense D435 depth camera through the librealsense SDK.

To meet the needs of the test specification for the subsystem and to provide training data for any machine learning component of the subsystem - if applicable - the camera would be used to generate raw input data for which the behaviour of the subsystem is known. However, due to restrictions imposed on obtaining and prototyping with physical components, this camera could not be sourced.

In its place, this project simulates a depth camera by performing stereo depth perception on pairs of images, the results of which can be used as originally intended by the obstacle perception subsystem.

#### Specification

This project enables determining the depth of a pixel in metres away from the camera. For this purpose the following inputs are required:

- Two RGB images

- The coordinates of the pixel for which the depth should be determined

- The distance between each camera

The following outputs are expected:

- The depth in metres of the pixel at the given coordinates in the first image

When used as a library, further configuration is possible whereby the following values can be adjusted:

- The focal length of the camera

- The scale of an individual pixel

- The radii of the neighbourhood used in the search algorithm

- The offset at which to start the search for a matching neighbourhood

- The radii of the search space

- The maximum value of sum of squared differences for which two neighbourhoods can be considered similar

#### Design

The algorithm for this task is based on the concept of binocular parallax, which describes the shift in perspective on an object when comparing the 2D scene as viewed from two viewpoints. By relating this shift in perspective to the distance between the two viewpoints, the depth of the object can be estimated.

What this implies for an algorithm is that to determine depth for a scene, first the pixels in one image must be correlated with the other, to find out the motion of that 'object' between the two.

To determine matching pixels in both images the 'neighbourhood' of pixels surrounding a given pixel can be determined, and the best match within the second image can be found using a block search algorithm - such as those used in video compression for motion estimation.

The displacement between the original location of a pixel and its location in the second image can be used along with the distance between the cameras to triangulate the depth of that pixel.

Once depth information has been produced for pixels, this is written as a single channel image to be used as an input to collision detection algorithms.

#### Implementation

This project is implemented as a library with two main modules: 'Search' and 'Depth'. Headers are found in 'include/wdp' and sources in 'lib/'. Currently the library is a header-only implementation.

Functionality required to find matching neighbourhoods can be found in the 'Search' module, while the functionality to calculate depth information from the displacement of pixels can be found in the 'Depth' module.

This project utilizes the Boost library, providing additional standard-library-style functionality, primarily the ability to load and operate on images.

#### Testing
