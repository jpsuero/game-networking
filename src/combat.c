#include "gf2d_draw.h"
#include "gf2d_windows.h"
#include "simple_logger.h"
#include "skills.h"

typedef enum
{
    CS_Idle = 0,        // able to perform any action from this state
    CS_Retreating,      // only able to perform a retreating action from this state Transition to far
    CS_Advancing,       // only able to perform an advancing action from this state Transition to close
    CS_Pain,            // cannot act, will automatically return to recover
    CS_Stun,            // cannot act, different animation from Pain, will automatically return to pain
    CS_Windup,          // begining of an action.  Committed and vulnerable in this state
    CS_Action,          // active frames of an action  returns to recover from here
    CS_Recover,         // Vulnerable, but automatically returns to Idle
    CS_DodgingLeft,     // Avoids linear attacks and swinging attacks that go right
    CS_DodgingRight,    // Avoids linear attacks and swinging attacks that go left
    CS_Parried,         // vulnerable state after an attack was parried
    CS_Dying            // triggers death animation
}CombatState;

typedef enum
{
    CA_LeftWeak = 0,
    CA_LeftStrong,
    CA_DodgingLeft,
    CA_RetreatingLeft,
    CA_AdvancingLeft,
    CA_RightWeak,
    CA_RightStrong,
    CA_DodgingRight,
    CA_RetreatingRight,
    CA_AdvancingRight,
    CA_MAX
}CombatActions;

typedef struct
{
    CombatState state;
    Uint32      cooldown;           /**<how long before the player can act again*/
    float       health,healthMax;
    float       mana,manaMax;
    float       stamina,staminaMax;
    Skill     * actions[CA_MAX];
    Color       color;
    Window      *statusMenu;
    Skill     * currentAction;
}Combatant;

typedef struct
{
    Window *combatMenu;
    Uint8 position;     /**<close or far*/
    Combatant combatant[2];
}Combat;

static Combat combat_info = {0};


void combat_draw_combatant(Combatant *com,Vector2D position)
{
    SDL_Rect rect = {0,0,128,256};
    if (!com)return;
    rect.x += position.x;
    rect.y += position.y;
    gf2d_draw_solid_rect(rect,gf2d_color_to_vector4(com->color));
}

void combatant_update(Combatant *com)
{
    if (!com)return;
    if (com->cooldown > 0) com->cooldown-=0.1;
}

void combat_update()
{
    combatant_update(&combat_info.combatant[0]);
    combatant_update(&combat_info.combatant[1]);
}

int combat_menu_update(Window *win,List *updateList)
{
    Combatant *player;
    int i,count;
    Element *e;
    if (!win)return 0;
    if (!updateList)return 0;
    player = &combat_info.combatant[0];
    combat_update();
    if (player->cooldown > 0) return 0;
    count = gf2d_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = gf2d_list_get_nth(updateList,i);
        if (!e)continue;
        switch(e->index)
        {
            case 1001:
                slog("Left Weak");
                if (player->actions[CA_LeftWeak] != NULL)
                {
                    slog("action: %s",player->actions[CA_LeftWeak]->name);
                    player->currentAction = player->actions[CA_LeftWeak];
                    player->cooldown = player->currentAction->windup;
                    player->state = CS_Windup;
                }
                break;
            case 1002:
                slog("Left Dodge");
                break;
            case 1003:
                slog("Left Strong");
                if (player->actions[CA_LeftStrong] != NULL)
                {
                    slog("action: %s",player->actions[CA_LeftStrong]->name);
                    player->currentAction = player->actions[CA_LeftStrong];
                    player->cooldown = player->currentAction->windup;
                    player->state = CS_Windup;
                }
                break;
            case 2001:
                slog("Advance");
                break;
            case 2003:
                slog("Retreat");
                break;
            case 3001:
                slog("Right Weak");
                if (player->actions[CA_RightWeak] != NULL)
                {
                    slog("action: %s",player->actions[CA_RightWeak]->name);
                    player->currentAction = player->actions[CA_RightWeak];
                    player->cooldown = player->currentAction->windup;
                    player->state = CS_Windup;
                }
                break;
            case 3002:
                slog("Right Dodge");
                break;
            case 3003:
                slog("Right Strong");
                if (player->actions[CA_RightStrong] != NULL)
                {
                    slog("action: %s",player->actions[CA_RightStrong]->name);
                    player->currentAction = player->actions[CA_RightStrong];
                    player->cooldown = player->currentAction->windup;
                    player->state = CS_Windup;
                }
                break;
        }
    }
    return 0;
}

int combat_menu_draw(Window *win)
{
    combat_draw_combatant(&combat_info.combatant[0],vector2d(400,280));
    combat_draw_combatant(&combat_info.combatant[1],vector2d(600,80));
    return 0;
}

