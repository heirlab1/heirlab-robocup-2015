TARGET		= libvision.a
OBJS		= Vision.o BallObject.o Head.o FieldObject.o Camera.o Tracking.o USB2AXmotorManager.o
SRCS		= $(OBJS:.o=.cpp)
INCLUDEDIRS	+= -I../include
LIBDIRS		+=
CFLAGS		= $(INCLUDEDIRS) -W -Wall -O2

CC			= g++
AR 			= ar

$(TARGET): $(OBJS)
	$(AR) rs $@ $^
	cp $(TARGET) ../lib
	
.cpp.o:
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "file deleted."

dep:
	g++makedep $(SRCS)