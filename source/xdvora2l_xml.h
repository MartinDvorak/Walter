///////////////////////////////////////////////////////////////////////////////
//				Dvořák Martin 
//				xdvora2l 3BIT
//				ISA project Atom - 2018
///////////////////////////////////////////////////////////////////////////////


#include <libxml/parser.h>
#include <libxml/tree.h>


#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>

#ifndef FLAG_H_
#define FLAG_H_
typedef struct {
	bool T_flag;
	bool a_flag;
	bool u_flag;
}flags_t;
#endif

typedef struct{
	xmlChar* title;
	xmlChar* link;
	xmlChar* author;
	xmlChar* date;
	xmlChar* email;
	xmlChar* sy_period;
	bool sy;
}item_t;

typedef struct{
	xmlChar* name;
	xmlChar* email;
}author_t;

typedef struct {
	bool rss1;
	bool rss2;
	bool atom;
	bool all;
}type_xml_t;


item_t init_item_t();

xmlNode* get_web_info(xmlDocPtr doc,xmlNode* xmlchannel,xmlChar** channel_title);

xmlNode* find_chanell(xmlNode* xmlroot);

void find_type(xmlNode* xmlroot, type_xml_t* xml_type);

int scan_xml(xmlNode* item);

void get_author(xmlNode* item_info,xmlDocPtr doc, author_t* author);

void author_free(author_t* author);

void author_init(author_t* author);

item_t parse_item(xmlDocPtr doc,xmlNode* item);

item_t parse_atom_item(xmlDocPtr doc,xmlNode* item);

item_t parse_rss1_item(xmlDocPtr doc,xmlNode* item);

item_t parse_rss2_item(xmlDocPtr doc,xmlNode* item);

void cat(item_t tmp,flags_t flags, bool last);

void data_free(item_t tmp);

int parse(char* input,flags_t flags, bool first);