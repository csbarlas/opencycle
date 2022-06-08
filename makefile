all: headers opencycle

#debug entry NOT FOR RELEASE
opencycle: opencycle.o buttons.o callbacks.o oc_lcd.o oc_gps.o state.h pin_mappings.h
	gcc -Wall -g -o bin/$@ $^ -lwiringPi -lwiringPiDev

#easy rule for backups
backup.zip: makefile *.c *.h
	zip "$$(date +"%Y_%m_%d_%Hh%Mm%Ss")_$@" $^

tags: *.c
	ctags *.c

headers: *.c
	makeheaders *.c

%.o:%.c *.h
	gcc -pedantic -Wimplicit-function-declaration -Wreturn-type -g -c $< -o $@
