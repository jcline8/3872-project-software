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
#define SONG_TEMPO 250
#define OCTIVE 4

// Song data
float NOTES[] = {C, REST, C, REST, C, REST, D, REST, E, REST, E, REST, D, REST, E, REST, F, REST, G, REST, HIGH_C, REST, HIGH_C, REST, HIGH_C, REST, G, REST, G, REST, G, REST, E, REST, E, REST, E, REST, C, REST, C, REST, C, REST, G, REST, F, REST, E, REST, D, REST, C, REST};
int BEATS[] = {2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 5, 1};

float* reference_signal;
float* create_reference();

bool find_tempo();
bool sync_time();

void setup() {
  // put your setup code here, to run once:
  reference_signal = create_reference();
}

void loop() {
  // put your main code here, to run repeatedly:

}

float* create_reference() {
  float frequency_hz[SONG_LENGTH];
  int duration_s[SONG_LENGTH];

  for (int i = 0; i < SONG_LENGTH; i++) {
    frequency_hz[i] = NOTES[i];
    duration_s[i] = BEATS[i] * SONG_TEMPO;
  }

  int cum_duration[SONG_LENGTH];
  cum_duration[0] = duration_s[0];
  int total_duration = cum_duration[0];

  for (int i = 1; i < SONG_LENGTH; i++) {
    cum_duration[i] = duration_s[i] + cum_duration[i -1];
    if (cum_duration[i] > total_duration) {
      total_duration = cum_duration[i];
    }
  }

  int fs_Hz = FS_KHZ * 1000;
  int length_time_s = (total_duration * fs_Hz) + 1;
  int time_s[length_time_s];
  
  time_s[0] = 0;
  for (int i = 1; i < length_time_s; i++) {
    time_s[i] = time_s[i - 1] + 1/fs_Hz;
  }
  time_s[length_time_s - 1] = total_duration;

  
}
