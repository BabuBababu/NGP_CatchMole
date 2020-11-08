#include "Framework.h"

int main(int argc, char* args[])
{
	SimpleSDL::init(600, 600);
	TestScene* initScene = new TestScene();
	gFramework = new Framework(initScene);
	//�����ӿ�ũ�� �ʱ� ������ ������ ������ �ȵǾ��ִ� �����̴�.
	//initScene���� ����ڿ��� ������ �ּҿ� ��Ʈ��ȣ�� �Է¹ް�
	//�����ӿ�ũ�� connectingToServ �Լ��� ȣ���Ͽ� �����Ѵ�.
	while (!gFramework->update());
	delete gFramework;
	SimpleSDL::quit();
	return 0;
}