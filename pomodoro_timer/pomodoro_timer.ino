// ------------------------------
// Projeto pomodoro físico com Arduino UNO 
// Desenvolvido por: Ijair Delani
// Componentes utilizados:
// - Display 7 segmentos 4 dígitos (cátodo comum)
// - LED verde e vermelho
// - Dois botões
// - Um buzzer
// ------------------------------

#define LED_FOCO A0
#define LED_PAUSA A1
#define BUZZER A4

#define BOTAO_INICIAR A2
#define BOTAO_PAUSAR A3


int segmentos[8] = {2, 3, 4, 5, 6, 7, 8, 9};

int digitos[4] = {10, 11, 12, 13};

// Dígitos de 0 a 9
byte numeros[10][8] = {
  {1,1,1,1,1,1,0,0},
  {0,1,1,0,0,0,0,0},
  {1,1,0,1,1,0,1,0},
  {1,1,1,1,0,0,1,0},
  {0,1,1,0,0,1,1,0},
  {1,0,1,1,0,1,1,0},
  {1,0,1,1,1,1,1,0},
  {1,1,1,0,0,0,0,0},
  {1,1,1,1,1,1,1,0},
  {1,1,1,1,0,1,1,0}
};

bool rodando = false;
bool pausado = false;
bool modoFoco = true;

unsigned long tempoInicio;
unsigned long tempoPausa = 0;
int duracao = 25 * 60; // 25 minutos

// Função para mostrar número no display
void mostrarNumero(int num) {
  int dig[4];
  dig[0] = (num / 1000) % 10;
  dig[1] = (num / 100) % 10;
  dig[2] = (num / 10) % 10;
  dig[3] = num % 10;

  for (int i = 0; i < 4; i++) {
    digitalWrite(digitos[i], LOW); // Ativa o dígito
    for (int s = 0; s < 8; s++)
      digitalWrite(segmentos[s], numeros[dig[i]][s]);
    delay(3);
    digitalWrite(digitos[i], HIGH); // Desativa o dígito
  }
}

void setup() {
  for (int i = 0; i < 8; i++) pinMode(segmentos[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digitos[i], OUTPUT);

  pinMode(LED_FOCO, OUTPUT);
  pinMode(LED_PAUSA, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(BOTAO_INICIAR, INPUT_PULLUP);
  pinMode(BOTAO_PAUSAR, INPUT_PULLUP);

  digitalWrite(LED_FOCO, HIGH);
  digitalWrite(LED_PAUSA, LOW);

  mostrarNumero(0);
}

void loop() {
  if (!digitalRead(BOTAO_INICIAR)) {
    rodando = true;
    pausado = false;
    tempoInicio = millis() - tempoPausa;
    tempoPausa = 0;
    delay(300);
  }

  if (!digitalRead(BOTAO_PAUSAR) && rodando) {
    pausado = !pausado;
    if (pausado)
      tempoPausa = millis() - tempoInicio;
    else
      tempoInicio = millis() - tempoPausa;
    delay(300);
  }

  if (rodando && !pausado) {
    unsigned long tempoDecorrido = (millis() - tempoInicio) / 1000;
    int tempoRestante = duracao - tempoDecorrido;

    if (tempoRestante <= 0) {
      rodando = false;
      tone(BUZZER, 2000, 1000); // Som de 1 segundo
      modoFoco = !modoFoco;

      if (modoFoco) {
        duracao = 25 * 60;
        digitalWrite(LED_FOCO, HIGH);
        digitalWrite(LED_PAUSA, LOW);
      } else {
        duracao = 5 * 60;
        digitalWrite(LED_FOCO, LOW);
        digitalWrite(LED_PAUSA, HIGH);
      }

      delay(1000);
    } else {
      int minutos = tempoRestante / 60;
      int segundos = tempoRestante % 60;
      int valor = minutos * 100 + segundos;
      mostrarNumero(valor);
    }
  } else if (!rodando) {
    if (modoFoco)
      mostrarNumero(2500);
    else
      mostrarNumero(500);
  } else {
    // Mostra tempo congelado
    unsigned long tempoDecorrido = tempoPausa / 1000;
    int tempoRestante = duracao - tempoDecorrido;
    int minutos = tempoRestante / 60;
    int segundos = tempoRestante % 60;
    int valor = minutos * 100 + segundos;
    mostrarNumero(valor);
  }
}
