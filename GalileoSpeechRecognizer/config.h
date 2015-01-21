#ifndef _CONFIG_H_
#define _CONFIG_H_

const int MICROPHONE_PIN = 0;
const int SAMPLING_FREQ = 16000; // Hz
const double FRAME_LENGTH = 0.025;
const int S = 400; //number of samples ( SAMPLING_FREQ * FRAME_LENGTH ) VS has complained about not constant expression
const int MEL_LOWER_BOUND_HZ = 300;
const int MEL_UPPER_BOUND_HZ = 8000;
const int FILTERBANKS = 26;
const int CLOCK_DIFF = 895;
const int BUFFER_SIZE = 10000000;

#endif