/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_xt002
SD%Complete:
SDComment: correct number of adds in 25man missing; Void Zone Damage; Heart Life; Achievment Nerf Gravity Bombs
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    // xt yells
    
    SAY_DEATH               = -1603040,
    SAY_BERSERK             = -1603041,
    SAY_ADDS                = -1603042,
    SAY_HEART_CLOSE         = -1603043,
    SAY_HEART_OPEN          = -1603044,
    SAY_SLAY_02             = -1603045,
    SAY_SLAY_01             = -1603046,
    SAY_TANCTRUM            = -1603047,
    SAY_AGGRO               = -1603048,
    EMOTE_HEART             = -1603049,
    EMOTE_REPAIR            = -1603050,

    // xt-002
    SPELL_TYMPANIC_TANTRUM  = 62776,
    SPELL_LIGHT_BOMB_TRIG   = 65598,
    SPELL_SEARING_LIGHT     = 63018,
    SPELL_SEARING_LIGHT_H   = 65121,
    SPELL_GRAVITY_BOMB      = 63024,
    SPELL_GRAVITY_BOMB_H    = 64234,
    SPELL_ENRAGE            = 47008,
    SPELL_ENERGY_ORB        = 62790,

    // hard mode
    SPELL_HEARTBREAK        = 65737,
    SPELL_HEARTBREAK_H      = 64193,
    SPELL_VOIDZONE          = 64203,
    SPELL_VOIDZONE_H        = 64235,
    SPELL_VOIDZONE_EFFECT   = 46264,
    SPELL_LIFE_SPARK        = 64210,

    // Life Spark
    SPELL_SHOCK             = 64230,
    SPELL_STATIC_CHARGED    = 64227,
    SPELL_STATIC_CHARGED_H  = 64236,

    // Aura Void Zone!?

    // heart of the deconstructor
    SPELL_EXPOSED_HEART     = 63849,

    // XS-013 Scrapbot
    SPELL_SCRAP_REPAIR      = 62832,
    
    // XE-321 Boombot
    SPELL_BOOM              = 62834,

    // XM-024 Pummeller
    SPELL_CLEAVE            = 8374,
    SPELL_TRAMPLE           = 5568,
    SPELL_UPPERCUT          = 10966,

    // NPC ids
    NPC_HEART               = 33329,
    NPC_SCRAPBOT            = 33343,
    NPC_BOOMBOT             = 33346,
    NPC_PUMMELER            = 33344,
    NPC_VOIDZONE            = 34001,
    NPC_LIFESPARK           = 34004
};

enum XT002Phase
{
    PHASE_NORMAL        = 1,
    PHASE_HEART         = 2
};

// void zone
struct MANGOS_DLL_DECL mob_voidzoneAI : public ScriptedAI
{
    mob_voidzoneAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        pCreature->setFaction(14);
        SetCombatMovement(false);
        Reset();
    }

    uint32 m_uiSpellTimer;
    bool m_bIsRegularMode;
    SpellEntry const* m_SpellInfo;

    void Reset()
    {
        m_SpellInfo = (SpellEntry*)GetSpellStore()->LookupEntry(m_bIsRegularMode ? SPELL_VOIDZONE : SPELL_VOIDZONE_H);
        m_uiSpellTimer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
        // should be an aura here. Couldn't find it
        // hacky way, needs fixing!
        if (m_uiSpellTimer <= diff)
        {
            if (m_SpellInfo)
            {
                int32 damage = m_SpellInfo->CalculateSimpleValue(EFFECT_INDEX_1);
                m_creature->CastCustomSpell(m_creature, SPELL_VOIDZONE_EFFECT, &damage, 0, 0, true);
            }
            m_uiSpellTimer = 1000;
        }
        else
            m_uiSpellTimer -= diff;
    }
};

CreatureAI* GetAI_mob_voidzone(Creature* pCreature)
{
    return new mob_voidzoneAI(pCreature);
}

// lifespark
struct MANGOS_DLL_DECL mob_lifesparkAI : public ScriptedAI
{
    mob_lifesparkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiShockTimer;

