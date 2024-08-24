// Bibliotecas utilizadas:
#include <WiFi.h> ///Permite que o ESP32 se conecte a redes WiFi.
#include <WiFiUdp.h> ///Permite a comunicação UDP(User Datagram Protocol).

#define pinInfra 25
#define pinLed 33
#define pinGas 32

#define lGas 500

///Define o pino 25 como pinGas, o sensor de gás está conectado.

// Declarações e Definições:
WiFiUDP udp; ///Cria um objeto udp para a comunicação UDP.

char packetBuffer[255]; ///Cria um buffer para armazenar dados recebidos via UDP
unsigned int localPort = 9999; ///Define a porta local para comunicação UDP
char *serverip = "192.168.176.3"; ///Define o endereço IP do servidor para enviar dados.
unsigned int serverport = 8888; ///Define a porta do servidor para enviar dados.

const char *ssid = "Redmi 12"; ///Define o SSID da rede WiFi.
const char *password = "thales5g.123"; ///Define a senha da rede WiFi.

bool infra;
int nivelGas;
int cont = 0;
// Função setup:
void setup() {
 	Serial.begin(115200); ///Inicializa a comunicação serial a 115200 bps.

  pinMode(pinInfra, INPUT); ///Configura o pino do sensor de gás como entrada.

  pinMode(pinLed, OUTPUT);

  pinMode(pinGas, INPUT);

  delay(500);
 	// Connect to Wifi network.
 	WiFi.begin(ssid, password); ///Conecta o Esp32 à rede WiFi especificada.
 	while (WiFi.status() != WL_CONNECTED) { ///Aguarda até que a comunicação WiFi seja estabelecida
 			delay(500); ////A cada 500ms
      Serial.print(F(".")); ////Imprime um ponto
 	}
 	udp.begin(localPort); ///Inicia a comunicação UDP na porta local definida
 	Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort); ///Imprime o endereço IP local e a porta UDP

  Serial.println("Fim do setup!");
}

// Função loop:
void loop() {
 
  infra = digitalRead(pinInfra);
  nivelGas = analogRead(pinGas);

if (nivelGas >= lGas) {
    disparaSirene(pinLed, 1000);
    alertaFogo();
  } else if (infra == LOW) {
    disparaSirene(pinLed, 1000);
    alertaFogo();
  } else {
    desligaSirene(pinLed);
  }

  Serial.println(infra);
  Serial.println(nivelGas);

    int packetSize = udp.parsePacket(); ///Verifica se há pacotes UDP recebidos.
 	if (packetSize) { ///Se houver pacotes recebidos:
 			Serial.print(" Received packet from : "); Serial.println(udp.remoteIP()); ///Informa o IP de onde foram recebidos,
 			int len = udp.read(packetBuffer, 255); ///Lê os dados

 			Serial.printf("Data : %s\n", packetBuffer); ///E os imprime.
 			Serial.println();

      if (packetBuffer[0] == 'F'){ //Se o pacote UDP recebido indicar fogo em algum ESP do servidor
        Serial.println("PEGOU FOGO"); //Indica que há fogo em algum lugar
        digitalWrite(pinLed, HIGH); //Acende o LED
        delay(1000); //Espera 1 segundo
        digitalWrite(pinLed, LOW); //Apaga o LED
      } else if(packetBuffer[0] == 'G'){
        Serial.println("Vazamento detectado");
        digitalWrite(pinLed, HIGH); //Acende o LED
        delay(1000); //Espera 1 segundo
        digitalWrite(pinLed, LOW); //Apaga o LED
      }
 	}



 	udp.beginPacket(serverip, serverport); ///Inicia um pacote UDP para o servidor. 
 	char buf[30]; ///Cria um buffer para a mensagem.
  char buf1[30];

  unsigned long testeID = millis(); ///Obtém o tempo em ms desde que o Esp32 foi ligado.

if(cont == 30){
char bufE[30];
 
 if(nivelGas < 350){
  sprintf(bufE, "Normal");
} else if(nivelGas >= 350 & nivelGas < 500) {
   sprintf(bufE, "Anormal");
} 

 	sprintf(buf, "Sensor do fogo: %lu", infra); ///Formata a mensagem com o valor do fogo.
  delay(200);
  sprintf(buf1, " Sensor de Gas: %lu ", nivelGas);  

 	udp.printf(buf); ///Envia a mensagem via UDP.
  udp.printf(buf1);
  udp.printf(bufE);
 	udp.endPacket(); ///Finaliza o pacote UDP.
  delay(500);
  cont = 0;
}
cont++;
Serial.print("temporizador: ");
Serial.println(cont);
delay(1000);
}


void disparaSirene (byte pin, int intervalo) {
  digitalWrite(pin, HIGH);

  delay(intervalo);

  digitalWrite(pin, LOW);

  delay(intervalo);
}

void desligaSirene(byte pin) {
  digitalWrite(pin,LOW);
}

void alertaFogo(){
  while(infra != HIGH || nivelGas > 500){
    Serial.println("Fogo detectado!");
    udp.beginPacket(serverip, serverport);
    char bufA[30];
    char bufB[30];
    char bufC[30];
    sprintf(bufA, "Sensor do Fogo: %lu ", infra);
    sprintf(bufB, "Sensor do Gas: %lu ", nivelGas);
    sprintf(bufC, "Alerta!");
    udp.printf(bufA);
    udp.printf(bufB);
    udp.printf(bufC);
    udp.endPacket();
    delay(500);
    infra = digitalRead(pinInfra);
    nivelGas = analogRead(pinGas);
  }
  cont = 0;
}