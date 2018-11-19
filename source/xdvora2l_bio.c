///////////////////////////////////////////////////////////////////////////////
//				Dvořák Martin 
//				xdvora2l 3BIT
//				ISA project Atom - 2018
///////////////////////////////////////////////////////////////////////////////

#include "xdvora2l_bio.h"

int find_last_colon(char* host)
{
	for(int i = strlen(host); i >= 0; i--)
	{
		if(host[i] == ':')
		{
			return i;
		}
	}
	return 0;
}
////as
void explicit_port(char* host,get_adrress_t* packet)
{
	regex_t regex_port;
	int reg_port;
	// regex prepera and use
	reg_port = regcomp(&regex_port, ":[0-9][0-9]*$",0);
	reg_port = regexec(&regex_port,host,0,NULL,0);
	if(reg_port != REG_NOMATCH)
	{
		int colon_ptr = find_last_colon(host);
		packet->port = atoi(&(host[colon_ptr+1]));
		host[colon_ptr] = '\0';
	}
	regfree(&regex_port);
}

void set_path(char* host, get_adrress_t* packet,int right_ptr)
{
	// cpy path to packet 
	if((strlen(host)-right_ptr+1) == 1)
	{
		packet->path = malloc(sizeof(char)*(2));
		strcpy(packet->path,"/");
	}
	else if((strlen(host)-right_ptr+1) > 0)
	{
		packet->path = malloc(sizeof(char)*(strlen(host)-right_ptr+1));
		strcpy(packet->path,&(host[right_ptr]));
		host[right_ptr] = '\0';
	}
}

void set_host(char* host, get_adrress_t* packet, int left_ptr,int right_ptr)
{   
	packet->host = malloc(sizeof(char)*(right_ptr- left_ptr +1));
	strcpy(packet->host,&(host[left_ptr]));

}

int find_rptr(char* host,int left_ptr)
{ // looking for slash first from right
	int right_ptr = strlen(host);
	for(int i = left_ptr; i < strlen(host);i++)
	{
		if(host[i] == '/')
		{
			right_ptr = i;
			break;
		}
	}
	return right_ptr;
}


get_adrress_t* examine_host(char* host)
{
	get_adrress_t* packet = NULL;

	packet = malloc(sizeof(get_adrress_t));
	if(packet == NULL)
		{exit(99);}
	packet->host = NULL;
	packet->path = NULL;

	int left_ptr;
	if(strncmp(host,"http://",strlen("http://")) == 0)
	{
		packet->https = false;
		packet->port = 80;
		left_ptr = strlen("http://");
		// protocol http
	}
	else if(strncmp(host,"https://",strlen("https://")) == 0)
	{
		packet->https = true;
		packet->port = 443;
		left_ptr = strlen("https://");

		// protocol https
	}
	else{
		fprintf(stderr, "ERR not valid URL\n");
		return NULL;
	}
	
	int right_ptr;
	if((right_ptr = find_rptr(host,left_ptr)) == 0)
	{
		fprintf(stderr, "ERR not valid URL\n");
		return NULL;
	}

	set_path(host,packet,right_ptr);	
	explicit_port(host,packet);

	set_host(host,packet,left_ptr,right_ptr);

	return packet;
}


int get_line(char* row,FILE* feed_file)
{
	int c;
	int i = 0;
	bool commet = false;
	do
	{
		c = fgetc(feed_file);
		//end of line or file
		if((c == '\n') ||(c == EOF))
		{
			row[i] = '\0';
			break;
   		}// remove coments in middle line
   		else if(c == '#')
   		{
   			commet = true;
   		}
   		else if(!commet){
   			row[i++] = c;
   		}

	}while (c != EOF);
	
	return c;

}

