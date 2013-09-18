/*
-- JOYSTICK-CONTROL-KLASSE :: IMPLEMENTIERUNG --
*/


/* INCLUDES */

// Klassenheader
#include "JoystickControl.h"
using namespace THOMAS;

// THOMASException-Klasse
#include "THOMASException.h"

// C-String-Funktionen
// Hier ist u.a. die memcpy()-Funktion definiert.
#include <cstring>


/* FUNKTIONEN */

JoystickControl::JoystickControl()
{
	// Verbindung zum Joystick herstellen
	_joystick = new Joystick(&JoystickControl::ComputeJoystickDataWrapper, static_cast<void *>(this));
	
	// Joystick-Daten abrufen
	_joystickAxisCount = _joystick->GetAxisCount();
	_joystickButtonCount = _joystick->GetButtonCount();
	
	// Sendepuffer-Länge berechnen
	_joystickDataSendBuffLength = 1 + sizeof(short) * _joystickAxisCount + _joystickButtonCount;
	
	// Server-Verbindungs-Objekt erstellen
	_serverCon = new TCPClient();
}

JoystickControl::~JoystickControl()
{
	// Ggf. Steuerung beenden
	if(_running)
		Stop();
	
	// Joystick-Verbindung löschen
	delete _joystick;
	
	// Server-Verbindung löschen
	delete _serverCon;
}

void JoystickControl::Run()
{
	// Läuft die Steuerung bereits?
	if(_running)
		throw THOMASException("Fehler: Die Steuerung läuft bereits!");
	
	// Mit Server verbinden
	_serverCon->Connect("localhost", 4242);
	
	// Joystick-Daten an Server übermitteln (JOYSTICK_HEADER)
	BYTE sendBuff[3] = {1, _joystickAxisCount, _joystickButtonCount};
	_serverCon->Send(sendBuff, 3);
	
	// Steuerung läuft
	_running = true;
	
	// Joystick-Empfangsmodus starten
	_joystick->BeginReceive();
}

void JoystickControl::Stop()
{
	// Läuft die Steuerung?
	if(!_running)
		throw THOMASException("Fehler: Die Steuerung läuft nicht!");
	
	// Steuerung läuft nicht mehr
	_running = false;
	
	// Joystick-Empfangsmodus beenden
	_joystick->EndReceive();
	
	// Server-Verbindung beenden
	_serverCon->Disconnect();
}

void JoystickControl::ComputeJoystickData(short *axis, BYTE *buttons)
{
	// Sende-Puffer erstellen
	BYTE *buff = new BYTE[_joystickDataSendBuffLength];
	
	// Kommandobyte (JOYSTICK_DATA)
	buff[0] = 2;
	buff++;
	
	// Joystick-Daten in Puffer kopieren
	memcpy(buff, axis, sizeof(short) * _joystickAxisCount);
	buff += sizeof(short) * _joystickAxisCount;
	memcpy(buff, buttons, _joystickButtonCount);
	
	// Daten an Server senden
	_serverCon->Send(buff, _joystickDataSendBuffLength);
	
	// Puffer löschen (TODO: Entsteht hier evtl. ein Speicherleck? Oder hat die Pointer-Inkrementierung keine Auswirkung auf delete?)
	delete buff;
}