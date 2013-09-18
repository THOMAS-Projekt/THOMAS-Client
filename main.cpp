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
int main(int argc, char **argv)
{
	// Joystick-Steuerungs-Objekt erzeugen
	JoystickControl *_jsC = new JoystickControl();
	_jsC->Run();
	
	// Programm laufen lassen, Prozessor nicht unnötig belasten (alles läuft in separaten Threads)
	// TODO: Programm-Befehle per Tastatur etc.
	while(true)
		sleep(2);
	
	// Programm erfolgreich beendet
	return 0;
}