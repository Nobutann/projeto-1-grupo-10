# SINALIZE – Jogo Educativo para Deficientes Auditivos

O **SINALIZE** é um jogo de tabuleiro interativo desenvolvido para auxiliar no aprendizado de Libras (Língua Brasileira de Sinais). O jogador aproxima duas cartas RFID de um leitor: a primeira carta contém uma palavra em português, e a segunda deve ser sua tradução em Libras. O sistema confere se o par é válido e fornece um feedback visual e tátil.

---

##  Componentes Utilizados

- Arduino Uno
- Leitor RFID RC522
- Cartas RFID
- LED verde e LED vermelho
- 2 Motores vibratórios
- Jumpers e resistores
- Protoboard

---

## Como Funciona

1. O sistema aguarda a aproximação da **primeira carta** (com palavra em português).
2. Em seguida, o usuário deve aproximar a **segunda carta**, que representa o sinal em Libras correspondente.
3. Se o par estiver correto:
   - O LED **verde** pisca.
   - Os motores vibratórios é ativado brevemente.
4. Se o par estiver incorreto:
   - O LED **vermelho** pisca.
   - Os motores também vibram como alerta.

---

## Como Usar

### 1. Instale as Bibliotecas Necessárias

Abra a **IDE do Arduino**, vá em:

Sketch > Incluir biblioteca > Gerenciar Bibliotecas

Pesquise e instale:

- `MFRC522`

### 2. Conecte os Componentes

Conexões básicas do RC522 ao Arduino Uno:

| RC522 | Arduino |
|-------|---------|
| SDA   | 10      |
| SCK   | 13      |
| MOSI  | 11      |
| MISO  | 12      |
| GND   | GND     |
| RST   | 9       |
| 3.3V  | 3.3V    |

Outros pinos:

- LED Verde → Pino 6
- LED Vermelho → Pino 7
- Motor Vibratório → Pino 5
- Motor Vbratório 2 → Pino 3

### 3. Configure os Pares de Cartas

No código-fonte, edite os valores no array `paresValidos` com os UIDs reais das suas cartas:

```cpp
ParCartas paresValidos[] = {
  { {0xXX, 0xXX, 0xXX, 0xXX}, {0xYY, 0xYY, 0xYY, 0xYY} },
  // Adicione mais pares aqui
};
```
## Descobrindo os UIDs das suas tags RFID

1. Abra a IDE do Arduino.
2. Vá até **Arquivo > Exemplos > MFRC522 > ReadUID**.
3. Conecte o leitor RFID ao Arduino conforme o esquema de pinos.
4. Compile e envie o código:
   - Conecte seu Arduino ao PC via USB.
   - Selecione a placa e a porta correta na IDE.
   - Clique em **Carregar**.
5. Abra o **Monitor Serial**. Aproximando uma tag do leitor, o UID será exibido no terminal.
6. Anote os UIDs das suas tags para configurar os pares no seu projeto.

---

## Objetivo Educacional

O projeto **SINALIZE** busca incentivar o aprendizado da **Libras** (Língua Brasileira de Sinais) de forma **lúdica**, integrando **tecnologia** com **educação inclusiva**.

Ele pode ser utilizado em:
- Escolas de ensino fundamental
- Aulas de Libras
- Ambientes interativos voltados à inclusão
- Brincadeiras com crianças deficientes auditivos