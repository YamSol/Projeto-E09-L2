
/* Projeto: Jogo PvP com ATmega328P
   Tema: Entretenimento
   Autor: [Seu Nome]
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000UL

// LEDs: PORTD 0-7
// Botões: PCINT16 (PD0) e PCINT17 (PD1)
// Botão de Reset: PCINT18 (PD2)

volatile uint8_t led_brightness[8] = {0};  // Armazena a intensidade de brilho (PWM) de cada LED (0-255)
volatile uint8_t score[2] = {0}; // score[0] = jogador 1, score[1] = jogador 2
volatile uint8_t leds_on[2] = {0};  // Armazena quantos LEDs cada jogador já acendeu
volatile uint8_t estado = 0; // 0 = desligado, 1 = em jogo, 2 = vitória

// Função de configuração dos pinos
void setup_io() {
    // LEDs como saída
    DDRD = 0xFF;  // Define todos os pinos de PD0 a PD7 como saída
    PORTD = 0x00; // Desliga todos os LEDs inicialmente

    // Botões como entrada com pull-up
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2));  // Configura PC0, PC1, PC2 como entrada
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2);    // Ativa resistores pull-up para PC0, PC1 e PC2

    // Habilita interrupções para PCINT[16:18] (PCIE1 para PCINT[14:8], PCMSK1)
    PCICR |= (1 << PCIE1);      // Habilita interrupções externas para PCINT[16:18]
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10);  // Habilita PCINT8, PCINT9 e PCINT10
}

// Interrupção para tratamento de mudanças nos botões
ISR(PCINT1_vect) {
    static uint8_t last_state = 0xFF;  // Variável para armazenar o último estado dos botões
    uint8_t current_state = PINC & ((1 << PC0) | (1 << PC1) | (1 << PC2));  // Lê o estado atual dos botões
    uint8_t changed = last_state ^ current_state;  // Determina quais botões mudaram de estado

    // Se o jogo estiver em andamento (estado = 1)
    if (estado == 1) {
        // Verifica se o botão do jogador 1 foi pressionado
        if ((changed & (1 << PC0)) && !(current_state & (1 << PC0))) {
            if (score[0] < 10) {  // Verifica se o jogador 1 não completou todos os LEDs
                score[0]++;  // Aumenta a pontuação de jogador 1
                led_brightness[leds_on[0]] = score[0] * 25;  // Atualiza a intensidade do LED correspondente
                if (score[0] >= 10) {  // Se o jogador 1 completou um LED
                    leds_on[0]++;  // Passa para o próximo LED
                    score[0] = 0;  // Reseta a pontuação
                    if (leds_on[0] >= 4) estado = 2;  // Se o jogador 1 completou todos os LEDs, o jogo acaba
                }
            }
        }

        // Verifica se o botão do jogador 2 foi pressionado
        if ((changed & (1 << PC1)) && !(current_state & (1 << PC1))) {
            if (score[1] < 10) {  // Verifica se o jogador 2 não completou todos os LEDs
                score[1]++;  // Aumenta a pontuação de jogador 2
                led_brightness[7 - leds_on[1]] = score[1] * 25;  // Atualiza a intensidade do LED correspondente
                if (score[1] >= 10) {  // Se o jogador 2 completou um LED
                    leds_on[1]++;  // Passa para o próximo LED
                    score[1] = 0;  // Reseta a pontuação
                    if (leds_on[1] >= 4) estado = 2;  // Se o jogador 2 completou todos os LEDs, o jogo acaba
                }
            }
        }
    }

    // Verifica se o botão de reset foi pressionado
    if ((changed & (1 << PC2)) && !(current_state & (1 << PC2))) {
        estado = 1;  // Inicia um novo jogo
        for (int i = 0; i < 8; i++) led_brightness[i] = 0;  // Reseta os LEDs
        score[0] = score[1] = 0;  // Reseta as pontuações
        leds_on[0] = leds_on[1] = 0;  // Reseta o número de LEDs acesos
    }

    last_state = current_state;  // Atualiza o estado anterior dos botões
}

// Função de PWM por software
void pwm_software() {
    // Incrementa o contador de brilho de cada LED
    for (uint8_t t = 0; t < 255; t++) {
        for (uint8_t i = 0; i < 8; i++) {
            // Se o brilho do LED for maior que o contador, acende o LED
            if (led_brightness[i] > t) PORTD |= (1 << i);
            else PORTD &= ~(1 << i);  // Caso contrário, apaga o LED
        }
        _delay_us(100);  // Delay para suavizar o PWM
    }
}

// Função para piscar LEDs do vencedor
void victory_blink() {
    static uint8_t counter = 0;  // Contador para alternar o estado de piscar
    counter++;  // Incrementa o contador
    uint8_t state = (counter >> 4) & 1;  // Alterna entre 0 e 1 a cada 16 ciclos

    // Altera o brilho dos LEDs do jogador vencedor
    for (int i = 0; i < 4; i++) {
        // Se o jogador 1 ganhou, alterna o brilho dos LEDs 0 a 3
        led_brightness[i] = (leds_on[0] >= 4) ? (state ? 255 : 0) : led_brightness[i];
        // Se o jogador 2 ganhou, alterna o brilho dos LEDs 4 a 7
        led_brightness[7 - i] = (leds_on[1] >= 4) ? (state ? 255 : 0) : led_brightness[7 - i];
    }
}

int main(void) {
    setup_io();  // Configura os pinos de entrada e saída
    sei();  // Habilita interrupções globais

    while (1) {
        if (estado == 2) {  // Se o jogo terminou (vitória)
            victory_blink();  // Pisca os LEDs do vencedor
        }
        pwm_software();  // Atualiza os LEDs com base no PWM
    }
}
