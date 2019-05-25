#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
  int txt_len; //deverá indicar o comprimento da string armazenada no campo texto
  char text[4000];
} data_t;


int my_connect(char *servername, char *port);
void send_data(int sd, data_t *dados);



int main (int argc, char* const argv[]) {

  if(argc!=3) {
    printf("Usage: %s server_ip_address port_number\n",argv[0]);
    exit(1); 
  }
    
  data_t dados = {12, "321teste123!"};
  
  int sd = my_connect(argv[1], argv[2]);
  printf("Connected!\n");
  
  send_data(sd, &dados);
  
  return 0;
}


//Completar estar função
void send_data(int sd, data_t *dados)
{
	int nwrite;
	
	FILE *fp = fdopen(sd, "r+");
	nwrite = fwrite(&dados, sizeof(dados), 1, fp); //Enviar data da variavel dados, todos os sizeof(data_t) bytes  são enviados
	
	printf("Alright, message sent!\n");
}




int my_connect(char *servername, char *port) {

  //get server address using getaddrinfo
  struct addrinfo hints;
  struct addrinfo *addrs;
  memset(&hints, 0, sizeof(struct addrinfo));
  
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;


  int r = getaddrinfo(servername, port, &hints, &addrs);
  if (r != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    exit(1);
  }
  
  //create socket
  int s = socket(AF_INET, SOCK_STREAM, 0);
  if (s == -1) {
    perror("socket");  
    exit(2);
  }
  
  //connect to server
  r = connect(s, addrs->ai_addr, addrs->ai_addrlen);
  if (r == -1) {
    perror("connect");  
    close(s);
    exit(3);
  }
  
  return s;
}


