// Copyright 2014-4 sxniu
#ifndef  INCLUDE_UTILS_H_
#define  INCLUDE_UTILS_H_

#include <vector>
#include <math.h>

typedef unsigned char uchar;

#define EIGHT_ARROUND_POSITION(center_x, center_y, width, height) \
{ \
  center_y * width + std::min(center_x + 1, width - 1), \
  std::min(center_y + 1, height - 1) * width + std::min(center_x + 1, width - 1), \
  std::min(center_y + 1, height - 1) * width + center_x, \
  std::min(center_y + 1, height - 1) * width + std::max(center_x - 1, 0), \
  center_y * width + std::max(center_x - 1, 0), \
  std::max(center_y - 1, 0) * width + std::max(center_x - 1, 0), \
  std::max(center_y - 1, 0) * width + center_x, \
  std::max(center_y - 1, 0) * width + std::min(center_x + 1, width - 1), \
}

#define FOUR_ARROUND_POSITION(center_x, center_y, width, height) \
{ \
  center_y * width + std::max(center_x - 1, 0), \
  center_y * width + std::min(center_x + 1, width - 1), \
  std::max(center_y - 1, 0) * width + center_x, \
  std::min(center_y + 1, height - 1) * width + center_x, \
}

#define GET_THREE_COORDINATE(colour) \
{ \
  (colour & RED) >> 16, \
  (colour & GREEN) >> 8, \
  colour & BLUE \
}

#define TURN_COORDINATE_TO_COLOUR(x, y, z) ((x << 16) + (y << 8) + z)

#define GET_DISTENCE(d, s) \
{ \
  abs(d[0] - s[0]), \
  abs(d[1] - s[1]), \
  abs(d[2] - s[2]), \
}

#define GET_DIFFERENCE(d, s) \
{ \
  d[0] - s[0], \
  d[1] - s[1], \
  d[2] - s[2], \
}

template <class T>
class ImageData;

namespace utils {

void ReadImage(const char* file_name, ImageData<int>* image_data);
void TurnGray(const ImageData<int>& input_image, ImageData<uchar>* gray_image);
void SaveImage(const char* out_file_name, const ImageData<int>& image_data);
void SaveImage(const char* out_file_name, const ImageData<uchar>& image_data);
void GetGradiendMap(const ImageData<uchar>& gray_image, ImageData<uchar>* grad_image);
void MarkConnectedArea(const ImageData<uchar>& grad_image, ImageData<int>* marked_image,
                       int max_threshold);
void Watershed(const ImageData<uchar>& grad_image, ImageData<int>* marked_image,
               int start_gradient);

}  // namespace utils

#endif  // INCLUDE_UTILS_H_
