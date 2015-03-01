/*
-- HAUPTPROGRAMM --
*/


/* INCLUDES */

// JoystickControl-Klasse
#include "JoystickControl.h"
using namespace THOMAS;

// UNIX-Standard-Funktionen [Non-Standard]
// Die sleep()-Funktion wird benötigt.
#include <unistd.h>

/* FUNKTIONEN */

// Programm-Einstiegsfunktion.
int main(int argc, char** argv)
{
	// Argumente prüfen. 1 = IP
	if(argc < 2)
	{
		// Fehlermeldung ausgeben
		printf("Es fehlen Parameter <IP>\n");
		return 0;
	}

	// Joystick-Steuerungs-Objekt erzeugen
	JoystickControl *_jsC = new JoystickControl();
	_jsC->Run(argv[1]);

	// Programm laufen lassen, Prozessor nicht unnötig belasten (alles läuft in separaten Threads)
	// TODO: Programm-Befehle per Tastatur etc.
	while(true)
		sleep(2);

	// Programm erfolgreich beendet
	return 0;
}
