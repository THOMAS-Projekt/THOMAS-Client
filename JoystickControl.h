/*
-- JOYSTICK-CONTROL-KLASSE :: HEADER --
Definiert die JoystickControl-Klasse.
Diese Klasse empfängt die Steuerdaten des Joysticks und leitet diese dann an den Server weiter.
*/


/* INCLUDES */

// Joystick-Klasse
#include "Joystick.h"

// TCPClient-Klasse
#include "TCPClient.h"


/* KLASSE */
namespace THOMAS
{
	class JoystickControl
	{
	private:
		// Gibt an, ob die Joystick-Steuerung aktiv ist.
		bool _running = false;
		
		// Die Anzahl der Joystick-Achsen.
		BYTE _joystickAxisCount;
		
		// Die Anzahl der Joystick-Buttons.
		BYTE _joystickButtonCount;
		
		// Die Verbindung zum Server.
		TCPClient *_serverCon;
		
		// Die Verbindung zum Joystick.
		Joystick *_joystick;
		
		// Die Länge des JOYSTICK_DATA-Sendepuffers.
		int _joystickDataSendBuffLength;
		
		// Verarbeitet die empfangenen Joystick-Daten.
		// Parameter:
		// -> axis: Die empfangenen Joystick-Achsenwerte.
		// -> buttons: Die empfangenen Joystick-Buttonwerte.
		void ComputeJoystickData(short *axis, BYTE *buttons);
		
		// Wrapper, um die ComputeJoystickData-Memberfunktion sauber an einen separaten Thread zu übergeben.
		// Parameter:
		// -> axis: Die empfangenen Joystick-Achsenwerte.
		// -> buttons: Die empfangenen Joystick-Buttonwerte.
		// -> obj: Das zur ComputeJoystickData-Funktion gehörende JoystickControl-Objekt.
		static void ComputeJoystickDataWrapper(short *axis, BYTE *buttons, void *obj)
		{
			(reinterpret_cast<JoystickControl *>(obj))->ComputeJoystickData(axis, buttons);
		}
		
	public:
		// Konstruktor.
		// Erstellt eine neue JoystickControl-Instanz.
		JoystickControl();
		
		// Destruktor.
		// Beendet alle noch bestehenden Verbindungen und gibt Speicher frei.
		~JoystickControl();
		
		// Startet die Joystick-Steuerung.
		void Run();
		
		// Beendet die Joystick-Steuerung.
		void Stop();
	};
}