#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#define MYPORT 8001      /* puerto donde SDRstick envia las muestras */
#define MAXBUFLEN 1500   /* Max. cantidad de bytes que podra recibir en una  llamada a recvfrom() */

int main() {
  int sockfd;
  FILE *archivo;
  int frame;
  struct sockaddr_in my_addr;    /* direccion IP y numero de puerto local */
  struct sockaddr_in their_addr; /* direccion IP y numero de puerto del cliente */
  /* addr_len contendra el tamanio de la estructura sockadd_in  y numbytes el numero de bytes recibidos */
  int addr_len, numbytes;
  char buf[MAXBUFLEN];   /* Buffer de recepcion */
  /* se crea el socket */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  /* Se establece la estructura my_addr para luego llamar a bind() */
  my_addr.sin_family = AF_INET;   /* host byte order */
  my_addr.sin_port = htons(MYPORT); /* network byte order */
  my_addr.sin_addr.s_addr = INADDR_ANY;  /* se asigna automaticamente la direccion IP local */
  bzero(&(my_addr.sin_zero), 8);  /* rellena con ceros el resto de la estructura */
  /* Se le da un nombre al socket */
  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))  == -1) {
    perror("bind");
    exit(1);
  }
  /* Abro archivo de salida */
  if ((archivo = fopen("/home/mdoallo/Descargas/sdrstick.dat","w")) == NULL) {
    perror("Archivo de salida");
    exit(1);
  }
  
  /* Se reciben los datos */
  printf("Esperando datos desde SDRstick\n");
  addr_len = sizeof(struct sockaddr);
  while (1) {
    if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }
    /* Se visualiza lo recibido */
    fwrite(&buf[8],1400,1,archivo);
  }
  /* devolvemos recursos al sistema */
  close(sockfd);
  fclose(archivo);
}
