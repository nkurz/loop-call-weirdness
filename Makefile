CC = gcc

CCOPT = -O2


all: loop-call-weirdness 

loop-call-weirdness: loop-call-weirdness.c
	$(CC) $(CCOPT) $^ -o $@
	$(CC) $(CCOPT) -DIACA -c $^ -o iaca.o

clean:
	rm -rf loop-call-weirdness iaca.o

