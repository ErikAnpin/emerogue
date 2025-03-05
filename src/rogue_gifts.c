#include "global.h"
#include "constants/abilities.h"
#include "constants/flags.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/region_map_sections.h"
#include "event_data.h"
#include "random.h"

#include "rogue_baked.h"
#include "rogue_gifts.h"
#include "rogue_hub.h"
#include "rogue_pokedex.h"
#include "rogue_query.h"
#include "rogue_quest.h"
#include "rogue_save.h"

struct CustomTrainerData
{
    u8 const* name;
    u32 trainerId;
    u8 trainerColour;
};

struct CustomMonData
{
    u8 const* nickname;
    u16 const* moves;
    u16 const* abilities;
    u32 otId;
    u16 movesCount;
    u16 customTrainerId;
    u16 species;
    u16 pokeball;
    u16 heldItem;
    u16 isShiny : 1;
    u16 isDefaultSpawn : 1;
};

static u8 const sRarityToCustomTrainerIndex[] = 
{
    [UNIQUE_RARITY_COMMON]      = CUSTOM_TRAINER_COMMON,
    [UNIQUE_RARITY_RARE]        = CUSTOM_TRAINER_RARE,
    [UNIQUE_RARITY_EPIC]        = CUSTOM_TRAINER_EPIC,
    [UNIQUE_RARITY_EXOTIC]      = CUSTOM_TRAINER_EXOTIC,
};

static u16 const sDynamicCustomMonAbilities[] = 
{
#ifdef ROGUE_EXPANSION
    ABILITY_GOOD_AS_GOLD, //1
    ABILITY_STORM_DRAIN,
    ABILITY_MOTOR_DRIVE,
    ABILITY_FLASH_FIRE,
    ABILITY_SPEED_BOOST, //5
    ABILITY_ADAPTABILITY,
    ABILITY_INTIMIDATE,
    ABILITY_DRIZZLE,
    ABILITY_DROUGHT,
    ABILITY_SNOW_WARNING, //10
    ABILITY_SAND_STREAM,
    ABILITY_UNBURDEN,
    ABILITY_NEUTRALIZING_GAS,
    ABILITY_ELECTRIC_SURGE,
    ABILITY_PSYCHIC_SURGE, //15
    ABILITY_MISTY_SURGE,
    ABILITY_GRASSY_SURGE,
    ABILITY_PROTEAN,
    ABILITY_MAGIC_BOUNCE,
    ABILITY_MOXIE, //20
    ABILITY_WANDERING_SPIRIT,
    ABILITY_REGENERATOR,
    ABILITY_SUPER_LUCK,
    ABILITY_CONTRARY,
    ABILITY_NO_GUARD, //25
    ABILITY_SKILL_LINK,
    ABILITY_MIRROR_ARMOR,
    ABILITY_GUTS,
    ABILITY_THICK_FAT,
    ABILITY_INTREPID_SWORD, //30
    ABILITY_SERENE_GRACE,
    ABILITY_DAUNTLESS_SHIELD,
    ABILITY_WEAK_ARMOR,
    ABILITY_TOXIC_DEBRIS,
    ABILITY_QUICK_DRAW, //35
    ABILITY_GORILLA_TACTICS,
    ABILITY_TOUGH_CLAWS,
    ABILITY_CLEAR_BODY,
    ABILITY_TECHNICIAN,
    ABILITY_GUARD_DOG, //40
    ABILITY_PRANKSTER,
    ABILITY_BEADS_OF_RUIN,
    ABILITY_SWORD_OF_RUIN,
    ABILITY_LEVITATE,
    ABILITY_MULTISCALE, //45
    ABILITY_VESSEL_OF_RUIN,
    ABILITY_POISON_HEAL,
    ABILITY_NEUROFORCE,
    ABILITY_MOODY,
    ABILITY_TABLETS_OF_RUIN, //50
    ABILITY_COMATOSE,
    ABILITY_ANGER_SHELL,
    ABILITY_SIMPLE,
    ABILITY_UNAWARE,
    ABILITY_HARVEST, //55
    ABILITY_MOLD_BREAKER,
    ABILITY_SHEER_FORCE,
    ABILITY_NATURAL_CURE,
    ABILITY_ROUGH_SKIN,
    ABILITY_DEFIANT, //60
    ABILITY_PERISH_BODY,
    ABILITY_SNIPER,
    ABILITY_COLOR_CHANGE,
    ABILITY_AERILATE,
    ABILITY_BAD_DREAMS, //65
    ABILITY_BEAST_BOOST,
    ABILITY_QUICK_FEET,
    ABILITY_COMPETITIVE,
    ABILITY_COMPOUND_EYES,
    ABILITY_CORROSION, //70
    ABILITY_COTTON_DOWN,
    ABILITY_RIPEN,
    ABILITY_DELTA_STREAM,
    ABILITY_DESOLATE_LAND,
    ABILITY_DOWNLOAD, //75
    ABILITY_DRAGONS_MAW,
    ABILITY_DRY_SKIN,
    ABILITY_EARTH_EATER,
    ABILITY_EFFECT_SPORE,
    ABILITY_FILTER, //80
    ABILITY_FLARE_BOOST,
    ABILITY_FUR_COAT,
    ABILITY_GALE_WINGS,
    ABILITY_GALVANIZE,
    ABILITY_HUSTLE, //85
    ABILITY_ICE_SCALES,
    ABILITY_LIGHTNING_ROD,
    ABILITY_MAGIC_GUARD,
    ABILITY_MEGA_LAUNCHER,
    ABILITY_MERCILESS, //90
    ABILITY_MINDS_EYE,
    ABILITY_MOTOR_DRIVE,
    ABILITY_NORMALIZE,
    ABILITY_OPPORTUNIST,
    ABILITY_OWN_TEMPO, //95
    ABILITY_PIXILATE,
    ABILITY_POISON_PUPPETEER,
    ABILITY_PRIMORDIAL_SEA,
    ABILITY_PROTOSYNTHESIS,
    ABILITY_PUNK_ROCK, //100
    ABILITY_PURIFYING_SALT,
    ABILITY_QUARK_DRIVE,
    ABILITY_QUEENLY_MAJESTY,
    ABILITY_RECKLESS,
    ABILITY_REFRIGERATE, //105
    ABILITY_ANGER_POINT,
    ABILITY_ROCKY_PAYLOAD,
    ABILITY_SAP_SIPPER,
    ABILITY_SHADOW_TAG,
    ABILITY_SHARPNESS, //110
    ABILITY_SHED_SKIN,
    ABILITY_SHIELD_DUST,
    ABILITY_SOUL_HEART,
    ABILITY_STAMINA,
    ABILITY_STEELY_SPIRIT, //115
    ABILITY_STENCH,
    ABILITY_STORM_DRAIN,
    ABILITY_STRONG_JAW,
    ABILITY_STURDY,
    ABILITY_TINTED_LENS, //120
    ABILITY_TOXIC_CHAIN,
    ABILITY_TRACE,
    ABILITY_ELECTROMORPHOSIS,
    ABILITY_TRIAGE,
    ABILITY_WATER_BUBBLE, //125
    ABILITY_WELL_BAKED_BODY,
    ABILITY_WIND_RIDER
#endif
};

