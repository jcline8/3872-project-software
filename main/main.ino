enum modes{INIT, MOTION, SOUND, LIGHTS};

volatile modes mode = INIT;
const byte mode_pin = 2;

void mode_handler();
void get_next_mode();
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
}

void serial_setup() {
  Serial.begin(9600);
  Serial.println("Setup Complete");
}

void pin_setup() {
  pinMode(mode_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(mode_pin), mode_handler, FALLING);
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
    switch (mode) {
    case INIT:
      Serial.println("Entering manual motion mode.");
      mode = MOTION;
      break;
    case MOTION:
      Serial.println("Entering manual sound mode.");
      mode = SOUND;
      break;
    case SOUND:
      Serial.println("Entering manual lights mode.");
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
  
}

void motion_mode() {
  
}

void sound_mode() {
  
}

void lights_mode() {
  
}
