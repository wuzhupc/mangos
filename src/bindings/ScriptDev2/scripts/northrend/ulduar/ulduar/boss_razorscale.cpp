/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: boss_razorscale
SD%Complete:
SDComment: harpoons display should change when clicked
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells/emotes
    SAY_INTRO               = -1603030,
    SAY_AGGRO1              = -1603031,
    SAY_AGGRO2              = -1603032,
    SAY_AGGRO3              = -1603033,
    SAY_GROUND              = -1603034,
    EMOTE_DEEP_BREATH       = -1603035,
    SAY_FIRES_EXTINGUISH    = -1603036,
    EMOTE_HARPOON           = -1603337,
    EMOTE_GROUNDED          = -1603338,

    //razorscale air phase
    SPELL_FIREBALL              = 62796,
    SPELL_FIREBALL_H            = 63815,
    SPELL_WING_BUFFET           = 62666,
    SPELL_STUN                  = 62794,
    SPELL_SUMMON_DWARF          = 62916,
    //both
    SPELL_BERSERK               = 47008,
    DEVOURING_FLAME_MISSILE     = 63236,
    SPELL_FLAME_BREATH          = 63317,
    SPELL_FLAME_BREATH_H        = 64021,
    NPC_CONTROLLER              = 33233,  // used for casting deep breath on turrets
    //ground
    SPELL_FLAME_BUFFET          = 64016,
    SPELL_FLAME_BUFFET_H        = 64023,
    SPELL_FUSE_ARMOR            = 64771,

    //devouring flame target
    NPC_DEVOURING_TARGET        = 34188,
    NPC_DEVOURING_TARGET_H      = 34189,
    AURA_DEVOURING_FLAME        = 64709,
    AURA_DEVOURING_FLAME_H      = 64734,

    // mole machine
    NPC_RAZORSCALE_SPAWNER      = 33245,    // used to summon adds in phase 1
    NPC_HARPOONS_DUMMY          = 33282,    // used to cast spells for harpoons
    SPELL_SUMMON_MOLE_MACHINE   = 73071,

    // harpoons
    SPELL_HARPOON_SHOT          = 63659,
    GO_REPAIR_HARPOON_4         = 194543, // BROKEN HARBOON 194565
    GO_REPAIR_HARPOON_3         = 194542,
    GO_REPAIR_HARPOON_2         = 194541,
    GO_REPAIR_HARPOON_1         = 194519,
    //dark rune watcher
    SPELL_LIGHTNING_BOLT        = 63809,
    SPELL_LIGHTNING_BOLT_H      = 64696,
    SPELL_CHAIN_LIGHTNING       = 64758,
    SPELL_CHAIN_LIGHTNING_H     = 64759,

    //dark rune sentinel
    SPELL_BATTLE_SHOUT          = 46763,
    SPELL_BATTLE_SHOUT_H        = 64062,
    SPELL_WHIRLWIND             = 63808,

    //dark rune guardian
    SPELL_STORMSTRIKE           = 64757,

    //NPC ids
    NPC_DARK_RUNE_WATCHER       = 33453,
    NPC_DARK_RUNE_SENTINEL      = 33846,
    NPC_DARK_RUNE_GUARDIAN      = 33388,
 
    NPC_EXPEDITION_TRAPPER      = 33259,
    NPC_EXPEDITION_DEFENDER     = 33816,
    NPC_EXP_ENGINEER            = 33287,

    FACTION_NEUTER              = 35,
    FACTION_HOSTILE             = 2105
};

//Positional defines 
struct Coordinate
{
    float x, y, z;
};

enum
{
    POSITION_RIGHT,
    POSITION_LEFT,
    POSITION_MIDDLE,
    POSITION_GROUND,
    POSITION_AIR
};

static Coordinate Point[]=
{
    {621.633301f, -228.671371f, 391.180328f},//right
    {564.140198f, -222.049149f, 391.517212f},//left
    {591.629761f, -209.629761f, 392.629761f},//middle
    {587.629761f, -179.022522f, 391.625061f},//ground
    {587.629761f, -179.022522f, 450.415070f} //air
};