static u16 const sDynamicCustomMonMoves[] = 
{
#ifdef ROGUE_EXPANSION
    MOVE_HYDRO_STEAM, //1
    MOVE_BOUNCY_BUBBLE,
    MOVE_TORCH_SONG,
    MOVE_AQUA_STEP,
    MOVE_STONE_AXE, //5
    MOVE_TRIPLE_ARROWS,
    MOVE_SANDSEAR_STORM,
    MOVE_SURGING_STRIKES,
    MOVE_PARTING_SHOT,
    MOVE_GRAV_APPLE, //10
    MOVE_CORE_ENFORCER,
    MOVE_THOUSAND_ARROWS,
    MOVE_BURNING_BULWARK,
    MOVE_DIAMOND_STORM,
    MOVE_MAGICAL_TORQUE, //15
    MOVE_FREEZE_DRY,
    MOVE_SHADOW_FORCE,
    MOVE_V_CREATE,
    MOVE_SECRET_SWORD,
    MOVE_TRICK_ROOM, //20
    MOVE_SPORE,
    MOVE_NOXIOUS_TORQUE,
    MOVE_TAIL_GLOW,
    MOVE_SUPERPOWER,
    MOVE_U_TURN,  //25
    MOVE_SHIFT_GEAR,
    MOVE_TAKE_HEART,
    MOVE_FLOWER_TRICK,
    MOVE_DOUBLE_IRON_BASH,
    MOVE_CEASELESS_EDGE, //30
    MOVE_COSMIC_POWER,
    MOVE_FAKE_OUT,
    MOVE_EXTREME_SPEED,
    MOVE_POWER_GEM,
    MOVE_RAPID_SPIN, //35
    MOVE_LEECH_LIFE,
    MOVE_SYNTHESIS,
    MOVE_ELECTRO_DRIFT,
    MOVE_SWORDS_DANCE,
    MOVE_THUNDERCLAP, //40
    MOVE_GIGATON_HAMMER,
    MOVE_SALT_CURE,
    MOVE_WICKED_BLOW,
    MOVE_SPECTRAL_THIEF,
    MOVE_METEOR_BEAM, //45
    MOVE_SPIRIT_BREAK,
    MOVE_SHORE_UP,
    MOVE_NO_RETREAT,
    MOVE_SHED_TAIL,
    MOVE_BELLY_DRUM, //50
    MOVE_TOPSY_TURVY,
    MOVE_QUIVER_DANCE,
    MOVE_BATON_PASS,
    MOVE_ASTRAL_BARRAGE,
    MOVE_ARMOR_CANNON, //55
    MOVE_POWER_TRIP,
    MOVE_BOOMBURST,
    MOVE_SUCKER_PUNCH,
    MOVE_COLLISION_COURSE,
    MOVE_MIGHTY_CLEAVE, //60
    MOVE_FIRST_IMPRESSION,
    MOVE_RAGE_FIST,
    MOVE_ACROBATICS,
    MOVE_AEROBLAST,
    MOVE_APPLE_ACID, //65
    MOVE_AURORA_VEIL,
    MOVE_BADDY_BAD,
    MOVE_INGRAIN,
    MOVE_BITTER_BLADE,
    MOVE_BODY_PRESS, //70
    MOVE_BOLT_BEAK,
    MOVE_BUZZY_BUZZ,
    MOVE_CLANGOROUS_SOUL,
    MOVE_COIL,
    MOVE_COTTON_GUARD, //75
    MOVE_ACUPRESSURE,
    MOVE_DIRE_CLAW,
    MOVE_DRACO_METEOR,
    MOVE_DRAGON_ASCENT,
    MOVE_DRAGON_DARTS, //80
    MOVE_DRAGON_ENERGY,
    MOVE_EARTH_POWER,
    MOVE_ERUPTION,
    MOVE_ESPER_WING,
    MOVE_FIERY_WRATH, //85
    MOVE_FISHIOUS_REND,
    MOVE_FLEUR_CANNON,
    MOVE_FLOATY_FALL,
    MOVE_FREEZY_FROST,
    MOVE_GEOMANCY, //90
    MOVE_GLACIAL_LANCE,
    MOVE_GLAIVE_RUSH,
    MOVE_GLITZY_GLOW,
    MOVE_HEADLONG_RUSH,
    MOVE_HYPER_DRILL, //95
    MOVE_INFERNAL_PARADE,
    MOVE_JUNGLE_HEALING,
    MOVE_LEAF_STORM,
    MOVE_MAKE_IT_RAIN,
    MOVE_MALIGNANT_CHAIN, //100
    MOVE_MATCHA_GOTCHA,
    MOVE_MOONBLAST,
    MOVE_MORTAL_SPIN,
    MOVE_TRIPLE_AXEL,
    MOVE_OBLIVION_WING, //105
    MOVE_PHOTON_GEYSER,
    MOVE_POLLEN_PUFF,
    MOVE_POPULATION_BOMB,
    MOVE_PSYBLADE,
    MOVE_PSYCHIC_FANGS, //110
    MOVE_PSYCHO_BOOST,
    MOVE_SAPPY_SEED,
    MOVE_SEARING_SHOT,
    MOVE_SHEER_COLD,
    MOVE_SHELL_SMASH, //115
    MOVE_SIZZLY_SLIDE,
    MOVE_SPARKLY_SWIRL,
    MOVE_SPLISHY_SPLASH,
    MOVE_STICKY_WEB,
    MOVE_STRENGTH_SAP, //120
    MOVE_STUFF_CHEEKS,
    MOVE_TACHYON_CUTTER,
    MOVE_TERRAIN_PULSE,
    MOVE_TOXIC_SPIKES,
    MOVE_VICTORY_DANCE, //125
    MOVE_WATER_SPOUT,
    MOVE_ZIPPY_ZAP
#endif
};

