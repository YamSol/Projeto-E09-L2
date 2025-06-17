# Jogo de Competição Arduino - Pressionar Botões

## Descrição do Projeto
Jogo interativo de competição entre dois jogadores onde cada um deve pressionar seu botão para preencher seus LEDs. O primeiro jogador a completar todos os 4 LEDs vence. O projeto utiliza programação em C puro para Arduino com técnicas avançadas de microcontroladores.

![Circuito do Projeto](assets/imagem-circuito.png)

## Componentes Técnicos Utilizados

### 1. Interrupções (Interrupts)
O projeto utiliza **Timer1** com interrupção por comparação (CTC - Clear Timer on Compare Match):
- **ISR(TIMER1_COMPA_vect)**: Interrupção executada a cada 1ms
- **Função**: Controla a sequência de bipes do buzzer (largada e fim de jogo)
- **Configuração**: Timer1 com prescaler 64, OCR1A = 249 para período de 1ms
- **Vantagem**: Execução precisa de timing sem bloquear o código principal

### 2. PWM Digital (Software PWM)
Implementação de PWM por software para controle de brilho dos LEDs:
- **Função pwm_software()**: Gera sinais PWM para 8 LEDs simultaneamente
- **Resolução**: 8 bits (0-255 níveis de brilho)
- **Princípio**: Controla o tempo que cada LED fica ligado/desligado em cada ciclo
- **Aplicação**: Mostra o progresso visual conforme o jogador pressiona o botão

### 3. Timer Contador
Sistema de contagem temporal baseado no Timer1:
- **buzzer_timer**: Contador incrementado a cada 1ms pela interrupção
- **Estados do buzzer**: Controla sequências temporais complexas (6 estados para largada)
- **Precisão**: Timing exato para bipes de diferentes durações
- **Reset automático**: Reinicia contadores ao completar sequências

## Lista de Componentes Físicos

### LEDs
- **8 LEDs de 5mm**
  - 4 LEDs verdes (jogador 1)
  - 4 LEDs amarelos (jogador 2)
- **Especificações**: 3V forward voltage, 20mA corrente típica

### Resistores
- **8 resistores de 220Ω** (para LEDs)
  - Cálculo: (5V - 3V) / 0.02A = 100Ω mínimo
  - 220Ω garante margem de segurança e brilho adequado

### Botões
- **3 push buttons** (normalmente aberto)
  - 2 botões para jogadores
  - 1 botão de reset/start

### Buzzer
- **1 buzzer piezoelétrico ativo 5V**
  - Não necessita resistor limitador
  - Controle digital on/off

### Protoboard e Jumpers
- **1 protoboard** 830 pontos
- **Jumpers macho-macho** coloridos para organização

## Montagem do Circuito

### Conexões dos LEDs
**Jogador 1 (LEDs Verdes)**:
- LED 1: Arduino pino D0 → resistor 220Ω → LED → GND
- LED 2: Arduino pino D1 → resistor 220Ω → LED → GND  
- LED 3: Arduino pino D2 → resistor 220Ω → LED → GND
- LED 4: Arduino pino D3 → resistor 220Ω → LED → GND

**Jogador 2 (LEDs Amarelos)**:
- LED 5: Arduino pino D4 → resistor 220Ω → LED → GND
- LED 6: Arduino pino D5 → resistor 220Ω → LED → GND
- LED 7: Arduino pino D6 → resistor 220Ω → LED → GND
- LED 8: Arduino pino D7 → resistor 220Ω → LED → GND

### Conexões dos Botões
- **Botão Jogador 1**: Arduino pino B0 → botão → GND (pull-up interno ativado)
- **Botão Jogador 2**: Arduino pino B1 → botão → GND (pull-up interno ativado)
- **Botão Reset**: Arduino pino B2 → botão → GND (pull-up interno ativado)

### Conexão do Buzzer
- **Buzzer**: Arduino pino B3 → buzzer positivo
- **GND**: buzzer negativo → GND Arduino

## Funcionamento do Jogo

### Estados do Jogo
1. **Parado (estado 0)**: Aguarda pressionar botão de reset
2. **Jogando (estado 1)**: Jogadores pressionam botões para preencher LEDs
3. **Fim (estado 2)**: Um jogador completou todos os LEDs

### Mecânica de Jogo
- Cada jogador deve pressionar seu botão **10 vezes** para acender completamente um LED
- A cada pressionada, o LED aumenta o brilho (PWM incremental)
- Após 10 pressionadas, o LED fica totalmente aceso e passa para o próximo
- O primeiro a completar os 4 LEDs vence

### Sequência de Início
O botão de reset/start inicia uma sequência de 3 bipes:
1. Bipe 1: 150ms ligado
2. Pausa: 100ms
3. Bipe 2: 150ms ligado  
4. Pausa: 100ms
5. Bipe 3: 500ms ligado
6. Jogo inicia automaticamente

## Características Técnicas

### Performance
- **Frequência de PWM**: Aproximadamente 1kHz (sem flickering visível)
- **Resolução de brilho**: 256 níveis por LED
- **Tempo de resposta**: < 1ms para detecção de botão
- **Precisão temporal**: 1ms para sequências de áudio

### Consumo
- **LEDs**: Máximo 160mA (8 LEDs × 20mA)
- **Arduino**: ~50mA
- **Buzzer**: ~30mA
- **Total**: ~240mA (dentro dos limites do Arduino)

## Compilação e Upload

### Configurações do Arduino IDE
- **Placa**: Arduino Uno
- **Porta**: Verificar porta serial correta
- **Programador**: AVRISP mkII (ou padrão)

### Bibliotecas Necessárias
O código utiliza apenas bibliotecas padrão do AVR:
```c
#include <avr/io.h>        // Operações de I/O de baixo nível
#include <avr/interrupt.h> // Sistema de interrupções
```

### Processo de Upload
1. Conectar Arduino via USB
2. Verificar seleção da porta no Arduino IDE
3. Compilar e fazer upload do código
4. Testar funcionamento dos componentes

## Expansões Possíveis
- Adicionar display LCD para mostrar score
- Implementar níveis de dificuldade
- Adicionar efeitos sonoros diferentes
- Criar modo torneio com múltiplas rodadas
- Implementar comunicação serial para debug
