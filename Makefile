# can-idps - CAN 总线入侵检测（SocketCAN）
# 需 Linux 内核 SocketCAN 支持

CC = gcc
CFLAGS = -Wall -O2 -I include
LDFLAGS =
LIBS =

SRCS = src/main.c src/capture.c src/decoder.c src/detect.c src/rate.c src/output.c
OBJS = $(SRCS:.c=.o)
TARGET = can-idps

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)
