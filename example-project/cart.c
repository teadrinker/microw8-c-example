
#include "header.h"
#include "micromod.c"
#include "konsum.h"


static float modhack_get_channel_instrument_1based(int ch) {
	return channels[ch].instrument;
}

static float modhack_get_channel_samplepos(int ch) {
	return channels[ch].instrument == 0 ? 9999999999.f : channels[ch].sample_idx / (float)(FP_MASK);
}


// This is the sample rate divider of the audio visualizer.
// On the audio thread, we do 735 samples per frame (44100 / 60)
// depending on the music, we can run Micromod at a lower sample rate
// but probably wise to keep to dividers that results in a whole number of samples: 
//   1, 3, 5, 7, 15, 21, 35, 49, 105 
#define AUDIO_VIS_DIV 5

int curFrame;
short stereoSample[2];
int waveOutPos = 0;
short* waveformL;
short* waveformR;


void start() {
    unsigned char last_byte_before_extended_mem;
    static_alloc_init((&last_byte_before_extended_mem) + 1);

    waveformL = (short*) static_alloc(320 * sizeof(short));
    waveformR = (short*) static_alloc(320 * sizeof(short));

    micromod_initialise((signed char *)konsum, 44100 / AUDIO_VIS_DIV);
    curFrame = 0;
}


void upd() {

    // set grayscale palette
    for (int i = 0; i < 256; i++)
    {
        PALETTE[i * 4 + 0] = (unsigned char)i;
        PALETTE[i * 4 + 1] = (unsigned char)i;
        PALETTE[i * 4 + 2] = (unsigned char)i;
    }
  
    // run micromod to catch up with the audio thread
    int frame = time() * 60.f;
    int frameDiff = frame - curFrame;
    curFrame = frame;
    for (int i = 0; i < frameDiff * 44100 / 60 / AUDIO_VIS_DIV; i++) {
        stereoSample[0] = 0;
        stereoSample[1] = 0;
        micromod_get_audio(stereoSample, 1);
        waveOutPos++;
        waveformL[waveOutPos % 320] = stereoSample[0];
        waveformR[waveOutPos % 320] = stereoSample[1];
    }

    // draw visualization
    cls(0);
    for (int i = 0; i < 320; i++){
        setPixel(i, clamp(waveformL[(waveOutPos + i) % 320] * (1.f / 32767.f), -1.f, 1.f) * 60.f + 120 + 60, 128);
        setPixel(i, clamp(waveformR[(waveOutPos + i) % 320] * (1.f / 32767.f), -1.f, 1.f) * 60.f + 120 + 60, 180);
    }
    for( int ch = 0; ch < 4; ch++)
    {
        int x = ch * 320 / 4;
        int n = max(0.f, 120.f - modhack_get_channel_samplepos(ch) / 32.f);
        for (int i = 0; i < n; i++)
            hline(x,x + 320/4, i, 8 + channels[ch].instrument * 256/32);    
    }


}


float snd(int sampleIndex) {
    if(sampleIndex == 0) micromod_initialise((signed char *)konsum, 44100);
    int channel = sampleIndex & 1;
    if(channel == 0) {
        stereoSample[0] = 0;
        stereoSample[1] = 0;
        micromod_get_audio(stereoSample, 1);
    }
    return stereoSample[channel] / 32767.f;
}