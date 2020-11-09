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


class TestScene: public Scene
{
public:
	TestScene();
	~TestScene();
	void render() override;
	void update() override;
	void handleEvnet(SDL_Event& e) override;
private:
	std::vector<SimpleSDL::WAV>tests;
	SimpleSDL::Music musik;
	SimpleSDL::Image Zergling;
	std::vector<SimpleSDL::Image>Hydra;
};