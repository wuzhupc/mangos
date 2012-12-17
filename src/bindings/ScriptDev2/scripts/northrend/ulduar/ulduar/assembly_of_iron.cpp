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
SDName: boss_iron_council
SD%Complete: 
SDComment: Achievment(Can't Do That While Stunned,
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    //yells
    SAY_MOLGEIM_AGGRO       = -1603060,
    SAY_MOLGEIM_DEATH1      = -1603061,
    SAY_MOLGEIM_DEATH2      = -1603062,
    SAY_MOLGEIM_DEATH_RUNE  = -1603063,
    SAY_MOLGEIM_SUMMON      = -1603064,
    SAY_MOLGEIM_SLAY1       = -1603065,
    SAY_MOLGEIM_SLAY2       = -1603066,
    SAY_MOLGEIM_BERSERK     = -1603067,

    SAY_STEEL_AGGRO         = -1603070,
    SAY_STEEL_DEATH1        = -1603071,
    SAY_STEEL_DEATH2        = -1603072,
    SAY_STEEL_SLAY1         = -1603073,
    SAY_STEEL_SLAY2         = -1603074,
    SAY_STEEL_OVERWHELMING  = -1603075,
    SAY_STEEL_BERSERK       = -1603076,

    SAY_BRUNDIR_AGGR0       = -1603080,
    SAY_BRUNDIR_WHIRL       = -1603081,
    SAY_BRUNDIR_DEATH1      = -1603082,
    SAY_BRUNDIR_DEATH2      = -1603083,
    SAY_BRUNDIR_SLAY1       = -1603084,
    SAY_BRUNDIR_SLAY2       = -1603085,
    SAY_BRUNDIR_BERSERK     = -1603086,
    SAY_BRUNDIR_FLY         = -1603087,

    //all
    SPELL_BERSERK               = 47008,
    SPELL_SUPERCHARGE           = 61920,
    SPELL_IRON_COUNCIL_CREDIT   = 65195,    // custom spell in spell_dbc.sql

    //steelbreaker
    SPELL_HIGH_VOLTAGE          = 61890,
    SPELL_HIGH_VOLTAGE_H        = 63498,
    SPELL_FUSION_PUNCH          = 61903,
    SPELL_FUSION_PUNCH_H        = 63493,
    SPELL_STATIC_DISRUPTION     = 44008,
    SPELL_STATIC_DISRUPTION_H   = 63494,
    SPELL_OVERWHELMING_POWER    = 64637,
    SPELL_OVERWHELMING_POWER_H  = 61888,
    SPELL_ELECTRICAL_CHARGE     = 61902,

    //runemaster molgeim
    SPELL_SHIELD                = 62274,
    SPELL_SHIELD_H              = 63489,
    SPELL_RUNE_OF_POWER         = 63513,
    SPELL_RUNE_OF_DEATH         = 62269,
    SPELL_RUNE_OF_DEATH_H       = 63490,
    SPELL_RUNE_OF_SUMMONING     = 62273,
    //rune of power
    AURA_RUNE_OF_POWER          = 61974,
    //rune of summoning
    AURA_RUNE_OF_SUMMONING      = 62019,
    //lightning elemental
    SPELL_LIGHTNING_BLAST       = 62054,
    SPELL_LIGHTNING_BLAST_H     = 63491,

    //stormcaller brundir
    SPELL_CHAIN_LIGHTNING       = 61879,
    SPELL_CHAIN_LIGHTNING_H     = 63479,
    SPELL_OVERLOAD              = 61869,
    SPELL_LIGHTNING_WHIRL       = 61915,
    SPELL_LIGHTNING_WHIRL_H     = 63483,
    SPELL_STORMSHIELD           = 64187,
    SPELL_LIGHTNING_TENDRILS    = 61887,
    SPELL_LIGHTNING_TENDRILS_H  = 63486,
    LIGHTNING_TENDRILS_VISUAL   = 61883,
    //NPC ids
    NPC_LIGHTNING_ELEMENTAL     = 32958,
    NPC_RUNE_OF_SUMMONING       = 33051,
    NPC_RUNE_OF_POWER           = 33705
};

enum BrundirPhase
{
    PHASE_NORMAL,
    PHASE_TENDRILS,
    PHASE_TENDRILS_DOWN
};

