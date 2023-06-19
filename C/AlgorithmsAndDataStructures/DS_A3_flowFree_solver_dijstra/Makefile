CC=gcc
CPPFLAGS= -Wall  -Werror -O3 -g 
#CPPFLAGS= -Wall  -Werror  -g 
LDFLAGS = -lm

SRC=src/node.o src/options.o src/utils.o src/extensions.o src/queues.o src/engine.o src/search.o src/flow_solver.o
TARGET=flow


%.o: %.c
	#dlskjfl;aj
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

# executable depends on object f
$(TARGET): $(SRC)
	$(CC) $(SRC) $(CPPFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) src/*.o

# RUNWITH = valgrind --track-origins=yes --leak-check=full
RUNWITH = 

run: $(TARGET)

	LD_PRELOAD=

	make clean
	make

	#$(RUNWITH) ./flow -A -d puzzles/deadlock_6x6_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/unsolvable_cross.txt

	#$(RUNWITH) ./flow -A -d puzzles/regular_5x5_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/regular_6x6_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/regular_7x7_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/regular_8x8_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/regular_9x9_01.txt

	$(RUNWITH) ./flow -A -d puzzles/jumbo_10x10_01.txt
	$(RUNWITH) ./flow -A -d puzzles/jumbo_11x11_01.txt
	$(RUNWITH) ./flow -A -d puzzles/jumbo_12x12_30.txt
	#$(RUNWITH) ./flow -A -d puzzles/jumbo_13x13_26.txt
	#$(RUNWITH) ./flow -A -d puzzles/jumbo_14x14_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/jumbo_14x14_02.txt
	#$(RUNWITH) ./flow -A -d puzzles/jumbo_14x14_19.txt
	#$(RUNWITH) ./flow -A -d puzzles/jumbo_14x14_21.txt
	#$(RUNWITH) ./flow -A -d puzzles/jumbo_14x14_30.txt

	$(RUNWITH) ./flow -A -d puzzles/extreme_8x8_01.txt
	$(RUNWITH) ./flow -A -d puzzles/extreme_9x9_01.txt
	$(RUNWITH) ./flow -A -d puzzles/extreme_9x9_30.txt
	$(RUNWITH) ./flow -A -d puzzles/extreme_10x10_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_10x10_30.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_11x11_07.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_11x11_15.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_11x11_20.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_11x11_30.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_12x12_01.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_12x12_02.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_12x12_28.txt
	#$(RUNWITH) ./flow -A -d puzzles/extreme_12x12_29.txt
	#$(RUNWITH) ./flow - -dA puzzles/extreme_12x12_30.txt

	

	
