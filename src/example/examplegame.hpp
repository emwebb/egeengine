#include "../EGE/game.hpp"

class ExampleGame : public ege::Game
{
private:
    /* data */
public:
    ExampleGame(/* args */);
    void loop(F32 deltaTime);
    ~ExampleGame();
};

ExampleGame::ExampleGame(/* args */)
{
}

void ExampleGame::loop(F32 deltaTime){

}

ExampleGame::~ExampleGame()
{
}
