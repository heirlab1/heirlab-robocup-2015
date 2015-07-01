TARGET		= master_control
OBJS		= master_control.o
SRCS		= $(OBJS:.o=.cpp)
INCLUDEDIRS	+= -I../include
LIBDIRS		+= -L../lib
CFLAGS		= $(INCLUDEDIRS) -W -Wall -O2
LDFLAGS		= $(LIBDIRS) -lm -ltxd -lvision

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