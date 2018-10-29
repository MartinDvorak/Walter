CFLAG= -std=c99

feedreader: main.o xdvora2l_bio.o xdvora2l_xml.o
	gcc -o feedreader main.o xdvora2l_bio.o xdvora2l_xml.o `xml2-config --cflags` `xml2-config --libs` -lcrypto -lssl

main.o: feedreader.c xdvora2l_bio.h xdvora2l_xml.h
	gcc -c feedreader.c -o main.o `xml2-config --cflags` `xml2-config --libs`
        
xdvora2l_bio.o: xdvora2l_bio.c xdvora2l_bio.h
	gcc -c xdvora2l_bio.c -lcrypto -lssl

xdvora2l_xml.o: xdvora2l_xml.c xdvora2l_xml.h
	gcc -c xdvora2l_xml.c `xml2-config --cflags` `xml2-config --libs`

clean:
	rm *.o
	rm feedreader

tests: 
	echo HELL
