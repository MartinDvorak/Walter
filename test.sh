#!/bin/sh
# script by Dvořák Martin to run tests on ISA project aplication.
# FIT VUT 2018 project Atom reader (TSL and RSS & ATOM)

INSTANCE=$((1))
echo "
INSTANCE $INSTANCE
Note: test wrong paremetres in comand line - more sources"
echo "./feedreader http://www.fit.vutbr.cz/news/news-rss.php -f ./feeds/feedfile
"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php -f ./feeds/feedfile


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test wrong paremetres in comand line - no source"
echo "./feedreader -c ./feeds/cacert.crt -C certs 
"
./feedreader -c ./feeds/cacert.crt -C certs


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test wrong paremetres in comand line - unknown x parameter"
echo "./feedreader http://www.fit.vutbr.cz/news/news-rss.php -a -U -x
"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php -a -u -x


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test basic http comunication and RSS xml version"
echo "./feedreader http://www.fit.vutbr.cz/news/news-rss.php
"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test https comunication with dedicated certificates
	aplication must end corectly because response is not in xml format"
echo "./feedreader https://wis.fit.vutbr.cz/FIT/ -c ./feeds/cacert.crt
"
./feedreader https://wis.fit.vutbr.cz/FIT/ -c ./feeds/cacert.crt


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test https comunication with dedicated certificates in folder
	aplication must end corectly because response is not in xml format"
echo "./feedreader https://wis.fit.vutbr.cz/FIT/ -C ./feeds
"
./feedreader https://wis.fit.vutbr.cz/FIT/ -C ./feeds


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test https comunication without dedicated certificates
	aplication must end corectly because connection is not safe and secure"
echo "./feedreader https://wis.fit.vutbr.cz/FIT/
"
./feedreader https://wis.fit.vutbr.cz/FIT/


INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test https comunication without certificates
	aplication must end corectly because connection is not safe and secure"
echo "./feedreader https://tools.ietf.org/agenda/atom -C /dev/null
"
./feedreader https://tools.ietf.org/agenda/atom -C /dev/null

INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test http and https comunication with default certificates
	aplication must find atom information from file of feeds"
echo "./feedreader -f ./feeds/feed_atoms
"
./feedreader -f ./feeds/feed_atoms

INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test http and https comunication with default certificates
	aplication must find atom information from file of feeds with more information
	like author -a link -u and last update -T"
echo "./feedreader -f ./feeds/feed_atoms -a -u -T
"
./feedreader -f ./feeds/feed_atoms -a -u -T

INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test on not existing URL 
	aplication must end corectly and informate clint about unsuccess"
echo "./feedreader https://www.this.sure.not.exists.php
"
./feedreader https://www.this.sure.not.exists.php

INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test bad protocol - not https or http
	aplication must end corectly and informate clint about unsuccess"
echo "./feedreader httpsa://www.fit.vutbr.cz/news/news-rss.php
"
./feedreader httpsa://www.fit.vutbr.cz/news/news-rss.php

INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test corect instance with specific https port explicit set
	aplication must successly connet with server"
echo "./feedreader https://xkcd.com:443/atom.xml
"
./feedreader https://xkcd.com:443/atom.xml

INSTANCE=$((1 + INSTANCE))
echo "
INSTANCE $INSTANCE
Note: test open not existing feed file
	aplication must eend successfuly"
echo "./feedreader -f notexist_feed_file
"
./feedreader -f notexist_feed_file