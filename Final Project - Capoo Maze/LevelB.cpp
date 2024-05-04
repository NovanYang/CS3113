#include "LevelB.h"
#include "Utility.h"
//(31.0f, -11.0f, 0.0f) checkpoint
#define LEVEL_WIDTH 40
#define LEVEL_HEIGHT 20
//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
unsigned int LEVELB_DATA[] =
{
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 25, 12, 28, 25, 12, 12, 12, 12, 12, 28, 9, 9, 9, 9, 9, 9, 9, 9, 21, 3, 24, 21, 3, 3, 3, 3, 3, 24, 9, 9, 9, 9,
    9, 9, 9, 9, 25, 12, 12, 12, 16, 18, 11, 13, 18, 18, 18, 18, 18, 11, 9, 9, 9, 9, 21, 3, 3, 3, 7, 18, 2, 4, 18, 18, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 18, 18, 18, 11, 13, 18, 18, 18, 18, 18, 11, 9, 9, 9, 9, 4, 18, 29, 29, 18, 18, 2, 4, 18, 18, 18, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 26, 10, 10, 10, 17, 29, 15, 16, 18, 14, 17, 29, 29, 11, 9, 9, 9, 9, 22, 1, 1, 1, 8, 18, 6, 7, 29, 5, 8, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 25, 12, 12, 12, 16, 18, 18, 18, 18, 11, 13, 18, 18, 11, 9, 9, 9, 9, 21, 3, 3, 3, 7, 18, 18, 18, 18, 2, 4, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 18, 18, 18, 18, 18, 18, 11, 13, 18, 18, 11, 9, 9, 9, 9, 4, 18, 29, 29, 18, 18, 18, 18, 18, 2, 4, 18, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 10, 10, 10, 10, 17, 18, 14, 27, 26, 17, 18, 11, 9, 9, 9, 9, 4, 18, 1, 1, 1, 1, 8, 29, 5, 23, 22, 8, 29, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 29, 29, 18, 9, 13, 18, 11, 18, 18, 13, 18, 11, 9, 9, 9, 9, 4, 18, 18, 18, 18, 9, 4, 18, 2, 19, 18, 4, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 29, 14, 17, 18, 9, 26, 10, 27, 9, 18, 13, 18, 11, 9, 9, 9, 9, 4, 18, 5, 8, 29, 9, 22, 1, 23, 9, 18, 4, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 29, 15, 16, 18, 9, 9, 9, 9, 18, 18, 13, 18, 11, 9, 9, 9, 9, 4, 18, 6, 7, 29, 9, 9, 9, 9, 29, 29, 4, 18, 2, 9, 9, 9, 9,
    9, 9, 9, 9, 13, 18, 18, 18, 18, 18, 18, 18, 18, 18, 9, 26, 10, 27, 9, 9, 9, 9, 4, 18, 29, 29, 18, 9, 9, 9, 9, 29, 9, 22, 1, 23, 9, 9, 9, 9,
    9, 9, 9, 9, 26, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 22, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
    9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
};

LevelB::~LevelB()
{
    delete[] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelB::initialise()
{
    m_state.next_scene_id = -1;

    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELB_DATA, map_texture_id, 1.0f, 5, 6);

    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(23.0f, -6.0f, 0.0f));
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
    m_state.enemies[0].set_position(glm::vec3(27.0f, -5.0f, 0.0f));//27.0
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].m_speed = 1.0f;
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));

    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(GUARD);
    m_state.enemies[1].set_ai_state(IDLE);
    m_state.enemies[1].m_texture_id = enemy_texture_id;
    m_state.enemies[1].set_position(glm::vec3(16.0f, -11.0f, 0.0f));//27.0
    m_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_state.enemies[1].m_speed = 1.0f;
    m_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));


    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("assets/Platformer by Ethan Meng.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/bounce.wav");
}

void LevelB::update(float delta_time)
{
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map);
    m_state.enemies[0].update(delta_time, m_state.player, 0, 0, m_state.map);
    m_state.enemies[1].update(delta_time, m_state.player, 0, 0, m_state.map);
}

void LevelB::render(ShaderProgram* program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    m_state.enemies[0].render(program);
    m_state.enemies[1].render(program);
}