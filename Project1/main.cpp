#include "Framework.h"

int main(int argc, char* args[])
{
	SimpleSDL::init(1280, 768);
	InitScene* initScene = new InitScene();
	gFramework = new Framework(initScene);
	//프레임워크는 초기 생성시 서버와 연결이 안되어있는 상태이다.
	//initScene에서 사용자에게 서버의 주소와 포트번호를 입력받고
	//프레임워크의 connectingToServ 함수를 호출하여 연결한다.
	while (!gFramework->update());
	delete gFramework;
	SimpleSDL::quit();
	return 0;
}