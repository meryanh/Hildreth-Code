/*
 * Copyright (c) 2015 Andrew Kelley
 *
 * This file is part of libsoundio, which is MIT licensed.
 * See http://opensource.org/licenses/MIT
 */

#include <soundio/soundio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

static void write_sample_s16ne(char *ptr, float sample) {
    int16_t *buf = (int16_t *)ptr;
    float range = (float)INT16_MAX - (float)INT16_MIN;
    float val = sample * range / 2.0;
    *buf = val;
}

static void write_sample_s32ne(char *ptr, float sample) {
    int32_t *buf = (int32_t *)ptr;
    float range = (float)INT32_MAX - (float)INT32_MIN;
    float val = sample * range / 2.0;
    *buf = val;
}

static void write_sample_float32ne(char *ptr, float sample) {
    float *buf = (float *)ptr;
    *buf = sample;
}

static void write_sample_float64ne(char *ptr, float sample) {
    float *buf = (float *)ptr;
    *buf = sample;
}

class Tone
{
public:
    float frequency;
    float duration;
    float amplitude;
    int type;
    Tone(float _frequency = 0.0, float _duration = 0.0, float _amplitude = 0.0, int _type = 0)
    {
        frequency = _frequency;
        duration = _duration;
        amplitude = _amplitude;
        type = _type;
    }
};

class SoundData
{
public:
    Tone *track;
    int length;
    bool paused;
    bool end;
    
    SoundData()
    {
        track = NULL;
        length = 0;
        paused = false;
        end = false;
    }
};

static void (*write_sample)(char *ptr, float sample);
#define PI 3.1415926
static volatile bool want_pause = false;
struct SoundIoOutStream *sound_outstream[16] = { NULL };
struct SoundData sound_outstream_data[16];
struct SoundIo *soundio = NULL;
struct SoundIoDevice *device = NULL;

static void wt_sine(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max) {
    float float_sample_rate = outstream->sample_rate;
    float seconds_per_frame = 1.0 / float_sample_rate;
    struct SoundIoChannelArea *areas;

    float pitch = 200.0;
    float radians_per_second = pitch * 2.0 * PI;
    int frame_count = frame_count_max;
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    soundio_outstream_begin_write(outstream, &areas, &frame_count);
    
    // Change this to get 
    
    for (int frame = 0; frame < frame_count; frame += 1) {
        float sample = sin(frame * seconds_per_frame * radians_per_second);
        for (int channel = 0; channel < layout->channel_count; channel += 1) {
            write_sample(areas[channel].ptr, sample);
            areas[channel].ptr += areas[channel].step;
        }
    }
    
    ////////////////////////////////////////
    
    soundio_outstream_end_write(outstream);
    
    soundio_outstream_pause(outstream, want_pause);
}

void init_audio()
{
    enum SoundIoBackend backend = SoundIoBackendNone;
    char *device_id = NULL;
    bool raw = false;
    char *stream_name = NULL;
    soundio = soundio_create();
    backend == SoundIoBackendNone ? soundio_connect(soundio) : soundio_connect_backend(soundio, backend);
    soundio_flush_events(soundio);

    int selected_device_index = -1;
    if (device_id) {
        int device_count = soundio_output_device_count(soundio);
        for (int i = 0; i < device_count; i += 1) {
            struct SoundIoDevice *_device = soundio_get_output_device(soundio, i);
            bool select_this_one = strcmp(_device->id, device_id) == 0 && _device->is_raw == raw;
            soundio_device_unref(_device);
            if (select_this_one) {
                selected_device_index = i;
                break;
            }
        }
    } else {
        selected_device_index = soundio_default_output_device_index(soundio);
    }

    device = soundio_get_output_device(soundio, selected_device_index);
    for (int i = 0; i < 16; i++)
    {
        sound_outstream[i] = soundio_outstream_create(device);
        sound_outstream[i]->name = stream_name;
        if (soundio_device_supports_format(device, SoundIoFormatFloat32NE)) {
            sound_outstream[i]->format = SoundIoFormatFloat32NE;
            write_sample = write_sample_float32ne;
        } else if (soundio_device_supports_format(device, SoundIoFormatFloat64NE)) {
            sound_outstream[i]->format = SoundIoFormatFloat64NE;
            write_sample = write_sample_float64ne;
        } else if (soundio_device_supports_format(device, SoundIoFormatS32NE)) {
            sound_outstream[i]->format = SoundIoFormatS32NE;
            write_sample = write_sample_s32ne;
        } else if (soundio_device_supports_format(device, SoundIoFormatS16NE)) {
            sound_outstream[i]->format = SoundIoFormatS16NE;
            write_sample = write_sample_s16ne;
        }
        soundio_outstream_open(sound_outstream[i]);
    }
}

void play_audio(void (*callback)(struct SoundIoOutStream *outstream, int frame_count_min, int frame_count_max))
{
    sound_outstream[0]->write_callback = callback;
    soundio_outstream_start(sound_outstream[0]);
}

int main(int argc, char **argv)
{
    init_audio();
    play_audio(wt_sine);
    
    for (;;) {
        soundio_flush_events(soundio);
        int c = getc(stdin);
        if (c == 'p') {
            soundio_outstream_pause(sound_outstream[0], true);
        } else if (c == 'P') {
            want_pause = true;
        } else if (c == 'u') {
            want_pause = false;
            soundio_outstream_pause(sound_outstream[0], false);
        } else if (c == 'c') {
            soundio_outstream_clear_buffer(sound_outstream[0]);
        } else if (c == 'q') {
            break;
        }
    }

    for (int i = 0; i < 16; i++)
    soundio_outstream_destroy(sound_outstream[i]);
    soundio_device_unref(device);
    soundio_destroy(soundio);
    return 0;
}