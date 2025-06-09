# 🎮 Jogo PvP com ATmega328P

Este projeto de jogo PvP utiliza o microcontrolador ATmega328P. O objetivo é pressionar os botões rapidamente para acender LEDs, com o primeiro jogador a completar todos os LEDs ganhando. 🏆

## Funcionalidade

- **Jogo PvP**: Dois jogadores competem pressionando seus botões para acender LEDs. ⚔️
- **Interrupções**: A lógica de jogo é controlada por interrupções, garantindo resposta rápida dos botões. ⚡
- **PWM por software**: O brilho dos LEDs é controlado via PWM por software. 💡
- **Vitória**: O primeiro jogador a acender todos os LEDs vence. 🎉

### Componentes utilizados:
- **ATmega328P**: Microcontrolador principal. 💻
- **8 LEDs**: Controlados via PWM. 🔴🟠🟡🟢🔵🟣🟤⚫
- **3 Botões**: 2 para os jogadores e 1 para resetar o jogo. 🕹️
- **Timer**: Utilizado para controle de tempo e PWM. ⏲️

## Estrutura do Projeto

- `Makefile`: Automatiza o processo de compilação e upload utilizando `arduino-cli`. ⚙️
- `src/main/main.ino`: Código principal do jogo. 📝

## Como Compilar e Fazer Upload

1. Clone o repositório:
   \```bash
   git clone <URL_do_repositório>
   cd PvpLedGame
   \```

2. Compile e faça o upload utilizando o `Makefile`:
   - Para compilar:
     \```bash
     make build
     \```
   - Para fazer o upload para o ATmega328P:
     \```bash
     make upload
     \```

## Como Funciona

1. O jogo começa no estado "desligado". Quando o botão de reset é pressionado, o jogo entra no estado "em jogo". 🔌
2. Cada jogador deve pressionar seu botão para acender LEDs. O objetivo é acender todos os LEDs antes do adversário. ⚡
3. Quando um jogador completa todos os seus LEDs, o jogo entra no estado "vitória", piscando os LEDs do vencedor. 🎉
4. O botão de reset reinicia o jogo, apagando todos os LEDs e zerando as pontuações. 🔄

## 📡 Esquemático do Projeto

O projeto é composto por um microcontrolador ATmega328P, 8 LEDs, 3 botões e resistores para pull-up nos botões. A seguir, explico a configuração dos componentes:

### Componentes:

1. **ATmega328P**: O microcontrolador principal que controla os LEDs, lê os botões e executa a lógica do jogo.
   
2. **LEDs (PORTD 0-7)**: Os LEDs estão conectados aos pinos PD0 a PD7 do ATmega328P. Eles são controlados por PWM por software para ajustar a intensidade do brilho de acordo com a pontuação dos jogadores. 🔴🟠🟡🟢🔵🟣🟤⚫

3. **Botões (PC0, PC1, PC2)**:
   - **PC0**: Botão do Jogador 1, conectado ao pino PC0 do ATmega328P.
   - **PC1**: Botão do Jogador 2, conectado ao pino PC1 do ATmega328P.
   - **PC2**: Botão de reset, conectado ao pino PC2.
   
   Todos os botões estão configurados com resistores pull-up internos para garantir um nível lógico alto quando não pressionados e baixo quando pressionados.

4. **Resistores Pull-Up**: Para cada botão, um resistor pull-up interno é ativado via software, mantendo o nível lógico alto (1) até que o botão seja pressionado, forçando o pino a um nível baixo (0).

5. **Fonte de Alimentação**: O circuito é alimentado com uma tensão de 5V, que é fornecida ao ATmega328P e aos LEDs.

### Conexões:
- **LEDs**: 
   - Conectados nos pinos PD0 a PD7 do ATmega328P. Cada pino de LED pode ser controlado independentemente com uma variação de intensidade de brilho (PWM por software).
   
- **Botões**: 
   - Botão de Jogador 1 conectado ao pino PC0.
   - Botão de Jogador 2 conectado ao pino PC1.
   - Botão de Reset conectado ao pino PC2.

### Funcionalidade:

- Quando o botão de um jogador é pressionado, a interrupção do microcontrolador é acionada e a pontuação desse jogador é incrementada.
- Quando a pontuação de um jogador atingir 10, um LED é aceso. Os LEDs são acesos em ordem, com o jogador 1 acendendo os LEDs de 0 a 3 e o jogador 2 acendendo de 7 a 4.
- O botão de reset reinicia o jogo, apagando todos os LEDs e zerando a pontuação dos jogadores.

### Diagrama Esquemático

O esquemático do projeto pode ser desenhado utilizando ferramentas como Fritzing ou Eagle. Para fins de entendimento, o esquemático pode ser ilustrado da seguinte maneira:

\```plaintext
                    +------------------+
                    |                  |
    5V -------------| VCC              |
                    |                  |
                    |  ATmega328P      |
    GND ------------| GND              |
                    |                  |
    PC0  ---------->| Button 1 (Player 1)|
                    |                  |
    PC1  ---------->| Button 2 (Player 2)|
                    |                  |
    PC2  ---------->| Reset Button      |
                    |                  |
                    |                  |
  PD0 - PD7  ------>| LEDs (Player 1 and Player 2)|
                    |                  |
                    +------------------+
\```

**Nota**: Esse diagrama é uma representação conceitual das conexões. Para criar um esquemático real, utilize uma ferramenta de design de circuitos eletrônicos como **Fritzing** ou **Eagle** para conectar os componentes e gerar o diagrama.

---

## Dependências

- `arduino-cli`: Certifique-se de ter o `arduino-cli` instalado em sua máquina para compilar e fazer o upload do código. 🛠️

## Contribuições

Contribuições são bem-vindas! Para qualquer sugestão ou melhoria, sinta-se à vontade para abrir um issue ou enviar um pull request. 💬

---

**Equipe de Desenvolvimento**: 👨‍💻
- **Lucas Belino**
- **Lucas Motta**
- **Yam Sol Bertamini**
