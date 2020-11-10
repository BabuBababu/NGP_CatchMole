#include "Framework.h"

Framework* gFramework = nullptr;

Framework::Framework(Scene* initScene)
{
	curScene = initScene;
	//Ŀ�´������� ����
	ctos = new ClientToServer();
	gs = new GameSate();
	communicator = new Communicator();
}
//�����ӿ�ũ�� �����ڿ��� SDL �ý����� ����ϱ� ���� �ʱ�ȭ �۾���
//������ ���� ���۽� ������ ���� �����ϰ�, ����ü�� ���� �޸𸮸� �Ҵ�޴´�.

Framework::~Framework()
{
	delete curScene;
	curScene = nullptr;
	//Ŀ�´������͸� �����Ѵ�.
	delete ctos;
	ctos = nullptr;
	delete gs;
	gs = nullptr;	
	delete communicator;
	communicator = nullptr;
}
//������ ����ǰ� �����ӿ�ũ�� �Ҹ��ڰ� ȣ��Ǹ�
//���� ���� ���������ν� ���� �Ҹ��ڸ� ���� �Ҵ���� ���
//�޸𸮸� ������ ������ �����ӿ�ũ���� ����� ���ҽ��� �Ҵ������� ��
//���������� SDL ����ý����� �����Ѵ�.

void Framework::render()
{
	SDL_RenderClear(SimpleSDL::gRenderer);
	curScene->render();
	SDL_RenderPresent(SimpleSDL::gRenderer);
}

void Framework::communicateWithServ()
{
	communicator->sendData(*ctos);
	//�������� Ŭ���̾�Ʈ�� ������ ������
	communicator->recvData(*gs);
	//�������� ������ ��Ȳ�� �޾ƿ´�.
}

void Framework::updateScene()
{
	curScene->update();
}

bool Framework::connectingToServ(const std::string& servAddr,const std::string& portNum)
{
	if (!communicator->connectToServ(servAddr, portNum)) return false;
	return true;
}

bool Framework::handleEvent()
{
	SDL_StartTextInput();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT||e.key.keysym.sym==SDLK_q)
		{
			return true;
		}
		for (auto& box : SimpleSDL::EditBox::boxes)
		{
			box->handleEvent(e);
		}
		curScene->handleEvnet(e);
	}
	return false;
}
//handleEvent���� �̺�Ʈ�� �ϳ��� �̾Ƽ� �˻縦 �����Ѵ�.
//x��ư�� �����ų� qŰ�� ������ ������ �����ϱ� ���� true�� ��ȯ�Ѵ�.
//�׸��� ȭ�鿡 ��ġ�� Editbox�� ��Ŀ�̰� �Է��� �޴´�.
//�׸��� �̺�Ʈ�� Scene�� handleEvent�� �Ѱ��־� ������ ó���� �������Ѵ�.

bool Framework::update()
{
	if (handleEvent()) return true;
	//�ڵ� �̺�Ʈ�� ���� ���� � �δ����� �������� �˻��ϰ� �� ������ ctos�� �ִ´�.
	communicateWithServ();
	//������ ����� �����Ѵ�.
	updateScene();
	//�������� ���� ���ӽ�����Ʈ�� ���� ���� ������Ʈ�Ѵ�.
	render();
	//ȭ�鿡 �׸���.
	return false;
}
//update�� �� ����� �Է��� ó���ϰ�, ������ ����� ���� ������Ʈ ������ �޾ƿ� ��
//���� ������Ʈ�ϰ� ȭ�鿡 �׸���.
//���� ������ ���� ���ǿ� �����Ѵٸ� true�� ��ȯ�� ������ ����ǰ� �Ѵ�.

void Framework::changeScene(Scene* scene)
{
	delete curScene;
	curScene = nullptr;
	curScene = scene;
}
