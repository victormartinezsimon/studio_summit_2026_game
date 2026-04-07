#pragma once
#include "ScreenConfig.h"

constexpr float POSITION_Y_PLAYER = 194;
constexpr int SCORE_POSITION_X = SCREEN_WIDTH;
constexpr int SCORE_POSITION_Y = SCREEN_HEIGHT - 23;
constexpr int TIME_POSITION_X = 0;

namespace MAIN_MENU_COORDS
{
    constexpr float TITLE_X = SCREEN_WIDTH * 0.5f;
    constexpr float TITLE_Y = 0.27 * SCREEN_HEIGHT;

    constexpr float SELECTOR_START_X = 0.2 * SCREEN_WIDTH;
    constexpr float SELECTOR_START_Y = 0.8 * SCREEN_HEIGHT;

    constexpr float START_X = SELECTOR_START_X;
    constexpr int START_Y = 0.6 * SCREEN_HEIGHT;

    constexpr float SELECTOR_EXIT_X = 0.8 * SCREEN_WIDTH;
    constexpr float SELECTOR_EXIT_Y = SELECTOR_START_Y;

    constexpr float EXIT_GAME_X = SELECTOR_EXIT_X;
    constexpr float EXIT_GAME_Y = START_Y;
};

namespace IMPROVEMENT_SELECTION_COORDS
{
    constexpr float OPTION_LEFT_X = SCREEN_WIDTH * 0.2;
    constexpr float OPTION_RIGHT_X = SCREEN_WIDTH * 0.8;
    constexpr float OPTION_Y = SCREEN_HEIGHT * 0.5;

    constexpr float SELECTOR_Y_PLAYER = MAIN_MENU_COORDS::SELECTOR_START_Y; //190;
    constexpr float SELECTOR_Y_ENEMY = SCREEN_HEIGHT - SELECTOR_Y_PLAYER;
};

namespace HIGH_SCORE_COORDS
{
    constexpr float DECREASE_LETTER_X = IMPROVEMENT_SELECTION_COORDS::OPTION_LEFT_X;
    constexpr float INCREASE_LETTER_X = IMPROVEMENT_SELECTION_COORDS::OPTION_RIGHT_X;
    constexpr float ACCEPT_LETTER_X = 0.5 * SCREEN_WIDTH;
    constexpr float SELECTOR_Y = IMPROVEMENT_SELECTION_COORDS::SELECTOR_Y_PLAYER;
    constexpr float SCORE_START_X = SCREEN_WIDTH * 0.5f;
    constexpr float SCORE_START_Y = SCREEN_HEIGHT * 0.1f;
    constexpr float RETURN_MENU_X = ACCEPT_LETTER_X;
    constexpr float RETURN_MENU_Y = MAIN_MENU_COORDS::START_Y;
};

namespace END_GAME_COORDS
{
    constexpr float TITLE_X = SCREEN_WIDTH * 0.5;
    constexpr float TITLE_Y = SCREEN_HEIGHT * 0.1;
    constexpr float SCORE_X = SCREEN_WIDTH * 0.5;
    constexpr float SCORE_Y = SCREEN_HEIGHT * 0.5;
    
};