int get_next_host(char* URL, bool f, FILE* feed_file,char** host)
{
	char row[2000];
	int ret_value = 0;
	
	if(!f)
	{
		*host = malloc(sizeof(char)*(strlen(URL)+1));
		strcpy(*host,URL);
	}
	else{
		regex_t regex_space;
		regex_t regex_hashtag;
		int reg_space;
		int reg_hashtag;
		// empty line
		reg_space = regcomp(&regex_space, "^\x20*$",0);
		reg_hashtag = regcomp(&regex_hashtag, "^#.*$",0);
		
		do{	
			ret_value = get_line(row,feed_file);	
	    	
	    	reg_space = regexec(&regex_space,row,0,NULL,0);
			reg_hashtag = regexec(&regex_hashtag,row,0,NULL,0);
			if((reg_space == REG_NOMATCH) && (reg_hashtag == REG_NOMATCH))
			{
				*host = malloc(sizeof(char)*(strlen(row)+1));
				strcpy(*host,row);
				ret_value = strlen(*host);
				break;
			}
		}while(ret_value != EOF);		
	
		regfree(&regex_space);
		regfree(&regex_hashtag);
	}
	return ret_value;
}

char* fill_packet(get_adrress_t* packet)
{
	int count = 0;
	count += strlen("GET ");
	count += strlen(" HTTP/1.1\x0D\x0AHost: ");
	count += strlen("\x0D\x0A\x43onnection: Close\x0D\x0AUser-Agent: FeedreaderXdvora2l\x0D\x0A");
	count += strlen("Accept: xml\x0D\x0A"); // application/xml ?????
	count += strlen("Accept-Charset: utf-8\x0D\x0A\x0D\x0A");
	
	count+= strlen(packet->host) + strlen(packet->path) +1;

	char* get = NULL;
	get = malloc(sizeof(char)*(count));

	int offset = 0;

	strcpy(get,"GET ");
	offset += strlen("GET ");
	
	strcpy( &(get[offset]), packet->path);
	offset += strlen(packet->path);

	strcpy( &(get[offset])," HTTP/1.1\x0D\x0AHost: ");
	offset += strlen(" HTTP/1.1\x0D\x0AHost: ");

	strcpy( &(get[offset]), packet->host);
	offset += strlen(packet->host);

	strcpy( &(get[offset]), "\x0D\x0A\x43onnection: Close\x0D\x0AUser-Agent: FeedreaderXdvora2l\x0D\x0A");
	offset += strlen("\x0D\x0A\x43onnection: Close\x0D\x0AUser-Agent: FeedreaderXdvora2l\x0D\x0A");

	strcpy( &(get[offset]), "Accept: xml\x0D\x0A");
	offset += strlen("Accept: xml\x0D\x0A");

	strcpy( &(get[offset]), "Accept-Charset: utf-8\x0D\x0A\x0D\x0A");
	offset += strlen("Accept-Charset: utf-8\x0D\x0A\x0D\x0A");

	return get;
}


char* get_host_port(get_adrress_t* packet)
{
	char str_port[7];
	str_port[0] = ':';
	sprintf(&(str_port[1]), "%d", packet->port);
	int len = strlen(str_port)+strlen(packet->host)+1;
	char* host_port = NULL;
	host_port = malloc(sizeof(char)*len);
	if(host_port == NULL)
		{exit(99);}
	strcpy(host_port,packet->host);
	strcat(host_port,str_port);

	return host_port;	
}

bool transfer_chunk(char* response)
{
	for(int i=3;i < strlen(response); i++)
	{
		if((response[i-3] == '\r')&&(response[i-2] == '\n')
			&&(response[i-1] == '\r')
			&&(response[i] == '\n'))
			return false;
		else if((response[i-2] == '\r')&&(response[i-1] == '\n'))
		{
			if(!strncmp(&(response[i]),"Transfer-Encoding: chunked",strlen("Transfer-Encoding: chunked")))			
				return true;
		}
	}
	return false;
}

bool check_content_type(char* response)
{
	char supported [50][50] = {"text/xml", "application/xml","application/xhtml+xml",
		"application/xhtml+xml", "application/atom+xml"};

	for(int i=0;i <= 4 ;i++)
	{
		if(!strncmp(response,supported[i],strlen(supported[i])))
			return true;
	}

	return false;
}

void print_head(char* response)
{
	for(int i=3;i < strlen(response); i++)
	{
		if((response[i-3] == '\r')&&(response[i-2] == '\n')
			&&(response[i-1] == '\r')
			&&(response[i] == '\n'))
			{
				printf("\n");
				break;
			}
		else{
				printf("%c", response[i]);
		}
	}
}

