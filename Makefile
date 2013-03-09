#CC=clang # -fsanitize=integer -fno-sanitize=unsigned-integer-overflow
#CFLAGS=-O3 -Wall -Wextra 

CC=gcc 
CFLAGS=-O3 -Wall 

OBJS = 	daniel.o bernd.o pascal.o toby.o \
	patrick.o francois_2.o yolanpa.o phil.o mikael.o \
	magnus.o magnus_2.o ryan.o matthew.o stefan.o \
	david.o matthew_2.o mats.o peter.o thomas.o \
	bernd_2.o ryanf.o mattias.o matthewf.o kevin.o \
	chucky.o daniel_2.o andrew.o david_2.o tordek.o \
	mikael_2.o jeffrey.o andrewh.o yang.o adam.o \
	olivier.o ahmed.o ken.o davidl.o bastian.o \
	francois.o ethan.o renaud.o davec.o dokoto.o \
	robert.o ken_2.o tordek_2.o till.o andrewh_2.o \
	gedare.o ethan_2.o markus.o matthewf_2.o adam_2.o \
	chucky_2.o tennessee.o davec_2.o greg.o ahmed_2.o \
	jonathan.o yang_2.o andrewh_3.o davidl_2.o \
	adam_3.o ken_3.o dario.o dokoto_2.o gedare_2.o \
	ben.o markus_2.o matthew_3.o robert_2.o ryanf_2.o \
	ken_4.o adam_4.o \
	str2long_test.o 

all: str2long

str2long: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o str2long -lm

clean:
	rm -f *.o str2long
