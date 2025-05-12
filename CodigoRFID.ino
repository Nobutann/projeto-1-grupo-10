#include <SPI.h>             // Biblioteca SPI para comunicação com o leitor RFID
#include <MFRC522.h>         // Biblioteca MFRC522 para controle do módulo RFID

#define SS_PIN 10            // Definindo o pino de seleção de chip (CS)
#define RST_PIN 9            // Definindo o pino de reset

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Instância do objeto MFRC522, configurando os pinos de CS e reset

// Definição dos pinos para LEDs e motor vibratório
#define LED_VERDE 6          // LED verde para indicar "par correto"
#define LED_VERMELHO 7       // LED vermelho para indicar "par incorreto"
#define MOTOR_PIN 5          // Pino para o motor vibratório

// Estrutura para armazenar pares de tags válidas
struct ParCartas {
  byte carta1[4];  // Armazena o UID da primeira carta do par
  byte carta2[4];  // Armazena o UID da segunda carta do par
};

// Array de pares de tags RFID válidos, onde cada par possui dois UIDs
ParCartas paresValidos[] = {
  { {0xC3, 0x45, 0x61, 0x1A}, {0xC3, 0x36, 0x74, 0x1A} },
  { {0x93, 0x2C, 0x77, 0x1A}, {0xA3, 0x96, 0x63, 0x1A} },
  // Adicione mais pares conforme necessário
};

const int numPares = sizeof(paresValidos) / sizeof(paresValidos[0]); // Conta o número total de pares válidos

byte primeiraLida[4];       // Array para armazenar o UID da primeira carta lida
bool primeiraCartaLida = false; // Flag para verificar se a primeira carta foi lida

void setup() {
  Serial.begin(9600);        // Inicia comunicação serial para debug
  SPI.begin();               // Inicia comunicação SPI para o leitor RFID
  mfrc522.PCD_Init();        // Inicializa o leitor RFID

  pinMode(LED_VERDE, OUTPUT);    // Define o pino do LED verde como saída
  pinMode(LED_VERMELHO, OUTPUT); // Define o pino do LED vermelho como saída
  pinMode(MOTOR_PIN, OUTPUT);    // Define o pino do motor vibratório como saída

  Serial.println("Sistema iniciado. Aproxime as cartas RFID.");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  byte uid[4];
  for (byte i = 0; i < 4; i++) uid[i] = mfrc522.uid.uidByte[i];

  Serial.print("UID lido: ");
  for (byte i = 0; i < 4; i++) {
    Serial.print("0x"); Serial.print(uid[i], HEX); Serial.print(" ");
  }
  Serial.println();

  if (!primeiraCartaLida) {
    memcpy(primeiraLida, uid, 4);
    primeiraCartaLida = true;
    Serial.println("Primeira carta registrada.");
  } else {
    bool corresponde = false;

    for (int i = 0; i < numPares; i++) {
      if ((compareUID(primeiraLida, paresValidos[i].carta1) && compareUID(uid, paresValidos[i].carta2)) ||
          (compareUID(primeiraLida, paresValidos[i].carta2) && compareUID(uid, paresValidos[i].carta1))) {
        corresponde = true;
        break;
      }
    }

    if (corresponde) {
      Serial.println("Par correto!");
      piscarComVibracao(LED_VERDE);
    } else {
      Serial.println("Par incorreto!");
      piscarComVibracao(LED_VERMELHO);
    }

    primeiraCartaLida = false;
  }

  mfrc522.PICC_HaltA();
  delay(500);
}

// Função para comparar dois UIDs
bool compareUID(byte *a, byte *b) {
  for (byte i = 0; i < 4; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

// Função para vibrar brevemente
void vibrar() {
  digitalWrite(MOTOR_PIN, HIGH);
  delay(300);
  digitalWrite(MOTOR_PIN, LOW);
}

// Função para piscar LED 5 vezes com vibração sincronizada
void piscarComVibracao(int pinoLED) {
  int count = 0;
  while (count < 5) {
    digitalWrite(pinoLED, HIGH);
    vibrar();               // Vibra junto com o acendimento do LED
    delay(200);             // LED aceso por 0.5s
    digitalWrite(pinoLED, LOW);
    delay(200);             // LED apagado por 0.5s
    count++;
  }
}
