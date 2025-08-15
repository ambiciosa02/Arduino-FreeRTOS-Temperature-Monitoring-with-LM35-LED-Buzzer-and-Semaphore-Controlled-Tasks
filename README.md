# Arduino-FreeRTOS-Temperature-Monitoring-with-LM35-LED-Buzzer-and-Semaphore-Controlled-Tasks
This is a Temperature Monitoring and Alarm System using Arduino with FreeRTOS, LEDs, and Buzzer.

🔍 How it works
1️⃣ Hardware used

Arduino (Uno, Nano, Mega, etc.)

Analog temperature sensor (e.g., LM35)

LED (pin 13)

Buzzer (pin 9)

2️⃣ Program flow

The program uses FreeRTOS to run tasks concurrently:

readTemperatureTask

Reads the analog temperature sensor every 100 ms.

Converts analog value to temperature (°C).

Stores it in a shared variable currentTemp protected by a mutex.

alarmTask

Checks the current temperature every 1 s.

If temperature exceeds 20°C, turns on the LED and buzzer.

Otherwise, turns them off.

logTask

Prints the current temperature to the Serial Monitor every 1 s.

Mutex ensures that multiple tasks don’t access currentTemp simultaneously, preventing race conditions.

3️⃣ Notes

FreeRTOS allows the reading, alarm, and logging tasks to run independently, which is more efficient than a single-loop approach.

Temperature threshold (20°C) can be adjusted as needed.
