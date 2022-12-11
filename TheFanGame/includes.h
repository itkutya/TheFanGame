#pragma once

//TODO:
//c++23 make it into a modular include...
//Inlcude only those that this need...

#include <unordered_map>
#include <chrono>
#include <string>
#include <vector>
#include <variant>
#include <future>
#include <mutex>
#include <utility>
#include <stack>
#include <memory>
#include <cstdio>
#include <array>
#include <fstream>
#include <atomic>
#include <thread>
#include <math.h>
#include <random>
#include <exception>
#include <optional>

#include <ctime>
#include <iomanip>
#include <sstream>

#if _WIN32 || _WIN64
	#if _WIN64
		#include "SFML64/SFML/Graphics.hpp"
		#include "SFML64/SFML/Audio.hpp"
		#include "SFML64/SFML/Network.hpp"
		#include "SFML64/SFML/System.hpp"
	#else
		#include "SFML32/Graphics.hpp"
		#include "SFML32/Audio.hpp"
		#include "SFML32/Network.hpp"
		#include "SFML32/System.hpp"
	#endif
#endif