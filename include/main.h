// https://www.instructables.com/How-to-Make-Proper-Rainbow-and-Random-Colors-With-
#include <Arduino.h>
#include <analogWrite.h>
#include <random>

#define GREEN_POINT 0
#define YELLOW_POINT 52 
#define ORANGE_POINT 71
#define RED_POINT 117
#define PURPLE_POINT 180
#define BLUE_POINT 236
#define LIGHT_BLUE_POINT 288

// #define GREEN_RANGE 25
// #define YELLOW_RANGE 10 
// #define ORANGE_RANGE 10
// #define RED_RANGE 25
// #define PURPLE_RANGE 25
// #define BLUE_RANGE 20
// #define LIGHT_BLUE_RANGE 25

#define GREEN_RANGE 10
#define YELLOW_RANGE 10 
#define ORANGE_RANGE 10
#define RED_RANGE 10
#define PURPLE_RANGE 10
#define BLUE_RANGE 10
#define LIGHT_BLUE_RANGE 10

std::default_random_engine generator;
std::normal_distribution<double> distribution(/*mean=*/127.0, /*stddev=*/127.0);
std::normal_distribution<double> distribution_time(/*mean=*/3000.0, /*stddev=*/3000.0);

std::default_random_engine random_normal_gen;
std::normal_distribution<> distr(0, 0);
std::normal_distribution<>::param_type distr_green(GREEN_POINT, GREEN_RANGE);
std::normal_distribution<>::param_type distr_yellow(YELLOW_POINT, YELLOW_RANGE);
std::normal_distribution<>::param_type distr_orange(ORANGE_POINT, ORANGE_RANGE);
std::normal_distribution<>::param_type distr_red(RED_POINT, RED_RANGE);
std::normal_distribution<>::param_type distr_purple(PURPLE_POINT, PURPLE_RANGE);
std::normal_distribution<>::param_type distr_blue(BLUE_POINT, BLUE_RANGE);
std::normal_distribution<>::param_type distr_light_blue(LIGHT_BLUE_POINT, LIGHT_BLUE_RANGE);

#define red_pin 21 // 16pin
#define green_pin 13 // 17pin
#define blue_pin 12 // 4pin
#define button 0

enum distr_name
{
  GREEN = 1,
  YELLOW,
  ORANGE,
  RED,
  PURPLE,
  BLUE,
  LIGHT_BLUE
};

const uint8_t lights[360]={
  0,   0,   0,   0,   0,   1,   1,   2, 
  2,   3,   4,   5,   6,   7,   8,   9, 
 11,  12,  13,  15,  17,  18,  20,  22, 
 24,  26,  28,  30,  32,  35,  37,  39, 
 42,  44,  47,  49,  52,  55,  58,  60, 
 63,  66,  69,  72,  75,  78,  81,  85, 
 88,  91,  94,  97, 101, 104, 107, 111, 
114, 117, 121, 124, 127, 131, 134, 137, 
141, 144, 147, 150, 154, 157, 160, 163, 
167, 170, 173, 176, 179, 182, 185, 188, 
191, 194, 197, 200, 202, 205, 208, 210, 
213, 215, 217, 220, 222, 224, 226, 229, 
231, 232, 234, 236, 238, 239, 241, 242, 
244, 245, 246, 248, 249, 250, 251, 251, 
252, 253, 253, 254, 254, 255, 255, 255, 
255, 255, 255, 255, 254, 254, 253, 253, 
252, 251, 251, 250, 249, 248, 246, 245, 
244, 242, 241, 239, 238, 236, 234, 232, 
231, 229, 226, 224, 222, 220, 217, 215, 
213, 210, 208, 205, 202, 200, 197, 194, 
191, 188, 185, 182, 179, 176, 173, 170, 
167, 163, 160, 157, 154, 150, 147, 144, 
141, 137, 134, 131, 127, 124, 121, 117, 
114, 111, 107, 104, 101,  97,  94,  91, 
 88,  85,  81,  78,  75,  72,  69,  66, 
 63,  60,  58,  55,  52,  49,  47,  44, 
 42,  39,  37,  35,  32,  30,  28,  26, 
 24,  22,  20,  18,  17,  15,  13,  12, 
 11,   9,   8,   7,   6,   5,   4,   3, 
  2,   2,   1,   1,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0};

void setup() 
{
  pinMode(button, INPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT); 
  Serial.begin(115200);
  unsigned int seed = esp_random();
  generator.seed(seed);
  seed = esp_random();
  random_normal_gen.seed(seed);
  // distr.param(distr_green);
  // distr.param(distr_red);
  // distr.param(distr_blue);
  distr.param(distr_green);
  // distr.param(distr_light_blue);
  // distr.param(distr_orange);
  // distr.param(distr_purple);
}

void lit_leds(uint8_t red, uint8_t green, uint8_t blue)
{
// this code is for common anode LEDs. If you use common cathode ones,
// remove the '255-' bits.
  analogWrite(red_pin, 255-red);
  analogWrite(green_pin, 255-green);
  analogWrite(blue_pin, 255-blue);
}