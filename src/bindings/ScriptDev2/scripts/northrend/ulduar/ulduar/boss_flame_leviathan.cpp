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
SDName: boss_leviathan
SD%Complete: 
SDComment: needs vehicles , add Flying Mobs, Throw Passenger, Grab Crate
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"
#include "Vehicle.h"

enum say
{
    SAY_AGGRO           = -1603203,
    SAY_DEATH           = -1603202,
    SAY_SLAY            = -1603201,
    SAY_CHANGE1         = -1603204,
    SAY_CHANGE2         = -1603205,
    SAY_CHANGE3         = -1603206,
    SAY_PLAYER_ON_TOP   = -1603207,
    SAY_OVERLOAD1       = -1603208,
    SAY_OVERLOAD2       = -1603209,
    SAY_OVERLOAD3       = -1603210,
    SAY_HARD_MODE       = -1603211,
    SAY_TOWERS_DOWN     = -1603212,
    SAY_FROST_TOWER     = -1603213,
    SAY_FIRE_TOWER      = -1603214,
    SAY_ENERGY_TOWER    = -1603215,
    SAY_NATURE_TOWER    = -1603216,

    EMOTE_PURSUE        = -1603217,
};

enum spells
{
    SPELL_PURSUED           = 62374,

    SPELL_MISSILE_BARRAGE   = 62400,
    SPELL_FLAME_VENTS       = 62396,
    SPELL_FLAME_VENTS_TIGGER= 63847,
    SPELL_BATTERING_RAM     = 62376,

    SPELL_GATHERING_SPEED   = 62375,
    // interupted by
    SPELL_OVERLOAD_CIRCUIT  = 62399,

    SPELL_SEARING_FLAME     = 62402, // used by defense turret
    // interupted by
    SPELL_SYSTEMS_SHUTDOWN  = 62475,

    SPELL_FLAME_CANNON      = 62395,
    //SPELL_FLAME_CANNON    = 64692, trigger the same spell
    
    // used by players -> to be added later
    SPELL_ELECTROSHOCK      = 62522,
    SPELL_SMOKE_TRAIL       = 63575,

    // tower of nature
    SPELL_LASH              = 65062,
    SPELL_FREYA_WARD        = 62906,
    SPELL_TOWER_OF_LIFE     = 64482,
    // tower of flames
    SPELL_MIMIRON_INFERNO   = 62909,
    SPELL_TOWER_OF_FLAMES   = 65075,
    // tower of frost
    SPELL_TOWER_OF_FROST    = 65079,
    SPELL_HODIR_FURY        = 62533,
    // tower of storms
    SPELL_THORIMS_HAMMER    = 62911,
    SPELL_TOWER_OF_STORMS   = 65076,

    // pool of tar
    SPELL_TAR_PASSIV        = 62288,
    SPELL_BLAZE             = 62292,

    // Pyrit Vehicles
    SPELL_LIQUID_PYRITE     = 62494,
    

    AURA_DUMMY_BLUE         = 63294,
    AURA_DUMMY_GREEN        = 63295,
    AURA_DUMMY_YELLOW       = 63292
};

enum Mobs
{
    MOB_MECHANOLIFT                 = 33214,
    MOB_LIQUID                      = 33189,
    MOB_CONTAINER                   = 33218,
    
    NPC_THORIM_HAMMER               = 33365,
    NPC_THORIM_HAMMER_TARGETTING    = 33364,

    NPC_MIMIRON_INFERNO             = 33370,
    NPC_MIMIRON_INFERNO_TARGETTING  = 33369,

    NPC_HODIR_FURY                  = 33212,
    NPC_HODIR_FURY_TARGETTING       = 33108,

    NPC_FREYA_WARD                  = 33367,
    NPC_FREYA_WARD_TARGETTING       = 33366,

    DEFENSE_TURRET                  = 33142
};

enum Seats
{
    SEAT_PLAYER = 0,
    SEAT_TURRET = 1,
    SEAT_DEVICE = 2,
    SEAT_CANNON = 7
};

enum Vehicles
{
    VEHICLE_SIEGE      = 33060,
    VEHICLE_CHOPPER    = 33062,
    VEHICLE_DEMOLISHER = 33109,

    MAX_VEHICLE        = 5,
    MIN_VEHICLE        = 2
};

