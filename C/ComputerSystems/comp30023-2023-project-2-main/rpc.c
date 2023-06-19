// Credit: code taken from W9 workshop


#define _POSIX_C_SOURCE 200112L
#define _DEFAULT_SOURCE

#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <pthread.h> 


#include <endian.h>


#include "rpc.h"
#include "global.h"
#include "ignored.h"

#define UNKNOWN_LEN 0

#define MAX_NUM_REQUESTS 5
#define MAX_REGISTERED_FUNCTIONS 15
#define NONBLOCKING
#define DATA2_LEN_MAX 100000


// define send and recieve ids to assert they match 1 to 1 in safe send and recieve
enum {
	COMMAND_ID,
	SEND_RPC_DATA_NULL,
	SEND_RPC_DATA_DATA1,
	SEND_RPC_DATA_DATA2_LEN,
	SEND_RPC_DATA_DATA2,
	FIND_FUNCTION_INDEX,
	CALL_FUNCTION_INDEX,
	FIND_FUNCTION_NAME,
};

////////

// use macros to create controlable debug print function 

#if DEBUG_PRINT
	#define dbg_print printf
#endif

#if !DEBUG_PRINT
void dbg_print(const char* format, ...) {
	assert(format);
}
#endif

////////

void print_data2(rpc_data *data) {

	if (!data) { return;}

	byte_t *bytes = data->data2;

		dbg_print("data2 bytes (%li) : ", data->data2_len);
		for (size_t i = 0; i < data->data2_len; i++) {
			dbg_print("[%li] > %i, ", i, bytes[i]);
		}
		dbg_print("\n");

}


void print_data(rpc_data *data) {

	if (data) {
		dbg_print("{%i %lu %p}	", data->data1, data->data2_len, data->data2);
	} else {
		dbg_print("Data is NULL");
	}

	dbg_print("\n");
}


bool is_valid_rpc_data(rpc_data *data) {
	if (!data) { return true; }


	bool has_data2 = (data->data2 != NULL);
	bool has_data2_len = (data->data2_len != 0);

	if (has_data2 != has_data2_len) {
	
		dbg_print("Error: data is invalid!\n");
		print_data(data);
		return false;
	}
	
	return true;
}


int create_listening_socket(int port) {
	int re, s, incoming_connections_fd;
	struct addrinfo hints, *res;

	// Create address we're going to listen on (with given port number)
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6;       // IPv4
	hints.ai_socktype = SOCK_STREAM; // Connection-mode byte streams
	hints.ai_flags = AI_PASSIVE;     // for bind, listen, accept
	
	// node (NULL means any interface), service (port), hints, res

	char service[10]; // port length is no longer than 10 chars
	sprintf(service, "%i", port);

	s = getaddrinfo(NULL, service, &hints, &res);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return -1;
	}
 
	// Create socket
	incoming_connections_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (incoming_connections_fd < 0) {
		perror("socket");
		return -1;
	}

	// Reuse port if possiblef
	re = 1;
	if (setsockopt(incoming_connections_fd, SOL_SOCKET, SO_REUSEADDR, &re, sizeof(int)) < 0) {
		perror("setsockopt");
		return -1;
	}
	// Bind address to the socket
	if (bind(incoming_connections_fd, res->ai_addr, res->ai_addrlen) < 0) {
		perror("bind");
		return -1;
	}
	freeaddrinfo(res);

	return incoming_connections_fd;
}


size_t safe_read(int fd, byte_t *buffer, long nbytes) {
	assert(buffer);
	assert(nbytes > 0);

  long n;
	while ((n = read(fd, (void *) buffer, nbytes)) != nbytes) {
		#if DEBUG_CLOSE
		fprintf(stderr, "Error, read returned %li != %lu\n", n, nbytes);
		exit(EXIT_FAILURE);
		#endif
	}

  return n;
}



size_t safe_write(int fd, byte_t *buffer, size_t nbytes) {
	assert(buffer);

  size_t n = write(fd, (void *) buffer, nbytes);
	if (n != nbytes) {
		fprintf(stderr, "Error, write returned %lu != %lu\n", n, nbytes);
		exit(EXIT_FAILURE);
	}

  return n;
}


