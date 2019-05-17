#include <netinet/in.h>
#include <unistd.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <arpa/inet.h> //inet_ntoa
#include <sys/stat.h>
#include <sys/types.h>

int create_server_socket(int port);

int main(int argc, char* argv[]){
    int new_socket_descriptor, socket_descriptor;
    int n;
    unsigned int clilen;
    struct sockaddr_in cli_addr;
    char buffer[4096];
    char filename[1024];

    if(argc!=2){
        printf("Usage: %s port_number\n", argv[0]);
        exit(1);
    }

  //SIGPIPE is sent to the process if a write is made to a closed connection.
  //By default, SIGPIPE terminates the process. This makes the process to ignore the signal.
  signal(SIGPIPE, SIG_IGN);

  socket_descriptor = create_server_socket(atoi(argv[1]));

  while(1){
      printf("Waiting connection\n");
      clilen = sizeof(cli_addr);

      new_socket_descriptor = accept(socket_descriptor, (struct sockaddr *) &cli_addr, &clilen);

      int r = fork(); //Para atender vários pedidos ao mesmo tempo

      if (r == 0) {
            printf("Connection from %s\n", inet_ntoa(*((struct in_addr *) &(cli_addr.sin_addr) )));

			strcpy(filename, "fileXXXXXX.bin");
            int fdd = mkstemps(filename, 4);
            if(fdd == -1){
                perror("open destination");
                return 1;
            }

            printf("Writing data received\n");
            while(1) {
                n = read(new_socket_descriptor, buf, BUFSIZE);
                if(n==0)//read until it detects a newline ('\n') or end of data stream
                    break;

                write(fdd, buf, n); //Writes n bytes contained in the buffer into fdd
                
            }

            close(fdd);
            printf("Done receiving file.\n");
            close(new_socket_descriptor);
            // Child done
            exit(0);    
      
  }
}
    return 0;
}

int create_server_socket(int port){
   int s;
   struct  sockaddr_in serv_addr;

   s = socket(PF_INET, SOCK_STREAM, 0);
   if(s<0){
       perror("socket");
       exit(1);
   }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    
    if(bind(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("bind");
        exit(1);
    }    
    
    listen(s, 5); 

    return s;


  
}