#define GOSSIP_START "We are ready to help!"

enum RazorscalePhase
{
    PHASE_AIR           = 1,
    PHASE_GROUND        = 2,
    PHASE_PERMAGROUND   = 3
};

// expedition commander
// start the event
struct MANGOS_DLL_DECL npc_expedition_commanderAI : public ScriptedAI
{
    npc_expedition_commanderAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bHasPlayerNear = false;
        Reset();
    }

    instance_ulduar* m_pInstance;

    bool m_bHasPlayerNear;
    bool m_bIsIntro;
    ObjectGuid m_uiPlayerGUID;
    uint32 m_uiSpeechTimer;
    uint32 m_uiIntroPhase;

    void Reset()
    {
        m_uiPlayerGUID.Clear();
        m_uiSpeechTimer     = 3000;
        m_bIsIntro          = false;
        m_uiIntroPhase      = 0;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasPlayerNear && pWho->GetTypeId() == TYPEID_PLAYER && !((Player*)pWho)->isGameMaster() &&
            m_creature->IsWithinDistInMap(pWho, 40.0) && m_creature->IsWithinLOSInMap(pWho))
        {
            if (Creature* pRazorscale = m_pInstance->GetSingleCreatureFromStorage(NPC_RAZORSCALE))
                DoScriptText(SAY_INTRO, pRazorscale);
            m_bHasPlayerNear = true;
        }
    }

    void GetRazorDown()
    {
        if (Creature* pRazorscale = m_pInstance->GetSingleCreatureFromStorage(NPC_RAZORSCALE))
        {
            pRazorscale->SetInCombatWithZone();
            if (Unit* pPlayer = m_creature->GetMap()->GetUnit(m_uiPlayerGUID))
            {
                pRazorscale->AddThreat(pPlayer, 0.0f);
                pRazorscale->AI()->AttackStart(pPlayer);
            }
        }
    }

    void BeginRazorscaleEvent(Player* pPlayer)
    {
        m_uiPlayerGUID      = pPlayer->GetObjectGuid();
        m_bIsIntro          = true;
        m_uiSpeechTimer     = 3000;
        m_uiIntroPhase      = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bIsIntro)
        {
            if (m_uiSpeechTimer < uiDiff)
            {
                switch(m_uiIntroPhase)
                {
                    case 0:
                        if (Creature* pEngineer = GetClosestCreatureWithEntry(m_creature, NPC_EXP_ENGINEER, 50.0f))
                            DoScriptText(SAY_AGGRO1, pEngineer);
                            GetRazorDown();
                        ++m_uiIntroPhase;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 1:
                    {
                        DoScriptText(SAY_AGGRO2, m_creature);
                        ++m_uiIntroPhase;
                        m_uiSpeechTimer = 7000;
                        std::list<Creature*> lExpedition;
                        GetCreatureListWithEntryInGrid(lExpedition, m_creature, NPC_EXPEDITION_DEFENDER, 50.0f);
                        GetCreatureListWithEntryInGrid(lExpedition, m_creature, NPC_EXPEDITION_TRAPPER, 50.0f);
                        for (std::list<Creature*>::iterator itr = lExpedition.begin(); itr != lExpedition.end(); ++itr)
                        {
                            if ((*itr)->isAlive())
                                (*itr)->SetFactionTemporary(FACTION_HOSTILE, TEMPFACTION_RESTORE_RESPAWN);
                        }
                        break;
                    }
                    case 2:
                        if (Creature* pEngineer = GetClosestCreatureWithEntry(m_creature, NPC_EXP_ENGINEER, 50.0f))
                            DoScriptText(SAY_AGGRO3, pEngineer);
                        ++m_uiIntroPhase;
                        m_uiSpeechTimer = 5000;
                        break;
                    case 3:
                        m_bIsIntro = false;
                        ++m_uiIntroPhase;
                        m_uiSpeechTimer = 10000;
                        break;
                    default:
                        m_uiSpeechTimer = 100000;
                }
            }
            else
                m_uiSpeechTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_npc_expedition_commander(Creature* pCreature)
{
    return new npc_expedition_commanderAI(pCreature);
}

bool GossipHello_npc_expedition_commander(Player* pPlayer, Creature* pCreature)
{
    instance_ulduar* pInstance = (instance_ulduar *) pCreature->GetInstanceData();

    if (pInstance->GetData(TYPE_RAZORSCALE) != DONE)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_START, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_expedition_commander(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF+1)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        ((npc_expedition_commanderAI*)pCreature->AI())->BeginRazorscaleEvent(pPlayer);
    }

    return true;
}