enum eAchievementData
{
    ACHIEV_10_ORBITAL_BOMBARDMENT = 2913, // one Tower
    ACHIEV_25_ORBITAL_BOMBARDMENT = 2918,
    ACHIEV_10_ORBITAL_DEVASTATION = 2914, // two Towers
    ACHIEV_25_ORBITAL_DEVASTATION = 2916,
    ACHIEV_10_NUKED_FROM_ORBIT    = 2915, // three Towers
    ACHIEV_25_NUKED_FROM_ORBIT    = 2917,
    ACHIEV_10_ORBIT_UARY          = 3056,
    ACHIEV_25_ORBIT_UARY          = 3057,
/* Nicht implementiert
    ACHIEV_10_SHUTOUT = 2911,
    ACHIEV_25_SHUTOUT = 2912,
    ACHIEV_10_THREE_CAR_GARAGE = 2907,
    ACHIEV_25_THREE_CAR_GARAGE = 2908,
    ACHIEV_10_UNBROKEN = 2905,
    ACHIEV_25_UNBROKEN = 2906,*/ 
};

struct Positions
{
    float x, y, z, o;
};
static Positions Center[]=
{
    {274.0f, -31.0f, 410.0f, 0.0f}
};

static Positions IntroPoint[]=
{
    {342.896f, -14.113f, 409.804f, -3.132478f}
};

const Positions FreyaWard[4] =
{
    {377.02f, -119.10f, 409.81f, 0.0f},
    {377.02f,  54.78f,  409.81f, 0.0f},
    {185.62f,  54.78f,  409.81f, 0.0f},
    {185.62f, -119.10f, 409.81f, 0.0f}
};

const Positions PosSiege[5] =
{
    {-814.59f,-64.54f,429.92f,5.969f},
    {-784.37f,-33.31f,429.92f,5.096f},
    {-808.99f,-52.10f,429.92f,5.668f},
    {-798.59f,-44.00f,429.92f,5.663f},
    {-812.83f,-77.71f,429.92f,0.046f}
};

const Positions PosChopper[5] =
{
    {-717.83f,-106.56f,430.02f,0.122f},
    {-717.83f,-114.23f,430.44f,0.122f},
    {-717.83f,-109.70f,430.22f,0.122f},
    {-718.45f,-118.24f,430.26f,0.052f},
    {-718.45f,-123.58f,430.41f,0.085f}
};

const Positions PosDemolisher[5] =
{
    {-724.12f,-176.64f,430.03f,2.543f},
    {-766.70f,-225.03f,430.50f,1.710f},
    {-729.54f,-186.26f,430.12f,1.902f},
    {-756.01f,-219.23f,430.50f,2.369f},
    {-798.01f,-227.24f,429.84f,1.446f}
};

const Positions WayMimironInferno[4] =
{
    {161.45f, -37.50f, 409.80f, 0.0f},
    {275.14f,  64.12f, 409.80f, 0.0f},
    {349.67f, -31.71f, 409.80f, 0.0f},
    {246.72f,-129.38f, 409.80f, 0.0f}
};


struct MANGOS_DLL_DECL boss_flame_leviathanAI : public ScriptedAI
{
    boss_flame_leviathanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiBatteringRamTimer;
    uint32 m_uiFlameVentsTimer;
    uint32 m_uiMissileBarrageTimer;
    uint32 m_uiPursueTimer;
    uint32 m_uiGatheringSpeedTimer;
    uint32 m_uiSummonFlyerTimer;

    uint8  m_uiActiveTowers;

    bool m_bAliveHodirsTower;
    bool m_bAliveFreyasTower;
    bool m_bAliveMimironsTower;
    bool m_bAliveThorimsTower;

    uint32 m_uiHodirFuryTimer;
    uint32 m_uiThorimHammerTimer;

