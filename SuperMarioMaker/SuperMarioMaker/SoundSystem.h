#pragma once
struct SoundData
{
	Sound* pSound;
	Channel* pChannel;
};

class SoundSystem
{
private:
	static SoundSystem* Inst;
	SoundSystem();

	unsigned int m_BGMTrackCount;
	vector<SoundData> m_BGM;
	unsigned int m_effectTrackCount;
	vector<SoundData> m_effect;

	SOUDN_BGM m_curPlayBGM;
	SOUND_EFFRCT m_curPlayEffect;
public:
	static SoundSystem* getInstance();

	System* pSystem;

	void LoadSoundData();
	void StartBGM(SOUDN_BGM);
	void StopBGM();
	void StartEffect(SOUND_EFFRCT);
	void StopEffect();

	~SoundSystem();
};