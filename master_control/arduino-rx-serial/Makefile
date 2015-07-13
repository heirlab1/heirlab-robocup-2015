TARGET		= libarduino.a
OBJS		= arduino-serial.o arduino-serial-lib.o
SRCS		= $(OBJS:.o=.c)
INCLUDEDIRS	+= -I../include
LIBDIRS		+=
CFLAGS		= $(INCLUDEDIRS) -W -Wall -O2

CC			= g++
AR 			= ar

$(TARGET): $(OBJS)
	$(AR) rs $@ $^
	cp $(TARGET) ../lib
	
.c.o:
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "file deleted."

dep:
	g++makedep $(SRCS)