// Lightning Elemental
struct MANGOS_DLL_DECL npc_ulduar_lightning_elementalAI : public ScriptedAI
{
    npc_ulduar_lightning_elementalAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiCheckTimer;

    void Reset()
    {
        m_uiCheckTimer = 1000;
        if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            m_creature->AddThreat(pTarget, 1000.0f);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            m_creature->SetInCombatWithZone();
            if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                m_creature->ForcedDespawn();
            return;
        }

        if (m_uiCheckTimer <= diff)
        {
            if (m_creature->CanReachWithMeleeAttack(m_creature->getVictim()))
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_BLAST : SPELL_LIGHTNING_BLAST_H);
                m_creature->ForcedDespawn(500);
                m_uiCheckTimer = 5000;
            }
            m_uiCheckTimer = 1000;
        }
        else
            m_uiCheckTimer -= diff;
    }
};

CreatureAI* GetAI_npc_ulduar_lightning_elemental(Creature* pCreature)
{
    return new npc_ulduar_lightning_elementalAI(pCreature);
}

//Stormcaller Brundir
struct MANGOS_DLL_DECL boss_brundirAI : public ScriptedAI
{
    boss_brundirAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiChainLightningTimer;
    uint32 m_uiOverloadTimer;
    uint32 m_uiLightningWhirlTimer;
    uint32 m_uiTendrilsTimer;
    uint32 m_uiTendrilsEndTimer;
    uint32 m_uiEnrageTimer;

    uint8 m_uiSuperchargeStack;

    BrundirPhase m_uiBrundirPhase;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetLevitate(false);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