// send and recieve data according to protocol

bool safe_send(int socket_fd, byte_t *message, size_t len, int sender_id) {

	// message passed in could have only read access and so for byte ordering, this data needs to be copied

	assert(len);

	dbg_print("		[safe_send] sending size of incoming message %lu\n", len);

	if (!message) { 

		dbg_print("		[safe_send] sending NULL\n");
		len = 0; 
	}

	safe_write(socket_fd, (byte_t *) &len, sizeof(len)); // send size of incoming message

	// verification byte is implimented to avoid byte ordering errors

	size_t len_verification;
	dbg_print("		[safe_send] recieving verification byte\n");
	safe_read(socket_fd, (byte_t *) &len_verification, sizeof(len_verification)); // read verification (len - 1)

	dbg_print("		[safe_send] recieved verification byte %lu\n", len_verification);
	if (len - 1 != len_verification) {
		printf("ERROR: Verification byte failed \nlen - 1 != len_verification \n%lu != %lu \n\n", len - 1, len_verification);
	}

	safe_write(socket_fd, (byte_t *) &sender_id, sizeof(sender_id)); // send message

	dbg_print("		[safe_send] sending message\n");

	safe_write(socket_fd, (byte_t *) message, len); // send message

	dbg_print("		[safe_send] message sent\n");

	return true;
}


void *safe_recieve(int data_fd, size_t expected_len, int receiver_id) {

	// read and reply len of incoming message

	dbg_print("		[safe_recieve] recieving size of incoming message\n");

	size_t len;
	safe_read(data_fd, (byte_t *) &len, sizeof(len));
	if (expected_len && len) { // if variable length
		if (expected_len != len) {
			printf("Error: expected_len != len\n");
			printf("       %lu != %lu\n", expected_len, len);
			exit(1);
		}
	}

	if (len == 0) {
		// message was NULL
		dbg_print("		[safe_recieve] recieved NULL\n");
		return NULL;
	}

	size_t len_verification = len - 1; 

	dbg_print("		[safe_recieve] message is %lu bytes, replying with verification %lu\n", len, len_verification);

	safe_write(data_fd, (byte_t *) &len_verification, sizeof(len_verification));

	// malloc space required buffer size and read

	dbg_print("		[safe_recieve] receive data of size %li\n", len);	

	int sender_id;
	safe_read(data_fd, (byte_t *) &sender_id, sizeof(int));
	if (sender_id != receiver_id) {
		printf("Error: sender_id != receiver_id\n");
		printf("       %i != %i\n\n", sender_id, receiver_id);
		exit(1);
	}

	byte_t *buffer = malloc(len);
	memset(buffer, 0, len);
	safe_read(data_fd, buffer, len);

	dbg_print("		[safe_recieve] message recieved\n");

	/*
	if (reverse_byte_ordering_bools[sender_id]) {
		reverse_endian_if_little(buffer, len);
	}
	*/

	return buffer;
}



void safe_recieve_into(int data_fd, size_t nbytes, void *dest, int receiver_id) {
	assert(nbytes);

	// write into pointer

	void *src = safe_recieve(data_fd, nbytes, receiver_id);
	assert(src);

	memcpy(dest, src, nbytes);
	free(src);
}

//


void send_rpc_data(int data_fd, rpc_data *data, char *name) {

	assert(name);

	if (!is_valid_rpc_data(data)) {
		// invalid data
		data = NULL;
	}

	if (data && !(data->data2_len < DATA2_LEN_MAX)) {
		fprintf(stderr, "Overlength error");
		exit(1);
	}

	dbg_print("%s sending rpc_data \n", name);
	print_data(data);

	bool is_null = (data == NULL);
	safe_send(data_fd, (byte_t *) &is_null, sizeof(is_null), SEND_RPC_DATA_NULL);
	if (is_null) { return; }
	

	int64_t d1 = data->data1;
	d1 = htobe64(d1);
	safe_send(data_fd, (byte_t *) &d1, sizeof(d1), SEND_RPC_DATA_DATA1);

	uint32_t d2 = data->data2_len; // only needs to encode unsigned lenth, so 32
	d2 = htobe32(d2);
	safe_send(data_fd, (byte_t *) &d2, sizeof(d2), SEND_RPC_DATA_DATA2_LEN);

	if (data && data->data2_len) {

		dbg_print("%s sending extra bytes\n", name);
		print_data2(data);

		safe_send(data_fd, data->data2, data->data2_len, SEND_RPC_DATA_DATA2);
	}

}





