///////////////////////////////////////////////////////////////////////////////
//				Dvořák Martin 
//				xdvora2l 3BIT
//				ISA project Atom - 2018
///////////////////////////////////////////////////////////////////////////////


#include "xdvora2l_xml.h"


xmlNode* find_chanell(xmlNode* xmlroot)
{
	// look for root item
	if((xmlroot->type == XML_ELEMENT_NODE) && (!strcmp(xmlroot->name,"feed")))
	{	
		return xmlroot;
	}
	xmlNode* node = NULL;
	for(node = xmlroot->children;node;node = node->next)
	{
		if((node->type == XML_ELEMENT_NODE) && (!strcmp(node->name,"channel")))
		{	
			break;
		}
	}
	return node;
}

xmlNode* get_web_info(xmlDocPtr doc,xmlNode* xmlchannel,xmlChar** channel_title)
{	// looking for web info about feed
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
	tmp.sy_period = NULL;
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
	{ // i nthis case has subitems like name or email
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
	else{ // have only atrribute 
		if(!strcmp(item_info->name,"author"))
		author->name = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);			
		else if(!strcmp(item_info->name,"dc:creator"))
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
item_t parse_atom_item(xmlDocPtr doc,xmlNode* item)
{	// in atom is complicated structure person
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
			{ // URL is hide in atrib name href
				data.link = xmlGetProp(item_info, "href");
			}
			else if(!strcmp(item_info->name,"author"))
			{ // author can obtain email address
				author_t author;
				author_init(&author);
				get_author(item_info,doc,&author);
			 	data.author = author.name;
			 	data.email = author.email;
			}
			else if(!strcmp(item_info->name,"updated"))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}
		}
	}
	return data;

}
item_t parse_rss1_item(xmlDocPtr doc,xmlNode* item)
{ // parse article in rss1 format
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
			}// dublin core module
			else if(!strcmp(item_info->name,"dc:creator"))
			{
			 	data.author = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}// dublin core
			else if(!strcmp(item_info->name,"dc:date"))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
				data.sy = false;
			}
			else if(!strcmp(item_info->name,"sy:updateBase"))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);	
				data.sy = true;
			} // syndication module
			else if((!strcmp(item_info->name,"sy:updatePeriod")))
			{
				data.sy_period = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}// dublin core
			else if(((!strcmp(item_info->name,"dc:title"))) && (data.title == NULL))
			{
				data.title = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}

		}
	}
	return data;
}
item_t parse_rss2_item(xmlDocPtr doc,xmlNode* item)
{ // run rss2 parse function
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
			}
			else if(!strcmp(item_info->name,"author"))
			{
			 	data.author = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}
			else if(!strcmp(item_info->name,"pubDate"))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}
		}
	}
	return data;
}

item_t parse_item(xmlDocPtr doc,xmlNode* item)
{ // can not reckognise version of xml then run universal parse function
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
			else if((!strcmp(item_info->name,"author"))||(!strcmp(item_info->name,"dc:creator")))
			{ // ofr atom and rss 1 and 2 
				author_t author;
				author_init(&author);
				get_author(item_info,doc,&author);
			 	data.author = author.name;
			 	data.email = author.email;
			} // dublin core module or atom
			else if((!strcmp(item_info->name,"updated")) || (!strcmp(item_info->name,"pubDate"))
				 || (!strcmp(item_info->name,"dc:date")))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
				data.sy = false;
			} // syndication module
			else if(!strcmp(item_info->name,"sy:updateBase"))
			{
				data.date = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);	
				data.sy = true;
			} // syndication module
			else if((!strcmp(item_info->name,"sy:updatePeriod")))
			{
				data.sy_period = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			} // dublin core module
			else if(((!strcmp(item_info->name,"dc:title"))) && (data.title == NULL))
			{
				data.title = xmlNodeListGetString(doc, item_info->xmlChildrenNode, 1);
			}

		}
	}
	return data;
}
// chcecking for set user parametr for information - all information are colected
void cat(item_t tmp,flags_t flags, bool last)
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

	if(((tmp.date != NULL)&&(flags.T_flag)) && ((!tmp.sy) || (tmp.sy_period == NULL)))
		printf("Aktualizace: %s\n", tmp.date);
	else if(((tmp.date != NULL)&&(flags.T_flag)) && (tmp.sy) && (tmp.sy_period != NULL))
		printf("Periodická aktualizace od: %s s krokem: %s\n", tmp.date,tmp.sy_period);


	if(((flags.u_flag)||(flags.a_flag)||(flags.T_flag))&&(!last))
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
	if(tmp.sy_period != NULL)
		free(tmp.sy_period);

}

void xml_type_init(type_xml_t* type)
{ 
	type->rss1 = false;
	type->rss2 = false;
	type->atom = false;
	type->all = false;
}

void find_type(xmlNode* xmlroot, type_xml_t* xml_type)
{
	if((xmlroot->type == XML_ELEMENT_NODE) && (!strcmp(xmlroot->name,"feed")))
	{
		xml_type->atom = true;
	}
	else if((xmlroot->type == XML_ELEMENT_NODE) && (!strcmp(xmlroot->name,"rss")))
	{
		xml_type->rss2 = true;
	}
	else if((xmlroot->type == XML_ELEMENT_NODE) && (!strcmp(xmlroot->name,"RDF")))
	{
		xml_type->rss1 = true;
	}
	else{
		xml_type->all = true;
	}
}

int parse(char* input,flags_t flags,bool first)
{
	type_xml_t xml_type;
	xml_type_init(&xml_type);
	
	xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

	//doc = xmlReadFile(input, NULL, 0);
    
	if(input != NULL)
    {// set atriv for parse xml
		doc = xmlReadMemory(input,strlen(input),NULL,NULL,XML_PARSE_NSCLEAN | XML_PARSE_NOERROR |XML_PARSE_NOWARNING |XML_PARSE_NOBLANKS);
    	//doc = xmlReadFile("rss2.xml",NULL,XML_PARSE_NSCLEAN | XML_PARSE_NOERROR |XML_PARSE_NOWARNING |XML_PARSE_NOBLANKS);
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

    find_type(root_element,&xml_type);
    
    xmlChar* channel_title = NULL;
    xmlNode* item = NULL;
    item = get_web_info(doc,channel,&channel_title);
    if(!first)
    {
    	printf("\n");
    }
    if(channel_title != NULL)
    { // chanell title
    	printf("*** %s ***\n", channel_title);
   		free(channel_title);
    }
    else{ // chanell has not any title 
    	printf("*** feed name does not set ***\n");
    }
    
    if(!strcmp(root_element->name,"RDF"))
    {
    	item = channel;
    	xml_type.rss1 = true;
    }
    
    item_t data;
    bool last = false;
    for( ;item; item = item->next)
    {// body of feed, articles
    	if(item->type == XML_ELEMENT_NODE)
    	{
    		if((!strcmp(item->name,"item"))||(!strcmp(item->name,"entry")))
    		{
    			if(xml_type.atom)
    				data = parse_atom_item(doc,item);
    			else if(xml_type.rss1)
    				data = parse_rss1_item(doc,item);
    			else if(xml_type.rss2)
    				data = parse_rss2_item(doc,item);
    			else{
    				data = parse_item(doc,item);
    			}
    			if(!(item->next))
    				last = true;
    			cat(data,flags,last);
    			data_free(data);
    		}
    	}
    }
    xmlFreeNode(item);
   
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}