#include "Framework.h"

int main(int argc, char* args[])
{
	SimpleSDL::init(1280, 768);
	InitScene* initScene = new InitScene();
	gFramework = new Framework(initScene);
	//�����ӿ�ũ�� �ʱ� ������ ������ ������ �ȵǾ��ִ� �����̴�.
	//initScene���� ����ڿ��� ������ �ּҿ� ��Ʈ��ȣ�� �Է¹ް�
	//�����ӿ�ũ�� connectingToServ �Լ��� ȣ���Ͽ� �����Ѵ�.
	while (!gFramework->update());
	delete gFramework;
	SimpleSDL::quit();
	return 0;
}