#include "test.h"
#include "Controller.h"

extern Controller arduino;

void test_execute(){
  int token_queue[] = {1, 2, 6, 102, 4, 101, 5, 100, 3, 102, 4, 103, 5, -1};
  arduino.TranspilaCodigo(token_queue, 14, 14);
  Serial.println(arduino.code);
}


