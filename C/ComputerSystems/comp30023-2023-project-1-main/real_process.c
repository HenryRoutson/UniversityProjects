

#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "types.h"

#ifdef IMPLEMENTS_REAL_PROCESS




long long return_pipe_num_bytes(int *pipe) {

  struct stat stats;
  if (fstat(pipe[FILE_READ_INDX], &stats) == -1) {
    perror("fstat");
    return 1;
  }

  return stats.st_size;
}





byte_t get_byte_at_offset(void *data, uint_t byte_index) {
  byte_t byte = *((byte_t *) data + byte_index);
  return byte;
}



bool is_system_big_endian() {
  
  int16_t i = 1; 
  
  // byte0    byte1
  // 00000000 00000001 in big endian   > 0 1
  // 00000001 00000000 in small endian > 1 0
  
  // https://stackoverflow.com/questions/8680220/how-to-get-the-value-of-individual-bytes-of-a-variable
  
  byte_t byte0 = get_byte_at_offset(&i, 0);
  byte_t byte1 = get_byte_at_offset(&i, 1);
  
  assert(byte0 <= 1); 
  assert(byte1 <= 1); 
  assert(byte0 ^ byte1); // XOR
  return byte1; 
}


void byte_reverse(char *start, int num_bytes) {
  assert(start);
  assert(num_bytes > 1);
  
  char stored;
  
  for (int i = 0; i < num_bytes / 2; i++) {
    stored = start[i];    
    start[i] = start[num_bytes - 1 - i];
    start[num_bytes - 1 - i] = stored;
  }
}


void convert_to_big_endian(void *start, int num_bytes) {
	if (!is_system_big_endian()) {
		byte_reverse((char *) start, num_bytes);
	}
}






void signal_process(int signal) {

  assert(signal != SIGSTOP);
  assert(signal);
  assert(running_process);
  assert(running_process->pid != -1);

  if (DEBUG_PRINTS2) {
    printf("SIGNALLING PROCESS %i with STOP %i CONTINUE %i TERM %i\n", running_process->pid,  signal == SIGTSTP, signal == SIGCONT, signal == SIGTERM);
  }

  int k_ret = kill(running_process->pid, signal);
  assert(k_ret != -1);
}




/*
Pipe creates an in memory file, and returns file descriptors to use this file
writing to a pipe stores data in this file, and reading reads from the file

A pipe can be read or writen to from any process, including from the same process
To avoid issues with interprocess communication
A pipe should be created for each direction of communication
*/


void assert_valid_pipes() {

  assert(to_process_pipe[0] > 0);
  assert(to_process_pipe[1] > 0);

  assert(from_process_pipe[0] > 0);
  assert(from_process_pipe[1] > 0);
}


void setup_pipes() {

  // 
  int pipe_ret;
  pipe_ret = pipe(to_process_pipe);    assert(pipe_ret != -1);
  pipe_ret = pipe(from_process_pipe);  assert(pipe_ret != -1);
  assert_valid_pipes();

}



byte_t time_signal(int signal) {

  assert(running_process);

  // send time and then signal to change process state
  // verification byte is returned

  uint_t cur_time_cpy = cur_time;
  convert_to_big_endian(&cur_time_cpy, sizeof(uint_t)); 
  assert(sizeof(uint_t) == 4);

  int ret;

  assert(return_pipe_num_bytes(from_process_pipe) == 0); 


  if (DEBUG_PRINTS2) {
    printf("\nWRITE TIME for process %i with ver byte %02x", running_process->pid, get_byte_at_offset(&cur_time_cpy, 4-1));
  }

  ret = write(to_process_pipe[FILE_WRITE_INDX], &cur_time_cpy, sizeof(uint_t)); 

  if (DEBUG_PRINTS2) {
    printf("\nDONE WRITE TIME for process %i with ver byte %02x\n", running_process->pid, get_byte_at_offset(&cur_time_cpy, 4-1));
  }
  
  assert(ret == 4);

  uint_t nbytes = return_pipe_num_bytes(from_process_pipe);
  assert(0 <= nbytes && nbytes <= sizeof(uint_t));

  if (signal != 0) {
    signal_process(signal);
  }

  return get_byte_at_offset(&cur_time_cpy, 4-1);
}



byte_t read_char_from_process() {

  byte_t c;
  int ret;

  uint_t nbytes = return_pipe_num_bytes(from_process_pipe);
  assert(nbytes == 1 || nbytes == 0); // could be equal to zero if byte isn't sent yet

  if (DEBUG_PRINTS2) {
    printf("\nMain is reading 1 byte for process %i\n", running_process->pid); 
  }

  ret = read(from_process_pipe[FILE_READ_INDX], &c, sizeof(char));
  assert(ret == 1);

  if (DEBUG_PRINTS2) {
    printf("\nMain finished reading 1 byte, %02x for process %i\n", c , running_process->pid); 
  }

  assert(return_pipe_num_bytes(from_process_pipe) == 0);

  return c;
}




