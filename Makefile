CFLAG= -std=c99

feedreader: main.o xdvora2l_bio.o xdvora2l_xml.o
	gcc -o feedreader bin/main.o bin/xdvora2l_bio.o bin/xdvora2l_xml.o `xml2-config --cflags` `xml2-config --libs` -lcrypto -lssl

main.o: source/feedreader.c source/xdvora2l_bio.h source/xdvora2l_xml.h
	gcc -c source/feedreader.c -o bin/main.o `xml2-config --cflags` `xml2-config --libs`
        
xdvora2l_bio.o: source/xdvora2l_bio.c source/xdvora2l_bio.h
	gcc -c source/xdvora2l_bio.c -o bin/xdvora2l_bio.o -lcrypto -lssl

xdvora2l_xml.o: source/xdvora2l_xml.c source/xdvora2l_xml.h
	gcc -c source/xdvora2l_xml.c -o bin/xdvora2l_xml.o `xml2-config --cflags` `xml2-config --libs`

clean:
	rm bin -r
	mkdir bin
	rm feedreader

test: 
	echo "run test scritp"
	chmod 777 test.sh
	./test.sh
