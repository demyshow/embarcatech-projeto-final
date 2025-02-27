/*
 * 🎮 Projeto de Sistema Embarcado - Projeto Final
 * Autor: Edemir de Carvalho Rodrigues
 * Programa de Residência: Embarcatech
 *
 * Objetivo do Jogo: Controlar um personagem (representado por um quadrado maior) que deve fugir de
 * mísseis inimigos (representados por quadrados menores) que aparecem no lado esquerdo da tela.
 * O jogador acumula pontos ao evitar colisões com os inimigos e vence ao atingir o nível 10 de pontuação.
 *
 * O jogo termina quando o jogador colide com um míssil inimigo ou atinge a pontuação máxima.
 *
 * O jogo possui 5 níveis de dificuldade, com a velocidade dos inimigos aumentando progressivamente conforme o nível avança.
 *
 * Controles:
 * - Joystick: Movimenta o jogador.
 * - Botão A (Start): Inicia ou desliga o jogo.
 * - Botão B (Reset): Reinicia o jogo.
 * - Botão do Joystick (Pause): Pausa ou retoma o jogo.
 *
 * Feedback Visual e Sonoro:
 * - LED Vermelho: Acende em caso de derrota.
 * - LED Verde: Acende em caso de vitória.
 * - LED Rosa: Acende ao reiniciar o jogo.
 * - LED Amarelo: Acende quando o jogo está pausado.
 * - Buzzer: Reproduz sons de vitória (fogos de artifício) e derrota (explosão).
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "lib/ssd1306.h"

// Definições de pinos e constantes
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define DISPLAY_ADDR 0x3C

#define JOYSTICK_X 27
#define JOYSTICK_Y 26
#define JOYSTICK_BUTTON 22  // Botão do joystick no eixo Z
#define LED_RED 13          // Vermelho
#define LED_GREEN 11        // Verde
#define LED_BLUE 12         // Azul
#define BUZZER 21
#define BUTTON_RESET 6      // Tecla "B"
#define BUTTON_START 5      // Tecla "A"

#define WIDTH 128
#define HEIGHT 64
#define JOYSTICK_CENTER_X 1929
#define JOYSTICK_CENTER_Y 2019
#define DEADZONE 100

// Variáveis globais
ssd1306_t ssd;
int score = 0;
bool game_running = false;
bool game_paused = false;

typedef struct {
    int x, y;
    int speed;
} Enemy;

Enemy enemy;
int player_x = WIDTH / 2 - 4, player_y = HEIGHT / 2 - 4; // Centraliza o jogador

/*
 * Função: setup_pwm
 * Descrição: Configura o PWM para um pino específico.
 * Parâmetros:
 *   - pin: Pino a ser configurado.
 */
void setup_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, 4095);
    pwm_set_enabled(slice, true);
    pwm_set_gpio_level(pin, 0);
}

/*
 * Função: play_tone
 * Descrição: Toca um tom no buzzer com uma frequência e duração específicas.
 * Parâmetros:
 *   - pin: Pino do buzzer.
 *   - frequency: Frequência do tom.
 *   - duration: Duração do tom em milissegundos.
 */
void play_tone(uint pin, uint16_t frequency, uint16_t duration) {
    uint slice = pwm_gpio_to_slice_num(pin);
    uint channel = pwm_gpio_to_channel(pin);
    uint32_t clock = 125000000;
    uint16_t divider = 64;
    uint16_t wrap = (clock / divider) / frequency;

    pwm_set_clkdiv(slice, divider);
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, channel, wrap / 2);
    pwm_set_enabled(slice, true);

    sleep_ms(duration);
    pwm_set_enabled(slice, false);
}

/*
 * Função: spawn_enemy
 * Descrição: Gera um novo inimigo em uma posição aleatória no topo da tela.
 */
void spawn_enemy() {
    enemy.x = rand() % (WIDTH - 4);
    enemy.y = 0;
    enemy.speed = 1 + (score / 2);
    if (enemy.speed > 10) enemy.speed = 10;
}

/*
 * Função: check_collision
 * Descrição: Verifica se há colisão entre o jogador e o inimigo.
 * Retorno: Verdadeiro se houver colisão, falso caso contrário.
 */
bool check_collision() {
    // Verifica se há sobreposição entre o jogador e o inimigo
    bool collision_x = (player_x < enemy.x + 4) && (player_x + 8 > enemy.x);
    bool collision_y = (player_y < enemy.y + 4) && (player_y + 8 > enemy.y);
    return collision_x && collision_y;
}