STATIC_ASSERT(ARRAY_COUNT(sDynamicCustomMonAbilities) <= 127, SizeOfDynamicCustomMonAbilities);
STATIC_ASSERT(ARRAY_COUNT(sDynamicCustomMonMoves) <= 127, SizeOfDynamicCustomMonMoves);

#include "data/rogue/custom_mons.h"

struct CompressedDynamicData
{
    u32 move1:7; // 127 indices
    u32 move2:7; // 127 indices
    u32 move3:7; // 127 indices
    u32 unused:2; // 127 indices
    u32 ability:7; // 127 indices
    u32 reserved:2; // reserved for bitmask OTID_FLAG_CUSTOM_MON etc.
};

struct DynamicMonData
{
    u16 moves[MAX_MON_MOVES];
    u16 movesCount;
    u16 ability;
};

STATIC_ASSERT(sizeof(struct CompressedDynamicData) == sizeof(u32), SizeOfDynamicCustomMonData);

static void UncompressDynamicMonData(u32 customMonId, struct DynamicMonData* outData)
{
    struct CompressedDynamicData* compressedData = (struct CompressedDynamicData*)&customMonId;

    outData->ability = ((compressedData->ability - 1) < ARRAY_COUNT(sDynamicCustomMonAbilities)) ? sDynamicCustomMonAbilities[compressedData->ability - 1] : ABILITY_NONE;
    outData->movesCount = 0;

    if(compressedData->move1 != 0 && (compressedData->move1 - 1) < ARRAY_COUNT(sDynamicCustomMonMoves))
        outData->moves[outData->movesCount++] = sDynamicCustomMonMoves[compressedData->move1 - 1];

    if(compressedData->move2 != 0 && (compressedData->move2 - 1) < ARRAY_COUNT(sDynamicCustomMonMoves))
        outData->moves[outData->movesCount++] = sDynamicCustomMonMoves[compressedData->move2 - 1];

    if(compressedData->move3 != 0 && (compressedData->move3 - 1) < ARRAY_COUNT(sDynamicCustomMonMoves))
        outData->moves[outData->movesCount++] = sDynamicCustomMonMoves[compressedData->move3 - 1];

    //if(compressedData->move4 != 0 && (compressedData->move4 - 1) < ARRAY_COUNT(sDynamicCustomMonMoves))
    //    outData->moves[outData->movesCount++] = sDynamicCustomMonMoves[compressedData->move4 - 1];
};

static u32 CompressedDynamicDataToCustomMonId(struct CompressedDynamicData* inData)
{
    u32 id;
    id = *((u32*)inData);
    return id;
}

u32 RogueGift_GetCustomMonId(struct Pokemon* mon)
{
    return RogueGift_GetCustomMonIdBySpecies(GetMonData(mon, MON_DATA_SPECIES), GetMonData(mon, MON_DATA_OT_ID));
}