// devouring_flame_target
struct MANGOS_DLL_DECL mob_devouring_flame_targetAI : public ScriptedAI
{
    mob_devouring_flame_targetAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiDeathTimer;

    void Reset()
    {
        m_uiDeathTimer = 25500;
        m_creature->SetDisplayId(11686);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        DoCast(m_creature, m_bIsRegularMode ? AURA_DEVOURING_FLAME : AURA_DEVOURING_FLAME_H);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // think that unnecessary because summon spell 63308 with duration 22 seconds
        if (m_uiDeathTimer < uiDiff)
            m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
        else
            m_uiDeathTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_devouring_flame_target(Creature* pCreature)
{
    return new mob_devouring_flame_targetAI(pCreature);
}

// dark rune watcher
struct MANGOS_DLL_DECL mob_dark_rune_watcherAI : public ScriptedAI
{
    mob_dark_rune_watcherAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiSpellTimer;

    void Reset()
    {
        m_uiSpellTimer = urand(5000, 10000);
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer < diff)
        {
            switch(urand(0, 1))
            {
                case 0:
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LIGHTNING_BOLT : SPELL_LIGHTNING_BOLT_H);
                    break;
                case 1:
                    DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H);
                    break;
            }
            m_uiSpellTimer = urand(5000, 10000);
        }
        else
            m_uiSpellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_dark_rune_watcher(Creature* pCreature)
{
    return new mob_dark_rune_watcherAI(pCreature);
}

// dark rune sentinel
struct MANGOS_DLL_DECL mob_dark_rune_sentinelAI : public ScriptedAI
{
    mob_dark_rune_sentinelAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWhirlTimer;
    uint32 m_uiShoutTimer;

    void Reset()
    {
        m_uiWhirlTimer = 10000;
        m_uiShoutTimer = 2000;
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiWhirlTimer < diff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_WHIRLWIND) == CAST_OK)
                m_uiWhirlTimer = urand(10000, 15000);
        }
        else
            m_uiWhirlTimer -= diff;

        if (m_uiShoutTimer < diff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_BATTLE_SHOUT : SPELL_BATTLE_SHOUT_H) == CAST_OK)
                m_uiShoutTimer = 30000;
        }
        else
            m_uiShoutTimer -= diff;

        DoMeleeAttackIfReady();
    }

};

CreatureAI* GetAI_mob_dark_rune_sentinel(Creature* pCreature)
{
    return new mob_dark_rune_sentinelAI(pCreature);
}

// dark rune guardian
struct MANGOS_DLL_DECL mob_dark_rune_guardianAI : public ScriptedAI
{
    mob_dark_rune_guardianAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStormstrikeTimer;

    void Reset()
    {
        m_uiStormstrikeTimer = 10000;
        m_creature->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStormstrikeTimer < diff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_STORMSTRIKE) == CAST_OK)
                m_uiStormstrikeTimer = urand(7000, 13000);
        }
        else
            m_uiStormstrikeTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_dark_rune_guardian(Creature* pCreature)
{
    return new mob_dark_rune_guardianAI(pCreature);
}

