/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_kologarn
SD%Complete: achievment Wenn Blicke töten könnten
SDComment: eye beam
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"
#include "Vehicle.h"

enum
{
    // Yells
    SAY_AGGRO           = -1603090,
    SAY_SHOCKWEAVE      = -1603091,
    SAY_GRAB            = -1603092,
    SAY_LEFT_ARM_LOST   = -1603093,
    SAY_RIGHT_ARM_LOST  = -1603094,
    SAY_SLAY1           = -1603095,
    SAY_SLAY2           = -1603096,
    SAY_BERSERK         = -1603097,
    SAY_DEATH           = -1603098,
    EMOTE_RIGHT_ARM     = -1603099,
    EMOTE_LEFT_ARM      = -1603100,
    EMOTE_STONE_GRIP    = -1603101,

    // Kologarn
    SPELL_INSTAKILL_KOLOGARN_ARM= 63628,
    SPELL_OVERHEAD_SMASH        = 63356,
    SPELL_OVERHEAD_SMASH_H      = 64003,
    SPELL_ONE_ARMED_SMASH       = 63573,
    SPELL_ONE_ARMED_SMASH_H     = 64006,
    SPELL_STONE_SHOUT           = 63716,
    SPELL_STONE_SHOUT_H         = 64005,
    SPELL_PETRIFYING_BREATH     = 62030,
    SPELL_PETRIFYING_BREATH_H   = 63980,
    // Focused Eyebeam
    SPELL_FOCUSED_EYEBEAM_SUMMON= 63342, // Trigger for 63343 and 63701
    NPC_FOCUSED_EYEBEAM_RIGHT   = 33802,
    NPC_FOCUSED_EYEBEAM_LEFT    = 33632,
    SPELL_EYEBEAM_PERIODIC      = 63347,
    SPELL_EYEBEAM_PERIODIC_H    = 63977,
    SPELL_EYEBEAM_PERIODIC_VIS  = 63369,
    SPELL_EYEBEAM_VISUAL_LEFT   = 63348,
    SPELL_EYEBEAM_VISUAL_RIGHT  = 63676,

    SPELL_BERSERK               = 47008,
    // Left arm
    SPELL_ARM_SWEEP             = 63766,
    SPELL_ARM_SWEEP_H           = 63983,
    SPELL_SHOCKWAVE             = 63783,
    SPELL_SHOCKWAVE_H           = 63982,
    // Right arm
    SPELL_STONE_GRIP_GRAB       = 62166, // Trigger for 62056, 64290, 63962
    SPELL_STONE_GRIP_GRAB_H     = 63981, // Trigger for 63985, 64292, 63962
    SPELL_STONE_GRIP            = 64290,
    SPELL_STONE_GRIP_H          = 64292,
    SPELL_STONE_GRIP_VEH        = 62056,
    SPELL_STONE_GRIP_VEH_H      = 63985,
    // Both
    SPELL_ARM_VISUAL                 = 64753,
    SPELL_ARM_DEAD_DAMAGE_KOLOGARN   = 63629,
    SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H = 63979,
    // rubble
    SPELL_SUMMON_RUBBLE         = 63633,
    SPELL_RUMBLE                = 63818,    // on 10 man
    SPELL_STONE_NOVA            = 63978,    // on 25 man
    // NPC ids
    NPC_RUBBLE                  = 33768,

    // Kologarn Vehicle seats
    RIGHT_ARM_SLOT              = 1,
    LEFT_ARM_SLOT               = 0
};

const float KoloFront[3] = {1771.683f, -24.230f, 448.806f};

// Rubble
struct MANGOS_DLL_DECL mob_ulduar_rubbleAI : public ScriptedAI
{
    mob_ulduar_rubbleAI(Creature* pCreature) : ScriptedAI(pCreature)
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
        m_uiSpellTimer = urand(8000, 12000);
        m_creature->SetInCombatWithZone();
    }

    void JustDied(Unit* pKiller)
    {
        m_creature->ForcedDespawn(1000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_pInstance && m_pInstance->GetData(TYPE_KOLOGARN) != IN_PROGRESS) 
            m_creature->ForcedDespawn();

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpellTimer <= diff)
        {
            DoCast(m_creature, m_bIsRegularMode ? SPELL_RUMBLE : SPELL_STONE_NOVA);
            m_uiSpellTimer = urand(7000, 9000);
        }
        else
            m_uiSpellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_ulduar_rubble(Creature* pCreature)
{
    return new mob_ulduar_rubbleAI(pCreature);
}

