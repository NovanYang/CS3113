/**
* Author: [Novan Yang]
* Assignment: Pong Clone
* Date due: 2024-03-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#define GL_GLEXT_PROTOTYPES 1
#define LOG(argument) std::cout << argument << '\n'

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"
#include "ctime"
#include "cmath"

const int WINDOW_WIDTH = 640 * 2,
		  WINDOW_HEIGHT = 480 * 2;

const float BG_RED = 0.1922f,
			BG_BLUE = 0.549f,
			BG_GREEN = 0.9059f,
			BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
		  VIEWPORT_Y = 0,
		  VIEWPORT_WIDTH = WINDOW_WIDTH,
		  VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const float MILLISECONDS_IN_SECOND = 1000.0;

const int NUMBER_OF_TEXTURES = 1;
const GLint LEVEL_OF_DETAIL = 0; 
const GLint TEXTURE_BORDER = 0;

const char  V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl",
PLAYER_KOBE_FILEPATH[] = "assets/kobe.png",
PLAYER_SHAQ_FILEPATH[] = "assets/shaq.png",
BALL_FILEPATH[] = "assets/ball.png",
KOBE__WIN_FILEPATH[] = "assets/kobe_win.png",
SHAQ_WIN_FILEPATH[] = "assets/shaq_win.png";


const float kobe_speed = 3.0f,
            shaq_speed = 3.0f,
            ball_speed = 1.0f;
            
const float left_max = -5.0f,
            right_max = 5.0f,
            up_max = 3.75f,
            down_max = -3.75f;


SDL_Window* g_display_window;
bool g_game_is_running = true;
bool g_is_growing = true;
bool single_player = false;
bool move_up = false;
bool ball_up = true;
bool ball_left = true;
bool game_end = false;
bool kobe_win = false;
bool shaq_win = false;
float g_previous_ticks = 0.0f;
int ball_num = 1;

ShaderProgram g_shader_program;
glm::mat4 view_matrix, g_kobe_model_matrix, g_shaq_model_matrix, g_ball_model_matrix, g_projection_matrix, g_trans_matrix, kobe_win_model_matrix, shaq_win_model_matrix;

GLuint g_kobe_texture_id;
GLuint g_shaq_texture_id;
GLuint g_ball_texture_id;
GLuint kobe_win_texture_id;
GLuint shaq_win_texture_id;
SDL_Joystick* g_kobe_controller;
SDL_Joystick* g_shaq_controller;

glm::vec3 g_kobe_position = glm::vec3(-3.5, 0.0f, 0.0f);
glm::vec3 g_kobe_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_shaq_position = glm::vec3(4.0, 0.0f, 0.0f);
glm::vec3 g_shaq_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_ball_position = glm::vec3(0.0, 0.0f, 0.0f);
glm::vec3 g_ball_movement = glm::vec3(0.0f, 0.0f, 0.0f);



GLuint load_texture(const char* filepath) {
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
        LOG(filepath);
        assert(false);
    }

    // STEP 2: Generating and binding a texture ID to our image
    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // STEP 3: Setting our texture filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // STEP 4: Releasing our file from memory and returning our texture id
    stbi_image_free(image);

    return textureID;

}

void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id) {
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool check_collision(glm::vec3& position_a, glm::vec3& position_b) {
    float x_distance = fabs(position_a.x - position_b.x) - (0.3f);
    float y_distance = fabs(position_a.y - position_b.y) - ((2.0f + 0.5f)/2.0f);

    return x_distance < 0 && y_distance < 0;
}



void initialise() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    g_kobe_controller = SDL_JoystickOpen(0);

    g_display_window = SDL_CreateWindow("Pong with the Kobe and Shaq",
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

    g_kobe_model_matrix = glm::mat4(1.0f);
    g_shaq_model_matrix = glm::mat4(1.0f);
    g_ball_model_matrix = glm::mat4(1.0f);
    kobe_win_model_matrix = glm::mat4(1.0f);
    shaq_win_model_matrix = glm::mat4(1.0f);
    view_matrix = glm::mat4(1.0f);  
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f); 

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    g_kobe_texture_id = load_texture(PLAYER_KOBE_FILEPATH);
    g_shaq_texture_id = load_texture(PLAYER_SHAQ_FILEPATH);
    g_ball_texture_id = load_texture(BALL_FILEPATH);
    kobe_win_texture_id = load_texture(KOBE__WIN_FILEPATH);
    shaq_win_texture_id = load_texture(SHAQ_WIN_FILEPATH);
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input() {
    g_kobe_movement = glm::vec3(0.0f);
    g_shaq_movement = glm::vec3(0.0f);
    g_ball_movement = glm::vec3(0.0f);

    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT_CLOSE:
            g_game_is_running = false;
            break;

        case SDL_QUIT:
            g_game_is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    g_game_is_running = false;
                    break;
                
                case SDLK_w:
                    g_kobe_movement.y = 1.0f;
                    break;

                case SDLK_s:
                    g_kobe_movement.y = -1.0f;
                    break;

                case SDLK_UP:
                    g_shaq_movement.y = 1.0f;
                    break;

                case SDLK_DOWN:
                    g_shaq_movement.y = -1.0f;
                    break;

                case SDLK_t:
                    single_player = !single_player;
                    break;

                case SDLK_1:
                    ball_num = 1;
                    break;

                case SDLK_2:
                    ball_num = 2;
                    break;

                case SDLK_3:
                    ball_num = 3;
                    break;

                default:
                    break;
            }
        default:
            break;
        }
    }

    const Uint8* key_state = SDL_GetKeyboardState(NULL);

    if (key_state[SDL_SCANCODE_W]) {
        g_kobe_movement.y = 1.0f;
    }
    else if (key_state[SDL_SCANCODE_S]) {
        g_kobe_movement.y = -1.0f;
    }

    if (key_state[SDL_SCANCODE_UP]) {
        g_shaq_movement.y = 1.0f;
    }
    else if (key_state[SDL_SCANCODE_DOWN]) {
        g_shaq_movement.y = -1.0f;
    }

    if (glm::length(g_kobe_movement) > 1.0f) {
        g_kobe_movement = glm::normalize(g_kobe_movement);
    }

    if (glm::length(g_kobe_movement) > 1.0f) {
        g_shaq_movement = glm::normalize(g_shaq_movement);
    }
}

void update() {
    if (!game_end) {
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
        float delta_time = ticks - g_previous_ticks;
        g_previous_ticks = ticks;

        if (single_player) {
            g_kobe_position += g_kobe_movement * kobe_speed * delta_time * 1.0f;
            if (g_shaq_position.y > up_max - 0.1f) {
                move_up = false;
            }
            if (g_shaq_position.y < down_max + 0.1f) {
                move_up = true;
            }
            if (move_up) {
                g_shaq_position.y += 1.0 * shaq_speed * delta_time;
            }
            else {
                g_shaq_position.y += -1.0f * shaq_speed * delta_time;
            }
        }
        else {
            g_kobe_position += g_kobe_movement * kobe_speed * delta_time * 1.0f;
            g_shaq_position += g_shaq_movement * shaq_speed * delta_time * 1.0f;
        }

        if (g_kobe_position.y > up_max) {
            g_kobe_position.y = up_max;
        }
        else if (g_kobe_position.y < down_max) {
            g_kobe_position.y = down_max;
        }

        if (g_shaq_position.y > up_max) {
            g_shaq_position.y = up_max;
        }
        else if (g_shaq_position.y < down_max) {
            g_shaq_position.y = down_max;
        }

        if (ball_left) {
            g_ball_position.x += ball_speed * delta_time * 1.5f;
        }
        else {
            g_ball_position.x += ball_speed * delta_time * -1.5f;
        }
        if (check_collision(g_shaq_position, g_ball_position)) {
            ball_left = false;
        }
        if (check_collision(g_kobe_position, g_ball_position)) {
            ball_left = true;
        }

        if (ball_up) {
            g_ball_position.y += ball_speed * delta_time * 2.5f;
        }
        else {
            g_ball_position.y += ball_speed * delta_time * -2.5f;
        }

        if (g_ball_position.y >= up_max) {
            ball_up = false;
        }
        if (g_ball_position.y <= down_max) {
            ball_up = true;
        }
        if (g_ball_position.x <= left_max) {
            shaq_win = true;
            game_end = true;
        }
        if (g_ball_position.x >= right_max) {
            kobe_win = true;
            game_end = true;
        }


        g_kobe_model_matrix = glm::mat4(1.0f);
        g_kobe_model_matrix = glm::translate(g_kobe_model_matrix, g_kobe_position);
        g_shaq_model_matrix = glm::mat4(1.0f);
        g_shaq_model_matrix = glm::translate(g_shaq_model_matrix, g_shaq_position);
        g_ball_model_matrix = glm::mat4(1.0f);
        g_ball_model_matrix = glm::translate(g_ball_model_matrix, g_ball_position);
    }
    else {
        
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    };

    float ball_vertices[] = {
        -0.5f, -0.5f, -0.1f, -0.5f, -0.1f, -0.1f,
        -0.5f, -0.5f, -0.1f, -0.1f, -0.5f, -0.1f
    };

    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    draw_object(g_kobe_model_matrix, g_kobe_texture_id);
    draw_object(g_shaq_model_matrix, g_shaq_texture_id);
    draw_object(g_ball_model_matrix, g_ball_texture_id);
    if (kobe_win) {
        draw_object(kobe_win_model_matrix, kobe_win_texture_id);
    }
    else if (shaq_win) {
        draw_object(shaq_win_model_matrix, shaq_win_texture_id);
    }

    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() {
    SDL_JoystickClose(g_kobe_controller);
    SDL_JoystickClose(g_shaq_controller);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    initialise();

    while (g_game_is_running) {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}