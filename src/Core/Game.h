#pragma once
#include"includes.h"
#include"ResourcesManager.h"
#include"SceneManager.h"
#include"IWindows.h"
#include"EasyxWindow.h"
#include"EasyxRender.h"


class Game
{
public:
	Game();
	~Game();
	
	//������Ϸ�ĳ�ʼ�����п�ʼ����
	
	//��ʼ����Ϸ
	bool init_game();
	//����ȫ����Ϸ��Դ
	void load_resources();
	//��ʼ��Ϸ��ѭ��
	void start();
};

