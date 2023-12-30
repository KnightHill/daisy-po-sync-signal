#include <string.h>
#include "daisy_pod.h"
#include "tempo_utils.h"

using namespace daisy;

DaisyPod hw;

#define TEMPO_MIN 30
#define TEMPO_DEFAUT 120
#define TEMPO_MAX 240

constexpr float threshold = 0.20f;

// sync tempo variables
static uint32_t prev_timestamp = 0;
static uint8_t tempo = TEMPO_DEFAUT;

// TODO: Try with Volca
// when sync is used the signal will be split between audio (right) and sync (left).
// sync modes: https://teenage.engineering/guides/po-33/en
// set PO sync mode to SY1, SY3 or SY5
// https://p0k3t0.wordpress.com/2017/10/18/pocket-operator-signals/

static float left_cached = 0;

// __attribute__((optimize("O0")))

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i++) {
    // left - sync
    float left = in[0][i];
    float right = in[1][i];

    hw.led1.Set(left, left, left);
    hw.led2.Set(right, right, right);
    hw.UpdateLeds();

    if (fabs(left - left_cached) > threshold) {
      // detect sync raising edge
      // Single pulse, 2.5ms long, with an amplitude of 1V above ground reference.
      if (left_cached < threshold && left > threshold) {
        uint32_t now = System::GetUs();
        uint32_t diff = now - prev_timestamp;
        uint32_t bpm = TempoUtils::fus_to_bpm(diff) / 2;

        if (bpm >= TEMPO_MIN && bpm <= TEMPO_MAX) {
          tempo = bpm;
        }

        prev_timestamp = now;
      }
      left_cached = left;
    }

    out[0][i] = left;
    out[1][i] = right;
  }
}

int main(void)
{
  hw.Init();
  hw.SetAudioBlockSize(4);
  hw.StartAudio(AudioCallback);

  // initialize the logger
  hw.seed.StartLog(false);

  while (1) {
    System::Delay(1000);
    hw.seed.PrintLine("tempo: %d", tempo);
  }
}
