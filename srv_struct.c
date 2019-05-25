#include <unistd.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct {
  int txt_len; //deverá indicar o comprimento da string armazenada no campo texto
  char text[4000];
} data_t;

int my_create_server_socket(char *port);
void print_client_address(const struct sockaddr * clt_addr, socklen_t addrlen);
int receive_data(int sd, data_t *dados);


int main(int argc, char *argv[]){
  int ns, socket_descriptor; 
  int n;
  struct sockaddr clt_addr;
  socklen_t addrlen;
  data_t dados;  
      
  if(argc!=2) {
    printf("Usage: %s port_number\n", argv[0]);
    exit(1); 
  }

  
  //SIGPIPE is sent to the process if a write is made to a closed connection.
  //By default, SIGPIPE terminates the process. This makes the process to ignore the signal.
  signal(SIGPIPE, SIG_IGN);

       
  socket_descriptor = my_create_server_socket(argv[1]);

  while(1) {
    
    printf("Waiting connection\n");  
  
    addrlen = sizeof(clt_addr);
    //accept a new connection to a client
    ns = accept(socket_descriptor, &clt_addr, &addrlen); 
    if(ns < 0) {
      perror("accept");
      sleep(1);
      continue;
    }
    print_client_address(&clt_addr, addrlen);


    n = receive_data(ns, &dados);
    printf("%d %d %s\n", n, dados.txt_len, dados.text);

    
    close(ns);
  }
  
  return 0;
}

//Completar estar função. O valor de retorno deverá ser o número de bytes recebidos
int receive_data(int sd, data_t *dados)
{		
  FILE *fp = fdopen(sd, "r+");
  fread(&dados, 1, sizeof(data_t), fp); // Receber a variavel dados
  
  printf("%d %s", dados->txt_len, dados->text);
}




void print_client_address(const struct sockaddr * clt_addr, socklen_t addrlen) 
{
  char hostname[256];  
  char port[6];

  int n = getnameinfo(clt_addr, addrlen, hostname, sizeof(hostname), 
    port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV); 
  if(n != 0)
    printf("%s\n", gai_strerror(n));
  printf("Connection from %s:%s\n", hostname, port);
}

int my_create_server_socket(char *port)
{
  int s, r;
  struct addrinfo hints, *a;

  s = socket(PF_INET, SOCK_STREAM, 0);
  if(s == -1){
    perror("socket");
    exit(1);
  }

  memset (&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_flags = AI_PASSIVE;
  r = getaddrinfo(NULL, port, &hints, &a);
  if (r != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  } 
  
  r = bind(s, a->ai_addr, a->ai_addrlen);
  if(r == -1) {
    perror("bind");
    exit(1);
  }    
  
  r = listen(s, 5); 
  if(r == -1) {
    perror("listen");
    exit(1);
  }   
  
  return s;
}

