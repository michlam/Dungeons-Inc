#include "combat_system.hpp"
#include "level/level_system.hpp"
#include "animation_system.hpp"
#include "dialogue/dialogue_system.hpp"
#include "render_system.hpp"



float combat_time = COMBAT_TIME;
int acc = 0;

// Declaration of combat system. DON'T DECLARE IT ANYWHERE ELSE!
CombatSystem combat_system;
AnimationSystem animation_system;


// flag character as in combat
void CombatSystem::init_combat(Entity character) {
    registry.inCombat.emplace(character);
    //this->audio_player = Audio_system::get_audio();
}

// TODO: Update the damage calculator to account for debuffs when that is created
void CombatSystem::calculate_damage(Entity adventurer, Entity mob, Entity room) {
    //printf("a \n");
    Character& advChar = registry.characters.get(adventurer);
    Character& mobChar = registry.characters.get(mob);
    Stats& adventurer_stats = advChar.stats;
    Stats& mob_stats = mobChar.stats;
    int finalHpEffect = 0;
    int finalAtkEffect = 0;
    int finalDefEffect = 0;

    if (registry.cards_on_room.has(room)) {
        auto& roomcards = registry.cards_on_room.get(room);
        auto& cards = roomcards.cards;
        for (Entity card : cards) {
            Card c = registry.cards.get(card);
            CardEffect& cardEffect = c.effect;
            if (cardEffect.is_buff) { // for now we assume buffs and debuffs only affect adventurers
                finalHpEffect += cardEffect.stat_effects.hp;
                finalAtkEffect += cardEffect.stat_effects.atk;
                finalDefEffect += cardEffect.stat_effects.def;
            }
            else { // debuff
                finalHpEffect -= cardEffect.stat_effects.hp;
                finalAtkEffect -= cardEffect.stat_effects.atk;
                finalDefEffect -= cardEffect.stat_effects.def;
            }
        }
    }

    // accumulate all the buffs and debuffs into one big one later for supporting multiple cards in a room
    int mobDamage = 0;
    int advDamage = 0;

    printf("Final: %i %i %i \n", finalHpEffect, finalAtkEffect, finalDefEffect);

    printf("The card damaged adventurer for %i! \n", finalHpEffect);
    adventurer_stats.hp += finalHpEffect;

    mobDamage = (int)trunc(mob_stats.atk * 100 / (100 + powf(2*adventurer_stats.def, 1.1)));
    advDamage = (int)trunc((adventurer_stats.atk + finalAtkEffect) * 100 / (100 + powf(2*mob_stats.def, 1.1))); // debuff


    // add random increment to damage
    mobDamage += rand() % 3;
    advDamage += rand() % 3;




    adventurer_stats.hp -= mobDamage;
    mob_stats.hp -= advDamage;

    if (adventurer_stats.hp < 0) {
        adventurer_stats.hp = 0;
    }
    else if (mob_stats.hp < 0) {
        mob_stats.hp = 0;
    }

    //printf("adventurer deals %i raw damage\n", max(0, (adventurer_stats.atk - cardEffect.stat_effects.atk)));
    printf("%d deals %i damage\n", registry.adventurerClass.get(adventurer).advClass, advDamage);
    printf("mob is at %i hp\n", mob_stats.hp);

    printf("mob deals %i damage\n", mobDamage);
    printf("adventurer is at %i hp\n", adventurer_stats.hp);

    CombatSystem::create_damage_number(registry.motions.get(adventurer).position, mobDamage);
    CombatSystem::create_damage_number(registry.motions.get(mob).position, advDamage);


    // update health bars


    Entity adv_hb = advChar.health_bar;
    Entity mob_hb = mobChar.health_bar;


    HealthBar& adv_health_bar = registry.healthBars.get(adv_hb);
    HealthBar& mob_health_bar = registry.healthBars.get(mob_hb);

    adv_health_bar.health = max(0.f, ceil((float)adventurer_stats.hp / (float)adv_health_bar.max_health * 6));
    mob_health_bar.health = max(0.f, ceil((float)mob_stats.hp / (float)mob_health_bar.max_health * 6));

}