u32 RogueGift_GetCustomBoxMonId(struct BoxPokemon* mon)
{
    return RogueGift_GetCustomMonIdBySpecies(GetBoxMonData(mon, MON_DATA_SPECIES), GetBoxMonData(mon, MON_DATA_OT_ID));
}

static u16 TransformCheckSpecies(u16 species)
{
#ifdef ROGUE_EXPANSION
    // Special case (probably should get a proper fix for this)
    if(species >= SPECIES_PIKACHU_COSPLAY && species <= SPECIES_PICHU_SPIKY_EARED)
        return species;

    if(!gSpeciesInfo[species].isAlolanForm && !gSpeciesInfo[species].isGalarianForm && !gSpeciesInfo[species].isHisuianForm && !gSpeciesInfo[species].isPaldeanForm)
    {
        // Fix for castform/other form changes
        return GET_BASE_SPECIES_ID(species);
    }
#endif

    return species;
}

u32 RogueGift_GetCustomMonIdBySpecies(u16 species, u32 otId)
{
    u32 i;

#ifdef ROGUE_EXPANSION
    // The mega itself has it's own ability so don't redirect
    if(species == SPECIES_PIKIN_MEGA)
        return CUSTOM_MON_NONE;
#endif

    species = TransformCheckSpecies(species);

    if((otId & OTID_FLAG_CUSTOM_MON) && (otId & OTID_FLAG_DYNAMIC_CUSTOM_MON))
    {
        // otId is the custom mon id, as it encodes the data into it
        return otId;
    }
    if(otId & OTID_FLAG_CUSTOM_MON)
    {
        species = Rogue_GetEggSpecies(species);

        for(i = CUSTOM_MON_NONE + 1; i < CUSTOM_MON_COUNT; ++i)
        {
            struct CustomMonData const* monData = &sCustomPokemon[i];
            
            if(monData->otId == otId && Rogue_GetEggSpecies(monData->species) == species)
                return i;
        }
    }

    return CUSTOM_MON_NONE;
}

u16 RogueGift_GetCustomMonMove(u32 id, u8 i)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        struct DynamicMonData dynamicData;
        UncompressDynamicMonData(id, &dynamicData);
        return i < dynamicData.movesCount ? dynamicData.moves[i] : MOVE_NONE;
    }
    else
    {
        AGB_ASSERT(id < CUSTOM_MON_COUNT);

        if(id != CUSTOM_MON_NONE)
        {
            struct CustomMonData const* monData = &sCustomPokemon[id];
            return i < monData->movesCount ? monData->moves[i] : MOVE_NONE;
        }
    }

    return MOVE_NONE;
}

u16 RogueGift_GetCustomMonMoveCount(u32 id)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        struct DynamicMonData dynamicData;
        UncompressDynamicMonData(id, &dynamicData);
        return dynamicData.movesCount;
    }
    else
    {
        AGB_ASSERT(id < CUSTOM_MON_COUNT);

        if(id != CUSTOM_MON_NONE)
        {
            struct CustomMonData const* monData = &sCustomPokemon[id];

            if(id == CUSTOM_MON_ABBIE_MAREEP && !FlagGet(FLAG_ROGUE_UNLOCKED_PIKIN_EASTER_EGG))
            {
                // Don't show last move until activated easter egg
                return monData->movesCount - 1;
            }

            return monData->movesCount;
        }
    }

    return 0;
}

u16 RogueGift_GetCustomMonAbility(u32 id, u8 i)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        struct DynamicMonData dynamicData;
        UncompressDynamicMonData(id, &dynamicData);
        return i == 0 ? dynamicData.ability : ABILITY_NONE;
    }
    else
    {
        AGB_ASSERT(id < CUSTOM_MON_COUNT);

        if(id != CUSTOM_MON_NONE)
        {
            struct CustomMonData const* monData = &sCustomPokemon[id];
            return (monData->abilities != NULL && i < NUM_ABILITY_SLOTS) ? monData->abilities[i] : ABILITY_NONE;
        }
    }

    return ABILITY_NONE;
}

u16 RogueGift_GetCustomMonAbilityCount(u32 id)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        struct DynamicMonData dynamicData;
        UncompressDynamicMonData(id, &dynamicData);
        return dynamicData.ability != ABILITY_NONE ? NUM_ABILITY_SLOTS : 0;
    }
    else
    {
        struct CustomMonData const* monData = &sCustomPokemon[id];
        AGB_ASSERT(id < CUSTOM_MON_COUNT);

        return (monData->abilities != NULL) ? NUM_ABILITY_SLOTS : 0;
    }
}

bool8 RogueGift_CanRenameCustomMon(u32 id)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        return TRUE;
    }
    else
    {
        // We're allowed to rename exotics, as they're technically dynamic custom mons too
        struct CustomMonData const* monData = &sCustomPokemon[id];
        AGB_ASSERT(id < CUSTOM_MON_COUNT);

        return monData->customTrainerId == CUSTOM_TRAINER_EXOTIC;
    }
}

bool8 RogueGift_DisplayCustomMonRarity(u32 id)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
        return TRUE;
    else
    {
        struct CustomMonData const* monData = &sCustomPokemon[id]; 
        if(monData->customTrainerId == CUSTOM_TRAINER_EXOTIC)
            return TRUE;
    }

    return FALSE;
}

