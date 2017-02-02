#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define DATA_PIN 6

#define WIDTH_MATRIX 59  
#define HIGHT_MATRIX 30 
#define DISPLAYED_GRAPHS 30
#define STEP_GRAPS (WIDTH_MATRIX/DISPLAYED_GRAPHS)
#define HEIGTH_ORANGE (HIGHT_MATRIX * 0.5)
#define HEIGHT_RED    (HIGHT_MATRIX * 0.75)

int mode = 0; //0=default, 1=graph, to be extended

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(WIDTH_MATRIX, HIGHT_MATRIX, DATA_PIN, NEO_MATRIX_TOP + NEO_MATRIX_RIGHT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG, NEO_RGB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void setup() {
  // initialize serial:
  Serial.begin(115200);

  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(30);
  matrix.setTextColor(colors[0]);

}

void loop() {
  if (Serial.available() > 0) {
    mode = Serial.parseInt();
    switch (mode){
      case 1:     //1, 30, 60, 80, ... (Mode:1, row0:30%, row1:60%, row3:80%, .....)
        displayGraphs(false);  
        break;
      case 2:     //test_fill
        matrix.setCursor(0,10);
        matrix.setTextColor(colors[2]);
        matrix.setTextSize(1);
        matrix.print("Siemens");
        matrix.show();
        break;
      case 3:
        matrix.drawLine(0,0,58,29, colors[0]);
        matrix.show();
        break;
      case 4:
        matrix.fillRect(0,0,30,29, colors[0]);
        matrix.show();
        break;
      case 5:
        matrix.clear();
        matrix.show();
        break;
      case 6:
        break;
      case 7:
        drawGraph();
        break;
    }

    
  }

}

void drawGraph(){
  if(Serial.available() > 0){
    int stripe_nr = Serial.parseInt();  // get Stripe Number from serial line
    int height = map(Serial.parseInt(), 0, 100, 0, HIGHT_MATRIX); // Map to height 

    matrix.drawLine(stripe_nr, 0, stripe_nr, height, matrix.Color(Serial.parseInt(), Serial.parseInt(), Serial.parseInt() ));
  }
}

void displayGraphs(bool amp){
  for (int i = 0; i<DISPLAYED_GRAPHS; i++){
    if (Serial.available() > 0) {
      //matrix.setCursor(i*STEP_GRAPS, 0); // cursor is only needed for text input
      int height = map(Serial.parseInt(), 0, 100, 0, HIGHT_MATRIX); // Map to height 
      if ( amp = true){
        if ( height > HEIGTH_ORANGE){
          matrix.drawLine(i, 0, i, HEIGTH_ORANGE, colors[1]); // green
        }else{
          matrix.drawLine(i, 0, i, height, colors[1]); // green
        }
        if (height > HEIGTH_ORANGE && height > HEIGHT_RED){
          matrix.drawLine(i, HEIGTH_ORANGE+1, i, HEIGHT_RED, matrix.Color(255, 208, 0)); // orange
          matrix.drawLine(i, HEIGHT_RED+1, i, height, colors[0]); // red
        }else if(height > HEIGTH_ORANGE){
          matrix.drawLine(i, HEIGTH_ORANGE+1, i, height, matrix.Color(255, 208, 0)); //orange
        }
      }
      else{ //amp = false -> defaultrgbrgb
        matrix.drawLine(i, 0, i, height, colors[1]); // green
      }
    }
    else{
      ;//print error Serial failed to deliver all needed values
    }
  }
  matrix.show(); //write generated matrix out to the bus-line
}

