#include <main.h>

uint16_t leds[3], color[3], nextColor[3];
long colorStep[3];
int random_value = 0;
int random_value_corrected = 0;
enum distr_name current_distr = GREEN;

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

void setLedsValue(int input_value)
{
    leds[0] = lights[input_value];
    leds[1] = lights[(input_value+120) % 360];
    leds[2] = lights[(input_value+240) % 360];
}

void searchColor()
{
  Serial.println("Search start");
    for (int i=0; i<360; ++i)
    {
        setLedsValue(i);
        Serial.print(i);
        Serial.print("   ");
        Serial.print(leds[0]);
        Serial.print(":");
        Serial.print(leds[1]);
        Serial.print(":");
        Serial.println(leds[2]);
        lit_leds(leds[0], leds[1], leds[2]);
        delay(50);
        while (!digitalRead(button))
        {
            delay(1);
        }
    }
}

int getRandomDistr()
{
    random_value = distr(random_normal_gen);
    // Serial.print("random_value=");
    // Serial.println(random_value);
    if (random_value < 0) random_value_corrected = 360 + (random_value % 360);
    else if (random_value > 360) random_value_corrected = random_value % 360;
    else random_value_corrected = random_value;
    // Serial.print("random_value_corrected=");
    // Serial.println(random_value_corrected);
    return random_value_corrected;
}

void runLedRand(int param)
{
    for (int i = 0; i < param; ++i)
    {
        int generated_rnd_val = getRandomDistr();
        setLedsValue(generated_rnd_val);
        lit_leds(leds[0], leds[1], leds[2]);
        delay(10);
    }
}

void runDistributions(int count)
{
    distr.param(distr_green);
    runLedRand(count);
    distr.param(distr_yellow);
    runLedRand(count);
    distr.param(distr_orange);
    runLedRand(count);
    distr.param(distr_red);
    runLedRand(count);
    distr.param(distr_purple);
    runLedRand(count);
    distr.param(distr_blue);
    runLedRand(count);
    distr.param(distr_light_blue);
    runLedRand(count);
}

void updateDistribution()
{
    switch (current_distr)
    {
        case GREEN:
            distr.param(distr_green);
        break;
        case YELLOW:
            distr.param(distr_yellow);
        break;
        case ORANGE:
            distr.param(distr_orange);
        break;
        case RED:
            distr.param(distr_red);
        break;
        case PURPLE:
            distr.param(distr_purple);
        break;
        case BLUE:
            distr.param(distr_blue);    
        break;    
        case LIGHT_BLUE:
            distr.param(distr_light_blue);      
        break; 
    }
}

void setLedsNextValue(int input_value)
{
    nextColor[0] = lights[input_value]  << 8;
    nextColor[1] = lights[(input_value+120) % 360] << 8;
    nextColor[2] = lights[(input_value+240) % 360] << 8;
}

