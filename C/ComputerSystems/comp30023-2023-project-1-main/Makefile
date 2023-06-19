
CC=gcc
EXE=allocate
OBJ=main.o list.o help.o print.o scheduler.o inputs.o memory.o real_process.o
CFLAGS= -Wall -g -Werror -O3

# executable depends on object f
$(EXE): $(OBJ) 
	$(CC) $(OBJ) $(CFLAGS) -o $(EXE)

# o depends on c
%.o: %.c %.h global.h types.h
	$(CC) $(CFLAGS) $(CFLAGS) -c -o $@ $<

format:
	clang-format -style=file -i *.c

clean:
	rm -f $(EXE) *.o main

RUNWITH = 
# RUNWITH = valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all


process:
	$(CC) $(CFLAGS) process.c -o process 

gdb: $(EXE)
	gdb --args allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3

input: $(EXE)

	$(RUNWITH) ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1
	$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3

	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 1
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3

	$(RUNWITH) ./allocate -f cases/task3/simple.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task4/spec.txt -s SJF -m infinite -q 3
	$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3
	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3

	$(RUNWITH) ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3


custom_tests: $(EXE)

	$(RUNWITH) ./allocate -f cases/tests/1.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/tests/1.txt -s RR -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/tests/2.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/tests/2.txt -s RR -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/tests/3.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/tests/3.txt -s RR -m best-fit -q 1




test: $(EXE)

	make custom_tests

	$(RUNWITH) ./allocate -f cases/tests/1.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/tests/1.txt -s RR -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/tests/2.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/tests/2.txt -s RR -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/tests/3.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/tests/3.txt -s RR -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/task1/simple.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s SJF -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s SJF -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task3/simple.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task3/simple.txt -s RR -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/task3/simple.txt -s RR -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 2

	$(RUNWITH) ./allocate -f cases/task4/spec.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task4/spec.txt -s SJF -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m best-fit -q 2

	$(RUNWITH) ./allocate -f cases/task4/spec.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m best-fit -q 1

	$(RUNWITH) ./allocate -f cases/task1/simple.txt -s SJF -m best-fit -q 1
	$(RUNWITH) ./allocate -f cases/task1/simple.txt -s RR -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task1/simple.txt -s RR -m best-fit -q 3

	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m best-fit -q 3
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s SJF -m best-fit -q 2
	$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s SJF -m best-fit -q 3

run: $(EXE)

	#-$(RUNWITH) ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1 | diff - cases/task1/simple-sjf.out
	#-$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3 | diff - cases/task1/more-processes.out

	-$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3 | diff - cases/task2/simple-rr.out
	#-$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 1 | diff - cases/task2/two-processes-1.out
	#-$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3 | diff - cases/task2/two-processes-3.out

	#-$(RUNWITH) ./allocate -f cases/task3/simple.txt -s SJF -m best-fit -q 3 | diff - cases/task3/simple-bestfit.out
	#-$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 3 | diff - cases/task3/non-fit-sjf.out
	#-$(RUNWITH) ./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3 | diff - cases/task3/non-fit-rr.out

	#-$(RUNWITH) ./allocate -f cases/task4/spec.txt -s SJF -m infinite -q 3 | diff - cases/task4/spec.out
	#-$(RUNWITH) ./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3 | diff - cases/task1/more-processes.out
	#-$(RUNWITH) ./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3 | diff - cases/task2/simple-rr.out

	#-$(RUNWITH) ./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1 | diff - cases/task1/simple-sjf.out
	#-$(RUNWITH) ./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3 | diff - cases/task2/two-processes-3.out

	

