#include "GameEngine.h"
#include "CommandProcessing.h"

int main()
{
      GameEngine *ge = new GameEngine();
      GameEngine::setComProc(new CommandProcessor());
      ge->gameLoop();
}