#pragma once
#ifndef _TEMPO_UTILS_H
#define _TEMPO_UTILS_H

class TempoUtils
{
public:
  static float tempo_to_freq(uint8_t tempo) { return tempo / 60.0f; }
  static uint8_t freq_to_tempo(float freq) { return freq * 60.0f; }
  static float bpm_to_freq(uint32_t tempo) { return tempo / 60.0f; }
  static uint32_t ms_to_bpm(uint32_t ms) { return 60000 / ms; }
  static uint32_t us_to_bpm(uint32_t us) { return 60000000 / us; }

  static uint32_t fus_to_bpm(uint32_t us)
  {
    float fus = static_cast<float>(us);
    float val = roundf(60000000.0f / fus);
    return static_cast<uint32_t>(val);
  }
};

#endif