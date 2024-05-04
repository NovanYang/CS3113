/**
* Author: [Novan]
* Assignment: [Capoo Maze]
* Date due: 2024-05-03, 3:00pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "Utility.h"
#include "Scene.h"
#include "Menu.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "Effects.h"

// 每每每每每 CONSTANTS 每每每每每 //
const int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;

const float BG_RED = 0.1922f,
BG_BLUE = 0.549f,
BG_GREEN = 0.9059f,
BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;


// 每每每每每 GLOBAL VARIABLES 每每每每每 //
Scene* g_current_scene;
Menu* g_menu;
LevelA* g_levelA;
LevelB* g_levelB;
LevelC* g_levelC;

Effects* g_effects;
Scene* g_levels[4];

SDL_Window* g_display_window;
bool g_game_is_running = true;
bool g_game_win = false;
bool g_game_lose = false;
bool g_game_end = false;
bool g_pause = false;
bool teleport = false;
bool teleport_ready = true;

int lives = 3;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool g_is_colliding_bottom = false;

std::string win = "You Win";
std::string lose = "You Lose";

// 每每每每每 GENERAL FUNCTIONS 每每每每每 //
void switch_to_scene(Scene* scene)
{
    g_current_scene = scene;
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Capoo Maze",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_menu = new Menu();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_levelC = new LevelC();

    g_levels[0] = g_menu;
    g_levels[1] = g_levelA;
    g_levels[2] = g_levelB;
    g_levels[3] = g_levelC;

    // Start at Menu
    switch_to_scene(g_levels[0]);

    g_effects = new Effects(g_projection_matrix, g_view_matrix);
    g_effects->start(FADEIN, 0.4f);
}

void process_input()
{
    if (g_current_scene != g_menu) {
        // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
        g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
                // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_q:
                    // Quit the game with a keystroke
                    g_game_is_running = false;
                    break;

                default:
                    break;
                }
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_t) {
                    teleport_ready = true; // Reset teleport on key release
                }
                break;
            default:
                break;
            }
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    if (!g_pause) {
        if (key_state[SDL_SCANCODE_A] || key_state[SDL_SCANCODE_LEFT])
        {
            g_current_scene->m_state.player->m_movement.x = -1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
        }
        else if (key_state[SDL_SCANCODE_D] || key_state[SDL_SCANCODE_RIGHT])
        {
            g_current_scene->m_state.player->m_movement.x = 1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
        }
        else if (key_state[SDL_SCANCODE_W] || key_state[SDL_SCANCODE_UP])
        {
            g_current_scene->m_state.player->m_movement.y = 1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->UP];
        }
        else if (key_state[SDL_SCANCODE_S] || key_state[SDL_SCANCODE_DOWN])
        {
            g_current_scene->m_state.player->m_movement.y = -1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->DOWN];
        }
        else if (key_state[SDL_SCANCODE_T] & teleport_ready) {
            Mix_PlayChannel(-1, g_current_scene->m_state.jump_sfx, 0);
            glm::vec3 current_pos = g_current_scene->m_state.player->get_position();
            float penetration_x = 0;
            float penetration_y = 0;

            if (g_current_scene == g_levelA) {
                if (teleport) {
                    if (!(g_current_scene->m_state.map->is_solid((glm::vec3(current_pos.x + 14.0f, current_pos.y, current_pos.z)), &penetration_x, &penetration_y))) {
                        g_current_scene->m_state.player->set_position(glm::vec3(current_pos.x + 14.0f, current_pos.y, current_pos.z));
                        teleport = !teleport;
                    }
                }
                else {
                    if (!(g_current_scene->m_state.map->is_solid((glm::vec3(current_pos.x - 14.0f, current_pos.y, current_pos.z)), &penetration_x, &penetration_y))) {
                        g_current_scene->m_state.player->set_position(glm::vec3(current_pos.x - 14.0f, current_pos.y, current_pos.z));
                        teleport = !teleport;
                    }
                }
            }
            if (g_current_scene == g_levelB) {
                if (teleport) {
                    if (!(g_current_scene->m_state.map->is_solid((glm::vec3(current_pos.x + 18.0f, current_pos.y, current_pos.z)), &penetration_x, &penetration_y))) {
                        g_current_scene->m_state.player->set_position(glm::vec3(current_pos.x + 18.0f, current_pos.y, current_pos.z));
                        teleport = !teleport;
                    }
                }
                else {
                    if (!(g_current_scene->m_state.map->is_solid((glm::vec3(current_pos.x - 18.0f, current_pos.y, current_pos.z)), &penetration_x, &penetration_y))) {
                        g_current_scene->m_state.player->set_position(glm::vec3(current_pos.x - 18.0f, current_pos.y, current_pos.z));
                        teleport = !teleport;
                    }
                }
            }
            if (g_current_scene == g_levelC) {
                if (teleport) {
                    if (!(g_current_scene->m_state.map->is_solid((glm::vec3(current_pos.x + 23.0f, current_pos.y, current_pos.z)), &penetration_x, &penetration_y))) {
                        g_current_scene->m_state.player->set_position(glm::vec3(current_pos.x + 23.0f, current_pos.y, current_pos.z));
                        teleport = !teleport;
                    }
                }
                else {
                    if (!(g_current_scene->m_state.map->is_solid((glm::vec3(current_pos.x - 23.0f, current_pos.y, current_pos.z)), &penetration_x, &penetration_y))) {
                        g_current_scene->m_state.player->set_position(glm::vec3(current_pos.x - 23.0f, current_pos.y, current_pos.z));
                        teleport = !teleport;
                    }
                }
            }
            teleport_ready = false;
        }

    }


    if (glm::length(g_current_scene->m_state.player->m_movement) > 1.0f)
    {
        g_current_scene->m_state.player->m_movement = glm::normalize(g_current_scene->m_state.player->m_movement);
    }
}

void update()
{
    if (!g_pause) {
        if (!g_game_end) {
            float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
            float delta_time = ticks - g_previous_ticks;
            g_previous_ticks = ticks;

            delta_time += g_accumulator;

            if (delta_time < FIXED_TIMESTEP)
            {
                g_accumulator = delta_time;
                return;
            }

            while (delta_time >= FIXED_TIMESTEP) {
                g_current_scene->update(FIXED_TIMESTEP);
                g_effects->update(FIXED_TIMESTEP);

                if (g_current_scene != g_menu) {
                    for (int i = 0; i < g_current_scene->m_number_of_enemies; i++) {
                        if (g_current_scene->m_state.player->check_collision(&g_current_scene->m_state.enemies[i])) {
                            if (lives > 1) {
                                if (teleport) {
                                    teleport = !teleport;
                                }
                                switch_to_scene(g_current_scene);
                                lives -= 1;
                            }
                            else {
                                g_game_end = true;
                                g_game_lose = true;
                                continue;
                            }
                        }
                    }
                }


                g_is_colliding_bottom = g_current_scene->m_state.player->m_collided_bottom;

                delta_time -= FIXED_TIMESTEP;
            }

            g_accumulator = delta_time;

            // Prevent the camera from showing anything outside of the "edge" of the level
            g_view_matrix = glm::mat4(1.0f);
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, -g_current_scene->m_state.player->get_position().y, 0));


            if (g_current_scene == g_levelA) {
                if (g_current_scene->m_state.player->get_position().x >= 22.8f && g_current_scene->m_state.player->get_position().y >= -3.2f) {
                    switch_to_scene(g_levelB);
                }
            }

            if (g_current_scene == g_levelB) {
                //(31.0f, -11.0f, 0.0f) checkpoint
                if (g_current_scene->m_state.player->get_position().x >= 30.2f && g_current_scene->m_state.player->get_position().y >= -11.5f) {
                    if (g_current_scene->m_state.player->get_position().x <= 31.5f && g_current_scene->m_state.player->get_position().y <= -10.8f) {
                        switch_to_scene(g_levelC);
                    }
                }
            }
            if (g_current_scene == g_levelC) {
                //(28.0f, -5.0f, 0.0f) checkpoint
                if (g_current_scene->m_state.player->get_position().x >= 27.2f && g_current_scene->m_state.player->get_position().y >= -5.5f) {
                    if (g_current_scene->m_state.player->get_position().x <= 28.5f && g_current_scene->m_state.player->get_position().y <= -4.8f) {
                        g_game_end = true;
                        g_game_win = true;
                    }
                }
            }
            g_view_matrix = glm::translate(g_view_matrix, g_effects->m_view_offset);
        }
    }

}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);

    float player_x_pos = g_current_scene->m_state.player->get_position().x;
    float player_y_pos = g_current_scene->m_state.player->get_position().y;
    float player_z_pos = g_current_scene->m_state.player->get_position().z;
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");

    glUseProgram(g_shader_program.get_program_id());
    g_current_scene->render(&g_shader_program);
    g_effects->render();
    if (g_current_scene != g_menu) {
        if (!g_game_end) {
            Utility::draw_text(&g_shader_program, font_texture_id, "Lives Remaining: " + std::to_string(lives), 0.3, -0.1, glm::vec3(player_x_pos - 2.0f, player_y_pos - 3.0f, player_z_pos));
            Utility::draw_text(&g_shader_program, font_texture_id, "PRESS [T] to transmit", 0.3, -0.1, glm::vec3(player_x_pos - 2.2f, player_y_pos - 3.3f, player_z_pos));
            Utility::draw_text(&g_shader_program, font_texture_id, "FIND ENTRANCE TO NEXT LEVEL", 0.3, -0.1, glm::vec3(player_x_pos - 2.4f, player_y_pos - 3.6f, player_z_pos));
        }
        else {
            if (g_game_win) {
                Utility::draw_text(&g_shader_program, font_texture_id, win, 0.3, -0.1, glm::vec3(player_x_pos - 1.0f, player_y_pos, player_z_pos));
            }
            else if (g_game_lose) {
                Utility::draw_text(&g_shader_program, font_texture_id, lose, 0.3, -0.1, glm::vec3(player_x_pos - 1.0f, player_y_pos, player_z_pos));
            }
        }
    }
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();

    delete g_levelA;
    delete g_levelB;
    delete g_levelC;
    delete g_effects;
}

// 每每每每每 DRIVER GAME LOOP 每每每每每 //
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();

        if (g_current_scene->m_state.next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);

        render();
    }

    shutdown();
    return 0;
}