u8 RogueGift_GetCustomMonRarity(u32 id)
{
    if(id & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        u16 moveCount = RogueGift_GetCustomMonMoveCount(id);
        u16 abilityCount = RogueGift_GetCustomMonAbilityCount(id);

        if(abilityCount != 0)
        {
            if(moveCount <= 1)
                return UNIQUE_RARITY_RARE;
            else
                return UNIQUE_RARITY_EPIC;
        }

        return UNIQUE_RARITY_COMMON;
    }
    else
    {
        struct CustomMonData const* monData = &sCustomPokemon[id]; 
        AGB_ASSERT(id < CUSTOM_MON_COUNT);

        switch (monData->customTrainerId)
        {
        case CUSTOM_TRAINER_COMMON:
            return UNIQUE_RARITY_COMMON;
        
        case CUSTOM_TRAINER_RARE:
            return UNIQUE_RARITY_RARE;
        
        case CUSTOM_TRAINER_EPIC:
            return UNIQUE_RARITY_EPIC;
        
        case CUSTOM_TRAINER_EXOTIC:
            return UNIQUE_RARITY_EXOTIC;
        }

        // We expect most/all of these to be classed as exotic
        return UNIQUE_RARITY_EXOTIC;
    }
}

u8 const* RogueGift_GetRarityName(u8 rarity)
{
    switch (rarity)
    {
    case UNIQUE_RARITY_COMMON:
        return sCustomTrainers[CUSTOM_TRAINER_COMMON].name;
    
    case UNIQUE_RARITY_RARE:
        return sCustomTrainers[CUSTOM_TRAINER_RARE].name;
    
    case UNIQUE_RARITY_EPIC:
        return sCustomTrainers[CUSTOM_TRAINER_EPIC].name;
    
    case UNIQUE_RARITY_EXOTIC:
        return sCustomTrainers[CUSTOM_TRAINER_EXOTIC].name;
    }

    AGB_ASSERT(FALSE);
    return sCustomTrainers[CUSTOM_TRAINER_QUESTS].name;
}

void RogueGift_CreateMon(u32 customMonId, struct Pokemon* mon, u16 species, u8 level, u8 fixedIV)
{
    struct CustomTrainerData const* trainerData = NULL;
    u32 temp = 0;
    u16 customMoveCount = RogueGift_GetCustomMonMoveCount(customMonId);
    u16 customAbilityCount = RogueGift_GetCustomMonAbilityCount(customMonId);
    
    if(customMonId & OTID_FLAG_DYNAMIC_CUSTOM_MON)
    {
        u8 customTrainerId = sRarityToCustomTrainerIndex[RogueGift_GetCustomMonRarity(customMonId)];
        trainerData = &sCustomTrainers[customTrainerId];

        ZeroMonData(mon);
        CreateMon(mon, species, level, fixedIV, 0, 0, OT_ID_CUSTOM_MON, customMonId);

        // Assign pokeball
        temp = ITEM_ROGUE_BALL;
        SetMonData(mon, MON_DATA_POKEBALL, &temp);
    }
    else
    {
        struct CustomMonData const* monData = &sCustomPokemon[customMonId]; 
        trainerData = &sCustomTrainers[monData->customTrainerId];

        AGB_ASSERT(customMonId < CUSTOM_MON_COUNT);
        AGB_ASSERT(Rogue_GetEggSpecies(species) == Rogue_GetEggSpecies(monData->species));
        AGB_ASSERT(trainerData->trainerId & OTID_FLAG_CUSTOM_MON);
        AGB_ASSERT(!(trainerData->trainerId & OTID_FLAG_DYNAMIC_CUSTOM_MON));

        ZeroMonData(mon);
        CreateMon(mon, species, level, fixedIV, 0, 0, OT_ID_CUSTOM_MON, trainerData->trainerId);

        // Update nickname
        if(monData->nickname != NULL)
        {
            SetMonData(mon, MON_DATA_NICKNAME, monData->nickname);
        }

        // Assign pokeball
        temp = monData->pokeball;
        SetMonData(mon, MON_DATA_POKEBALL, &temp);

        // Set shiny state
        temp = monData->isShiny ? 1 : 0;
        SetMonData(mon, MON_DATA_IS_SHINY, &temp);
    }

    // Met location
    temp = METLOC_FATEFUL_ENCOUNTER;
    SetMonData(mon, MON_DATA_MET_LOCATION, &temp);

    // Update OT name
    SetMonData(mon, MON_DATA_OT_NAME, trainerData->name);

    // Update OT colour
    temp = trainerData->trainerColour;
    SetMonData(mon, MON_DATA_OT_GENDER, &temp);

    // Default moveset is first 4 moves from custom moveset
    if(customMoveCount)
    {
        u8 m, j;
        u16 moves[MAX_MON_MOVES];

        for(m = 0; m < MAX_MON_MOVES && m < customMoveCount; ++m)
        {
            moves[m] = RogueGift_GetCustomMonMove(customMonId, m);
        }

        // Fill the rest of the moves with default moves
        for(j = 0; m < MAX_MON_MOVES; ++m, ++j)
        {
            moves[m] = GetMonData(mon, MON_DATA_MOVE1 + j);
        }

        // Give back moves
        for(m = 0; m < MAX_MON_MOVES; ++m)
        {
            temp = moves[m];
            SetMonData(mon, MON_DATA_MOVE1 + m, &temp);
            SetMonData(mon, MON_DATA_PP1 + m, &gBattleMoves[temp].pp);
        }
    }

    // Default custom ability
    if(customAbilityCount)
    {
        temp = (RogueGift_GetCustomMonAbility(customMonId, 1) != ABILITY_NONE) ? (Random() % 2) : 0;
        SetMonData(mon, MON_DATA_ABILITY_NUM, &temp);
    }
}