        m_uiBrundirPhase = PHASE_NORMAL;
        m_uiChainLightningTimer = 0;
        m_uiOverloadTimer       = urand(35000, 40000);
        m_uiEnrageTimer         = 900000;
        m_uiSuperchargeStack    = 0;
        m_uiTendrilsTimer       = 60000;
        m_uiLightningWhirlTimer = urand(12000, 15000);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_SUPERCHARGE)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth());
            ++m_uiSuperchargeStack;
            if (m_uiSuperchargeStack > 1)
                DoCast(m_creature, SPELL_STORMSHIELD, true);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_uiSuperchargeStack > 1)
        {
            m_pInstance->SetData(TYPE_ASSEMBLY, DONE);
            DoCast(m_creature, SPELL_IRON_COUNCIL_CREDIT, true);
        }
        else
        {
            if (m_pInstance)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_YOU_BRUNDIR, false);
            m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            DoCast(m_creature, SPELL_SUPERCHARGE, true);
        }

        DoScriptText(urand(0, 1) ? SAY_BRUNDIR_DEATH1 : SAY_BRUNDIR_DEATH2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_BRUNDIR_AGGR0, m_creature);
        if (!m_pInstance)
            return;
        if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_STEELBREAKER))
        {
            if (pTemp->isAlive())
                pTemp->SetInCombatWithZone();
        }
        if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_RUNEMASTER_MOLGEIM))
        {
            if (pTemp->isAlive())
                pTemp->SetInCombatWithZone();
        }
        if (m_pInstance->GetData(TYPE_ASSEMBLY) != IN_PROGRESS)
            m_pInstance->SetData(TYPE_ASSEMBLY, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ASSEMBLY) != FAIL)
                m_pInstance->SetData(TYPE_ASSEMBLY, FAIL);
        }
    }

    void KilledUnit(Unit* pWho)
    {
        DoScriptText(urand(0, 1) ? SAY_BRUNDIR_SLAY1 : SAY_BRUNDIR_SLAY2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEnrageTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_BRUNDIR_BERSERK, m_creature);
                m_uiEnrageTimer = 1800000;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        switch(m_uiBrundirPhase)
        {
            case PHASE_NORMAL:
            {
                // Supercharge Abilities
                if (m_uiSuperchargeStack)
                {
                    if (m_uiLightningWhirlTimer <= uiDiff)
                    {
                        if (m_uiTendrilsTimer > uiDiff && DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_WHIRL : SPELL_LIGHTNING_WHIRL_H) == CAST_OK)
                        {
                            DoScriptText(SAY_BRUNDIR_WHIRL, m_creature);
                            m_uiLightningWhirlTimer = urand(12000, 15000);
                        }
                    }
                    else
                        m_uiLightningWhirlTimer -= uiDiff;

                    if (m_uiSuperchargeStack > 1)
                    {
                        if (m_uiTendrilsTimer <= uiDiff)
                        {
                            if (!m_creature->IsNonMeleeSpellCasted(false))
                            {
                                m_uiBrundirPhase = PHASE_TENDRILS;
                                DoScriptText(SAY_BRUNDIR_FLY, m_creature);

                                m_creature->StopMoving();
                                m_creature->GetMotionMaster()->Clear();
                                m_creature->GetMotionMaster()->MoveIdle();
                                DoCast(m_creature, m_bIsRegularMode ? SPELL_LIGHTNING_TENDRILS : SPELL_LIGHTNING_TENDRILS_H, true);
                                DoCast(m_creature, LIGHTNING_TENDRILS_VISUAL, true);
                                m_creature->SetLevitate(true);
                                m_creature->SetWalk(false);
                                m_creature->SetSpeedRate(MOVE_RUN, 0.8f);
                                m_creature->GetMotionMaster()->MovePoint(1, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 10.0f);
                                m_uiTendrilsTimer = 4000;
                                m_uiTendrilsEndTimer = 26000;
                            }
                            else
                                m_uiTendrilsTimer = 0;
                        }
                        else
                            m_uiTendrilsTimer -= uiDiff;
                    }
                }

                if (m_uiOverloadTimer <= uiDiff)
                {
                    if (m_uiTendrilsTimer > uiDiff && DoCastSpellIfCan(m_creature, SPELL_OVERLOAD) == CAST_OK)
                    {
                        m_uiOverloadTimer = 60000;
                        m_uiLightningWhirlTimer = urand(12000, 15000);
                    }
                }
                else
                    m_uiOverloadTimer -= uiDiff;

                if (m_uiChainLightningTimer <= uiDiff)
                {
                    if (m_uiTendrilsTimer > uiDiff)
                    {
                        Unit* pVictim = m_creature->getVictim();
                        bool bMeeleeAttack = pVictim && m_creature->GetDistance2d(pVictim) < 15.0f;
                        if (bMeeleeAttack && !m_creature->CanReachWithMeleeAttack(pVictim))
                        {
                            m_creature->GetMotionMaster()->MoveChase(pVictim);
                            m_uiChainLightningTimer = urand(5000, 10000);
                        }
                        else
                        {
                            if (pVictim = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                            {
                                if (DoCastSpellIfCan(pVictim, m_bIsRegularMode ? SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING_H) == CAST_OK)
                                    m_uiChainLightningTimer = bMeeleeAttack ? urand(5000, 10000) : 2000;
                            }
                        }
                    }
                }
                else
                    m_uiChainLightningTimer -= uiDiff;

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_TENDRILS:
                if (m_uiTendrilsTimer <= uiDiff)
                {
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
                    {
                        m_creature->SetSpeedRate(MOVE_RUN, 1.8f);
                        m_creature->GetMotionMaster()->MovePoint(1, pTarget->GetPositionX(), pTarget->GetPositionY(), m_creature->GetPositionZ());
                    }
                    m_uiTendrilsTimer = 4000;
                }
                else
                    m_uiTendrilsTimer -= uiDiff;

                if (m_uiTendrilsEndTimer <= uiDiff)
                {
                    m_uiBrundirPhase = PHASE_TENDRILS_DOWN;

                    m_creature->StopMoving();
                    m_creature->GetMotionMaster()->Clear();
                    float x = m_creature->GetPositionX();
                    float y = m_creature->GetPositionY();
                    float z = m_creature->GetTerrain()->GetWaterOrGroundLevel(x, y, m_creature->GetPositionZ());
                    m_creature->GetMotionMaster()->MovePoint(1, x, y, z);
                    m_creature->SetSpeedRate(MOVE_RUN, 0.8f);

                    m_uiTendrilsTimer = 4000;
                }
                else
                    m_uiTendrilsEndTimer -= uiDiff;
                break;
            case PHASE_TENDRILS_DOWN:
                if (m_uiTendrilsTimer <= uiDiff)
                {
                    m_uiBrundirPhase = PHASE_NORMAL;
                    m_creature->SetLevitate(false);
                    m_creature->SetWalk(true);
                    if (m_creature->HasAura(SPELL_LIGHTNING_TENDRILS))
                        m_creature->RemoveAurasDueToSpell(SPELL_LIGHTNING_TENDRILS);
                    if (m_creature->HasAura(SPELL_LIGHTNING_TENDRILS_H))
                        m_creature->RemoveAurasDueToSpell(SPELL_LIGHTNING_TENDRILS_H);
                    if (m_creature->HasAura(LIGHTNING_TENDRILS_VISUAL))
                        m_creature->RemoveAurasDueToSpell(LIGHTNING_TENDRILS_VISUAL);

                    m_uiTendrilsTimer = 60000;
                }
                else
                    m_uiTendrilsTimer -= uiDiff;
                break;
        }
    }
};

