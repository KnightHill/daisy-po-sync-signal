#include <string.h>
#include "daisy_seed.h"

using namespace daisy;

DaisySeed hw;

static void Callback(AudioHandle::InterleavingInputBuffer  in,
                     AudioHandle::InterleavingOutputBuffer out,
                     size_t                                size)
{
  for(size_t i = 0; i < size; i++)
  {
    // left
    out[i] = in[i];
    // right
    out[i+1] = in[i+1];
  }
}

int main(void)
{
    hw.Init();
    hw.SetAudioBlockSize(4);
    hw.StartAudio(Callback);
    while(1) {}
}
