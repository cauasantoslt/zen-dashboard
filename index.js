// --- 0. Importações ---
const express = require('express');
const http = require('http');
const path = require('path');
const { WebSocketServer } = require('ws');
require('dotenv').config(); // Carrega o ficheiro .env
const { GoogleGenerativeAI } = require('@google/generative-ai');
const SpotifyWebApi = require('spotify-web-api-node');

let latestSensorData = null; // "Memória" para os dados dos sensores

// --- 1. Configuração do "Alpha Centauri" (Gemini) ---
if (!process.env.GOOGLE_API_KEY) {
  console.error("ERRO: GOOGLE_API_KEY não encontrada no .env");
  process.exit(1);
}
const genAI = new GoogleGenerativeAI(process.env.GOOGLE_API_KEY);
// Usando o modelo mais rápido
const geminiModel = genAI.getGenerativeModel({ model: "gemini-2.5-flash" }); 
console.log("Cliente do 'Alpha Centauri' (Gemini) inicializado.");

// --- 2. Configuração do Spotify ---
if (!process.env.SPOTIFY_CLIENT_ID || !process.env.SPOTIFY_CLIENT_SECRET) {
  console.error("ERRO: SPOTIFY_CLIENT_ID ou SPOTIFY_CLIENT_SECRET não encontradas no .env");
  process.exit(1.1);
}

const spotifyApi = new SpotifyWebApi({
  clientId: process.env.SPOTIFY_CLIENT_ID,
  clientSecret: process.env.SPOTIFY_CLIENT_SECRET,
  redirectUri: 'http://127.0.0.1:3000/callback' 
});

const scopes = [
  'user-read-playback-state',
  'user-modify-playback-state',
  'user-read-currently-playing'
];
console.log("Cliente do Spotify inicializado.");

// --- 3. Configuração do Servidor Web (Express) ---
const app = express();
const server = http.createServer(app);
app.use(express.static(__dirname)); 

// --- 4. Rotas de Autenticação do Spotify ---
app.get('/login', (req, res) => {
  console.log("Redirecionando para o login do Spotify...");
  res.redirect(spotifyApi.createAuthorizeURL(scopes));
});

app.get('/callback', async (req, res) => {
  const code = req.query.code;
  if (!code) { return res.send("Erro: Não foi recebido nenhum código do Spotify."); }
  
  try {
    const data = await spotifyApi.authorizationCodeGrant(code);
    spotifyApi.setAccessToken(data.body['access_token']);
    spotifyApi.setRefreshToken(data.body['refresh_token']);

    console.log("Sucesso! Tokens do Spotify obtidos.");
    // Começa a buscar dados do Spotify a cada 5 segundos
    setTimeout(() => setInterval(getCurrentlyPlaying, 5000), 5000); 
    res.redirect('/'); 

  } catch (err) {
    console.error('Erro ao obter tokens do Spotify:', err);
    res.send("Erro ao obter tokens. Tente novamente.");
  }
});

// --- 5. Configuração do Servidor WebSocket ---
const wss = new WebSocketServer({ server });
let wsClients = new Set(); 

