#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// Definições de pinos
#define JOYSTICK_X 26          // Pino do eixo X do joystick
#define JOYSTICK_Y 27          // Pino do eixo Y do joystick
#define SW 22                  // Pino do botão do joystick
#define BUTTON_A 5             // Pino do botão A
#define LED_RED 13             // Pino do LED Vermelho
#define LED_GREEN 11           // Pino do LED Verde
#define LED_BLUE 12            // Pino do LED Azul
#define I2C_PORT i2c1          // Porta I2C
#define I2C_SDA 14             // Pino SDA do I2C
#define I2C_SCL 15             // Pino SCL do I2C
#define DISPLAY_ADDR 0x3C      // Endereço I2C do display SSD1306
#define DISPLAY_WIDTH 128      // Largura do display
#define DISPLAY_HEIGHT 64      // Altura do display

// Variáveis globais
volatile bool button_sw_pressed = false;  // Flag para o botão do joystick
volatile bool button_a_pressed = false;   // Flag para o botão A
absolute_time_t last_button_sw_time;      // Último tempo do botão do joystick
absolute_time_t last_button_a_time;       // Último tempo do botão A
bool led_green_state = false;             // Estado do LED Verde
bool pwm_enabled = true;                  // Estado do PWM (ligado/desligado)
ssd1306_t display;                        // Estrutura do display SSD1306

// Função para configurar o PWM
void setup_pwm(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, 4095);  // 12 bits de resolução
    pwm_set_enabled(slice, true);
}

// Função para ajustar o brilho do LED via PWM
void set_led_brightness(uint pin, uint16_t value) {
    pwm_set_gpio_level(pin, value);
}

// Função de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    absolute_time_t current_time = get_absolute_time();

    if (gpio == SW && absolute_time_diff_us(last_button_sw_time, current_time) >= 200000) {
        button_sw_pressed = true;
        last_button_sw_time = current_time;
    } else if (gpio == BUTTON_A && absolute_time_diff_us(last_button_a_time, current_time) >= 200000) {
        button_a_pressed = true;
        last_button_a_time = current_time;
    }
}

// Função para inicializar o display
void setup_display() {
    printf("Inicializando I2C...\n");
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o I2C com frequência de 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Configura o pino SDA como I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Configura o pino SCL como I2C
    gpio_pull_up(I2C_SDA); // Habilita pull-up no pino SDA
    gpio_pull_up(I2C_SCL); // Habilita pull-up no pino SCL

    printf("Inicializando display SSD1306...\n");
    ssd1306_init(&display, DISPLAY_WIDTH, DISPLAY_HEIGHT, false, DISPLAY_ADDR, I2C_PORT); // Inicializa o display SSD1306
    ssd1306_config(&display); // Configura o display
    ssd1306_send_data(&display); // Envia os dados para o display
    ssd1306_fill(&display, false); // Limpa o display
    ssd1306_send_data(&display); // Atualiza o display
    printf("Display inicializado e limpo.\n");
}

// Função para desenhar um quadrado no display
void draw_square(int x, int y) {
    ssd1306_fill(&display, false);  // Limpa o display

    // Desenha as linhas do quadrado
    ssd1306_draw_line(&display, x, y, x + 8, y, true);          // Linha superior
    ssd1306_draw_line(&display, x + 8, y, x + 8, y + 8, true);  // Linha direita
    ssd1306_draw_line(&display, x + 8, y + 8, x, y + 8, true);  // Linha inferior
    ssd1306_draw_line(&display, x, y + 8, x, y, true);          // Linha esquerda

    ssd1306_send_data(&display);  // Atualiza o display
}

// Função principal
int main() {
    stdio_init_all(); // Inicializa a comunicação serial
    printf("Iniciando programa...\n");

    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);

    // Configurar GPIOs
    gpio_init(SW);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(SW);
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configurar interrupções
    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Configurar PWM para os LEDs
    setup_pwm(LED_RED);
    //setup_pwm(LED_GREEN);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    setup_pwm(LED_BLUE);

    // Inicializar o display
    setup_display();

    // Posição inicial do quadrado
    int square_x = (DISPLAY_WIDTH / 2) - 4;
    int square_y = (DISPLAY_HEIGHT / 2) - 4;

    while (true) {
        // Ler valores do joystick
        adc_select_input(0);
        uint16_t x_value = adc_read();
        adc_select_input(1);
        uint16_t y_value = adc_read();

        // Controlar LEDs RGB
        if (pwm_enabled) {
            set_led_brightness(LED_RED, abs(x_value - 2048));  // Ajusta o brilho do LED Vermelho
            set_led_brightness(LED_BLUE, abs(y_value - 2048));  // Ajusta o brilho do LED Azul
        }

        // Atualizar posição do quadrado no display
        square_x = (x_value * (DISPLAY_WIDTH - 8)) / 4095;
        square_y = (y_value * (DISPLAY_HEIGHT - 8)) / 4095;
        draw_square(square_x, square_y);

        // Tratar interrupções dos botões
        if (button_sw_pressed) {
            button_sw_pressed = false;
            led_green_state = !led_green_state; // Alterna o estado do LED Verde
            gpio_put(LED_GREEN, led_green_state);  // Atualiza o LED Verde
        }
        if (button_a_pressed) {
            button_a_pressed = false;
            pwm_enabled = !pwm_enabled;  // Alterna o estado do PWM
            if (!pwm_enabled) {
                set_led_brightness(LED_RED, 0);  // Desliga o LED Vermelho
                set_led_brightness(LED_BLUE, 0);  // Desliga o LED Azul
            }
        }

        sleep_ms(10);  // Aguarda 10 ms antes da próxima iteração
    }

    return 0;
}
