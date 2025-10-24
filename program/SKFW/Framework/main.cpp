#include<SKFW/Framework/GameManager.hpp>

#ifdef _DEBUG
#define ENTRY_POINT int main()
#else
#include<Windows.h>
#define ENTRY_POINT int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#endif // _DEBUG

#include<SKFW/Framework/SoundManager.hpp>

ENTRY_POINT
{
	auto& game = GameManager::Instance();
	game.Initialize();
	game.Run();
	game.Finalize();
}