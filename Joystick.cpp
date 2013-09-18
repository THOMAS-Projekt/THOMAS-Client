/*
-- JOYSTICK-KLASSE :: IMPLEMENTIERUNG --
*/


/* INCLUDES */

// Klassenheader
#include "Joystick.h"
using namespace THOMAS;

// THOMASException-Klasse
#include "THOMASException.h"

// C-String-Funktionen
// Hier ist u.a. die memcpy()-Funktion definiert.
#include <cstring>

// UNIX-Funktionen [Non-Standard]
// In diesem Header ist u.a. die close()-Funktion definiert, ebenso die usleep()-Funktion.
#include <unistd.h>

// File-Control-Definitionen [Non-Standard]
// Dieser Header erhält u.a. die Definition des open()-Systemaufrufs.
#include <fcntl.h>

// Linux-I/O-Kontroll-Funktionen [Non-Standard]
// Hier wird die ioctl()-Funktion definiert.
#include <sys/ioctl.h>

// Linux-Joystick-Funktionen [Non-Standard]
#include <linux/joystick.h>


/* FUNKTIONEN */

Joystick::Joystick(ComputeReceivedDataFunction computeReceivedDataFunction, void *cRDFParams)
{
	// Datenverarbeitungsfunktion merken
	_computeReceivedDataFunction = computeReceivedDataFunction;
	_cRDFParams = cRDFParams;
	
	// Verbindung zum Joystick herstellen
	_js = open("/dev/input/js0", O_RDONLY | O_NDELAY);
	
	// Wurde die Verbindung erfolgreich hergestellt?
	if(_js == -1)
	{
		// Nicht gut
		throw THOMASException("Die Verbindung zum Joystick konnte nicht hergestellt werden!");
	}
	
	// Joystickdaten abrufen
	ioctl(_js, JSIOCGAXES, &_joystickAxisCount);
	ioctl(_js, JSIOCGBUTTONS, &_joystickButtonCount);
}

Joystick::~Joystick()
{
	// Ggf. Empfangsmodus beenden
	if(_receiving)
		EndReceive();
	
	// Joystick-Verbindung trennen
	close(_js);
}

int Joystick::GetAxisCount()
{
	// Anzahl der Achsen zurückgeben
	return static_cast<BYTE>(_joystickAxisCount);
}

int Joystick::GetButtonCount()
{
	// Anzahl der Buttons zurückgeben
	return static_cast<BYTE>(_joystickButtonCount);
}

void Joystick::BeginReceive()
{
	// Läuft schon der Empfangsvorgang?
	if(_receiving)
		throw THOMASException("Fehler: Der Empfangsvorgang läuft bereits!");
	
	// Empfangsvorgang läuft
	_receiving = true;
	
	// Empfangsthread erstellen
	_receiveThread = new std::thread(&Joystick::ReceiveWrapper, this);
}

void Joystick::EndReceive()
{
	// Läuft der Empfangsvorgang?
	if(!_receiving)
		throw THOMASException("Fehler: Der Empfangsvorgang läuft nicht!");
	
	// Empfangsvorgang beenden
	_receiving = false;
	_receiveThread->join();
	
	// Joystick-Arrays löschen
	delete _joystickAxis;
	delete _joystickButtons;
	
	// Thread-Objekt löschen
	delete _receiveThread;
}

void Joystick::Receive()
{
	// Joystick-Daten-Arrays initialisieren
	_joystickAxis = new short[_joystickAxisCount];
	_joystickButtons = new BYTE[_joystickButtonCount];
	
	// Solange Daten empfangen, bis das Programm beendet wird
	js_event e; // Joystick-Ereignisdaten
	short _tempAxis[_joystickAxisCount]; // Hilfsarray, um Joystick-Achsenwerte sicher zu übergeben
	BYTE _tempButtons[_joystickButtonCount]; // Hilfsarray, um Joystick-Buttonwerte sicher zu übergeben
	while(_receiving)
	{
		// Joystick-Ereignis-Daten auslesen
		if(read(_js, &e, sizeof(js_event)) == -1)
		{
			// Fehler
			throw THOMASException("Fehler beim Auslesen der aktuellen Joystick-Daten!");
		}
		
		// Ereignistypen unterscheiden
		switch(e.type & ~JS_EVENT_INIT)
		{
			// Eine Bewegung entlang einer Achse wurde durchgeführt
			case JS_EVENT_AXIS:
			{
				// Achsenwert speichern
				_joystickAxis[e.number] = e.value;
				
				// Fertig
				break;
			}
			
			// Ein Button wurde gedrückt / losgelassen
			case JS_EVENT_BUTTON:
			{
				// Buttonwert speichern
				_joystickButtons[e.number] = e.value;
				
				// Fertig
				break;
			}
		}
		
		// Joystick-Daten-Arrays kopieren => so kann die Handler-Funktion die Arrays verändern, ohne dass das weitere Auswirkungen hat
		memcpy(_tempAxis, _joystickAxis, sizeof(short) * _joystickAxisCount);
		memcpy(_tempButtons, _joystickButtons, _joystickButtonCount);
		
		// Handler-Funktion aufrufen
		_computeReceivedDataFunction(_tempAxis, _tempButtons, _cRDFParams);
		
		// Kurz warten, um Joystickverbindung und Handlerfunktion nicht zu stark zu belasten
		usleep(_joystickReadDelay);
	}
}