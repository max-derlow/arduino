enum ledStates {ALERT, DETECTION, HUMMING, INCREASE, DECREASE, STAY, WAVE, OFF, ON}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'
enum ledStates previousLedState = ledState;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
float iDelay = 0;
float iDelayVelocity = 1;
float iDelayVelocityMultiplier = 0.01;
int brightness = 0; // our main variable for setting the brightness of the LED
int bufferer = 0;
float velocity = 1.0; // the speed at which we change the brightness.
const int ledPin = 9; // we use pin 9 for PWM
const int buttonPin = 2;
const int photoSensor = 13;
int buttonState = 0;
int p = 0; // use to keep track how often we plot
int plotFrequency = 3; // how often we plot, every Nth time.
int photoValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); // set ledPin as an output.
  pinMode(buttonPin, INPUT);
  pinMode(photoSensor, INPUT);
  ledState = INCREASE;
  Serial.begin(9600); // initiate the Serial monitor so we can use the Serial Plotter to graph our patterns
}

void loop() {
  // put your main code here, to run repeatedly:
  photoValue = digitalRead(photoSensor);
  Serial.println(photoSensor);
  compose();
  delay(14 + iDelay);
  buttonState = digitalRead(buttonPin);
  analogWrite(ledPin, brightness);
  currentMillis = millis(); //store the current time since the program started
  //Serial.println("Buttonstate: " + buttonState);
  //Serial.print(buttonState);

  /*
    Serial.println("Buttonstate: " + buttonState);
    if (buttonState == HIGH) {
    // turn LED on:
      Serial.println("Buttonstate: High");
    } else {
    // turn LED off:
      Serial.println("Buttonstate: Low");
    }
  */
}

void compose() {
  // this is a state machine which allows us to decouple the various operations from timed loops.
  // instead we just switch from state to state when particular conditions are met.
  // we switch states by calling the changeState() function.

  switch (ledState) {

    case INCREASE:
    // smooth out the curve when @ values of 6 or lower.
      if (brightness <= 6){
        //Serial.println("increase trigger");
        brightness = increase_brightness(brightness, 1.5);
      } else {
      brightness = increase_brightness(brightness, 2.8);
      //plot("INCREASING", brightness);
      }

      if (brightness >= 64) {
        //ledState = WAVE;
        changeState(DECREASE);
      }
      break;

    case DECREASE:
    // smooth out the curve when @ values of 6 or higher.
      if(brightness >= 6){
        //Serial.println("decrease trigger");
        brightness = decrease_brightness(brightness, 1.4);
      } else {
        brightness = decrease_brightness(brightness, 2.2);
      }


 /*
float iDelay = 0;
float iDelayVelocity = 1;
float iDelayVelocityMultiplier = 0.01;
 */
      //plot("DECREASING", brightness);
      if (brightness <= 2) {
        iDelay = iDelay + (0.85 * iDelayVelocity);
        iDelayVelocity = iDelayVelocity + 0.05 + iDelayVelocityMultiplier;
        iDelayVelocityMultiplier = iDelayVelocityMultiplier + 0.01;
        //Serial.println(iDelay + 14);
        //Serial.println(iDelayVelocity);
        changeState(INCREASE);
      }
      break;

    case WAVE:
      //plot("WAVE", brightness);

      brightness = sinewave(1000, 256, 0); // you can tweak the parameters of the sinewave
      analogWrite(ledPin, brightness);

      if (currentMillis - startMillis >= 5000) { //change state after 5 secs by comparing the time elapsed since we last change state
        changeState(DECREASE);
      }
      break;

    case STAY:
      //plot("STAY", brightness);
      brightness = brightness;
      break;

    case ON:
      //plot("ON", brightness);
      brightness = 255;
      break;

    case OFF:
      //plot("OFF", brightness);
      brightness = 0;
      if (currentMillis - startMillis >= 1000) {
        changeState(INCREASE);
      }
      break;

    case HUMMING:
      if (buttonState == 1) {
        changeState(DETECTION);
      } else {
        //Serial.println("HUMMING");
        wavyshine();
      }
      break;

    case DETECTION:
      //Serial.println("DETECTION");
      brightness = 255;
      //plot("DETECTION", brightness);
      changeState(ALERT);
      break;

    case ALERT:
      if (buttonState == 1) {
        changeState(DETECTION);
      } else {
        //Serial.println("ALERT");
        doForMs(5000, alert);
        changeState(HUMMING);
      }
      break;
  }
}

void alert() { //testing
  if (buttonState == 1) {
    //Serial.println("NEW ALERT!!!");
    changeState(DETECTION);
  }
  brightness = sinewave(500, 255, 128); // you can tweak the parameters of the sinewave
  analogWrite(ledPin, brightness);
  //plot("ALERT", brightness);
}

//placeholder for fade-effect to make smoother transitions
/*void alert(){ //testing
  brightness = sinewave(1000,255,128); // you can tweak the parameters of the sinewave
  analogWrite(ledPin, brightness);
  changeState(HUMMING);
  }
*/
void wavyshine() {
  brightness = sinewave(1000, 16, 2); // you can tweak the parameters of the sinewave
  analogWrite(ledPin, brightness);
  //plot("HUMING", brightness);
}

void changeState(ledStates newState) {
  // call to change state, will keep track of time since last state
  startMillis = millis();
  ledState = newState;
}

void plot(char *state, int brightness) {
  // use this function to plot a graph.
  // it will normalize the auto-scaling plotter

  if ((p % plotFrequency) == 0) {
    Serial.print(state);
    Serial.print(", ");
    Serial.print(brightness);
    Serial.println(", 0, 300");
  }
  p++;
}

int increase_brightness (int brightness, float velocity) {
  return brightness = brightness + 1 * velocity;
}

int decrease_brightness (int brightness, float velocity) {
  return brightness = brightness - 1 * velocity;
}

int sinewave(float duration, float amplitude, int offset) {
  // Generate a sine oscillation, return a number.
  // In case you are using this for analogWrite, make sure the amplitude does not exceed 256
  float period = millis() / duration; // Duration in ms determines the wavelength.
  float midpoint = amplitude / 2; // set the midpoint of the wave at half the amplitude so there are no negative numbers
  int value = midpoint + midpoint * sin ( period * 2.0 * PI );
  value = value + offset; //offset allows you to move the wave up and down on the Y-axis. Should not exceed the value of amplitude to prevent clipping.
  return value;
}


void doForMs(int duration, void (*function)()) {
  // this helper function allows us to execute another function for 'duration' amount of millisecs
  bool doing = true;
  startMillis = millis();
  while (doing) {
    currentMillis = millis();
    buttonState = digitalRead(buttonPin);

    if (buttonState == 1) {
      //Serial.println("please abort ffs");
      changeState(DETECTION);
      doing = false;
    }
    (*function)();

    if (currentMillis - startMillis >= duration) {
      doing = false;
    }
  }
}

void doAfterMs(int duration, void (*function)()) {
  // this helper function allows us to execute another function AFTER a 'duration' amount of millisecs
  bool doing = true;
  startMillis = millis();
  while (doing) {
    currentMillis = millis();
    if (currentMillis - startMillis >= duration) {
      doing = false;
      (*function)();
    }
  }
}