    void Reset()
    {  
        DoCast(m_creature, m_bIsRegularMode ? SPELL_STATIC_CHARGED : SPELL_STATIC_CHARGED_H);
        m_creature->SetInCombatWithZone();
        m_uiShockTimer = 1000;
    }

    void UpdateAI(const uint32 uidiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiShockTimer <= uidiff)
        {
            DoCast(m_creature->getVictim(), SPELL_SHOCK);
            m_uiShockTimer = urand(4000, 7000);
        }
        else
            m_uiShockTimer -= uidiff;
    }
};

CreatureAI* GetAI_mob_lifespark(Creature* pCreature)
{
    return new mob_lifesparkAI(pCreature);
}

// XM-024 Pummeller
struct MANGOS_DLL_DECL mob_pummelerAI : public ScriptedAI
{
    mob_pummelerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;
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

        if (m_uiSpellTimer <= diff)
        {
            switch(urand(0, 2))
            {
                case 0:
                    DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                    break;
                case 1:
                    DoCast(m_creature->getVictim(), SPELL_TRAMPLE);
                    break;
                case 2:
                    DoCast(m_creature->getVictim(), SPELL_UPPERCUT);
                    break;
            }
            m_uiSpellTimer = urand(5000, 10000);
        }
        else
            m_uiSpellTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_pummeler(Creature* pCreature)
{
    return new mob_pummelerAI(pCreature);
}

// XE-321 Boombot
struct MANGOS_DLL_DECL mob_boombotAI : public ScriptedAI
{
    mob_boombotAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
        if (m_pInstance)
        {
            if (Creature* pXT002 = m_pInstance->GetSingleCreatureFromStorage(NPC_XT002))
                m_creature->GetMotionMaster()->MoveFollow(pXT002, 0.0f, 0.0f);
        }
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (m_creature->GetHealth() - uiDamage > m_creature->GetMaxHealth() * 0.5f)
            return;
        DoCast(m_creature, SPELL_BOOM);
        uiDamage = 0;
    }

    void MoveInLineOfSight(Unit* pWho)
    {
        // If we are within range melee the target
        if (pWho->GetTypeId() == TYPEID_PLAYER && m_creature->IsWithinDistInMap(pWho, 5.0f) && m_creature->IsWithinLOSInMap(pWho))
            DoCast(m_creature, SPELL_BOOM);
    }

    void UpdateAI(const uint32 diff)
    {
    }
};

CreatureAI* GetAI_mob_boombot(Creature* pCreature)
{
    return new mob_boombotAI(pCreature);
}

// XS-013 Scrapbot
struct MANGOS_DLL_DECL npc_scrapbotAI : public ScriptedAI
{
    npc_scrapbotAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;
    uint32 m_uiTimer;

    void Reset()
    {
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();
        if (m_pInstance)
        {
            if (Creature* pXT002 = m_pInstance->GetSingleCreatureFromStorage(NPC_XT002))
                m_creature->GetMotionMaster()->MoveFollow(pXT002, 0.0f, 0.0f);
        }
        m_uiTimer = 1000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiTimer <= diff)
        {
            if (m_pInstance)
            {
                Creature* pXT002 = m_pInstance->GetSingleCreatureFromStorage(NPC_XT002);
                if (pXT002 && pXT002->isAlive() && m_creature->IsWithinDistInMap(pXT002, 10.0f))
                {
                    pXT002->CastSpell(m_creature, SPELL_SCRAP_REPAIR, true);
                    m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_NERF_ENGINEERING, false);
                    DoScriptText(EMOTE_REPAIR, pXT002);
                    m_creature->ForcedDespawn(1000);
                    m_uiTimer = 2000;
                }
                else
                    m_uiTimer = 500;
            }
        }
        else
            m_uiTimer -= diff;
    }
};

CreatureAI* GetAI_npc_scrapbot(Creature* pCreature)
{
    return new npc_scrapbotAI(pCreature);
}

// Heart of the Deconstructor
struct MANGOS_DLL_DECL mob_xtheartAI : public ScriptedAI
{
    mob_xtheartAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        SetCombatMovement(false);
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiEnergyOrbTimer;
    uint32 m_uiDeathTimer;
    uint32 m_uiTotalDamage;

