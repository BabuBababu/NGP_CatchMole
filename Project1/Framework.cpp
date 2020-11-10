#include "Framework.h"

Framework* gFramework = nullptr;

Framework::Framework(Scene* initScene)
{
	curScene = initScene;
	//커뮤니케이터 생성
	ctos = new ClientToServer();
	gs = new GameSate();
	communicator = new Communicator();
}
//프레임워크의 생성자에서 SDL 시스템을 사용하기 위한 초기화 작업과
//게임이 최초 시작시 진입할 씬을 지정하고, 구조체를 위한 메모리를 할당받는다.

Framework::~Framework()
{
	delete curScene;
	curScene = nullptr;
	//커뮤니케이터를 삭제한다.
	delete ctos;
	ctos = nullptr;
	delete gs;
	gs = nullptr;	
	delete communicator;
	communicator = nullptr;
}
//게임이 종료되고 프레임워크의 소멸자가 호출되면
//현재 씬을 삭제함으로써 씬의 소멸자를 통해 할당받은 모든
//메모리를 날리고 나머지 프레임워크에서 사용한 리소스를 할당해제한 후
//마지막으로 SDL 서브시스템을 종료한다.

void Framework::render()
{
	SDL_RenderClear(SimpleSDL::gRenderer);
	curScene->render();
	SDL_RenderPresent(SimpleSDL::gRenderer);
}

void Framework::communicateWithServ()
{
	communicator->sendData(*ctos);
	//서버에게 클라이언트의 정보를 보내고
	communicator->recvData(*gs);
	//서버에게 게임의 현황을 받아온다.
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
//handleEvent에선 이벤트를 하나씩 뽑아서 검사를 진행한다.
//x버튼을 누르거나 q키를 누르면 게임을 종료하기 위해 true를 반환한다.
//그리고 화면에 배치된 Editbox의 포커싱과 입력을 받는다.
//그리고 이벤트를 Scene의 handleEvent에 넘겨주어 적절한 처리를 진행케한다.

bool Framework::update()
{
	if (handleEvent()) return true;
	//핸들 이벤트를 통해 언제 어떤 두더지를 눌렀는지 검사하고 그 정보를 ctos에 넣는다.
	communicateWithServ();
	//서버와 통신을 수행한다.
	updateScene();
	//서버에게 받은 게임스테이트를 통해 씬을 업데이트한다.
	render();
	//화면에 그린다.
	return false;
}
//update할 땐 사용자 입력을 처리하고, 서버와 통신을 통해 업데이트 정보를 받아온 후
//씬을 업데이트하고 화면에 그린다.
//만약 게임이 종료 조건에 부합한다면 true를 반환해 게임이 종료되게 한다.

void Framework::changeScene(Scene* scene)
{
	delete curScene;
	curScene = nullptr;
	curScene = scene;
}
