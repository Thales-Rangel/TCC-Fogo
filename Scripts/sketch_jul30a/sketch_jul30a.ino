// Bibliotecas utilizadas:
#include <WiFi.h>    ///Permite que o ESP32 se conecte a redes WiFi.
#include <WiFiUdp.h> ///Permite a comunicação UDP(User Datagram Protocol).

#define pinInfra 25
#define pinLed 33
#define pinGas 32

int lGas = 1000;

/// Define o pino 25 como pinGas, o sensor de gás está conectado.

// Declarações e Definições:
WiFiUDP udp; /// Cria um objeto udp para a comunicação UDP.

char packetBuffer[255];         /// Cria um buffer para armazenar dados recebidos via UDP
unsigned int localPort = 9999;  /// Define a porta local para comunicação UDP
char *serverip = "10.108.1.200"; /// Define o endereço IP do servidor para enviar dados.
unsigned int serverport = 8888; /// Define a porta do servidor para enviar dados.

const char *ssid = "Redmi 12";         /// Define o SSID da rede WiFi.
const char *password = "thales5g.123"; /// Define a senha da rede WiFi.


bool infra;
int nivelGas;
int cont = 30;

IPAddress local_ip(10,108,1,201);
IPAddress gateway(10,108,0,1);
IPAddress subnet(255,255,0,0);

// Função setup:
void setup()
{
  Serial.begin(115200); /// Inicializa a comunicação serial a 115200 bps.

  pinMode(pinInfra, INPUT); /// Configura o pino do sensor de gás como entrada.

  pinMode(pinLed, OUTPUT);

  pinMode(pinGas, INPUT);

  WiFi.mode(WIFI_STA);

  delay(500);
  // Connect to Wifi network.

  if(!WiFi.config(local_ip, gateway, subnet)){
  Serial.println("Falha ao configurar o IP fixo.");
} else {
  Serial.println("IP fixo aplicado com êxito.");
}

  WiFi.begin(ssid, password); /// Conecta o Esp32 à rede WiFi especificada.
  while (WiFi.status() != WL_CONNECTED)
  {                       /// Aguarda até que a comunicação WiFi seja estabelecida
    delay(500);           ////A cada 500ms
    Serial.print(F(".")); ////Imprime um ponto
  }
  udp.begin(localPort); /// Inicia a comunicação UDP na porta local definida

  Serial.println(" ");
  Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort); /// Imprime o endereço IP local e a porta UDP

  Serial.print("Endereço MAC do Esp32: ");
  Serial.println(WiFi.macAddress());

  Serial.print("Endereço IP do Esp32:");
  Serial.println(WiFi.localIP());

  String mac = WiFi.macAddress();
  udp.beginPacket(serverip, serverport); /// Inicia um pacote UDP para o servidor.
  udp.print("MAC: ");
  udp.println(mac);
  udp.endPacket();

  Serial.println("Fim do setup!");
}

// Função loop:
void loop()
{

  infra = digitalRead(pinInfra);
  nivelGas = analogRead(pinGas);
  int an = lGas / 2;
  if (nivelGas >= lGas)
  {
    disparaSirene(pinLed, 1000);
    alertaFogo();
  }
  else if (infra == LOW)
  {
    disparaSirene(pinLed, 1000);
    alertaFogo();
  }
  else
  {
    desligaSirene(pinLed);
  }

  Serial.println(infra);
  Serial.println(nivelGas);

  int packetSize = udp.parsePacket(); /// Verifica se há pacotes UDP recebidos.
  if (packetSize)
  { /// Se houver pacotes recebidos:
    Serial.print(" Received packet from : ");
    Serial.println(udp.remoteIP());        /// Informa o IP de onde foram recebidos,
    int len = udp.read(packetBuffer, 255); /// Lê os dados

    Serial.printf("Data : %s\n", packetBuffer); /// E os imprime.
    Serial.println();

    if (packetBuffer[0] == 'F')
    {                               // Se o pacote UDP recebido indicar fogo em algum ESP do servidor
      Serial.println("PEGOU FOGO"); // Indica que há fogo em algum lugar
      digitalWrite(pinLed, HIGH);   // Acende o LED
      delay(1000);                  // Espera 1 segundo
      digitalWrite(pinLed, LOW);    // Apaga o LED
    }
    else if (packetBuffer[0] == 'G')
    {
      Serial.println("Vazamento detectado");
      digitalWrite(pinLed, HIGH); // Acende o LED
      delay(1000);                // Espera 1 segundo
      digitalWrite(pinLed, LOW);  // Apaga o LED
    }

    if (strncmp(packetBuffer, "V=", 2) == 0)
    {
      lGas = atoi(packetBuffer + 2);
      Serial.print("Valor limite de gás alterado para: ");
      Serial.println(lGas);
    }
  }

  udp.beginPacket(serverip, serverport); /// Inicia um pacote UDP para o servidor.
  char buf[30];                          /// Cria um buffer para a mensagem.
  char buf1[30];

  unsigned long testeID = millis(); /// Obtém o tempo em ms desde que o Esp32 foi ligado.

  if (cont == 30)
  {
    char bufE[30];

    if (nivelGas < an)
    {
      sprintf(bufE, "AOk");
    }
    else if (nivelGas >= an & nivelGas < lGas)
    {
      sprintf(bufE, "Anormal");
    }

    sprintf(buf, "Sensor do fogo: %lu", infra); /// Formata a mensagem com o valor do fogo.
    delay(200);
    sprintf(buf1, " Sensor de Gas: %lu ", nivelGas);

    udp.printf(buf); /// Envia a mensagem via UDP.
    udp.printf(buf1);
    udp.printf(bufE);
    udp.endPacket(); /// Finaliza o pacote UDP.
    delay(500);
    cont = 0;
  }
  cont++;
  Serial.print("temporizador: ");
  Serial.println(cont);
  delay(1000);
}

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

void alertaFogo()
{
  while (infra != HIGH || nivelGas > lGas)
  {
    Serial.println("Fogo detectado!");
    udp.beginPacket(serverip, serverport);
    char bufA[30];
    char bufB[30];
    char bufC[30];
    sprintf(bufA, "Sensor do Fogo: %lu ", infra);
    sprintf(bufB, "Sensor do Gas: %lu ", nivelGas);
    sprintf(bufC, "Alerta!");
    disparaSirene(pinLed, 500);
    udp.printf(bufA);
    udp.printf(bufB);
    udp.printf(bufC);
    udp.endPacket();
    delay(500);
    infra = digitalRead(pinInfra);
    nivelGas = analogRead(pinGas);
  }
  desligaSirene(pinLed);
  cont = 0;
}