    void Reset()
    {
        DoCast(m_creature, SPELL_EXPOSED_HEART);
        m_creature->SetLevitate(true); // TODO: vehicle system not work properly
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_uiEnergyOrbTimer  = 0;
        m_uiDeathTimer      = 30000;
        m_uiTotalDamage     = 0;
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        m_uiTotalDamage += uiDamage;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_XT002_HARD, IN_PROGRESS);
        m_creature->ForcedDespawn();
    }

    ObjectGuid SelectRandomXtToyPile()
    {
        if (!m_pInstance || m_pInstance->m_lXtToyPileGuids.empty())
            return ObjectGuid((uint64)0);

        GuidList::iterator iter = m_pInstance->m_lXtToyPileGuids.begin();
        advance(iter, urand(0, m_pInstance->m_lXtToyPileGuids.size() - 1));

        return *iter;
    }

    void UpdateAI(const uint32 diff)
    {
        if (m_uiEnergyOrbTimer <= diff)
        {
            if (Creature* pXtToyPile = m_creature->GetMap()->GetCreature(SelectRandomXtToyPile()))
                m_creature->CastSpell(pXtToyPile, SPELL_ENERGY_ORB, false); // TODO: Arcane must be effect in target but effect in caster, disable by false triggered

            m_uiEnergyOrbTimer = urand(500, 2000);
        }
        else
            m_uiEnergyOrbTimer -= diff;

        // despawns the creature if not killed in 30 secs
        if (m_uiDeathTimer <= diff)
        {
            // pass damage to boss
            if (m_pInstance)
            {
                if (Creature* pXT002 = m_pInstance->GetSingleCreatureFromStorage(NPC_XT002))
                {
                    if (pXT002->isAlive())
                        pXT002->DealDamage(pXT002, m_uiTotalDamage, NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                }
            }
            m_creature->ForcedDespawn();
        }
        else
            m_uiDeathTimer -= diff;
    }
};

CreatureAI* GetAI_mob_xtheart(Creature* pCreature)
{
    return new mob_xtheartAI(pCreature);
}

// XT-002 Deconstructor
struct MANGOS_DLL_DECL boss_xt_002AI : public ScriptedAI
{
    boss_xt_002AI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    XT002Phase m_Phase;

    // spell timers
    uint32 m_uiHeartTimer;
    uint32 m_uiLightBombTimer;
    uint32 m_uiGravityBombTimer;
    uint32 m_uiTanctrumTimer;
    uint32 m_uiEnrageTimer;
    uint32 m_uiVoidZoneTimer;
    uint32 m_uiLifeSparkTimer;

    // Summon
    uint32 m_uiSpawnScrapbotGroupsTimer;
    uint8  m_uiScrapbotSpawnedGroups;
    uint8  m_uiScrapbotMaxGroups;
    uint32 m_uiSpawnPummelerTimer;
    GuidList m_lAddsGuidList;

    float   m_fHealthPercent;
    bool    m_bIsEnrage;

    ObjectGuid m_XtHeartGUID;

