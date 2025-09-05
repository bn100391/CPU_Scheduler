CC=gcc
CFLAGS=-Wall -g -O0
LDFLAGS=

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
all: scheduler

#
# Scheduler program
#
scheduler: scheduler.c scheduler.h
	$(CC) -o scheduler $(CFLAGS) scheduler.c

#
# Cleanup the files that we have created
#
clean:
	$(RM) scheduler
	$(RM) -rf *.dSYM
	$(RM) test_out


