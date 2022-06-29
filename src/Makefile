FILENAME           = main
ADDITIONAL_SOURCES = time_manager.c block_manager.c
DEVICE             = atmega32
PROGRAMMER         = usbasp
FLAGS              = -std=c99 -Wall -g -Os -mmcu=$(DEVICE) -DF_CPU=1000000UL -I.


default: compile upload clean

compile:
	avr-gcc $(FLAGS) $(FILENAME).c $(ADDITIONAL_SOURCES) -o $(FILENAME).bin
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).bin $(FILENAME).hex 

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -F -B5 -U flash:w:$(FILENAME).hex:i

clean:
	rm $(FILENAME).bin
	rm $(FILENAME).hex
