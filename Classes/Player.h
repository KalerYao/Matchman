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

//�ܶ�����
enum RunDir
{
	RUNLEFT,
	STAND,
	RUNRIGHT
};

//ս��״̬
enum BattleState
{
	NONE,
	ATTACK,
	UATTACK,
	HURT
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

	//�����ж϶��� run stand
	void ReLoadAction();

	//�õ�����ķ���-1 �� 1 ��
	int GetTowards();
	//�õ�����״̬
	BattleState GetBattleState();
	//ǿ�ƴ�϶�������
	void ForceToHurt(int damage);
	//�õ���������λ��
    float GetAnimationPositionX();
	
	static int GetPunchHurt() { return m_punchHurt; }
	static int GetKickHurt() { return m_kickHurt; }
	static int GetUltimateSkillHurt() { return m_ultimateSkillHurt; }

	//�õ�����
	Sprite* getAnimature();
private:
	//���ж�������
	static std::vector<std::string> m_playerActionType;
	//����ƶ���λ
	static float m_playerMoveUnit;
	//�����Ծ��λ
	static float m_playerJumpUnit;
	//�����Ѫ��
	static int m_maxHealth;
	//����ͷŴ�����Ҫ����
	static int m_maxUltimateSlillNeed;
	//��Ҽ����˺�
	static int m_punchHurt;
	static int m_kickHurt;
	static int m_ultimateSkillHurt;
	//ʹ��С�������ӵ�����
	static int m_addPower;

	bool init(int id, std::string type);

	//�����Ϣ
	int m_playerID;
	std::string m_playerType;
	//���Ѫ�� ����
	int m_nowHealth;
	int m_nowPower;
	//ս��״̬
	BattleState m_battleState;
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
	//�Ƿ�����Ծ״̬
	bool m_isJump;
	//�Ƿ�������״̬
	bool m_isFall;
	//�鿴�Ƿ񳬳���Χ
	void checkRange();
	//���Ÿ���UI��Ϣ
	void SendToUpateUI();
	//����Ч��
	ParticleSystemQuad* m_hurtEffect;
};