#include <SDL2/SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define CELL_SIZE 8
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRID_WIDTH (SCREEN_WIDTH / CELL_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / CELL_SIZE)

int grid[GRID_WIDTH][GRID_HEIGHT];
int next[GRID_WIDTH][GRID_HEIGHT];


void fill_cell(SDL_Renderer* renderer, int x, int y){
    SDL_Rect rect = {
        .x = x * CELL_SIZE,
        .y = y * CELL_SIZE,
        .w = CELL_SIZE,
        .h = CELL_SIZE
    };
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_grid_board(SDL_Renderer* renderer){
    SDL_Rect cell;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;


    for(int x = 0; x < GRID_WIDTH; ++x){
        
        for(int y = 0; y < GRID_HEIGHT; ++y){
            cell.x = x * CELL_SIZE;
            cell.y = y * CELL_SIZE;
            
            if(grid[x][y] == 1)
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
            
            SDL_RenderFillRect(renderer, &cell);
        }
    }

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);

    for(int i = 0; i < GRID_WIDTH; ++i){
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
    }

    for(int j = 0; j < GRID_HEIGHT; ++j){
        SDL_RenderDrawLine(renderer, 0, j * CELL_SIZE, SCREEN_WIDTH, j * CELL_SIZE);
    }
}

int count_neighbors(int board[GRID_WIDTH][GRID_HEIGHT], int x, int y){
    int count = 0;
    for(int i = -1; i < 2; ++i){
        for(int j = -1; j < 2; ++j){
            if(i == 0 && j == 0)
                continue;
            int r = x + i;
            int c = y + j;

            if(r >= 0 && r < GRID_WIDTH && c >= 0 && c < GRID_HEIGHT){
                count += board[r][c];
            }
        }
    }
    return count;
}

void initialize_game(){
    srand(time(NULL));

    for(int x = 0; x < GRID_WIDTH; ++x){
        for(int y = 0; y < GRID_HEIGHT; ++y){
            grid[x][y] = (rand() % 100 < 20) ? 1 : 0; 
        }
    }
}


void update_board(){
    for(int x = 0; x < GRID_WIDTH; ++x){
        for(int y = 0; y < GRID_HEIGHT; ++y){
            int neighbors = count_neighbors(grid, x, y);

            if(grid[x][y] == 1){
                if(neighbors == 2 || neighbors == 3)
                    next[x][y] = 1;
                else
                    next[x][y] = 0;
            }
            else{
                if(neighbors == 3)
                    next[x][y] = 1;
                else
                    next[x][y] = 0;
            }
        }
    }

    for(int x = 0; x < GRID_WIDTH; ++x){
        for(int y = 0; y < GRID_HEIGHT; ++y){
            grid[x][y] = next[x][y];
        }
    }
}



int main(){

    int running = 1;
    int paused = 0;
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);

    // Creating window
    SDL_Window* window = SDL_CreateWindow(
        "Tester", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_RESIZABLE
    );

    if(!window){
        printf("SDL_CreateWindow Error\n");
        exit(EXIT_FAILURE);
    }

    // Creating renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!renderer){
        printf("Renderer error\n");
        SDL_DestroyWindow(window);
        exit(EXIT_FAILURE);
    }

    SDL_Event event;

    initialize_game();

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = 0;
            }
            else if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    running = 0;
                }
                else if(event.key.keysym.sym == SDLK_r){
                    initialize_game();
                }
                else if(event.key.keysym.sym == SDLK_SPACE)
                    paused = !paused;
            }
        }
        if(!paused){
            update_board();
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        draw_grid_board(renderer);
        
        SDL_RenderPresent(renderer);
        
        SDL_Delay(100);
        
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}