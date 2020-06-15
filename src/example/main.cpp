#include "../EGE/engine.hpp"
#include "examplegame.hpp"

int main(int argc, char const *argv[])
{
    ege::Engine* engine = new ege::Engine("Test Game");
    engine->init();
    engine->start(800, 600, "Hello World!", new ExampleGame());
    return 0;
}
