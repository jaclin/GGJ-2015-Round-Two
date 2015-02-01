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

class Tile
{
    public:
        // Unique ID
        const int uid;

        //colors
        int red = 255;
        int blue = 255;
        int green = 255;

        int tPosX, tPosY;

        Tile(int,int);
        void render();
        ~Tile();

    private:
        static int newUID;
};

class TileMap
{
    public:
        std::vector<Tile> TileHolder;
        TileMap();
        void render();
        ~TileMap();
};


class Player
{
    public:
        // Unique ID
        const int uid;

        Player(int, int, int,int );
        void render();
        void move(int);
        void changeTileColor(TileMap& );
        ~Player();

        int tileLocation;
        int origTileLocation;

    private:
        // uid hack
        static int newUID;

        //movement
        int velX = 25;
        int velY = 25;

        //colors
        int red, blue;

        // dimension
        double pRadius = 10;

        //position
        double pPosX, pPosY;
};

int Player::newUID = 0;

Player::Player(int x, int y, int r = 0, int b = 0): uid(newUID++)
{
    // Starting position
    pPosX = x;
    pPosY = y;
    red = r;
    blue = b;
    if (uid == 0){
        origTileLocation = 0;
    }
    else{
        origTileLocation = 780;
    }
    tileLocation = origTileLocation;
}

void Player::move(int direction)
{
    int prevTileLocation = tileLocation;
    // Left
    if (direction == 0)
    {
        pPosX += velX;
        tileLocation+=20;
    }
    // Right
    else if (direction == 1)
    {
        pPosX -= velX;
        tileLocation-=20;
    }
    // Down
    else if (direction == 2)
    {
        pPosY += velY;
        tileLocation+=1;
    }
    // Up
    else if (direction == 3)
    {
        pPosY -= velY;
        tileLocation-=1;
    }
    // Left boundary
    if (pPosX <= 0+pRadius)
    {
        this->pPosX = pRadius+2;
        tileLocation = prevTileLocation;
    }
    // Right boundary
    else if  (pPosX >= 500-pRadius)
    {
        this->pPosX = 500-pRadius-3;
        tileLocation = prevTileLocation;
    }

    if (pPosY <= 0+pRadius)
   	{
		this->pPosY = pRadius+2;
		tileLocation = prevTileLocation;
    }
    else if (pPosY >= SCREEN_HEIGHT-pRadius)
    {
        this->pPosY = SCREEN_HEIGHT-pRadius-3;
        tileLocation = prevTileLocation;
    }
    printf("%u\n",tileLocation);

}


void Player::render()
{
    filledCircleRGBA(renderer, pPosX, pPosY, pRadius, red, 0, blue, 255);
}

void Player::changeTileColor(TileMap& tilemap){
    for (auto &x : tilemap.TileHolder){
        if (x.uid == tileLocation){
            x.red = 0;
        }
    }
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
     boxRGBA(renderer, tPosX, tPosY, tPosX+25, tPosY+25, red, green, blue,255);
}

Tile::~Tile() {}

TileMap::TileMap(){
    for (int i = 0; i < 20; i++){
        for (int j = 0; j < 20; j++){
            TileHolder.push_back(Tile(i*25, j*25));
        }
     }
}

void TileMap::render(){
    for (auto &x : TileHolder){
        x.render();
    }
}


TileMap::~TileMap() {}

void drawGrid(){
    for(int i = 0; i < 20; i++){
        lineRGBA(renderer,0+(i+1)*25,0,0+(i+1)*25,500,0,0,0,255);
    }
    for(int i = 0; i < 20; i++){
        lineRGBA(renderer,0,0+(i+1)*25,500,0+(i+1)*25,0,0,0,255);
    }
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
  pattern = loadTexture( "star.png" );
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
  pattern = loadTexture( "mushroom.png" );
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
    Player * player2 = new Player(500-13,12,0,255);
    TileMap map1;
    TileMap map2;

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
                    switch( e.type )
                    {
                        case SDL_QUIT:
                            quit = true;
                        /* Look for a keypress */
                        case SDL_KEYDOWN:
                            /* Check the SDLKey values and move change the coords */
                            switch( e.key.keysym.sym )
                            {
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
                                case SDLK_TAB:
                                    player2->changeTileColor(map2);
                                case SDLK_SPACE:
                                    player1->changeTileColor(map1);
                                default:
                                    break;
                            }
                    }
                }
                // Clear Screen
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);


                //Pattern viewport
                SDL_Rect patternViewport;
                patternViewport.x = 1050;
                patternViewport.y = 150;
                patternViewport.w = 200;
                patternViewport.h = 200;
                SDL_RenderSetViewport(renderer, &patternViewport);

                SDL_RenderCopy( renderer, pattern, NULL, NULL );

                //Player 1 viewport
                SDL_Rect player1Viewport;
                player1Viewport.x = 0;
                player1Viewport.y = 0;
                player1Viewport.w = 500;
                player1Viewport.h = 500;
                SDL_RenderSetViewport(renderer, &player1Viewport);

                // Render objects
                map1.render();
                drawGrid();
                player1->render();


                //Player 2 viewport
                SDL_Rect player2Viewport;
                player2Viewport.x = 500;
                player2Viewport.y = 0;
                player2Viewport.w = 500;
                player2Viewport.h = 500;
                SDL_RenderSetViewport(renderer, &player2Viewport);

                map2.render();
                drawGrid();
                lineRGBA(renderer,0,0,0,500,255,255,0,255);
                player2->render();
                //printf("%u\n", map2.TileHolder.back().uid);

                // Update screen
                SDL_RenderPresent(renderer);
            }
        }
    }

    close();
    return 0;
}
