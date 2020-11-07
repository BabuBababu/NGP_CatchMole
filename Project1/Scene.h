#pragma once

#include "SimpleSDL.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual void render()=0;
	virtual void update()=0;
	virtual void handleEvnet(SDL_Event& e)=0;
};


class InitScene: public Scene
{
	InitScene() :Scene()
	{
		//�̰��� ���ӿ��� ����� ������Ʈ�� �߰��Ѵ�.
	}
	void render() override;
	void update() override;
	void handleEvnet(SDL_Event& e) override;
};