#pragma once

#include <vector>
#include <map>
#include <list>

class ALSound;
class ALSource;
class Transform;

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

class ALManager {
public:
	ALManager() {};
	virtual ~ALManager();
	void init();
	void loadAllWaveFile();

	void loadWaveFile(std::string soundName, std::string soundFileName);	// create ALSound

	ALSource* getNewALSource(std::string& soundName, Transform* transform, float pitch, float gain);	// create ALSource
	void removeALSource(ALSource* delSource);

private:
	ALSound* getALSoundPtrWithName(std::string soundName);
	ALSound* loadWAVE(const char* filename);	//reference : https://www.gamedev.net/forums/topic/645923-loading-wav-file-with-openal-incorrect-audioformat/


	ALCdevice*	alcDevice;
	ALCcontext*	alCcontext;
	std::map<std::string, ALSound*> m_nameToALSound;	// sound file
	std::list<ALSource*> _ALSourceContainer;				// AL source for game

};