static u32 SelectNextMoveIndex(struct CompressedDynamicData* compressedData, u16 species)
{
    if(RogueMiscQuery_AnyActiveElements())
    {
        u32 i;
        u16 moveId = RogueMiscQuery_SelectRandomElement(Random());
        RogueMiscQuery_EditElement(QUERY_FUNC_EXCLUDE, moveId);

        for (i = 0; i < ARRAY_COUNT(sDynamicCustomMonMoves); i++)
        {
            if(sDynamicCustomMonMoves[i] == moveId)
                return 1 + i;
        }

        // Should never get here
        AGB_ASSERT(FALSE);
        return 1;
    }

    // Can get here if we've ran out of move options, as everything else is already known
    return 0;
}

static u32 SelectNextAbilityIndex(struct CompressedDynamicData* compressedData, u16 species)
{
    u8 i;

    // Give the mon a new ability for it
    while(TRUE)
    {
        u32 idx = (Random() % ARRAY_COUNT(sDynamicCustomMonAbilities));

        for(i = 0; i < NUM_ABILITY_SLOTS; ++i)
        {
            if(GetAbilityBySpecies(species, i, 0) == sDynamicCustomMonAbilities[idx])
            {
                idx = 10000;
                break;
            }
        }

        if(idx != 10000)
            return 1 + idx;
    }

    return 0;
}

u32 RogueGift_CreateDynamicMonId(u8 rarity, u16 species)
{
    u16 i;
    u32 temp;
    struct CompressedDynamicData compressedData = {0};
    struct CustomTrainerData const* trainerData = &sCustomTrainers[sRarityToCustomTrainerIndex[rarity]];

    // Start query with moves which are valid
    RogueCustomQuery_Begin();

    for (i = 0; i < ARRAY_COUNT(sDynamicCustomMonMoves); i++)
    {
        RogueMiscQuery_EditElement(QUERY_FUNC_INCLUDE, sDynamicCustomMonMoves[i]);
    }

    for (i = 0; gRoguePokemonProfiles[species].levelUpMoves[i].move != MOVE_NONE; i++)
    {
        RogueMiscQuery_EditElement(QUERY_FUNC_EXCLUDE, gRoguePokemonProfiles[species].levelUpMoves[i].move);
    }

    for (i = 0; gRoguePokemonProfiles[species].tutorMoves[i] != MOVE_NONE; i++)
    {
        RogueMiscQuery_EditElement(QUERY_FUNC_EXCLUDE, gRoguePokemonProfiles[species].tutorMoves[i]);
    }

    switch (rarity)
    {
    case UNIQUE_RARITY_COMMON:
        compressedData.move1 = SelectNextMoveIndex(&compressedData, species);
        compressedData.move2 = SelectNextMoveIndex(&compressedData, species);
        break;

    case UNIQUE_RARITY_RARE:
        compressedData.move1 = SelectNextMoveIndex(&compressedData, species);
        compressedData.ability = SelectNextAbilityIndex(&compressedData, species);
        break;

    case UNIQUE_RARITY_EPIC:
        compressedData.move1 = SelectNextMoveIndex(&compressedData, species);
        compressedData.move2 = SelectNextMoveIndex(&compressedData, species);
        compressedData.move3 = SelectNextMoveIndex(&compressedData, species);
        //compressedData.move4 = SelectNextMoveIndex(&compressedData, species);
        compressedData.move1 = 44; //st
        compressedData.move2 = 69; //bb
        compressedData.move3 = 127; //zz
        compressedData.ability = SelectNextAbilityIndex(&compressedData, species);
        compressedData.ability = 43; //sor
        break;

    default:
        AGB_ASSERT(FALSE);
        break;
    }

    RogueCustomQuery_End();

    temp = CompressedDynamicDataToCustomMonId(&compressedData);
    temp |= (OTID_FLAG_CUSTOM_MON | OTID_FLAG_DYNAMIC_CUSTOM_MON);

#ifdef ROGUE_DEBUG
    // Ensure data (un)compresses correctly
    {
        struct DynamicMonData dynamicData = {0};
        u16 ability = (compressedData.ability == 0) ? ABILITY_NONE : sDynamicCustomMonAbilities[compressedData.ability - 1];
        u16 moves[MAX_MON_MOVES] = 
        {
            (compressedData.move1 == 0) ? MOVE_NONE : sDynamicCustomMonMoves[compressedData.move1 - 1],
            (compressedData.move2 == 0) ? MOVE_NONE : sDynamicCustomMonMoves[compressedData.move2 - 1],
            (compressedData.move3 == 0) ? MOVE_NONE : sDynamicCustomMonMoves[compressedData.move3 - 1],
            MOVE_NONE
            //(compressedData.move4 == 0) ? MOVE_NONE : sDynamicCustomMonMoves[compressedData.move4 - 1],
        };

        UncompressDynamicMonData(temp, &dynamicData);

        AGB_ASSERT(moves[0] == dynamicData.moves[0]);
        AGB_ASSERT(moves[1] == dynamicData.moves[1]);
        AGB_ASSERT(moves[2] == dynamicData.moves[2]);
        AGB_ASSERT(moves[3] == dynamicData.moves[3]);

        AGB_ASSERT(ability == dynamicData.ability);
    }

#endif
    return temp;
}

