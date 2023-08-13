#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FREQ 220
#define RATE 44100
#define LEN RATE

ALCdevice *device;
ALCcontext *context;
ALuint sources[16];
ALuint buffers[16];
short **freqs;

int init_openal() {
	ALCenum error;
	int i, j;

	freqs = malloc(sizeof(short*) * 126);
	for(j = 0; j < 126; j++) {
		freqs[j] = malloc(sizeof(short) * LEN);
		for(i = 0; i < LEN; i++)
			freqs[j][i] = (short)(32767.0 * sin(i * M_PI * 2.0 * (FREQ + j * 3) / LEN));
	}

	device = alcOpenDevice(NULL);
	if (device == NULL)
		return -1;

	context = alcCreateContext(device, NULL);
	if (alcMakeContextCurrent(context) == 0)
		return -2;

	error = alGetError();
	if (error != AL_NO_ERROR)
		return -3;


	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	alListener3f(AL_POSITION, 0, 0, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -4;
	alListener3f(AL_VELOCITY, 0, 0, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -5;
	alListenerfv(AL_ORIENTATION, listenerOri);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -6;

	alGenSources((ALuint)16, sources);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -7;

	for(i = 0; i < 16; i++) {
		alSourcef(sources[i], AL_PITCH, 1);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -8;
		alSourcef(sources[i], AL_GAIN, 1);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -9;
		alSourcei(sources[i], AL_LOOPING, AL_FALSE);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -10;
		alSource3f(sources[i], AL_VELOCITY, 0, 0, 0);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -11;
	}

	alSource3f(sources[0], AL_POSITION, -0.05, 0, -0.1);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -12;

	alSource3f(sources[1], AL_POSITION, 0.05, 0, -0.1);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -13;

	alSource3f(sources[2], AL_POSITION, -0.075, 0.1, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -14;

	alSource3f(sources[3], AL_POSITION, 0.075, 0.1, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -15;

	alSource3f(sources[4], AL_POSITION, -0.075, 0, -0.075);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -16;

	alSource3f(sources[5], AL_POSITION, 0.075, 0, -0.075);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -17;

	alSource3f(sources[6], AL_POSITION, -0.05, 0, 0.1);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -18;

	alSource3f(sources[7], AL_POSITION, 0.05, 0, 0.1);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -19;

	alSource3f(sources[8], AL_POSITION, -0.075, 0, 0.075);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -20;

	alSource3f(sources[9], AL_POSITION, 0.075, 0, 0.075);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -21;

	alSource3f(sources[10], AL_POSITION, -0.05, 0.05, -0.05);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -22;

	alSource3f(sources[11], AL_POSITION, 0.05, 0.05, -0.05);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -23;

	alSource3f(sources[12], AL_POSITION, -0.1, 0, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -24;

	alSource3f(sources[13], AL_POSITION, 0.1, 0, 0);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -25;

	alSource3f(sources[14], AL_POSITION, -0.05, 0.05, 0.05);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -26;

	alSource3f(sources[15], AL_POSITION, 0.05, 0.05, 0.05);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -27;

	alGenBuffers((ALuint)16, buffers);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -28;

	for(i = 0; i < 16; i++) {
		alBufferData(buffers[i], AL_FORMAT_MONO16, freqs[i], LEN * sizeof(short), LEN);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -(29 + i);

		alSourceQueueBuffers(sources[i], 1, &buffers[i]);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -(29 + 16 + i);

		alSourcePlay(sources[i]);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -(29 + 32 + i);
	}

	return 0;
}

void set_openal(double **mat) {
	ALuint unqueuedBuffer;
	short bufferData[LEN];
	int i, j, k;

	for(k = 0; k < 16; k++) {
		memset(bufferData, 0, sizeof(bufferData));

		for(j = 0; j < 126; j++) {
			for(i = 0; i < LEN; i++)
				bufferData[i] += (mat[9][j] / (300.0 * 16.0)) * freqs[j][i];
		}

		alSourceStop(sources[k]);
		alSourceUnqueueBuffers(sources[k], 1, &unqueuedBuffer);
		alBufferData(buffers[k], AL_FORMAT_MONO16, bufferData, LEN * sizeof(short), LEN);
		alSourceQueueBuffers(sources[k], 1, &buffers[k]);
		alSourcePlay(sources[k]);
	}
}

void delete_openal() {
	alDeleteSources(1, sources);
	alDeleteBuffers(1, buffers);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
