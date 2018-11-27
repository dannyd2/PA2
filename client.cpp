#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include "packet.cpp"

using namespace std;

int main(int argc, char *argv[]) {

  char payload[512];
  
  int send_to_portno = atoi(argv[2]);
  int recv_from_portno = atoi(argv[3]);
  
  struct sockaddr_in server;
  socklen_t slen = sizeof(server);
  
  //Declaring Socket
  int send_to_emulator_socket = 0;
  int receive_from_emulator_socket = 0;
  send_to_emulator_socket = socket(AF_INET, SOCK_DGRAM, 0);
  receive_from_emulator_socket = socket(AF_INET, SOCK_DGRAM, 0);

  //Get Host IP Address
  struct hostent *s;
  s = gethostbyname(argv[1]);

  //Set Destination Info
  bzero((char *) &server, sizeof(server));
  //memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(send_to_portno);
  bcopy((char *) s->h_addr, (char *) &server.sin_addr.s_addr, s->h_length);
  //connect to server
  connect(send_to_emulator_socket, (struct sockaddr *)&server, slen);
  cout << "\nSend to Emulator Port: " << server.sin_port << endl << endl;
  
  
  
  server.sin_port = htons(recv_from_portno);
  connect(receive_from_emulator_socket, (struct sockaddr *)&server, slen);
  cout << "\nReceive from Emulator Port: " << server.sin_port << endl << endl;
  
  

  
  


  packet pckt(1,1,30,NULL);

  pckt.printContents();

  



  
  //Send Data to Server
  //CITATION: http://www.cplusplus.com/reference/fstream/ifstream/
  string line;
  ifstream file ("file.txt");
  if(file.is_open())
    {
      while (!file.eof())
	{
	  string output;
	  char next_line[5] = "\n";
	  int index = 0;
	  getline(file, line);
	  for(int count = 0; count < 18; count++)
	    {
	      for (int countt = 0; countt < 20; countt++)
		{
		  output = toupper(line[index]);
		  cout << output;
		  payload[index] = *output.c_str();
		  index = index + 1;
		}
	      cout << endl;
	    }
	  sendto(send_to_emulator_socket, payload, 512, 0, (struct sockaddr *)&server, slen);
	  sendto(send_to_emulator_socket, next_line, 512, 0, (struct sockaddr *)&server, slen);
	}
    }
	
  file.close();
  
  close(send_to_emulator_socket);
  close(receive_from_emulator_socket);
  return 0;
}
 
