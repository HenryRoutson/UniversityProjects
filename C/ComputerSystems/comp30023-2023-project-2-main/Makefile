
CC = gcc
OBJ = rpc.o
LDFLAGS = -lm -pthread -lc 
CFLAGS = $(LDFLAGS) -Wall -g -Werror -O3 -ggdb3  -Wextra -Wsign-compare -Wint-conversion  # -pedantic


ALL = server client
HEADERS = shared.h global.h ignored.h

PORT = 6000
IP = "::1"

# executable depends on object files

all: $(ALL)

server: $(OBJ)
	$(CC) -o rpc-server server.c $(OBJ) $(CFLAGS)

client: $(OBJ)
	$(CC) -o rpc-client client.c $(OBJ) $(CFLAGS)

testing: $(OBJ)

	make clean
	make 

	$(CC) -o client client.a $(OBJ) $(CFLAGS)
	$(CC) -o server server.a $(OBJ) $(CFLAGS)

# o depends on c
%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<


format:
	clang-format -style=file -i *.c  *.h

clean:
	rm -f *.o $(ALL) *.o rpc-client rpc-server

RUNWITH = 
# RUNWITH = valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all


# python automation
# files = ["1+1", "127+127", "abc", "block1", "block2", "call-twice", "call2", "missing", "switch1"]
# for f in files:
# 	# print("./server < cases/"+f+"/server.in & ./client < cases/"+f+"/client1.in  & ./client < cases/"+f+"/client2.in")


test1: all 

	./rpc-server -p $(PORT) & ./rpc-client -i $(IP) -p $(PORT)
	

test2: all testing


	./server < cases/1+1/server.in     & ./client < cases/1+1/client.in
	
	./server < cases/127+127/server.in & ./client < cases/127+127/client.in
	
	./server < cases/abc/server.in     & ./client < cases/abc/client.in
	
	./server < cases/call-twice/server.in     & ./client < cases/call-twice/client.in

	./server < cases/call2/server.in     & ./client < cases/call2/client.in
	
	./server < cases/missing/server.in     & ./client < cases/missing/client.in
	
	./server < cases/switch1/server.in     & ./client < cases/switch1/client.in


	./server < cases/block1/server.in & ./client < cases/block1/client1.in  & ./client < cases/block1/client2.in
	./server < cases/block2/server.in & ./client < cases/block2/client1.in  & ./client < cases/block2/client2.in
	


test2_output: all testing

	make kill_port

	./server < cases/1+1/server.in  | diff - cases/1+1/server.out  & ./client < cases/1+1/client.in | diff - cases/1+1/client.out
	./server < cases/127+127/server.in | diff - cases/127+127/server.out & ./client < cases/127+127/client.in | diff - cases/127+127/client.out
	./server < cases/abc/server.in | diff - cases/abc/server.out & ./client < cases/abc/client.in  | diff - cases/abc/client.out

	./server < cases/call-twice/server.in | diff - cases/call-twice/server.out    & ./client < cases/call-twice/client.in | diff - cases/call-twice/client.out

	./server < cases/call2/server.in  | diff - cases/call2/server.out   & ./client < cases/call2/client.in | diff - cases/call2/client.out

	./server < cases/missing/server.in  | diff - cases/missing/server.out   & ./client < cases/missing/client.in | diff - cases/missing/client.out 
	
	./server < cases/switch1/server.in   | diff - cases/switch1/server.out   & ./client < cases/switch1/client.in  | diff - cases/switch1/client.out

	./server < cases/bad.server/server.in   | diff - cases/bad.server/server.out   & ./client < cases/bad.server/client.in  | diff - cases/bad.server/client.out
	./server < cases/bad.client/server.in   | diff - cases/bad.client/server.out   & ./client < cases/bad.client/client.in  | diff - cases/bad.client/client.out

	./server < cases/endian1/server.in   | diff - cases/endian1/server.out   & ./client < cases/endian1/client.in  | diff - cases/endian1/client.out
	./server < cases/endian2/server.in   | diff - cases/endian2/server.out   & ./client < cases/endian2/client.in  | diff - cases/endian2/client.out
	./server < cases/endian3/server.in   | diff - cases/endian3/server.out   & ./client < cases/endian3/client.in  | diff - cases/endian3/client.out


test2_output_blocking: all testing


	# blocking cases 
	# need to change DEBUG_NUM_CLIENTS to 2

	make kill_port

	./server < cases/block1/server.in | diff - cases/block1/server.out & ./client < cases/block1/client1.in | diff - cases/block1/client1.out  & ./client < cases/block1/client2.in | diff - cases/block1/client2.out
	./server < cases/block2/server.in | diff - cases/block2/server.out & ./client < cases/block2/client1.in | diff - cases/block2/client1.out  & ./client < cases/block2/client2.in | diff - cases/block2/client2.out


test2_save_blocking: all testing


	# blocking cases 
	# need to change DEBUG_NUM_CLIENTS to 2

	make kill_port

	./server < cases/block1/server.in | tee output/server.txt & ./client < cases/block1/client1.in | tee output/client1.txt  & ./client < cases/block1/client2.in | tee output/client2.txt
	./server < cases/block2/server.in | tee output/server.txt & ./client < cases/block2/client1.in | tee output/client1.txt  & ./client < cases/block2/client2.in | tee output/client2.txt


# new combinations of inputs
test3: all testing

	# TODO contains some which are not passing

	./server < cases_Port6000/1+1/server.in     & ./client < cases_Port6000/127+127/client.in 
	
	#./server < cases_Port6000/1+1/server.in     & ./client < cases_Port6000/abc/client.in 
	
	./server < cases_Port6000/1+1/server.in     & ./client < cases_Port6000/call-twice/client.in
	
	# ./server < cases_Port6000/1+1/server.in     & ./client < cases_Port6000/call2/client.in
	
	./server < cases_Port6000/1+1/server.in     & ./client < cases_Port6000/missing/client.in
	
	#./server < cases_Port6000/127+127/server.in     & ./client < cases_Port6000/abc/client.in
	
	./server < cases_Port6000/127+127/server.in     & ./client < cases_Port6000/call-twice/client.in
	
	# ./server < cases_Port6000/127+127/server.in     & ./client < cases_Port6000/call2/client.in
	
	./server < cases_Port6000/127+127/server.in     & ./client < cases_Port6000/missing/client.in
	
	# ./server < cases_Port6000/abc/server.in     & ./client < cases_Port6000/call-twice/client.in
	
	# ./server < cases_Port6000/abc/server.in     & ./client < cases_Port6000/call2/client.in
	
	./server < cases_Port6000/abc/server.in     & ./client < cases_Port6000/missing/client.in
	
	# ./server < cases_Port6000/call-twice/server.in     & ./client < cases_Port6000/call2/client.in
	
	./server < cases_Port6000/call-twice/server.in     & ./client < cases_Port6000/missing/client.in
	
	./server < cases_Port6000/call2/server.in     & ./client < cases_Port6000/missing/client.in






# test assert output TODO



kill_port: 

	# https://stackoverflow.com/questions/11583562/how-to-kill-a-process-running-on-particular-port-in-linux
	-fuser -k 3000/tcp
	-fuser -k 6000/tcp
	-fuser -k 6001/tcp
	-fuser -k 2923/tcp
