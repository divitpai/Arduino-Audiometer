#include <Adafruit_NeoPixel.h> // Replace with your LED matrix library

// Define LED matrix size
const int MATRIX_WIDTH = 8;
const int MATRIX_HEIGHT = 8;

// Define switch, rotary encoder, and buzzer pins
const int switchPin = 2;
const int encoderA = 3;
const int encoderB = 4;
const int buzzerPin = 8;

// Predefined line graph data (red and blue)
int redLine[MATRIX_HEIGHT] = {0, 2, 4, 6, 7, 5, 3, 1};
int blueLine[MATRIX_HEIGHT] = {7, 5, 3, 1, 0, 2, 4, 6};

// Global variables for sound and rotary encoder
int frequency = 1000; // Initial frequency
int encoderPos = 0;
volatile bool switchPressed = false; // Flag for switch press

#define LED_PIN 6
#define NUMPIXELS 64

Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pixels.begin();
  attachInterrupt(digitalPinToInterrupt(encoderA), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderB), readEncoder, CHANGE);
  pixels.clear(); // Clear LED matrix
}

void loop() {
  // Check switch press
  if (!digitalRead(switchPin) && !switchPressed) {
    switchPressed = true;
    playFrequency(frequency); // Play current frequency
  } else if (digitalRead(switchPin)) {
    switchPressed = false;
  }

  // Check rotary encoder for frequency adjustment
  if (encoderPos > 0) {
    frequency += 100; // Increase frequency
    encoderPos = 0;
  } else if (encoderPos < 0) {
    frequency -= 100; // Decrease frequency (ensure it stays positive)
    if (frequency < 0) {
      frequency = 0;
    }
    encoderPos = 0;
  }

  // Display line graph after switch press
  if (switchPressed) {
    displayLineGraph();
  }
}

void playFrequency(int freq) {
  tone(buzzerPin, freq, 100); // Play sound for 100 milliseconds
}

void readEncoder() {
  int change = (digitalRead(encoderA) << 1) | digitalRead(encoderB);
  static int lastState = 0;
  if ((lastState == 0 && change == 3) || (lastState == 3 && change == 1)) {
    encoderPos++;
  } else if ((lastState == 1 && change == 0) || (lastState == 2 && change == 2)) {
    encoderPos--;
  }
  lastState = change;
}

void displayLineGraph() {
  pixels.clear();
  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    pixels.setPixelColor(y, redLine[y] ? pixels.Color(255,0,0) : pixels.Color(0,0,0));
    pixels.setPixelColor(y + MATRIX_HEIGHT, blueLine[y] ? pixels.Color(0,0,255) : pixels.Color(0,0,0));
  }
  pixels.show();
}