CreatureAI* GetAI_boss_brundir(Creature* pCreature)
{
    return new boss_brundirAI(pCreature);
}

// Runemaster Molgeim
struct MANGOS_DLL_DECL boss_molgeimAI : public ScriptedAI
{
    boss_molgeimAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiShieldOfRunesTimer;
    uint32 m_uiRuneOfPowerTimer;
    uint32 m_uiRuneOfDeathTimer;
    uint32 m_uiRuneOfSummoningTimer;
    uint32 m_uiEnrageTimer;

    uint8 m_uiSuperchargeStack;
    GuidList m_lAddsGuidList;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
        m_uiShieldOfRunesTimer   = urand(30000, 40000);;
        m_uiRuneOfPowerTimer     = urand(10000, 20000);
        m_uiEnrageTimer          = 900000;
        m_uiSuperchargeStack     = 0;
        m_uiRuneOfDeathTimer     = urand(20000, 40000);
        m_uiRuneOfSummoningTimer = urand(10000, 20000);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_SUPERCHARGE)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth());
            ++m_uiSuperchargeStack;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_RUNE_OF_SUMMONING:
                pSummoned->CastSpell(pSummoned, AURA_RUNE_OF_SUMMONING, true);
                break;
            case NPC_RUNE_OF_POWER:
                pSummoned->CastSpell(pSummoned, AURA_RUNE_OF_POWER, true);
                pSummoned->ForcedDespawn(urand(34000, 38000));
                break;
            case NPC_LIGHTNING_ELEMENTAL:
                break;
            default:
                return;
        }
        m_lAddsGuidList.push_back(pSummoned->GetObjectGuid());
    }

    void DespawnSummonedCreatures()
    {
        if (!m_lAddsGuidList.empty())
        {
            for (GuidList::iterator iter = m_lAddsGuidList.begin(); iter != m_lAddsGuidList.end(); ++iter)
            {
                if (Creature* pSummoned = m_creature->GetMap()->GetCreature(*iter))
                    pSummoned->ForcedDespawn();
            }
            m_lAddsGuidList.clear();
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_uiSuperchargeStack > 1)
        {
            m_pInstance->SetData(TYPE_ASSEMBLY, DONE);
            DoCast(m_creature, SPELL_IRON_COUNCIL_CREDIT, true);
            DespawnSummonedCreatures();
        }
        else
        {
            if (m_pInstance)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_YOU_MOLGEIM, false);
            m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            DoCast(m_creature, SPELL_SUPERCHARGE, true);
        }

        DoScriptText(urand(0, 1) ? SAY_MOLGEIM_DEATH1 : SAY_MOLGEIM_DEATH2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_MOLGEIM_AGGRO, m_creature);
        if (!m_pInstance)
            return;
        if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_STEELBREAKER))
        {
            if (pTemp->isAlive())
                pTemp->SetInCombatWithZone();
        }
        if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_STORMCALLER_BRUNDIR))
        {
            if (pTemp->isAlive())
                pTemp->SetInCombatWithZone();
        }
        if (m_pInstance->GetData(TYPE_ASSEMBLY) != IN_PROGRESS)
            m_pInstance->SetData(TYPE_ASSEMBLY, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ASSEMBLY) != FAIL)
                m_pInstance->SetData(TYPE_ASSEMBLY, FAIL);
        }
        DespawnSummonedCreatures();
    }

    void KilledUnit(Unit* who)
    {
        DoScriptText(urand(0, 1) ? SAY_MOLGEIM_SLAY1 : SAY_MOLGEIM_SLAY2, m_creature);
    }

    void DoCastRuneOfPower()
    {
        Creature* pRuneOfPowerTarget = NULL;
        switch(urand(0, 2))
        {
            case 0:
                pRuneOfPowerTarget = m_pInstance->GetSingleCreatureFromStorage(NPC_STORMCALLER_BRUNDIR);
                if (pRuneOfPowerTarget && !pRuneOfPowerTarget->isAlive() && urand(0, 1))
                    pRuneOfPowerTarget = m_pInstance->GetSingleCreatureFromStorage(NPC_STEELBREAKER);
                break;
            case 1:
                pRuneOfPowerTarget = m_pInstance->GetSingleCreatureFromStorage(NPC_STEELBREAKER);
                if (pRuneOfPowerTarget && !pRuneOfPowerTarget->isAlive() && urand(0, 1))
                    pRuneOfPowerTarget = m_pInstance->GetSingleCreatureFromStorage(NPC_STORMCALLER_BRUNDIR);
                break;
            case 2:
                break;
        }
        if (pRuneOfPowerTarget && pRuneOfPowerTarget->isAlive())
            pRuneOfPowerTarget->CastSpell(pRuneOfPowerTarget, SPELL_RUNE_OF_POWER, true, 0, 0, m_creature->GetObjectGuid());
        else
            m_creature->CastSpell(m_creature, SPELL_RUNE_OF_POWER, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // Normal spells
        if (m_uiShieldOfRunesTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SHIELD : SPELL_SHIELD_H) == CAST_OK)
                m_uiShieldOfRunesTimer = urand(30000, 35000);;
        }
        else
            m_uiShieldOfRunesTimer -= uiDiff;

        if (m_uiRuneOfPowerTimer <= uiDiff)
        {
            DoCastRuneOfPower();
            m_uiRuneOfPowerTimer = 32000;
        }
        else
            m_uiRuneOfPowerTimer -= uiDiff;

        // Supercharge Abilities
        if (m_uiSuperchargeStack)
        {
            if (m_uiRuneOfDeathTimer <= uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
                {
                    if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_RUNE_OF_DEATH : SPELL_RUNE_OF_DEATH_H) == CAST_OK)
                    {
                        DoScriptText(SAY_MOLGEIM_DEATH_RUNE, m_creature);
                        m_uiRuneOfDeathTimer = 30000;
                    }
                }
            }
            else
                m_uiRuneOfDeathTimer -= uiDiff;

            if (m_uiSuperchargeStack > 1)
            {
                if (m_uiRuneOfSummoningTimer <= uiDiff)
                {
                    m_creature->CastStop();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0, (uint32)0, SELECT_FLAG_PLAYER))
                    {
                        if (DoCastSpellIfCan(pTarget, SPELL_RUNE_OF_SUMMONING) == CAST_OK)
                        {
                            DoScriptText(SAY_MOLGEIM_SUMMON, m_creature);
                            m_uiRuneOfSummoningTimer = urand(20000, 30000);
                        }
                    }
                }
                else
                    m_uiRuneOfSummoningTimer -= uiDiff;
            }
        }

        if (m_uiEnrageTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_MOLGEIM_BERSERK, m_creature);
                m_uiEnrageTimer = 1800000;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_molgeim(Creature* pCreature)
{
    return new boss_molgeimAI(pCreature);
}