wss.on('connection', (ws) => {
  console.log('Cliente WebSocket conectado!');
  wsClients.add(ws); 

  ws.on('message', async (message) => {
    let data;
    try { data = JSON.parse(message); } catch (error) { return; }

    // --- LÓGICA DE DIRECIONAMENTO ---
    
    // 1. É do ESP32? (Se tiver 'luz', vem do sensor)
    if (data.luz !== undefined) {
      latestSensorData = data; // Guarda os dados na "memória"
      
      // --- Lógica de Modo Duna ---
      let modoDuna = "Sietch"; // Começa por assumir Sietch
      const luz = data.luz;
      
      // Lógica baseada APENAS NA LUZ (Escala Reversa: Menor = Mais Luz)
      // > 3900 = Escuro (Sietch)
      // 1500 - 3900 = Amarelo (Arrakis)
      // < 1500 = Branco (Atreides)

      if (luz < 1500) {  
        modoDuna = "Atreides"; // Luz branca
      } else if (luz >= 1500 && luz <= 3900) { 
        modoDuna = "Arrakis"; // Luz amarela
      } else {
        modoDuna = "Sietch"; // Escuro
      }

      // Adiciona o modo calculado ao JSON
      data.mode = modoDuna; 
      const dataString = JSON.stringify(data);
      
      // Envia para o Dashboard, mas NÃO devolve para o ESP32 (ws)
      broadcast(dataString, ws); 
    }
    
    // 2. É do Chat?
    else if (data.chat !== undefined) {
      console.log(`Pergunta para o Alpha Centauri: ${data.chat}`);
      
      const chatPrompt = `
        Seja um assistente de IA chamado Alpha Centauri.
        Responda sempre de forma muito concisa (máximo 3 frases).
        Não use markdown.
        Pergunta do utilizador: "${data.chat}"
      `;

      try {
        const result = await geminiModel.generateContent(chatPrompt); 
        const response = await result.response;
        broadcast(JSON.stringify({ sender: "Alpha Centauri", message: response.text() }));
      } catch (error) {
        console.error("Erro na API do Gemini:", error);
        broadcast(JSON.stringify({ sender: "Alpha Centauri", message: "Desculpe, estou com problemas." }));
      }
    }
    
    // 3. É um comando do Spotify?
    else if (data.spotify_command !== undefined) {
      console.log(`Comando Spotify recebido: ${data.spotify_command}`);
      try {
        if (data.spotify_command === "play") await spotifyApi.play();
        if (data.spotify_command === "pause") await spotifyApi.pause();
        if (data.spotify_command === "next") await spotifyApi.skipToNext();
        if (data.spotify_command === "prev") await spotifyApi.skipToPrevious();

        // Atualização rápida
        setTimeout(getCurrentlyPlaying, 300);

      } catch (err) {
        console.error("Erro ao executar comando do Spotify:", err);
      }
    }
  });

  ws.on('close', () => {
    console.log('Cliente WebSocket desconectado.');
    wsClients.delete(ws); 
  });

  // Boas-vindas
  ws.send(JSON.stringify({ sender: "Servidor", message: "Bem-vindo ao Servidor Central!" }));
});

// Função para enviar mensagem para todos
function broadcast(message, originWs = null) {
  wsClients.forEach((client) => {
    if (client === originWs) return; // Ignora o remetente
    if (client.readyState === client.OPEN) {
      client.send(String(message));
    }
  });
}

// --- 6. Lógica de Fundo do Spotify ---
async function getCurrentlyPlaying() {
  try {
    const data = await spotifyApi.getMyCurrentPlaybackState();
    
    if (data.body && data.body.item) {
      const track = {
        sender: "Spotify",
        is_playing: data.body.is_playing, 
        name: data.body.item.name,
        artist: data.body.item.artists.map(a => a.name).join(', '),
        image_url: data.body.item.album.images[0].url
      };
      broadcast(JSON.stringify(track));
    } else {
      broadcast(JSON.stringify({ sender: "Spotify", is_playing: false }));
    }
  } catch (err) {
    if (err.statusCode === 401) {
      console.log("Token do Spotify expirou. A renovar...");
      try {
        const data = await spotifyApi.refreshAccessToken();
        spotifyApi.setAccessToken(data.body['access_token']);
        console.log("Token renovado!");
      } catch (refreshErr) {
        // Silencioso se falhar a renovação
      }
    }
  }
}

// --- 6.5. Lógica de Análise do Ambiente (IA) ---
async function analyzeEnvironment() {
  if (!latestSensorData || latestSensorData.luz === undefined) {
    console.log("Análise da IA pulada (sem dados de luz).");
    return;
  }

  const luz = latestSensorData.luz; 

  const prompt = `
    Você é o "Alpha Centauri", uma IA que monitora um ambiente.
    Analise este dado do sensor de luz:
    - Luminosidade: ${luz} (ATENÇÃO: A escala é REVERSA. 0 é claridade máxima, 4095 é escuridão total).

    Descreva o status da iluminação em uma frase muito curta e amigável (máximo 10 palavras).
    Exemplos: "O quarto está muito claro." (se for 1000) ou "Está escuro como uma caverna." (se for 4000).
  `;

  try {
    const result = await geminiModel.generateContent(prompt);
    const response = await result.response;
    const text = response.text();
    
    console.log(`Status da IA gerado: ${text}`);
    
    broadcast(JSON.stringify({
      sender: "IA_Status",
      status: text
    }));

  } catch (error) {
    console.error("Erro ao gerar status da IA:", error);
  }
}

// --- 7. Iniciar o Servidor ---
const port = 3000;
server.listen(port, () => {
  console.log(`Servidor a funcionar em http://localhost:${port}`);
  console.log("-------------------------------------------------");
  console.log("PARA O SPOTIFY FUNCIONAR:");
  console.log(`http://localhost:3000/login`);
  console.log("-------------------------------------------------");
  
  // Corre a IA a cada 1 minuto (60000 ms)
  setTimeout(() => setInterval(analyzeEnvironment, 60000), 30000);
});