void CombatSystem::process_combat(float elapsed_ms_since_last_update) {


    //printf("in combat!");
    // Define a constant for the time interval at which you want to call calculate_damage
    const float damage_interval = 500.0; // 0.5 seconds in milliseconds
    // Update a timer or accumulator
    static float damage_timer = 0.0;

    // Add the elapsed time to the timer
    damage_timer += elapsed_ms_since_last_update;

    std::vector<Entity> mobArray;
    std::vector<Entity> advArray;


    // Check if it's time to call calculate_damage
    if (damage_timer >= damage_interval) {
        auto currRoom = registry.currentRoom.entities[0];
        auto& combat_container = registry.inCombat;
        if (combat_container.size() <= 1) {
            printf("battle has ended!\n");
            damage_timer = 0.0;
            acc = 0;
            return;

        } else {
            acc++;
            printf("acc %i:", acc);
            // MAKE IT HANDLE ARBITRARY AMOUNTS OF LEVELS IN THE FUTURE
            // 
            //level_type selectedLevelName = level_type::LEVEL_TUTORIAL;
            //auto& levels = registry.levels.components;
            //Level selectedLevel;
            //for (const auto& level : levels) {
            //    if (level == selectedLevelName) {
            //        selectedLevel = level;
            //        break;  // Exit the loop when a match is found
            //    }
            //}

            // loop through combat container array, check for if entity has mob flag

            for (int i = 0; i < combat_container.size(); i++) {
                if (registry.mob.has(combat_container.entities[i])) {
                    mobArray.push_back(combat_container.entities[i]);
                }
                else {
                    advArray.push_back(combat_container.entities[i]);

                }
            }

            // for every mob in combat, randomly select an adventurer to initiate combat with

            std::srand(static_cast<unsigned int>(std::time(nullptr))); // Use time as a seed

            // Generate a random index
            for (Entity mob : mobArray) {
                size_t randomIndex = std::rand() % advArray.size();
                Entity adventurer = advArray.at(randomIndex);
                if (adventurer != mob) {
                    calculate_damage(adventurer, mob, currRoom);


                    Motion& motion1 = registry.motions.get(adventurer);
                    Motion& motion2 = registry.motions.get(mob);                  

                    Entity& weapon1 = registry.characters.get(adventurer).weapon;
                    Motion& weapon_motion1 = registry.motions.get(weapon1);

                    Entity& weapon2 = registry.characters.get(mob).weapon;
                    Motion& weapon_motion2 = registry.motions.get(weapon2);


                    
                    //animate blood 
                    float combat_angle = atan2(motion2.position[1] - motion1.position[1], motion2.position[0] - motion1.position[0]);
                   

                    
                    // face target
                    if (motion1.position.x > motion2.position.x) { //adventurer right of mob
                        if (motion1.scale.x > 0) {
                            motion1.scale.x = -motion1.scale.x;
                            weapon_motion1.angle = - M_PI / 8;

                  
                        }
                        if (motion2.scale.x < 0) {
                            motion2.scale.x = -motion2.scale.x;
                            weapon_motion2.angle = M_PI / 6;
                        }
                    }
                    else { //adventurer left of mob
                        if (motion1.scale.x < 0) {
                            motion1.scale.x = -motion1.scale.x;
                            weapon_motion1.angle = M_PI / 8;

                        }
                        if (motion2.scale.x > 0) {
                            motion2.scale.x = -motion2.scale.x;
                            weapon_motion2.angle = -M_PI / 6;

                        }
                    }

                    //reposition weapons with target changes           
                    
                    //adventurer
                    if (registry.adventurerClass.get(adventurer).advClass == Class::RANGER) {
                        weapon_motion1.scale.x = motion1.scale.x;
                    }
                    else {
                        weapon_motion1.scale.x = motion1.scale.x / 3 * 2;

                    }
                    //weapon_motion1.scale.x = motion1.scale.x / 3 * 2;
                    weapon_motion1.position.x = motion1.position.x + motion1.scale.x / 3;
                    
                    //mob
                    weapon_motion2.scale.x = motion2.scale.x / 3 * 2;
                    weapon_motion2.scale.y = motion2.scale.y * 1.5;

                    weapon_motion2.position.x = motion2.position.x + motion2.scale.x / 3;

                    
                    

                    
                    // cast attack animations      
                    
                     /*
                    if (!registry.stabbing.has(weapon2)) {
                        Stabbing& stabbing = registry.stabbing.emplace(weapon2);
                        stabbing.target_angle = M_PI * 3 / 2 + combat_angle;
                    }         
                    */
                    switch (registry.mobClass.get(mob).mobClass) {
                    case MobClass::BARBARIAN:
                        if (!registry.stabbing.has(weapon2)) {
                            Stabbing& stabbing = registry.stabbing.emplace(weapon2);
                            stabbing.target_angle = M_PI * 3 / 2 + combat_angle;
                        }
                        break;
                    case MobClass::SHAMAN:
                        if (!registry.casting.has(weapon2)) {
                            Casting& casting = registry.casting.emplace(weapon2);
                            casting.target_angle = M_PI * 3 / 2 + combat_angle;

                        }

                        break;
                    case MobClass::BRUTE:
                        if (!registry.swinging.has(weapon2)) {
                            Swinging& swinging = registry.swinging.emplace(weapon2);
                            swinging.target_angle = M_PI * 3 / 2 + combat_angle;

                        }
                        break;
                    default:
                        break;
                    }

                   
                    
                    switch (registry.adventurerClass.get(adventurer).advClass) {
                        case Class::WARRIOR:
                            if (!registry.swinging.has(weapon1)) {
                                Swinging& swinging = registry.swinging.emplace(weapon1);
                                swinging.target_angle = M_PI * 3 / 2 + combat_angle + M_PI;

                            }
                            break;
                        case Class::MAGE:
                            if (!registry.casting.has(weapon1)) {
                                Casting& casting = registry.casting.emplace(weapon1);
                                casting.target_angle = M_PI * 3 / 2 + combat_angle + M_PI;
                            }
                            break;
                        case Class::RANGER:
                            if (!registry.shooting.has(weapon1)) {
                                Shooting& shooting = registry.shooting.emplace(weapon1);
                                shooting.target_angle = M_PI * 3 / 2 + combat_angle + M_PI;
                                shooting.original_pos = weapon_motion1.position;
                            }
                            break;
                        default:
                            break;
                    }
                    
                   

                    // Check if adventurer or mob's HP has reached 0
                    Stats& adventurer_stats = registry.characters.get(adventurer).stats;
                    Stats& mob_stats = registry.characters.get(mob).stats;

                    if (adventurer_stats.hp <= 0 || mob_stats.hp <= 0) {
                        if (adventurer_stats.hp <= 0) {

                            registry.fading.emplace(weapon1);
                            FadingWeapon& fadingWeapon = registry.fadingWeapons.emplace(weapon1);
                            fadingWeapon.angle = combat_angle + M_PI;

                            registry.remove_all_components_of(registry.characters.get(adventurer).stat_box);

                            //set healthbar to 0 and fade
                            Entity adv_hb = registry.characters.get(adventurer).health_bar;
                            //registry.healthBars.get(adv_hb).health = 0;
                            registry.fading.emplace(adv_hb);

                            //remove from containers and set up death anim
                            combat_container.remove(adventurer);
                            registry.characters.remove(adventurer);
                            registry.fading.emplace(adventurer);


                            printf("Adventurer has died! \n");
                            //this->audio_player->play_adv_hurt();
                        }
                        if (mob_stats.hp <= 0) {
                            registry.fading.emplace(weapon2);
                            FadingWeapon& fadingWeapon = registry.fadingWeapons.emplace(weapon2);
                            fadingWeapon.angle = combat_angle;

                            registry.remove_all_components_of(registry.characters.get(mob).stat_box);

                            //set healthbar to 0 and fade
                            Entity mob_hb = registry.characters.get(mob).health_bar;
                            //registry.healthBars.get(mob_hb).health = 0;
                            registry.fading.emplace(mob_hb);

                            combat_container.remove(mob);
                            registry.characters.remove(mob);
                            registry.mob.remove(mob);

                            registry.fading.emplace(mob);



                            printf("Mob has been slain! \n");
                            //this->audio_player->play_monster_hurt();
                        }

                        // return; // Exit combat
                    }

                    animation_system.animate_blood(mob, combat_angle);
                    animation_system.animate_blood(adventurer, combat_angle + M_PI);

                }
            }
            //// Check if combat has exceeded COMBAT_TIME
            if (acc >= (COMBAT_TIME * 2) - 1) {
                for (int i = 0; i < mobArray.size(); i++) {
                    Character& mobChar = registry.characters.get(mobArray[i]);
                    Stats& mobStats = mobChar.stats;
                    mobStats.hp = 0;

                    Motion& motion2 = registry.motions.get(mobArray[i]);

                    Entity& weapon2 = registry.characters.get(mobArray[i]).weapon;
                    Motion& weapon_motion2 = registry.motions.get(weapon2);

                    registry.fading.emplace(weapon2);
                    FadingWeapon& fadingWeapon = registry.fadingWeapons.emplace(weapon2);

                    registry.remove_all_components_of(registry.characters.get(mobArray[i]).stat_box);

            //        //set healthbar to 0 and fade
                    Entity mob_hb = registry.characters.get(mobArray[i]).health_bar;
                    registry.healthBars.get(mob_hb).health = 0;
                    registry.fading.emplace(mob_hb);

                    combat_container.remove(mobArray[i]);
                    registry.characters.remove(mobArray[i]);
                    registry.mob.remove(mobArray[i]);

                    registry.fading.emplace(mobArray[i]);
                }
                acc = 0;
                //combat_container.clear();
            }
            //if (damage_timer / 1000.f >= COMBAT_TIME) { // not even called wtf
            //    combat_container.clear();
            //}

            //Entity adventurer = combat_container.entities[0];
            //Entity mob = combat_container.entities[1];
            damage_timer = 0;
        }


    }
}


