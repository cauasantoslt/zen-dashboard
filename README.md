# Zen-Dashboard 🪐 Duna System (IoT Room Controller) 

### "He who controls the spice controls the universe."


## 🔗 Índice

- [📍 Visão Geral](#-visão-geral)
- [👾 Funcionalidades](#-funcionalidades)
- [📂 Índice do Projeto](#-índice-do-projeto)
- [🎗 Licença](#-licença)
- [🙌 Agradecimentos](#-agradecimentos)

---

## 📍 Visão Geral

Este projeto é a fusão de dois mundos: uma interface web moderna e um sistema físico de automação residencial.

1. **Zen-Dashboard:** Um painel web elegante e responsivo que integra controles do Spotify, chat com Inteligência Artificial (Gemini), e visualização de dados em tempo real via WebSocket.

2. **Duna System:** Um controlador IoT baseado no ESP32 inspirado no universo de Frank Herbert. Ele monitora o ambiente físico (temperatura, luz, movimento), controla a iluminação do quarto automaticamente e permite o controle de mídia por gestos "sem toque".

---

## 👾 Funcionalidades

|     |     Funcionalidade      | Resumo                                                                                                                                                                                                                                                                                                                                            |
| :-- | :---------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ⚙️  |     **Hardware**     | <ul><li> ***Monitoramento Ambiental:*** Leitura em tempo real de temperatura, umidade, luminosidade e presença.</li>***Iluminação Temática:*** Fitas de LED RGB mudam de cor e tema (Casas de Duna) baseadas na luz do ambiente.<li>***Controle Gestual (Force Control):*** Controle o Spotify (Play/Pause, Próxima, Anterior) passando a mão na frente de um sensor, sem tocar em nada.</li><li></li>***Display OLED Sci-Fi:*** Interface visual com arte em pixel (Bitmaps) e proteção de tela inteligente contra burn-in.</ul>   
| ⚙️  |     **Arquitetura**     | <ul><li>Comunicação em tempo real usando **WebSocket** para troca de dados e manipulação de eventos.</li><li>Suporte para processamento de dados de sensores, interações de chat com IA e comandos do Spotify em **index.js**.</li><li>Design de UI consistente com layouts responsivos em **style.css**.</li></ul>                               |
| 🔩  | **Qualidade do Código** | <ul><li>Dependências e versões definidas em **package-lock.json** para estabilidade.</li><li>Configuração e instalação adequadas de dependências em **package.json** para integração sem problemas.</li><li>Código modular e bem estruturado em **index.js** para manutenção.</li></ul>                                                           |
| 📄  |    **Documentação**     | <ul><li>Múltiplos tipos de arquivos com explicações detalhadas: **css**, **json**, **js**, **html**, **txt**.</li><li>Comandos claros de instalação e uso para **npm** na documentação.</li><li>Documentação completa das dependências e gerenciadores de pacotes utilizados no projeto.</li></ul>                                                |
| 🔌  |     **Integrações**     | <ul><li>Integração do **ngrok** para desenvolvimento e testes locais em **load.txt**.</li><li>Utilização de bibliotecas como **@google/generative-ai**, **dotenv** e **express** para funcionalidade aprimorada.</li><li>Atualizações em tempo real e interação com Spotify, sensores ambientais e mensagens de chat em **index.html**.</li></ul> |
| 🧩  |    **Modularidade**     | <ul><li>Separação de preocupações em diferentes arquivos como **index.js**, **style.css** e **index.html**.</li><li>Divisão clara de funcionalidades para fácil manutenção e atualizações.</li><li>Encapsulamento de recursos específicos para melhor organização do código.</li></ul>                                                            |
| 🧪  |       **Testes**        | <ul><li>Informações ausentes sobre comandos de teste no contexto fornecido.</li><li>Recomendação: Implementar testes unitários para funcionalidades críticas para garantir confiabilidade.</li><li>Considerar testes de integração para cenários de ponta a ponta para validar o comportamento do sistema.</li></ul>                              |
| ⚡️ |     **Desempenho**      | <ul><li>Comunicação em tempo real eficiente usando **WebSocket** para interações responsivas.</li><li>Renderização otimizada da UI para uma experiência de usuário suave em diferentes dispositivos.</li><li>Consideração de melhorias de desempenho na base de código para processamento de dados mais rápido.</li></ul>                         |
| 🛡️  |      **Segurança**      | <ul><li>Sem detalhes específicos de segurança fornecidos no contexto.</li><li>Recomendação: Implementar conexões WebSocket seguras para troca de dados.</li><li>Garantir validação adequada de entrada e sanitização de dados para prevenir vulnerabilidades.</li></ul>                                                                           |
| 📦  |    **Dependências**     | <ul><li>Uso de bibliotecas essenciais como **@google/generative-ai**, **dotenv** e **express** para funcionalidade do projeto.</li><li>Gerenciamento de dependências através de **package-lock.json** para consistência de versões.</li><li>Configuração e instalação adequadas de dependências usando **npm**.</li></ul>                         |

---


# 🛠️ Hardware e Conexões (Duna System)
## 📋 Lista de Materiais (BOM)
* 1x Placa ESP32 (DevKit V1)

* 1x Display OLED 0.96" I2C (SSD1306)

* 1x Sensor de Temperatura e Umidade (DHT11)

* 1x Sensor de Movimento PIR (HC-SR501)

* 1x Sensor de Obstáculo IR (KY-032) - Para os gestos

* 1x Sensor de Luz (LDR 5mm) + Resistor 10kΩ

* 1x LED Emissor Infravermelho (IR) + Resistor 220Ω

## 🔌 Mapa de Pinagem (Pinout)
Use esta tabela para garantir as conexões corretas no microcontrolador:

| Componente | Pino do Componente | **Pino no ESP32** | Observação |
| :--- | :--- | :---: | :--- |
| **LED IR** (Emissor) | Anodo (+) | **GPIO 4** | Necessário resistor de 220Ω |
| **OLED Display** | SDA | **GPIO 21** | Pino I2C Padrão |
| **OLED Display** | SCL | **GPIO 22** | Pino I2C Padrão |
| **Sensor Gestos** | OUT / S | **GPIO 25** | Sensor de Obstáculo IR |
| **Sensor Temp** | DATA / OUT | **GPIO 26** | DHT11 |
| **Sensor Movimento** | OUT / S | **GPIO 27** | PIR HC-SR501 |
| **Sensor Luz** | Saída Analógica | **GPIO 34** | Divisor de Tensão LDR |
| **Alimentação** | VCC / GND | **3.3V / GND** | Compartilhado |

---

# 🎮 Manual de Uso (Gestos e Luzes)
## 🖐️ Controle de Música (Gestos)
Passe a mão na frente do Sensor de Obstáculo (Pino 25) para controlar o Spotify:

* Passe Rápido (< 1s): Próxima Música (Display mostra >>>)
* Segurar (1s a 3s): Play / Pause (Display mostra || ou >)
* Manter Segurado (> 3s): Música Anterior (Display mostra <<<)

# 💡 Modos de Iluminação Automática
O sistema define a "Casa" (Tema) dependendo da claridade do quarto:

| Leitura LDR | Modo     | Casa    | Cor do LED      | Ícone OLED          |
|-------------|----------|---------|-----------------|---------------------|
| < 1500      | ATREIDES | Nobreza | Ciano/Turquesa  | Falcão              |
| 1500 - 3900 | ARRAKIS  | Deserto | Laranja Intenso | Shai-Hulud (Verme)  |
| > 3900      | SIETCH   | Caverna | Azul Índigo     | Gota (Água da Vida) |

# 💻 Instalação e Software
## Pré-requisitos
* Node.js (v14 ou superior) para o Dashboard.
* Arduino IDE para o firmware do ESP32.

# Configuração do Firmware (ESP32)
Instale as seguintes bibliotecas na Arduino IDE: 
* Adafruit SSD1306
* Adafruit GFX
* DHT sensor library
* IRremoteESP8266
* WebSockets (Markus Sattler).

#### Edite as credenciais no código .ino:

```c++
const char* ssid = "SUA_REDE_WIFI";
const char* password = "SUA_SENHA_WIFI";
const char* websocket_server = "seu-backend.onrender.com";
const char* spotify_pass = "duna1234";
```

### 📂 Índice do Projeto

<details open> <summary><b><code>ZEN-DASHBOARD/</code></b></summary> <details> <summary><b>Core Files</b></summary> <blockquote> <table> <tr> <td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/style.css'>style.css</a></b></td> <td>Define estilos Glassmorphism, temas globais e design responsivo.</td> </tr> <tr> <td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/index.js'>index.js</a></b></td> <td>Servidor Express e WebSocket. Gerencia IA, Spotify e comunicação com ESP32.</td> </tr> <tr> <td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/index.html'>index.html</a></b></td> <td>Front-end do dashboard com visualização de sensores e chat.</td> </tr> <tr> <td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/package.json'>package.json</a></b></td> <td>Dependências do Node.js (@google/generative-ai, spotify-web-api-node, etc).</td> </tr> </table> </blockquote> </details> </details>

---

## 🎗 Licença

Este projeto é distribuído sob uma licença didática e sem fins lucrativos, desenvolvida por [Cauã Santos](https://github.com/cauasantoslt). O objetivo é promover o aprendizado, compartilhamento de conhecimento e uso acadêmico. Qualquer uso comercial é proibido.

Para mais informações, acesse o [GitHub de Cauã Santos](https://github.com/cauasantoslt).

---

## 🙌 Agradecimentos

Agradeço primeiramente a Deus e a todos os que me apoiaram de alguma forma.

> "Seja você quem for, seja qual for a posição social que você tenha na vida, a mais alta ou a mais baixa, tenha sempre como meta muita força, muita determinação e sempre faça tudo com muito amor e com muita fé em Deus, que um dia você chega lá. De alguma maneira você chega lá."
>
> Ayrton Senna.

