/*
 * File name: myping_server.c
 * Project: UDP Ping Assignment 2
 * CPSC 333
 * Author: @Rakan AlZagha
 * Comments: workstations are still down so was only able to test
 * with lab3.cs.trincoll.edu (127.0.1.1) via SSH to lab3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define SERVER_PORT 9700

/*
 *   Function: main()
 *   ----------------------------
 *   Purpose: Establish connection with the client
 *   in order to receive message and send back validation (pong)
 *
 */

int main() {
    struct sockaddr_in client_address; //socket address for client
    struct sockaddr_in server_address; //socket address for server
    struct hostent *hostConvert;       //convert hostname to IP
    struct in_addr inaddr;             //internet address
    char* ip;
    char* hostname;
    char* message = "Pinging server message.";
    int create_socket, address_size, error_check, buffer_size = 100;
    char data_buffer[100];

    //socket creation for use by the server (IPV4, UDP, sysDefault)
    create_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(create_socket < 0){ //error checking
        perror("Failed to create socket, please try again!");
        exit(1);
    }

    //socket structures set to null
    bzero(&server_address, sizeof(server_address));
    bzero(&client_address, sizeof(client_address));

    //server ipv4, server port, and IP address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    //assign the address specified by addr to the socket referred to by the file descriptor
    error_check = bind(create_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if(error_check < 0){ //error checking
        perror("ERROR at bind function!");
        exit(1);
    }

    address_size = sizeof(client_address); //establishing the size of the address
    //infinite while loop to keep server available for incoming client requests (terminate by ctrl-z)
    while(1){
        bzero(data_buffer, buffer_size); //buffer set to null
        //receiving message from socket
        error_check = recvfrom(create_socket, data_buffer, buffer_size, 0, (struct sockaddr *)&client_address, &address_size);
        if (error_check < 0){ //error checking
            perror("ERROR at recvfrom function!");
            exit(1);
        }
        //returning message from socket
        error_check = sendto(create_socket, (const char *)message, strlen(message), 0, (struct sockaddr *)&client_address, address_size);
        if (error_check < 0){ //error checking
            perror("ERROR at sendto function!");
            exit(1);
        }
    }
}
