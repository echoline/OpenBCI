#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>
#include <math.h>

ALCdevice *device;
ALCcontext *context;
ALuint sources[2];
ALuint buffers[1];

int init_openal() {
	ALCenum error;
	int i;
	short bufferData[44100/440];

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

	alListener3f(AL_POSITION, 0, 0, 1.0f);
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

	alGenSources((ALuint)2, sources);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -7;

	for(i = 0; i < 2; i++) {
		alSourcef(sources[i], AL_PITCH, 1);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -8;
		alSourcef(sources[i], AL_GAIN, 1);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -9;
		alSourcei(sources[i], AL_LOOPING, AL_TRUE);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -10;
		alSource3f(sources[i], AL_VELOCITY, 0, 0, 0);
		error = alGetError();
		if (error != AL_NO_ERROR)
			return -11;
	}

	alSource3f(sources[0], AL_POSITION, -0.2, 0, -1);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -12;

	alSource3f(sources[1], AL_POSITION, 0.2, 0, -1);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -13;

	alGenBuffers((ALuint)1, buffers);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -28;

	for(i = 0; i < (44100/440); i++)
		bufferData[i] = (short)(32767.0 * sin(i * M_2_PI * 440.0 / 44100.0));
	alBufferData(buffers[0], AL_FORMAT_MONO16, bufferData, sizeof(bufferData), 44100);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -29;

	alSourcei(sources[0], AL_BUFFER, buffers[0]);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -30;

	alSourcei(sources[1], AL_BUFFER, buffers[0]);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -31;

	alSourcePlay(sources[0]);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -47;

	alSourcePlay(sources[1]);
	error = alGetError();
	if (error != AL_NO_ERROR)
		return -48;

	return 0;
}

void delete_openal() {
	alDeleteSources(2, sources);
	alDeleteBuffers(1, buffers);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}
