#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Definição de pinos dos sensores e atuadores
#define DHTPIN 15           // Pino do sensor DHT22
#define DHTTYPE DHT22       // Tipo do sensor DHT
#define TRIG_PIN 4          // Pino trigger do sensor ultrassônico
#define ECHO_PIN 5          // Pino echo do sensor ultrassônico
#define LED_PIN 19          // LED de alerta de enchente
#define BUZZER_PIN 23       // Buzzer de alerta

// Variáveis de ajuste e cálculo
float TempAjus;
float VentAjus;
float probabilidade;

// Instância dos sensores
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MPU6050 mpu;

// Array circular para armazenar últimas 10 leituras do sensor ultrassônico
float distancias[10];
int totalLeituras = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando sensores...");

  // Inicializa o sensor DHT
  dht.begin();

  // Inicializa o sensor MPU6050 e configura os parâmetros
  if (!mpu.begin()) {
    Serial.println("Falha ao detectar o MPU6050. Verifique as conexões.");
    while (1) delay(10); // Loop infinito se falhar
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Configura os pinos de entrada/saída
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Sensores iniciados com sucesso.");
}

void loop() {
  // ======== LEITURA DE TEMPERATURA E UMIDADE ========
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler do sensor DHT!");
  } else {
    Serial.printf("Temperatura: %.2f °C, Umidade: %.2f %%\n", temperatura, umidade);
  }

  // ======== LEITURA DO ACELERÔMETRO COMO "ANEMÔMETRO" ========
  sensors_event_t acc, gyro, temp;
  mpu.getEvent(&acc, &gyro, &temp);
  float vento = acc.acceleration.x * 3.6;  // Simula a velocidade do vento em km/h
  Serial.println("Velocidade do vento: " + (String)vento + " km/h");

  /*
    OBS: Como o Wokwi não possui anemômetro, o acelerômetro foi usado
    como substituto apenas para simulação da lógica de cálculo do vento.
  */

  // ======== AJUSTES PARA CÁLCULO DA PROBABILIDADE DE CHUVA ========
  TempAjus = (temperatura < 18) ? 0 : (temperatura > 30) ? 24 : (temperatura - 18) * 2;
  VentAjus = (vento < 5) ? 0 : (vento > 20) ? 30 : (vento - 5) * 2;

  probabilidade = (0.6 * umidade) + (0.3 * TempAjus) + (0.1 * VentAjus);
  Serial.println("Probabilidade de Chuva: " + (String)probabilidade + "%");

  // ======== LEITURA DO SENSOR ULTRASSÔNICO (NÍVEL DA ÁGUA) ========
  long duracao;
  float distancia;

  // Pulso para medir a distância
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duracao = pulseIn(ECHO_PIN, HIGH);
  distancia = duracao * 0.034 / 200; // Conversão da duração para distância (em metros)

  // ======== ARMAZENAMENTO DAS ÚLTIMAS 10 LEITURAS ========
  if (totalLeituras < 10) {
    distancias[totalLeituras++] = distancia;
  } else {
    // Desloca os valores para inserir a nova leitura no final (FIFO)
    for (int i = 0; i < 9; i++) {
      distancias[i] = distancias[i + 1];
    }
    distancias[9] = distancia;
  }

  Serial.printf("Distância atual: %.2f m\n", distancia);

  // ======== ANÁLISE DE RISCO DE ENCHENTE ========
  if (totalLeituras == 10) {
    float primeira = distancias[0];
    float ultima = distancias[9];
    float diferenca = abs(ultima - primeira); // Usa valor absoluto

    Serial.printf("Diferença entre a 1ª e a 10ª leitura: %.2f m\n", diferenca);

    // Condição de alerta de enchente: distância baixa e diferença crescente
    if (distancia < 0.5 && diferenca > 0.3) {
      Serial.println("CUIDADO: Alta chance de enchente nos próximos momentos!");
      digitalWrite(LED_PIN, HIGH);       // Acende LED de alerta
      tone(BUZZER_PIN, 1000);            // Ativa buzzer com tom de 1000 Hz
    } else {
      digitalWrite(LED_PIN, LOW);        // Desliga LED
      noTone(BUZZER_PIN);                // Desliga buzzer
    }
  }

  Serial.println("-----------------------------");
  delay(5000); // Espera 5 segundos antes de repetir o ciclo
}