// Kologarn
struct MANGOS_DLL_DECL boss_kologarnAI : public ScriptedAI
{
    boss_kologarnAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_pVehicleKit = m_creature->GetVehicleKit();
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    VehicleKitPtr m_pVehicleKit;

    uint32 m_uiOverheadSmashTimer;
    uint32 m_uiStoneShoutTimer;
    uint32 m_uiEyebeamTimer;
    uint32 m_uiRespawnRightTimer;
    uint32 m_uiRespawnLeftTimer;
    uint32 m_uiPetrifyingBreathTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiDisarmedTimer;
    uint32 m_uiRubbleCount;

    bool m_bIsRightDead;
    bool m_bIsLeftDead;

    void Reset()
    {
        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        m_uiOverheadSmashTimer    = urand(10000, 15000);
        m_uiStoneShoutTimer       = 0;
        m_uiEnrageTimer           = 480000;
        m_uiEyebeamTimer          = 10000 + urand(1000, 5000);
        m_uiPetrifyingBreathTimer = 4000;
        m_uiDisarmedTimer         = 0;

        m_bIsRightDead = false;
        m_bIsLeftDead  = false;

        m_uiRubbleCount = 0;
    }

    void EnterEvadeMode()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, FAIL);
        ScriptedAI::EnterEvadeMode();
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
        // kill Arms
        DoCast(m_creature, SPELL_INSTAKILL_KOLOGARN_ARM);
        DoCast(m_creature, SPELL_INSTAKILL_KOLOGARN_ARM);

        if (m_pInstance)
        {
            // For Disarmed Achievement, Kologarn must be kill by players
            if (m_uiDisarmedTimer > 12000 || pKiller->GetEntry() == m_creature->GetEntry())
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, false);

            if (m_uiRubbleCount >= 25)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_RUBBLE_AND_ROLL, true);

            m_pInstance->SetData(TYPE_KOLOGARN, DONE);
        }

    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_RUBBLE:
                ++m_uiRubbleCount;
                break;
            case NPC_FOCUSED_EYEBEAM_RIGHT:
                pSummoned->CastSpell(pSummoned, SPELL_EYEBEAM_VISUAL_RIGHT, true);
                break;
            case NPC_FOCUSED_EYEBEAM_LEFT:
                pSummoned->CastSpell(pSummoned, SPELL_EYEBEAM_VISUAL_LEFT, true);
                break;
            case NPC_RIGHT_ARM:
            case NPC_LEFT_ARM:
                pSummoned->CastSpell(pSummoned, SPELL_ARM_VISUAL, true);
            default:
                break;
        }
    }

    void SummonedCreatureDespawn(Creature* pSummoned)
    {
        if (m_uiRubbleCount && pSummoned->GetEntry() == NPC_RUBBLE)
            --m_uiRubbleCount;
    }

    void LeftArmDead()
    {
        DoScriptText(SAY_LEFT_ARM_LOST, m_creature);
        m_uiRespawnLeftTimer = urand(40000, 50000);
        m_bIsLeftDead = true;
        DoCast(m_creature, m_bIsRegularMode ? SPELL_ARM_DEAD_DAMAGE_KOLOGARN : SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H);
        DoCast(m_creature, SPELL_SUMMON_RUBBLE, true);
        if (m_pInstance)
        {
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_OPEN_ARMS, false);

            if (m_bIsRightDead && m_uiDisarmedTimer <= 12000)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, true);
            else
                m_uiDisarmedTimer = 0;
        }
    }

    void RightArmDead()
    {
        DoScriptText(SAY_RIGHT_ARM_LOST, m_creature);
        m_uiRespawnRightTimer = urand(40000, 50000);
        m_bIsRightDead = true;
        DoCast(m_creature, m_bIsRegularMode ? SPELL_ARM_DEAD_DAMAGE_KOLOGARN : SPELL_ARM_DEAD_DAMAGE_KOLOGARN_H);
        DoCast(m_creature, SPELL_SUMMON_RUBBLE, true);
        if (m_pInstance)
        {
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_OPEN_ARMS, false);

            if (m_bIsLeftDead && m_uiDisarmedTimer <= 12000)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, true);
            else
                m_uiDisarmedTimer = 0;
        }
    }

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KOLOGARN, IN_PROGRESS);
        //aggro yell
        DoScriptText(SAY_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void InstallArm(uint32 uiEntry)
    {
        if (m_pInstance)
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_DISARMED, false);
        int8 iSeat;
        switch(uiEntry)
        {
            case NPC_RIGHT_ARM:
                iSeat = RIGHT_ARM_SLOT;
                m_bIsRightDead = false;
                DoScriptText(EMOTE_RIGHT_ARM, m_creature);
                break;
            case NPC_LEFT_ARM:
                iSeat = LEFT_ARM_SLOT;
                DoScriptText(EMOTE_LEFT_ARM, m_creature);
                m_bIsLeftDead = false;
                break;
            default:
                return;
        }
        if (!m_pVehicleKit || m_pVehicleKit->GetPassenger(iSeat))
            return;
        m_pVehicleKit->InstallAccessory(iSeat);
    }
    
    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // disarmed achiev check
        if (m_bIsLeftDead || m_bIsRightDead)
            m_uiDisarmedTimer += uiDiff;

        if (m_bIsRightDead && m_bIsLeftDead)
        {
            if (m_uiStoneShoutTimer <= uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_STONE_SHOUT : SPELL_STONE_SHOUT_H);
                m_uiStoneShoutTimer = 2000;
            }
            else
                m_uiStoneShoutTimer -= uiDiff;
        }
        else if (m_uiOverheadSmashTimer <= uiDiff)
        {
            if (!m_bIsRightDead && !m_bIsLeftDead)
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OVERHEAD_SMASH : SPELL_OVERHEAD_SMASH_H);
            else
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ONE_ARMED_SMASH : SPELL_ONE_ARMED_SMASH_H);
            m_uiOverheadSmashTimer = urand(10000, 15000);
        }
        else
            m_uiOverheadSmashTimer -= uiDiff;

        if (m_uiEyebeamTimer <= uiDiff)
        {
            DoCast(m_creature, SPELL_FOCUSED_EYEBEAM_SUMMON, true);
            m_uiEyebeamTimer = 20000;
        }
        else
            m_uiEyebeamTimer -= uiDiff;

        // respawn arms
        if (m_bIsLeftDead)
        {
            if (m_uiRespawnLeftTimer <= uiDiff)
                InstallArm(NPC_LEFT_ARM);
            else
                m_uiRespawnLeftTimer -= uiDiff;
        }

        if (m_bIsRightDead)
        {
            if (m_uiRespawnRightTimer <= uiDiff)
                InstallArm(NPC_RIGHT_ARM);
            else
                m_uiRespawnRightTimer -= uiDiff;
        }
        
        if (m_uiPetrifyingBreathTimer <= uiDiff)
        {
            if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
                m_uiPetrifyingBreathTimer = 4000;
            else
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PETRIFYING_BREATH : SPELL_PETRIFYING_BREATH_H);
                m_uiPetrifyingBreathTimer = 1000;
            }
        }
        else
            m_uiPetrifyingBreathTimer -= uiDiff;

        if (m_uiEnrageTimer <= uiDiff)
        {
            DoScriptText(SAY_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            if (Creature* pRightArm = m_pInstance->GetSingleCreatureFromStorage(NPC_RIGHT_ARM))
                pRightArm->CastSpell(pRightArm, SPELL_BERSERK, false);
            if (Creature* pLeftArm = m_pInstance->GetSingleCreatureFromStorage(NPC_LEFT_ARM))
                pLeftArm->CastSpell(pLeftArm, SPELL_BERSERK, false);
            m_uiEnrageTimer = 30000;
        }
        else
            m_uiEnrageTimer -= uiDiff;

        if (!m_bIsLeftDead || !m_bIsRightDead) // TODO: Do direct attack by arms ?!
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_kologarn(Creature* pCreature)
{
    return new boss_kologarnAI(pCreature);
}

// Left Arm
struct MANGOS_DLL_DECL boss_left_armAI : public ScriptedAI
{
    boss_left_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetRespawnDelay(7*DAY);
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShockwaveTimer;

    void Reset()
    {
        m_uiShockwaveTimer = urand(10000, 25000);
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Creature* pKologarn = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN))
        {
            boss_kologarnAI* KologarnAI = (boss_kologarnAI*)pKologarn->AI();
            if (KologarnAI)
                KologarnAI->LeftArmDead();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShockwaveTimer <= uiDiff)
        {
            if (Creature* pKologarn = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN))
                DoScriptText(SAY_SHOCKWEAVE, pKologarn);

            DoCast(m_creature, m_bIsRegularMode ? SPELL_ARM_SWEEP : SPELL_ARM_SWEEP_H, true);
            m_uiShockwaveTimer = urand(10000, 25000);
        }
        else
            m_uiShockwaveTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_left_arm(Creature* pCreature)
{
    return new boss_left_armAI(pCreature);
}

