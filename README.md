# Tarefa 10/02 - Controle de Joystick, LEDs e Display SSD1306

Este projeto utiliza o Raspberry Pi Pico para controlar LEDs RGB, exibir um quadrado em um display SSD1306 e ler valores de um joystick analógico. O sistema também possui dois botões para interagir com o controle de LEDs e PWM.

## Requisitos Atendidos

### 1) Configuração dos LEDs RGB com PWM
✅ Os LEDs Vermelho e Azul têm seu brilho ajustado conforme a posição do joystick nos eixos X e Y.

### 2) Controle do Display SSD1306
✅ O código desenha um quadrado no display e sua posição é controlada pelos valores do joystick.
✅ O display é atualizado continuamente com a nova posição do quadrado.

### 3) Interatividade com os Botões
✅ **Botão do Joystick (SW):** Alterna o estado do LED Verde (ligado/desligado).
✅ **Botão A:** Ativa/desativa o controle PWM dos LEDs. Ao desativar, os LEDs Vermelho e Azul são desligados.

### 4) Leitura do Joystick
✅ O código lê os valores dos eixos X e Y do joystick e ajusta a posição do quadrado no display com base nesses valores.
✅ A intensidade do brilho dos LEDs é ajustada de acordo com os valores do joystick.

## Como Usar
1. Clonar o repositório do GitHub.
2. Compilar e carregar o código no Raspberry Pi Pico.
3. Executar o programa para interagir com os LEDs e ver a movimentação do quadrado no display.

## Bibliotecas Utilizadas
- `stdio.h` – Para entrada e saída de dados.
- `pico/stdlib.h` – Para uso das funções básicas do Raspberry Pi Pico.
- `hardware/adc.h` – Para leitura dos valores analógicos do joystick.
- `hardware/pwm.h` – Para controle do PWM dos LEDs.
- `hardware/i2c.h` – Para comunicação com o display SSD1306.
- `ssd1306.h` – Para controle do display SSD1306.

## Vídeo Demonstrativo
[https://youtu.be/K0lRynUN0FU]
