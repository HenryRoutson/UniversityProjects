# modified from
# https://edstem.org/au/courses/9173/lessons/25174/slides/178431

# with inspiration taken from
# https://youtube.com/watch/?v=DtGrdB8wQ_8




CC = gcc
CFLAGS = -Wall -g  -Werror -pedantic-errors
LDLIBS = 

C_SHARED = driver.c PR_QuadTree.c csv.c list.c footpath.c
C_STAGES = stage3.c stage4.c

O_SHARED = $(C_SHARED:.c=.o)
O_STAGES = $(C_STAGES:.c=.o)

C = $(C_SHARED) $(C_STAGES)
O = $(O_SHARED) $(O_STAGES)



# executables depend on object files
exe: $(O)
	LD_PRELOAD=
	rm -f output.*
	$(CC) $(CFLAGS) -o dict3 $(O) $(LDLIBS)
	cp dict3 dict4

# object files depend on c files 
# ($^ is the c file name)
%.o: %.c 
	$(CC) $(CFLAGS) -c $^
	
# dict executable depends on stage and shared c files
dict3: exe
dict4: exe


run1:
	LD_PRELOAD=
	make exe

	./dict3 3 tests/dataset_1.csv output.out 144.9375 -37.8750 145.0000 -37.6875 < tests/test1.s3.in > output.stdout.out
	./dict3 3 tests/dataset_1.csv output.out 144.969 -37.7965 144.971 -37.795 < tests/test2.s3.in > output.stdout.out
	./dict3 3 tests/dataset_1.csv output.out 144.969 -37.7965 144.9725 -37.7945 < tests/test3.s3.in > output.stdout.out
	./dict3 3 tests/dataset_1.csv output.out 144.969 -37.7965 144.976 -37.7925 < tests/test4.s3.in > output.stdout.out
	./dict3 3 tests/dataset_2.csv output.out 144.968 -37.797 144.977 -37.79 < tests/test5.s3.in > output.stdout.out
	./dict3 3 tests/dataset_20.csv output.out 144.952 -37.81 144.978 -37.79 < tests/test6.s3.in > output.stdout.out
	./dict3 3 tests/dataset_100.csv output.out 144.9538 -37.812 144.9792 -37.784 < tests/test7.s3.in > output.stdout.out
	./dict3 3 tests/dataset_1000.csv output.out 144.9375 -37.8750 145.0000 -37.6875 < tests/test8.s3.in > output.stdout.out


	# valgrind --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all -s ./dict3 3 tests/dataset_1000.csv output.out 144.9375 -37.8750 145.0000 -37.6875 < tests/test8.s3.in > output.stdout.out


	./dict4 4 tests/dataset_1.csv output.out 144.969 -37.7975 144.971 -37.7955 < tests/test9.s4.in > output.stdout.out
	./dict4 4 tests/dataset_1.csv output.out 144.969 -37.7965 144.9725 -37.7945 < tests/test10.s4.in > output.stdout.out
	./dict4 4 tests/dataset_2.csv output.out 144.968 -37.797 144.977 -37.79 < tests/test11.s4.in > output.stdout.out
	./dict4 4 tests/dataset_20.csv output.out 144.952 -37.81 144.978 -37.79 < tests/test12.s4.in > output.stdout.out
	./dict4 4 tests/dataset_100.csv output.out 144.9375 -37.8750 145.0000 -37.6875 < tests/test13.s4.in > output.stdout.out
	./dict4 4 tests/dataset_1000.csv output.out 144.9375 -37.8750 145.0000 -37.6875 < tests/test14.s4.in > output.stdout.out


	# valgrind --tool=memcheck --leak-check=yes --track-origins=yes --show-leak-kinds=all -s ./dict4 4 tests/dataset_1000.csv output.out 144.9375 -37.8750 145.0000 -37.6875 < tests/test14.s4.in > output.stdout.out




run2:
	LD_PRELOAD=
	make exe

	./dict3 3 tests/dataset_1.csv output.out 144.969 -37.7965 144.971 -37.795

clean:
	LD_PRELOAD=
	rm -f *.o dict3 dict4 output.* driver

