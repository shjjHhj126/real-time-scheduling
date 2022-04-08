CC=gcc
obj:=main.o stream.o schedule.o
exe:=main
CFLAGS=-O1 -g -Wall -std=gnu99

all:$(obj)
	$(CC) -o $(exe) $(obj)

%.o:%.c
	$(CC) -c $^ $(CFLAGS) -o $@

clean:
	rm -rf $(exe) $(obj)
