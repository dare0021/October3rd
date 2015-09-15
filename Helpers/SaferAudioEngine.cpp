#include "SaferAudioEngine.h"
#include "Consts.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

std::vector<std::string> SaferAudioEngine::bgmVect, SaferAudioEngine::soundEffectVect;
std::unordered_map<std::string, int> SaferAudioEngine::namedSoundEffects;
std::string SaferAudioEngine::currentBGM = "";

SaferAudioEngine::SaferAudioEngine() :
engine(SimpleAudioEngine::getInstance())
{
}

void SaferAudioEngine::destroy()
{
    ((SimpleAudioEngine*)engine)->end();
}

#ifndef NO_SOUND

int SaferAudioEngine::playSoundEffect(std::string path, float gain, bool loop)
{
    CCASSERT(soundEffectLoaded(path), "sound effect was not loaded");
    CCASSERT(0 <= gain && gain <= 1, "gain should be [0, 1]");
    return ((SimpleAudioEngine*)engine)->playEffect((SOUND_PATH+path).c_str(), loop, 1, 0, gain);
}

///Will not play if the effect is already present
int SaferAudioEngine::playNamedEffect(std::string path, std::string name, float gain, bool loop)
{
    if(namedSoundEffects.find(name) != namedSoundEffects.end())
        return namedSoundEffects.find(name)->second;
    int id = playSoundEffect(path, gain, loop);
    namedSoundEffects[name] = id;
    return id;
}

void SaferAudioEngine::pauseSoundEffect(int id)
{
    ((SimpleAudioEngine*)engine)->pauseEffect((unsigned int)id);
}

bool SaferAudioEngine::pauseNamedEffect(std::string name)
{
    if(namedSoundEffects.find(name) == namedSoundEffects.end())
        return false;
    pauseSoundEffect(namedSoundEffects.find(name)->second);
    return true;
}

void SaferAudioEngine::stopSoundEffect(int id)
{
    ((SimpleAudioEngine*)engine)->stopEffect((unsigned int)id);
}

bool SaferAudioEngine::stopNamedEffect(std::string name)
{
    if(namedSoundEffects.find(name) == namedSoundEffects.end())
        return false;
    stopSoundEffect(namedSoundEffects.find(name)->second);
    namedSoundEffects.erase(name);
    return true;
}

void SaferAudioEngine::resumeSoundEffect(int id)
{
    ((SimpleAudioEngine*)engine)->resumeEffect((unsigned int)id);
}

bool SaferAudioEngine::resumeNamedEffect(std::string name)
{
    if(namedSoundEffects.find(name) == namedSoundEffects.end())
        return false;
    resumeSoundEffect(namedSoundEffects.find(name)->second);
    return true;
}

///Will not play if the same BGM is already playing
void SaferAudioEngine::playBGM(std::string path, bool loop)
{
    CCASSERT(bgmLoaded(path), "BGM was not loaded");
    if(currentBGM != path)
        forcePlayBGM(path, loop);
}

void SaferAudioEngine::forcePlayBGM(std::string path, bool loop)
{
    CCASSERT(bgmLoaded(path), "BGM was not loaded");
    currentBGM = path;
    stopBGM();
    ((SimpleAudioEngine*)engine)->playBackgroundMusic((SOUND_PATH+path).c_str(), loop);
}

void SaferAudioEngine::stopBGM()
{
    ((SimpleAudioEngine*)engine)->stopBackgroundMusic(false);
}

void SaferAudioEngine::pauseBGM()
{
    ((SimpleAudioEngine*)engine)->pauseBackgroundMusic();
}

void SaferAudioEngine::resumeBGM()
{
    ((SimpleAudioEngine*)engine)->resumeBackgroundMusic();
}

void SaferAudioEngine::rewindBGM()
{
    ((SimpleAudioEngine*)engine)->rewindBackgroundMusic();
}

void SaferAudioEngine::loadSoundEffect(std::string path)
{
    ((SimpleAudioEngine*)engine)->preloadEffect((SOUND_PATH+path).c_str());
    soundEffectVect.push_back(path);
}

void SaferAudioEngine::loadBGM(std::string path)
{
    ((SimpleAudioEngine*)engine)->preloadBackgroundMusic((SOUND_PATH+path).c_str());
    bgmVect.push_back(path);
}

bool SaferAudioEngine::soundEffectLoaded(std::string path)
{
    for (std::string s : soundEffectVect)
    {
        if(s == path)
            return true;
    }
    return false;
}

bool SaferAudioEngine::bgmLoaded(std::string path)
{
    for (std::string s : bgmVect)
    {
        if(s == path)
            return true;
    }
    return false;
}

bool SaferAudioEngine::unloadSoundEffect(std::string path)
{
    int toRemove = 0;
    for(std::string s : soundEffectVect)
    {
        if(s == path)
            break;
    }
    if(toRemove < soundEffectVect.size())
    {
        soundEffectVect.erase(soundEffectVect.begin() + toRemove);
    }
    else
        return false;
    ((SimpleAudioEngine*)engine)->unloadEffect((SOUND_PATH+path).c_str());
    return true;
}

void SaferAudioEngine::resetSoundEffectVolume(float v)
{
    CCASSERT(0 <= v && v <= 1, "volume must be [0, 1]");
    ((SimpleAudioEngine*)engine)->setEffectsVolume(v);
}

float SaferAudioEngine::getSoundEffectVolume()
{
    return ((SimpleAudioEngine*)engine)->getEffectsVolume();
}

void SaferAudioEngine::setBGMVolume(float v)
{
    CCASSERT(0 <= v && v <= 1, "volume must be [0, 1]");
    ((SimpleAudioEngine*)engine)->setBackgroundMusicVolume(v);
}

float SaferAudioEngine::getBGMVolume()
{
    return ((SimpleAudioEngine*)engine)->getBackgroundMusicVolume();
}

bool SaferAudioEngine::isMuted()
{
    return !(getBGMVolume() || getSoundEffectVolume());
}

void SaferAudioEngine::stopAllEffects()
{
    ((SimpleAudioEngine*)engine)->stopAllEffects();
    namedSoundEffects.clear();
}