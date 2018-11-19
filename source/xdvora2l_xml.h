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

/**
 * @brief set default values to structure item_t like NULL to pointer.
 * @return on stack saved structure item_t
 */
item_t init_item_t();

/**
 * @brief extrahing meta information from xml response
 * @details extrahing "title" in feed from meta information. 
 * Then skip rest of meta infromation and return pointer to first article
 * 
 * @param doc pointer to root object of xml
 * @param xmlchannel actual pointer to meta data object
 * @param channel_title title in whole feed is returned in this value
 * @return pointer to first "article" object
 */
xmlNode* get_web_info(xmlDocPtr doc,xmlNode* xmlchannel,xmlChar** channel_title);

/**
 * @brief looking for chanell or feed of xml
 * 
 * @param xmlroot pointer to root object of xml
 * @return NULL or pointer to channel object
 */
xmlNode* find_chanell(xmlNode* xmlroot);

/**
 * @brief find xml version
 * @details find xml version of response. supported are Atom, RSS1 and RSS2.
 * Fill with this information structure type_xml_t 
 * 
 * @param xmlroot pointer to root object in xml
 * @param xml_type structure which will be set
 */
void find_type(xmlNode* xmlroot, type_xml_t* xml_type);

/**
 * @brief helping function to get author information
 * @details helping function to find that actual Node has any child node and return count of child
 * 
 * @param actual xml object which is investigated 
 * @return number of child nodes
 */
int scan_xml(xmlNode* item);

/**
 * @brief get information about author of article
 * @details function are geting information from xml note about author.
 * Atom has complicated structure of child nodes. Can obtains authors email as well.
 * 
 * @param item_info pointer to actual author xml node
 * @param doc xml root object pointer
 * @param author return structure where are set information about author
 */
void get_author(xmlNode* item_info,xmlDocPtr doc, author_t* author);

/**
 * @brief releas all sources in stucture author_t
 * 
 * @param author pointer to stucture, this structure be free
 */
void author_free(author_t* author);

/**
 * @brief init function
 * @details init structure author_t to default values NULL 
 * 
 * @param author pointer to structure which is going to be inicialized
 */
void author_init(author_t* author);

/**
 * @brief parsing article with all specification
 * @details parsing with supported Atom RSS1 RSS2 and updated RSS1 (dublin core and syndacated)
 * 
 * @param doc root pointer to xml tree
 * @param item actual xml object referenting to article
 * 
 * @return colection all data with specific protocol (RSS1, RSS2, Atom)
 */
item_t parse_item(xmlDocPtr doc,xmlNode* item);

/**
 * @brief parsing article with all specification
 * @details parsing with supported Atom. Geting information rom title, 
 * link atribut href, structure author and update node 
 * 
 * @param doc root pointer to xml tree
 * @param item actual xml object referenting to article
 * 
 * @return colection all data with specific protocol (RSS1, RSS2, Atom)
 */
item_t parse_atom_item(xmlDocPtr doc,xmlNode* item);

/**
 * @brief parsing article with all specification
 * @details parsing with supported RSS1. Geting information rom title, 
 * link, dublin core extension - creator to author and date to lastUpdate.
 * And syndication module for updateBase and updatePeriod 
 * 
 * @param doc root pointer to xml tree
 * @param item actual xml object referenting to article
 * 
 * @return colection all data with specific protocol (RSS1, RSS2, Atom)
 */
item_t parse_rss1_item(xmlDocPtr doc,xmlNode* item);

/**
 * @brief parsing article with all specification
 * @details parsing with supported RSS2. Geting information rom title, 
 * link, author and pubDate
 * 
 * @param doc root pointer to xml tree
 * @param item actual xml object referenting to article
 * 
 * @return colection all data with specific protocol (RSS1, RSS2, Atom)
 */
item_t parse_rss2_item(xmlDocPtr doc,xmlNode* item);

/**
 * @brief fuction to print results
 * @details fuction print result to every arcitle in feed. 
 * existing data are printed in order: Title, URL, Author, Email, Aktualizace
 * 
 * @param tmp stucture of colected data
 * @param flags flags from user, which data are printed
 * @param last flag that indicated if last article is ocured -> dont print newline
 */
void cat(item_t tmp,flags_t flags, bool last);

/**
 * @brief release all data in structure item_t
 * 
 * @param tmp released stucture
 */
void data_free(item_t tmp);

/**
 * @brief main function for parsing xml response
 * @details function for parsing xml response. with libxml2 parse to tree structure xml.
 * Have main loop for geting information from individual article
 * 
 * @param input body of http repsonse
 * @param flags flags defined from user, which information colected
 * @param first to better formation between messeges
 * @return 0 -> OK and -1 if error ocured
 */
int parse(char* input,flags_t flags, bool first);