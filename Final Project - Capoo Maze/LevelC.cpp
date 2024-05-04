#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 50
#define LEVEL_HEIGHT 25
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//28.0f, -5.0f, 0.0f
unsigned int LEVELC_DATA[] =
{
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 25, 19, 28, 29, 29, 12, 12, 12, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 21, 3, 24, 29, 29, 3, 3, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 11, 18, 18, 18, 18, 29, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 28, 9, 9, 9, 9, 4, 19, 2, 18, 18, 18, 18, 29, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 24, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 11, 25, 12, 12, 18, 29, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 11, 9, 9, 9, 9, 4, 18, 2, 21, 3, 3, 18, 29, 18, 18, 18, 18, 18, 18, 29, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 11, 13, 18, 18, 18, 29, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 11, 9, 9, 9, 9, 4, 18, 2, 4, 18, 18, 18, 29, 18, 18, 18, 18, 18, 18, 29, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 11, 13, 18, 18, 18, 18, 18, 18, 14, 10, 10, 10, 10, 17, 18, 18, 11, 9, 9, 9, 9, 4, 18, 2, 4, 18, 18, 18, 18, 18, 18, 5, 1, 1, 1, 1, 8, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 15, 16, 18, 18, 14, 17, 29, 18, 15, 28, 9, 9, 9, 13, 18, 18, 11, 9, 9, 9, 9, 4, 18, 6, 7, 18, 18, 5, 8, 29, 29, 6, 24, 9, 9, 9, 4, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 29, 18, 18, 15, 16, 18, 18, 18, 15, 12, 12, 12, 16, 18, 18, 11, 9, 9, 9, 9, 4, 18, 18, 29, 18, 18, 6, 7, 18, 18, 18, 6, 3, 3, 3, 7, 29, 29, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 11, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 11, 9, 9, 9, 9, 4, 18, 18, 2, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 29, 29, 11, 29, 29, 14, 17, 18, 18, 18, 14, 17, 18, 10, 10, 10, 10, 11, 9, 9, 9, 9, 4, 18, 18, 2, 18, 18, 5, 8, 18, 18, 18, 5, 8, 18, 1, 1, 1, 1, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 11, 18, 18, 15, 16, 29, 18, 14, 27, 13, 18, 18, 18, 18, 18, 11, 9, 9, 9, 9, 4, 18, 18, 2, 18, 18, 6, 7, 29, 18, 5, 23, 4, 18, 18, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 11, 18, 18, 18, 18, 29, 18, 15, 12, 16, 18, 14, 10, 17, 18, 11, 9, 9, 9, 9, 4, 18, 18, 2, 18, 18, 18, 18, 29, 18, 6, 3, 7, 18, 5, 1, 8, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 11, 18, 18, 18, 18, 29, 18, 29, 18, 18, 18, 11, 18, 13, 18, 11, 9, 9, 9, 9, 4, 18, 18, 2, 18, 18, 18, 18, 29, 18, 29, 18, 18, 18, 2, 18, 4, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 15, 12, 12, 12, 12, 28, 18, 10, 10, 10, 10, 27, 18, 13, 18, 11, 9, 9, 9, 9, 4, 29, 29, 6, 3, 3, 3, 3, 24, 18, 1, 1, 1, 1, 23, 18, 4, 29, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 18, 18, 18, 18, 18, 11, 18, 18, 18, 18, 18, 18, 18, 13, 18, 11, 9, 9, 9, 9, 4, 18, 18, 29, 18, 18, 18, 18, 2, 18, 18, 18, 29, 29, 18, 18, 4, 29, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 18, 18, 18, 18, 18, 15, 12, 12, 12, 12, 12, 12, 12, 16, 18, 11, 9, 9, 9, 9, 4, 18, 18, 29, 18, 18, 18, 18, 6, 3, 3, 3, 3, 3, 3, 3, 7, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 29, 29, 18, 18, 18, 18, 11, 9, 9, 9, 9, 4, 18, 18, 29, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 26, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 27, 9, 9, 9, 9, 22, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 23, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
};

LevelC::~LevelC()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelC::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 5, 6);

    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(36.0f, -11.0f, 0.0f));// 36.0f, -11.0f, 0.0f
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->m_speed = 2.5f;
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/ch003.png");

    // Walking
    m_state.player->m_walking[m_state.player->LEFT] = new int[4] { 8, 9, 10, 11 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 12, 13, 14, 15 };
    m_state.player->m_walking[m_state.player->UP] = new int[4] { 4, 5, 6, 7 };
    m_state.player->m_walking[m_state.player->DOWN] = new int[4] { 0, 1, 2, 3 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 4;
    m_state.player->m_animation_rows = 4;
    m_state.player->set_height(0.8f);
    m_state.player->set_width(0.8f);

    // Jumping
    m_state.player->m_jumping_power = 5.0f;

    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/capoo.png");

    set_enemy(ENEMY_COUNT);
    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(IDLE);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(21.0f, -6.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].m_speed = 0.8f;
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(GUARD);
    m_state.enemies[1].set_ai_state(IDLE);
    m_state.enemies[1].m_texture_id = enemy_texture_id;
    m_state.enemies[1].set_position(glm::vec3(30.0f, -5.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_state.enemies[1].m_speed = 1.0f;
    m_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    m_state.enemies[2].set_entity_type(ENEMY);
    m_state.enemies[2].set_ai_type(GUARD);
    m_state.enemies[2].set_ai_state(IDLE);
    m_state.enemies[2].m_texture_id = enemy_texture_id;
    m_state.enemies[2].set_position(glm::vec3(36.0f, -19.0f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_state.enemies[2].m_speed = 1.0f;
    m_state.enemies[2].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    m_state.enemies[3].set_entity_type(ENEMY);
    m_state.enemies[3].set_ai_type(GUARD);
    m_state.enemies[3].set_ai_state(IDLE);
    m_state.enemies[3].m_texture_id = enemy_texture_id;
    m_state.enemies[3].set_position(glm::vec3(7.0f, -17.0f, 0.0f));
    m_state.enemies[3].set_movement(glm::vec3(0.0f));
    m_state.enemies[3].m_speed = 1.0f;
    m_state.enemies[3].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/Platformer by Ethan Meng.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelC::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    m_state.enemies[0].update(delta_time, m_state.player, 0, 0, m_state.map);
    m_state.enemies[1].update(delta_time, m_state.player, 0, 0, m_state.map);
    m_state.enemies[2].update(delta_time, m_state.player, 0, 0, m_state.map);
    m_state.enemies[3].update(delta_time, m_state.player, 0, 0, m_state.map);
}

void LevelC::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    m_state.enemies[0].render(program);
    m_state.enemies[1].render(program);
    m_state.enemies[2].render(program);
    m_state.enemies[3].render(program);
}