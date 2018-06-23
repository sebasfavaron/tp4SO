#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define IP "127.0.0.1"
#define PUERTO 6667
#define PACKAGESIZE 1024

int main(void)
{

	char *ip;
	int fd, numbytes,puerto;
	char buf[100];

	puerto=PUERTO;
	ip=IP;
 	
	struct hostent *he;
	/* estructura que recibira informacion sobre el nodo remoto */
 	struct sockaddr_in server;
 
	if ((he=gethostbyname(ip))==NULL)
	{
	printf("gethostbyname() error\n");
 	exit(-1);
 	}
 
	//fd es el socket
 	if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){
 	printf("socket() error\n");
 	exit(-1);
 	}

	//Datos del servidor
 	server.sin_family = AF_INET;
 	server.sin_port = htons(puerto);
 	server.sin_addr = *((struct in_addr *)he->h_addr);
 	bzero(&(server.sin_zero),8);
 
 	if(connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr))==-1)
 	{
 	printf("connect() error\n");
 	exit(-1);
 	}
 
	if ((numbytes=recv(fd,buf,100,0)) == -1)
	{
 	printf("Error en recv() \n");
 	exit(-1);
 	}
 
	buf[numbytes]='\0';
 	//ahi se guarda algo que nos haya mandado el servidor (en formato string)
 
	close(fd);
 	return 0;
}

int sendMessage(){
	int enviar = 1;
	char message[PACKAGESIZE];

	printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");

	while(enviar){
		fgets(message, PACKAGESIZE, stdin);			// Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
		if (!strcmp(message,"exit\n")) enviar = 0;			// Chequeo que el usuario no quiera salir
		if (enviar) send(fd, message, strlen(message) + 1, 0); 	// Solo envio si el usuario no quiere salir.
	}

}