static bool8 IsDynamicUniqueMonValid(struct UniqueMon* mon)
{
    return !(mon->species == SPECIES_NONE || mon->customMonId == 0 || mon->countDown == 0);
}

static u8 RandomRarity()
{
    u8 rarity;

    switch (Random() % 7)
    {
    case 0:
        rarity = UNIQUE_RARITY_EXOTIC;
        rarity = UNIQUE_RARITY_EPIC;
        break;

    case 1:
    case 2:
        rarity = UNIQUE_RARITY_EPIC;
        break;

    case 3:
    case 4:
        rarity = UNIQUE_RARITY_RARE;
        rarity = UNIQUE_RARITY_EPIC;
        break;
    
    case 5:
    case 6:
        rarity = UNIQUE_RARITY_COMMON;
        rarity = UNIQUE_RARITY_EPIC;
        break;

    default:
        AGB_ASSERT(FALSE);
        rarity = UNIQUE_RARITY_COMMON;
        rarity = UNIQUE_RARITY_EPIC;
        break;
    }

    if(rarity == UNIQUE_RARITY_EXOTIC && !RogueHub_HasUpgrade(HUB_UPGRADE_LAB_UNIQUE_MON_RARITY_EXOTIC))
        rarity = UNIQUE_RARITY_EPIC;

    if(rarity == UNIQUE_RARITY_EPIC && !RogueHub_HasUpgrade(HUB_UPGRADE_LAB_UNIQUE_MON_RARITY_EPIC))
        rarity = UNIQUE_RARITY_RARE;

    if(rarity == UNIQUE_RARITY_RARE && !RogueHub_HasUpgrade(HUB_UPGRADE_LAB_UNIQUE_MON_RARITY_RARE))
        rarity = UNIQUE_RARITY_COMMON;

    return rarity;
}

static bool8 IsSlotUnlocked(u8 slot)
{
    if(RogueHub_HasUpgrade(HUB_UPGRADE_LAB_UNIQUE_MON_LAB))
    {
        // 3 slots unlocked by default
        if(slot <= 2)
            return TRUE;

        if(slot == 3)
            return RogueHub_HasUpgrade(HUB_UPGRADE_LAB_UNIQUE_MON_SLOTS0);

        if(slot == 4)
            return RogueHub_HasUpgrade(HUB_UPGRADE_LAB_UNIQUE_MON_SLOTS1);
    }

    return FALSE;
}

static bool8 IsCustomMonInUse(u32 customMonId)
{
    u32 i;

    for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
    {
        if(IsDynamicUniqueMonValid(&gRogueSaveBlock->dynamicUniquePokemon[i]) && gRogueSaveBlock->dynamicUniquePokemon[i].customMonId == customMonId)
            return TRUE;
    }

    return FALSE;
}

static u32 SelectUnusedUnlockedExoticMon()
{
    struct RogueQuestReward const* reward;
    u32 currentList[CUSTOM_MON_COUNT];
    u32 listSize = 0;
    u32 questId, j;
    u32 questCount;

    for(j = 0; j < CUSTOM_MON_COUNT; ++j)
    {
        if(sCustomPokemon[j].isDefaultSpawn)
            currentList[listSize++] = j;
    }

    // Populate with exotic mons we have already unlocked
    for(questId = 0; questId < QUEST_ID_COUNT; ++questId)
    {
        if(RogueQuest_HasCollectedRewards(questId))
        {
            questCount = RogueQuest_GetRewardCount(questId);

            for(j = 0; j < questCount; ++j)
            {
                reward = RogueQuest_GetReward(questId, j);

                if(reward->type == QUEST_REWARD_POKEMON && reward->perType.pokemon.customMonId != 0)
                {
                    if(!IsCustomMonInUse(reward->perType.pokemon.customMonId))
                        currentList[listSize++] = reward->perType.pokemon.customMonId;
                }
            }
        }
    }

    // Pick from random options
    if(listSize != 0)
        return currentList[Random() % listSize];

    return 0;
}

