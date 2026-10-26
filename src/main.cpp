#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  Serial.begin(115200);
}

void setup() 
{
  
  while(!Serial);
  Serial.println("Serial terminal is connected!");
}