int readhead(char* response)
{
	if(strncmp(response,"HTTP/1.1 200 OK",strlen("HTTP/1.1 200 OK")))
	{
		fprintf(stderr, "in Response ocured any error - returned code is not 200\n");
		for(int i=0; ;i++)
			{
				fprintf(stderr,"%c",response[i]);
				if (response[i] == '\n')
					break;
			}
		return -1;
	}


	for(int i=3;i < strlen(response); i++)
	{
		if((response[i-3] == '\r')&&(response[i-2] == '\n')
			&&(response[i-1] == '\r')
			&&(response[i] == '\n'))
			return i+1;
		
		else if((response[i-2] == '\r')&&(response[i-1] == '\n'))
		{
			if(!strncmp(&(response[i]),"Content-Type: ",strlen("Content-Type: ")))			
				if(!check_content_type(&(response[i+strlen("Content-Type: ")]) )) 
					{
						fprintf(stderr, "Content-Type is not supported\n");
						return -1;
					}
		}
	}

	return 0;
}

int read_chunk(char* recieved, int* num_bytes)
{
	*num_bytes = 2*2; // /r + /n
	char number[20];
	for(int i=0;;i++)
	{
		if((recieved[i] == '\r') || (recieved[i] == '\n'))
		{
			number[i] = '\0';
			break;
		}
		number[i] = recieved[i];
		recieved[i] = ' ';
		*num_bytes += 1;
	}
	return (int)strtol(number, NULL, 16);
}

void free_resources(BIO *bio,char* host_port,SSL_CTX* ctx, bool https)
{
	SSL_COMP_free_compression_methods(); /// clean SSL_library_init (); 
    EVP_cleanup(); /// clean OpenSSL_add_all_algorithms();
    ERR_free_strings(); /// clean ERR_load_BIO_strings();
    free(host_port);
    BIO_flush(bio);
    BIO_free_all(bio);
    if(https)
		SSL_CTX_free(ctx);
}

