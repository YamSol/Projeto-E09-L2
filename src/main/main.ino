#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// Definição dos pinos de LEDs
#define LED_PORT        PORTD
#define LED_DDR         DDRD
#define LED_PIN_MASK    0b11111111  // Pinos PD0 a PD7

// Definição dos pinos dos botões
#define BUTTON_PORT     PORTC
#define BUTTON_PIN      PINC
#define BUTTON_DDR      DDRC
#define BUTTON1_PIN     PB0  // Botão jogador 1
#define BUTTON2_PIN     PB1  // Botão jogador 2
#define BUTTON_RESET_PIN PB2 // Botão de reset

// Definição das interrupções de pinos
#define PCINT1_MASK     ((1 << BUTTON1_PIN) | (1 << BUTTON2_PIN) | (1 << BUTTON_RESET_PIN)) // Máscara para interrupções nos pinos PC0, PC1 e PC2
#define PCINT1_VECTOR   PCINT1_vect  // Vetor de interrupção

// Definição do pino do buzzer (ajuste conforme necessário)
#define BUZZER_PIN      PB3 // Pino para o buzzer
#define BUZZER_PORT     PORTB
#define BUZZER_DDR      DDRB

volatile uint8_t led_brightness[8] = {0};  // Armazena a intensidade de brilho (PWM) de cada LED (0-255)
volatile uint8_t leds_on = 0;  // Armazena o estado dos LEDs, os 4 primeiros bits para jogador 1 e os 4 últimos bits para jogador 2
volatile uint8_t score = 0; // Os 4 primeiros bits são o score do jogador 1 e os 4 últimos bits são o score do jogador 2
volatile uint8_t estado = 0; // 0 = desligado, 1 = em jogo, 2 = vitória

// Função de configuração dos pinos
void setup_io() {
    // LEDs como saída
    LED_DDR = LED_PIN_MASK;  // Define todos os pinos de PD0 a PD7 como saída
    LED_PORT = 0x00; // Desliga todos os LEDs inicialmente

    // Botões como entrada com pull-up
    BUTTON_DDR &= ~PCINT1_MASK;  // Configura PC0, PC1, PC2 como entrada
    BUTTON_PORT |= PCINT1_MASK;    // Ativa resistores pull-up para PC0, PC1 e PC2

    // Configura o pino do buzzer como saída
    BUZZER_DDR |= (1 << BUZZER_PIN);  // Define o pino PB3 como saída
    BUZZER_PORT &= ~(1 << BUZZER_PIN);  // Desliga o buzzer inicialmente

    // Habilita interrupções para PCINT[16:18] (PCIE1 para PCINT[14:8], PCMSK1)
    PCICR |= (1 << PCIE1);      // Habilita interrupções externas para PCINT[16:18]
    PCMSK1 |= PCINT1_MASK;  // Habilita PCINT8, PCINT9 e PCINT10
}

