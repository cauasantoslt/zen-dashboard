const express = require('express');
const http = require('http');
const path = require('path');
const { WebSocketServer } = require('ws');
require('dotenv').config(); 
const { GoogleGenerativeAI } = require('@google/generative-ai');
const SpotifyWebApi = require('spotify-web-api-node');
const mongoose = require('mongoose');

let latestSensorData = null; 

// --- 1. Conexão MongoDB ---
mongoose.connect(process.env.MONGO_URI)
  .then(() => console.log("✅ Conectado ao MongoDB!"))
  .catch(err => console.error("❌ Erro MongoDB:", err));

const HistoricoSchema = new mongoose.Schema({
  data: { type: Date, default: Date.now }, 
  luz: Number,
  modo: String
});
const Historico = mongoose.model('Historico', HistoricoSchema);

// --- 2. Configuração IA (Gemini) ---
if (!process.env.GOOGLE_API_KEY) { console.error("ERRO: Falta GOOGLE_API_KEY"); process.exit(1); }
const genAI = new GoogleGenerativeAI(process.env.GOOGLE_API_KEY);
const geminiModel = genAI.getGenerativeModel({ model: "gemini-2.5-flash" }); 

// --- 3. Configuração Spotify ---
if (!process.env.SPOTIFY_CLIENT_ID || !process.env.SPOTIFY_CLIENT_SECRET) { console.error("ERRO: Falta chaves Spotify"); process.exit(1); }

const spotifyApi = new SpotifyWebApi({
  clientId: process.env.SPOTIFY_CLIENT_ID,
  clientSecret: process.env.SPOTIFY_CLIENT_SECRET,
  // ATUALIZADO PARA O SEU LINK DO RENDER
  redirectUri: process.env.REDIRECT_URI || 'https://zen-dashboard-97j0.onrender.com/callback'
});

const scopes = ['user-read-playback-state', 'user-modify-playback-state', 'user-read-currently-playing'];

// --- 4. Servidor Web ---
const app = express();
const server = http.createServer(app);
app.use(express.static(__dirname)); 

// Rotas Spotify
app.get('/login', (req, res) => { res.redirect(spotifyApi.createAuthorizeURL(scopes)); });
app.get('/callback', async (req, res) => {
  const code = req.query.code;
  if (!code) return res.send("Erro no código Spotify.");
  try {
    const data = await spotifyApi.authorizationCodeGrant(code);
    spotifyApi.setAccessToken(data.body['access_token']);
    spotifyApi.setRefreshToken(data.body['refresh_token']);
    setTimeout(() => setInterval(getCurrentlyPlaying, 5000), 5000); 
    res.redirect('/'); 
  } catch (err) { res.send("Erro login Spotify."); }
});

// Rota Histórico (para o gráfico, se voltarmos a usar)
app.get('/api/historico', async (req, res) => {
  try {
    const dados = await Historico.find().sort({ data: -1 }).limit(20);
    res.json(dados.reverse());
  } catch (err) { res.status(500).json({ error: 'Erro banco' }); }
});

// --- 5. WebSocket ---
const wss = new WebSocketServer({ server });
let wsClients = new Set(); 

wss.on('connection', (ws) => {
  wsClients.add(ws); 
  ws.on('message', async (message) => {
    let data;
    try { data = JSON.parse(message); } catch (error) { return; }

    if (data.luz !== undefined) { // Dados do ESP32
      latestSensorData = data; 
      
      // Lógica Duna
      let modoDuna = "Sietch";
      const luz = data.luz;
      if (luz < 1500) { modoDuna = "Atreides"; } 
      else if (luz >= 1500 && luz <= 3900) { modoDuna = "Arrakis"; } 
      else { modoDuna = "Sietch"; }

      data.mode = modoDuna; 
      
      // Salvar no Banco
      const novoRegistro = new Historico({ luz: luz, modo: modoDuna });
      novoRegistro.save().catch(err => console.error("Erro salvar banco:", err));
      
      // Enviar para Frontend (não devolve ao ESP32)
      broadcast(JSON.stringify(data), ws); 
    }
    else if (data.chat !== undefined) { // Chat IA
      const chatPrompt = `Seja o Alpha Centauri. Responda curto (max 3 frases). Pergunta: "${data.chat}"`;
      try {
        const result = await geminiModel.generateContent(chatPrompt); 
        broadcast(JSON.stringify({ sender: "Alpha Centauri", message: result.response.text() }));
      } catch (error) { console.error("Erro IA", error); }
    }
    else if (data.spotify_command !== undefined) { // Comandos Spotify
      try {
        if (data.spotify_command === "play") await spotifyApi.play();
        if (data.spotify_command === "pause") await spotifyApi.pause();
        if (data.spotify_command === "next") await spotifyApi.skipToNext();
        if (data.spotify_command === "prev") await spotifyApi.skipToPrevious();
        setTimeout(getCurrentlyPlaying, 300);
      } catch (err) { console.error("Erro Cmd Spotify"); }
    }
  });
  ws.on('close', () => { wsClients.delete(ws); });
});

function broadcast(message, originWs = null) {
  wsClients.forEach((client) => {
    if (client === originWs) return; 
    if (client.readyState === client.OPEN) client.send(String(message));
  });
}

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
    } else { broadcast(JSON.stringify({ sender: "Spotify", is_playing: false })); }
  } catch (err) { 
    if (err.statusCode === 401) {
      try {
        const data = await spotifyApi.refreshAccessToken();
        spotifyApi.setAccessToken(data.body['access_token']);
      } catch (e) {}
    }
  }
}

async function analyzeEnvironment() {
  if (!latestSensorData || latestSensorData.luz === undefined) return;
  const prompt = `Alpha Centauri, analise: Luminosidade ${latestSensorData.luz} (0=claro, 4095=escuro). Status curto (max 10 palavras).`;
  try {
    const result = await geminiModel.generateContent(prompt);
    broadcast(JSON.stringify({ sender: "IA_Status", status: result.response.text() }));
  } catch (error) { console.error("Erro IA Status"); }
}

const port = process.env.PORT || 3000;
server.listen(port, () => {
  console.log(`Servidor rodando na porta ${port}`);
  setTimeout(() => setInterval(analyzeEnvironment, 60000), 30000);
});