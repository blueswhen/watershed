// Copyright 2014-4 sxniu
#ifndef INCLUDE_IMAGEDATA_H_
#define INCLUDE_IMAGEDATA_H_

#include <vector>
#include <stdio.h>
#include "include/utils.h"
#include "include/colour.h"

typedef unsigned char uchar;

#define GET_PIXEL(data, index) ((*(data)->m_data)[index])
#define SET_PIXEL(data, index, colour) ((*(data)->m_data)[index] = (colour))

template <class T>
class ImageData;

template <class T>
class ImageData {
 public:
  friend void utils::ReadImage(const char* file_name, ImageData<int>* image_data);

  ImageData();
  ImageData(const ImageData& image_data);
  ~ImageData();
  bool IsEmpty() const;
  void CreateEmptyImage(int width, int height);
  const char* GetFileName() const;
  int GetWidth() const;
  int GetHeight() const;
  int GetRandomSeed() const;
#if 0
  inline T GetPixel(int index) const {
    return (*m_data)[index];
  }
  inline void SetPixel(int index, T colour) {
    (*m_data)[index] = colour;
  }
#endif
  // do not use m_data directly, please use macro GET_PIXEL, SET_PIXEL
  std::vector<T>* m_data;

 private:
  const char* m_file_name;
  int m_width;
  int m_height;
  mutable int m_random_seed;
};

template <class T>
ImageData<T>::ImageData()
  : m_file_name(NULL)
  , m_width(0)
  , m_height(0)
  , m_data(new std::vector<T>(0))
  , m_random_seed(-1) {}

template <class T>
ImageData<T>::ImageData(const ImageData<T>& image_data)
  : m_file_name(image_data.m_file_name)
  , m_width(image_data.m_width)
  , m_height(image_data.m_height)
  , m_data(new std::vector<T>(*(image_data.m_data)))
  , m_random_seed(-1) {}

template <class T>
ImageData<T>::~ImageData() {
  m_file_name = NULL;
  if (m_data != NULL) {
    delete m_data;
    m_data = NULL;
  }
}

template <class T>
bool ImageData<T>::IsEmpty() const {
  if (m_width == 0 || m_height == 0) {
    return true;
  } else {
    return false;
  }
}

template <class T>
void ImageData<T>::CreateEmptyImage(int width, int height) {
  if (m_data->size() != 0) {
    printf("error: this is an image with data\n");
    return;
  }
  m_data = new std::vector<T>(width * height, 0);
  m_width = width;
  m_height = height;
}

template <class T>
const char* ImageData<T>::GetFileName() const {
  return m_file_name;
}

template <class T>
int ImageData<T>::GetWidth() const {
  return m_width;
}

template <class T>
int ImageData<T>::GetHeight() const {
  return m_height;
}

template <class T>
int ImageData<T>::GetRandomSeed() const {
  if (m_random_seed != -1) {
    return m_random_seed;
  }
  if (m_data->size() == 0) {
    m_random_seed = 0;
    return m_random_seed;
  }
  int sum = 0;
  for (int i = 0; i < m_data->size(); ++i) {
    sum += static_cast<int>((*m_data)[i]) & BLUE;
  }
  m_random_seed = sum;
  return m_random_seed;
}

#endif  // INCLUDE_IMAGEDATA_H_
