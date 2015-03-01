#pragma once
/*
-- TCP-CLIENT-KLASSE :: HEADER --
Definiert die TCPClient-Klasse.
Diese Klasse stellt einen TCP-Client an einen angegebenen Server-Port dar, der Daten an diesen Server übermitteln kann.
*/


/* INCLUDES */

// C++-string-Klasse
// Hiermit wird die Übergabe der Server-URL realisiert.
#include <string>


/* KONSTANTEN */

// BYTE-Typ.
#define BYTE char


/* KLASSE */
namespace THOMAS
{
	class TCPClient
	{
	private:
		// Das Socket-Handle.
		int _socket;

		// Gibt an, ob eine Verbindung besteht.
		bool _connected = false;

	public:
		// Konstruktor.
		// Erstellt eine neue TCPClient-Instanz.
		TCPClient();

		// Destruktor.
		// Trennt die Client-Server-Verbindung und gibt noch allokierten Speicher frei.
		~TCPClient();

		// Stellt die Verbindung mit dem angegebenen Server her.
		// Parameter:
		// -> serverAddress: Die Adresse des Servers, mit dem verbunden werden soll.
		// -> serverPort: Der Port des Servers, mit dem verbunden werden soll.
		void Connect(std::string serverAddress, unsigned short port);

		// Trennt die Verbindung mit dem Server.
		void Disconnect();

		// Sendet die angegebenen Daten an den Server.
		// Parameter:
		// -> data: Die zu sendenden Daten.
		// -> dataLength: Die Länge der zu sendenden Daten.
		void Send(BYTE *data, int dataLength);
	};
}
