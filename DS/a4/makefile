# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99 -lm

# Targets
Q1 = q1
Q2 = q2

# Default target: compile both programs
all: $(Q1) $(Q2)

# Q1 build
$(Q1): mysort.c mysort_ptest.c mysort.h
	$(CC) mysort.c mysort_ptest.c -o $(Q1) $(CFLAGS)

# Q2 build
$(Q2): myrecord.c mysort.c myrecord_ptest.c myrecord.h mysort.h
	$(CC) myrecord.c mysort.c myrecord_ptest.c -o $(Q2) $(CFLAGS)

# Run commands
run_q1: $(Q1)
	./$(Q1)

run_q2: $(Q2)
	./$(Q2)

# Clean command
clean:
	rm -f $(Q1) $(Q2) *.o