/*
 * Função: reset_game
 * Descrição: Reinicia o jogo, zerando a pontuação e reposicionando o jogador e o inimigo.
 */
void reset_game() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Reiniciando...", 20, 28);
    ssd1306_send_data(&ssd);

    // LED rosa (mistura de vermelho e azul)
    pwm_set_gpio_level(LED_RED, 4095);    // Acende o LED vermelho no máximo
    pwm_set_gpio_level(LED_GREEN, 0);     // Desliga o LED verde
     pwm_set_gpio_level(LED_BLUE, 2048);   // Acende o LED azul pela metade

    sleep_ms(1000);

    // Zera todas as variáveis do jogo
    score = 0;
    game_running = true;
    game_paused = false;
    player_x = WIDTH / 2 - 4; // Centraliza o jogador
    player_y = HEIGHT / 2 - 4;

    // LED azul (início do jogo)
    pwm_set_gpio_level(LED_RED, 0);      // Desliga o LED vermelho
    pwm_set_gpio_level(LED_GREEN, 0);    // Desliga o LED verde
    pwm_set_gpio_level(LED_BLUE, 4095);  // Acende o LED azul

    spawn_enemy();

    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Jogo iniciado!", 10, 28);
    ssd1306_send_data(&ssd);
}

/*
 * Função: start_game
 * Descrição: Inicia o jogo sem exibir a mensagem "Reiniciando...".
 */
void start_game() {
    // Inicia o jogo sem exibir a mensagem "Reiniciando..."
    score = 0;
    game_running = true;
    game_paused = false;
    player_x = WIDTH / 2 - 4; // Centraliza o jogador
    player_y = HEIGHT / 2 - 4;

    // LED azul (início do jogo)
    pwm_set_gpio_level(LED_RED, 0);      // Desliga o LED vermelho
    pwm_set_gpio_level(LED_GREEN, 0);    // Desliga o LED verde
    pwm_set_gpio_level(LED_BLUE, 4095);  // Acende o LED azul

    spawn_enemy();

    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Jogo iniciado!", 10, 28);
    ssd1306_send_data(&ssd);
}

/*
 * Função: toggle_game
 * Descrição: Liga ou desliga o jogo, alternando entre os estados de execução e desligado.
 */
void toggle_game() {
    if (game_running) {
        game_running = false;
        pwm_set_gpio_level(LED_BLUE, 0);  // Desliga o LED azul
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, "Jogo desligado!", 10, 28);
        ssd1306_send_data(&ssd);
    } else {
        // Inicia o jogo diretamente, sem reiniciar
        start_game();
    }
    sleep_ms(500);
}

/*
 * Função: toggle_pause
 * Descrição: Pausa ou retoma o jogo, alternando entre os estados de pausado e em execução.
 */
void toggle_pause() {
    if (game_running) {
        game_paused = !game_paused;
        if (game_paused) {
            // LED amarelo (mistura de vermelho e verde, com menos vermelho)
            pwm_set_gpio_level(LED_RED, 2048);    // Acende o LED vermelho pela metade
            pwm_set_gpio_level(LED_GREEN, 4095);  // Acende o LED verde no máximo
            pwm_set_gpio_level(LED_BLUE, 0);      // Desliga o LED azul
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "Jogo Pausado", 20, 28);
            ssd1306_send_data(&ssd);
        } else {
            // Volta para o LED azul (jogo retomado)
            pwm_set_gpio_level(LED_RED, 0);      // Desliga o LED vermelho
            pwm_set_gpio_level(LED_GREEN, 0);    // Desliga o LED verde
            pwm_set_gpio_level(LED_BLUE, 4095);  // Acende o LED azul
        }
    }
    sleep_ms(300);
}

/*
 * Função: main
 * Descrição: Função principal do programa, responsável por inicializar o hardware e executar o loop do jogo.
 */
