#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define TRIG_PIN 4
#define ECHO_PIN 5
#define LED_PIN 19
#define BUZZER_PIN 23

float TempAjus;
float VentAjus;
float probabilidade;

DHT dht(DHTPIN, DHTTYPE);
Adafruit_MPU6050 mpu;


float distancias[10];
int totalLeituras = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando sensores...");

  dht.begin();

  if (!mpu.begin()) {
    Serial.println("Falha ao detectar o MPU6050. Verifique as conexões.");
    while (1) delay(10);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Sensores iniciados com sucesso.");
}

void loop() {
  
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Falha ao ler do sensor DHT!");
  } else {
    Serial.printf("Temperatura: %.2f °C, Umidade: %.2f %%\n", temperatura, umidade);
  }

 
  sensors_event_t acc, gyro, temp;
  mpu.getEvent(&acc, &gyro, &temp);
  float vento = acc.acceleration.x * 3.6; 
  Serial.println("Velocidade do vento: "+(String)vento + "km/h"); 

  /* Seria necessario um anenometro para captar a velocidade do vento mas para 
  essa simulação utilizei um acelerometro como anenometro pois o wokwi não possui anenometro
  
  sensor ultrassonico verifica o nível da água em relação a superfície
  */

 
  TempAjus = (temperatura < 18) ? 0 : (temperatura > 30) ? 24 : (temperatura - 18) * 2;
  VentAjus = (vento < 5) ? 0 : (vento > 20) ? 30 : (vento - 5) * 2;

  probabilidade = (0.6 * umidade) + (0.3 * TempAjus) + (0.1 * VentAjus);
  Serial.println("Probabilidade de Chuva: " + (String)probabilidade + "%");

  
  long duracao;
  float distancia;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duracao = pulseIn(ECHO_PIN, HIGH);
  distancia = duracao * 0.034 / 200;  // metros

  
  if (totalLeituras < 10) {
    distancias[totalLeituras++] = distancia;
  } else {
    for (int i = 0; i < 9; i++) {
      distancias[i] = distancias[i + 1];
    }
    distancias[9] = distancia;
  }

  Serial.printf("Distância atual: %.2f m\n", distancia);

  if (totalLeituras == 10) {
    float primeira = distancias[0];
    float ultima = distancias[9];
    float diferenca = abs(ultima - primeira);

    Serial.printf("Diferença entre a 1ª e a 10ª leitura: %.2f m\n", diferenca);

    if (distancia < 0.5 && diferenca > 0.3){
      Serial.println("Cuidado a chance de enchente nos proximos momentos é alta");
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZER_PIN, 1000);


    }else
    {
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZER_PIN); 
    }
  }

  Serial.println("-----------------------------");
  delay(5000);
}