    bool m_bIsHardMode;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);

        m_Phase               = PHASE_NORMAL;
        // spell timers
        m_uiLightBombTimer    = 5000;     // 7 seconds the first 14 secs all after(7 secs in 25man)
        m_uiGravityBombTimer  = 30000;    // 11 seconds first 18 secs all after(11 secs in 25man)
        m_uiTanctrumTimer     = 35000;    // 38 seconds first 40 secs all after
        m_uiEnrageTimer       = 600000;   // 10 min
        m_uiVoidZoneTimer     = 60000;
        m_uiLifeSparkTimer    = 60000;

        m_fHealthPercent       = 75.0f;


        m_bIsEnrage             = false;
        m_bIsHardMode           = false;
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, DONE);
            if (m_bIsHardMode)
                m_pInstance->SetData(TYPE_XT002_HARD, DONE);
        }

        DoScriptText(SAY_DEATH, m_creature);
        DespawnSummonedCreatures();
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

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, IN_PROGRESS);
            if (m_pInstance->GetData(TYPE_XT002_TP) != DONE)
                m_pInstance->SetData(TYPE_XT002_TP, DONE);
        }

        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_XT002, FAIL);
            m_pInstance->SetData(TYPE_XT002_HARD, NOT_STARTED);
        }

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);

        DespawnSummonedCreatures();
        if (Creature* pHeart = m_creature->GetMap()->GetCreature(m_XtHeartGUID))
            pHeart->ForcedDespawn();
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SLAY_01 : SAY_SLAY_02, m_creature);
    }

    void JustSummoned(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_SCRAPBOT:
            case NPC_BOOMBOT:
            case NPC_PUMMELER:
            case NPC_VOIDZONE:
            case NPC_LIFESPARK:
                m_lAddsGuidList.push_back(pCreature->GetObjectGuid());
                break;
        }
    }

    ObjectGuid SelectRandomXtToyPile()
    {
        if (!m_pInstance || m_pInstance->m_lXtToyPileGuids.empty())
            return ObjectGuid((uint64)0);

        GuidList lXtToyPileGuids;
        for (GuidList::iterator iter = m_pInstance->m_lXtToyPileGuids.begin(); iter != m_pInstance->m_lXtToyPileGuids.end(); ++iter)
        {
            if (Creature* pToyPile = m_creature->GetMap()->GetCreature(*iter))
            {
                if (pToyPile->GetPositionX() < 850.0f) // Toys only spawn in two location!
                    lXtToyPileGuids.push_back(*iter);
            }
        }
        if (lXtToyPileGuids.empty())
            return ObjectGuid((uint64)0);

        GuidList::iterator iter = lXtToyPileGuids.begin();
        advance(iter, urand(0, lXtToyPileGuids.size() - 1));

        return *iter;
    }

    void SwitchIntoPhaseOne()
    {
        m_Phase = PHASE_NORMAL;
        DoScriptText(SAY_HEART_CLOSE, m_creature);
        m_creature->SetStandState(UNIT_STAND_STATE_STAND);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        if (m_creature->getVictim())
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
        m_uiLightBombTimer  = 7000;
        m_uiGravityBombTimer= 11000;
        m_uiTanctrumTimer   = 38000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (m_Phase != PHASE_NORMAL)
            {
                m_Phase = PHASE_NORMAL;
                m_creature->SetStandState(UNIT_STAND_STATE_STAND);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
            return;
        }

        if (!m_bIsEnrage)
        {
            if (m_uiEnrageTimer <= uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_ENRAGE) == CAST_OK)
                {
                    m_bIsEnrage = true;
                    DoScriptText(SAY_BERSERK, m_creature);
                }
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        switch(m_Phase)
        {
            case PHASE_NORMAL:
            {
                if (m_uiLightBombTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SEARING_LIGHT : SPELL_SEARING_LIGHT_H) == CAST_OK)
                        m_uiLightBombTimer = urand(10000, 14000);
                }
                else
                    m_uiLightBombTimer -= uiDiff;

                if (m_uiGravityBombTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_GRAVITY_BOMB : SPELL_GRAVITY_BOMB_H) == CAST_OK)
                        m_uiGravityBombTimer = urand(25000, 30000);
                }
                else
                    m_uiGravityBombTimer -= uiDiff;

                if (m_uiTanctrumTimer <= uiDiff)
                {
                    if (DoCastSpellIfCan(m_creature, SPELL_TYMPANIC_TANTRUM) == CAST_OK)
                    {
                        DoScriptText(SAY_TANCTRUM, m_creature);
                        m_uiTanctrumTimer = 60000;
                    }
                }
                else
                    m_uiTanctrumTimer -= uiDiff;

                if (!m_bIsHardMode && m_creature->GetHealthPercent() < m_fHealthPercent)
                {
                    m_fHealthPercent -= 25.0f;
                    m_uiHeartTimer = 30000;
                    m_creature->CastStop();
                    m_Phase = PHASE_HEART;

                    m_creature->StopMoving();
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_creature->SetStandState(UNIT_STAND_STATE_CUSTOM);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                    DoScriptText(SAY_HEART_OPEN, m_creature);
                    DoScriptText(EMOTE_HEART, m_creature);

                    m_uiScrapbotMaxGroups     = urand(7, 10);
                    m_uiScrapbotSpawnedGroups = 0;

                    // timers
                    m_uiSpawnScrapbotGroupsTimer  = urand(0, 4000);
                    m_uiSpawnPummelerTimer        = urand(1, 29);

                    float x, y, z;
                    m_creature->GetPosition(x, y, z);
                    if (Creature* pHeart = m_creature->SummonCreature(NPC_HEART, x, y, z + 2.0f, m_creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0))
                        m_XtHeartGUID = pHeart->GetObjectGuid();
                }

                DoMeleeAttackIfReady();
                break;
            }
            case PHASE_HEART:
            {
                if (m_uiScrapbotSpawnedGroups < m_uiScrapbotMaxGroups)
                {
                    if (m_uiSpawnScrapbotGroupsTimer <= uiDiff)
                    {
                        if (Creature* pXtToyPile = m_creature->GetMap()->GetCreature(SelectRandomXtToyPile()))
                        {
                            if (m_uiScrapbotSpawnedGroups == 0)
                                DoScriptText(SAY_ADDS, m_creature);
                            ++m_uiScrapbotSpawnedGroups;
                            float x, y, z;
                            pXtToyPile->GetPosition(x, y, z);
                            for (uint8 i = 0; i < 5; ++i)
                                m_creature->SummonCreature(i ? NPC_SCRAPBOT : NPC_BOOMBOT, x + frand(-5.0f, 5.0f) , y + frand(-5.0f, 5.0f), z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                        }
                        m_uiSpawnScrapbotGroupsTimer = urand(0, 4000);
                    }
                    else
                        m_uiSpawnScrapbotGroupsTimer -= uiDiff;
                }

                if (m_uiSpawnPummelerTimer)
                {
                    if (m_uiSpawnPummelerTimer <= uiDiff)
                    {
                        if (Creature* pXtToyPile = m_creature->GetMap()->GetCreature(SelectRandomXtToyPile()))
                        {
                            float x, y, z;
                            pXtToyPile->GetPosition(x, y, z);
                            m_creature->SummonCreature(NPC_PUMMELER, x, y, z, 0.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                            m_uiSpawnPummelerTimer = 0;
                        }
                    }
                    else
                        m_uiSpawnPummelerTimer -= uiDiff;
                }


                // Switch into Phase 1
                if (m_uiHeartTimer <= uiDiff)
                {
                    SwitchIntoPhaseOne();
                }
                else
                    m_uiHeartTimer -= uiDiff;

                if (m_pInstance && m_pInstance->GetData(TYPE_XT002_HARD) == IN_PROGRESS)
                {
                    SwitchIntoPhaseOne();
                    DoCast(m_creature, m_bIsRegularMode ? SPELL_HEARTBREAK : SPELL_HEARTBREAK_H);
                    m_pInstance->SetSpecialAchievementCriteria(TYPE_ACHIEV_HEARTBREAKER, true);
                    m_bIsHardMode = true;
                }

                break;
            }
            default:
                SwitchIntoPhaseOne();
                break;
        }
        // Prevent exploit
        if (m_creature->GetPositionX() < 760.0f)
            EnterEvadeMode();
    }
};

CreatureAI* GetAI_boss_xt_002(Creature* pCreature)
{
    return new boss_xt_002AI(pCreature);
}

void AddSC_boss_xt002()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_xt_002";
    pNewScript->GetAI = GetAI_boss_xt_002;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_xtheart";
    pNewScript->GetAI = &GetAI_mob_xtheart;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_pummeler";
    pNewScript->GetAI = &GetAI_mob_pummeler;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_boombot";
    pNewScript->GetAI = &GetAI_mob_boombot;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_scrapbot";
    pNewScript->GetAI = &GetAI_npc_scrapbot;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_voidzone";
    pNewScript->GetAI = &GetAI_mob_voidzone;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_lifespark";
    pNewScript->GetAI = &GetAI_mob_lifespark;
    pNewScript->RegisterSelf();
}
