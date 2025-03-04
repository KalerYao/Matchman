#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;
USING_NS_CC;

class GameUI : public Layer
{
public:
	CREATE_FUNC(GameUI);
	bool init() override;
	void SetHealthByID(int id, int health);
	void SetPowerByID(int id, int power);
	void SetScoreByID(int id,int score);
private:
	std::vector<ImageView*> m_healths;
	std::vector<ImageView*> m_powers;
	std::vector<ImageView*> m_maxShows;
	void PauseGame(Ref*, TouchEventType type);
	void CloseGame(Ref*, TouchEventType type);
	void ResetGame(Ref*, TouchEventType type);
};