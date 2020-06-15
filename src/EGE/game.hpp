#pragma once

#include "Utils/types.hpp"
namespace ege
{
    class Engine;

    class Game
    {
    private:
        Engine* engine;
    public:
        void init(Engine* engine);
        virtual void loop(F32 deltaTime) = 0;
    };
}
