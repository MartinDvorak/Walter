///////////////////////////////////////////////////////////////////////////////
//				Dvořák Martin 
//				xdvora2l 3BIT
//				ISA project Atom - 2018
///////////////////////////////////////////////////////////////////////////////

#include "xdvora2l_bio.h"
#include "xdvora2l_xml.h"

void print_help(){
	printf("HELP\n");
	printf("made by xdvora2l 	Martin Dvorak	www.fit.vutbr.cz/.en\n");
	printf("feedreader to course ISA 2018/2019\n");
	printf("\n");
	printf("comand line parameters are:\n");
	printf("feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]\n");
	printf("\n");
	printf("URL 		specific url adress of server can by with port number\n");
	printf("	 	implicit port numbers are 80 for http and 443 for https\n");
	printf("\n");
	printf("-f <feedfile>	text file with servers adress in format Atom or RSS \n");
	printf("-c <certfile>	certificates for verification https conections\n");
	printf("-C <certaddr>	folderr of certificates for verification https conections\n");
	printf("\n");
	printf("-T 	display for every record time of change if contained\n");
	printf("-a 	display for every record author information if contained\n");
	printf("-u 	display for every record asociated URL if contained\n");
}

void init_flag(flags_t* flag)
{
	flag->T_flag = false;
	flag->a_flag = false;
	flag->u_flag = false;
}

int main(int argc, char** argv){

	flags_t flags;

	init_flag(&flags);
	bool f_flag = false,c_flag = false,C_flag = false;
	bool URL_flag = false;
	
	char* feedfile = NULL;
	char* certfile = NULL;
	char* certadd = NULL;
	char* URL = NULL;

	bool debug = false;
	bool first = true;

	int arg;
	if (argc == 1)
	{
		print_help();
		return 0;
	}

	for(int i=1; i < argc; i++)
	{
		if(argv[i][0] == '-')
		{
			if(!strcmp(argv[i],"-f"))
			{
				f_flag = true;
				if(i+1 < argc)
				{
					if(argv[++i][0] != '-')
						feedfile = argv[i];
					else{
						print_help();
						return 0;}
				}
				else{
					print_help();
					return 0;
				}
			}
			else if(!strcmp(argv[i],"-c"))
			{
				c_flag = true;
				if(i+1 < argc)
				{
					if(argv[++i][0] != '-')
						certfile = argv[i];
					else{
						print_help();
						return 0;}
				}
				else{
					print_help();
					return 0;
				}
			}
			else if(!strcmp(argv[i],"-C"))
			{
				C_flag = true;
				if(i+1 < argc)
				{
					if(argv[++i][0] != '-')
						certadd = argv[i];
					else{
						print_help();
						return 0;}
				}
				else{
					print_help();
					return 0;
				}
			}
			else if(!strcmp(argv[i],"-T"))
			{
				flags.T_flag = true;
			}
			else if(!strcmp(argv[i],"-a"))
			{
				flags.a_flag = true;
			}
			else if(!strcmp(argv[i],"-u"))
			{
				flags.u_flag = true;
			}
			else{
				print_help();
				return 0;
			}
		}
		else if(!URL_flag)
		{
			URL_flag = true;
			URL = argv[i];
		}
		else{
			print_help();
			return 0;
		}
	}

   if(((URL == NULL) && (feedfile == NULL)) ||((URL != NULL) && (feedfile != NULL))){
		fprintf(stderr, "not specificed input file\n");
		print_help();
		return 0;
	}

	char* host = NULL;
	FILE* feed_file = NULL;
	
	if(f_flag)
		{
			feed_file = fopen(feedfile,"r");
			if(feed_file == NULL)
			{
				fprintf(stderr, "Error - can not open file: %s \n",feedfile);
				return 0;
			}
		}	

	get_adrress_t* set_packet;
	char* xml_string = NULL;
	int indicate_eof = 0;

	while((indicate_eof = get_next_host(URL,f_flag,feed_file,&host)) != EOF)
	{
		char* http_get = NULL;
		if((set_packet = examine_host(host)) != NULL)
		{

		http_get = fill_packet(set_packet);
		if(http_get == NULL)
		{
			fprintf(stderr, "ERR\n");
			return -1;
		}		
		 if(debug)
		 {	
		 	//printf("###############################\n");
		 	printf("%s\n", http_get);
			//printf("###############################\n");
		 }
		if((send_packet(http_get,set_packet,c_flag,C_flag,certadd,certfile,&xml_string)) == 0)
		{
			(void)parse(xml_string,flags,first);
			first = false;
		}
		
		// printf("\n");
		if(xml_string != NULL)
			free(xml_string);
		// CLEAN 
		if(set_packet->host != NULL)
			free(set_packet->host);
		if(set_packet->path != NULL)
			free(set_packet->path);
		if(set_packet != NULL)
			free(set_packet);
		}
		if(http_get != NULL)
			free(http_get);
		if(host != NULL)
			free(host);

		if(!f_flag)
			break;

	}

	if(feed_file != NULL)
		fclose(feed_file);
	return 0;
}