// Right Arm
struct MANGOS_DLL_DECL boss_right_armAI : public ScriptedAI
{
    boss_right_armAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_pVehicleKit = m_creature->GetVehicleKit();
        Reset();
    }

    instance_ulduar* m_pInstance;
    VehicleKitPtr m_pVehicleKit;
    bool m_bIsRegularMode;
    bool m_bIsDead;

    uint32 m_uiStoneGripTimer;
    uint32 m_uiTeleportTimer;
    uint32 m_uiFreeDamage;
    uint32 m_uiMaxDamage;

    void Reset()
    {
        m_bIsDead = false;

        m_uiTeleportTimer       = 0;
        m_uiStoneGripTimer      = urand (10000, 30000);
        m_uiFreeDamage          = 0;
        m_uiMaxDamage           = m_bIsRegularMode ? 80000 : 380000;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (m_bIsDead)
            return;
        m_uiFreeDamage += uiDamage;
        if (m_pVehicleKit)
        {
            if (m_creature->GetHealth() <= uiDamage)
            {
                m_pVehicleKit->RemoveAllPassengers();
                m_bIsDead = true;
                uiDamage = 0;
            }
            else if (m_uiFreeDamage > m_uiMaxDamage || m_creature->GetHealth() <= uiDamage)
                m_pVehicleKit->RemoveAllPassengers();
        }
    }

    void PassengerBoarded(Unit* pWho, int8 seatId, bool apply)
    {
        if (!apply)
        {
            pWho->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_STONE_GRIP : SPELL_STONE_GRIP_H);
            pWho->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_STONE_GRIP_VEH : SPELL_STONE_GRIP_VEH_H);
            m_uiTeleportTimer = 1000;
        }
        else
            m_uiTeleportTimer = 16000; // After 15sec kill players
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance)
            return;

        if (Creature* pKologarn = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN))
        {
            boss_kologarnAI* KologarnAI = (boss_kologarnAI*)pKologarn->AI();
            if (KologarnAI)
                KologarnAI->RightArmDead();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiTeleportTimer)
        {
            if (m_uiTeleportTimer <= uiDiff)
            {
                Map::PlayerList const& pPlayers = m_creature->GetMap()->GetPlayers();
                if (!pPlayers.isEmpty())
                {
                    for (Map::PlayerList::const_iterator itr = pPlayers.begin(); itr != pPlayers.end(); ++itr)
                    {
                        Player* pPlayer = itr->getSource();
                        if (pPlayer && pPlayer->GetPositionX() > 1790.0f && pPlayer->GetDistance(m_creature) < 20.0f)
                            DoTeleportPlayer(pPlayer, KoloFront[0], KoloFront[1], KoloFront[2], 0.0f);
                    }
                }
                if (m_bIsDead)
                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
                m_uiTeleportTimer = 0;
            }
            else
                m_uiTeleportTimer -= uiDiff;
        }

        if (m_bIsDead)
            return;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiStoneGripTimer <= uiDiff)
        {
            Creature* pKologarn = m_pInstance->GetSingleCreatureFromStorage(NPC_KOLOGARN);
            if (pKologarn && DoCastSpellIfCan(pKologarn, m_bIsRegularMode ? SPELL_STONE_GRIP_GRAB : SPELL_STONE_GRIP_GRAB_H, CAST_TRIGGERED, pKologarn->GetObjectGuid()) == CAST_OK)
            {
                DoScriptText(SAY_GRAB, pKologarn);
                //DoScriptText(EMOTE_STONE_GRIP, m_creature);
                m_uiFreeDamage = 0;
                m_uiStoneGripTimer = urand (20000, 30000);
            }
        }
        else
            m_uiStoneGripTimer -= uiDiff;
    }
};

