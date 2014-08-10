// Copyright 2014-4 sxniu
#include "include/utils.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <opencv/highgui.h>
#include <vector>
#include <stack>
#include <queue>

#include "include/colour.h"
#include "include/ImageData.h"

#define COMPONENTS 3
#define WASHED 0xffffffff
#define IN_QUEUE 0xffff0000

typedef unsigned char uchar;

namespace utils {

void ReadImage(const char* file_name, ImageData<int>* image_data) {
  if (!image_data->IsEmpty()) {
    printf("error: image data must be empty");
    return;
  }
  image_data->m_file_name = file_name;
  int& width = image_data->m_width;
  int& height = image_data->m_height;
  std::vector<int>* data = image_data->m_data;

  IplImage* cv_image = cvLoadImage(file_name);

  width = cv_image->width;
  height = cv_image->height;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int index = y * cv_image->widthStep + x * COMPONENTS;
      uchar* cv_data = reinterpret_cast<uchar*>(cv_image->imageData);
      int colour = (static_cast<int>(cv_data[index + 2]) << 16) +
                   (static_cast<int>(cv_data[index + 1]) << 8) +
                   (static_cast<int>(cv_data[index]));
      data->push_back(colour);
    }
  }
}

void TurnGray(const ImageData<int>& input_image, ImageData<uchar>* gray_image) {
  if (input_image.IsEmpty()) {
    printf("error: input image data is empty");
    return;
  }
  int height = input_image.GetHeight();
  int width = input_image.GetWidth();

  if (gray_image->IsEmpty()) {
    gray_image->CreateEmptyImage(width, height);
  }
  for (int y = 0; y < height; ++y) {
    for (int x  = 0; x < width; ++x) {
      int index = y * width + x;
      int red = (GET_PIXEL(&input_image, index) & RED) >> 16;
      int green = (GET_PIXEL(&input_image, index) & GREEN) >> 8;
      int blue = GET_PIXEL(&input_image, index) & BLUE;
      uchar gray = static_cast<uchar>(red * 0.3 + green * 0.59 + blue * 0.11);
      SET_PIXEL(gray_image, index, gray);
    }
  }
}

void SaveImage(const char* out_file_name, const ImageData<int>& image_data) {
  if (image_data.IsEmpty()) {
    printf("error: image data is empty");
    return;
  }
  int width = image_data.GetWidth();
  int height = image_data.GetHeight();

  CvSize size;
  size.width = width;
  size.height = height;
  IplImage* cv_image = cvCreateImage(size, 8, COMPONENTS);
  if (cv_image == NULL) {
    printf("error: the creation of cv image is failure");
    return;
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int index = y * cv_image->widthStep + x * COMPONENTS;
      uchar* cv_data = reinterpret_cast<uchar*>(cv_image->imageData);
      int colour = GET_PIXEL(&image_data, y * width + x);
      cv_data[index + 2] = static_cast<uchar>((colour & RED) >> 16);
      cv_data[index + 1] = static_cast<uchar>((colour & GREEN) >> 8);
      cv_data[index] = static_cast<uchar>(colour & BLUE);
    }
  }
  cvSaveImage(out_file_name, cv_image);
}

void SaveImage(const char* out_file_name, const ImageData<uchar>& image_data) {
  if (image_data.IsEmpty()) {
    printf("error: image data is empty");
    return;
  }
  int width = image_data.GetWidth();
  int height = image_data.GetHeight();

  CvSize size;
  size.width = width;
  size.height = height;
  IplImage* cv_image = cvCreateImage(size, 8, COMPONENTS);
  if (cv_image == NULL) {
    printf("error: the creation of cv image is failure");
    return;
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      int index = y * cv_image->widthStep + x * COMPONENTS;
      uchar* cv_data = reinterpret_cast<uchar*>(cv_image->imageData);
      uchar gray = GET_PIXEL(&image_data, y * width + x);
      cv_data[index + 2] = gray;
      cv_data[index + 1] = gray;
      cv_data[index] = gray;
    }
  }
  cvSaveImage(out_file_name, cv_image);
}

void GetGradiendMap(const ImageData<uchar>& gray_image, ImageData<uchar>* grad_image) {
  int image_width = gray_image.GetWidth();
  int image_height = gray_image.GetHeight();

  if (grad_image != NULL && grad_image->IsEmpty()) {
    grad_image->CreateEmptyImage(image_width, image_height);
  } else {
    printf("error: grad_image should be an empty image\n");
  }

  for (int y = 0; y < image_height; ++y) {
    for (int x = 0; x < image_width; ++x) {
      int index_cen = y * image_width + x;
      if (x == 0 || x == image_width - 1 ||
          y == 0 || y == image_height - 1) {
        SET_PIXEL(grad_image, index_cen, 255);
        continue;
      }
      int index[8] = EIGHT_ARROUND_POSITION(x, y, image_width, image_height);

      double gx = GET_PIXEL(&gray_image, index[3]) +
                  2 * GET_PIXEL(&gray_image, index[2]) +
                  GET_PIXEL(&gray_image, index[1]) +
                  -GET_PIXEL(&gray_image, index[5]) +
                  - 2 * GET_PIXEL(&gray_image, index[6]) +
                  - GET_PIXEL(&gray_image, index[7]);

      double gy = GET_PIXEL(&gray_image, index[7]) +
                  2 * GET_PIXEL(&gray_image, index[0]) +
                  GET_PIXEL(&gray_image, index[1]) +
                  - GET_PIXEL(&gray_image, index[5]) +
                  - 2 * GET_PIXEL(&gray_image, index[4]) +
                  - GET_PIXEL(&gray_image, index[3]);

      double sum_of_squares = pow(gx, 2) + pow(gy, 2);
	    uchar dst_gray = std::min(static_cast<int>(sqrt(sum_of_squares)), 255);
      // dst_gray = TURN_COORDINATE_TO_COLOUR(dst_gray, dst_gray, dst_gray);
      SET_PIXEL(grad_image, index_cen, dst_gray);
    }
  }
}