/// mole machine
// used to summon dwarfes
struct MANGOS_DLL_DECL mob_mole_machineAI : public ScriptedAI
{
    mob_mole_machineAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        pCreature->SetDisplayId(11686);     // make invisible
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiSummonTimer;
    bool m_bIsSentinel;

    void Reset()
    {
        m_uiSummonTimer     = 8000;
        m_bIsSentinel       = false;
        DoCast(m_creature, SPELL_SUMMON_MOLE_MACHINE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiSummonTimer < diff)
        {
            // summon 2 dwarfes
            if (!m_bIsSentinel)
            {
                m_creature->SummonCreature(NPC_DARK_RUNE_WATCHER, m_creature->GetPositionX() + 5.0f, m_creature->GetPositionY() + 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
                m_creature->SummonCreature(NPC_DARK_RUNE_GUARDIAN, m_creature->GetPositionX() - 5.0f, m_creature->GetPositionY() - 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);
            }
            // summon 1 sentinel
            else
                m_creature->SummonCreature(NPC_DARK_RUNE_SENTINEL, m_creature->GetPositionX() - 5.0f, m_creature->GetPositionY() - 5, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000);

            m_uiSummonTimer = 60000;
        }
        else
            m_uiSummonTimer -= diff;
    }
};

CreatureAI* GetAI_mob_mole_machine(Creature* pCreature)
{
    return new mob_mole_machineAI(pCreature);
}

//razorscale
struct MANGOS_DLL_DECL boss_razorscaleAI : public ScriptedAI
{
    boss_razorscaleAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFireballTimer;
    uint32 m_uiDevouringFlameTimer;
    uint32 m_uiFlameBuffetTimer;
    uint32 m_uiFuseArmorTimer;
    uint32 m_uiFlameBreathTimer;
    uint32 m_uiWavespawn;
    uint32 m_uiBerserkTimer;
    uint32 m_uiGroundStepTimer;
    uint32 m_uiGroundStepCount;
    uint32 m_uiRepairHarpoonTimer;
    
    uint8 m_uiHarpoonsRepaired;
    uint8 m_uiMaxHarpoons;
    uint8 m_uiHarpoonsUsed;
    uint8 m_uiFlyNo;
    uint8 m_uiScorchedDwarves;

    GuidList m_lRepairHarpoonsGUID;
    GuidList m_lHarpoonsDummyGUID;

    RazorscalePhase m_Phase;

    void Reset()
    {
        m_uiMaxHarpoons = m_bIsRegularMode ? 2 : 4;
        //if (m_pInstance->m_lBrokenHarpoonGUID.size() != m_uiMaxHarpoons)
            //m_creature->MonsterSay("Fehler in der Suche der Harpoonen", LANG_UNIVERSAL);
        BreakHarpoons();

        m_uiFireballTimer           = 10000;    // 10 secs for the first
        m_uiDevouringFlameTimer     = 18000;    // 18 secs first, 12 seconds after
        m_uiWavespawn               = urand(5000, 10000);
        m_uiBerserkTimer            = 600000;   // 10 min
        m_uiRepairHarpoonTimer      = 51000;
        m_uiHarpoonsRepaired        = 0;
        
        m_Phase             = PHASE_AIR;
        m_uiFlyNo           = 0;
        m_uiHarpoonsUsed    = 0;
        m_uiScorchedDwarves = 0;

        m_creature->SetWalk(true);
        SetCombatMovement(false);
        //  make boss fly
        m_creature->SetLevitate(true);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);

        m_creature->GetMotionMaster()->MoveIdle();

        if (Creature* pCommander = m_pInstance->GetSingleCreatureFromStorage(NPC_COMMANDER))
            pCommander->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, DONE);

        BreakHarpoons();


