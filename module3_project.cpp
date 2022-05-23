#include "mbed.h"

#define START_BUTTON p5
#define STOP_BUTTON p6
#define GUARD_SENSOR p7
#define TEMP_SENSOR p8

#define STANDBY_STATE_LED LED1
#define MOTOR_RUNNING_LED LED2
#define GUARD_OPENED_WARNING_LED LED3
#define EXCESS_TEMP_WARNING_LED LED4


// Define the Input pins
	//Write your code here
// DigitalIn button1(START_BUTTON);
DigitalIn startButton(START_BUTTON);
DigitalIn stopButton(STOP_BUTTON);
DigitalIn guardSensor(GUARD_SENSOR);
DigitalIn tempSensor(TEMP_SENSOR);

//Define the Output pins
	//Write your code here
// DigitalOut ledRed(RED_LED);
DigitalOut standbyStateLed(STANDBY_STATE_LED);
DigitalOut motorRunningLed(MOTOR_RUNNING_LED);
DigitalOut guardOpenWarningLed(GUARD_OPENED_WARNING_LED);
DigitalOut tempWarningLed(EXCESS_TEMP_WARNING_LED);

//Define Input/Output buses
	//Write your code here
// BusIn buttonBus(START_BUTTON, STOP_BUTTON, BUTTON_3, BUTTON_4);
// BusOut leds(BLUE_LED, YELLOW_LED, RED_LED);
// BusIn userInput(START_BUTTON, STOP_BUTTON);
// BusIn sensorInput(GUARD_SENSOR, TEMP_SENSOR);

int getUserInput();
int getGuardSensorInput();
int getTempSensorInput();

// motor status
// STANDBY: STANDBY to run,
// running: motor is running
// off: motor is off
enum MotorStatus {
    STANDBY,
    RUNNING,
    OFF
};

// guard open: the saftey guard is open and the motor can't run
// guard closed: the saftey guard is down and the motor CAN run
enum GuardStatus {
    GUARD_OPEN,
    GUARD_CLOSED
};

// temp safe: the temp is low and it IS safe to run the motor
// temp high: temp is too high to run the motor
enum TempStatus {
    TEMP_SAFE,
    TEMP_HIGH
};

void setStandbyLED(int motorState, int guardState, int tempState);
void setRunningLED(int motorState);
void setGuardFaultLED(int guardState);
void setTempFaultLED(int tempState);

int confirmMotorStatus(int motorState, int guardState, int tempState);

void setMotorState(int motorState, int guardState, int tempState);
void startMotor();
void stopMotor();

void pause_app(int ms);

int main()
{
    
    int motorStatus = OFF;
    int guardStatus = GUARD_OPEN;
    int tempStatus = TEMP_HIGH;

    while(1) {
        // STEP 1: GET USER AND SENSOR INPUT
        motorStatus = getUserInput();
        guardStatus = getGuardSensorInput();
        tempStatus = getTempSensorInput();

        // elevate the motor status to on if the conditions are right
        motorStatus = confirmMotorStatus(motorStatus, guardStatus, tempStatus);
        
        // STEP 2: SHOW OUTPUT
        setStandbyLED(motorStatus, guardStatus, tempStatus);
        setRunningLED(motorStatus);
        setGuardFaultLED(guardStatus);
        setTempFaultLED(tempStatus);

        // STEP 3: START OR STOP THE MOTOR
        setMotorState(motorStatus, guardStatus, tempStatus);

        wait(0.25);
    }
}

// return STANDBY if the user wants to start the motor
// we need to do extra checks to make sure if motor is STANDBY to run
int getUserInput()
{
    // turning the motor off should have higher precedence
    // than turning it on
    if(stopButton) {
        return OFF;
    } 
    // still explicitly query the start button
    // so we can default to stop if necessary
    else if (startButton) {
        return STANDBY;
    }
    else {
        return OFF;
    }
}

int getGuardSensorInput()
{
    // check safety guard
    // the sensor returns true if the guard is down
    if (guardSensor) {
        return GUARD_CLOSED;
    }
    else {
        return GUARD_OPEN;
    }
}

int getTempSensorInput()
{
    if(tempSensor) {
        return TEMP_HIGH;
    }
    else {
        return TEMP_SAFE;
    }
}

void setStandbyLED(int motorState, int guardState, int tempState)
{
    if(motorState == STANDBY
        && guardState == GUARD_CLOSED
        && tempState == TEMP_SAFE)
    {
        standbyStateLed = 1;
    }
    else
    {
        standbyStateLed = !standbyStateLed;
        // pause_app for half second
        pause_app(500);
    }
}

void setRunningLED(int motorState)
{
    if (motorState == RUNNING)
    {
        motorRunningLed = 1;
    }
    else
    {
        motorRunningLed = 0;
    }
}

void setGuardFaultLED(int guardState)
{
    if (guardState == GUARD_OPEN) {
        guardOpenWarningLed = 1;
        pause_app(500);
    }
    
    guardOpenWarningLed = 0;
}

void setTempFaultLED(int tempState)
{
    if(tempState == TEMP_HIGH)
    {
        tempWarningLed = 1;
        pause_app(500);
    }
    
    tempWarningLed = 0;
}

// check if the motor should be running
// this will set the motor state to RUNNING only if
// - the motor is in standby
// - the guard is lowered
// - the temp is safe
// otherwise it will return the input motor state
int confirmMotorStatus(int motorState, int guardState, int tempState)
{
    if ((motorState == STANDBY || motorState == RUNNING)
        && guardState == GUARD_CLOSED
        && tempState == TEMP_SAFE)
    {
        return RUNNING;
    }
    else
    {
        return motorState;
    }
}

void setMotorState(int motorState, int guardState, int tempState)
{
    if (motorState == STANDBY
        && guardState == GUARD_CLOSED
        && tempState == TEMP_SAFE)
    {
        startMotor();
    }
    else
    {
        stopMotor();
    }
}

void startMotor()
{
    printf("Motor running");
}
void stopMotor()
{
    printf("Motor stopped");
}

void pause_app(int ms)
{
    wait_ms(ms);
}
