/*
 * File name: myping.c
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
#include <time.h>

#define SERVER_PORT 9700

/*
 *   Function: main()
 *   ----------------------------
 *   Purpose: Establish connection with the server
 *   in order to send message and receive validation (ping)
 *
 */

int main( int argc, char *argv[] ) {
    struct sockaddr_in client_address; //socket address for client
    struct hostent *hostConvert;       //convert hostname to IP
    struct in_addr inaddr;             //internet address
    char* ip;
    char* hostname;
    char* message = "Pinging server message.";
    char data_buffer[100];            //buffer of size 100
    clock_t start, end;
    int create_socket, ping, address_size, error_check, buffer_size = 100;
    double total_time;

    //socket creation for use by the client (IPV4, UDP, sysDefault)
    create_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(create_socket < 0){ //error checking
        perror("Failed to create socket, please try again!");
        exit(1);
    }
    
    hostname = argv[1]; //extracting the name of the host we are connecting to (first argument)
    hostConvert = gethostbyname(hostname); //converting name to host address
    if(hostConvert == NULL){ //error checking that host is valid (ex. syslab001... would error)
        printf("Not a valid hostname. Please try again.\n");
        exit(1);
    }
    ip = inet_ntoa(*(struct in_addr *) *hostConvert->h_addr_list); //converts host address to a string in dot notation

    bzero(&client_address, sizeof(client_address)); //socket structures set to null

    //client ipv4, server port, and IP address
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(SERVER_PORT);
    client_address.sin_addr.s_addr = inet_addr(ip);

    address_size = sizeof(client_address); //establishing the size of the address

    //ping host server 5 times and report on time between each ping
    ping = 0;
    while(ping < 5){
        total_time = 0.0;
        sleep(1); //pause for one second before next ping
        error_check = sendto(create_socket, (char *)message, strlen(message), 0, (struct sockaddr *)&client_address, address_size);
        if (error_check < 0){ //error checking
            perror("ERROR in sendto function");
            exit(1);
        }
        start = clock(); //message sent time

        //receiving message from socket
        error_check = recvfrom(create_socket, data_buffer, 50, 0, (struct sockaddr *)&client_address, &address_size);
        if (error_check < 0){ //error checking
            perror("ERROR in recvfrom function");
            exit(1);
        }
        else if(error_check != -1){
            ping++; //UDP is not reliable, need to make sure they were 5 successful pings (packets of data successfully sent)
        }
        end = clock(); //message returned time
        total_time += (double)(end - start) / (CLOCKS_PER_SEC/1000); //total time from send to receive in milliseconds (RTT)
        printf("from %s (%s): time = %3f ms\n", hostname, ip, total_time);
    }
    close(create_socket);
    return 0;
}
