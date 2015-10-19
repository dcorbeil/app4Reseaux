////////////////////////////////////////////////////////////////////////////////
//
//		Fichier:  Sockets server.c
//
////////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>

#define BUFFER_LENGTH  1024
#define END_OF_STRING  '\0'


main(int argc, char *argv[])
{	
	int sock, msgsock;
	int length;
	int rval;
	char buf[BUFFER_LENGTH];
	struct sockaddr_in server;
	struct sockaddr_in client;
	struct hostent *cname;
	int port;

	WSADATA wsadata;
	WORD version = (1 << 8) + 1;  /* Version 1.1 de WINSOCK*/

	if (argc > 2)
	{	fprintf(stderr,"Usage: %s [portnumber]\n", argv[0]);
		exit(1);
	}

	/*  ???? Indiquer quelle opération on effectue ici????? */

	if (WSAStartup(version, &wsadata) != 0)    
	{	perror("lors de l'initialisation winsock");
		exit(1);
	}
	
	/*  ???? Indiquer quelle opération on effectue ici????? */
	if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{	perror("lors de l'ouverture du socket");
		exit(1);
	}


	server.sin_family = AF_INET;    /*?????????????????????*/
	server.sin_addr.s_addr = INADDR_ANY; /*?????????????????????*/
	if (argc == 2)  /*?????????????????????*/
		port = htons((short) atoi(argv[1]));  /*?????????????????????*/
	else
		port = 0;  /*?????????????????????*/
	server.sin_port = port; /*?????????????????????*/
	if (bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0) /*?????????????????????*/
	{	perror("lors de l'association au numero de port");
		exit(1);
	}

	/*  ???? Indiquer quelle opération on effectue ici????? */

	length = sizeof(server);
	if (getsockname(sock, (struct sockaddr *)&server, &length) < 0) /*?????????????????????*/
	{	perror("lors de l'interrogation sur le socket");
		exit(1);
	}
	printf("Le port %d a ete associe au socket\n", ntohs(server.sin_port));

	/*  ???? Indiquer quelle opération on effectue ici????? */
	listen(sock, 5);

	/*  ???? Indiquer quelles opérations on effectue ici????? */
	while (1)  /*?????????????????????*/
	{	length = sizeof(client);
		msgsock = accept(sock, (struct sockaddr *) &client, &length); /*?????????????????????*/
		cname = gethostbyaddr((char *) &client.sin_addr, 4, AF_INET); /*?????????????????????*/
		if (cname == NULL)
			printf("Connection d'une machine inconnue (%s), ",
				inet_ntoa(client.sin_addr));
		else
			printf("Connection de la machine %s (%s), ", cname->h_name, /*?????????????????????*/
				inet_ntoa(client.sin_addr));
		printf("au port %d\n", client.sin_port); /*?????????????????????*/

		if (msgsock < 0)
		{	perror("lors de l'acceptation"); /*?????????????????????*/
		}

		/*  ???? Indiquer quelle opération on effectue ici????? */
		else do  /*?????????????????????*/
		{	if ((rval = recv(msgsock, buf, sizeof(buf), 0)) < 0)
			{	perror("lors de la lecture du message");
			}
			if (rval == 0)
				printf("Fin de connection\n\n");
			else
			{	buf[rval] = END_OF_STRING;
				printf("Recu: '%s'\n", buf);
				printf("Entrer les caracteres+retour de chariot pour envoyer, ou juste retour de chariot pour terminer:\n ");
				gets_s(buf, BUFFER_LENGTH);
				length = strlen(buf);
				if (length == 0)  break;
				if (send(msgsock, buf, length, 0) < 0) /*?????????????????????*/
					{	perror("lors de l'ecriture dans le socket");
						exit(1);
					}
			}
		} while (rval != 0);


		closesocket(msgsock); /*?????????????????????*/


	} 

	closesocket(sock); /*?????????????????????*/
	WSACleanup(); /*?????????????????????*/

	return 0;
}  