void verified_time_signal(int signal) {

  assert(signal != SIGTERM);
  assert(signal != SIGSTOP);

  byte_t sent_byte = time_signal(signal);
  
  assert(running_process->pid != -1);
  int k_ret = kill(running_process->pid, signal);
  assert(k_ret != -1);

  byte_t recieved_byte = read_char_from_process(); // TODO

  if (DEBUG_PRINTS2) {
    //printf("\nSENT |%02x| RECIEVED |%02x| \n", sent_byte, recieved_byte);
  }

  assert(sent_byte == recieved_byte); // checked for verification

}





void create_process() {

  process_count__START_++;

  assert(running_process);
  assert(running_process->pid == -1);
  assert(running_process->service_time == 
         running_process->service_time_unchanged);

  assert_valid_pipes();

  // fork
  pid_t fork_pid = -1;

  fork_pid = fork(); 
  assert(fork_pid >= 0);

  if (fork_pid == 0) {

    // dup2 for pipe redirection ----

    assert(return_pipe_num_bytes(  to_process_pipe) == 0); 
    assert(return_pipe_num_bytes(from_process_pipe) == 0); 

    dup2(  to_process_pipe[FILE_READ_INDX],  STDIN_FILENO );
    dup2(from_process_pipe[FILE_WRITE_INDX], STDOUT_FILENO);

    close(  to_process_pipe[FILE_WRITE_INDX]);
    close(from_process_pipe[FILE_READ_INDX]);

    // process execution line
    execl("process", "process",
     "-v", 
     running_process->process_name, NULL); 
    assert(false); 
  }

  // main execution line
  running_process->pid = fork_pid; 

  if (DEBUG_PRINTS2) {
    printf("CREATE PROCESS %i", fork_pid);
  }
  
  verified_time_signal(0);
}




void suspend_process() {

  process_count_SIGTSTP++;

  assert(running_process);
  assert(running_process->pid != -1);

  time_signal(SIGTSTP);

  // wait on a particular process, returns seconds waited

  // The WUNTRACED flag indicates that the waitpid() should also return if a child has stopped (as opposed to just terminated or resumed execution)

  assert(running_process->pid != -1);

  // WIFSTOPPED is a macro to test if the process has been sent the signal to stop

  if (DEBUG_PRINTS2) { printf("SIGTSTP START process %i\n", running_process->pid); }

  int wstatus = -1;
  while (true) {
    int ret = waitpid(running_process->pid, &wstatus, WUNTRACED); // See $ man 2 waitpid 
    assert(ret != -1);
    if (ret != 0) { 
      assert(WIFSTOPPED(wstatus));
      break; 
    }
  };

  if (DEBUG_PRINTS2) { printf("SIGTSTP DONE  process %i\n", running_process->pid); }

}


void terminate_process(char *sha) {

  process_count_SIGTERM++;

  assert(running_process);
  assert(running_process->pid != -1);

  assert(return_pipe_num_bytes(  to_process_pipe) == 0); 
  assert(return_pipe_num_bytes(from_process_pipe) == 0); 

  time_signal(SIGTERM);

  int ret;

  uint_t nbytes = return_pipe_num_bytes(from_process_pipe);
  assert(0 <= nbytes && nbytes <= NUM_SHA_BYTES);

  ret = read(from_process_pipe[FILE_READ_INDX], sha, NUM_SHA_BYTES);

  assert(return_pipe_num_bytes(from_process_pipe) == 0);

  if (ret == 1) { printf("\n\n\nTERMINATE READ BYTE INTO SHA %02x\n\n\n", sha[0]); }
  assert(ret != 1); // check if reads verification byte
  assert(ret == NUM_SHA_BYTES);
  assert(sha[NUM_SHA_BYTES - 1] == '\n'); 

  sha[NUM_SHA_BYTES - 1] = '\0';  // add null pointer 
  assert(strlen(sha) == NUM_SHA_BYTES - 1);


  if (DEBUG_PRINTS2) { printf("SIGTERM START process %i\n", running_process->pid); }
  int child_pid = wait(NULL);
  assert(child_pid == running_process->pid);
  if (DEBUG_PRINTS2) { printf("SIGTERM DONE  process %i\n", running_process->pid); }

}




void continue_process() {

  process_count_SIGCONT++;
  verified_time_signal(SIGCONT);
}







#endif

