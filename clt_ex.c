#include<netinet/in.h>
#include<netdb.h> 
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

int my_connect_to_server(char *servername, char *port);

int main(int argc, char* const argv[]){
     
     if(argc!=4) {
        printf("Usage: %s server_ip_address port_number file_name\n",argv[0]);
        exit(1); 
    }
    
    char buffer[4096];  
    int n;
    
    // connect to server
    int socket_descriptor = my_connect_to_server(argv[1], argv[2]);
    printf("Connected!\n");

    //open file

    int fds = open(argv[3], O_RDONLY);
    if(fds == -1){
        perror("open source");
        return 1;
    }
    printf("File is being sent");

    while(1){

        //read at most sizeof(buffer) bytes from file
        n= read(fds, buffer, sizeof(buffer));
        if(n==0)
            break;
        printf("%s \n\n", buffer); //terminar string
        write(socket_descriptor, buffer, n); //send buffer to server
    }
    close(fds);
    close(socket_descriptor);
    printf("You've copied you're file successfully");

    return 0;
}

int my_connect_to_server(char *servername, char *port){
   //get server address using getaddrinfo
   struct addrinfo hints;
   struct addrinfo *addrs;
   memset(&hints, 0, sizeof(struct addrinfo));

   hints.ai_family = AF_INET;  //socket domain
   hints.ai_socktype = SOCK_STREAM;  //socket type
   hints.ai_flags = 0;
   hints.ai_protocol = 0;   /* Any protocol */

   int r = getaddrinfo(servername, port, &hints, &addrs);
   if(r!=0){
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
       exit(1);
   }

   //create socket
   int s = socket(AF_INET, SOCK_STREAM, 0);
   if(s == -1){
       perror("socket");
       exit(2);
   }

   //connect to server
   if(connect(s, addrs->ai_addr, addrs->ai_addrlen) == -1){
       perror("connect");
       close(s);
       exit(3);
   }

  return s;

}