#include <iostream>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <ctime>

//CITATION: Maxwell Young's Lecture 3 Notes
using namespace std;

int main(int argc, char *argv[]) {
    
        char payload[512]; 
        char next_line[5] = "\n";
        struct sockaddr_in client; //store client IP and port number
        int receive_from_portno = atoi(argv[3]);
		int send_to_portno = atoi(argv[2]);
        //CITATION (structs): http://beej.us/guide/bgnet/html/multi/ipstructsdata.html#structs	
	socklen_t clen = sizeof(client);

        
    /*  
	//Random Number
	srand((unsigned)time(0));
	int random = rand() % 64512 + 1024;
	*/
	
    
	int receive_from_emulator_socket = 0; 
	int send_to_emulator_socket = 0;
	receive_from_emulator_socket = socket(AF_INET, SOCK_DGRAM, 0);
	send_to_emulator_socket = socket(AF_INET, SOCK_DGRAM, 0);

	
	
	struct sockaddr_in server;
	memset((char *) &server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(receive_from_portno);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//Bind socket to an address/port #
	bind(receive_from_emulator_socket, (struct sockaddr *) &server, sizeof(server));
	bind(send_to_emulator_socket, (struct sockaddr *) &server, sizeof(server));

    //listen for client
    listen(receive_from_emulator_socket, 5);
	cout << "\nNegotiation has been detected. receive_from_emulator port: " << server.sin_port << endl;

	
	server.sin_port = htons(send_to_portno);
	listen(send_to_emulator_socket, 5); 
    cout << "\nNegotiation has been detected. send_to_emulator port: " << server.sin_port << endl;
        
    //accept connection from client
    int new_receive_from_emulator_socket = 0;
    new_receive_from_emulator_socket = accept(receive_from_emulator_socket, (struct sockaddr *)&client, &clen);
	
	int new_send_to_emulator_socket = 0;
    new_send_to_emulator_socket = accept(send_to_emulator_socket, (struct sockaddr *)&client, &clen);

    
    //CITATION (ofstream): http://www.cplusplus.com/doc/oldtutorial/files/
    ofstream outfile ("output.txt");
    int i = 0;
    while(i != 3){ //receive each line of file.txt and send to output.txt, line by line.
		recvfrom(receive_from_emulator_socket, payload, 512, 0, (struct sockaddr *)&client, &clen); //listen using mysocket and store received data in 512 byte payload
        recvfrom(receive_from_emulator_socket, next_line, 512, 0, (struct sockaddr *)&client, &clen);
        outfile << payload;
        outfile << next_line;
        i++;
    }
        
    outfile.close();
    i = 0;
	
	close(receive_from_emulator_socket);
	close(send_to_emulator_socket);
    
	return 0;
}