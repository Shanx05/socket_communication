
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//socket related headers
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define UDS_FILE "/tmp/thisisanewportfor.uds"
using namespace std; 

int createSocket(){
    if((socket(AF_LOCAL, SOCK_STREAM, 0)) > 0){
        cout<<"Socket creation successful\n"; 
    }
}

int socketBind(int socket, struct sockaddr_un){
    sockaddr_un address;
    unlink(UDS_FILE);
    if(bind(socket, (struct sockaddr *) &address, sizeof (address)) != 0){
        cout<<"The port is already occupied!\n"; 
    }
}

int acceptConnnection(int socket, struct sockaddr_un, socklen_t addrlen){
    sockaddr_un address;
    int newSocketConnection = accept(socket, (struct sockaddr *) &address, &addrlen);
    if(newSocketConnection > 0){
        cout<<"A client is connected\n"; 
    }
}

int main (void) {
    int socket, new_socket;
    socklen_t addrlen;
    char * buffer = (char *)malloc(1024);
    ssize_t size;
    struct sockaddr_un address;
    const int y = 1;

    address.sun_family = AF_LOCAL;
    strcpy (address.sun_path, UDS_FILE);

    createSocket(); 

    unlink(UDS_FILE);

    socketBind(socket, address);

    listen(socket, 5);

    addrlen = sizeof(struct sockaddr_in);

    while (1) {
        new_socket = acceptConnnection(socket, address, addrlen); 
        do {
            std::cout<<"Message to send: ";
            fgets(buffer, 1024, stdin);
            send(new_socket, buffer, strlen(buffer), 0);
            size = recv(new_socket, buffer, 1023, 0);
            if (size> 0)
                buffer[size] = '\0';
            std::cout<<"Message received: "<<buffer;
        } while(strcmp(buffer, "quit \n") != 0);
        close(new_socket);
  }
  close(socket);
  return EXIT_SUCCESS;
}