#include "CharactersManager.h"

CharactersManager& CharactersManager::getInstance()
{
	static CharactersManager instance;
	return instance;
}