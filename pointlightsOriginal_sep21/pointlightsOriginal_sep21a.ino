enum ledStates {INCREASE, DECREASE, STAY, WAVE, OFF, ON}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

int brightness = 0; // our main variable for setting the brightness of the LED
float velocity = 1.0; // the speed at which we change the brightness.
int ledPin = 9; // we use pin 9 for PWM

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set ledPin as an output.
  Serial.begin(9600); // initiate the Serial monitor so we can use the Serial Plotter to graph our patterns
  

}

void loop() {
  // put your main code here, to run repeatedly:
  compose();
  delay(10);
  analogWrite(ledPin, brightness);

}

void compose() {
  // this is a state machine which allows us to decouple the various operations from timed loops. 
  // instead we just switch from state to state when particular conditions are met. 
  
  switch (ledState){

  case INCREASE:
    brightness = increase_brightness(brightness, 1);

    Serial.print("INCREASING  ");
    Serial.println(brightness);
    
    if (brightness > 250){
      ledState = WAVE;
      }
    break;
   
  case DECREASE:
    brightness = decrease_brightness(brightness, 0.5);
    
    Serial.print("DECREASING  ");
    Serial.println(brightness);
      if (brightness == 0){
      ledState = OFF;
      }
     break;

  case WAVE:
    Serial.print("WAVE  ");
    Serial.println(brightness);
    doForMs(5000, wavyshine); // this you might want to do for number of pulses, rather than for duration
    ledState = DECREASE;
    break;
    
  case STAY:
    Serial.print("STAY"  );
    Serial.println(brightness);
    brightness = brightness;
    break;

  case ON:
    Serial.print("ON"  );
    Serial.println(brightness);
    brightness = 255;
    break;

  case OFF:
    Serial.print("OFF"  );
    Serial.println(brightness);
    brightness = 0;
    doAfterMs(5000, goBackOn);
    break;
    
  
  }
}

void goBackOn(){
  ledState=INCREASE;
  }

void wavyshine(){
  Serial.print("WAVE  ");
  Serial.println(brightness);
  brightness = sinewave(1000,256,0); // you can tweak the parameters of the sinewave
  analogWrite(ledPin, brightness);
  }

int increase_brightness (int brightness, float velocity){
   return brightness = brightness + 1 * velocity;
  }

int decrease_brightness (int brightness, float velocity){
   return brightness = brightness - 1 * velocity;
  }

int sinewave(float duration, float amplitude, int offset){
    // Generate a sine oscillation, return a number.
    // In case you are using this for analogWrite, make sure the amplitude does not exceed 256
    float period = millis()/duration; // Duration in ms determines the wavelength.
    float midpoint = amplitude / 2; // set the midpoint of the wave at half the amplitude so there are no negative numbers
    int value = midpoint + midpoint * sin ( period * 2.0 * PI );
    value = value + offset; //offset allows you to move the wave up and down on the Y-axis. Should not exceed the value of amplitude to prevent clipping.
    return value;
  }

void doForMs(int duration, void (*function)()){
  // this helper function allows us to execute another function for 'duration' amount of millisecs
  bool doing = true;
  startMillis = millis();
  while(doing){
    currentMillis = millis();
    (*function)();
    if (currentMillis - startMillis >= duration){
      doing = false;
      }
    }
   
  }

  void doAfterMs(int duration, void (*function)()){
  // this helper function allows us to execute another function AFTER a 'duration' amount of millisecs
  bool doing = true;
  startMillis = millis();
  while(doing){
    currentMillis = millis();
    if (currentMillis - startMillis >= duration){
      doing = false;
      (*function)();
      }
    }
   
  }
