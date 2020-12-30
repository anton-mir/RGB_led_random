#include <main.h>

uint16_t leds[3], color[3], nextColor[3];
long colorStep[3];

void setNextColorSine()
{
    byte random_led_number, second_led_number, zero_led_number, random_led_power, second_led_power;

    random_led_number = esp_random() % 3;
    second_led_number = (random_led_number + 1) % 3;
    zero_led_number = (random_led_number + 2) % 3;

    random_led_power=static_cast<byte>(distribution(generator));
    if (random_led_power < 0) random_led_power = 0;
    else if (random_led_power > 255) random_led_power = 255;

    second_led_power = (random_led_power + random(0,100)) % 255;

    nextColor[random_led_number]=lights[random_led_power] << 8;
    nextColor[second_led_number]=lights[second_led_power] << 8;
    nextColor[zero_led_number]=0;

    for (byte k=0; k<3; k++)
    {
        colorStep[k]=((long)nextColor[k] - color[k])/255;
    } 

    for (byte k=0; k<120;k++)
        {
            lit_leds((color[0]+colorStep[0]*lights[k])>>8,
            (color[1]+colorStep[1]*lights[k])>>8,
            (color[2]+colorStep[2]*lights[k])>>8);
            delay(10);
        }

    for (byte k=0; k<3; k++) 
    {
        color[k] = nextColor[k];
    }

    lit_leds(color[0]>>8, color[1]>>8, color[2]>>8);
}

void randomColour()
{
    byte random_led_number, second_led_number, zero_led_number, random_led_power, second_led_power;

    random_led_number = esp_random() % 3;
    second_led_number = (random_led_number + 1) % 3;
    zero_led_number = (random_led_number + 2) % 3;

    random_led_power=static_cast<byte>(distribution(generator));
    if (random_led_power < 0) random_led_power = 0;
    else if (random_led_power > 255) random_led_power = 255;

    second_led_power = (random_led_power + random(0,100)) % 255;

    leds[random_led_number]=lights[random_led_power];
    leds[second_led_number]=lights[second_led_power];
    leds[zero_led_number]=0;

    lit_leds(leds[0], leds[1], leds[2]);
}

void loop() 
{
    if (esp_random() % 2)
    {
        setNextColorSine();
    }
    else
    {
        randomColour();
    }
    

    int delay_time = static_cast<int>(distribution_time(generator));
    if (delay_time > 0) delay(delay_time);
}