# credit to this person https://edstem.org/au/courses/10873/discussion/1305892
generated_tests:  $(EXE)

	./allocate -f cases/generated-tests/data1.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output1.txt
	./allocate -f cases/generated-tests/data2.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output2.txt
	./allocate -f cases/generated-tests/data3.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output3.txt
	./allocate -f cases/generated-tests/data4.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output4.txt
	./allocate -f cases/generated-tests/data5.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output5.txt
	./allocate -f cases/generated-tests/data6.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output6.txt
	./allocate -f cases/generated-tests/data7.txt -s SJF -m infinite -q 1 | diff - cases/generated-tests/output7.txt
	./allocate -f cases/generated-tests/data8.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output8.txt
	./allocate -f cases/generated-tests/data9.txt -s SJF -m infinite -q 1 | diff - cases/generated-tests/output9.txt
	./allocate -f cases/generated-tests/data10.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output10.txt
	./allocate -f cases/generated-tests/data11.txt -s SJF -m infinite -q 3 | diff - cases/generated-tests/output11.txt
	./allocate -f cases/generated-tests/data12.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output12.txt
	./allocate -f cases/generated-tests/data13.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output13.txt
	./allocate -f cases/generated-tests/data14.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output14.txt
	./allocate -f cases/generated-tests/data15.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output15.txt
	./allocate -f cases/generated-tests/data16.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output16.txt
	./allocate -f cases/generated-tests/data17.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output17.txt
	./allocate -f cases/generated-tests/data18.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output18.txt
	./allocate -f cases/generated-tests/data19.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output19.txt
	./allocate -f cases/generated-tests/data20.txt -s RR -m best-fit -q 1 | diff - cases/generated-tests/output20.txt
	./allocate -f cases/generated-tests/data21.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output21.txt
	./allocate -f cases/generated-tests/data22.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output22.txt
	./allocate -f cases/generated-tests/data23.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output23.txt
	./allocate -f cases/generated-tests/data24.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output24.txt
	./allocate -f cases/generated-tests/data25.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output25.txt
	./allocate -f cases/generated-tests/data26.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output26.txt
	./allocate -f cases/generated-tests/data27.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output27.txt
	./allocate -f cases/generated-tests/data28.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output28.txt
	./allocate -f cases/generated-tests/data29.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output29.txt
	./allocate -f cases/generated-tests/data30.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output30.txt
	./allocate -f cases/generated-tests/data31.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output31.txt
	./allocate -f cases/generated-tests/data32.txt -s SJF -m infinite -q 1 | diff - cases/generated-tests/output32.txt
	./allocate -f cases/generated-tests/data33.txt -s SJF -m infinite -q 1 | diff - cases/generated-tests/output33.txt
	./allocate -f cases/generated-tests/data34.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output34.txt
	./allocate -f cases/generated-tests/data35.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output35.txt
	./allocate -f cases/generated-tests/data36.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output36.txt
	./allocate -f cases/generated-tests/data37.txt -s SJF -m infinite -q 3 | diff - cases/generated-tests/output37.txt
	./allocate -f cases/generated-tests/data38.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output38.txt
	./allocate -f cases/generated-tests/data39.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output39.txt
	./allocate -f cases/generated-tests/data40.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output40.txt
	./allocate -f cases/generated-tests/data41.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output41.txt
	./allocate -f cases/generated-tests/data42.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output42.txt
	./allocate -f cases/generated-tests/data43.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output43.txt
	./allocate -f cases/generated-tests/data44.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output44.txt
	./allocate -f cases/generated-tests/data45.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output45.txt
	./allocate -f cases/generated-tests/data46.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output46.txt
	./allocate -f cases/generated-tests/data47.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output47.txt
	./allocate -f cases/generated-tests/data48.txt -s SJF -m infinite -q 3 | diff - cases/generated-tests/output48.txt
	./allocate -f cases/generated-tests/data49.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output49.txt
	./allocate -f cases/generated-tests/data50.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output50.txt
	./allocate -f cases/generated-tests/data51.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output51.txt
	./allocate -f cases/generated-tests/data52.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output52.txt
	./allocate -f cases/generated-tests/data53.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output53.txt
	./allocate -f cases/generated-tests/data54.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output54.txt
	./allocate -f cases/generated-tests/data55.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output55.txt
	./allocate -f cases/generated-tests/data56.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output56.txt
	./allocate -f cases/generated-tests/data57.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output57.txt
	./allocate -f cases/generated-tests/data58.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output58.txt
	./allocate -f cases/generated-tests/data59.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output59.txt
	./allocate -f cases/generated-tests/data60.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output60.txt
	./allocate -f cases/generated-tests/data61.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output61.txt
	./allocate -f cases/generated-tests/data62.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output62.txt
	./allocate -f cases/generated-tests/data63.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output63.txt
	./allocate -f cases/generated-tests/data64.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output64.txt
	./allocate -f cases/generated-tests/data65.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output65.txt
	./allocate -f cases/generated-tests/data66.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output66.txt
	./allocate -f cases/generated-tests/data67.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output67.txt
	./allocate -f cases/generated-tests/data68.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output68.txt
	./allocate -f cases/generated-tests/data69.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output69.txt
	./allocate -f cases/generated-tests/data70.txt -s RR -m infinite -q 2 | diff - cases/generated-tests/output70.txt
	./allocate -f cases/generated-tests/data71.txt -s SJF -m infinite -q 3 | diff - cases/generated-tests/output71.txt
	./allocate -f cases/generated-tests/data72.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output72.txt
	./allocate -f cases/generated-tests/data73.txt -s SJF -m infinite -q 2 | diff - cases/generated-tests/output73.txt
	./allocate -f cases/generated-tests/data74.txt -s SJF -m infinite -q 3 | diff - cases/generated-tests/output74.txt
	./allocate -f cases/generated-tests/data75.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output75.txt
	./allocate -f cases/generated-tests/data76.txt -s SJF -m best-fit -q 1 | diff - cases/generated-tests/output76.txt
	./allocate -f cases/generated-tests/data77.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output77.txt
	./allocate -f cases/generated-tests/data78.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output78.txt
	./allocate -f cases/generated-tests/data79.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output79.txt
	./allocate -f cases/generated-tests/data80.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output80.txt
	./allocate -f cases/generated-tests/data81.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output81.txt
	./allocate -f cases/generated-tests/data82.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output82.txt
	./allocate -f cases/generated-tests/data83.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output83.txt
	./allocate -f cases/generated-tests/data84.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output84.txt
	./allocate -f cases/generated-tests/data85.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output85.txt
	./allocate -f cases/generated-tests/data86.txt -s SJF -m infinite -q 3 | diff - cases/generated-tests/output86.txt
	./allocate -f cases/generated-tests/data87.txt -s SJF -m infinite -q 1 | diff - cases/generated-tests/output87.txt
	./allocate -f cases/generated-tests/data88.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output88.txt
	./allocate -f cases/generated-tests/data89.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output89.txt
	./allocate -f cases/generated-tests/data90.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output90.txt
	./allocate -f cases/generated-tests/data91.txt -s SJF -m infinite -q 1 | diff - cases/generated-tests/output91.txt
	./allocate -f cases/generated-tests/data92.txt -s SJF -m best-fit -q 3 | diff - cases/generated-tests/output92.txt
	./allocate -f cases/generated-tests/data93.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output93.txt
	./allocate -f cases/generated-tests/data94.txt -s RR -m best-fit -q 2 | diff - cases/generated-tests/output94.txt
	./allocate -f cases/generated-tests/data95.txt -s SJF -m best-fit -q 2 | diff - cases/generated-tests/output95.txt
	./allocate -f cases/generated-tests/data96.txt -s RR -m infinite -q 1 | diff - cases/generated-tests/output96.txt
	./allocate -f cases/generated-tests/data97.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output97.txt
	./allocate -f cases/generated-tests/data98.txt -s RR -m best-fit -q 3 | diff - cases/generated-tests/output98.txt
	./allocate -f cases/generated-tests/data99.txt -s RR -m infinite -q 3 | diff - cases/generated-tests/output99.txt