    void Reset()
    {
//        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
//        m_creature->ApplySpellImmune(49560, 0, 0, true);
        
        m_creature->RemoveAurasDueToSpell(SPELL_TOWER_OF_FROST);
        m_creature->RemoveAurasDueToSpell(SPELL_TOWER_OF_STORMS);
        m_creature->RemoveAurasDueToSpell(SPELL_TOWER_OF_FLAMES);
        m_creature->RemoveAurasDueToSpell(SPELL_TOWER_OF_LIFE);

        m_uiBatteringRamTimer   = 15000 + rand()%20000;
        m_uiFlameVentsTimer     = 15000 + rand()%10000;
        m_uiMissileBarrageTimer = 1000;
        m_uiPursueTimer         = 0;
        m_uiGatheringSpeedTimer = 50000;
        m_uiSummonFlyerTimer    = 2000;

        m_uiActiveTowers = 0;

        m_bAliveHodirsTower   = false;
        m_bAliveFreyasTower   = false;
        m_bAliveMimironsTower = false;
        m_bAliveThorimsTower  = false;

        m_uiHodirFuryTimer      = urand(15000, 20000);
        m_uiThorimHammerTimer   = urand(15000, 20000);

        m_creature->SetSpeedRate(MOVE_RUN, 0.3f);
    }

    void Aggro(Unit* who)
    {
        if (m_pInstance)
        {
            if (m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) == HARD_DIFFICULTY)
                CheckForTowers();
            m_pInstance->SetData(TYPE_LEVIATHAN, IN_PROGRESS);
            if (m_pInstance->GetData(TYPE_LEVIATHAN_TP) != DONE)
                m_pInstance->SetData(TYPE_LEVIATHAN_TP, DONE);
        }

