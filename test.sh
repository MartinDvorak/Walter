#!/bin/bash 
# script by Dvořák Martin to run tests on ISA project aplication.
# FIT VUT 2018 project Atom reader (TSL and RSS & ATOM)

((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test wrong paremetres in comand line - more sources"
echo "./feedreader http://www.fit.vutbr.cz/news/news-rss.php -f feedfile
"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php -f feedfile


((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test wrong paremetres in comand line - no source"
echo "./feedreader -c cacert.crt -C certs 
"
./feedreader -c cacert.crt -C certs


((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test wrong paremetres in comand line - unknown x parameter"
echo "./feedreader http://www.fit.vutbr.cz/news/news-rss.php -a -U -x
"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php -a -u -x


((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test basic http comunication and RSS xml version"
echo "./feedreader http://www.fit.vutbr.cz/news/news-rss.php
"
./feedreader http://www.fit.vutbr.cz/news/news-rss.php


((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test https comunication with dedicated certificates
	aplication must end corectly because response is not in xml format"
echo "./feedreader https://wis.fit.vutbr.cz/FIT/ -c cacert.crt
"
./feedreader https://wis.fit.vutbr.cz/FIT/ -c cacert.crt


((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test https comunication without dedicated certificates
	aplication must end corectly because connection is not safe and secure"
echo "./feedreader https://wis.fit.vutbr.cz/FIT/
"
./feedreader https://wis.fit.vutbr.cz/FIT/


((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test https comunication without certificates
	aplication must end corectly because connection is not safe and secure"
echo "./feedreader https://tools.ietf.org/agenda/atom -C /dev/null
"
./feedreader https://tools.ietf.org/agenda/atom -C /dev/null

((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test http and https comunication with default certificates
	aplication must find atom information from file of feeds"
echo "./feedreader -f feed_atoms
"
./feedreader -f feed_atoms

((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test http and https comunication with default certificates
	aplication must find atom information from file of feeds with more information
	like author -a link -u and last update -T"
echo "./feedreader -f feed_atoms -a -u -T
"
./feedreader -f feed_atoms -a -u -T

((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test on not existing URL 
	aplication must end corectly and informate clint about unsuccess"
echo "./feedreader https://www.this.sure.not.exists.php
"
./feedreader https://www.this.sure.not.exists.php

((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test bad protocol - not https or http
	aplication must end corectly and informate clint about unsuccess"
echo "./feedreader httpsa://www.fit.vutbr.cz/news/news-rss.php
"
./feedreader httpsa://www.fit.vutbr.cz/news/news-rss.php

((num_of_instance++))
echo "
INSTANCE $num_of_instance
Note: test corect instance with specific https port explicit set
	aplication must successly connet with server"
echo "./feedreader https://xkcd.com:443/atom.xml
"
./feedreader https://xkcd.com:443/atom.xml
