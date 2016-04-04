//#include <ESP8266WiFi.h>

#include <Stepper.h>

const int STEPS = 240;

Stepper stepper1(STEPS, 8, 9, 10, 11);
Stepper stepper2(STEPS, 2, 3, 4, 5);

int alphabet_braille_mapper[26][2] = {
  {1, 0}, //A
  {4, 0}, //B
  {1, 1}, //C
  {1, 4}, //D
  {1, 2}, //E
  {4, 1}, //F
  {4, 4}, //G
  {4, 2}, //H
  {2, 1}, //I
  {2, 4}, //J

  {5, 0}, //K
  {7, 0}, //L
  {5, 1}, //M
  {5, 4}, //N
  {5, 2}, //O
  {7, 1}, //P
  {7, 4}, //Q
  {7, 2}, //R
  {6, 1}, //S
  {6, 4}, //T

  {5, 3}, //U
  {7, 3}, //V
  {2, 7}, //W
  {5, 5}, //X
  {5, 7}, //Y
  {5, 6}, //Z

};

char stringBuffer[14];

int prev_left_motor_position = 0;
int prev_right_motor_position = 0;
int current_left_motor_position = 0;
int current_right_motor_position = 0;
int left_motor_steps = 0;
int right_motor_steps = 0;



void setup() {
  stepper1.setSpeed(60);
  stepper2.setSpeed(60);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  if (Serial.available())
  {
    String line = Serial.readStringUntil('\r');
    Serial.print(line);
    line.toCharArray(stringBuffer, 14);
    int index;
    int ascii_value;

    for (int i = 0; i < 14; i++)
    {
      prev_left_motor_position = current_left_motor_position;
      prev_right_motor_position = current_right_motor_position;

      //Getting the single charectars from string
      //Serial.print("This is one character -- ");
      ascii_value = int(stringBuffer[i]);
      //Serial.write(stringBuffer[i]);

      //If-else statements to check what the letter is
      if ( ascii_value >= 65 && ascii_value < 91)
      {
        index = ascii_value - 65;
      }
      else if ( ascii_value >= 97 && ascii_value < 123)
      {
        index = ascii_value - 97;
      }
      //steps for the motor to turn
      current_left_motor_position = alphabet_braille_mapper[index][0];
      current_right_motor_position = alphabet_braille_mapper[index][1];

      left_motor_steps = calculateSteps(current_left_motor_position, prev_left_motor_position);
      right_motor_steps = -calculateSteps(current_right_motor_position, prev_right_motor_position);

      Serial.print( "motor spin - ");
      Serial.print( left_motor_steps);
      Serial.print( " -- " );
      Serial.println(right_motor_steps );
      stepper1.step(left_motor_steps);
      stepper2.step(right_motor_steps);
      delay(2000);
    }

  }

  //Clear the buffer array
  for (int i = 0; i < 14; i++)
  {
    stringBuffer[i] = 0;
  }
}

int calculateSteps(int curr_position, int prev_position)
{
  //Get the difference
  int diff = curr_position - prev_position;
  int dir = 0;


  if (abs(diff) > 4)
  {
    if (diff > 0)
    {
      dir = -1;
    }
    else if (diff < 0)
    {
      dir = 1;
    }
    else
      dir = 0 ;

    diff = dir * (8 - abs(diff));

  }
  else
  {
    diff = diff; //cya
  }

  //if diff > 4 switch direction and make noOfSteps = 8 - diff
  return (diff * 30);
}

