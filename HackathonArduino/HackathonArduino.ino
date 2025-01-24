// Pinos do Arduino
// Pinos de controle
const int Save = 7;
const int Play = 27;
const int Hold = 24;
const int Delete = 30;
const int Reset = 53;

// Pinos dos instrumentos musicais
const int Bateria = 2;
const int GuitarraBase = 14;
const int GuitarraSolo = 23;
const int Teclado = 36;
const int Sax = 51;

// Pinos dos potenciômetros responsáveis por controlar a velocidade dos instrumentos
const int PinoVelocidadeBateria = A0;
const int PinoVelocidadeGuitarraBase = A1;
const int PinoVelocidadeGuitarraSolo = A2;
const int PinoVelocidadeTeclado = A3;
const int PinoVelocidadeSax = A4;

// Pinos dos LEDs dos instrumentos
const int LedsBateria[] = {6, 5, 4, 3};
const int LedsGuitarraBase[] = {37, 15, 16, 19};
const int LedsGuitarraSolo[] = {20, 23, 21, 22};
const int LedsTeclado[] = {32, 33, 35, 34};
const int LedsSax[] = {45, 47, 49, 43};

// Variáveis padrão do código
bool FimDaMusica = false;
String VelocidadeBateria, VelocidadeGuitarraBase, VelocidadeGuitarraSolo, VelocidadeTeclado, VelocidadeSax;
int ModoBateria = 0, ModoGuitarraBase = 0, ModoGuitarraSolo = 0, ModoTeclado = 0, ModoSax = 0;
String MensagemBateria,MensagemGuitarraBase,MensagemGuitarraSolo,MensagemTeclado,MensagemSax;
const int debounceDelay = 250;

// Função para calcular velocidade com base na leitura do potenciômetro
String CalcularVelocidade(int pino) {
  int leitura = analogRead(pino);
  if (leitura < 172) return "05";
  if (leitura < 342) return "10";
  if (leitura < 512) return "15";
  if (leitura < 682) return "20";
  if (leitura < 853) return "25";
  return "30";
}

// Função para atualizar o modo de um instrumento
void AtualizarModo(int &modo, int pino) {
  if (digitalRead(pino) == HIGH) {
    modo = (modo < 4) ? (modo + 1) : 0;
    delay(debounceDelay);
  }
}

// Função para gerar mensagem de um instrumento
String GerarMensagem(String prefixo, int modo, String velocidade) {
  return (modo != 0) ? (prefixo + String(modo) + velocidade) : "";
}

// Função para configurar LEDs de um instrumento com base no modo
void ConfigurarLeds(const int leds[], int modo) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leds[i], (i == (modo - 1)) ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);

  // Configurar pinos de controle e instrumentos como entrada
  pinMode(Save, INPUT_PULLUP);
  pinMode(Play, INPUT_PULLUP);
  pinMode(Hold, INPUT_PULLUP);
  pinMode(Delete, INPUT_PULLUP);
  pinMode(Reset, INPUT_PULLUP);

  pinMode(Bateria, INPUT_PULLUP);
  pinMode(GuitarraBase, INPUT_PULLUP);
  pinMode(GuitarraSolo, INPUT_PULLUP);
  pinMode(Teclado, INPUT_PULLUP);
  pinMode(Sax, INPUT_PULLUP);

  // Configurar pinos dos LEDs como saída
  for (int i = 0; i < 4; i++) {
    pinMode(LedsBateria[i], OUTPUT);
    pinMode(LedsGuitarraBase[i], OUTPUT);
    pinMode(LedsGuitarraSolo[i], OUTPUT);
    pinMode(LedsTeclado[i], OUTPUT);
    pinMode(LedsSax[i], OUTPUT);
  }
}

void loop() {
  if (digitalRead(Save) == HIGH || digitalRead(Reset) == HIGH) {
    FimDaMusica = true;
    Serial.println(FimDaMusica ? "s" : "r");
  }

  if (!FimDaMusica) {
    while (digitalRead(Hold) == HIGH) {
      // Calcular velocidades
      VelocidadeBateria = CalcularVelocidade(PinoVelocidadeBateria);
      VelocidadeGuitarraBase = CalcularVelocidade(PinoVelocidadeGuitarraBase);
      VelocidadeGuitarraSolo = CalcularVelocidade(PinoVelocidadeGuitarraSolo);
      VelocidadeTeclado = CalcularVelocidade(PinoVelocidadeTeclado);
      VelocidadeSax = CalcularVelocidade(PinoVelocidadeSax);

      // Atualizar modos
      AtualizarModo(ModoBateria, Bateria);
      AtualizarModo(ModoGuitarraBase, GuitarraBase);
      AtualizarModo(ModoGuitarraSolo, GuitarraSolo);
      AtualizarModo(ModoTeclado, Teclado);
      AtualizarModo(ModoSax, Sax);

      // Configurar LEDs para cada instrumento
      ConfigurarLeds(LedsBateria, ModoBateria);
      ConfigurarLeds(LedsGuitarraBase, ModoGuitarraBase);
      ConfigurarLeds(LedsGuitarraSolo, ModoGuitarraSolo);
      ConfigurarLeds(LedsTeclado, ModoTeclado);
      ConfigurarLeds(LedsSax, ModoSax);
    }

    // Gerar mensagens individuais
    MensagemBateria = GerarMensagem("b", ModoBateria, VelocidadeBateria);
    MensagemGuitarraBase = GerarMensagem("g", ModoGuitarraBase, VelocidadeGuitarraBase); // Alteração de "gb" para "g"
    MensagemGuitarraSolo = GerarMensagem("h", ModoGuitarraSolo, VelocidadeGuitarraSolo); // Alteração de "gs" para "h"
    MensagemTeclado = GerarMensagem("t", ModoTeclado, VelocidadeTeclado);
    MensagemSax = GerarMensagem("s", ModoSax, VelocidadeSax);

    // Construir mensagem completa
    String MensagemCompleta = "";
    if (MensagemBateria != "") MensagemCompleta += MensagemBateria + "/";
    if (MensagemGuitarraBase != "") MensagemCompleta += MensagemGuitarraBase + "/";
    if (MensagemGuitarraSolo != "") MensagemCompleta += MensagemGuitarraSolo + "/";
    if (MensagemTeclado != "") MensagemCompleta += MensagemTeclado + "/";
    if (MensagemSax != "") MensagemCompleta += MensagemSax + "/";

    // Remover o último "/" se existir
    if (MensagemCompleta.endsWith("/")) {
      MensagemCompleta = MensagemCompleta.substring(0, MensagemCompleta.length() - 1);
    }

    // Enviar mensagem pela serial
    Serial.println(MensagemCompleta);
  }
}
