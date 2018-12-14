
TARGET = bma456_test
OBJS   = bma4.o bma456.o rpi_bma456.o

all: $(TARGET)

$(TARGET): $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clelan
