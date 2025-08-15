#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// --- Définition des broches
const int tempPin = A0;
const int ledPin = 13;
const int buzzerPin = 9;

// --- Variables partagées
volatile float currentTemp = 0.0;
SemaphoreHandle_t tempMutex;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Créer le mutex
  tempMutex = xSemaphoreCreateMutex();

  // Créer les tâches FreeRTOS
  xTaskCreate(readTemperatureTask, "ReadTemp", 128, NULL, 3, NULL); // priorité haute pour lecture
  xTaskCreate(alarmTask, "Alarm", 128, NULL, 2, NULL);              // priorité moyenne pour l'alarme
  xTaskCreate(logTask, "Log", 128, NULL, 1, NULL);                  // priorité basse pour affichage
}

// --- Tâche : Lecture température
void readTemperatureTask(void *pvParameters) {
  for (;;) {
    int value = analogRead(tempPin);
    float voltage = value * (5.0 / 1023.0);
    float temp = voltage * 100; // Pour un LM35 par exemple

    if (xSemaphoreTake(tempMutex, (TickType_t)10) == pdTRUE) {
      currentTemp = temp;
      xSemaphoreGive(tempMutex);
    }

    vTaskDelay(100 / portTICK_PERIOD_MS); // Lire toutes les 100 ms
  }
}

// --- Tâche : Vérifier température et activer alarme
void alarmTask(void *pvParameters) {
  for (;;) {
    float tempCopy = 0.0;

    if (xSemaphoreTake(tempMutex, (TickType_t)10) == pdTRUE) {
      tempCopy = currentTemp;
      xSemaphoreGive(tempMutex);
    }

    if (tempCopy > 20.0) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
    } else {
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS); // Vérifie toutes les 150 ms
  }
}

// --- Tâche : Afficher la température dans le Moniteur Série
void logTask(void *pvParameters) {
  for (;;) {
    float tempCopy = 0.0;

    if (xSemaphoreTake(tempMutex, (TickType_t)10) == pdTRUE) {
      tempCopy = currentTemp;
      xSemaphoreGive(tempMutex);
    }

    Serial.print("Température actuelle : ");
    Serial.print(tempCopy, 1);
    Serial.println(" °C");

    vTaskDelay(1000 / portTICK_PERIOD_MS); // Affiche toutes les 1 seconde
  }
}

void loop() {
  // Ne rien mettre ici : FreeRTOS gère tout
}

