#ifndef GUARD_CONSTANTS_GLOBAL_H
#define GUARD_CONSTANTS_GLOBAL_H

#include "config/battle.h"
#include "config/debug.h"
#include "config/item.h"
#include "config/pokemon.h"
#include "config/overworld.h"

// Invalid Versions show as "----------" in Gen 4 and Gen 5's summary screen.
// In Gens 6 and 7, invalid versions instead show "a distant land" in the summary screen.
// In Gen 4 only, migrated Pokemon with Diamond, Pearl, or Platinum's ID show as "----------".
// Gen 5 and up read Diamond, Pearl, or Platinum's ID as "Sinnoh".
// In Gen 4 and up, migrated Pokemon with HeartGold or SoulSilver's ID show the otherwise unused "Johto" string.
#define VERSION_SAPPHIRE 1
#define VERSION_RUBY 2
#define VERSION_EMERALD 3
#define VERSION_FIRE_RED 4
#define VERSION_LEAF_GREEN 5
#define VERSION_HEART_GOLD 7
#define VERSION_SOUL_SILVER 8
#define VERSION_DIAMOND 10
#define VERSION_PEARL 11
#define VERSION_PLATINUM 12
#define VERSION_GAMECUBE 15

#define LANGUAGE_JAPANESE 1
#define LANGUAGE_ENGLISH  2
#define LANGUAGE_FRENCH   3
#define LANGUAGE_ITALIAN  4
#define LANGUAGE_GERMAN   5
#define LANGUAGE_KOREAN   6 // 6 goes unused but the theory is it was meant to be Korean
#define LANGUAGE_SPANISH  7
#define NUM_LANGUAGES     7

#define GAME_VERSION (VERSION_EMERALD)
#define GAME_LANGUAGE (LANGUAGE_ENGLISH)

// party sizes
#define PARTY_SIZE 6
#define MULTI_PARTY_SIZE (PARTY_SIZE / 2)
#define FRONTIER_PARTY_SIZE         3
#define FRONTIER_DOUBLES_PARTY_SIZE 4
#define FRONTIER_MULTI_PARTY_SIZE   2
#define MAX_FRONTIER_PARTY_SIZE    (max(FRONTIER_PARTY_SIZE,        \
                                    max(FRONTIER_DOUBLES_PARTY_SIZE,\
                                        FRONTIER_MULTI_PARTY_SIZE)))
#define UNION_ROOM_PARTY_SIZE       2

// capacities of various saveblock objects
#define DAYCARE_MON_COUNT 2
#define OBJECT_EVENTS_COUNT 16
#define MAIL_COUNT (10 + PARTY_SIZE)
#define SECRET_BASES_COUNT 5 // Originally 20 (5 is likely more than enough for rogue)
#define POKE_NEWS_COUNT 16

#define PC_ITEMS_COUNT 50
#define BAG_ITEM_CAPACITY (450) //236 from old slots + 40 * 2 from pokeblock + 158 * 15 from secret bases

// These slots will be shared between charms & key items
#define BAG_ITEM_RESERVED_SLOTS 50

// Old capacities
//#define BAG_ITEMS_COUNT 30
//#define BAG_KEYITEMS_COUNT 30
//#define BAG_POKEBALLS_COUNT 16
//#define BAG_TMHM_COUNT 64
//#define BAG_BERRIES_COUNT 46

#define OBJECT_EVENT_TEMPLATES_COUNT 64
#define DECOR_MAX_SECRET_BASE 16
#define DECOR_MAX_PLAYERS_HOUSE 12
#define APPRENTICE_COUNT 4
#define APPRENTICE_MAX_QUESTIONS 9
#define MAX_REMATCH_ENTRIES 100 // only REMATCH_TABLE_ENTRIES (78) are used
#define NUM_CONTEST_WINNERS 13
#define UNION_ROOM_KB_ROW_COUNT 10
#define GIFT_RIBBONS_COUNT 11
#define SAVED_TRENDS_COUNT 5
#define PYRAMID_BAG_ITEMS_COUNT 10
#define MAX_REGISTERED_ITEMS 4

// Number of facilities for Ranking Hall.
// 7 facilities for single mode + tower double mode + tower multi mode.
// Excludes link modes. See RANKING_HALL_* in include/constants/battle_frontier.h
#define HALL_FACILITIES_COUNT 9
// Received via record mixing, 1 for each player other than yourself
#define HALL_RECORDS_COUNT 3

// Battle Frontier level modes.
#define FRONTIER_LVL_50         0
#define FRONTIER_LVL_OPEN       1
#define FRONTIER_LVL_MODE_COUNT 2
#define FRONTIER_LVL_TENT       FRONTIER_LVL_MODE_COUNT // Special usage for indicating Battle Tent

#define TRAINER_ID_LENGTH 4
#define MAX_MON_MOVES 4
#define ALL_MOVES_MASK ((1 << MAX_MON_MOVES) - 1)

#define CONTESTANT_COUNT 4
#define CONTEST_CATEGORY_COOL     0
#define CONTEST_CATEGORY_BEAUTY   1
#define CONTEST_CATEGORY_CUTE     2
#define CONTEST_CATEGORY_SMART    3
#define CONTEST_CATEGORY_TOUGH    4
#define CONTEST_CATEGORIES_COUNT  5