rpc_data *recieve_rpc_data(int data_fd, char *name) {

	bool is_null;
	safe_recieve_into(data_fd, sizeof(is_null), &is_null, SEND_RPC_DATA_NULL);
	if (is_null) { return NULL; }

	rpc_data *data = malloc(sizeof(rpc_data));
	
	int64_t d1;
	safe_recieve_into(data_fd, sizeof(d1), &d1, SEND_RPC_DATA_DATA1);
	data->data1 = be64toh(d1);
	
	uint32_t d2;
	safe_recieve_into(data_fd, sizeof(d2), &d2, SEND_RPC_DATA_DATA2_LEN);
	data->data2_len = be32toh(d2);

	dbg_print("%s recieved rpc_data \n", name);

	if (data->data2_len) {
		dbg_print("%s reading %lu extra bytes\n", name, data->data2_len);

		data->data2 = safe_recieve(data_fd, data->data2_len, SEND_RPC_DATA_DATA2);
	} else {
		data->data2 = NULL;
	}

	print_data(data);
	print_data2(data);

	return data;
}



bool send_string(int data_fd, char *string, int sender_id) {

	size_t len = strlen(string);
	assert(len != 0);
	assert(string[len] == '\0');

	size_t nbytes = len + 1;

	return safe_send(data_fd, (byte_t *) string, nbytes, sender_id);
}


////////


struct rpc_server {
    /* Add variable(s) for server state */

    size_t num_functions;
    rpc_handler functions[MAX_REGISTERED_FUNCTIONS]; // procedures (function pointers)
    char *function_names[MAX_REGISTERED_FUNCTIONS]; // procedures (function pointers)

		int incoming_connections_fd;		
};

rpc_server *rpc_init_server(int port) {

	rpc_server *server = malloc(sizeof(rpc_server));
	server->num_functions = 0;
	server->incoming_connections_fd = create_listening_socket(port);

	if (server->incoming_connections_fd == -1) { return NULL; }  

	memset(&server->functions, 0, sizeof(void *) * MAX_REGISTERED_FUNCTIONS);

	// Listen on socket - means we're ready to accept connections,
	// incoming connection requests will be queued, man 3 listen
	if (listen(server->incoming_connections_fd, MAX_NUM_REQUESTS) < 0) {
		perror("listen");
		return NULL;
	}

	return server;
}

int rpc_register(rpc_server *server, char *name, rpc_handler handler) {
	// handler is a function pointer

	if (!server || !name || !handler || server->num_functions == MAX_REGISTERED_FUNCTIONS) { return NEG_ERROR; }

	dbg_print("[server] registering function %s\n", name);
	
	size_t i = server->num_functions;
	server->functions[i] = handler;
	server->num_functions++;

  int nchars = strlen(name) + 1;
  server->function_names[i] = malloc(nchars);
  sprintf(server->function_names[i], name, nchars);
	
	return 1;
}


struct rpc_handle {
    /* Add variable(s) for handle */

    uint32_t function_indx; // the handle uniquely identifies a function (remote procedure)
};


typedef struct {
	rpc_server *server;
	int32_t data_fd;
} rpc_serve_t;


