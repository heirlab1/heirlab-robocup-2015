TARGET		= MasterControl
OBJS		= MasterControl.o
SRCS		= $(OBJS:.o=.cpp)
INCLUDEDIRS	+= -I../include -I/usr/local/include/
LIBDIRS		+= -L../lib -L/usr/lib
CFLAGS		+= $(INCLUDEDIRS) -W -Wall -O2
#LDFLAGS		+= $(LIBDIRS) -ldxl -lpthread -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect
LDFLAGS		+= $(LIBDIRS) -lm -ltxd -lvision -larduino -ldxl -lopencv_calib3d -lpthread -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videostab

CC			= g++

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "file deleted."

dep:
	g++makedep $(SRCS)
