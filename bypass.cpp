#include <string.h>
#include "daisy_seed.h"

using namespace daisy;

DaisySeed hw;

#define TEMPO_MIN 30
#define TEMPO_DEFAUT 120
#define TEMPO_MAX 240

// tap tempo variables
static uint32_t prev_ms = 0;
static uint8_t tempo = TEMPO_DEFAUT;

// sync modes: https://teenage.engineering/guides/po-33/en

static float left_cached = 0;

class TempoUtils
{
public:
  static float tempo_to_freq(uint8_t tempo) { return tempo / 60.0f; }
  static uint8_t freq_to_tempo(float freq) { return freq * 60.0f; }
  static float bpm_to_freq(uint32_t tempo) { return tempo / 60.0f; }
  static uint32_t ms_to_bpm(uint32_t ms) { return 60000 / ms; }
};

static void Callback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
  float left;

  for (size_t i = 0; i < size; i++) {
    // right - signal
    out[i] = out[i + 1] = in[i + 1];

    // left - sync
    left = in[i];
    if (left != left_cached) {
      if (left_cached == 0 && left != 0) {
        uint32_t ms = System::GetNow();
        uint32_t diff = ms - prev_ms;
        uint32_t bpm = TempoUtils::ms_to_bpm(diff);
        if (bpm >= TEMPO_MIN && bpm <= TEMPO_MAX) {
          tempo = bpm;
          // chopper.SetFreq(TempoUtils::tempo_to_freq(tempo));
        }

        prev_ms = ms;
      }
      left_cached = left;
    }
  }
}

int main(void)
{
  hw.Init();
  hw.SetAudioBlockSize(4);
  hw.StartAudio(Callback);

  // initialize the logger
  hw.StartLog(false);

  while (1) {
    System::Delay(1000);
    hw.PrintLine("tempo: %d", tempo);
  }
}
