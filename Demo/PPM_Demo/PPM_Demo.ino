#include <PPMReader.h>
// #include <InterruptHandler.h>   <-- You may need this on some versions of Arduino

// Initialize a PPMReader on digital pin 2 with 6 expected channels.
int interruptPin = 2;
int channelAmount = 6;
PPMReader ppm(interruptPin, channelAmount);

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Print latest valid values from all channels
    for (int channel = 1; channel <= channelAmount; ++channel) {
        unsigned long value = ppm.latestValidChannelValue(channel, 0);
        Serial.print(String(value) + " ");
    }
    Serial.println();
}