        std::list<Creature*> lExpedition;
        GetCreatureListWithEntryInGrid(lExpedition, m_creature, NPC_EXPEDITION_DEFENDER, 4000.0f);
        GetCreatureListWithEntryInGrid(lExpedition, m_creature, NPC_EXPEDITION_TRAPPER, 4000.0f);
        for (std::list<Creature*>::iterator itr = lExpedition.begin(); itr != lExpedition.end(); ++itr)
        {
            if ((*itr)->isAlive())
                (*itr)->setFaction(FACTION_NEUTER);
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, IN_PROGRESS);
        m_creature->MonsterMoveWithSpeed(Point[POSITION_AIR].x, Point[POSITION_AIR].y, Point[POSITION_AIR].z, 28.0f);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_FLAME_BREATH || pSpell->Id == SPELL_FLAME_BREATH_H)
        {
            if (pTarget->GetEntry() == NPC_DARK_RUNE_GUARDIAN) // only this one
            {
                 ++m_uiScorchedDwarves;
                 if (m_uiScorchedDwarves > 25)
                 {
                     if (m_pInstance)
                         m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_IRON_DWARF_MEDIUM_RARE, true);
                 }
                 m_creature->DealDamage(pTarget, pTarget->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }
    }

    void EnterEvadeMode()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_RAZORSCALE, FAIL);

        m_creature->ForcedDespawn();
        if (m_creature)
            m_creature->Respawn();

        std::list<Creature*> lExpedition;
        GetCreatureListWithEntryInGrid(lExpedition, m_creature, NPC_EXPEDITION_DEFENDER, 4000.0f);
        GetCreatureListWithEntryInGrid(lExpedition, m_creature, NPC_EXPEDITION_TRAPPER, 4000.0f);
        for (std::list<Creature*>::iterator itr = lExpedition.begin(); itr != lExpedition.end(); ++itr)
        {
            if ((*itr)->isAlive())
                (*itr)->setFaction(FACTION_NEUTER);
            else
                (*itr)->Respawn();
        }
    }

    void BreakHarpoons()
    {
        // reset harpoons
        for (GuidVector::iterator iter = m_pInstance->m_lBrokenHarpoonGUID.begin(); iter != m_pInstance->m_lBrokenHarpoonGUID.end(); ++iter)
        {
            if (GameObject* pBreakHarpoon = m_creature->GetMap()->GetGameObject(*iter))
            {
                pBreakHarpoon->SetPhaseMask(1, true);
            }
        }
        for (GuidList::iterator iter = m_lRepairHarpoonsGUID.begin(); iter != m_lRepairHarpoonsGUID.end(); ++iter)
        {
            if (GameObject* pRepairHarpoon = m_creature->GetMap()->GetGameObject(*iter))
            {
                pRepairHarpoon->Delete();
            }
        }
        for (GuidList::iterator iter = m_lHarpoonsDummyGUID.begin(); iter != m_lHarpoonsDummyGUID.end(); ++iter)
        {
            if (Creature* pHarpoonDummy = m_creature->GetMap()->GetCreature(*iter))
            {
                pHarpoonDummy->ForcedDespawn();
            }
        }
        m_lHarpoonsDummyGUID.clear();
        m_lRepairHarpoonsGUID.clear();
    }

    void RepairHarpoons()
    {
        if (GameObject* pHarpoon = m_creature->GetMap()->GetGameObject(m_pInstance->m_lBrokenHarpoonGUID.at(m_uiHarpoonsRepaired)))
        {
            switch(m_uiHarpoonsRepaired)
            {
                case 0:
                {
                    if (GameObject* pRepairHarpoon =  pHarpoon->SummonGameobject(GO_REPAIR_HARPOON_1, pHarpoon->GetPositionX(), pHarpoon->GetPositionY(), pHarpoon->GetPositionZ(), 4.732974f, 0))
                    {
                        m_lRepairHarpoonsGUID.push_back(pRepairHarpoon->GetObjectGuid());
                    }
                    break;
                }
                case 1:
                {
                    if (GameObject* pRepairHarpoon =  pHarpoon->SummonGameobject(GO_REPAIR_HARPOON_2, pHarpoon->GetPositionX(), pHarpoon->GetPositionY(), pHarpoon->GetPositionZ(), 5.269379f, 0))
                    {
                        m_lRepairHarpoonsGUID.push_back(pRepairHarpoon->GetObjectGuid());
                    }
                    break;
                }
                case 2:
                {
                    if (GameObject* pRepairHarpoon =  pHarpoon->SummonGameobject(GO_REPAIR_HARPOON_3, pHarpoon->GetPositionX(), pHarpoon->GetPositionY(), pHarpoon->GetPositionZ(), pHarpoon->GetOrientation(), 0))
                        m_lRepairHarpoonsGUID.push_back(pRepairHarpoon->GetObjectGuid());
                    break;
                }
                case 3:
                {
                    if (GameObject* pRepairHarpoon =  pHarpoon->SummonGameobject(GO_REPAIR_HARPOON_4, pHarpoon->GetPositionX(), pHarpoon->GetPositionY(), pHarpoon->GetPositionZ(), pHarpoon->GetAngle(m_creature), 0))
                        m_lRepairHarpoonsGUID.push_back(pRepairHarpoon->GetObjectGuid());
                    break;
                }
            }
            if (Creature* pHarpoonDummy = pHarpoon->SummonCreature(NPC_HARPOONS_DUMMY, pHarpoon->GetPositionX(), pHarpoon->GetPositionY(), pHarpoon->GetPositionZ(), 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
            {
                m_lHarpoonsDummyGUID.push_back(pHarpoonDummy->GetObjectGuid());
            }
            pHarpoon->SetPhaseMask(128, true);
        }
    }

    void SetToGroundPhase()
    {
        if (Creature* pCommander = m_pInstance->GetSingleCreatureFromStorage(NPC_COMMANDER))
            DoScriptText(SAY_GROUND, pCommander);

        // make boss land
        m_creature->SetLevitate(false);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, 0);

        // ground position
        m_creature->MonsterMoveWithSpeed(Point[POSITION_GROUND].x, Point[POSITION_GROUND].y, Point[POSITION_GROUND].z, 40.0f);

        // timers
        m_uiHarpoonsUsed    = 0;
        m_uiGroundStepTimer = 2000;
        m_uiGroundStepCount = 0;

        m_Phase = PHASE_GROUND;
    }

    void SetToAirPhase()
    {
        if (Creature* pCommander = m_pInstance->GetSingleCreatureFromStorage(NPC_COMMANDER))
            DoScriptText(SAY_FIRES_EXTINGUISH, pCommander);
        //  make boss fly
        m_creature->SetLevitate(true);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_UNK_2);
        m_Phase                     = PHASE_AIR;
        m_uiFireballTimer           = 10000;
        m_uiDevouringFlameTimer     = 18000;
        m_uiWavespawn               = urand(5000, 10000);
        m_uiRepairHarpoonTimer      = 50000;
        m_uiHarpoonsRepaired        = 0;
        
        // achiev counter
        ++m_uiFlyNo;
        if (m_uiFlyNo > 1)
        {
            if (m_pInstance)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_QUICK_SHAVE, false);
        }
    }

    void SetToPermGroundedPhase()
    {
        if (m_creature->HasAura(SPELL_STUN))
            m_creature->RemoveAurasDueToSpell(SPELL_STUN);

        DoScriptText(EMOTE_GROUNDED, m_creature);
        m_Phase                     = PHASE_PERMAGROUND;
        m_uiDevouringFlameTimer     = 12000;
        m_uiFlameBuffetTimer        = 10000; //every 10 secs
        m_uiFuseArmorTimer          = 13000; //every ~13
        m_uiFlameBreathTimer        = 6000;  //every 14
        SetCombatMovement(true);
        BreakHarpoons();
        //  make boss land
        m_creature->SetLevitate(false);
        m_creature->SetByteValue(UNIT_FIELD_BYTES_1, 3, 0);

        if (m_creature->getVictim())
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // make boss land at 50% hp
        if (m_Phase != PHASE_PERMAGROUND && m_creature->GetHealthPercent() < 50.0f)
        {
            SetToPermGroundedPhase();
        }

        switch(m_Phase)
        {
            case PHASE_AIR:
            {
                // Switch from Air to Ground Phase
                if (m_uiHarpoonsUsed == m_uiMaxHarpoons)
                {
                    SetToGroundPhase();
                    return;
                }
                // air position check (sometimes it falls to the ground like a rock
                if (m_creature->GetPositionZ() < 440.0f)
                {
                    m_creature->MonsterMoveWithSpeed(Point[POSITION_AIR].x, Point[POSITION_AIR].y, Point[POSITION_AIR].z, 28.0f);
                }

                // air spells
                if (m_uiFireballTimer < uiDiff)
                {
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
                        DoCast(target, m_bIsRegularMode ? SPELL_FIREBALL : SPELL_FIREBALL_H);
                        m_uiFireballTimer = 2000;
                }
                else
                    m_uiFireballTimer -= uiDiff;

                if (m_uiDevouringFlameTimer < uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, DEVOURING_FLAME_MISSILE) == CAST_OK)
                            m_uiDevouringFlameTimer = 12000;
                    }
                }
                else
                    m_uiDevouringFlameTimer -= uiDiff;

                // repair harpoons
                if (m_uiHarpoonsRepaired < m_uiMaxHarpoons && m_uiHarpoonsRepaired < m_pInstance->m_lBrokenHarpoonGUID.size()) // i know the double check
                {
                    if (m_uiRepairHarpoonTimer < uiDiff)
                    {
                        RepairHarpoons();
                        ++m_uiHarpoonsRepaired;
                        DoScriptText(EMOTE_HARPOON, m_creature);
                        m_uiRepairHarpoonTimer = 20000;
                    }
                    else
                        m_uiRepairHarpoonTimer -= uiDiff;
                }

                break;
            }
            case PHASE_GROUND:
            {
                if (m_uiGroundStepTimer < uiDiff)
                {
                    switch(m_uiGroundStepCount)
                    {
                        case 0:
                            DoCast(m_creature, SPELL_STUN, true);
                            m_uiGroundStepTimer = 33000;
                            break;
                        case 1:
                            if (Creature* pCommander = m_pInstance->GetSingleCreatureFromStorage(NPC_COMMANDER))
                                m_creature->SetFacingToObject(pCommander);
                            m_creature->RemoveAurasDueToSpell(SPELL_STUN);
                            DoScriptText(EMOTE_DEEP_BREATH, m_creature);
                            DoCast(m_creature, m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H);
                            m_uiGroundStepTimer = 7000;
                            break;
                        case 2:
                            DoCast(m_creature, SPELL_WING_BUFFET);
                            BreakHarpoons();
                            m_uiGroundStepTimer = 4000;
                            break;
                        case 3:
                            SetToAirPhase();
                            m_uiGroundStepTimer = 4000;
                            break;
                        default: // debug next action Fly 
                            m_uiGroundStepCount = 2;
                            m_creature->RemoveAurasDueToSpell(SPELL_STUN);
                            m_uiGroundStepTimer = 4000;
                            break;
                    }
                    ++m_uiGroundStepCount;
                }
                else
                    m_uiGroundStepTimer -= uiDiff;

                break;
            }
            case PHASE_PERMAGROUND:
            {
                if (m_uiDevouringFlameTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), DEVOURING_FLAME_MISSILE) == CAST_OK)
                        m_uiDevouringFlameTimer = 12000;
                }
                else
                    m_uiDevouringFlameTimer -= uiDiff;

                if (m_uiFuseArmorTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FUSE_ARMOR) == CAST_OK)
                        m_uiFuseArmorTimer = 13000;
                }
                else
                    m_uiFuseArmorTimer -= uiDiff;

                if (m_uiFlameBuffetTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FLAME_BUFFET : SPELL_FLAME_BUFFET_H) == CAST_OK)
                        m_uiFlameBuffetTimer = 13000;
                }
                else
                    m_uiFlameBuffetTimer -= uiDiff;

                if (m_uiFlameBreathTimer < uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H) == CAST_OK)
                    {
                        DoScriptText(EMOTE_DEEP_BREATH, m_creature);
                        m_uiFlameBreathTimer = 14000;
                    }
                }
                else
                    m_uiFlameBreathTimer -= uiDiff;

                DoMeleeAttackIfReady();

                break;
            }
            default:
                EnterEvadeMode();
        }

        // ground adds only in Air and grounded phase NOT in Permagrounded Phase
        if (m_Phase != PHASE_PERMAGROUND)
        {
            if (m_uiWavespawn < uiDiff)
            {
                m_creature->SummonCreature(NPC_RAZORSCALE_SPAWNER, Point[POSITION_RIGHT].x, Point[POSITION_RIGHT].y, Point[POSITION_RIGHT].z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 15000);
                m_creature->SummonCreature(NPC_RAZORSCALE_SPAWNER, Point[POSITION_LEFT].x, Point[POSITION_LEFT].y, Point[POSITION_LEFT].z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 15000);
                if (roll_chance_i(33))
                {
                    if (Creature* pRazorscaleSpawner = m_creature->SummonCreature(NPC_RAZORSCALE_SPAWNER, Point[POSITION_MIDDLE].x, Point[POSITION_MIDDLE].y, Point[POSITION_MIDDLE].z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 15000))
                        if (mob_mole_machineAI* molAI = (mob_mole_machineAI*)pRazorscaleSpawner->AI())
                            molAI->m_bIsSentinel = true;
                }
                m_uiWavespawn = urand(40000, 50000);
            }
            else
                m_uiWavespawn -= uiDiff;
        }

        // berserk
        if (m_uiBerserkTimer)
        {
            if (m_uiBerserkTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_BERSERK, CAST_TRIGGERED | CAST_AURA_NOT_PRESENT) == CAST_OK)
                    m_uiBerserkTimer = 0;
            }
            else
                m_uiBerserkTimer -= uiDiff;
        }

        if (m_creature->GetDistance2d(Point[POSITION_AIR].x, Point[POSITION_AIR].y) > 100.0f)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_razorscale(Creature* pCreature)
{
    return new boss_razorscaleAI(pCreature);
}

