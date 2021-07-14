#include <math.h>

#define C 16.35
#define D 18.35
#define E 20.60
#define F 21.82
#define G 24.49
#define A 27.50
#define B 30.86
#define high_C 32.70
#define rest 0.00

enum modes{INIT, MOTION, SOUND, LIGHTS};

const float max_pot_val = 1023.0;
const int max_octive = 4;

const int song_length = 54;
const int song_tempo = 250;
const int beats[] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};
const float notes[] = {C, rest, C, rest, C, rest, D, rest, E, rest, E, rest, D, rest, E, rest, F, rest, G, rest, high_C, rest, high_C, rest, high_C, rest, G, rest, G, rest, G, rest, E, rest, E, rest, E, rest, C, rest, C, rest, C, rest, G, rest, F, rest, E, rest, D, rest, C, rest};

const byte mode_pin = 2;
const byte speaker_pin = 5;
const byte motor_pin = 7;
const byte orange_pin = 10;
const byte blue_pin = 11;
const byte green_pin = 12;
const byte red_pin = 13;
const int pot_pin = A0;

int note_idx = 0;
float pot_val = 0.0;
modes mode = INIT;

void mode_handler();
void get_next_mode();
void mode_transition();
void blink_orange(int);

void init_mode();
void motion_mode();
void sound_mode();
void lights_mode();

void pin_setup();
void serial_setup();

void setup() {
  serial_setup();
  pin_setup();
}

void loop() {
  pot_val = analogRead(pot_pin) / max_pot_val;
  
  switch (mode) {
    case INIT:
      init_mode();
      break;
    case MOTION:
      motion_mode();
      break;
    case SOUND:
      sound_mode();
      break;
    case LIGHTS:
      lights_mode();
      break;
    default:
      break;
  }

  delay(100);
}

void serial_setup() {
  Serial.begin(9600);
  Serial.println("Setup Complete");
}

void pin_setup() {
  pinMode(mode_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(mode_pin), mode_handler, FALLING);
  pinMode(pot_pin, INPUT);
  
  pinMode(speaker_pin, OUTPUT);
  pinMode(motor_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(orange_pin, OUTPUT);

  digitalWrite(blue_pin, HIGH);
  digitalWrite(green_pin, HIGH);
  digitalWrite(red_pin, HIGH);
  digitalWrite(orange_pin, LOW);
}

void mode_handler() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) {
    get_next_mode();
  }
  last_interrupt_time = interrupt_time;
}

void get_next_mode() {
    mode_transition();
    
    switch (mode) {
    case INIT:
      Serial.println("Entering manual motion mode.");
      blink_orange(1);
      mode = MOTION;
      break;
    case MOTION:
      Serial.println("Entering manual sound mode.");
      blink_orange(2);
      mode = SOUND;
      break;
    case SOUND:
      Serial.println("Entering manual lights mode.");
      blink_orange(3);
      mode = LIGHTS;
      break;
    case LIGHTS:
      Serial.println("Entering init mode.");
      mode = INIT;
      break;
    default:
      Serial.println("Unknown mode. Entering init mode.");
      mode = INIT;
      break;
  }
}

void init_mode() {
  digitalWrite(green_pin, LOW);
  delay(250);
  digitalWrite(green_pin, HIGH);
  delay(250);
}

void motion_mode() {
  
}

void sound_mode() {
  int note_duration = beats[note_idx] * song_tempo;
  int current_octive = ceil(pot_val * max_octive);
  unsigned int note = floor(notes[note_idx] * pow(2, current_octive));
  tone(speaker_pin, note, note_duration);
  delay(500);
  note_idx++;
  if (note_idx >= song_length) {
    note_idx = 0;
  }
}

void lights_mode() {
  
}

void mode_transition() {
  digitalWrite(blue_pin, HIGH);
  digitalWrite(green_pin, HIGH);
  digitalWrite(red_pin, HIGH);
  digitalWrite(orange_pin, LOW);

  note_idx = 0;
}

void blink_orange(int c) {
  for (int i = 0; i < c; i++) {
    digitalWrite(orange_pin, HIGH);
    delay(250);
    digitalWrite(orange_pin, LOW);
    delay(250);
  }
}