        DoScriptText(SAY_AGGRO, m_creature);
    }

    void JustDied(Unit* killer)
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_LEVIATHAN, DONE);
            
            if (m_uiActiveTowers)
            {
                switch (m_uiActiveTowers)
                {
                    case 4:
                        m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_10_ORBIT_UARY : ACHIEV_25_ORBIT_UARY);
                    case 3:
                        m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_10_NUKED_FROM_ORBIT : ACHIEV_25_NUKED_FROM_ORBIT);
                    case 2:
                        m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_10_ORBITAL_DEVASTATION : ACHIEV_25_ORBITAL_DEVASTATION);
                    case 1:
                        m_pInstance->DoCompleteAchievement(m_bIsRegularMode ? ACHIEV_10_ORBITAL_BOMBARDMENT : ACHIEV_25_ORBITAL_BOMBARDMENT);
                }
            }
        }

        DoScriptText(SAY_DEATH, m_creature);
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        if (m_creature->isAlive())
            m_creature->NearTeleportTo(IntroPoint[0].x, IntroPoint[0].y, IntroPoint[0].z, IntroPoint[0].o);

        m_creature->SetLootRecipient(NULL);

        Reset();
        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, FAIL);
    }

    void KilledUnit(Unit* who)
    {
        DoScriptText(SAY_SLAY, m_creature);

        if (who->HasAura(SPELL_PURSUED)) // current target is destroyed
        {
            if (m_uiPursueTimer < 25000)
                m_uiPursueTimer = 0;
            else
                m_uiPursueTimer = m_uiPursueTimer - 25000; // cooldown
        }
    }

    void SpellHitTarget(Unit* pTarget, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_PURSUED)
        {
            DoResetThreat();
            m_creature->AddThreat(pTarget, 100000000.0f);
            m_creature->SetInCombatWithZone();
            AttackStart(pTarget);
        }
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        /*if (spell->Id == 62472)
            vehicle->InstallAllAccessories();
        else
            if (spell->Id == SPELL_ELECTROSHOCK)
                m_creature->InterruptSpell(CURRENT_CHANNELED_SPELL);*/
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        uiDamage *= 4;
        if (m_creature->HasAura(SPELL_SYSTEMS_SHUTDOWN, EFFECT_INDEX_0))
            uiDamage += uiDamage/2;
    }

    bool CollossusDead()
    {
        std::list<Creature*> lCreatureList;
        GetCreatureListWithEntryInGrid(lCreatureList, m_creature, 33237, 100.0f);
        if (lCreatureList.empty())
            return false; // not loaded yet
        for (std::list<Creature*>::iterator itr = lCreatureList.begin(); itr != lCreatureList.end(); ++itr)
        {
            if ((*itr)->isAlive())
                return false;
        }
        return true;
    }

    void CheckForTowers()
    {
        if (!m_bAliveHodirsTower)
        {
            if (GameObject* pTower = m_pInstance->GetSingleGameObjectFromStorage(GO_TOWER_OF_FROST))
                if (pTower->GetHealth())
                {
                    m_bAliveHodirsTower = true;
                    m_creature->CastSpell(m_creature, SPELL_TOWER_OF_FROST,true);
                    m_uiActiveTowers++;
                }
        }
        if (!m_bAliveFreyasTower)
        {
            if (GameObject* pTower = m_pInstance->GetSingleGameObjectFromStorage(GO_TOWER_OF_LIFE))
                if (pTower->GetHealth())
                {
                    m_bAliveFreyasTower = true;
                    m_creature->CastSpell(m_creature, SPELL_TOWER_OF_LIFE, true);
                    // Summon Freya's Ward (invisible)
                    for (uint8 i = 0; i < 4; ++i )
                        m_creature->SummonCreature(NPC_FREYA_WARD, FreyaWard[i].x, FreyaWard[i].y, FreyaWard[i].z, FreyaWard[i].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    m_uiActiveTowers++;
                }
        }
        if (!m_bAliveMimironsTower)
        {
            if (GameObject* pTower = m_pInstance->GetSingleGameObjectFromStorage(GO_TOWER_OF_FLAME))
                if (pTower->GetHealth())
                {
                    m_bAliveMimironsTower = true;
                    m_creature->CastSpell(m_creature, SPELL_TOWER_OF_FLAMES,true);
                    // Summon Mimiron's Inferno (invisible)
                    m_creature->SummonCreature(NPC_MIMIRON_INFERNO, WayMimironInferno[0].x, WayMimironInferno[0].y, WayMimironInferno[0].z, WayMimironInferno[0].o, TEMPSUMMON_MANUAL_DESPAWN, 0);
                    m_uiActiveTowers++;
                }
        }
        if (!m_bAliveThorimsTower)
        {
            if (GameObject* pTower = m_pInstance->GetSingleGameObjectFromStorage(GO_TOWER_OF_STORMS))
                if (pTower->GetHealth())
                {
                    m_bAliveThorimsTower = true;
                    m_creature->CastSpell(m_creature, SPELL_TOWER_OF_STORMS,true);
                    m_uiActiveTowers++;
                }
        }
        m_creature->SetHealth(m_creature->GetMaxHealth());
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (m_pInstance && (m_pInstance->GetData(TYPE_LEVIATHAN) == NOT_STARTED || m_pInstance->GetData(TYPE_LEVIATHAN) == FAIL) && m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) != NONE_DIFFICULTY)
        {
            if (CollossusDead())
            {
                m_creature->GetMotionMaster()->MovePoint(1, IntroPoint[0].x, IntroPoint[0].y, IntroPoint[0].z);
                m_pInstance->SetData(TYPE_LEVIATHAN, SPECIAL);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // pursue
        if (m_uiPursueTimer < uiDiff)
        {
            switch(urand(0, 3))
            {
                case 0: DoScriptText(SAY_CHANGE1, m_creature); break;
                case 1: DoScriptText(SAY_CHANGE2, m_creature); break;
                case 2: DoScriptText(SAY_CHANGE3, m_creature); break;
            }
            DoScriptText(EMOTE_PURSUE, m_creature);
            m_creature->CastSpell(m_creature, SPELL_PURSUED, true);

            m_uiPursueTimer = 30000;

            // Prevent exploit
            if (m_creature->GetPositionX() > 400.0f || m_creature->GetPositionX() < 148.0f)
                EnterEvadeMode();
        }
        else
            m_uiPursueTimer -= uiDiff;

        // flame vents
        if (m_uiFlameVentsTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_FLAME_VENTS);
            m_uiFlameVentsTimer = 30000 + rand()%20000;
        }
        else
            m_uiFlameVentsTimer -= uiDiff;

        // battering ram
        if (m_uiBatteringRamTimer < uiDiff)
        {
            DoCast(m_creature->getVictim(), SPELL_BATTERING_RAM);
            m_uiBatteringRamTimer = 25000 + rand()%15000;
        }
        else
            m_uiBatteringRamTimer -= uiDiff;

        /* flyers
        it should summon some flyers. needs more research!
        if (m_uiSummonFlyerTimer < uiDiff)
        {
            m_creature->SummonCreature(MOB_MECHANOLIFT, m_creature->GetPositionX() + rand()%20, m_creature->GetPositionY() + rand()%20, m_creature->GetPositionZ() + 50, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 40000);
            m_uiSummonFlyerTimer = 2000;
        }
        else
            m_uiSummonFlyerTimer -= uiDiff;*/

        // missile barrage
        if (m_uiMissileBarrageTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_MISSILE_BARRAGE);
            m_uiMissileBarrageTimer = 3000 + rand()%2000;
        }
        else
            m_uiMissileBarrageTimer -= uiDiff;

        if (m_uiGatheringSpeedTimer < uiDiff)
        {
            DoCast(m_creature, SPELL_GATHERING_SPEED);
            m_uiGatheringSpeedTimer = urand(50000, 60000);
        }
        else
            m_uiGatheringSpeedTimer -= uiDiff;

        // tower of hodir
        if (m_bAliveHodirsTower)
        {
            if (m_uiHodirFuryTimer <= uiDiff)
            {
                m_creature->SummonCreature(NPC_HODIR_FURY, Center[0].x + frand(-30.0f, 30.0f), Center[0].y + frand(-30.0f, 30.0f), Center[0].z, Center[0].o, TEMPSUMMON_TIMED_DESPAWN, urand(30000, 60000));
                m_uiHodirFuryTimer = urand(30000, 60000);
            }
            else
                m_uiHodirFuryTimer -= uiDiff;
        }

        // tower of thorim
        if (m_bAliveThorimsTower)
        {
            if (m_uiThorimHammerTimer <= uiDiff)
            {
                m_creature->SummonCreature(NPC_THORIM_HAMMER, Center[0].x + frand(-30.0f, 30.0f), Center[0].y + frand(-30.0f, 30.0f), Center[0].z, Center[0].o, TEMPSUMMON_TIMED_DESPAWN, urand(30000, 60000));
                m_uiThorimHammerTimer = urand(30000, 60000);
            }
            else
                m_uiThorimHammerTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_defense_turretAI : public ScriptedAI
{
    mob_defense_turretAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    uint32 m_uiSpell_Timer;

    void Reset()
    {
        m_uiSpell_Timer = urand(10000, 15000);
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (spell->Id == SPELL_SYSTEMS_SHUTDOWN)
            m_creature->ForcedDespawn();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSpell_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_SEARING_FLAME);
            m_uiSpell_Timer = urand(10000, 15000);
        }
        else
            m_uiSpell_Timer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_pool_of_tarAI : public ScriptedAI
{
    mob_pool_of_tarAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        DoCast(m_creature, SPELL_TAR_PASSIV, true);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        SetCombatMovement(false);        
    }

    void SpellHit(Unit* caster, const SpellEntry* spell)
    {
        if (spell->SchoolMask & SPELL_SCHOOL_MASK_FIRE && !m_creature->HasAura(SPELL_BLAZE))
            DoCast(m_creature, SPELL_BLAZE, true);
    }
    void DamageTaken(Unit* killer, uint32 &uidamage)
    {
        uidamage = 0;
    }
};

struct MANGOS_DLL_DECL mob_mechanoliftAI : public ScriptedAI
{
    mob_mechanoliftAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
    }

    void JustDied(Unit* pKiller)
    {
        if (Creature* pLiquid = DoSpawnCreature(MOB_LIQUID, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN, 30000))
        {
            pLiquid->CastSpell(pLiquid, SPELL_LIQUID_PYRITE, true);
        }

    }
};

