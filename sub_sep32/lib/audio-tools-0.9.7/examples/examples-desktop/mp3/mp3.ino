// Simple wrapper for Arduino sketch to compilable with cpp in cmake
#include "Arduino.h"
#include "AudioTools.h"
#include "BabyElephantWalk60_mp3.h"
#include "AudioLibs/PortAudioStream.h"
#include "AudioCodecs/CodecMP3Helix.h"


MemoryStream mp3(BabyElephantWalk60_mp3, BabyElephantWalk60_mp3_len);
PortAudioStream out;   // Output of sound on desktop 
EncodedAudioStream dec(&out, new MP3DecoderHelix()); // MP3 data source
StreamCopy copier(dec, mp3); // copy in to out

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);  

  out.begin();
  mp3.begin();
  dec.begin();
}

void loop(){
  if (mp3) {
    copier.copy();
  } else {
    auto info = dec.decoder().audioInfo();
    LOGI("The audio rate from the mp3 file is %d", info.sample_rate);
    LOGI("The channels from the mp3 file is %d", info.channels);
    exit(0);
  }
}