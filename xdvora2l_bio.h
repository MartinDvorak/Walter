#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>

#define BIO_SIZE 4096 

#ifndef FLAG_H_
#define FLAG_H_
typedef struct {
	bool T_flag;
	bool a_flag;
	bool u_flag;
}flags_t;
#endif

typedef struct get_adrress{
	bool https;   // false == http
	char* host;
	char* path;
	int port;  // implicit 80 and 443

}get_adrress_t;

int find_last_colon(char* host);

void explicit_port(char* host,get_adrress_t* packet);

void set_path(char* host, get_adrress_t* packet,int right_ptr);

void set_host(char* host, get_adrress_t* packet, int left_ptr,int right_ptr);

int find_rptr(char* host,int left_ptr);

get_adrress_t* examine_host(char* host);

int get_line(char* row,FILE* feed_file);

int get_next_host(char* URL, bool f, FILE* feed_file,char** host);

char* fill_packet(get_adrress_t* packet);

char* get_host_port(get_adrress_t* packet);

bool transfer_chunk(char* response);

int readhead(char* response);

int read_chunk(char* recieved, int* num_bytes);

int send_packet(char* http_get, get_adrress_t* packet,bool c_flag,bool C_flag,char* certadd,char* certfile,char** xml_string);

bool check_content_type(char* response);

void free_resources(BIO *bio,char* host_port,SSL_CTX* ctx, bool https);

void print_head(char* response);