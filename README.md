### WHEEL.E Depth Perception

#### Background

Depth perception is a task required for the obstacle perception subsystem of the WHEEL.E autonomous wheelchair. This task was originally intended to be performed using the Intel Realsense D435 depth camera through the librealsense SDK.

To meet the needs of the test specification for the subsystem and to provide training data for any machine learning component of the subsystem - if applicable - the camera would be used to generate raw input data for which the behaviour of the subsystem is known. However, due to restrictions imposed on obtaining and prototyping with physical components, this camera could not be sourced.

In its place, this project simulates a depth camera by performing stereo depth perception on pairs of images, the results of which can be used as originally intended by the obstacle perception subsystem.

#### Specification

The following inputs are required:

- Two RGB images.

These images should ideally be taken by cameras aligned parallel to each other on the horizontal axis. So long as there is no movement in the frame, using two images taken from the same camera should suffice.

- The distance between each camera.

This distance is used within the depth perception algorithm to triangulate depth and should be as accurate as possible. On the Intel Realsense camera this distance is approximately 53mm.

The following outputs are expected:

- A Grayscale image where pixel values represent the depth of pixels in the first image.

The scale of the depth will be dynamically determined from the maximum depth recognisable with the given distance between each camera.

#### Design

The algorithm for this task is based on the concept of binocular parallax, which describes the shift in perspective on an object when comparing the 2D scene as viewed from two viewpoints. By relating this shift in perspective to the distance between the two viewpoints, the depth of the object can be estimated.

What this implies for an algorithm is that to determine depth for a scene, first the pixels in one image must be correlated with the other, to find out the motion of that 'object' between the two.

To determine matching pixels in both images the 'neighbourhood' of pixels surrounding a given pixel can be determined, and the best match within the second image can be found using a block search algorithm - such as those used in video compression for motion estimation.

The displacement between the original location of a pixel and its location in the second image can be used along with the distance between the cameras to triangulate the depth of that pixel.

Once depth information has been produced for pixels, this is written as a single channel image to be used as an input to collision detection algorithms.

#### Implementation

This project is implemented as a library with two main modules: 'Search' and 'Depth'. Headers are found in 'include/wdp' and sources in 'lib/'.

Functionality required to find matching neighbourhoods can be found in the 'Search' module, while the functionality to calculate depth information from the displacement of pixels can be found in the 'Depth' module.

This project utilizes the Boost library, providing additional standard-library-style functionality, primarily the ability to load and operate on images.

#### Testing
