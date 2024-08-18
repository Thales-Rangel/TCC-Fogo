// Bibliotecas utilizadas:
#include <WiFi.h>    ///Permite que o ESP32 se conecte a redes WiFi.
#include <WiFiUdp.h> ///Permite a comunicação UDP(User Datagram Protocol).

#define pinInfra 25 /// Define o pino 25 como pinGas, o sensor de gás está conectado.
#define pinLed 33

// Declarações e Definições:
WiFiUDP udp; /// Cria um objeto udp para a comunicação UDP.

char packetBuffer[255];           /// Cria um buffer para armazenar dados recebidos via UDP
unsigned int localPort = 9999;    /// Define a porta local para comunicação UDP
char *serverip = "192.168.176.3"; /// Define o endereço IP do servidor para enviar dados.
unsigned int serverport = 8888;   /// Define a porta do servidor para enviar dados.

const char *ssid = "Redmi 12";         /// Define o SSID da rede WiFi.
const char *password = "thales5g.123"; /// Define a senha da rede WiFi.

// Função setup:
void setup(){
  Serial.begin(115200); /// Inicializa a comunicação serial a 115200 bps.

  pinMode(pinInfra, INPUT); /// Configura o pino do sensor de gás como entrada.

  pinMode(pinLed, OUTPUT);

  delay(500);

  // Connect to Wifi network.
  WiFi.begin(ssid, password); /// Conecta o Esp32 à rede WiFi especificada.
  while (WiFi.status() != WL_CONNECTED){ /// Aguarda até que a comunicação WiFi seja estabelecida
    delay(500);           ////A cada 500ms
    Serial.print(F(".")); ////Imprime um ponto
  }

  udp.begin(localPort);/// Inicia a comunicação UDP na porta local definida
  Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort); /// Imprime o endereço IP local e a porta UDP

  Serial.println("Fim do setup!");
}

// Função loop:
void loop(){
  bool infra = digitalRead(pinInfra); /// Lê o valor do sensor de fogo

  if (infra == LOW){ /// Se identificar fogo
    digitalWrite(pinLed, HIGH); /// Acende o LED
    delay(1000);                /// Espera 1 segundo
    digitalWrite(pinLed, LOW);  /// Apaga o LED
    Serial.println("Fogo Detectado.");
  }
  delay(500);

  int packetSize = udp.parsePacket(); /// Verifica se há pacotes UDP recebidos.
  
  if (packetSize){ /// Se houver pacotes recebidos:
    Serial.print(" Received packet from : ");
    Serial.println(udp.remoteIP());        /// Informa o IP de onde foram recebidos,
    int len = udp.read(packetBuffer, 255); /// Lê os dados
    Serial.printf("Data : %s\n", packetBuffer); /// E os imprime.
    Serial.println();

    if (packetBuffer[0] == 'F'){ /// Se o pacote UDP recebido indicar fogo em algum ESP do servidor
      Serial.println("PEGOU FOGO"); /// Indica que há fogo em algum lugar
      digitalWrite(pinLed, HIGH);   /// Acende o LED
      delay(1000);                  /// Espera 1 segundo
      digitalWrite(pinLed, LOW);    /// Apaga o LED
    }
  }

  udp.beginPacket(serverip, serverport); /// Inicia um pacote UDP para o servidor.
  char buf[30];                          /// Cria um buffer para a mensagem.

  unsigned long testeID = millis();           /// Obtém o tempo em ms desde que o Esp32 foi ligado.
  sprintf(buf, "Sensor do fogo: %lu", infra); /// Formata a mensagem com o valor do fogo.

  udp.printf(buf); /// Envia a mensagem via UDP.
  udp.endPacket(); /// Finaliza o pacote UDP.
}