// Steelbreaker
struct MANGOS_DLL_DECL boss_steelbreakerAI : public ScriptedAI
{
    boss_steelbreakerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiFusionPunchTimer;
    uint32 m_uiStaticDisruptionTimer;
    uint32 m_uiOverwhelmingPowerTimer;
    uint32 m_uiEnrageTimer;

    uint8 m_uiSuperchargeStack;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

        m_uiFusionPunchTimer       = urand(15000, 20000);
        m_uiEnrageTimer            = 900000;
        m_uiSuperchargeStack       = 0;
        m_uiOverwhelmingPowerTimer = 5000;
        m_uiStaticDisruptionTimer  = 12000;
    }

    void KilledUnit(Unit* pVictim)
    {
        if (m_uiSuperchargeStack > 1)
            DoCast(m_creature, SPELL_ELECTRICAL_CHARGE);

        DoScriptText(urand(0, 1) ? SAY_STEEL_SLAY1 : SAY_STEEL_SLAY2, m_creature);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_SUPERCHARGE)
        {
            m_creature->SetHealth(m_creature->GetMaxHealth());
            ++m_uiSuperchargeStack;
            if (m_uiSuperchargeStack > 1)
                DoCast(m_creature, m_bIsRegularMode ? SPELL_HIGH_VOLTAGE : SPELL_HIGH_VOLTAGE_H, true);
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_uiSuperchargeStack > 1)
        {
            m_pInstance->SetData(TYPE_ASSEMBLY, DONE);
            DoCast(m_creature, SPELL_IRON_COUNCIL_CREDIT, true);
        }
        else
        {
            if (m_pInstance)
                m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_YOU_STEELBREAKER, false);
            m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            DoCast(m_creature, SPELL_SUPERCHARGE, true);
        }
        DoScriptText(urand(0, 1) ? SAY_STEEL_DEATH1 : SAY_STEEL_DEATH2, m_creature);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_STEEL_AGGRO, m_creature);
        if (!m_pInstance)
            return;
        if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_RUNEMASTER_MOLGEIM))
        {
            if (pTemp->isAlive())
                pTemp->SetInCombatWithZone();
        }
        if (Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(NPC_STORMCALLER_BRUNDIR))
        {
            if (pTemp->isAlive())
                pTemp->SetInCombatWithZone();
        }
        if (m_pInstance->GetData(TYPE_ASSEMBLY) != IN_PROGRESS)
            m_pInstance->SetData(TYPE_ASSEMBLY, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_ASSEMBLY) != FAIL)
                m_pInstance->SetData(TYPE_ASSEMBLY, FAIL);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFusionPunchTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FUSION_PUNCH : SPELL_FUSION_PUNCH_H))
                m_uiFusionPunchTimer = 15000;
        }
        else
            m_uiFusionPunchTimer -= uiDiff;
        
        // Supercharge Abilities
        if (m_uiSuperchargeStack)
        {
            if (m_uiStaticDisruptionTimer <= uiDiff)
            {
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, (uint32)0, SELECT_FLAG_PLAYER))
                {
                    if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_STATIC_DISRUPTION : SPELL_STATIC_DISRUPTION_H) == CAST_OK)
                        m_uiStaticDisruptionTimer = 12000;
                }
                else // Solo player
                    DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_STATIC_DISRUPTION : SPELL_STATIC_DISRUPTION_H);
            }
            else
                m_uiStaticDisruptionTimer -= uiDiff;

            if (m_uiSuperchargeStack > 1)
            {
                if (m_uiOverwhelmingPowerTimer <= uiDiff)
                {

                    if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OVERWHELMING_POWER : SPELL_OVERWHELMING_POWER_H) == CAST_OK)
                    {
                        DoScriptText(SAY_STEEL_OVERWHELMING, m_creature);
                        m_uiOverwhelmingPowerTimer = m_bIsRegularMode ? 65000 : 35000;
                    }
                }
                else
                    m_uiOverwhelmingPowerTimer -= uiDiff;
            }
        }

        if (m_uiEnrageTimer <= uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
            {
                DoScriptText(SAY_STEEL_BERSERK, m_creature);
                m_uiEnrageTimer = 1800000;
            }
        }
        else
            m_uiEnrageTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_steelbreaker(Creature* pCreature)
{
    return new boss_steelbreakerAI(pCreature);
}

void AddSC_boss_iron_council()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_brundir";
    pNewScript->GetAI = GetAI_boss_brundir;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_molgeim";
    pNewScript->GetAI = GetAI_boss_molgeim;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "boss_steelbreaker";
    pNewScript->GetAI = GetAI_boss_steelbreaker;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_ulduar_lightning_elemental";
    pNewScript->GetAI = &GetAI_npc_ulduar_lightning_elemental;
    pNewScript->RegisterSelf();
}
