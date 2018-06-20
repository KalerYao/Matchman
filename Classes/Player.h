/*
	�����
	������Ҳ��ݽ�ɫ�߼�
*/

#pragma once
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PlayerType.h"

USING_NS_CC;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;


enum RunDir
{
	RUNLEFT,
	STAND,
	RUNRIGHT
};


//�����
class Player : public Sprite
{
public:
	Player();
	~Player();
	static Player* create(int id, std::string type);
	static void LoadPlayerActionFromLua(const char* file);

	//������ת��
	bool TryTurnTo(std::string newAction);
	//ͨ��ActionID�õ�Action����
	static std::string getPlayerActionTypeByID(int actionID);
	static int getActionIDByActionType(std::string actionType);

	RunDir getRunDir();
	//����change�ı�dir
	void changeRunDir(int change);
	bool isActionEnd();

	//�����ж϶���
	void ReLoadAction();
private:
	//���ж�������
	static std::vector<std::string> m_playerActionType;
	//����ƶ���λ
	static float m_playerMoveUnit;
	//�����Ծ��λ
	static float m_playerJumpUnit;

	bool init(int id, std::string type);

	//�����Ϣ
	int m_playerID;
	std::string m_playerType;
	//����
	Armature *m_armature;
	//Ŀǰִ�еĶ���
	std::string m_nowAction;
	//�Ƿ���Դ�nowActionת����newAction
	bool CanTurnTo(std::string newAction);
	//ĳЩ����ѭ����Action��Ҫ�ȴ�end��������
	bool m_isActionEnd;
	//�ܶ�����
	RunDir m_runDir;
	//֡����
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	//����end�¼�
	static std::vector<std::string> m_endEvent;
	//��дupdate����
	void update(float dt) override;
};