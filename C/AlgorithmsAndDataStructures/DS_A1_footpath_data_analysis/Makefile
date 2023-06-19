# modified from
# https://edstem.org/au/courses/9173/lessons/25174/slides/178431

# with inspiration taken from
# https://youtube.com/watch/?v=DtGrdB8wQ_8




CC = gcc
CFLAGS = -Wall -Werror -g
LDLIBS = 

C_SHARED = driver.c csv.c linked_list.c footpath.c search.c
C_STAGES = stage1.c stage2.c

O_SHARED = $(C_SHARED:.c=.o)
O_STAGES = $(C_STAGES:.c=.o)

C = $(C_SHARED) $(C_STAGES)
O = $(O_SHARED) $(O_STAGES)



# executables depend on object files
exe: $(O)
	$(CC) $(CFLAGS) -o dict1 $(O) $(LDLIBS)
	cp dict1 dict2

# object files depend on c files 
# ($^ is the c file name)
%.o: %.c 
	$(CC) $(CFLAGS) -c $^
	
# dict executable depends on stage and shared c files
dict1: exe
dict2: exe

clean:
	rm -f $(O) dict1 dict2