Entity CombatSystem::create_damage_number(vec2 pos, float num) {
    Entity entity = Entity();

    Motion& motion = registry.motions.emplace(entity);
    motion.position = {pos.x, pos.y};
    motion.scale = { 50, 50 }; 

    
    Text text = Text{};
    text.colour = { 255, 255, 255 };
    //text.colour = { 0, 0, 0 };


    //printf("Float value: %f\n", num);
    //printf("please print something ");


    text.text_lines[0] = std::to_string((int)num);


    //vec2 scaler = vec2(1.f / 3.f);
    //text.position = pos;
    text.scale = 1.f;
    vec2 scaler = vec2(1.f / 3.f);
    text.position = { (2 * scaler.x) * pos.x, (2 * scaler.y) * pos.y - motion.scale.y / 4};
    //text.position.x += (rand() % 20 - 10);
    //text.position.y += (rand() % 10 - 5);


    
    registry.renderRequests.insert(
        entity,
        { TEXTURE_ASSET_ID::DMG_BG,
         EFFECT_ASSET_ID::TEXTURED,
         GEOMETRY_BUFFER_ID::SPRITE,
        "DAMAGE NUMBER"
        });


    registry.texts.insert(entity, text);
    Fading& fading = registry.fading.emplace(entity);
    //fading.max_time = 2.f;
    fading.fade_factor = 0.45f;
    registry.damageNumbers.emplace(entity);

    return entity;

}
