# ZEN-DASHBOARD

### Painel IoT Full-Stack com design Glassmorphism, integrado com Spotify, Inteligência Artificial (Gemini 2.5) e monitoramento ambiental, inspirado no universo de Duna.


## 🔗 Índice

- [📍 Visão Geral](#-visão-geral)
- [👾 Funcionalidades](#-funcionalidades)
- [📂 Índice do Projeto](#-índice-do-projeto)
- [🎗 Licença](#-licença)
- [🙌 Agradecimentos](#-agradecimentos)

---

## 📍 Visão Geral

Zen-Dashboard é um projeto open-source elegante que revoluciona a experiência do usuário ao oferecer um painel dinâmico para exibição de dados em tempo real. Ele integra perfeitamente controles do Spotify, interações de chat, dados de sensores e temas visuais, melhorando a responsividade do sistema. Ideal para desenvolvedores que buscam criar aplicações web envolventes e interativas com um toque moderno.

---

## 👾 Funcionalidades

|     |     Funcionalidade      | Resumo                                                                                                                                                                                                                                                                                                                                            |
| :-- | :---------------------: | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
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


### 📂 Índice do Projeto

<details open>
	<summary><b><code>ZEN-DASHBOARD/</code></b></summary>
	<details> <!-- __root__ Submodule -->
		<summary><b>__root__</b></summary>
		<blockquote>
			<table>
			<tr>
				<td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/style.css'>style.css</a></b></td>
				<td>- Define estilos e temas globais para o projeto, incluindo esquemas de cores, grades de layout e designs de widget<br>- Configura uma UI visualmente atraente e consistente em toda a aplicação, melhorando a experiência do usuário e a identidade da marca<br>- Suporta design responsivo para diferentes tamanhos de tela, garantindo exibição ideal em vários dispositivos.</td>
			</tr>
			<tr>
				<td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/package-lock.json'>package-lock.json</a></b></td>
				<td>- Resumo:
O arquivo package-lock.json na estrutura do projeto define dependências e suas versões para o projeto "zen-servidor"<br>- Garante que o projeto use versões específicas de bibliotecas como @google/generative-ai, dotenv, express, mongoose e spotify-web-api-node, mantendo consistência e estabilidade na arquitetura da base de código.</td>
			</tr>
			<tr>
				<td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/index.js'>index.js</a></b></td>
				<td>- Facilita a comunicação e interação em tempo real entre um servidor web e clientes via WebSocket, permitindo troca de dados e manipulação de eventos<br>- Gerencia conexões, processa mensagens recebidas e transmite respostas aos clientes com base em tipos de dados específicos<br>- Suporta funcionalidades como processamento de dados de sensores, interações de chat com IA e comandos do Spotify<br>- Melhora a experiência do usuário e a responsividade do sistema.</td>
			</tr>
			<tr>
				<td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/package.json'>package.json</a></b></td>
				<td>- Facilita a configuração e instalação das dependências do projeto, incluindo Google Generative AI, Spotify Web API e Express, entre outras<br>- Permite integração e operação sem problemas das bibliotecas essenciais para a funcionalidade do projeto.</td>
			</tr>
			<tr>
				<td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/index.html'>index.html</a></b></td>
				<td>- Gerencia um painel dinâmico exibindo informações do Spotify, sensores ambientais, mensagens de chat e temas visuais<br>- Manipula atualizações em tempo real via conexões WebSocket, permitindo interação do usuário com controles do Spotify e funcionalidade de chat<br>- Atualiza automaticamente dados dos sensores, status da IA e exibição do relógio<br>- Controla o tema visual com base nos modos selecionados pelo usuário.</td>
			</tr>
			<tr>
				<td><b><a href='https://github.com/cauasantoslt/zen-dashboard/blob/master/load.txt'>load.txt</a></b></td>
				<td>- Habilita o tunelamento ngrok para desenvolvimento local, expondo um servidor local para a internet<br>- Executa ngrok com os parâmetros especificados e inicia o servidor Node.js<br>- Integra-se perfeitamente à estrutura do projeto para fácil configuração e teste.</td>
			</tr>
			</table>
		</blockquote>
	</details>
</details>

---

## 🔰 Contribuindo

- **💬 [Participe das Discussões](https://github.com/cauasantoslt/zen-dashboard/discussions)**: Compartilhe suas ideias, forneça feedback ou faça perguntas.
- **🐛 [Relatar Problemas](https://github.com/cauasantoslt/zen-dashboard/issues)**: Envie bugs encontrados ou registre solicitações de recursos para o projeto `zen-dashboard`.
- **💡 [Enviar Pull Requests](https://github.com/cauasantoslt/zen-dashboard/blob/main/CONTRIBUTING.md)**: Revise PRs abertas e envie suas próprias PRs.

<details closed>
<summary>Diretrizes de Contribuição</summary>

1. **Faça um Fork do Repositório**: Comece fazendo um fork do repositório do projeto na sua conta do github.
2. **Clone Localmente**: Clone o repositório forkado na sua máquina local usando um cliente git.
   ```sh
   git clone https://github.com/cauasantoslt/zen-dashboard
   ```
3. **Crie uma Nova Branch**: Sempre trabalhe em uma nova branch, dando a ela um nome descritivo.
   ```sh
   git checkout -b nova-funcionalidade-x
   ```
4. **Faça suas Alterações**: Desenvolva e teste suas alterações localmente.
5. **Commite suas Alterações**: Faça o commit com uma mensagem clara descrevendo suas atualizações.
   ```sh
   git commit -m 'Implementada nova funcionalidade x.'
   ```
6. **Envie para o github**: Envie as alterações para o seu repositório forkado.
   ```sh
   git push origin nova-funcionalidade-x
   ```
7. **Envie um Pull Request**: Crie um PR contra o repositório original do projeto. Descreva claramente as alterações e suas motivações.
8. **Revisão**: Uma vez que seu PR seja revisado e aprovado, ele será mesclado na branch principal. Parabéns pela sua contribuição!
</details>

<details closed>
<summary>Gráfico de Contribuidores</summary>
<br>
<p align="left">
   <a href="https://github.com{/cauasantoslt/zen-dashboard/}graphs/contributors">
      <img src="https://contrib.rocks/image?repo=cauasantoslt/zen-dashboard">
   </a>
</p>
</details>

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

