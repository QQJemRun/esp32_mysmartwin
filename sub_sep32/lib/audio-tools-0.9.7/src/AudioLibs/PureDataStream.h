#include "AudioTools/AudioStreams.h"
#include "AudioTools/Buffers.h"

/**
 * @brief Input and output of Pure Data PD using code generated by the hvcc
 * compiler.
 * @ingroup io
 * @author phil schatzmann
 */
class PureDataStream : public AudioStream {
 public:
  PureDataStream(HeavyContextInterface &heavy, int bufferSize = 1024 * 2) {
    p_heavy = &heavy;
    buffer_size = bufferSize;
  }

  AudioInfo audioInfo() {
    AudioInfo result(p_heavy->getSampleRate(), p_heavy->getNumOutputChannels(),
                     sample_size * 8);
    return result;
  }

  AudioInfo audioInfoIn() {
    AudioInfo result(p_heavy->getSampleRate(), p_heavy->getNumInputChannels(),
                     sample_size * 8);
    return result;
  }

  size_t readBytes(uint8_t *data, size_t len) {
    int len_max = std::min(len, buffer_read.size());
    if (buffer_read.isEmpty()) readWrite(len_max);
    return buffer_read.readArray(data, len_max);
  }

  size_t write(const uint8_t *data, size_t len) {
    int len_max = std::min(len, buffer_write.size());
    if (!buffer_write.isEmpty()) readWrite(len_max);
    return buffer_write.writeArray(data, len_max);
  }

  bool begin() {
    in_channels = p_heavy->getNumInputChannels();
    out_channels = p_heavy->getNumOutputChannels();
    if (out_channels > 0) buffer_read.resize(buffer_size);
    if (in_channels > 0) buffer_write.resize(buffer_size);
    LOGI("channels: in=%d, out=%d", in_channels, out_channels);
    return in_channels > 0 || out_channels > 0;
  }

  void flush() { readWrite(buffer_write.available() / sample_size); }

  void end() {
    flush();
    buffer_read.resize(0);
    buffer_write.resize(0);
  }

 protected:
  HeavyContextInterface *p_heavy = nullptr;
  RingBuffer<uint8_t> buffer_write{0};
  RingBuffer<uint8_t> buffer_read{0};
  float volume = 1.0f;
  int buffer_size;
  const float max_int = 32767.0;
  const int sample_size = sizeof(int16_t);
  int in_channels = 0;
  int out_channels = 0;

  // returns bytes
  void readWrite(int bytes) {
    if (bytes == 0) return;
    int samples = bytes / sample_size;
    // size must be multiple of HV_N_SIMD
    samples = samples / HV_N_SIMD * HV_N_SIMD;
    int frames = samples / out_channels;

    float in[samples] = {0};
    float out[samples] = {0};

    // convert int16 to float
    if (buffer_write.size() > 0) {
      for (int j = 0; j < samples; j++) {
        int16_t tmp16 = 0;
        size_t eff = buffer_write.readArray((uint8_t *)&tmp16, sample_size);
        assert(eff == sample_size);
        in[j] = volume * tmp16 / max_int;
      }
    }

    // process data
    int frames_eff = p_heavy->processInlineInterleaved(in, out, frames);
    // LOGI("%d -> %d", frames, frames_eff);
    assert(frames == frames_eff);

    // convert intput to int16
    if (buffer_read.size() > 0) {
      for (int j = 0; j < samples; j++) {
        int16_t tmp16 = volume * out[j] * max_int;
        size_t eff = buffer_read.writeArray((uint8_t *)&tmp16, sample_size);
        assert(eff == sample_size);
      }
    }
  }
};