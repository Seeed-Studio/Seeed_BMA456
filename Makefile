# tary, 16:56 2018/12/14
#
TARGET     = bma456_test
BMA456_LIB = libbma456.so
OBJS       = bma4.o bma456.o

all: $(TARGET) $(BMA456_LIB)

$(TARGET): $(OBJS) rpi_bma456_m.o
	$(CC) -o $@ $^

$(BMA456_LIB): $(OBJS) rpi_bma456.o
	$(CC) --shared -o $@ $^

rpi_bma456_m.o: CFLAGS=-D_HAS_MAIN=1
rpi_bma456_m.o: rpi_bma456.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f *.o $(TARGET) $(BMA456_LIB)

.PHONY: all clelan
