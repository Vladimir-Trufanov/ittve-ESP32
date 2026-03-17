const char* host     = "ESP-OTA"; // Используется для разрешения DNS - Used for MDNS resolution

// "OPPO A9 2020"; "TP-Link_B394"; "tve-DESKTOP"; "linksystve"; "linksystve";
// "b277a4ee84e8"; "18009217"    ; "Ue18-647"   ; "x93k6kq6wf"; "X93K6KQ6WF";
const char* ssid     = "TP-Link_B394";
const char* password = "18009217";

#include "WebOTA.h"

long start       = 0;
long max_seconds = 10;
long i           = 2; // Start at 2
long found       = 0; // Количество найденных нами приматов - Number of primtes we've found
int LED_PIN      = 33;

bool is_prime(long num) 
{
	// Нужно только проверить наличие делимого для sqrt(числа) - Only have to check for divisible for the sqrt(number)
	int upper = sqrt(num);

	// Проверьте, делится ли число поровну (начинайте с 2 и увеличивайте) - Check if the number is evenly divisible (start at 2 going up)
	for (long cnum = 2; cnum <= upper; cnum++) 
	{
		long mod = num % cnum; // Remainder

		// If the remainer is 0 it's evenly divisible
		if (mod == 0) 
		{
			return false;
		}
	}

	// Если ты зайдешь так далеко, это будет просто замечательно - If you get this far it's prime
	return true;
}

////////////////////////////////////////////////////

void setup() 
{
	Serial.begin(115200);

	pinMode(LED_PIN, OUTPUT);
	start = millis();

	init_wifi(ssid, password, host);

	// Defaults to 8080 and "/webota"
	// webota.init(80, "/update");
}

void loop() 
{
	digitalWrite(LED_PIN, HIGH);
	bool prime = is_prime(i); // Check if the number we're on is prime

	if (prime) 
	{
		Serial.print(i);
		Serial.println(" is prime ");

		found++;
	}

	int running_seconds = (millis() - start) / 1000;

	if (max_seconds > 0 && (running_seconds >= max_seconds)) 
	{
		Serial.print("Found ");
		Serial.print(found);
		Serial.print(" primes in ");
		Serial.print(max_seconds);
		Serial.println(" seconds");
		digitalWrite(LED_PIN, LOW);

		webota.delay(35000);

		i     = 2;
		found = 0;
		start = millis();
	}

	i++;

	webota.handle();
}
