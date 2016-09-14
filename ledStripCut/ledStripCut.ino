#include <PololuLedStrip.h>

PololuLedStrip<A0> ledStrip;
PololuLedStrip<A1> ledStrip2;
PololuLedStrip<A2> ledStrip3;

#define LED_COUNT 144
#define N 144/3

#define BLUE  (rgb_color) { 0, 0, 255 }
#define GREEN  (rgb_color) { 0,255, 0 }
#define RED  (rgb_color) { 255,0, 0 }
#define BLACK  (rgb_color) { 0, 0, 0 }

rgb_color colors[N];
rgb_color colors2[N];
rgb_color colors3[N];

int t = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  
  for (int i=t; i < N ; i+=3) {
    colors[i] = BLUE;
    colors[i+1] = GREEN;
    colors[i+2] = RED;
  }
  ledStrip.write(colors, N);


  for (int i=t; i < N ; i+=2) {
    colors2[i] = GREEN;
    colors2[i+1] = BLACK;
  }
  ledStrip2.write(colors2, N);

   for (int i=t; i < N ; i+=2) {
    colors3[i] = RED;
    colors3[i+1] = BLACK;
  }
  ledStrip3.write(colors3, N);

  delay(400);

  t = (t+1) %2;
}
