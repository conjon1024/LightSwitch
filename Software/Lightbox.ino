// Connor Johnson
// Code for LightBox Project

#define ButtonIn 3 // input pin is PD2 Pin 4
#define PWM_LED_Out 9 // output pin is PB1 Pin 15

int OperatingMode = 0; // operating mode: 0 for off, 1 for bright, 2 for mid, 3 for low, 4 for flash
bool ButtonPushed = false; // will switch from false to true when the button is pushed
int PWM_Out = 0; // initial output state is 0, or the off state
unsigned long PrevMillis = 0; // store last time LED was updated

void setup() {
  attachInterrupt(digitalPinToInterrupt(ButtonIn), button_pushed, FALLING); // interrupt will trigger event, sense when button is pushed
  
  pinMode(PWM_LED_Out,OUTPUT); // set pin to output mode (define output pin connected to LED)
}

void button_pushed() {
  ButtonPushed = true; // when the button is pushed, output from false to true
}

void loop() {

  check_button_push();
  
  set_pwm_based_on_operating_mode();
  
  shine_LED();

}

void check_button_push() {  //set operating mode and reset previous button state, etc.
  if (ButtonPushed == true) { // if button pressed, change operating mode to different states
    OperatingMode = OperatingMode + 1;
    if (OperatingMode == 5) {
      OperatingMode = 0; // after case 4, go back to 0
    }
  }

  ButtonPushed = false; // set button press back to false after changing operating mode
}

void set_pwm_based_on_operating_mode() { //changing the output state based on how many times the button was pressed

switch (OperatingMode) {
    case 0:
      PWM_Out = 0; // off state
      break;
    case 1:
      PWM_Out = 255; // PWM Max, bright state
      break;
    case 2:
      PWM_Out = 128; // PWM Max/2, mid state
      break;
    case 3:
      PWM_Out = 64; // PWM Max/4, low state
      break;
    case 4:
      flash_the_light(); // flashing light state
      break;
  }
}

void flash_the_light() {
  unsigned long CurrMillis = millis();
  if (CurrMillis - PrevMillis >= 500) { // defining 500ms as 2 Hz delay, checking if it is time to blink LED
    PrevMillis = CurrMillis;
    if (PWM_Out == 0) {
      PWM_Out = 255; // if LED is off, turn it on and vice-versa
    } else {
      PWM_Out = 0;
    }
  }
}

void shine_LED() {
  analogWrite(PWM_LED_Out, PWM_Out); // set the LED with the output state
}

