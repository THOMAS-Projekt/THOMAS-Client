/*
-- JOYSTICK-KLASSE :: HEADER --
Definiert die Joystick-Klasse.
Diese Klasse erlaubt es, die Verbindung zu einem Joystick herzustellen und dessen Eingabewerte auszulesen.
*/


/* INCLUDES */

// C++-thread-Klasse
#include <thread>


/* KONSTANTEN */

// BYTE-Typ.
#define BYTE char


/* KLASSE */
namespace THOMAS
{
	class Joystick
	{
		// Definiert den Typ der Funktion, die die empfangenen Joystick-Daten verarbeitet.
		// Parameter:
		// -> [1]: Die Joystick-Achsenwerte (Länge: Joystick::GetAxisCount()).
		// -> [2]: Die Joystick-Buttonwerte (Länge: Joystick::GetButtonCount()).
		// -> [3]: Weitere benutzerdefinierte Parameter, die an die Funktion übergeben werden sollen.
		typedef void (*ComputeReceivedDataFunction)(short *, BYTE *, void *);
	
	private:
		// Konstante, definiert die Verzögerung (in us), die mindestens zwischen zwei erkannten Joystick-Ereignissen liegen muss.
		const int _joystickReadDelay = 50000;
		
		// Das interne Joystick-Handle.
		int _js;
		
		// Gibt an, ob der Joystick-Empfangsvorgang läuft.
		bool _receiving = false;
		
		// Der interne Receive()-Thread.
		std::thread *_receiveThread;
		
		// Die Anzahl der Joystick-Achsen.
		int _joystickAxisCount;
		
		// Die Anzahl der Joystick-Buttons.
		int _joystickButtonCount;
		
		// Die Joystick-Achsenwerte.
		short *_joystickAxis;
		
		// Die Joystick-Buttonwerte.
		BYTE *_joystickButtons;
		
		// Die Funktion, die die empfangenen Joystick-Daten verarbeitet.
		ComputeReceivedDataFunction _computeReceivedDataFunction;
		
		// Die an die Verarbeitungsfunktion zu übergebenden benutzerdefinierten Parameter.
		void *_cRDFParams;
		
		// Empfängt die Daten vom Joystick.
		void Receive();
		
		// Wrapper, um die Receive-Memberfunktion sauber an einen separaten Thread zu übergeben. Wird nur von BeginReceive() benutzt.
		// Parameter:
		// -> obj: Das zur Receive-Funktion gehörende Joystick-Objekt.
		static void ReceiveWrapper(Joystick *obj)
		{
			obj->Receive();
		}
		
	public:
		// Konstruktor.
		// Erstellt eine neue Joystick-Instanz und stellt die Verbindung mit dem Joystick her.
		// Parameter:
		// -> computeReceivedDataFunction: Die Funktion, die die empfangenen Joystick-Daten verarbeitet.
		// -> cRDFParams: Die Parameter, die zusätzlich an computeReceivedDataFunction übergeben werden sollen.
		Joystick(ComputeReceivedDataFunction computeReceivedDataFunction, void *cRDFParams);
		
		// Destruktor.
		// Trennt ggf. die Joystick-Verbindung und gibt noch allokierten Speicher frei.
		~Joystick();
		
		// Startet den asynchronen Joystick-Empfangs-Vorgang.
		void BeginReceive();
		
		// Beendet den asynchronen Joystick-Empfangs-Vorgang.
		void EndReceive();
		
		// Gibt die Anzahl der Achsen des verbundenen Joysticks zurück.
		BYTE GetAxisCount();
		
		// Gibt die Anzahl der Buttons des verbundenen Joysticks zurück.
		BYTE GetButtonCount();
	};
}