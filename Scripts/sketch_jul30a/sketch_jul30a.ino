// Bibliotecas utilizadas:
#include <WiFi.h>    ///Permite que o ESP32 se conecte a redes WiFi.
#include <WiFiUdp.h> ///Permite a comunicação UDP(User Datagram Protocol).

#define pinInfra 25 /// Define o pino 25 como pinInfra, o sensor de infravermelho está conectado.
#define pinGas 32   /// Define o pino 32 como pinGas, o sensor de gás está conectado.
#define pinLed 33   /// Define o pino 33 como pinLed, o led de alerta está conectado.

#define lGas 500 /// Define o limite de gás como 500

// Declarações de Funções:
void disparaSirene(byte pin, int intervalo);
void desligaSirene(byte pin);

// Declarações e Definições:
WiFiUDP udp; /// Cria um objeto udp para a comunicação UDP.

char packetBuffer[255];           /// Cria um buffer para armazenar dados recebidos via UDP
unsigned int localPort = 9999;    /// Define a porta local para comunicação UDP
char *serverip = "192.168.176.3"; /// Define o endereço IP do servidor para enviar dados.
unsigned int serverport = 8888;   /// Define a porta do servidor para enviar dados.

const char *ssid = "Redmi 12";         /// Define o SSID da rede WiFi.
const char *password = "thales5g.123"; /// Define a senha da rede WiFi.

// Função setup:
void setup()
{
  Serial.begin(115200); /// Inicializa a comunicação serial a 115200 bps.

  pinMode(pinInfra, INPUT); /// Configura o pino do sensor de infravermelho como entrada.
  pinMode(pinGas, INPUT);   /// Configura o pino do sensor de gás como entrada.
  pinMode(pinLed, OUTPUT);  /// Configura o pino do LED como saida.

  delay(500);

  // Connect to Wifi network.
  WiFi.begin(ssid, password); /// Conecta o Esp32 à rede WiFi especificada.
  while (WiFi.status() != WL_CONNECTED)
  {                       /// Aguarda até que a comunicação WiFi seja estabelecida
    delay(500);           ////A cada 500ms
    Serial.print(F(".")); ////Imprime um ponto
  }

  udp.begin(localPort);                                                                 /// Inicia a comunicação UDP na porta local definida
  Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort); /// Imprime o endereço IP local e a porta UDP

  Serial.println("Fim do setup!");
}

// Função loop:
void loop()
{
  bool infra = digitalRead(pinInfra); /// Lê o valor do sensor de fogo
  int nivelGas = analogRead(pinGas);  /// Lê o valor do sensor de gás

  if (infra == LOW || nivelGas >= lGas)
  {                             /// Se identificar fogo
    disparaSirene(pinLed, 500); //// Dispara a sirene por meio segundo
  }
  else
  {                        /// Se não
    desligaSirene(pinLed); //// Desliga a sirene
  }

  // Imprime os valores dos sensores:
  Serial.println(infra);
  Serial.println(nivelGas);

  int packetSize = udp.parsePacket(); /// Verifica se há pacotes UDP recebidos.

  if (packetSize)
  { /// Se houver pacotes recebidos:
    Serial.print(" Received packet from : ");
    Serial.println(udp.remoteIP()); /// Informa o IP de onde foram recebidos,

    int len = udp.read(packetBuffer, 255);      /// Lê os dados
    Serial.printf("Data : %s\n", packetBuffer); /// E os imprime.
    Serial.println();

    if (packetBuffer[0] == 'F')
    {                               /// Se o pacote UDP recebido indicar fogo em algum ESP do servidor
      Serial.println("PEGOU FOGO"); /// Indica que há fogo em algum lugar
      disparaSirene(pinLed, 1000);  /// Dispara sirene por 1 segundo
    }
  }

  udp.beginPacket(serverip, serverport); /// Inicia um pacote UDP para o servidor.
  char buf[30];                          /// Cria um buffer para a mensagem.
  char buf2[30];                         /// Cria dois buffers para a mensagem.

  sprintf(buf, "Sensor do fogo: %lu ", infra); /// Formata a mensagem com o valor do fogo.
  delay(200);
  sprintf(buf2, "Sensor de Gas: %lu ", nivelGas); /// Formata a mensagem com o valor do gás.

  udp.printf(buf);  /// Envia a mensagem via UDP.
  udp.printf(buf2); /// Envia a segunda mensagem via UDP.
  udp.endPacket();  /// Finaliza o pacote UDP.

  delay(500);
}

// Declaração das funções:
void disparaSirene(byte pin, int intervalo)
{
  digitalWrite(pin, HIGH);

  delay(intervalo);

  digitalWrite(pin, LOW);

  delay(intervalo);
}

void desligaSirene(byte pin)
{
  digitalWrite(pin, LOW);
}