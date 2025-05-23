#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

#define LED_VERDE 6
#define LED_VERMELHO 7
#define MOTOR_PIN 5

struct ParCartas {
  byte carta1[4];
  byte carta2[4];
};

ParCartas paresValidos[] = {
  { {0xC3, 0x45, 0x61, 0x1A}, {0xC3, 0x36, 0x74, 0x1A} },
  { {0x93, 0x2C, 0x77, 0x1A}, {0xA3, 0x96, 0x63, 0x1A} },
  { {0x83, 0x2B, 0xB5, 0x29}, {0xE3, 0x66, 0x69, 0x29} },
  { {0x53, 0xB7, 0x8, 0x2A}, {0xF3, 0x8D, 0xED, 0x27} },
  { {0x43, 0xF0, 0x9, 0x2A}, {0x23, 0x2, 0xC7, 0x27} },
  { {0x3, 0x67, 0x6F, 0x1A}, {0x94, 0x39, 0xAB, 0x1E} },
  { {0x03, 0x71, 0x72, 0x2A}, {0x23, 0x86, 0x75, 0x1A} },
  { {0x93, 0xF3, 0xF, 0x2A}, {0xBA, 0xB, 0xDB, 0x16} },
  { {0xA3, 0xCC, 0x86, 0x29}, {0xF3, 0x67, 0x43, 0x29} },
  { {0xA3, 0xE9, 0x36, 0x28}, {0xF3, 0xC8, 0x86, 0x29} },
  { {0x53, 0x42, 0x66, 0x1A}, {0xE3, 0x3A, 0x74, 0x1A} },
  { {0x83, 0xCB, 0x36, 0x28}, {0xC3, 0x5A, 0xAE, 0x27} }
};

const int numPares = sizeof(paresValidos) / sizeof(paresValidos[0]);

byte primeiraLida[4];
bool primeiraCartaLida = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
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
      piscarComVibracaoCorreta(LED_VERDE);
    } else {
      Serial.println("Par incorreto!");
      piscarComVibracaoIncorreta(LED_VERMELHO);
    }

    primeiraCartaLida = false;
  }

  mfrc522.PICC_HaltA();
  delay(500);
}

bool compareUID(byte *a, byte *b) {
  for (byte i = 0; i < 4; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

void vibrarCurto() {
  digitalWrite(MOTOR_PIN, HIGH);
  delay(100);
  digitalWrite(MOTOR_PIN, LOW);
}

void vibrarLongo() {
  digitalWrite(MOTOR_PIN, HIGH);
  delay(400);
  digitalWrite(MOTOR_PIN, LOW);
}

void piscarComVibracaoCorreta(int pinoLED) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pinoLED, HIGH);
    vibrarLongo();
    delay(300);
    digitalWrite(pinoLED, LOW);
    delay(300);
  }
}

void piscarComVibracaoIncorreta(int pinoLED) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(pinoLED, HIGH);
    vibrarCurto();
    delay(150);
    digitalWrite(pinoLED, LOW);
    delay(150);
  }
}