int send_packet(char* http_get, get_adrress_t* packet,bool c_flag,bool C_flag,char* certadd,char* certfile,char** xml_string)
{
	*xml_string = NULL;
	BIO * bio;
    SSL * ssl;
    SSL_CTX * ctx;
	char* host_port;
	/* Set up the library */
     ERR_load_BIO_strings();
     SSL_load_error_strings();
     OpenSSL_add_all_algorithms();
     SSL_library_init (); 

     bool debug = false;
    /*
	Some code (BIO cocket) are taken from recommended literature https://developer.ibm.com/tutorials/l-openssl/
	All rights belongs authors of this article. 	
    */
	if(packet->https)
	{
		 /* Set up the SSL context */
    	ctx = SSL_CTX_new(SSLv23_client_method());
		/* Load the trust store */
		if((c_flag) && (C_flag))
		{
			if((! SSL_CTX_load_verify_locations(ctx, certfile, NULL))&&
				(! SSL_CTX_load_verify_locations(ctx, NULL, certadd)))
			{
		        fprintf(stderr, "Error loading trust store\n");
		        ERR_print_errors_fp(stderr);
		        free_resources(bio,host_port,ctx,packet->https);
		        return -1;
			}	
		}
		else if(c_flag) 
		{
			if(! SSL_CTX_load_verify_locations(ctx, certfile, NULL))
			{
		        fprintf(stderr, "Error loading trust store\n");
		        ERR_print_errors_fp(stderr);
		        free_resources(bio,host_port,ctx,packet->https);
		        return -1;
			}
		}
   		else if(C_flag)
   		{
   			if(! SSL_CTX_load_verify_locations(ctx, NULL, certadd))
   			{
		        fprintf(stderr, "Error loading trust store\n");
		        ERR_print_errors_fp(stderr);
		        free_resources(bio,host_port,ctx,packet->https);
		        return -1;   				
   			}
   		}
   		else//if((c_flag  == 0) && (C_flag == 0)) 
   		{
   			if(!SSL_CTX_set_default_verify_paths(ctx))
	   	 	{
		        fprintf(stderr, "Error loading trust store\n");
		        ERR_print_errors_fp(stderr);
		        free_resources(bio,host_port,ctx,packet->https);
		        return -1;
		    }
		}
	    /* Setup the connection */

	    bio = BIO_new_ssl_connect(ctx);

	    /* Set the SSL_MODE_AUTO_RETRY flag */

	    BIO_get_ssl(bio, & ssl);
	    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	    /* Create and setup the connection */
		host_port = get_host_port(packet);
	    BIO_set_conn_hostname(bio, host_port); // TODO ??

	    if(BIO_do_connect(bio) <= 0)
	    {
	        fprintf(stderr, "Error attempting to connect\n");
	        //ERR_print_errors_fp(stderr);
	        free_resources(bio,host_port,ctx,packet->https);
	        return -1;
	    }

	    /* Check the certificate */

	    if(SSL_get_verify_result(ssl) != X509_V_OK)
	    {
	        fprintf(stderr, "Certificate verification error: %li\n", SSL_get_verify_result(ssl));
	        fprintf(stderr, "Cannot verify the connectin, connection is going to close\n");
	        free_resources(bio,host_port,ctx,packet->https);
	        return -1;
	    }
	}
	else{
        /* Create and setup the connection */

	host_port = get_host_port(packet);
	bio = BIO_new_connect(host_port); // leak 416byte 6blocks
    
    if(bio == NULL) { fprintf(stderr,"BIO is null\n"); return -1; }

    if(BIO_do_connect(bio) <= 0)
    {
        //ERR_print_errors_fp(stderr);
        fprintf(stderr, "Error attempting to connect\n");
	    free_resources(bio,host_port,ctx,packet->https);
        return -1;
    }

    }

    // write socket
   	BIO_write(bio, http_get, strlen(http_get));
   
	char bio_read[BIO_SIZE];
  	int byte_read; 

   	char* response = NULL;
   	int  response_size = 0;
	// read response in socket
	for(;;)
    {
       	memset(bio_read,0,BIO_SIZE);
       	byte_read = BIO_read(bio, bio_read, BIO_SIZE/2);
        if(byte_read <= 0) break;
        bio_read[byte_read] = 0;

        response = realloc(response, response_size + byte_read);
        memcpy(&(response[response_size]),bio_read,byte_read);

        response_size += byte_read;
        
        if(debug)
       	{
       		printf("%s\n",bio_read );
    	}
    }

    response = realloc(response,response_size+1);
    response[response_size] = '\0';
    
    int chunk_ptr = 0;
    int chunk_value;
    int chunk_bytes = 0;

    if(debug)
    {
    	print_head(response);
    }

    if((chunk_ptr = readhead(response)) == -1)
    {
    	free(response);
    	free_resources(bio,host_port,ctx,packet->https);
    	return -1;
    }
    char* xml = malloc(sizeof(char)*strlen(response));
    int xml_ptr = 0;

    if(transfer_chunk(response))
    {
    	while(chunk_ptr < response_size)
		{
		    chunk_value = read_chunk(&(response[chunk_ptr]),&chunk_bytes);
		 
		    memcpy(&(xml[xml_ptr]),&(response[chunk_ptr+chunk_bytes-2]),chunk_value);
		    xml_ptr += chunk_value;

		    chunk_ptr += chunk_value + chunk_bytes;

			//printf(">>>%c%c%c%c<<<\n",response[chunk_ptr-1],response[chunk_ptr],response[chunk_ptr+1],response[chunk_ptr+2]);
		   	//printf(">>>%d<<<\n", chunk_value);	    
		}
		xml[xml_ptr] = '\0';
	}
	else{
		memcpy(xml,&(response[chunk_ptr]),strlen(response)-chunk_ptr);
		xml[strlen(response)-chunk_ptr] = '\0';
	}
	//printf("%s\n", xml);
	*xml_string = xml;
	
	free(response);
	free_resources(bio,host_port,ctx,packet->https);
   	//free(response);
    /* Close the connection and free the context */
    // 	SSL_COMP_free_compression_methods(); /// clean SSL_library_init (); 
    //     EVP_cleanup(); /// clean OpenSSL_add_all_algorithms();
    //     ERR_free_strings(); /// clean ERR_load_BIO_strings();
    //     free(host_port);
    //     BIO_flush(bio);
    //     BIO_free_all(bio);
    //     if(packet->https)
    // 		SSL_CTX_free(ctx);

	return 0;
}
