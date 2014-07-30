// Copyright sxniu 2014-7

#include <stdio.h>
#include <stdlib.h>

#include "include/ImageData.h"
#include "include/utils.h"
#include "include/colour.h"
#include "include/CountTime.h"

typedef unsigned char uchar;

#define IMAGE_OUT_NAME "result.bmp"
#define START_GRADIENT 10

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("error: need a image to input\n");
    return 1;
  }
  const char* file_name = argv[1];
  ImageData<int> source_image;
  ImageData<int> marked_image;
  ImageData<uchar> gray_image;
  ImageData<uchar> grad_image;

  CountTime ct;
  ct.ContBegin();
  utils::ReadImage(file_name, &source_image);
  utils::TurnGray(source_image, &gray_image);
  utils::GetGradiendMap(gray_image, &grad_image);
  utils::MarkConnectedArea(grad_image, &marked_image, START_GRADIENT);
  utils::Watershed(grad_image, &marked_image, START_GRADIENT);
  ct.ContEnd();
  ct.ContResult();

  utils::SaveImage(IMAGE_OUT_NAME, marked_image);

  return 0;
}
