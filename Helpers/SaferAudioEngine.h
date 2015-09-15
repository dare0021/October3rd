#ifndef __Slasher__SaferAudioEngine__
#define __Slasher__SaferAudioEngine__

#include <unordered_map>
#include "Consts.h"

///No pitch pan gain control on Win32
class SaferAudioEngine
{
public:
    SaferAudioEngine();

    int playSoundEffect(std::string path, bool loop = false);
    int playNamedEffect(std::string path, std::string name, bool loop = false);
    void pauseSoundEffect(int id);
    bool pauseNamedEffect(std::string name);
    void stopSoundEffect(int id);
    bool stopNamedEffect(std::string name);
    void resumeSoundEffect(int id);
    bool resumeNamedEffect(std::string name);
    
    void playBGM(std::string path, bool loop = true);
    void forcePlayBGM(std::string path, bool loop = true);
    void stopBGM();
    void pauseBGM();
    void resumeBGM();
    void rewindBGM();
    void loadSoundEffect(std::string path);
    void loadBGM(std::string path);
    bool unloadSoundEffect(std::string path);
    
    void resetSoundEffectVolume(float);
    float getSoundEffectVolume();
    void setBGMVolume(float);
    float getBGMVolume();
    bool isMuted();
    void stopAllEffects();
    void destroy();
    
private:
    //CocosDenshion::SimpleAudioEngine *engine;
    ///Prevent use of SimpleAudioEngine.h outside of this class
    void *engine;
    static std::vector<std::string> bgmVect, soundEffectVect;
    static std::unordered_map<std::string, int> namedSoundEffects;
    static std::string currentBGM;
    
    bool soundEffectLoaded(std::string path);
    bool bgmLoaded(std::string path);
};

#endif /* defined(__Slasher__SaferAudioEngine__) */