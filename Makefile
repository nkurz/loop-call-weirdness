CC = gcc 
CCOPT = -O2 -fno-inline

all: loop-call-weirdness 

loop-call-weirdness: loop-call-weirdness.c
	$(CC) $(CCOPT) $^ -o $@
	$(CC) $(CCOPT) -DIACA -c $^ -o iaca.o
	objdump -d $@ > objdump-d.txt

clean:
	rm -rf loop-call-weirdness iaca.o

test: all
	run_all.sh