CreatureAI* GetAI_boss_right_arm(Creature* pCreature)
{
    return new boss_right_armAI(pCreature);
}

// Focused Eyebeam trigger mobs - just make them not attack in melee
struct MANGOS_DLL_DECL mob_eyebeam_triggerAI : public ScriptedAI
{
    mob_eyebeam_triggerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiStartMoveTimer;

    void Reset()
    {
        SetCombatMovement(false);
        m_uiStartMoveTimer = 1000;

        m_creature->SetDisplayId(11686);
        m_creature->SetSpeedRate(MOVE_RUN, 1.0);
        m_creature->ForcedDespawn(10000);

        if (m_creature->GetEntry() == NPC_FOCUSED_EYEBEAM_LEFT)
            m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_EYEBEAM_PERIODIC : SPELL_EYEBEAM_PERIODIC_H, true);
        else
            m_creature->CastSpell(m_creature, SPELL_EYEBEAM_PERIODIC_VIS, true);
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* spell)
    {
        if (m_pInstance && pTarget->GetTypeId() == TYPEID_PLAYER)
            m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_IF_LOOKS_COULD_KILL, false);
    }

    void MovementInform(uint32 uiMotionType, uint32 uiPointId)
    {
        if (uiMotionType != POINT_MOTION_TYPE)
            return;

        m_uiStartMoveTimer = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiStartMoveTimer <= uiDiff)
        {
            Unit* pTarget = m_creature->GetCreator();
            if (pTarget)
            {
                m_creature->GetMotionMaster()->MovePoint(0, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), false, false);
                m_uiStartMoveTimer = 2000;
            }
        }
        else
            m_uiStartMoveTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_eyebeam_trigger(Creature* pCreature)
{
    return new mob_eyebeam_triggerAI(pCreature);
}

