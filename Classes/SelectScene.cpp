#include "SelectScene.h"
#include "GameManager.h"
#include "MusicUtil.h"

bool SelectScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//����UI��
	auto UI = cocostudio::GUIReader::getInstance()->
		widgetFromJsonFile("UI/SelectSceneUI/Second_1.ExportJson");
	UI->setPosition(Point(0, 0));
	this->addChild(UI);

	//���VS��Ұ�ť
	auto p1 = (Button*)Helper::seekWidgetByName(UI, "P1");
	p1->addTouchEventListener(this, toucheventselector(SelectScene::PlayerVSPlayer));

	//���VS���԰�ť
	auto p2 = (Button*)Helper::seekWidgetByName(UI, "P2");
	p2->addTouchEventListener(this, toucheventselector(SelectScene::PlayerVSPC));

	//����
	auto setting = (Button*)Helper::seekWidgetByName(UI, "Button_6");
	setting->addTouchEventListener(this, toucheventselector(SelectScene::Setting));
	//��Ч
	auto music = (Button*)Helper::seekWidgetByName(UI, "Button_7");
	music->addTouchEventListener(this, toucheventselector(SelectScene::Music));

	return true;
}

Scene* SelectScene::createScene()
{
	auto layer = SelectScene::create();
	auto scene = Scene::create();
	scene->addChild(layer);
	return scene;
}

void SelectScene::PlayerVSPlayer(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
		auto GM = new GameManager();
		Director::getInstance()->replaceScene(GM->getGameScene());
	}
}

void SelectScene::PlayerVSPC(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
	}
}

void SelectScene::Setting(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
	}
}

void SelectScene::Music(Ref*, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		MusicUtil::playClickMusic();
	}
}
