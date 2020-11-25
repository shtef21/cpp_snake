#include <iostream>
#include <conio.h>
#include <Windows.h>

#include <string>
#include <vector>

class Coordinate
{
public:
    int x, y;
    Coordinate(int _x=0, int _y=0) : x(_x), y(_y) {}
    bool operator==(const Coordinate& b);
};

class App
{
private:
    App& app;
    const int w, h;
    std::string screen;
    // Textures:
    const char wall, background, snake_dead, food_icon;
    char snake_head, snake_body;
    bool ESC_pressed;

    enum direction
    {
        up, down, left, right
    } snake_direction;
    Coordinate food;
    std::vector<Coordinate> snake;

    App(int w=30, int h=15, char wall='X', char background=' ', char snake_head='@', char snake_body='+', char snake_dead='X', char food_icon='.');

    char& operator()(int idx_x, int idx_y);
    void update();
    void make_food();
    bool check_game_over();
    bool print();
    void input();

public:
    static void run();

};

int main(int argc, char *argv[])
{
    App::run();

    return 0;
}

// -------------------------------------------------- //

void App::run()
{
    App app;

    while(app.print())
    {
        app.input();
        // Sleep(500);
    }
}

bool App::print()
{
    this->update();
    
    std::string screen_copy = this->screen;
    // Draw food
    screen_copy[food.y * this->w + food.x] = this->food_icon;
    // Draw snake body
    for (auto it = snake.rbegin() + 1; it != snake.rend(); ++it)
        screen_copy[it->y * this->w + it->x] = this->snake_body;
    // Snake head
    Coordinate last = snake[snake.size() - 1];
    screen_copy[last.y * this->w + last.x] = this->snake_head;

    std::cout << screen_copy;
    return !this->check_game_over();
}

bool Coordinate::operator==(const Coordinate& b)
{
    return this->x == b.x && this->y == b.y;
}

App::App(int w, int h, char wall, char background, char snake_head, char snake_body, char snake_dead, char food_icon) : app(*this), w(w), h(h), wall(wall), background(background), snake_head(snake_head), snake_body(snake_body), snake_dead(snake_dead), food_icon(food_icon), screen(w * h, background), snake_direction(App::direction::up), snake({ { w/2, h/2+2 }, { w/2, h/2+1 }, { w/2, h/2 } }), ESC_pressed(false)
{
    // Make walls
    for (int i = 0; i < this->h; ++i)
        for (int j = 0; j < this->w; ++j)
            if (i == 0 || j == 0 || i == this->h - 1 || j == this->w - 1)
                this->app(j, i) = this->wall;

    srand(time(NULL));
}

char& App::operator()(int idx_x, int idx_y)
{
    return this->screen[idx_y * this->w + idx_x];
}

void App::update()
{
    Coordinate new_coordinate = this->snake[this->snake.size() - 1];
    if (this->snake_direction == App::direction::up)
        --new_coordinate.y;
    else if (this->snake_direction == App::direction::down)
        ++new_coordinate.y;
    else if (this->snake_direction == App::direction::left)
        --new_coordinate.x;
    else if (this->snake_direction == App::direction::right)
        ++new_coordinate.x;

    if (new_coordinate == this->food)
    {
        this->snake.push_back(new_coordinate);
        this->food = {};
    }
    else
    {
        for (int idx = 0; idx < this->snake.size() - 1; ++idx)
            this->snake[idx] = this->snake[idx + 1];
        this->snake[this->snake.size() - 1] = new_coordinate;
    }

    this->make_food();

    if (check_game_over())
    {
        this->snake_head = 'X';
        this->snake_body = 'X';
    }
}

bool App::check_game_over()
{
    Coordinate head = this->snake[this->snake.size() - 1];

    if (head.x == 0 || head.x == this->w - 1 || head.y == 0 || head.y == this->h - 1)
        return true;

    for (auto it = this->snake.rbegin() + 1; it != this->snake.rend(); ++it)
        if (*it == head || it->x == 0 || it->x == this->w - 1 || it->y == 0 || it->y == this->h - 1)
            return true;
            
    return this->ESC_pressed || false;
}

void App::make_food()
{
    if (this->food.x && this->food.x && this->food.y && this->food.y)
        return;

    bool coord_ok = true;
    Coordinate coord;
    do
    {
        coord.x = 1 + rand() % (this->w - 2);
        coord.y = 1 + rand() % (this->h - 2);

        for (auto it = snake.begin(); it != snake.end(); ++it)
        {
            if (*it == coord)
            {
                coord_ok = false;
                break;
            }
        }
    } while(coord_ok == false);

    this->food = coord;
}

void App::input()
{
    if (_kbhit() || true)
    {
        int c = getch();
        if (c == 27)
            this->ESC_pressed = true;
        else if (c == 'w' && this->snake_direction != App::direction::down)
            this->snake_direction = App::direction::up;
        else if (c == 'a' && this->snake_direction != App::direction::right)
            this->snake_direction = App::direction::left;
        else if (c == 's' && this->snake_direction != App::direction::up)
            this->snake_direction = App::direction::down;
        else if (c == 'd' && this->snake_direction != App::direction::left)
            this->snake_direction = App::direction::right;
    }
}

