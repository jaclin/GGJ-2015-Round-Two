#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <cmath>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 500;

SDL_Window* gWindow = nullptr;
SDL_Renderer* renderer = nullptr;

// Class Declaration

// Class Declaration
class Player
{
 public:

  // dimensions
  double pRadius = 10;
  double pPosX, pPosY;

  // Unique ID
  const int uid;

  Player(int, int, int,int );
  void render();
  void move();
  ~Player();

  private:
     // uid hack
    static int newUID;
    // colorsssss
    int velX = 25;
    int velY = 25;
    int red, blue;
};

int Player::newUID = 0;

Player::Player(int x, int y, int r = 0, int b = 0): uid(newUID++)
{
// Starting position
  pPosX = x;
  pPosY = y;
  red = r;
  blue = b;
}


void Player::move(){

  if (pPosX <= 0+pRadius)
  {
    this->pPosX = pRadius;
    velX = std::fabs(velX);
  }
  else if  (pPosX >= SCREEN_WIDTH-pRadius)
  {
    this->pPosX = SCREEN_WIDTH-pRadius;
    velX = -std::fabs(velX);
  }

  else if (pPosY <= 0+pRadius)
  {
    this->pPosY = pRadius;
    velY = std::fabs(velY);
  }
  else if (pPosY >= SCREEN_HEIGHT-pRadius)
  {
    this->pPosY = SCREEN_HEIGHT-pRadius;
    velY = -std::fabs(velY);
  }
  pPosX += velX;
  pPosY += velY;
}

void Player::render()
{
  filledCircleRGBA(renderer, pPosX, pPosY, pRadius, red, 0, blue, 255);
}

Player::~Player(){}

// SDL init(), load(), and close() from LazyFoo Productions
bool init()
{
  bool success = true;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    success = false;
  }
  else
  {
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
    {
      printf("Warning: Linear texture filtering not enabled!");
    }

    gWindow = SDL_CreateWindow("Pepero", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if(gWindow == NULL)
    {
      printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
      success = false;
    }
    else
    {
      renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

      if(renderer == NULL)
      {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        success = false;
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }
    }
  }
  return success;
}

void close()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  renderer = NULL;

  SDL_Quit();
}

int main(int argc, char* args[])
{
  bool quit = false;
  Player * player1 = new Player(12,12,255,0);

  if(!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    SDL_Event e;
    while(!quit)
    {
      uint capTimer = SDL_GetTicks();
      while(SDL_PollEvent(&e) != 0)
      {
        if(e.type == SDL_QUIT)
        {
          quit = true;
        }
        if (e.type == SDL_KEYDOWN)
          player1->move();
      }
      // Clear Screen
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
      // Render Objects
      player1->render();
      // Render other objects
      SDL_RenderPresent(renderer);
      //SDL_Delay(200);
    }
  }
  close();
  return 0;
}
