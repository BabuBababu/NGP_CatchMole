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
		//이곳에 게임에서 사용할 오브젝트를 추가한다.
	}
	void render() override;
	void update() override;
	void handleEvnet(SDL_Event& e) override;
};