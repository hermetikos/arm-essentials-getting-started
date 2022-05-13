#include "mbed.h"

#define BUTTON_1 p5
#define BUTTON_2 p6
#define BUTTON_3 p7
#define BUTTON_4 p8

#define RED_LED p9
#define YELLOW_LED p10
#define BLUE_LED p11


// Define the Input pins
	//Write your code here
DigitalIn button1(BUTTON_1);
DigitalIn button2(BUTTON_2);
DigitalIn button3(BUTTON_3);
DigitalIn button4(BUTTON_4);

//Define the Output pins
	//Write your code here
DigitalOut ledRed(RED_LED);
DigitalOut ledYellow(YELLOW_LED);
DigitalOut ledBlue(BLUE_LED);

//Define Input/Output buses
	//Write your code here
// OutBus leds()

void ControlLED_DigitalIO ();
void ControlLED_BusIO ();


int main()
{
    while(1) {
        ControlLED_DigitalIO ();
        // ControlLED_BusIO ();
        wait(0.25);
    }
}

void ControlLED_DigitalIO (){
          //Write your code here
    int val = button1 | (button2 << 1) | (button3 << 2) | (button4 << 3);
    // int val = 6;
    printf("val %i\n", val);
    if (val <= 3) {
        ledRed = 1;
        ledYellow = 0;
        ledBlue = 0;
    }
    else if (val == 4 || val == 5) {
        // 
        ledRed = 0;
        ledYellow = 1;
        ledBlue = 0;
    }
    else if (val == 6 || val == 7) {
        // 
        ledRed = 1;
        ledYellow = 1;
        ledBlue = 0;
    }
    else if (8 <= val && val <= 11) {
        ledRed = 0;
        ledYellow = 1;
        ledBlue = 1;
    }
    else if (12 <= val && val <= 14) {
        // 
        ledRed = 0;
        ledYellow = 0;
        ledBlue = 1;
    }
    else {
        ledRed = 1;
        ledYellow = 1;
        ledBlue = 1;
    }
    
}

void ControlLED_BusIO (){
    //Write your code here
}