void *rpc_serve(void *void_pointer) { // serve a single client

	rpc_serve_t *rpc_serve_data = void_pointer;

	int32_t data_fd = rpc_serve_data->data_fd;
	rpc_server *server = rpc_serve_data->server;


	while (true) {

		dbg_print("[server thread %i] rpc_serve_all receiving message type\n", data_fd);

		command_t command = -1;
		safe_recieve_into(data_fd, sizeof(command_t), &command, COMMAND_ID);

		assert(command < NUM_COMMANDS);

		if (command == FIND) { 

			dbg_print("[server thread %i] recieved FIND\n", data_fd); 

			char *function_name = safe_recieve(data_fd, UNKNOWN_LEN, FIND_FUNCTION_NAME);

			dbg_print("[server thread %i] client has requested function %s len %li\n", data_fd, function_name, strlen(function_name));

			int function_indx_pp = 0; // 0 if not found, else index + 1 (only requires one value to be sent) (pp is ++)

			dbg_print("[server thread %i] checking %lu functions to name %s\n", data_fd, server->num_functions, function_name); 
			for (size_t i = 0; i < server->num_functions; i++) {
				
				dbg_print("[server thread %i] 		strcpm function with name %s len %li\n", data_fd, server->function_names[i], strlen(server->function_names[i])); 

				int cpm = strcmp(server->function_names[i], function_name);
				if (cpm == 0) {
					dbg_print("[server thread %i] function found\n", data_fd); 
					function_indx_pp = i + 1;
					break;
				}
			}

			free(function_name);
			if (!function_indx_pp) { dbg_print("[server thread %i] no function found\n", data_fd); }

			safe_send(data_fd, (byte_t *) &function_indx_pp, sizeof(function_indx_pp), FIND_FUNCTION_INDEX);

		}	

		if (command == CALL) { 
			dbg_print("[server thread %i] recieved CALL\n", data_fd); 

			dbg_print("[server thread %i] recieving handle\n", data_fd); 
			
			// TODO change

			rpc_handle handle;
			safe_recieve_into(data_fd, sizeof(handle.function_indx), &handle.function_indx, CALL_FUNCTION_INDEX);
			assert(handle.function_indx < server->num_functions);

			dbg_print("[server thread %i] recieving data\n", data_fd); 

			rpc_data *payload = recieve_rpc_data(data_fd, "[server]");

			rpc_handler handler_func = server->functions[handle.function_indx];

			dbg_print("[server thread %i] doing computation\n", data_fd); 

			rpc_data *response_data = NULL;
			if (is_valid_rpc_data(response_data) && payload != NULL) {
				response_data = handler_func(payload);
			}

			dbg_print("[server thread %i] computations finished \n", data_fd);

			dbg_print("[server thread %i] sending results\n", data_fd); 

			send_rpc_data(data_fd, response_data, "[server thread]");

			if (response_data) { free(response_data); }
			if (payload && payload->data2) { free(payload->data2); }

			free(payload);

			dbg_print("[server thread %i] CALL fin\n", data_fd); 
		}

		if (command == CLOSE)    { 
			dbg_print("[server thread %i] CLOSED was recieved\n", data_fd); 
			return NULL; // close thread
		}

	}
}





void rpc_serve_all(rpc_server *server) {

	if (!server) { 
		dbg_print("[server] server was null\n");
		return; 
	}

	dbg_print("[server] rpc_serve_all called\n");
	
	struct sockaddr_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	size_t n_clients = 0;

	#ifdef NONBLOCKING
	pthread_t thread;
	#endif

	rpc_serve_t rpc_serve_data;
	rpc_serve_data.server = server;
	
	while (true) {

		//dbg_print("[server] accept() called\n");

		// Accept a connection - blocks until a connection is ready to be accepted
		// Get back a new file descriptor to communicate on 

		rpc_serve_data.data_fd = accept(server->incoming_connections_fd, (struct sockaddr*)&client_addr, &client_addr_size);
		dbg_print("[server] accept() returned %i\n", rpc_serve_data.data_fd);

		if (rpc_serve_data.data_fd > 0) { 
			
			dbg_print("[server] calling rpc_serve\n");

			#ifdef NONBLOCKING

			if(pthread_create(&thread, NULL, rpc_serve, &rpc_serve_data) < 0) {
				dbg_print("[server] could not create thread\n");
				return;
      }

			// https://edstem.org/au/courses/10873/discussion/1366957
			if (pthread_detach(thread) != 0) {
				dbg_print("[server] could not detatch thread\n");
				return;
			}

			#endif


			#ifndef NONBLOCKING	
			rpc_serve(&rpc_serve_data);
			#endif

			n_clients++;
			dbg_print("[server] has served %lu clients\n", n_clients);
		}


		#if DEBUG_CLOSE

		size_t n_clients_to_close_after = 1;

		#ifdef NONBLOCKING
		n_clients_to_close_after = 2;
		#endif

		if (n_clients == n_clients_to_close_after) { 

			// combine threads
			dbg_print("[server] exiting after sleep, accepted %lu clients\n", n_clients);
			sleep(30);

			dbg_print("[server] exit\n");
			return;
		}

		#endif
	}




}