int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);

    setup_pwm(LED_RED);
    setup_pwm(LED_GREEN);
    setup_pwm(LED_BLUE);
    setup_pwm(BUZZER);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(BUTTON_RESET);
    gpio_set_dir(BUTTON_RESET, GPIO_IN);
    gpio_pull_up(BUTTON_RESET);

    gpio_init(BUTTON_START);
    gpio_set_dir(BUTTON_START, GPIO_IN);
    gpio_pull_up(BUTTON_START);

    gpio_init(JOYSTICK_BUTTON);
    gpio_set_dir(JOYSTICK_BUTTON, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, DISPLAY_ADDR, I2C_PORT);
    ssd1306_config(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "Pressione A", 10, 28);  // Primeira linha
    ssd1306_draw_string(&ssd, "para iniciar", 10, 40);  // Segunda linha
    ssd1306_send_data(&ssd);

    spawn_enemy();

    while (true) {
        if (gpio_get(BUTTON_RESET) == 0) {
            reset_game();
        }
        if (gpio_get(BUTTON_START) == 0) {
            toggle_game();
        }
        if (gpio_get(JOYSTICK_BUTTON) == 0) {
            toggle_pause();
        }

        if (!game_running || game_paused) {
            sleep_ms(200);
            continue;
        }

        adc_select_input(0);
        uint16_t x_val = adc_read();
        adc_select_input(1);
        uint16_t y_val = adc_read();

        int adjusted_x = x_val - JOYSTICK_CENTER_X;
        int adjusted_y = y_val - JOYSTICK_CENTER_Y;

        if (abs(adjusted_x) > DEADZONE)
            player_x -= (adjusted_x * 5) / 2048;
        if (abs(adjusted_y) > DEADZONE)
            player_y += (adjusted_y * 5) / 2048;

        // Limita o movimento do jogador dentro da tela
        if (player_x < 0) player_x = 0;
        if (player_x > WIDTH - 8) player_x = WIDTH - 8;
        if (player_y < 0) player_y = 0;
        if (player_y > HEIGHT - 8) player_y = HEIGHT - 8;

        enemy.y += enemy.speed;

        if (enemy.y > HEIGHT) {
            spawn_enemy();
            score++;
            if (score >= 10) {
                game_running = false;

                // Acende o LED verde e desliga os outros LEDs
                pwm_set_gpio_level(LED_RED, 0);      // Desliga o LED vermelho
                pwm_set_gpio_level(LED_GREEN, 4095); // Acende o LED verde
                pwm_set_gpio_level(LED_BLUE, 0);     // Desliga o LED azul

                // Exibe a mensagem de vitória
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "Voce venceu!", 20, 28);
                ssd1306_send_data(&ssd);

                play_tone(BUZZER, 1000, 500); // Toca um som de vitória
                sleep_ms(2000);  // Pausa para garantir que o LED verde permaneça aceso

                // Aguarda o jogador pressionar o botão de reset ou start para reiniciar
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "Pressione", 20, 28);
                ssd1306_draw_string(&ssd, "A ou B para", 20, 40);
                ssd1306_draw_string(&ssd, "reiniciar", 20, 52);
                ssd1306_send_data(&ssd);

                while (true) {
                    if (gpio_get(BUTTON_RESET) == 0 || gpio_get(BUTTON_START) == 0) {
                        reset_game();  // Reinicia o jogo
                        break;
                    }
                    sleep_ms(100);
                }
            }
        }

        ssd1306_fill(&ssd, false);
        ssd1306_rect(&ssd, enemy.x, enemy.y, 4, 4, true, true);
        ssd1306_rect(&ssd, player_x, player_y, 8, 8, true, true);

        // Exibe pontuação e velocidade lado a lado
        char score_text[20];
        sprintf(score_text, "Pontos:%d", score);
        ssd1306_draw_string(&ssd, score_text, 0, 0);  // Pontuação no canto superior esquerdo

        char speed_text[20];
        sprintf(speed_text, "Vel:%d", enemy.speed);
        ssd1306_draw_string(&ssd, speed_text, 70, 0);  // Velocidade ao lado da pontuação

        ssd1306_send_data(&ssd);

        if (check_collision()) {
            game_running = false;
            pwm_set_gpio_level(LED_RED, 4095);  // Acende o LED vermelho
            pwm_set_gpio_level(LED_GREEN, 0);   // Desliga o LED verde
            pwm_set_gpio_level(LED_BLUE, 0);    // Desliga o LED azul
            ssd1306_fill(&ssd, false);
            ssd1306_draw_string(&ssd, "Perdeu...", 40, 28);
            ssd1306_draw_string(&ssd, "Aguardando", 30, 40);  // Primeira linha
            ssd1306_draw_string(&ssd, "reiniciar", 30, 50);   // Segunda linha (y = 50)
            ssd1306_send_data(&ssd);
            play_tone(BUZZER, 500, 500);

            // Aguarda o jogador pressionar o botão de reset ou start
            while (true) {
                if (gpio_get(BUTTON_RESET) == 0 || gpio_get(BUTTON_START) == 0) {
                    reset_game();
                    break;
                }
                sleep_ms(100);
            }
        }

        sleep_ms(20);
    }
}