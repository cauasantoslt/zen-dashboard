#include <WiFi.h>
#include <WebSocketsClient.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// --- FONTE NOVA HD ---
#include <Fonts/FreeSansBold9pt7b.h> 

// --- 1. Configuração dos Pinos ---
#define LDRPIN 34
#define PIRPIN 27
#define DHTPIN 26
#define OBSPIN 25
#define DHTTYPE DHT11   

// --- CONFIGURAÇÃO DO PROTETOR DE TELA ---
// Tempo em milissegundos para desligar a tela (5 minutos = 300000)
// Para testar rápido, mude para 10000 (10 segundos)
#define TEMPO_PROTETOR  150000 

// --- OLED CONFIG ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DHTPIN, DHTTYPE);

// --- 2. IR e Rede ---
const uint16_t kIrLed = 4;  
IRsend irsend(kIrLed);     

// === CÓDIGOS DE COR ===
#define IR_ON         0xF7C03F
#define IR_BRILHO_UP  0xF700FF 
#define IR_ARRAKIS    0xF710EF 
#define IR_ATREIDES   0xF7B04F 
#define IR_SIETCH     0xF750AF 

const char* ssid = "NET-TOP-CC-";
const char* password = "01152024";
const char* websocket_server = "zen-dashboard-97j0.onrender.com";
const int websocket_port = 443;
const char* websocket_path = "/";
const char* spotify_pass = "duna1234"; 

WebSocketsClient webSocket;
bool isConnected = false;

unsigned long lastSendTime = 0;
String lastMode = "System"; 

// --- Variáveis de Gesto e Estado ---
bool maoDetectada = false;
unsigned long inicioGesto = 0;
bool acaoExecutada = false; 
bool spotifyTocando = false; 

// --- Variáveis do Protetor de Tela ---
unsigned long ultimaAtividade = 0; // Guarda a hora do último movimento
bool telaLigada = true;

// Variáveis globais
float dispTemp = 0;
int dispLuz = 0;

// =================================================================
// --- GALERIA DUNA MINI (32x32 Pixels) ---
// =================================================================
const unsigned char bitmap_worm [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x3f, 0xfc, 0x00, 
  0x00, 0xfc, 0x3f, 0x00, 0x01, 0xe0, 0x07, 0x80, 0x03, 0x80, 0x01, 0xc0, 
  0x07, 0x00, 0x00, 0xe0, 0x0e, 0x03, 0xc0, 0x70, 0x0c, 0x0f, 0xf0, 0x30, 
  0x18, 0x1c, 0x38, 0x18, 0x18, 0x30, 0x0c, 0x18, 0x30, 0x60, 0x06, 0x0c, 
  0x30, 0xc0, 0x03, 0x0c, 0x31, 0x81, 0x81, 0x8c, 0x33, 0x07, 0xe0, 0xcc, 
  0x36, 0x1c, 0x38, 0x6c, 0x34, 0x30, 0x0c, 0x2c, 0x3c, 0x60, 0x06, 0x3c, 
  0x1c, 0xc0, 0x03, 0x38, 0x18, 0x80, 0x01, 0x18, 0x18, 0x80, 0x01, 0x18, 
  0x09, 0x80, 0x01, 0x90, 0x01, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 0xc0, 
  0x03, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x06, 0x00, 0x00, 0x60, 
  0x0e, 0x00, 0x00, 0x70, 0x0c, 0x00, 0x00, 0x30, 0x1c, 0x00, 0x00, 0x38, 
  0x18, 0x00, 0x00, 0x18, 0x38, 0x00, 0x00, 0x1c, 0x30, 0x00, 0x00, 0x0c
};