struct rpc_client {
    /* Add variable(s) for client state */

		int data_fd;
};


rpc_client *rpc_init_client(char *addr, int port) {

	rpc_client *client = malloc(sizeof(rpc_client));

	sleep(1); 

	// Create address

	// Get addrinfo of server. From man page:
	// The getaddrinfo() function combines the functionality provided by the
	// gethostbyname(3) and getservbyname(3) functions into a single interface

	char str_port[10];
	sprintf(str_port, "%i", port);

	struct addrinfo hints = {0};
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo *servinfo;
	int s = getaddrinfo(addr, str_port, &hints, &servinfo);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		return NULL;
	}

	// Connect to first valid result
	// Why are there multiple results? see man page (search 'several reasons')
	// How to search? enter /, then text to search for, press n/N to navigate

	struct addrinfo *rp;
	for (rp = servinfo; rp != NULL; rp = rp->ai_next) {
		client->data_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (client->data_fd == -1)
			continue;

		if (connect(client->data_fd, rp->ai_addr, rp->ai_addrlen) != -1)
			break; // success

		close(client->data_fd); // 
	}
	
	if (rp == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return NULL;
	}
	
	freeaddrinfo(servinfo);

	return client;
}



rpc_handle *rpc_find(rpc_client *client, char *name) {
	// client attempting to find function

	if (!client || !name) { return NULL; }

	dbg_print("     [client] rpc_find requests function %s\n", name);

	////

	command_t command = FIND;
	dbg_print("     [client] send FIND\n");
	safe_send(client->data_fd, (byte_t *) &command, sizeof(command_t), COMMAND_ID);

	dbg_print("     [client] send funtion name %s\n", name);

	send_string(client->data_fd, name, FIND_FUNCTION_NAME);

	int *p_int = safe_recieve(client->data_fd, sizeof(int), FIND_FUNCTION_INDEX);
	if (!p_int) {
		return NULL;
	}

	int function_indx_pp = *p_int;
	free(p_int);

	if (function_indx_pp) {
		rpc_handle *handle = malloc(sizeof(rpc_handle));
		memset(handle, 0, sizeof(rpc_handle));

		handle->function_indx = function_indx_pp - 1;

		dbg_print("     [client] rpc_find fin - function found \n");
		return handle;
	}

	dbg_print("     [client] rpc_find fin - no function found \n");
	return NULL;
}

rpc_data *rpc_call(rpc_client *client, rpc_handle *handle, rpc_data *payload) {
	// client calling function

	if (!client || !handle) { return NULL; }
	if (!is_valid_rpc_data(payload)) { return NULL; }

	// send command
	dbg_print("     [client] sending CALL\n");
	command_t command = CALL;
	safe_send(client->data_fd, (byte_t *) &command, sizeof(command), COMMAND_ID);

	// send handle (function) and payload for function to use
	safe_send(client->data_fd, (byte_t *) &handle->function_indx, sizeof(handle->function_indx), CALL_FUNCTION_INDEX);

	send_rpc_data(client->data_fd, payload, "     [client]");

	// recieve response_data
	rpc_data *response_data = recieve_rpc_data(client->data_fd, "     [client]");
	
	dbg_print("     [client] rpc_call fin\n");
	return response_data;
}

void rpc_close_client(rpc_client *client) {

	if (!client) { return; }

	command_t command = CLOSE;
	dbg_print("     [client] sending CLOSE\n");
	if (safe_send(client->data_fd, (byte_t *) &command, sizeof(command), COMMAND_ID)) {
		close(client->data_fd);
	}

	free(client);
}

void rpc_data_free(rpc_data *data) {
    if (data == NULL) {
        return;
    }

    if (data->data2 != NULL) {
			free(data->data2);
    }

    free(data);
}
