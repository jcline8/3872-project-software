/*
Engineer: Jared Clark

This code is meant to be used with an Arduino Uno to play Row Your Boat, light
LEDs in time with the music, and spin a motor.

Design: Qubey the Qubic Qube
File: qubeySim.cpp

Inputs:
  Volume- controls volume
  Tempo- controls tempo

Outputs:
  RGB LED- lights in time and color corresponding to current note
  Orange LED- need ROYGBIV and RGB can't make O (aware of PWM, but worried about PWM limitations in Arduino)
  DC motor- moves Qubey's sides
  Speaker- plays music

History: created 6/17/2021 using Tim Brother's conductor.c as a guide

*/


#include <stdio.h>
#include <math.h>

#define TempoCal 512
#define TempoPotMax 1023
#define PwmMax 255

#define rest 0

#define Octive 4

//Music Notes based on Octive--
#define C 16.3516*pow(2,Octive)
#define D 18.35405*pow(2,Octive)
#define E 20.60172*pow(2,Octive)
#define F 21.82676*pow(2,Octive)
#define G 24.49971*pow(2,Octive)
#define A 27.5*pow(2,Octive)
#define B 30.86771*pow(2,Octive)
#define high_C 32.70320*pow(2,Octive)
#define rest 0

#define autoPIN 2
#define powerPIN 4
#define speakerPIN 5
#define stepPIN 6
#define dirPIN 7
#define redPIN 3
#define greenPIN 9
#define bluePIN 10
//#define orangePIN 8


//Row Row Row Your Boat
int songLength = 54;
int notes[] = {C, rest, C, rest, C, rest, D, rest, E, rest, E, rest, D, rest, E, rest, F, rest, G, rest, high_C, rest, high_C, rest, high_C, rest, G, rest, G, rest, G, rest, E, rest, E, rest, E, rest, C, rest, C, rest, C, rest, G, rest, F, rest, E, rest, D, rest, C, rest};
int beats[] = {2,1,2,1,2,1,1,1,2,1,2,1,1,1,2,1,1,1,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,2,1,1,1,5,1};

int song_tempo = 250;


void setup()
{
  //pinMode(A0, INPUT); //tempo pot
  pinMode(A1, INPUT);
    //set up outputs
  pinMode(autoPIN, OUTPUT);
  pinMode(powerPIN, OUTPUT);
  pinMode(speakerPIN, OUTPUT);
  pinMode(stepPIN, OUTPUT);
  pinMode(dirPIN, OUTPUT);
  pinMode(redPIN, OUTPUT);
  pinMode(greenPIN, OUTPUT);
  pinMode(bluePIN, OUTPUT);
  //pinMode(orangePIN, OUTPUT);


  //set the motor and servo to zero initial condition.
  //digitalWrite(motorPIN, 0);

  //debug only
  Serial.begin(9600);
  Serial.print("Setup Complete\n");
}

void loop()
{
  int duration;
  int tempo;
  int tempo_pot;
  int motor_speed;
  static const uint8_t analog_pins[] = {A0,A2,A3,A4,A5};

  //Set the LEDs
  digitalWrite(powerPIN, HIGH); //set LEDs to white
  digitalWrite(autoPIN, LOW);
//  digitalWrite(orangePIN, LOW); //^^

  //play the song
  int i_note_index = 0;
  digitalWrite(redPIN,HIGH);//turn off LEDs after note has played
    digitalWrite(greenPIN,HIGH);
    digitalWrite(bluePIN,HIGH);
    digitalWrite(dirPIN, HIGH);
  while (digitalRead(A1))
  {
    //read the tempo pot
    //tempo_pot = analogRead(A0);
    tempo = 250;//song_tempo*float(tempo_pot)/TempoCal; //read the tempo POT

    //set the Motor speed
    //motor_speed = (1 - float(tempo_pot)/TempoPotMax)*PwmMax;
    //analogWrite(motorPIN, motor_speed);
    digitalWrite(autoPIN, HIGH);
    //play the song
    duration = beats[i_note_index] * tempo;
    tone(speakerPIN, notes[i_note_index], duration);
    if ((notes[i_note_index] == (int)floor(C)) || (notes[i_note_index] == (int)floor(high_C))) { //light red on C
      digitalWrite(redPIN, LOW);
    }
    if (notes[i_note_index] == (int)floor(D)) { //light orange on D
      analogWrite(greenPIN, LOW);
    }
    if (notes[i_note_index] == (int)floor(E)) { //light yellow on E
      digitalWrite(redPIN, LOW);
      digitalWrite(greenPIN, LOW);
    }
    if (notes[i_note_index] == (int)floor(F)) { //light green on F
      digitalWrite(greenPIN, LOW);
    }
    if (notes[i_note_index] == (int)floor(G)) { //light blue(cyan) on G
      digitalWrite(greenPIN, LOW);
      digitalWrite(bluePIN, LOW);
    }
    if (notes[i_note_index] == (int)floor(A)) { //light indigo(blue) on A
      digitalWrite(bluePIN, LOW);
    }
    if (notes[i_note_index] == (int)floor(B)) { //light violet on B
      digitalWrite(bluePIN, LOW);
      digitalWrite(redPIN, LOW);
    }
//    delay(2000);
    for(int x=0;x<100;x++){
      digitalWrite(stepPIN, HIGH);
      delay(1);
      digitalWrite(stepPIN, LOW);
      delay(1);
    }
    delay(duration-200);
    digitalWrite(redPIN,HIGH);//turn off LEDs after note has played
    digitalWrite(greenPIN,HIGH);
    digitalWrite(bluePIN,HIGH);
    
    //increment the note counter
    ++i_note_index;
    if(i_note_index >= songLength){
      i_note_index = 0;
    }
  }
}
