#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "client.h"

int sendMessage(int fd)
{
	int enviar = 1;
	char message[PACKAGESIZE];

	while (enviar)
	{
		fgets(message, PACKAGESIZE, stdin); // Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
		if (!strcmp(message, "exit\n"))
			enviar = 0; // Chequeo que el usuario no quiera salir
		if (enviar){
			send(fd, message, strlen(message) + 1, 0); // Solo envio si el usuario no quiere salir.
		}else{
			exit(0);
		}
	}
	return 0;
}

int main(void)
{

	char *ip;
	int fd, numbytes, puerto;
	char buf[100];

	// puerto = PUERTO;
	// ip = IP;

	struct hostent *he;
	/* estructura que recibira informacion sobre el nodo remoto */
	struct sockaddr_in server;

	system("clear");

	if ((he = gethostbyname(IP)) == NULL)
	{
		printf("gethostbyname() error\n");
		exit(-1);
	}

	//fd es el socket
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket() error\n");
		exit(-1);
	}

	//Datos del servidor
	server.sin_family = AF_INET;
	server.sin_port = htons(PUERTO);
	server.sin_addr.s_addr=inet_addr(IP);
	bzero(&(server.sin_zero), 8);

	if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		printf("connect() error\n");
		exit(-1);
	}

	printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n\n");
	printf("Comandos:\n\n");
	printf("Para dar de alta un vuelo\n");
	printf("create flight + Nombre del vuelo + Cantidad de lugares\n\n");
	printf("Para dar de baja un vuelo\n");
	printf("cancel flight + Nombre del vuelo\n");
	printf("Para reservar su asiento\n");
	printf("book + Nombre del vuelo + Numero de asiento\n\n");
	printf("Para cencelar su reserva\n");
	printf("cancel seat + Nombre del vuelo + Numero de asiento\n\n");

	sendMessage(fd);

	if ((numbytes = recv(fd, buf, 100, 0)) == -1)
	{
		printf("Error en recv() \n");
		exit(-1);
	}

	buf[numbytes] = '\0';
	//ahi se guarda algo que nos haya mandado el servidor (en formato string)

 	printf("Servidor: %s\n",buf);
 	sendMessage(fd);

	close(fd);
	return 0;
}