const unsigned char bitmap_hawk [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0xff, 0xff, 0x00, 
  0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xe0, 
  0x0f, 0xff, 0xff, 0xf0, 0x1f, 0xe7, 0xe7, 0xf8, 0x3f, 0x83, 0xc1, 0xfc, 
  0x3e, 0x01, 0x80, 0x7c, 0x3c, 0x00, 0x00, 0x3c, 0x18, 0x00, 0x00, 0x18, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x01, 0xff, 0xff, 0x80, 
  0x03, 0xf1, 0x8f, 0xc0, 0x07, 0xc0, 0x03, 0xe0, 0x07, 0x80, 0x01, 0xe0, 
  0x07, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0xc0, 
  0x01, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 
  0x00, 0xc0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// GOTA
const unsigned char bitmap_knife [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 
  0x00, 0x07, 0xe0, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x1f, 0xf8, 0x00, 
  0x00, 0x1f, 0xf8, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
  0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0x80, 
  0x01, 0xff, 0xff, 0x80, 0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 
  0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 
  0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 
  0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 
  0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xe0, 
  0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0x80, 
  0x00, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 
  0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void showFeedback(String tipo) {
  // Se a tela estiver desligada, acorda ela primeiro
  telaLigada = true;
  
  display.clearDisplay();
  display.setFont(NULL); 
  display.setTextColor(SSD1306_WHITE);
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  
  display.setTextSize(2);
  if (tipo == "NEXT") {
    display.setCursor(45, 25);
    display.print(">>>");
  } else if (tipo == "PREV") {
    display.setCursor(45, 25);
    display.print("<<<");
  } else if (tipo == "PLAY") {
    display.setCursor(55, 25);
    display.print(">"); 
  } else if (tipo == "PAUSE") {
    display.setCursor(50, 25);
    display.print("||"); 
  }
  display.display();
}

void updateDisplay(String modo) {
  // Só atualiza se a tela deve estar ligada
  if (!telaLigada) {
    display.clearDisplay();
    display.display();
    return;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  // 1. NOME DA CASA (Esquerda + Fonte HD)
  display.setFont(&FreeSansBold9pt7b); 
  display.setTextSize(1); 
  display.setCursor(0, 28); 
  display.println(modo);

  // 2. Volta para fonte normal
  display.setFont(NULL); 
  
  // Linha
  display.drawLine(0, 36, 85, 36, SSD1306_WHITE);

  // 3. Sensores
  display.setTextSize(1);
  display.setCursor(0, 42);
  display.print("T: " + String(dispTemp, 0) + "C"); 
  
  display.setCursor(0, 54);
  display.print("L: " + String(dispLuz));         

  // 4. ÍCONE (Direita)
  if (modo == "Arrakis") display.drawBitmap(90, 16, bitmap_worm, 32, 32, SSD1306_WHITE);
  else if (modo == "Atreides") display.drawBitmap(90, 16, bitmap_hawk, 32, 32, SSD1306_WHITE);
  else display.drawBitmap(90, 16, bitmap_knife, 32, 32, SSD1306_WHITE);

  // WiFi
  if (isConnected) display.fillCircle(124, 4, 2, SSD1306_WHITE);

  display.display();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: isConnected = false; break;
    case WStype_CONNECTED: isConnected = true; break;
    case WStype_TEXT: 
      String text = (char*)payload;
      if (text.indexOf("\"sender\":\"Spotify\"") > 0) {
          if (text.indexOf("\"is_playing\":true") > 0) spotifyTocando = true;
          else if (text.indexOf("\"is_playing\":false") > 0) spotifyTocando = false;
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Falha no OLED"));
    for(;;);
  }
  display.clearDisplay();
  display.setFont(NULL); 
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("DUNE SYSTEM v4.0");
  display.println("GOLD EDITION");
  display.println("----------------");
  display.println("Booting...");
  display.display();

  pinMode(LDRPIN, INPUT);
  pinMode(PIRPIN, INPUT);
  pinMode(OBSPIN, INPUT); 
  dht.begin();       
  irsend.begin();    
  
  // Inicializa o timer do protetor de tela
  ultimaAtividade = millis();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(200);
    display.print(".");
    display.display();
  }
  display.println("\nONLINE.");
  display.display();
  delay(1000);

  webSocket.beginSSL(websocket_server, websocket_port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void comandoSpotify(String cmd) {
  if (isConnected) {
      String json = "{";
      json += "\"spotify_command\": \"" + cmd + "\",";
      json += "\"password\": \"" + String(spotify_pass) + "\"";
      json += "}";
      webSocket.sendTXT(json);
  }
}

void dispararIR(uint32_t codigo) {
  for (int i = 0; i < 3; i++) {
    irsend.sendNEC(codigo, 32);
    delay(40); 
  }
}

void enviarSinalIR(String modo) {
  if (modo == lastMode) return;
  lastMode = modo;

  dispararIR(IR_ON);
  delay(100); 
  
  if (modo == "Atreides") {
    dispararIR(IR_ATREIDES); 
  } 
  else if (modo == "Arrakis") {
    dispararIR(IR_ARRAKIS); 
    delay(100);
    for(int i=0; i<5; i++){
       irsend.sendNEC(IR_BRILHO_UP, 32);
       delay(50);
    }
  } 
  else {
    dispararIR(IR_SIETCH); 
  }
}

void loop() {
  webSocket.loop();
  unsigned long now = millis();

  // --- 1. VERIFICA ATIVIDADE (PIR ou Gesto) ---
  bool sensorMusica = !digitalRead(OBSPIN); 
  int sensorMovimento = digitalRead(PIRPIN); // Lê o sensor de presença

  // Se tiver movimento ou estiver mexendo na música, reseta o timer
  if (sensorMusica || sensorMovimento == HIGH) {
    ultimaAtividade = now;
    if (!telaLigada) {
      telaLigada = true; // Acorda a tela imediatamente
      Serial.println("Movimento detectado: Tela Ligada!");
      updateDisplay(lastMode); // Atualiza a tela na hora
    }
  }

  // --- 2. VERIFICA TIMEOUT (Protetor de Tela) ---
  if (telaLigada && (now - ultimaAtividade > TEMPO_PROTETOR)) {
    telaLigada = false;
    Serial.println("Inatividade: Desligando tela...");
    updateDisplay(lastMode); // A função updateDisplay vai limpar a tela
  }

  // --- 3. LÓGICA DE GESTOS (MÚSICA) ---
  if (sensorMusica && !maoDetectada) {
    maoDetectada = true;
    inicioGesto = now;
    acaoExecutada = false;
  }

  if (!sensorMusica && maoDetectada) {
    maoDetectada = false;
    unsigned long duracao = now - inicioGesto;

    if (!acaoExecutada) {
      ultimaAtividade = now; // Garante que a tela não apague enquanto mexe na música
      if (duracao < 1000) { 
        showFeedback("NEXT");
        comandoSpotify("next");
        delay(1200); 
        updateDisplay(lastMode);
      } 
      else if (duracao >= 1000 && duracao < 3000) {
        if (spotifyTocando) {
            showFeedback("PAUSE");
            comandoSpotify("pause");
        } else {
            showFeedback("PLAY");
            comandoSpotify("play");
        }
        delay(1200);
        updateDisplay(lastMode);
      }
    }
  }

  if (sensorMusica && maoDetectada && !acaoExecutada) {
    if (now - inicioGesto >= 3000) {
      ultimaAtividade = now;
      showFeedback("PREV");
      comandoSpotify("prev");
      acaoExecutada = true;
      delay(1200);
      updateDisplay(lastMode);
    }
  }

  // --- 4. DADOS AMBIENTAIS (A cada 2 seg) ---
  if (now - lastSendTime > 2000) {
    lastSendTime = now;

    dispLuz = analogRead(LDRPIN);
    // Nota: Já lemos o PIR ali em cima, vamos usar a variável
    float hum = dht.readHumidity();
    dispTemp = dht.readTemperature();
    int obsStatus = sensorMusica ? 0 : 1; 

    if (isnan(hum) || isnan(dispTemp)) { hum = 0; dispTemp = 0; }

    String modo;
    if (dispLuz < 1500) modo = "Atreides"; 
    else if (dispLuz >= 1500 && dispLuz <= 3900) modo = "Arrakis"; 
    else modo = "Sietch";  

    // Atualiza a tela (Se ela estiver em modo ligado)
    updateDisplay(modo);
    
    enviarSinalIR(modo);

    if (isConnected) {
        String json = "{";
        json += "\"luz\":" + String(dispLuz) + ",";
        json += "\"temp\":" + String(dispTemp) + ",";
        json += "\"hum\":" + String(hum) + ",";
        json += "\"pir\":" + String(sensorMovimento) + ",";
        json += "\"obs\":" + String(obsStatus);
        json += "}";
        webSocket.sendTXT(json);
    }
  }
}