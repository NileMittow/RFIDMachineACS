updateDB: updateDB.cpp membership_db.h member.o network_tools.o
	g++ -Wall -pedantic -g -o updateDB updateDB.cpp membership_db.h member.o -network_tools.o -l sqlite3 -l json-c -l curl -I /opt/libs/

member.o: member.cpp member.h
	g++ -Wall -pedantic member.cpp -c

network_tools.o: /opt/libs/network_tools.cpp /opt/libs/network_tools.h
	g++ -Wall -pedantic network_tools.cpp -c -I /opt/libs/

clean:
	rm -f *.o test list updateDB
