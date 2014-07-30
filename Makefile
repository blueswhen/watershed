#Copyright 2014-5 sxniu 

CXX = g++
LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc
INCS = -I.

watershed : \
  include/utils.h \
  include/ImageData.h \
  include/colour.h \
  include/CountTime.h \
  src/utils.cpp \
  src/main.cpp \
  src/CountTime.cpp
	$(CXX) $^ $(LIBS) $(INCS) -o $@ -O2

clean :
	rm -f watershed 
	rm -f *.jpg
	rm -f *.bmp
