// Required libraries
#include <math.h>

// World constants
#define FS_KHZ 5

// Song notes
#define C 16.3516*pow(2, OCTIVE)
#define D 18.35405*pow(2, OCTIVE)
#define E 20.60172*pow(2, OCTIVE)
#define F 21.82676*pow(2, OCTIVE)
#define G 24.49971*pow(2, OCTIVE)
#define A 27.5*pow(2, OCTIVE)
#define B 30.86771*pow(2, OCTIVE)
#define HIGH_C 32.70320*pow(2, OCTIVE)
#define REST 0

// Song constants
#define SONG_LENGTH 54
#define SONG_TEMPO 0.6
#define OCTIVE 4

// Song data
float NOTES[] = {C, REST, C, REST, C, REST, D, REST, E, REST, E, REST, D, REST, E, REST, F, REST, G, REST, HIGH_C, REST, HIGH_C, REST, HIGH_C, REST, G, REST, G, REST, G, REST, E, REST, E, REST, E, REST, C, REST, C, REST, C, REST, G, REST, F, REST, E, REST, D, REST, C, REST};
static int BEATS[] = {2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 5, 1};

// Variables
float* digital;

// Functions
void set_up_pins();
void set_up_serial();
void create_song(float*, float*, int*);
float* create_signal(float*, float*, int*);

void setup() {
  float frequency_hz[SONG_LENGTH];
  float cumulative_duration[SONG_LENGTH];
  int* time_s;
  
  set_up_pins();
  set_up_serial();
  
  create_song(frequency_hz, cumulative_duration, time_s);
  digital = create_signal(frequency_hz, cumulative_duration, time_s);

  free(time_s);
}

void loop() {
  // Check for manual mode switch

  // Enter manual mode or 

  // Populate audio sample buffer

  // Attempt find tempo

  // Attempt sync time

  // Enter manual mode if syncs not found
  
}

void set_up_pins() {
  // Inputs
  /*
   * Microphone (Analog)
   * Power Switch (Digital)
   * Mode Pushbutton (Digital)
   * Pitch Potentiometer (Analog)
   */
   // Outputs 
   /*
    * Speaker (Analog)
    * RGB LED (x3) (PWM?)
    * Red LED (Digital)
    * Orange LED (Digital)
    * Motors (Digital?)
    */
}

void set_up_serial() {
  Serial.begin(9600);
  Serial.print("Setup Complete\n");
}

void create_song(float* frequency_hz, float* cumulative_duration, int* time_s) {
  float duration_s[SONG_LENGTH];

  for (int i = 0; i < SONG_LENGTH; i++) {
    frequency_hz[i] = NOTES[i]; // Working
    duration_s[i] = BEATS[i] * SONG_TEMPO; // Working
  }

  cumulative_duration[0] = duration_s[0];
  float total_duration = cumulative_duration[0];

  for (int i = 1; i < SONG_LENGTH; i++) {
    cumulative_duration[i] = duration_s[i] + cumulative_duration[i -1];
    Serial.println(cumulative_duration[i]);
    if (cumulative_duration[i] > total_duration) {
      total_duration = cumulative_duration[i];
    }
  }

  int fs_Hz = FS_KHZ * 1000;
  int length_time_s = (total_duration * fs_Hz) + 1;
  time_s = new int[length_time_s];
  time_s[0] = 0;
  
  for (int i = 1; i < length_time_s; i++) {
    time_s[i] = time_s[i - 1] + 1/fs_Hz;
  }
  
  time_s[length_time_s - 1] = total_duration;
}

float* create_signal(float* frequency_hz, float* cumulative_duration, int* time_s) {
  int length_digital;
  float* digital;
  
  int min_time_idx = 0;
  
  for (int i = 0; i < SONG_LENGTH; i++) {
    int max_time_idx = 0;

    for (int j = 0; cumulative_duration[i] <= time_s[j]; j++) {
      max_time_idx = j;
    }

    int length_digital_notes = max_time_idx - min_time_idx + 1;
    float digital_notes[length_digital_notes];
    int digital_notes_idx = 0;
    
    for (int j = min_time_idx; j <= max_time_idx; j++) {
      digital_notes[digital_notes_idx] = (float) sin(2 * PI * frequency_hz[i] * time_s[j]);
      digital_notes_idx++;
    }

    min_time_idx = max_time_idx + 1;

    if (i == 0) {
      length_digital = length_digital_notes;
      digital = new float[length_digital];
      
      for (int j = 0; j < length_digital_notes; j++) {
        digital[j] = digital_notes[j];
      }
    } else {
      int old_length_digital = length_digital;
      length_digital += length_digital_notes;
      float* new_digital = new float[length_digital];
      
      for (int j = 0; j < length_digital; j++) {
        if (j < old_length_digital) {
          new_digital[j] = digital[j];
        } else {
          new_digital[j] = digital_notes[j - old_length_digital];
        }
      }

      free(digital);
      digital = new_digital;
    }
  }
  
  return digital;
}