void process2nextColor(enum distr_name next_colour_value)
{
    switch (next_colour_value)
    {
        case GREEN:
            setLedsNextValue(GREEN_POINT);
        break;
        case YELLOW:
            setLedsNextValue(YELLOW_POINT);
        break;
        case ORANGE:
            setLedsNextValue(ORANGE_POINT);
        break;
        case RED:
            setLedsNextValue(RED_POINT);
        break;
        case PURPLE:
            setLedsNextValue(PURPLE_POINT);
        break;
        case BLUE:
            setLedsNextValue(BLUE_POINT);   
        break;    
        case LIGHT_BLUE:
            setLedsNextValue(LIGHT_BLUE_POINT);     
        break; 
    }

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

void shiftColourRight(bool right)
{
    switch (current_distr)
    {
        case GREEN:
            if (right) current_distr = YELLOW;
            else current_distr = LIGHT_BLUE;
        break;
        case YELLOW:
            if (right) current_distr = ORANGE;
            else current_distr = GREEN;
        break;
        case ORANGE:
            if (right) current_distr = RED;
            else current_distr = YELLOW;
        break;
        case RED:
            if (right) current_distr = PURPLE;
            else current_distr = ORANGE;
        break;
        case PURPLE:
            if (right) current_distr = BLUE;
            else current_distr = RED;
        break;
        case BLUE:
            if (right) current_distr = LIGHT_BLUE; 
            else current_distr = PURPLE;
        break;    
        case LIGHT_BLUE:
            if (right) current_distr = GREEN; 
            else current_distr = BLUE;    
        break; 
    }
    process2nextColor(current_distr);
}

void litCurrentApproxLight()
{
    switch (current_distr)
    {
        case GREEN:
            setLedsValue(GREEN_POINT);
        break;
        case YELLOW:
            setLedsValue(YELLOW_POINT);
        break;
        case ORANGE:
            setLedsValue(ORANGE_POINT);
        break;
        case RED:
            setLedsValue(RED_POINT);
        break;
        case PURPLE:
            setLedsValue(PURPLE_POINT);
        break;
        case BLUE:
            setLedsValue(BLUE_POINT);   
        break;    
        case LIGHT_BLUE:
            setLedsValue(LIGHT_BLUE_POINT);     
        break; 
    }
    lit_leds(leds[0], leds[1], leds[2]);
}

void runLedRand_check(int param)
{
    int right_shift = 0; 
    int left_shift = 0;
    const float kf = 2.5;
    const byte shift_limit = 2;
    random_normal_gen.seed(esp_random());
    for (int i = 0; i < param; ++i)
    {
        int generated_rnd_val = getRandomDistr();
        // setLedsValue(generated_rnd_val);
        // lit_leds(leds[0], leds[1], leds[2]);
        // litCurrentApproxLight();
        switch (current_distr)
        {
            case GREEN:
                // Serial.print("generated_rnd_val=");
                // Serial.println(generated_rnd_val);
                if (generated_rnd_val < (360-GREEN_RANGE*kf) && generated_rnd_val > 360/2) ++left_shift;
                else if (generated_rnd_val > GREEN_POINT+GREEN_RANGE*kf && generated_rnd_val < 360/2) ++right_shift;
            break;
            case YELLOW:
                if (generated_rnd_val < (YELLOW_POINT-YELLOW_RANGE*kf)) ++left_shift;
                else if (generated_rnd_val > YELLOW_POINT+YELLOW_RANGE*kf) ++right_shift;
            break;
            case ORANGE:
                if (generated_rnd_val < (ORANGE_POINT-ORANGE_RANGE*kf)) ++left_shift;
                else if (generated_rnd_val > ORANGE_POINT+ORANGE_RANGE*kf) ++right_shift;            
            break;
            case RED:
                if (generated_rnd_val < (RED_POINT-RED_RANGE*kf)) ++left_shift;
                else if (generated_rnd_val > RED_POINT+RED_RANGE*kf) ++right_shift;
            break;
            case PURPLE:
                if (generated_rnd_val < (PURPLE_POINT-PURPLE_RANGE*kf)) ++left_shift;
                else if (generated_rnd_val > PURPLE_POINT+PURPLE_RANGE*kf) ++right_shift;
            break;
            case BLUE:
                if (generated_rnd_val < (BLUE_POINT-BLUE_RANGE*kf)) ++left_shift;
                else if (generated_rnd_val > BLUE_POINT+BLUE_RANGE*kf) ++right_shift;            
            break;    
            case LIGHT_BLUE:
                if (generated_rnd_val < (LIGHT_BLUE_POINT-LIGHT_BLUE_RANGE*kf)) ++left_shift;
                else if (generated_rnd_val > LIGHT_BLUE_POINT+LIGHT_BLUE_RANGE*kf) ++right_shift;            
            break; 
        }
        delay(10);
    }
    Serial.println("------------");
    Serial.print("LIGHT=");
    Serial.println(current_distr);
    Serial.print("right_shift=");
    Serial.println(right_shift);
    Serial.print("left_shift=");
    Serial.println(left_shift);
    
    if (left_shift>=shift_limit && right_shift>=shift_limit) 
    {
        Serial.println("                  Both worth shifting! Shift randomly!");
        if (esp_random()%2) 
        {
            shiftColourRight(true);
            Serial.println("                  Shift RIGHT >>>>");    
        }
        else 
        {
            shiftColourRight(false);
            Serial.println("                  <<<< Shift LEFT");
        }
    }
    else if (left_shift>=shift_limit)
    {
        Serial.println("                  <<<< Shift LEFT");
        shiftColourRight(false);
    } 
    else if (right_shift>=shift_limit)
    {
        Serial.println("                  Shift RIGHT >>>>");
        shiftColourRight(true);
    }
    updateDistribution(); 
}

void loop() 
{
    runLedRand_check(100);
    // int count = 100;

    // current_distr = GREEN;
    // distr.param(distr_green);
    // runLedRand_check(count);
    
    // current_distr = YELLOW;
    // distr.param(distr_yellow);
    // runLedRand_check(count);

    // current_distr = ORANGE;
    // distr.param(distr_orange);
    // runLedRand_check(count);

    // current_distr = RED;
    // distr.param(distr_red);
    // runLedRand_check(count);

    // current_distr = PURPLE;
    // distr.param(distr_purple);
    // runLedRand_check(count);

    // current_distr = BLUE;
    // distr.param(distr_blue);
    // runLedRand_check(count);

    // current_distr = LIGHT_BLUE;
    // distr.param(distr_light_blue);
    // runLedRand_check(count);


    // if (esp_random() % 2)
    // {
    //     setNextColorSine();
    //     delay(3000);
    // }
    // else
    // {
    //     randomColour();
    // }
    
    // searchColor();
    // runDistributions(700);



    // int delay_time = static_cast<int>(distribution_time(generator));
    // if (delay_time > 0) delay(delay_time);
}