#pragma once

#if _WIN32 || _WIN64
    #if _WIN64
        #include "SFML64/Graphics.hpp"
    #else
        #include "SFML32/Graphics.hpp"
    #endif
#endif

class ray
{
public:
    ray();
    virtual ~ray();

    const bool castRay();
private:
    bool side = false;
    bool hit = false;
    sf::Vector2i step;
    float perpWallDist = 0.f;
    sf::Vector2f sideDist;
};

