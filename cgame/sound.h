#include <soundio/soundio.h>

#define SAMPLING_HZ 2048
#define WT_NONE     0x00
#define WT_SINE     0x01
#define WT_SQUARE   0x02
#define WT_TRIANGLE 0x03
#define WT_SAWTOOTH 0x04
#define WT_ABS_SINE 0x05
#define WV_MAX      0x7FFF
#define WV_HALF     0x3FFF
#define WV_NONE     0x0000
#define WV_INV_HALF 0xC001
#define WV_INV_MAX  0x8001
#define PI          3.1415926535
#define SQUARE_PI   9.8696044011
#define CUBE_PI     31.006276680
#define HALF_PI     1.5707963268
#define TWO_PI      6.2831853078
#define TWO_PI_INV  0.1591549431
#define AUDIO_BUFFER_SIZE 16
ALCdevice *device;
ALCcontext *context;
ALuint buffer[AUDIO_BUFFER_SIZE];
ALuint source[AUDIO_BUFFER_SIZE];
int next_source = 0;
 
inline float part_sin(float x)
{
    float square_x = x * x;
    float cube_x = square_x * x;

    return 1.0 + (2.0 / PI - 12.0 / SQUARE_PI) * square_x + (16.0 / CUBE_PI - 4.0 / SQUARE_PI) * cube_x;
}

float fast_sin(float x)
{
    float a = x * TWO_PI_INV;
    x -= (int)a * TWO_PI;
    if (x < 0.0)
        x += TWO_PI;
     
    if (x < HALF_PI)
        return part_sin(HALF_PI - x);
    else if (x < PI)
        return part_sin(x - HALF_PI);
    else if (x < 3.0 * HALF_PI)
        return -part_sin(3.0 * HALF_PI - x);
    else
        return -part_sin(x - 3.0 * HALF_PI);
}

void wg_sine(ALshort *data, unsigned short buffer_length, unsigned short frequency, unsigned short amplitude)
{
    for (int i = 0; i < buffer_length; ++i)
        {
            data[i] = fast_sin(6.282 * frequency * i / buffer_length) * amplitude; 
        }
}

void wg_square(ALshort *data, unsigned short buffer_length, unsigned short frequency, unsigned short amplitude)
{
    signed short y = -amplitude;
    unsigned short step = SAMPLING_HZ / frequency;

    for (int i = 0; i < buffer_length; i++)
    {
        if (!(i % step))
            y = -y;
        data[i] = y;
    }
}

void wg_triangle(ALshort *data, unsigned short buffer_length, unsigned short frequency, unsigned short amplitude)
{
    float y = -amplitude;
    float step = amplitude / ((float)SAMPLING_HZ / (float)frequency);

    for (int i = 0; i < buffer_length; i++)
    {
        y += step;
        if (y > amplitude)
        {
            step = -step;
            y = amplitude;
        }
        else if (y < -amplitude)
        {
            step = -step;
            y = -amplitude;
        }
        data[i] = (signed short)y;
    }
}

void wg_sawtooth(ALshort *data, unsigned short buffer_length, unsigned short frequency, unsigned short amplitude)
{
    float y = -amplitude;
    float step = amplitude / ((float)SAMPLING_HZ / (float)frequency);

    for (int i = 0; i < buffer_length; i++)
    {
        y += step;
        if (y > amplitude)
            y = -amplitude;
        data[i] = (signed short)y;
    }
}

void wg_abs_sine(ALshort *data, unsigned short buffer_length, unsigned short frequency, unsigned short amplitude)
{
    if (amplitude > 0)
        for (int i = 0; i < buffer_length; ++i)
            {
                data[i] = fast_sin(6.282 * frequency * i / buffer_length) * amplitude;
                if (data[i] < 0)
                    data[i] = -data[i];
            }
    else if (amplitude < 0)
        for (int i = 0; i < buffer_length; ++i)
            {
                data[i] = fast_sin(6.282 * frequency * i / buffer_length) * amplitude;
                if (data[i] > 0)
                    data[i] = -data[i];
            }
}

void queue_sound(unsigned short frequency, unsigned short duration, signed short amplitude = WV_MAX, unsigned short type = 0x00)
{
    int buffer_length = (int)((float)SAMPLING_HZ * (((float)duration)/500.0));
    ALshort data[buffer_length];
    
    switch (type)
    {
        case WT_SINE:
            wg_sine(data, buffer_length, frequency, amplitude); // FIX THIS!
        break;
        case WT_SQUARE:
            wg_square(data, buffer_length, frequency, amplitude);
        break;
        case WT_TRIANGLE:
            wg_triangle(data, buffer_length, frequency, amplitude);
        break;
        case WT_SAWTOOTH:
            wg_sawtooth(data, buffer_length, frequency, amplitude);
        break;
        case WT_ABS_SINE:
            wg_abs_sine(data, buffer_length, frequency, amplitude); // ALSO THIS!
        break;
            default:
        return;
    }
    
    int source_id = next_source;
    next_source = (next_source + 1) % AUDIO_BUFFER_SIZE;
    alDeleteSources(1, &(source[source_id]));
    alBufferData(buffer[source_id], AL_FORMAT_STEREO16, data, sizeof(data), SAMPLING_HZ);
    alGenSources(1, &(source[source_id]));
    alSourcei(source[source_id], AL_BUFFER, buffer[source_id]);
    alSourcei(source[source_id], AL_LOOPING, AL_FALSE);
    alSourcePlay(source[source_id]);
}

void init_sounds()
{
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);
    alGenBuffers(AUDIO_BUFFER_SIZE, buffer);
    alGenSources(AUDIO_BUFFER_SIZE, source);
}

void cleanup_sounds()
{
    for (int i = 0; i < AUDIO_BUFFER_SIZE; i++)
        alSourceStop(source[i]);
    alDeleteBuffers(AUDIO_BUFFER_SIZE, buffer);
    alDeleteSources(AUDIO_BUFFER_SIZE, source);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
