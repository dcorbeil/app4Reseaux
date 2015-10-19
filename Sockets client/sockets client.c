////////////////////////////////////////////////////////////////////////////////
//
//		Fichier:  socket client.c
//
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <winsock.h>

#define BUFFER_LENGTH  1025
#define TEXT_LENGH	   1025
#define END_OF_STRING  '\0'

main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	// Les informations du serveur qui host après la connection
	struct hostent *hp;
	char buf[BUFFER_LENGTH];
	int len;
	int rval;
	char text[TEXT_LENGH] = "Bonjour";
	SOCKET ConnectSocket = INVALID_SOCKET;

	WSADATA wsadata;
	WORD version = (1 << 8) + 1;  /* Version 1.1 */

	if (argc != 1)
	{
		fprintf(stderr, "%s manque d'arguments\n", argv[0]);
		exit(1);
	}

	/*  A faire: initialisation de Winsock */
	int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (result != 0)
	{
		printf("Il y a eu un erreur a WSAStartup avec le code: %d\n", result);
		return 1;
	}

	/*  A faire: Création d'un socket de communication  */
	server.sin_family = AF_INET;

	ConnectSocket = socket(server.sin_family, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("Le socket a failé avec l'erreur: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	printf("Entrer le nom de la machine distante: ");
	gets_s(buf, BUFFER_LENGTH); // demande a l'utilisateur le nom de la machine serveur
	hp = gethostbyname(buf); // donne toutes les informations relative au host
	if (hp == 0)
	{
		fprintf(stderr, "%s: machine inconnue\n", buf);
		exit(1);
	}
	memcpy((char *)&server.sin_addr, (char *)hp->h_addr, hp->h_length); // copie l'adresse du serveur dans une autre variable
	printf("Entrer le numero de port dans la machine distante: ");
	gets_s(buf, BUFFER_LENGTH); // demande a l'utilisateur le port sur lequelle il veux se connecté sur le serveur
	server.sin_port = htons((short)atoi(buf)); // cast le numéro de port en int

	/*  A faire: connection au serveur  */
	result = connect(ConnectSocket, &server, &hp->h_length);
	if (result == SOCKET_ERROR)
	{
		printf("Incapable de se connecté");
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}
	
	len = strlen(text);
	if (len != 0)  
	{
		if (send(ConnectSocket, text, len, 0) == SOCKET_ERROR)
		{
			printf("L'envoi a failé avec l'erreur: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
	}

	/*  A faire: Fermeture de socket  */
	/*  A faire: Terminaison de winsock  */
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
