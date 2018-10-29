#include "xdvora2l_xml.h"

xmlNode* find_chanell(xmlNode* xmlroot)
{
	
	if((xmlroot->type == XML_ELEMENT_NODE) && (!strcmp(xmlroot->name,"feed")))
			return xmlroot;

	xmlNode* node = NULL;
	for(node = xmlroot->children;node;node = node->next)
	{
		if((node->type == XML_ELEMENT_NODE) && (!strcmp(node->name,"channel")))
			break;
	}
	return node;
}

xmlNode* get_web_info(xmlDocPtr doc,xmlNode* xmlchannel,xmlChar** channel_title)
{
	xmlNode* meta = NULL;

	for(meta = xmlchannel->children; meta; meta = meta->next)
	{
		if(meta->type == XML_ELEMENT_NODE)
		{
			if(!strcmp(meta->name,"title"))
			{
				*channel_title = xmlNodeListGetString(doc, meta->xmlChildrenNode, 1);
			}
			if((!strcmp(meta->name,"item")) ||((!strcmp(meta->name,"entry"))))
			{
				break;
			}		
		}
	}
	return meta;
}

item_t init_item_t(){

	item_t tmp;
	tmp.title = NULL;
	tmp.link = NULL;
	tmp.author = NULL;
	tmp.date = NULL;
	tmp.email = NULL;
	return tmp;
}

int scan_xml(xmlNode* item)
{
	int count = 0;
	for(item = item->children ; item ;item = item->next)
	{
		if(item->type == XML_ELEMENT_NODE)
			count++;
	}
	return count;
}

void get_author(xmlNode* item_info,xmlDocPtr doc, author_t* author)
{
	xmlNode* tmp = item_info;
	int count = scan_xml(tmp);
	if(count)
	{
		for(item_info = item_info->children; item_info; item_info = item_info->next)
		{
			if(item_info->type == XML_ELEMENT_NODE)
			{
				if(!strcmp(item_info->name,"name"))
				{
					author->name = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);				
				}
				if(!strcmp(item_info->name,"email"))
				{
					author->email = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);			
				}
			}
		}

	}
	else{
		if(!strcmp(item_info->name,"author"))
		author->name = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);			
		else if(!strcmp(item_info->name,"name"))
		author->name = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);			
	
	}
	//return author;
}
void author_free(author_t* author)
{
	if(author->name != NULL)
		free(author->name);
	if(author->email != NULL)		
		free(author->email);
	
}
void author_init(author_t* author)
{
	author->name = NULL;
	author->email = NULL;
}

item_t parse_item(xmlDocPtr doc,xmlNode* item)
{
	item_t data = init_item_t();
	xmlNode* item_info;
	for(item_info = item->children; item_info; item_info = item_info->next)
	{
		if(item_info->type == XML_ELEMENT_NODE)
		{
			if(!strcmp(item_info->name,"title"))
			{
				data.title = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}
			else if(!strcmp(item_info->name,"link"))
			{
				data.link = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
				if(data.link == NULL)
					data.link = xmlGetProp(item_info, "href");
			}
			else if((!strcmp(item_info->name,"author"))||(!strcmp(item_info->name,"name")))
			{
				author_t author;
				author_init(&author);
				get_author(item_info,doc,&author);
			 	data.author = author.name;
			 	data.email = author.email;
			}
			else if((!strcmp(item_info->name,"updated")) || (!strcmp(item_info->name,"pubDate"))
				 || (!strcmp(item_info->name,"date")))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}

		}
	}
	return data;
}

void cat(item_t tmp,flags_t flags)
{
	if((tmp.title != NULL)&&(true))
		printf("%s\n", tmp.title);
	else{
		printf("title name have not set\n");
	}
	if((tmp.link != NULL)&&(flags.u_flag))
		printf("URL: %s\n", tmp.link);
	
	if((tmp.author != NULL)&&(flags.a_flag))
		printf("Autor: %s\n", tmp.author);
	
	if((tmp.email != NULL)&&(flags.a_flag))		
		printf("Email: %s\n", tmp.email);

	if((tmp.date != NULL)&&(flags.T_flag))
		printf("Aktualizace: %s\n", tmp.date);
	
	if((flags.u_flag)||(flags.a_flag)||(flags.T_flag))
		printf("\n");
}

void data_free(item_t tmp)
{
	if(tmp.title != NULL)
		free(tmp.title);
	if(tmp.link != NULL)
		free(tmp.link);
	if(tmp.author != NULL)
		free(tmp.author);
	if(tmp.date != NULL)
		free(tmp.date);
	if(tmp.email != NULL)
		free(tmp.email);
}

int parse(char* input,flags_t flags,bool first)
{
	xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

	//doc = xmlReadFile(input, NULL, 0);
    
	if(input != NULL)
    {
		//doc = xmlReadMemory(input,strlen(input),NULL,NULL,XML_PARSE_NSCLEAN | XML_PARSE_NOERROR |XML_PARSE_NOWARNING |XML_PARSE_NOBLANKS);
    	doc = xmlReadFile("fit",NULL,XML_PARSE_NSCLEAN | XML_PARSE_NOERROR |XML_PARSE_NOWARNING |XML_PARSE_NOBLANKS);
    }
    else{
    	fprintf(stderr, "in packet are not any data\n");
    	return -1;
    }
    
    if (doc == NULL) {
        fprintf(stderr,"error: could not parse XML \n");
        return -1;
    }

    root_element = xmlDocGetRootElement(doc);

    xmlNode* channel = NULL;
    channel = find_chanell(root_element);
    
    xmlChar* channel_title = NULL;
    xmlNode* item = NULL;
    item = get_web_info(doc,channel,&channel_title);
    if(!first)
    {
    	printf("\n");
    }
    if(channel_title != NULL)
    {
    	printf("*** %s ***\n", channel_title);
    }
    else{
    	printf("*** feed name does not set ***\n");
    }
    free(channel_title);
    
    if(!strcmp(root_element->name,"RDF"))
    {
    	item = channel;
    }
    
    item_t data;
    for( ;item; item = item->next)
    {
    	if(item->type == XML_ELEMENT_NODE)
    	{
    		if((!strcmp(item->name,"item"))||(!strcmp(item->name,"entry")))
    		{
    			data = parse_item(doc,item);
    			cat(data,flags);
    			data_free(data);
    		}
    	}
    }
    xmlFreeNode(item);
   
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}