// string lengths
#define ITEM_NAME_LENGTH 16 // increased
#define POKEMON_NAME_LENGTH 10
#define POKEMON_NAME_BUFFER_SIZE max(20, POKEMON_NAME_LENGTH + 1) // Frequently used buffer size. Larger than necessary
#define PLAYER_NAME_LENGTH 7
#define MAIL_WORDS_COUNT 9
#define EASY_CHAT_BATTLE_WORDS_COUNT 6
#if B_EXPANDED_MOVE_NAMES == TRUE
#define MOVE_NAME_LENGTH 16
#else
#define MOVE_NAME_LENGTH 12
#endif
#define NUM_QUESTIONNAIRE_WORDS 4
#define QUIZ_QUESTION_LEN 9
#define WONDER_CARD_TEXT_LENGTH 40
#define WONDER_NEWS_TEXT_LENGTH 40
#define WONDER_CARD_BODY_TEXT_LINES 4
#define WONDER_NEWS_BODY_TEXT_LINES 10
#define TYPE_NAME_LENGTH 6
#if B_EXPANDED_ABILITY_NAMES == TRUE
#define ABILITY_NAME_LENGTH 16
#else
#define ABILITY_NAME_LENGTH 12
#endif
#define TRAINER_NAME_LENGTH 10
#define POKEMON_HUB_NAME_LENGTH 15

#define MAX_STAMP_CARD_STAMPS 7

#define MALE 0
#define FEMALE 1
#define GENDER_COUNT 2

#define STYLE_EMR_BRENDAN   0
#define STYLE_EMR_MAY       1
#define STYLE_RED           2
#define STYLE_LEAF          3
#define STYLE_ETHAN         4
#define STYLE_LYRA          5
#define PLAYER_STYLE_COUNT 6

#define BARD_SONG_LENGTH       6
#define NUM_STORYTELLER_TALES  4
#define NUM_TRADER_ITEMS       4
#define GIDDY_MAX_TALES       10
#define GIDDY_MAX_QUESTIONS    8

#define OPTIONS_BUTTON_MODE_NORMAL 0
#define OPTIONS_BUTTON_MODE_LR 1
#define OPTIONS_BUTTON_MODE_L_EQUALS_A 2

#define OPTIONS_TEXT_SPEED_SLOW 0
#define OPTIONS_TEXT_SPEED_MID 1
#define OPTIONS_TEXT_SPEED_FAST 2

#define OPTIONS_SOUND_MONO 0
#define OPTIONS_SOUND_STEREO 1

#define OPTIONS_BATTLE_STYLE_SHIFT 0
#define OPTIONS_BATTLE_STYLE_SET 1

#define OPTIONS_BATTLE_SCENE_1X         0
#define OPTIONS_BATTLE_SCENE_2X         1
#define OPTIONS_BATTLE_SCENE_3X         2
#define OPTIONS_BATTLE_SCENE_4X         3
#define OPTIONS_BATTLE_SCENE_DISABLED   4
#define OPTIONS_BATTLE_SCENE_COUNT      5

#define OPTIONS_HEALTH_BEEP_OFF        0
#define OPTIONS_HEALTH_BEEP_3_BEEPS    1
#define OPTIONS_HEALTH_BEEP_LOOPING    2
#define OPTIONS_HEALTH_BEEP_COUNT      3

#define OPTIONS_NICKNAME_MODE_ASK           0
#define OPTIONS_NICKNAME_MODE_ALWAYS        1
#define OPTIONS_NICKNAME_MODE_NEVER         2
#define OPTIONS_NICKNAME_RANDOM             3
#define OPTIONS_NICKNAME_COUNT              4

// Mocha additions
#define OPTIONS_DIFFICULTY_REWARD_MODE_VANILLA      0
#define OPTIONS_DIFFICULTY_REWARD_MODE_MULTIPLIER   1

#define OPTIONS_RIDEMON_CONTROL_VANILLA 0
#define OPTIONS_RIDEMON_CONTROL_MOCHA 1

#define OPTIONS_SHOWMONEY_OFF 0
#define OPTIONS_SHOWMONEY_ON 1

#define OPTIONS_QUICKROUTE_OFF 0
#define OPTIONS_QUICKROUTE_ON  1

#define OPTIONS_IGNOREBAG_OFF 0
#define OPTIONS_IGNOREBAG_ON  1

// Mocha additions end

#define DIR_NONE        0
#define DIR_SOUTH       1
#define DIR_NORTH       2
#define DIR_WEST        3
#define DIR_EAST        4
#define DIR_SOUTHWEST   5
#define DIR_SOUTHEAST   6
#define DIR_NORTHWEST   7
#define DIR_NORTHEAST   8

#define CONNECTION_INVALID -1
#define CONNECTION_NONE     0
#define CONNECTION_SOUTH    1
#define CONNECTION_NORTH    2
#define CONNECTION_WEST     3
#define CONNECTION_EAST     4
#define CONNECTION_DIVE     5
#define CONNECTION_EMERGE   6

#define SIDEWAYS_STAIRS_IMPLEMENTED         TRUE
#define FOLLOW_ME_IMPLEMENTED               TRUE
#define POST_BATTLE_FOLLOWER_FIX            FALSE   //if you experience the follower de-syncing with the player after battle, set to TRUE

#endif // GUARD_CONSTANTS_GLOBAL_H
