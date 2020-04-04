#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//socket related headers
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UDS_FILE "/tmp/thisisanewportfor.uds"

int createSocket(){
    int create_socket; 
    create_socket = socket(PF_LOCAL, SOCK_STREAM, 0); 
    if(create_socket > 0){
        std::cout<<"Socket creation was successful\n";
    }
    return create_socket; 
}

int connectSocket(int socket, struct sockaddr_un){
    int isConnectionSuccessful; 
    sockaddr_un addr;

    isConnectionSuccessful = connect(socket, (struct sockaddr *) &addr, sizeof (addr));
    if(isConnectionSuccessful == 0){
        std::cout<<"Connection to the server is successful\n";
    }
}

int main (int argc, char ** argv) {
    int size;
    char *buffer = (char *)malloc(1024);
    struct sockaddr_un addr;
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, UDS_FILE);

    int socket = createSocket();
    
    connectSocket(socket, addr); 
  
    do {
        size = recv(socket, buffer, 1023, 0); 

        if (size > 0){
            buffer[size] = '\0';
        }
            
        std::cout<<"Received message:\n"<<buffer;       

        if (strcmp (buffer, "quit \n")) {
            std::cout<<"Message to send:"; 

            fgets(buffer, 1024, stdin);
            send(socket, buffer, strlen (buffer), 0);
       }
    } while (strcmp(buffer, "quit \n") != 0);

    close (socket);
    
    return EXIT_SUCCESS;
}