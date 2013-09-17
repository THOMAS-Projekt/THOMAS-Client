/*
-- TCP-CLIENT-KLASSE :: IMPLEMENTIERUNG --
*/


/* INCLUDES */

// Klassenheader
#include "TCPClient.h"
using namespace THOMAS;

// THOMASException-Klasse
#include "THOMASException.h"

// C-String-Funktionen
// In diesem Header ist u.a. die memcpy()-Funktion definiert, ebenso die memset()-Funktion.
#include <cstring>

// UNIX-Funktionen [Non-Standard]
// In diesem Header ist u.a. die close()-Funktion definiert.
#include <unistd.h>

// POSIX-Socket-Funktionen [Non-Standard]
#include <sys/socket.h>

// POSIX-Internet-Protokoll-Funktionen [Non-Standard]
#include <netinet/in.h>

// Netzwerk-Datenbank-Definitionen [Non-Standard]
// Hier wird u.a. die hostent-Struktur definiert, außerdem die gethostbyname()-Funktion.
#include <netdb.h>


/* FUNKTIONEN */

TCPClient::TCPClient()
{
	
}

TCPClient::~TCPClient()
{
	// Ggf. Verbindung trennen
	if(_connected)
		Disconnect();
}

void TCPClient::Connect(std::string serverAddress, unsigned short port)
{
	// Steht schon eine Verbindung?
	if(_connected)
		throw THOMASException("Fehler: Es besteht bereits eine Verbindung zum Server!");
	
	// Socket erstellen
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// Ist bei der Socketerstellung ein Fehler aufgetreten?
	if(_socket == -1)
	{
		// Exception auslösen
		throw THOMASException("Beim Erstellen des Client-Sockets ist ein Fehler aufgetreten!");
	}
	
	// Serverdaten abrufen
	hostent *serverData = gethostbyname(serverAddress.c_str());
	
	// Konnten die Serverdaten ermittelt werden?
	if(serverData == NULL)
		throw THOMASException("Fehler: Der angegebene Server wurde nicht gefunden!");
	
	// Server-Adressstruktur erstellen
	sockaddr_in serverAdd;
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(port);
	memcpy(reinterpret_cast<char *>(&serverAdd.sin_addr.s_addr), reinterpret_cast<char *>(serverData->h_addr_list[0]), serverData->h_length); // Kopiert die ermittelten Serverdaten (z.B. die IP) in die Serveradress-Struktur
	memset(&serverAdd.sin_zero, 0, 8); // Das letzte Element der Struktur muss auf 0 gesetzt sein
	
	// Verbindung herstellen
	if(connect(_socket, reinterpret_cast<sockaddr *>(&serverAdd), sizeof(serverAdd)) == -1)
	{
		// Fehler, Exception auslösen
		throw THOMASException("Fehler: Die Verbindung mit dem Server konnte nicht hergestellt werden!");
	}
	
	// Verbindung steht
	_connected = true;
	
	// Serverdaten-Variable löschen
	delete serverData;
}

void TCPClient::Disconnect()
{
	// Steht überhaupt eine Verbindung?
	if(!_connected)
		throw THOMASException("Fehler: Es besteht keine Verbindung zum Server!");
	
	// Client-Socket schließen
	close(_socket);
	_connected = false;
}

void TCPClient::Send(BYTE *data, int dataLength)
{
	// Besteht eine Verbindung?
	if(!_connected)
		throw THOMASException("Fehler: Es besteht keine Verbindung zum Server!");
	
	// Daten senden
	send(_socket, data, dataLength, 0);
}