Window * combat_menu()
{
    Window *win;
    win = gf2d_window_load_from_file("config/combatoptions.cfg");
    if (win)
    {
        win->update = combat_menu_update;
        win->draw = combat_menu_draw;
    }
    return win;
}

int status_menu_draw(Window *win)
{
    Combatant *com;
    SDL_Rect rect = {0,0,220,24};
    SDL_Rect level = {0,0,220,24};
    if (!win)return 0;
    com = (Combatant*)win->data;
    if (!com)return 0;
    // health
    level.x = rect.x = win->dimensions.x + 5;
    level.y = rect.y = win->dimensions.y + 25;
    gf2d_draw_solid_rect(rect, vector4d(128,0,0,255));
    level.w = rect.w * (com->health / com->healthMax);
    gf2d_draw_solid_rect(level, vector4d(255,0,0,255));    
    gf2d_draw_rect(rect, vector4d(185,121,73,255));
    
    // stamina
    rect.y += 25;
    level.y += 25;
    gf2d_draw_solid_rect(rect, vector4d(0,128,0,255));
    level.w = rect.w * (com->stamina / com->staminaMax);
    gf2d_draw_solid_rect(level, vector4d(0,255,0,255));
    gf2d_draw_rect(rect, vector4d(185,121,73,255));

    // mana
    rect.y += 25;
    level.y += 25;
    gf2d_draw_solid_rect(rect, vector4d(0,0,128,255));
    level.w = rect.w * (com->mana / com->manaMax);
    gf2d_draw_solid_rect(level, vector4d(0,0,255,255));
    gf2d_draw_rect(rect, vector4d(185,121,73,255));
    
    //cooldown
    level.x = rect.x = win->dimensions.x + 227;
    level.y = rect.y = win->dimensions.y + 25;
    level.w = rect.w = 20;
    rect.h = 75;
    gf2d_draw_solid_rect(rect, vector4d(100,0,100,255));
    level.h = rect.h * (MIN(100,com->cooldown * 10)/rect.h);
    level.y = rect.y + rect.h - level.h;
    gf2d_draw_solid_rect(level, vector4d(200,0,200,255));
    gf2d_draw_rect(rect, vector4d(185,121,73,255));
    
    return 0;
}

Window * statusMenu(Vector2D offset,Combatant *com)
{
    Window *win;
    win = gf2d_window_load_from_file("config/statusmenu.cfg");
    if (win)
    {
        win->draw = status_menu_draw;
        win->data = (void*)com;
        gf2d_window_set_position(win,offset);
    }
    return win;
}

void combatant_setup_skills(Combatant *com)
{
    if (!com)return;
    com->actions[CA_LeftWeak] = skill_get_by_name("shield_block");
    com->actions[CA_LeftStrong] = skill_get_by_name("shield_parry");
    com->actions[CA_DodgingLeft] = skill_get_by_name("shield_deflect");
    com->actions[CA_RetreatingLeft] = skill_get_by_name("shield_deflect");
    com->actions[CA_AdvancingLeft] = NULL;
    com->actions[CA_RightWeak] = skill_get_by_name("dagger_thrust");
    com->actions[CA_RightStrong] = skill_get_by_name("dagger_slash");
    com->actions[CA_DodgingRight] = skill_get_by_name("dagger_strike");
    com->actions[CA_RetreatingRight] = skill_get_by_name("dagger_parry");
    com->actions[CA_AdvancingRight] = skill_get_by_name("dagger_lunge");
}

void combat_init()
{
    Element *e;
    int i;
    combat_info.combatMenu = combat_menu();
    combat_info.combatant[0].color = gf2d_color8(0,0,255,255);
    combat_info.combatant[0].statusMenu = statusMenu(vector2d(20,20),&combat_info.combatant[0]);
    combatant_setup_skills(&combat_info.combatant[0]);
    e = gf2d_window_get_element_by_name(combat_info.combatant[0].statusMenu,"title");
    if (e)
    {
        gf2d_element_label_set_text(e,"Human Player");
    }
    
    
    combat_info.combatant[1].color = gf2d_color8(255,0,0,255);
    combat_info.combatant[1].statusMenu = statusMenu(vector2d(930,20),&combat_info.combatant[1]);
    e = gf2d_window_get_element_by_name(combat_info.combatant[1].statusMenu,"title");
    if (e)
    {
        gf2d_element_label_set_text(e,"NPC Player");
    }

    for (i = 0;i < 2;i++)
    {
        combat_info.combatant[i].health = combat_info.combatant[i].healthMax = 100;
        combat_info.combatant[i].stamina = combat_info.combatant[i].staminaMax = 100;
        combat_info.combatant[i].mana = combat_info.combatant[i].manaMax = 100;
        combat_info.combatant[i].cooldown = 0;
    }
}

/*eol@eof*/