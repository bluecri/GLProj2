#pragma once

#include <vector>
#include <map>
#include <list>

class ALSound;
class ALSource;
class ALListener;
class RigidbodyComponent;

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

class ALManager {
public:
	ALManager() {};
	virtual ~ALManager();

	void init();
	void loadAllWaveFile();

	void loadWaveFile(std::string soundName, std::string soundFileName);	// create ALSound

	ALSource* getNewALSource(std::string& soundName, RigidbodyComponent* rigidbodyComponent, float pitch = 1.0f, float gain = 1.0f);	// create ALSource
	void updateALSource();
	void updateALListenerWithWorldMat(const glm::mat4 & worldMat);

private:
	ALSound* getALSoundPtrWithName(std::string& soundName);
	ALSound* loadWAVE(const char* filename);	//reference : https://www.gamedev.net/forums/topic/645923-loading-wav-file-with-openal-incorrect-audioformat/


	ALCdevice*	alcDevice;
	ALCcontext*	alCcontext;
	ALListener* alListener;

	std::map<std::string, ALSound*> m_nameToALSound;	// sound file
	std::list<ALSource*> _ALSourceContainer;				// AL source for game

};

extern ALManager *GALManager;