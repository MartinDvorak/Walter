///////////////////////////////////////////////////////////////////////////////
//				Dvořák Martin 
//				xdvora2l 3BIT
//				ISA project Atom - 2018
///////////////////////////////////////////////////////////////////////////////

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

/**
 * @brief looking for last colon
 * @details last colon is need for seperation host and port if exist
 * 
 * @param host hostname with port
 * @return index of colon or zero
 */
int find_last_colon(char* host);

/**
 * @brief if set port in host, find int value of  port to structure get)adrress_t, port in host is deleted
 * 
 * @param hostname with port, after function port is eresed and set in variable packet
 * @param packet pointer to structere of new connection between pc and server. Function set the specific port
 */
void explicit_port(char* host,get_adrress_t* packet);

/**
 * @brief examinig path in host
 * @details examine path in host and set in variable packet colon path. Path in host is deleted
 * 
 * @param host hostname with port and path of file
 * @param packet pointer to structere of new connection between pc and server. Function set the path of file. 
 * @param right_ptr index in hostname array, and set the first character in path. 
 */
void set_path(char* host, get_adrress_t* packet,int right_ptr);

/**
 * @brief set host from host to packet
 * @details Host param is only name of server, the rest info like port number or file path are deleted in other function 
 * 
 * @param host only hostname 
 * @param packet pointer to structere of new connection between pc and server. Function set the name of server.
 * @param left_ptr index in char array host, set after string "https://" or "http://"
 * @param right_ptr inxdex in char array host, set after end of hostname.
 */
void set_host(char* host, get_adrress_t* packet, int left_ptr,int right_ptr);

/**
 * @brief function lookng for first slash
 * @details fuction looking for first slah in hostname (start of file path)
 * 
 * @param host hostname with path and port
 * @param left_ptr index in array hostname set after "https://" or "https://" string
 * 
 * @return integer value of first index of slash.
 */
int find_rptr(char* host,int left_ptr);

/**
 * @brief main function for set all information from hostname
 * @details set in return structure all information, that can get from hostname.Function calls other function above this function.
 * 
 * @param host hostname
 * @return set parametrs from hostname (port, host, path)
 */
get_adrress_t* examine_host(char* host);


/**
 * @brief reading line from file
 * @details fuction read a line from file specific in param and save it in row param. 
 * Function detect comments and end of file as well. 
 * 
 * @param row return line
 * @param feed_file FILE* pointer to read file
 * 
 * @return last char of row
 */
int get_line(char* row,FILE* feed_file);


/**
 * @brief function reads from file and get next line
 * @details in newt line is next host to comunicated with.
 * 
 * @param URL param from console line - specific hostname
 * @param f bool value if file is set
 * @param feed_file FILE* pointer to file. If f param is set.
 * @param host return hostname in param
 * @return integer error value, not returning any data. 
 */
int get_next_host(char* URL, bool f, FILE* feed_file,char** host);

/**
 * @brief set packet  header
 * @details in long string is set header of http/https communication. All infomation is set in param stucture.
 * 
 * @param packet structure where is specificed all params of cumunication
 * @return long head string is returned
 */
char* fill_packet(get_adrress_t* packet);

/**
 * @brief extrahing from get_adrress_t port 
 * @details get port from get_adrress_t host structure. 
 * looking for ":" and next to this value is host port for BIO structure
 * 
 * @param packet structure where is specificed all params of cumunication, specialy host 
 * @return string where is specified port number or NULL 
 */
char* get_host_port(get_adrress_t* packet);

/**
 * @brief read head and looking for transfer encoding
 * @details looking exactly string in header of response "Transfer-Encoding: chunked"
 * 
 * @param response in long string is response. Head of response ended with double "\r\n"
 * @return bool value, true if chunked is set or false if not
 */
bool transfer_chunk(char* response);

/**
 * @brief read head of response
 * @details check if response code is 200 -> OK. then looking for supported content type
 * 
 * @param response string with whole response.
 * @return integer value 0 if everything is OK and -1 if error ocured.
 */
int readhead(char* response);

/**
 * @brief read and convert chunk
 * @details read and convert the first chunk. This is first bytes after header of response.
 * 
 * 
 * @param recieved string with whole response.
 * @param num_bytes int value for offset 
 * 
 * @return int value with chunked pointer cnverted from string hexa code to int decimal.
 */
int read_chunk(char* recieved, int* num_bytes);

/**
 * @brief main function for send and receive packet
 * @details functionc set all params of communicarion in structure get_adrress_t. 
 * correctly receive response and maintain HTTPS communication.
 * 
 * @param http_get head of request 
 * @param packet structure wtih whole communication
 * @param c_flag if certificate is specified
 * @param C_flag if certificates folder is specified
 * @param certadd NULL or folder 
 * @param certfile NULL of file
 * @param xml_string set the response in this string
 * @return -1 when error ocured or 0 if everithing ok
 */
int send_packet(char* http_get, get_adrress_t* packet,bool c_flag,bool C_flag,char* certadd,char* certfile,char** xml_string);

/**
 * @brief check for header option in response
 * @details check supported header in response. The right response is predefined in string array
 * 
 * @param response string with whole response.
 * @return true if conntent type is supported
 */
bool check_content_type(char* response);

/**
 * @brief free all allocated sources in BIO communication
 * 
 * @param bio BIO socket
 * @param host_port string with host port
 * @param ctx BIO structure for communication https
 * @param https if true ctx is free
 */
void free_resources(BIO *bio,char* host_port,SSL_CTX* ctx, bool https);

/**
 * @brief debuging function for print response header
 * 
 * @param response string with whole response.
 */
void print_head(char* response);