// kologarn kill pit bunny - kills players that fall down into the pit. also handling bridge respawn after server restarts
struct MANGOS_DLL_DECL mob_kologarn_pit_kill_bunnyAI : public ScriptedAI
{
    mob_kologarn_pit_kill_bunnyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_fPositionZ = m_creature->GetPositionZ();
    }
    float m_fPositionZ;
    bool m_bBridgeLocked;
    void Reset(){}
    void UpdateAI(const uint32 uiDiff){}

    void MoveInLineOfSight(Unit* pWho)
    {
        if (pWho->GetTypeId() == TYPEID_PLAYER && !pWho->GetVehicle() && !((Player*)pWho)->isGameMaster())
        {
            if (m_creature->IsWithinLOSInMap(pWho) && pWho->GetPositionZ() - m_fPositionZ <= 15.0f)
                pWho->DealDamage(pWho, pWho->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NONE, NULL, false);
        }
    }
};
CreatureAI* GetAI_mob_kologarn_pit_kill_bunny(Creature* pCreature)
{
    return new mob_kologarn_pit_kill_bunnyAI(pCreature);
}

void AddSC_boss_kologarn()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_kologarn";
    NewScript->GetAI = GetAI_boss_kologarn;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_ulduar_rubble";
    NewScript->GetAI = &GetAI_mob_ulduar_rubble;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "boss_left_arm";
    NewScript->GetAI = &GetAI_boss_left_arm;
    NewScript->RegisterSelf();
 
    NewScript = new Script;
    NewScript->Name = "boss_right_arm";
    NewScript->GetAI = &GetAI_boss_right_arm;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_eyebeam_trigger";
    NewScript->GetAI = &GetAI_mob_eyebeam_trigger;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_kologarn_pit_kill_bunny";
    NewScript->GetAI = &GetAI_mob_kologarn_pit_kill_bunny;
    NewScript->RegisterSelf();
}
