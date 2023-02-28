#include <iostream>
#include <vector>
#include <cmath>
#include <pigpio.h>

// 灯带的LED数量和数据引脚
#define LED_COUNT 60
#define LED_PIN 18
/*
 * 通过将每个LED的颜色值转换为三个8位颜色分量，
 * 并使用PiGPIO库的gpioWaveAddGeneric函数将颜色分量写入到GPIO口上，
 * 最终实现对WS2812B灯带的控制。
 * 注意，在使用PiGPIO库时需要先初始化GPIO口，
 * 通过gpioCfgClock函数设置GPIO时钟频率，
 * 然后再调用gpioInitial
 * */


// 用于存储每个LED的颜色值
std::vector<unsigned char> led_colors(LED_COUNT * 3);

// 将一个32位颜色值转换为三个8位颜色分量
void split_color(unsigned int color, unsigned char &red, unsigned char &green, unsigned char &blue) {
    red = (color >> 16) & 0xFF;
    green = (color >> 8) & 0xFF;
    blue = color & 0xFF;
}

// 将三个8位颜色分量合并为一个32位颜色值
unsigned int merge_color(unsigned char red, unsigned char green, unsigned char blue) {
    return (red << 16) | (green << 8) | blue;
}

// 初始化GPIO口和灯带
void init_ws2812b() {
    gpioCfgClock(5, 1, 0);
    gpioInitialise();
}

// 关闭GPIO口和灯带
void close_ws2812b() {
    gpioTerminate();
}

// 设置每个LED的颜色
void set_led_colors(unsigned int color) {
    unsigned char red, green, blue;
    split_color(color, red, green, blue);
    for (int i = 0; i < LED_COUNT; ++i) {
        led_colors[i * 3] = green;
        led_colors[i * 3 + 1] = red;
        led_colors[i * 3 + 2] = blue;
    }
}

// 将颜色值写入到灯带上
void write_ws2812b() {
    gpioSetMode(LED_PIN, PI_OUTPUT);
    gpioWaveAddGeneric(LED_COUNT * 24, (gpioPulse_t *) led_colors.data());
    gpioWaveTxSend(gpioWaveCreate(), PI_WAVE_MODE_ONE_SHOT);
}

int main(int argc, char **argv) {
    // 初始化GPIO口和灯带
    init_ws2812b();

    // 设置每个LED的颜色为红色
    set_led_colors(0xFF0000);

    // 将颜色值写入到灯带上
    write_ws2812b();

    // 关闭GPIO口和灯带
    close_ws2812b();
}
