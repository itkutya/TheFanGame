#include "ResourceManager.h"

ResourceManager& ResourceManager::getInstance()
{
    static ResourceManager instance;
    return instance;
}