void DoMarkConnectedArea(const ImageData<uchar>& grad_image, ImageData<int>* marked_image,
                         int x, int y, int width, int height, int mark_num,
                         int max_threshold) {

  std::stack<int> unsearched_points;
  unsearched_points.push(y * width + x);
  while (unsearched_points.size() != 0) {
    int index = unsearched_points.top(); 
    unsearched_points.pop();
    int y = index / width;
    int x = index - y * width;
    int arrounds[8] = EIGHT_ARROUND_POSITION(x, y, width, height);
    for (int i = 0; i < 8; ++i) {
      int gradient = static_cast<int>(GET_PIXEL(&grad_image, arrounds[i]));
      int mark_value = GET_PIXEL(marked_image, arrounds[i]);

      assert(mark_value == 0 || std::abs(mark_value) == mark_num);
      if (mark_value == 0) {
        if (gradient <= max_threshold) {
          SET_PIXEL(marked_image, arrounds[i], mark_num);
          unsearched_points.push(arrounds[i]);
        } else {
          SET_PIXEL(marked_image, index, -mark_num);
        }
      }
    }
  }
}

void MarkConnectedArea(const ImageData<uchar>& grad_image, ImageData<int>* marked_image,
                       int max_threshold) {
  if (grad_image.IsEmpty()) {
    printf("error: the grad_image is empty\n");
    return;
  }
  int width = grad_image.GetWidth();
  int height = grad_image.GetHeight();
  if (marked_image->IsEmpty()) {
    marked_image->CreateEmptyImage(width, height);
  }

  int mark_num = 100;
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      int index = y * width + x;
      int gradient = static_cast<int>((*grad_image.m_data)[index]);
      int mark_value = (*marked_image->m_data)[index];
      if (mark_value == 0 && gradient <= max_threshold) {
        SET_PIXEL(marked_image, index, mark_num);
        DoMarkConnectedArea(grad_image, marked_image, x, y, width, height, mark_num,
                            max_threshold);
        // mark_num++;
        mark_num += 10000;
      }
    }
  }
}

void Watershed(const ImageData<uchar>& grad_image, ImageData<int>* marked_image,
               int start_gradient) {
  if (grad_image.IsEmpty()) {
    printf("error: the grad_image is empty\n");
    return;
  }
  int width = grad_image.GetWidth();
  int height = grad_image.GetHeight();
  std::queue<int> grad_queues[256];
  for (int y = 1; y < height - 1; ++y) {
    for (int x = 1; x < width - 1; ++x) {
      int index = y * width + x;
      int mark_value = (*marked_image->m_data)[index];
      if (mark_value < 0 && mark_value != IN_QUEUE) {
        SET_PIXEL(marked_image, index, -mark_value);
        int arrounds[4] = FOUR_ARROUND_POSITION(x, y, width, height);
        for (int i = 0; i < 4; ++i) {
          int arround_mark_value = (*marked_image->m_data)[arrounds[i]];
          if (arround_mark_value == 0) {
            int grad_value = (*grad_image.m_data)[arrounds[i]];
            assert(grad_value > start_gradient && grad_value < 256);
            grad_queues[grad_value].push(arrounds[i]);
            SET_PIXEL(marked_image, arrounds[i], IN_QUEUE);
          }
        }
      }
    }
  }

  int start_idx = start_gradient;
  for (; start_idx < 256; ++start_idx) {
    if (!grad_queues[start_idx].empty()) {
      break;
    }
  }
  int queues_idx = start_idx;
  while(true) {
    if (grad_queues[queues_idx].empty()) {
      if(++queues_idx >= 256) {
        break;
      }
      continue;
    }
    int mark_index = grad_queues[queues_idx].front();
    grad_queues[queues_idx].pop();
    int mark_value = GET_PIXEL(marked_image, mark_index);
    assert(mark_value == IN_QUEUE);

    int mark_y = mark_index / width;
    int mark_x = mark_index - mark_y * width;
    int mark_arrounds[4] = FOUR_ARROUND_POSITION(mark_x, mark_y, width, height);

    int mark_number = 0;
    for (int i = 0; i < 4; ++i) {
      int mark_value = GET_PIXEL(marked_image, mark_arrounds[i]);
      if (mark_value != WASHED && mark_value != IN_QUEUE && mark_value != 0) {
        if (mark_number == 0) {
          mark_number = mark_value;
          SET_PIXEL(marked_image, mark_index, mark_number);
        } else if (mark_number != mark_value) {
          SET_PIXEL(marked_image, mark_index, WASHED);
          mark_number = WASHED;
        }
      }
    }
    if (mark_number == WASHED) {
      continue;
    }

    for (int i = 0; i < 4; ++i) {
      int mark_value = GET_PIXEL(marked_image, mark_arrounds[i]);
      if (mark_value == 0) {
        int grad_value = static_cast<int>((*grad_image.m_data)[mark_arrounds[i]]);
        assert(grad_value > start_gradient);
        grad_queues[grad_value].push(mark_arrounds[i]);
        queues_idx = std::min(queues_idx, grad_value);
        SET_PIXEL(marked_image, mark_arrounds[i], IN_QUEUE);
      }
    }
  }
}

}  // namespace utils
