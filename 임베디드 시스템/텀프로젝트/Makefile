obj-m += gpio_driver.o
obj-m += seg_driver.o
obj-m += buzzer_driver.o
KDIR = ~/working/kernel
RESULT = main
SRC = $(RESULT).cpp
CCC = g++
FLAG = `pkg-config opencv4 --libs --cflags`
FLAG2 = `pkg-config tesseract --libs --cflags`
LIBS = $(FLAG) $(FLAG2)

all:
	make -C $(KDIR) M=$(PWD) modules
	$(CCC) -o $(RESULT) $(SRC) $(LIBS)

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f $(RESULT)