#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <string>

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 500;

SDL_Window* gWindow = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* pattern = nullptr;

// Class Declaration
class Player
{
 public:

  // Unique ID
  const int uid;

  Player(int, int, int,int );
  void render();
  void move(int);
  ~Player();

  private:
     // uid hack
    static int newUID;
    // colorsssss
    int velX = 25;
    int velY = 25;
    int red, blue;
      // dimensions
    double pRadius = 10;
    double pPosX, pPosY;
};

class Tile
{
  public:
  // Unique ID
  const int uid;

  int red = 255;
  int blue = 255;

  Tile(int,int);
  void render();
  ~Tile();

  private:
   static int newUID;
   int tPosX, tPosY;
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

void Player::move(int direction){

  if (direction == 0)
   pPosX += velX;
  if (direction == 1)
   pPosX -= velX;
  if (direction == 2)
   pPosY += velY;
  if (direction == 3)
   pPosY -= velY;

  if (pPosX <= 0+pRadius)
  {
    this->pPosX = pRadius;
  }
  else if  (pPosX >= SCREEN_WIDTH-pRadius)
  {
    this->pPosX = SCREEN_WIDTH-pRadius;
  }
  else if (pPosY <= 0+pRadius)
  {
    this->pPosY = pRadius;
  }
  else if (pPosY >= SCREEN_HEIGHT-pRadius)
  {
    this->pPosY = SCREEN_HEIGHT-pRadius;
  }

}

void Player::render()
{
  filledCircleRGBA(renderer, pPosX, pPosY, pRadius, red, 0, blue, 255);
}

Player::~Player(){}

int Tile::newUID = 0;

Tile::Tile(int x, int y): uid(newUID++)
{
// Starting position
  tPosX = x;
  tPosY = y;
}

void Tile::render(){

}

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

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

bool loadPatternOne()
{
	//Loading success flag
	bool success = true;

	//Load texture
	pattern = loadTexture( "viewport.png" );
	if( pattern == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	//Nothing to load
	return success;
}

bool loadPatternTwo()
{
	//Loading success flag
	bool success = true;

	//Load texture
	pattern = loadTexture( "water_PNG3319.png" );
	if( pattern == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	//Nothing to load
	return success;
}

void close()
{
    SDL_DestroyTexture(pattern);
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
  Player * player2 = new Player(SCREEN_WIDTH-12,SCREEN_HEIGHT-12,0,255);

  if(!init())
  {
    printf("Failed to initialize!\n");
  }
  else
  {
    if( !loadPatternOne() )
		{
			printf( "Failed to load media!\n" );
		}
    else
    {
     SDL_Event e;
    while(!quit)
      {
      uint capTimer = SDL_GetTicks();
      while(SDL_PollEvent(&e) != 0)
      {
        switch( e.type ){
            /* Look for a keypress */
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( e.key.keysym.sym ){
                    case SDLK_LEFT:
                        player1->move(1);
                        break;
                    case SDLK_RIGHT:
                        player1->move(0);
                        break;
                    case SDLK_UP:
                        player1->move(3);
                        break;
                    case SDLK_DOWN:
                        player1->move(2);
                        break;
                    case SDLK_a:
                        player2->move(1);
                        break;
                    case SDLK_d:
                        player2->move(0);
                        break;
                    case SDLK_w:
                        player2->move(3);
                        break;
                    case SDLK_s:
                        player2->move(2);
                        break;
                    default:
                        break;
                }
          }
      }
      // Clear Screen
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      // Render other objects
      SDL_RenderPresent(renderer);

        //Pattern viewport
        SDL_Rect patternViewport;
        patternViewport.x = 1000;
        patternViewport.y = 0;
        patternViewport.w = 300;
        patternViewport.h = 300;
        SDL_RenderSetViewport(renderer, &patternViewport);

        SDL_RenderCopy( renderer, pattern, NULL, NULL );

        //Game viewport
        SDL_Rect mainViewport;
        mainViewport.x = 0;
        mainViewport.y = 0;
        mainViewport.w = 1000;
        mainViewport.h = 500;
        SDL_RenderSetViewport(renderer, &mainViewport);

        // Render Objects
        player1->render();
        player2->render();

        // Render other objects
        SDL_RenderPresent(renderer);
        }
    }
  }

  close();
  return 0;
}