// Interrupção para tratamento de mudanças nos botões
ISR(PCINT1_VECTOR) {
    static uint8_t last_state = 0xFF;  // Variável para armazenar o último estado dos botões
    uint8_t current_state = BUTTON_PIN & PCINT1_MASK;  // Lê o estado atual dos botões
    uint8_t changed = last_state ^ current_state;  // Determina quais botões mudaram de estado

    // Se o jogo estiver em andamento (estado = 1)
    if (estado == 1) {
        // Verifica se o botão do jogador 1 foi pressionado
        if ((changed & (1 << BUTTON1_PIN)) && !(current_state & (1 << BUTTON1_PIN))) {
            if ((score & 0x0F) < 10) {  // Verifica se o jogador 1 não completou todos os LEDs
                score++;  // Aumenta a pontuação de jogador 1
                led_brightness[leds_on & 0x0F] = (score & 0x0F) * 25;  // Atualiza a intensidade do LED correspondente
                if ((score & 0x0F) >= 10) {  // Se o jogador 1 completou um LED
                    leds_on++;  // Passa para o próximo LED (jogador 1)
                    score &= 0xF0;  // Reseta a pontuação do jogador 1 (mantém o score do jogador 2)
                    if ((leds_on & 0x0F) >= 4) estado = 2;  // Se o jogador 1 completou todos os LEDs, o jogo acaba
                }
            }
        }

        // Verifica se o botão do jogador 2 foi pressionado
        if ((changed & (1 << BUTTON2_PIN)) && !(current_state & (1 << BUTTON2_PIN))) {
            if ((score >> 4) < 10) {  // Verifica se o jogador 2 não completou todos os LEDs
                score += (1 << 4);  // Aumenta a pontuação de jogador 2
                led_brightness[4 + (leds_on >> 4)] = ((score >> 4) & 0x0F) * 25;  // Atualiza a intensidade do LED correspondente
                if ((score >> 4) >= 10) {  // Se o jogador 2 completou um LED
                    leds_on |= (1 << (4 + (leds_on >> 4)));  // Passa para o próximo LED (jogador 2)
                    score &= 0x0F;  // Reseta a pontuação do jogador 2 (mantém o score do jogador 1)
                    if ((leds_on >> 4) >= 4) estado = 2;  // Se o jogador 2 completou todos os LEDs, o jogo acaba
                }
            }
        }
    }

    // Verifica se o botão de reset foi pressionado
    if ((changed & (1 << BUTTON_RESET_PIN)) && !(current_state & (1 << BUTTON_RESET_PIN))) {
        estado = 1;  // Inicia um novo jogo
        for (int i = 0; i < 8; i++) led_brightness[i] = 0;  // Reseta os LEDs
        score = 0;  // Reseta a pontuação
        leds_on = 0;  // Reseta o número de LEDs acesos
    }

    last_state = current_state;  // Atualiza o estado anterior dos botões
}

// Função de PWM por software
void pwm_software() {
    // Incrementa o contador de brilho de cada LED
    for (uint8_t t = 0; t < 255; t++) {
        for (uint8_t i = 0; i < 8; i++) {
            // Se o brilho do LED for maior que o contador, acende o LED
            if (led_brightness[i] > t) LED_PORT |= (1 << i);
            else LED_PORT &= ~(1 << i);  // Caso contrário, apaga o LED
        }
        _delay_us(100);  // Delay para suavizar o PWM
    }
}

// Função para piscar LEDs do vencedor e ativar o buzzer
void victory_blink() {
    static uint8_t counter = 0;  // Contador para alternar o estado de piscar
    counter++;  // Incrementa o contador
    uint8_t state = (counter >> 4) & 1;  // Alterna entre 0 e 1 a cada 16 ciclos

    // Ativa o buzzer
    BUZZER_PORT |= (1 << BUZZER_PIN);  // Liga o buzzer
    _delay_ms(100);  // Duração do bipe
    BUZZER_PORT &= ~(1 << BUZZER_PIN);  // Desliga o buzzer
    _delay_ms(100);  // Pausa entre os bipes

    // Altera o brilho dos LEDs do jogador vencedor
    for (int i = 0; i < 4; i++) {
        // Se o jogador 1 ganhou, alterna o brilho dos LEDs 0 a 3
        led_brightness[i] = ((leds_on & 0x0F) >= 4) ? (state ? 255 : 0) : led_brightness[i];
        // Se o jogador 2 ganhou, alterna o brilho dos LEDs 4 a 7
        led_brightness[4 + i] = ((leds_on >> 4) >= 4) ? (state ? 255 : 0) : led_brightness[4 + i];
    }
}

int main(void) {
    setup_io();  // Configura os pinos de entrada e saída
    sei();  // Habilita interrupções globais

    while (1) {
        if (estado == 2) {  // Se o jogo terminou (vitória)
            victory_blink();  // Pisca os LEDs do vencedor e ativa o buzzer
        }
        pwm_software();  // Atualiza os LEDs com base no PWM
    }
}
