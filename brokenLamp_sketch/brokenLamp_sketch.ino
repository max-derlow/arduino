enum ledStates {FADE, INCREASE, DECREASE, STAY, WAVE, OFF, ON}; // Here we make nicknames for the different states our program supports.
enum ledStates ledState; // We define 'ledState' as type ledStates'
enum ledStates previousLedState = ledState;

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

int brightness = 255; // our main variable for setting the brightness of the LED
float velocity = 1.0; // the speed at which we change the brightness.
int ledPin = 9; // we use pin 9 for PWM
int p = 0; // use to keep track how often we plot
int plotFrequency = 3; // how often we plot, every Nth time.

//randy variables
int randomDuration;
float randomVelocity;
float randomResistance;

void setup() {
  // put your setup code here, to run once:
  ledState = FADE;
  pinMode(ledPin, OUTPUT); // set ledPin as an output.
  Serial.begin(9600); // initiate the Serial monitor so we can use the Serial Plotter to graph our patterns
}

void loop() {
  // put your main code here, to run repeatedly:
  compose();
  delay(10);
  analogWrite(ledPin, brightness);
  currentMillis = millis(); //store the current time since the program started
}

void compose() {
  // this is a state machine which allows us to decouple the various operations from timed loops. 
  // instead we just switch from state to state when particular conditions are met.
  // we switch states by calling the changeState() function.
  
  switch (ledState){

  case FADE:
    randomDuration = random(500,5000);
    randomVelocity = random(float(50),float(100))/float(100);
    Serial.print("Random duration: ");
    Serial.println(randomDuration);
    Serial.print("Random velocity: ");
    Serial.println(randomVelocity);
    decreaseForMs(brightness, randomDuration, randomVelocity);
   //doForMs(randomDuration, decrease_brightness(randomDuration, randomVelocity)); //breaks the code
    break;

  case INCREASE:
    //brightness = increase_brightness(brightness, 1);

    //plot("INCREASING", brightness);
        
    if (brightness > 250){
      //ledState = WAVE;
      }
    break;
   
  case DECREASE:
    //brightness = decrease_brightness(brightness, 0.5);
    //plot("DECREASING", brightness);
      if (brightness == 0){
      }
     break;

  case WAVE:
    //plot("WAVE", brightness);
    
    //brightness = sinewave(1000,256,0); // you can tweak the parameters of the sinewave
    analogWrite(ledPin, brightness);
    
    if (currentMillis - startMillis >= 5000){ //change state after 5 secs by comparing the time elapsed since we last change state
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
    if (currentMillis - startMillis >= 1000){
      }
    break;
  }
}

void goBackOn(){
  ledState=INCREASE;
}

void Fade(int brightness, float velocity){
  brightness = sinewave(1000,128,64); // you can tweak the parameters of the sinewave
  analogWrite(ledPin, brightness);
}

void wavyshine(){
  Serial.print("WAVE  ");
  Serial.println(brightness);
  brightness = sinewave(1000,128,64); // you can tweak the parameters of the sinewave
  analogWrite(ledPin, brightness);
}

void changeState(ledStates newState){
    // call to change state, will keep track of time since last state
    startMillis = millis();
    ledState = newState;
  }
  
void plot(char *state, int brightness){
    // use this function to plot a graph.
    // it will normalize the auto-scaling plotter

    if ((p % plotFrequency) == 0){
      Serial.print(state);
      Serial.print(", ");
      Serial.print(brightness);
      Serial.println(", 0, 300");
    }
    p++;
  }

int increase_brightness (int brightness, float velocity){
    return brightness = brightness + 1 * velocity;
  }

int decrease_brightness (int brightness, float velocity){
    brightness = brightness - 5 * velocity;
    analogWrite(ledPin, brightness);
    Serial.print("Brightness: ");
    Serial.println(brightness);
    return brightness;
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

void decreaseForMs(int brightness, int duration, float velocity){
  // this helper function allows us to execute another function for 'duration' amount of millisecs
  bool doing = true;
  startMillis = millis();
  while(doing){
    currentMillis = millis();
    decrease_brightness(brightness, float(velocity));
    if (currentMillis - startMillis >= duration){
      doing = false;
      }
    }
  }
/*
void doForMs(int duration, void (*function)(int brightness, float velocity)){
  // this helper function allows us to execute another function for 'duration' amount of millisecs
  bool doing = true;
  startMillis = millis();
  while(doing){
    currentMillis = millis();
    (*function)(brightness, velocity);
    if (currentMillis - startMillis >= duration){
      doing = false;
      }
    }
  }
*/
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
