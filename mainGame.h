#ifndef MAINGAME_H
#define MAINGAME_H


#include "Breakout.h"
class mainGame : public Engine::Breakout{
public :
	mainGame();
	~mainGame();

	void OnUserDefinedEvent(SDL_Event & evt);
};

#endif