void RogueGift_EnsureDynamicCustomMonsAreValid()
{
    u8 i, write;
    u16 newSpecies[DYNAMIC_UNIQUE_MON_COUNT];
    u8 dexVariant = RoguePokedex_GetDexVariant();

    // We use query below, so grab some new unique species now
    {
        // Override dex variant to our max nat dex
        u8 dexVariantToRestore = RoguePokedex_GetDexVariant();
        RoguePokedex_SetDexVariant(POKEDEX_VARIANT_DEFAULT);

        RogueMonQuery_Begin();
        RogueMonQuery_IsSpeciesActive();
        RogueMonQuery_TransformIntoEggSpecies();
        //RogueMonQuery_IsLegendary(QUERY_FUNC_EXCLUDE);
        
        for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
        {
            if(IsDynamicUniqueMonValid(&gRogueSaveBlock->dynamicUniquePokemon[i]))
            {
                RogueMiscQuery_EditElement(QUERY_FUNC_EXCLUDE, gRogueSaveBlock->dynamicUniquePokemon[i].species);
            }
        }

        // Remove random entries until we can safely calcualte weights without going over
        while(RogueWeightQuery_IsOverSafeCapacity())
        {
            RogueMiscQuery_FilterByChance(Random(), QUERY_FUNC_INCLUDE, 50, DYNAMIC_UNIQUE_MON_COUNT);
        }

        RogueWeightQuery_Begin();
        RogueWeightQuery_FillWeights(1);

        for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
        {
            newSpecies[i] = RogueWeightQuery_SelectRandomFromWeightsWithUpdate(Random(), 0);
        //newSpecies[i] = SPECIES_MELOETTA;
        }

        RogueWeightQuery_End();

        RogueMonQuery_End();

        RoguePokedex_SetDexVariant(dexVariantToRestore);
    }

    // Shift all the empty slots up 
    write = 0;
    for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
    {
        if(IsDynamicUniqueMonValid(&gRogueSaveBlock->dynamicUniquePokemon[i]))
        {
            if(write != i)
            {
                memcpy(&gRogueSaveBlock->dynamicUniquePokemon[write], &gRogueSaveBlock->dynamicUniquePokemon[i], sizeof(struct UniqueMon));
                memset(&gRogueSaveBlock->dynamicUniquePokemon[i], 0, sizeof(struct UniqueMon));
            }

            write++;
        }
        else
        {
            memset(&gRogueSaveBlock->dynamicUniquePokemon[i], 0, sizeof(struct UniqueMon));
        }
    }

    // Replace invalid mons
    for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
    {
        // We don't want to populate slots that aren't active
        if(!IsSlotUnlocked(i))
            continue;

        if(!IsDynamicUniqueMonValid(&gRogueSaveBlock->dynamicUniquePokemon[i]))
        {
            u8 rarity = RandomRarity();

            gRogueSaveBlock->dynamicUniquePokemon[i].countDown = 60 + 30 * i; // Time remaining is based on the slot

            if(rarity == UNIQUE_RARITY_EXOTIC)
            {
                u32 customMonId = SelectUnusedUnlockedExoticMon();

                if(customMonId != 0)
                {
                    struct CustomMonData const* monData = &sCustomPokemon[customMonId];
                    AGB_ASSERT(customMonId < CUSTOM_MON_COUNT);

                    gRogueSaveBlock->dynamicUniquePokemon[i].species = Rogue_GetEggSpecies(monData->species);
                    gRogueSaveBlock->dynamicUniquePokemon[i].customMonId = customMonId;
                    continue;
                }

                // Fallback to just have an epic mon in this slot
                rarity = UNIQUE_RARITY_EPIC;
            }

            gRogueSaveBlock->dynamicUniquePokemon[i].species = newSpecies[i];
            gRogueSaveBlock->dynamicUniquePokemon[i].customMonId = RogueGift_CreateDynamicMonId(rarity, newSpecies[i]);
        }
    }

}

void RogueGift_ClearDynamicCustomMons()
{
    memset(&gRogueSaveBlock->dynamicUniquePokemon[0], 0, sizeof(struct UniqueMon) * DYNAMIC_UNIQUE_MON_COUNT);
}

struct UniqueMon* RogueGift_GetDynamicUniqueMon(u8 slot)
{
    AGB_ASSERT(slot < DYNAMIC_UNIQUE_MON_COUNT);
    return &gRogueSaveBlock->dynamicUniquePokemon[slot];
}

void RogueGift_CountDownDynamicCustomMons()
{
    u8 i;

    for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
    {
        if(gRogueSaveBlock->dynamicUniquePokemon[i].countDown != 0)
            --gRogueSaveBlock->dynamicUniquePokemon[i].countDown;
    }
}

void RogueGift_RemoveDynamicCustomMon(u32 customMonId)
{
    u8 i;

    for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
    {
        if(gRogueSaveBlock->dynamicUniquePokemon[i].customMonId == customMonId)
        {
            memset(&gRogueSaveBlock->dynamicUniquePokemon[i], 0, sizeof(struct UniqueMon));
        }
    }
}

bool8 RogueGift_IsDynamicMonSlotEnabled(u8 slot)
{
    AGB_ASSERT(slot < DYNAMIC_UNIQUE_MON_COUNT);

    if(!IsSlotUnlocked(slot))
        return FALSE;

    return IsDynamicUniqueMonValid(&gRogueSaveBlock->dynamicUniquePokemon[slot]);
}

u32 RogueGift_TryFindEnabledDynamicCustomMonForSpecies(u16 species)
{
    u8 i;

    species = Rogue_GetEggSpecies(species);

    for(i = 0; i < DYNAMIC_UNIQUE_MON_COUNT; ++i)
    {
        if(RogueGift_IsDynamicMonSlotEnabled(i))
        {
            if(gRogueSaveBlock->dynamicUniquePokemon[i].species == species)
                return gRogueSaveBlock->dynamicUniquePokemon[i].customMonId;
        }
    }

    return 0;
}