struct MANGOS_DLL_DECL mob_freyas_wardAI : public ScriptedAI
{
    mob_freyas_wardAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiTimer;

    void Reset()
    {
        m_uiTimer = urand(20000, 30000);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_FREYA_WARD_TARGETTING)
            pSummoned->CastSpell(pSummoned, SPELL_FREYA_WARD, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiTimer <= uiDiff)
        {
            DoCast(m_creature, AURA_DUMMY_GREEN, true);
            DoSpawnCreature(NPC_FREYA_WARD_TARGETTING, 0.0f, 0.0f, 100.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 10000);
            m_uiTimer = urand(20000, 30000);
        }
        else
            m_uiTimer -= uiDiff;

        if (m_pInstance->GetData(TYPE_LEVIATHAN) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL mob_hodirs_furyAI : public ScriptedAI
{
    mob_hodirs_furyAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiHodirFuryTimer;
    uint32 m_uiRandomMovementTimer;

    void Reset()
    {
        m_uiHodirFuryTimer         = urand(5000, 10000);
        m_uiRandomMovementTimer    = 50000;

        SwitchPosition();
    }

    void MovementInform(uint32 type, uint32 id)
    {
        SwitchPosition();
    }

    void SwitchPosition()
    {
        if (m_pInstance)
        {
            if (Player* pPlayer = m_pInstance->GetPlayerInMap(true, true))
                m_creature->GetMotionMaster()->MovePoint(1, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiHodirFuryTimer <= uiDiff)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->MoveIdle();
            if (Creature* pTrigger = DoSpawnCreature(NPC_HODIR_FURY_TARGETTING, 0.0f, 0.0f, 100.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 10000))
                pTrigger->CastSpell(m_creature, SPELL_HODIR_FURY, true);
            m_uiHodirFuryTimer = urand(10000, 15000);
            m_uiRandomMovementTimer = 4000;
        }
        else
            m_uiHodirFuryTimer -= uiDiff;

        if (m_uiRandomMovementTimer <= uiDiff)
        {
            SwitchPosition();
            m_uiRandomMovementTimer = urand(2000, 10000);
        }
        else
            m_uiRandomMovementTimer -= uiDiff;

        if (!m_creature->HasAura(AURA_DUMMY_BLUE))
            DoCast(m_creature, AURA_DUMMY_BLUE, true);

        if (m_pInstance->GetData(TYPE_LEVIATHAN) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL mob_mimirons_infernoAI : public ScriptedAI
{
    mob_mimirons_infernoAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;

    uint32 m_uiInfernoTimer;
    uint32 m_uiStartTimer;
    uint8  m_uiWayPoint;

    void Reset()
    {
        m_uiInfernoTimer = urand(10000, 20000);
        m_uiStartTimer   = urand(10000, 20000);
        m_uiWayPoint     = 1;

        m_creature->SetSpeedRate(MOVE_RUN, 1.0f);
        m_creature->SetWalk(false);
        m_creature->GetMotionMaster()->MovePoint(m_uiWayPoint, WayMimironInferno[m_uiWayPoint].x, WayMimironInferno[m_uiWayPoint].y, WayMimironInferno[m_uiWayPoint].z);
    }

    void MovementInform(uint32 type, uint32 id)
    {
        ++m_uiWayPoint;
        if (m_uiWayPoint > 3)
            m_uiWayPoint = 0;
        m_creature->GetMotionMaster()->MovePoint(m_uiWayPoint, WayMimironInferno[m_uiWayPoint].x, WayMimironInferno[m_uiWayPoint].y, WayMimironInferno[m_uiWayPoint].z);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiStartTimer > uiDiff)
        {
            m_uiStartTimer -= uiDiff;
            return;
        }
        else if (m_uiStartTimer)
            m_uiStartTimer = 0;

        if (!m_creature->HasAura(AURA_DUMMY_YELLOW))
            DoCast(m_creature, AURA_DUMMY_YELLOW, true);

        if (m_uiInfernoTimer <= uiDiff)
        {
            if (Creature* pTrigger = DoSpawnCreature(NPC_MIMIRON_INFERNO_TARGETTING, 0.0f, 0.0f, 100.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 30000))
            {
                pTrigger->CastSpell(m_creature, SPELL_MIMIRON_INFERNO, true);
                m_uiInfernoTimer = 2000;
            }
        }
        else
            m_uiInfernoTimer -= uiDiff;

        if (m_pInstance->GetData(TYPE_LEVIATHAN) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

struct MANGOS_DLL_DECL mob_thorims_hammerAI : public ScriptedAI
{
    mob_thorims_hammerAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        Reset();
    }

    instance_ulduar* m_pInstance;
    uint32 m_uiThorimsHammerTimer;
    uint32 m_uiRandomMovementTimer;

    void Reset()
    {
        m_uiThorimsHammerTimer     = urand(5000, 10000);
        m_uiRandomMovementTimer    = 50000;

        SwitchPosition();
    }

    void MovementInform(uint32 type, uint32 id)
    {
        SwitchPosition();
    }

    void SwitchPosition()
    {
        if (m_pInstance)
        {
            if (Player* pPlayer = m_pInstance->GetPlayerInMap(true, true))
                m_creature->GetMotionMaster()->MovePoint(1, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ());
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiThorimsHammerTimer <= uiDiff)
        {
            m_creature->StopMoving();
            m_creature->GetMotionMaster()->MoveIdle();
            if (Creature* pTrigger = DoSpawnCreature(NPC_THORIM_HAMMER_TARGETTING, 0.0f, 0.0f, 100.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 10000))
                pTrigger->CastSpell(m_creature, SPELL_THORIMS_HAMMER, true);
            m_uiThorimsHammerTimer = urand(10000, 15000);
            m_uiRandomMovementTimer = 4000;
        }
        else
            m_uiThorimsHammerTimer -= uiDiff;

        if (m_uiRandomMovementTimer <= uiDiff)
        {
            SwitchPosition();
            m_uiRandomMovementTimer = urand(2000, 10000);
        }
        else
            m_uiRandomMovementTimer -= uiDiff;

        if (!m_creature->HasAura(AURA_DUMMY_BLUE))
            DoCast(m_creature, AURA_DUMMY_BLUE, true);

        if (m_pInstance->GetData(TYPE_LEVIATHAN) != IN_PROGRESS)
            m_creature->ForcedDespawn();
    }
};

CreatureAI* GetAI_mob_defense_turret(Creature* pCreature)
{
    return new mob_defense_turretAI(pCreature);
}

CreatureAI* GetAI_boss_flame_leviathan(Creature* pCreature)
{
    return new boss_flame_leviathanAI(pCreature);
}

CreatureAI* GetAI_mob_pool_of_tar(Creature* pCreature)
{
    return new mob_pool_of_tarAI(pCreature);
}

CreatureAI* GetAI_mob_mechanolift(Creature* pCreature)
{
    return new mob_mechanoliftAI(pCreature);
}

CreatureAI* GetAI_mob_freyas_ward(Creature* pCreature)
{
    return new mob_freyas_wardAI(pCreature);
}

CreatureAI* GetAI_mob_hodirs_fury(Creature* pCreature)
{
    return new mob_hodirs_furyAI(pCreature);
}

CreatureAI* GetAI_mob_mimirons_inferno(Creature* pCreature)
{
    return new mob_mimirons_infernoAI(pCreature);
}

CreatureAI* GetAI_mob_thorims_hammer(Creature* pCreature)
{
    return new mob_thorims_hammerAI(pCreature);
}

struct MANGOS_DLL_DECL mob_lorekeeperAI : public ScriptedAI
{
    mob_lorekeeperAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_ulduar* m_pInstance;

    bool m_bIsRegularMode;

    uint32 m_uiSummonTimer;

    ObjectGuid m_SiegeGuids[MAX_VEHICLE];
    ObjectGuid m_DemolisherGuids[MAX_VEHICLE];
    ObjectGuid m_ChopperGuids[MAX_VEHICLE];

    void Reset()
    {
        m_uiSummonTimer = 5000;
    }

    void UpdateVehicleFlag(Creature* pVehicle)
    {
        if (!pVehicle)
            return;
        switch (pVehicle->GetEntry())
        {
            case VEHICLE_SIEGE:
            case VEHICLE_DEMOLISHER:
            case VEHICLE_CHOPPER:
                if (m_pInstance->GetData(TYPE_LEVIATHAN) == DONE || m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) == NONE_DIFFICULTY)
                    pVehicle->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                else
                    pVehicle->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_LEVIATHAN) == IN_PROGRESS)
            return;

        if (m_uiSummonTimer <= uiDiff)
        {
            Creature* pVehicle = NULL;
            for (uint8 i = 0; i < (m_bIsRegularMode ? MIN_VEHICLE : MAX_VEHICLE); ++i)
            {
                if (m_SiegeGuids[i].IsEmpty() || !(pVehicle = m_creature->GetMap()->GetCreature(m_SiegeGuids[i])))
                {
                    if (pVehicle = m_creature->SummonCreature(VEHICLE_SIEGE, PosSiege[i].x, PosSiege[i].y, PosSiege[i].z, PosSiege[i].o, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                        m_SiegeGuids[i] = pVehicle->GetObjectGuid();
                }
                UpdateVehicleFlag(pVehicle);

                if (m_DemolisherGuids[i].IsEmpty() || !(pVehicle = m_creature->GetMap()->GetCreature(m_DemolisherGuids[i])))
                {
                    if (pVehicle = m_creature->SummonCreature(VEHICLE_DEMOLISHER, PosDemolisher[i].x, PosDemolisher[i].y, PosDemolisher[i].z, PosDemolisher[i].o, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                        m_DemolisherGuids[i] = pVehicle->GetObjectGuid();
                }
                UpdateVehicleFlag(pVehicle);

                if (m_ChopperGuids[i].IsEmpty() || !(pVehicle = m_creature->GetMap()->GetCreature(m_ChopperGuids[i])))
                {
                    if (pVehicle = m_creature->SummonCreature(VEHICLE_CHOPPER, PosChopper[i].x, PosChopper[i].y, PosChopper[i].z, PosChopper[i].o, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                        m_ChopperGuids[i] = pVehicle->GetObjectGuid();
                }
                UpdateVehicleFlag(pVehicle);

            }
            m_uiSummonTimer = 5000;
        }
        else
            m_uiSummonTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_lorekeeper(Creature* pCreature)
{
    return new mob_lorekeeperAI(pCreature);
}

bool GossipHello_mob_lorekeeper(Player* player, Creature* pCreature)
{
    instance_ulduar* m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
    if (!m_pInstance)
        return true;
    if (m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) == NONE_DIFFICULTY)
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Activate secondary defencive systems.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_mob_lorekeeper(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        pPlayer->CLOSE_GOSSIP_MENU();
        instance_ulduar* m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        if (!m_pInstance)
            return true;
        if (m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) == NONE_DIFFICULTY && m_pInstance->GetData(TYPE_LEVIATHAN) == NOT_STARTED)
        {
            m_pInstance->SetData(TYPE_LEVIATHAN_DIFFICULTY, HARD_DIFFICULTY);
            pCreature->SetVisibility(VISIBILITY_OFF);
            if (Creature* pBrannBronzebeard = m_pInstance->GetSingleCreatureFromStorage(NPC_BRANN_BRONZEBEARD))
                pBrannBronzebeard->SetVisibility(VISIBILITY_OFF);
        }
    }
    return true;
}

bool GossipHello_npc_brann_bronzebeard(Player* pPlayer, Creature* pCreature)
{
    instance_ulduar* m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
    if (!m_pInstance)
        return true;

    if (m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) == NONE_DIFFICULTY)
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "We're ready. Begin the assault!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

    pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetObjectGuid());
    return true;
}

bool GossipSelect_npc_brann_bronzebeard(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_INFO_DEF)
    {
        instance_ulduar* m_pInstance = (instance_ulduar*)pCreature->GetInstanceData();
        if (!m_pInstance)
            return true;
        pPlayer->CLOSE_GOSSIP_MENU();

        if (m_pInstance->GetData(TYPE_LEVIATHAN_DIFFICULTY) == NONE_DIFFICULTY && m_pInstance->GetData(TYPE_LEVIATHAN) == NOT_STARTED)
        {
            m_pInstance->SetData(TYPE_LEVIATHAN_DIFFICULTY, EASY_DIFFICULTY);
            pCreature->SetVisibility(VISIBILITY_OFF);
            if (Creature* pKeeperOfNorgannon = m_pInstance->GetSingleCreatureFromStorage(NPC_KEEPER_OF_NORGANNON))
                pKeeperOfNorgannon->SetVisibility(VISIBILITY_OFF);
        }
    }
    return true;
}

void AddSC_boss_leviathan()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "boss_flame_leviathan";
    pNewScript->GetAI = &GetAI_boss_flame_leviathan;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_defense_turret";
    pNewScript->GetAI = &GetAI_mob_defense_turret;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_pool_of_tar";
    pNewScript->GetAI = &GetAI_mob_pool_of_tar;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_mechanolift";
    pNewScript->GetAI = &GetAI_mob_mechanolift;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_freyas_ward";
    pNewScript->GetAI = &GetAI_mob_freyas_ward;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_hodirs_fury";
    pNewScript->GetAI = &GetAI_mob_hodirs_fury;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_mimiron_inferno";
    pNewScript->GetAI = &GetAI_mob_mimirons_inferno;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_thorims_hammer";
    pNewScript->GetAI = &GetAI_mob_thorims_hammer;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "mob_lorekeeper";
    pNewScript->GetAI = &GetAI_mob_lorekeeper;
    pNewScript->pGossipHello = &GossipHello_mob_lorekeeper;
    pNewScript->pGossipSelect = &GossipSelect_mob_lorekeeper;
    pNewScript->RegisterSelf();

    pNewScript = new Script;
    pNewScript->Name = "npc_brann_bronzebeard";
    pNewScript->pGossipHello = &GossipHello_npc_brann_bronzebeard;
    pNewScript->pGossipSelect = &GossipSelect_npc_brann_bronzebeard;
    pNewScript->RegisterSelf();
}