bool GOHello_go_repair_harpoon(Player* pPlayer, GameObject* pGo)
{
    instance_ulduar* m_pInstance = (instance_ulduar*)pGo->GetInstanceData();

    if (!m_pInstance)
        return false;
    if (Creature* pRazor = m_pInstance->GetSingleCreatureFromStorage(NPC_RAZORSCALE))
        if (boss_razorscaleAI* pRazorAI = (boss_razorscaleAI*)pRazor->AI())
            ++pRazorAI->m_uiHarpoonsUsed;

    return false;
}

void AddSC_boss_razorscale()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_razorscale";
    pNewScript->GetAI = GetAI_boss_razorscale;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_devouring_flame_target";
    pNewScript->GetAI = &GetAI_mob_devouring_flame_target;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_dark_rune_watcher";
    pNewScript->GetAI = &GetAI_mob_dark_rune_watcher;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_dark_rune_sentinel";
    pNewScript->GetAI = &GetAI_mob_dark_rune_sentinel;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_dark_rune_guardian";
    pNewScript->GetAI = &GetAI_mob_dark_rune_guardian;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_mole_machine";
    pNewScript->GetAI = &GetAI_mob_mole_machine;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_expedition_commander";
    pNewScript->GetAI = &GetAI_npc_expedition_commander;
    pNewScript->pGossipHello = &GossipHello_npc_expedition_commander;
    pNewScript->pGossipSelect = &GossipSelect_npc_expedition_commander;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "go_repair_harpoon";
    pNewScript->pGOUse = &GOHello_go_repair_harpoon;
    pNewScript->RegisterSelf();
}
