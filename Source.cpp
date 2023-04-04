#include <iostream>
#include <tuple>
#include <vector>
#include <random>
#include <map>
#include <unordered_map>

#include <SDL.h>

using namespace std;

int random(int begin, int end)
{
    random_device rnd;
    uniform_int_distribution<> dist(begin, end);
    return dist(rnd);
}


struct vec2d
{
    int x;
    int y;
};

class Entity
{
public:
    Entity(vec2d pos, uint16_t hp, uint8_t speed, vec2d size) : position_(pos), hp_(hp), speed_(speed), size_{ size } {};
    void Render(SDL_Renderer* renderer)
    {
        SDL_Rect view = { position_.x, position_.y, size_.x, size_.y };
        SDL_RenderFillRect(renderer, &view);
    };
    virtual std::pair<int, int> GetPos() { return std::make_pair(position_.x, position_.y); }

protected:
    vec2d position_;
    uint16_t hp_;
    uint8_t speed_;
    vec2d size_;
};

class Player : public Entity
{
public:
    using Entity::Entity;
    void Movement(const Uint8* state)
    {
        if (state[SDL_SCANCODE_UP] and !state[SDL_SCANCODE_DOWN])
            position_.y -= speed_;
        if (!state[SDL_SCANCODE_UP] and state[SDL_SCANCODE_DOWN])
            position_.y += speed_;
        if (state[SDL_SCANCODE_RIGHT] and !state[SDL_SCANCODE_LEFT])
            position_.x += speed_;
        if (!state[SDL_SCANCODE_RIGHT] and state[SDL_SCANCODE_LEFT])
            position_.x -= speed_;
    }
    void lvlup() {}
private:
};


class Turrel : public Entity
{
public:
    using Entity::Entity;
    void Movement(const Uint8* state) {}
    void lvlup() {}
private:
};


class Enemy : public Entity
{
public:
    using Entity::Entity;
    void Movement()
    {
        position_.y += speed_;
    }
    void lvlup() {};
private:
};

class Bullet : public Entity
{
public:
    using Entity::Entity;
    void Movement()
    {
        position_.y -= speed_;
    }

private:
};



int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    int WIDTH = 1200, HEIGHT = 800;


    SDL_Window* window = SDL_CreateWindow("SDL GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event ev;

    const uint8_t FPS = 60;
    const uint8_t PLAYER_SIZE = 15;
    const uint8_t TURREL_SIZE = 10;
    const uint8_t ENEMY_RAND_SIZE = random(5, 20);
    const Uint8* state = SDL_GetKeyboardState(NULL);
    vec2d mouse_coord;


    vector<Turrel> turrels = {};
    vector<Enemy> enemies = {};
    vector<Bullet> bullets = {};

    Player player({ WIDTH / 2, HEIGHT - 50 }, 100, 1, { PLAYER_SIZE, PLAYER_SIZE });
    Enemy enemy_temp({ WIDTH / 2, 50 }, 100, 1, {PLAYER_SIZE, PLAYER_SIZE});
    Bullet bullet({ player.GetPos().first, player.GetPos().second }, 1, 5, {4, 9});

    enemies.push_back(enemy_temp);
    bullets.push_back(bullet);



    bool isRunning = true;
    while (isRunning)
    {
        #pragma region EVENTS
        bool created_bullet = false;
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (ev.button.button == SDL_BUTTON_LEFT)
                {
                    //turrel_count++;
                    mouse_coord.x = ev.button.x;
                    mouse_coord.y = ev.button.y;
                    Turrel tur(mouse_coord, 25, 0, {TURREL_SIZE, TURREL_SIZE });
                    turrels.push_back(tur);
                }
            case SDL_KEYDOWN:
                switch (ev.key.keysym.scancode)
                {
                case SDL_SCANCODE_ESCAPE:
                    isRunning = false;
                    break;
                case SDL_SCANCODE_D:
                    created_bullet = true;
                    break;
                }
            }
        }

        

        player.Movement(state);

        

        #pragma endregion


        #pragma region DRAWING

        SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        player.Render(renderer);

        
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        for (Turrel turrel : turrels)
            turrel.Render(renderer);


        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        for (Enemy enemy : enemies)
        {
            enemy.Movement();
            enemy.Render(renderer);
        }
        
        
        if (created_bullet)
        {
            Bullet bullet({ player.GetPos().first, player.GetPos().second }, 1, 5, {4, 9});
            bullets.push_back(bullet);
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for(Bullet bullet : bullets)
        {
            bullet.Movement();
            bullet.Render(renderer);
        }
        



        SDL_RenderPresent(renderer);
        #pragma endregion

        SDL_Delay(1000 / FPS);
    }



    SDL_Quit();

    return 0;
}