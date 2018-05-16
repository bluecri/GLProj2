#include "../../stdafx.h"
#include "ALManager.h"

#include <al.h>
#include <alc.h>

#include "./ALSound.h"
#include "./ALSource.h"

void ALManager::loadAllWaveFile() {
	std::vector<std::pair<std::string, std::string>> listWaveFiles;
	listWaveFiles.push_back(std::make_pair("laser", "sound/springMono.wav"));
	listWaveFiles.push_back(std::make_pair("hit", "sound/hitMono.wav"));
	listWaveFiles.push_back(std::make_pair("explosion", "sound/explosion.wav"));

	for (std::vector<std::pair<std::string, std::string>>::iterator it = listWaveFiles.begin(); it != listWaveFiles.end(); ++it) {
		loadWaveFile(it->first, it->second);
	}

}

void ALManager::loadWaveFile(std::string soundName, std::string soundFileName) {
	ALSound *alSound = loadWAVE(soundFileName.c_str());
	alSound->allocBuffer(soundName);
	m_nameToALSound.insert(std::make_pair(soundName, alSound));
}

ALSource * ALManager::getNewALSource(std::string & soundName, Transform * transform, float pitch = 1.0f, float gain = 1.0f)
{
	ALSound* targetALSound = getALSoundPtrWithName(soundName);
	ALSource * newALSource = new ALSource(transform, pitch, gain);
	if (targetALSound == nullptr)
	{
		printf_s("[LOG] : Fail to find ALSound has %s name in ALManager::getNewALSource\n", soundName.c_str());
		return nullptr;
	}
	newALSource->bindSourceToALSound(targetALSound);

	_ALSourceContainer.push_back(newALSource);

	return newALSource;
}

void ALManager::removeALSource(ALSource * delSource)
{
	for (auto it = _ALSourceContainer.begin(); it != _ALSourceContainer.end();		)
	{
		if ((*it) == delSource)
		{
			_ALSourceContainer.erase(it);
			return;
		}
		++it;
	}
	printf_s("[LOG] : Fail remove alsource in ALManager::removeALSource\n");
	return;
}

//reference : https://www.gamedev.net/forums/topic/645923-loading-wav-file-with-openal-incorrect-audioformat/

ALSound * ALManager::loadWAVE(const char * filename) {
	FILE* fp = NULL;

	fp = fopen(filename, "rb");
	if (!fp) {
		cout << "Could NOT open: " << filename << "!" << endl;
		fclose(fp);
		return NULL;
	}

	char* ChunkID = new char[4];
	fread(ChunkID, sizeof(char), 4, fp);

	if (strncmp(ChunkID, "RIFF", 4)) {
		delete[] ChunkID;
		cout << "Not RIFF!" << endl;
		fclose(fp);
		return NULL;
	}

	fseek(fp, 8, SEEK_SET);
	char* Format = new char[4];
	fread(Format, 4, sizeof(char), fp);

	if (strncmp(Format, "WAVE", 4)) {
		delete[] ChunkID;
		delete[] Format;
		cout << "Not WAVE!" << endl;
		fclose(fp);
		return NULL;
	}

	char* SubChunk1ID = new char[4];
	fread(SubChunk1ID, 4, sizeof(char), fp);

	if (strncmp(SubChunk1ID, "fmt ", 3)) {
		delete[] ChunkID;
		delete[] Format;
		delete[] SubChunk1ID;
		cout << "Corrupt SubChunk1ID!" << endl;
		fclose(fp);
		return NULL;
	}

	unsigned int SubChunk1Size;
	fread(&SubChunk1Size, 1, sizeof(unsigned int), fp);
	unsigned int SubChunk2Location = (unsigned int)ftell(fp) + SubChunk1Size;

	// -------------------------------------- THIS PART

	unsigned short AudioFormat;
	fread(&AudioFormat, 1, sizeof(unsigned short), fp);

	if (AudioFormat != 1) { // AudioFormat = 85, should be 1
		delete[] ChunkID;
		delete[] Format;
		delete[] SubChunk1ID;
		cout << "Audio is NOT PCM!" << endl;
		fclose(fp);
		return NULL;
	}

	// --------------------------------------

	unsigned short NumChannels;
	fread(&NumChannels, 1, sizeof(unsigned short), fp);
	unsigned int SampleRate;
	fread(&SampleRate, 1, sizeof(unsigned int), fp);

	fseek(fp, 34, SEEK_SET);

	unsigned short BitsPerSample;
	fread(&BitsPerSample, 1, sizeof(unsigned short), fp);

	int ALFormat;
	if (NumChannels == 1 && BitsPerSample == 8) {
		ALFormat = AL_FORMAT_MONO8;
	}
	else if (NumChannels == 1 && BitsPerSample == 16) {
		ALFormat = AL_FORMAT_MONO16;
	}
	else if (NumChannels == 2 && BitsPerSample == 8) {
		ALFormat = AL_FORMAT_STEREO8;
	}
	else if (NumChannels == 2 && BitsPerSample == 16) {
		ALFormat = AL_FORMAT_STEREO16;
	}
	else {
		delete[] ChunkID;
		delete[] Format;
		delete[] SubChunk1ID;
		cout << "Audio is Not correctly formatted!" << endl;
		fclose(fp);
		return NULL;
	}

	fseek(fp, SubChunk2Location, SEEK_SET);
	char* SubChunk2ID = new char[4];
	fread(SubChunk2ID, 4, sizeof(char), fp);

	if (strncmp(SubChunk2ID, "data", 4)) {
		delete[] ChunkID;
		delete[] Format;
		delete[] SubChunk1ID;
		delete[] SubChunk2ID;
		cout << "Corrupt SubChunk2ID!" << endl;
		fclose(fp);
		return NULL;
	}

	unsigned int SubChunk2Size;
	fread(&SubChunk2Size, 1, sizeof(unsigned int), fp);

	unsigned char* Data = new unsigned char[SubChunk2Size];
	fread(Data, SubChunk2Size, sizeof(unsigned char), fp);

	delete[] ChunkID;
	delete[] Format;
	delete[] SubChunk1ID;
	delete[] SubChunk2ID;
	fclose(fp);

	return new ALSound(filename, Data, SubChunk2Size, ALFormat, SampleRate);

}

void ALManager::init() {
	alcDevice = alcOpenDevice(NULL);
	if (alcDevice) {
		alCcontext = alcCreateContext(alcDevice, NULL);
		alcMakeContextCurrent(alCcontext);
	}
	//al mode
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	loadAllWaveFile();
}

ALSound * ALManager::getALSoundPtrWithName(std::string soundName) {
	std::map<std::string, ALSound*>::iterator it = m_nameToALSound.find(soundName);
	if (it != m_nameToALSound.end()) {
		return it->second;
	}
	else {
		printf("error in getALSoundPtrWithName\n");
		return nullptr;
	}

}

ALManager::~ALManager() {
	alcMakeContextCurrent(NULL);
	alcDestroyContext(alCcontext);
	alcCloseDevice(alcDevice);
}
