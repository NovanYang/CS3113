/**
* Author: [Novan Yang]
* Assignment: Simple 2D Scene
* Date due: 2024-02-17, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "stb_image.h"

#define LOG(argument) std::cout << argument << '\n'

const int WINDOW_WIDTH = 640 * 2,
WINDOW_HEIGHT = 480 * 2;

const float BG_RED = 1.0f,
BG_BLUE = 1.0f,
BG_GREEN = 1.0f,
BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const int MAX_FRAME = 1400;
const float GROWTH_FACTOR = 1.01f;
const float SHRINK_FACTOR = 0.99f;

const float MILLISECONDS_IN_SECOND = 1000.0;
const float DEGREES_PER_SECOND = 90.0f;

const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0;   // this value MUST be zero

const char EARTH_PATH[] = "assets/earth.png",
MOON_PATH[] = "assets/moon.png";

SDL_Window* g_display_window;
bool g_game_is_running = true;
bool g_is_growing = true;

ShaderProgram g_shader_program;
glm::mat4 view_matrix, g_earth_matrix, g_moon_matrix, g_projection_matrix, g_trans_matrix;

float g_previous_ticks = 0.0f;
float g_angle = 0.0f;
float g_earth_rotate = 0.0f;
float g_moon_x = 0.0f;
float g_moon_y = 0.0f;
float g_moon_elapse = 0.0f;
int g_frame_counter = 0;
float g_scale_factor = 0.0f;
float g_time_counter = 0.0f;
bool g_day = true;

GLuint g_earth_texture_id;
GLuint g_moon_texture_id;


GLuint load_texture(const char* filepath)
{
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

void initialise()
{

    g_display_window = SDL_CreateWindow("Simple 2D Scene",
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

    g_earth_matrix = glm::mat4(1.0f);
    g_moon_matrix = glm::mat4(1.0f);
    view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    g_earth_texture_id = load_texture(EARTH_PATH);
    g_moon_texture_id = load_texture(MOON_PATH);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            g_game_is_running = false;
        }
    }
}

void update()
{

    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND; // get the current number of ticks
    float delta_time = ticks - g_previous_ticks; // the delta time is the difference from the last frame
    g_previous_ticks = ticks;

    float radius = 2.0f;
    float speed = 3.14f / 2;

    g_angle += speed * delta_time;

    float earthX = cos(g_angle) * radius;
    float earthY = sin(g_angle) * radius;

    g_moon_elapse += 1.0f * delta_time;
    float moon_amplitude = 1.0f;
    float moon_speed = 2.0f;

    g_moon_x = cos(g_moon_elapse * moon_speed) * moon_amplitude;
    g_moon_y = sin(g_moon_elapse * moon_speed) * moon_amplitude;
    
    g_time_counter += delta_time;

    if(g_time_counter >= 2.0f){
        g_day = !g_day;
        g_time_counter = 0.0f;
    }

    if (g_day) {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    g_scale_factor = sin(g_moon_elapse) * 0.4f;

    g_earth_matrix = glm::mat4(1.0f);
    g_moon_matrix = glm::mat4(1.0f);
    g_earth_rotate +=2* DEGREES_PER_SECOND * delta_time;

    g_earth_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(earthX, earthY, 0.0f));
    g_earth_matrix = glm::rotate(g_earth_matrix, glm::radians(g_earth_rotate), glm::vec3(0.0f, 0.0f, 1.0f));

    g_moon_matrix = glm::translate(g_earth_matrix, glm::vec3(g_moon_x, g_moon_y, 0.0f));
    g_moon_matrix = glm::scale(g_moon_matrix, glm::vec3(g_scale_factor, g_scale_factor, 1.0f));

}

void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); // we are now drawing 2 triangles, so we use 6 instead of 3
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    };

    // Textures
    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    draw_object(g_earth_matrix, g_earth_texture_id);
    draw_object(g_moon_matrix, g_moon_texture_id);

    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}

void shutdown()
{
    SDL_Quit();
}

/**
 Start here¡ªwe can see the general structure of a game loop without worrying too much about the details yet.
 */
int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
