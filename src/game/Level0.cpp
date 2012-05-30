/*
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
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

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "Player.h"
#include "Opcodes.h"
#include "Chat.h"
#include "ObjectAccessor.h"
#include "Language.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "SystemConfig.h"
#include "revision.h"
#include "revision_nr.h"
#include "revision_R2.h"
#include "Util.h"

bool ChatHandler::HandleHelpCommand(char* args)
{
    if(!*args)
    {
        ShowHelpForCommand(getCommandTable(), "help");
        ShowHelpForCommand(getCommandTable(), "");
    }
    else
    {
        if (!ShowHelpForCommand(getCommandTable(), args))
            SendSysMessage(LANG_NO_CMD);
    }

    return true;
}

bool ChatHandler::HandleCommandsCommand(char* /*args*/)
{
    ShowHelpForCommand(getCommandTable(), "");
    return true;
}

bool ChatHandler::HandleAccountCommand(char* args)
{
    // let show subcommands at unexpected data in args
    if (*args)
        return false;

    AccountTypes gmlevel = GetAccessLevel();
    PSendSysMessage(LANG_ACCOUNT_LEVEL, uint32(gmlevel));
    return true;
}

bool ChatHandler::HandleStartCommand(char* args)
{
    Player *chr = m_session->GetPlayer();

    if(chr->IsTaxiFlying())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    if(chr->isInCombat())
    {
        SendSysMessage(LANG_YOU_IN_COMBAT);
        SetSentErrorMessage(true);
        return false;
    }

	//wuzhu start
	char *otherchar = strtok ((char*)args, " ");

	if (!otherchar)
	{
		// cast spell Stuck
		if(chr->isAlive())//wuzhu
			chr->CastSpell(chr,7355,false);
		//wuzhu 增加复活指令
		else
		{
			if(chr->getRace() == RACE_ORC||chr->getRace()== RACE_UNDEAD||chr->getRace() == RACE_TAUREN||chr->getRace() == RACE_TROLL||chr->getRace() == RACE_BLOODELF)
			{//bl
				chr->TeleportTo(1,	1317	,-4383,	27,	0);//	部落 奥格瑞玛
			}
			else
			{//LM
				chr->TeleportTo(0,	-9065,	434,	94,	0);//	联盟 暴风城
			}
			if(!chr->isAlive())
			{
				chr->ResurrectPlayer(0.5f);
				chr->SpawnCorpseBones();
				chr->SaveToDB();
			}
		}
	}
	else
	{
		std::string strotherchar=otherchar;
		//select guid from characters where account=( SELECT account FROM characters WHERE guid = '271') and name='恶影'
		QueryResult *result = CharacterDatabase.PQuery("select guid from characters where account=( SELECT account FROM characters WHERE guid = '%u') and name='%s'", chr->GetGUIDLow(),strotherchar.c_str());
		sLog.outError("WUZHU OUT:select guid from characters where account=( SELECT account FROM characters WHERE guid = '%u') and name='%s'", chr->GetGUIDLow(),strotherchar.c_str());
		if (!result)
		{
			sLog.outError("WUZHU NOT Result!");
			sWorld.SendWorldText(8218,strotherchar.c_str());
			return true;
		}

		Field *fields = result->Fetch();
		uint32 otherguid = fields[0].GetUInt32();
		delete result;
		if(!otherguid)
		{
			sLog.outError("WUZHU otherguid=0!");
			sWorld.SendWorldText(8218,strotherchar.c_str());
			return true;
		}
		else
		{
			//UPDATE `characters` SET `position_x`='1613.79',`position_y`='-4391.92',`position_z`='10.2909',`map`='1' WHERE (`guid`='271')  
			if(chr->getRace() == RACE_ORC||chr->getRace()== RACE_UNDEAD||chr->getRace() == RACE_TAUREN||chr->getRace() == RACE_TROLL||chr->getRace() == RACE_BLOODELF)
			{//bl	部落 奥格瑞玛
				CharacterDatabase.PExecute("UPDATE `characters` SET `position_x`='1317',`position_y`='-4383',`position_z`='27',`map`='1',`orientation`='0' WHERE `guid`= '%u'", otherguid);
			}
			else
			{//LM 联盟 暴风城
				CharacterDatabase.PExecute("UPDATE `characters` SET `position_x`='-9065',`position_y`='434',`position_z`='94',`map`='0',`orientation`='0' WHERE `guid`= '%u'", otherguid);
			}
			sWorld.SendWorldText(8219,strotherchar.c_str());
		}

	}
	//wuzhu end
    return true;
}

bool ChatHandler::HandleServerInfoCommand(char* /*args*/)
{
    uint32 activeClientsNum = sWorld.GetActiveSessionCount();
    uint32 queuedClientsNum = sWorld.GetQueuedSessionCount();
    uint32 maxActiveClientsNum = sWorld.GetMaxActiveSessionCount();
    uint32 maxQueuedClientsNum = sWorld.GetMaxQueuedSessionCount();
    std::string str = secsToTimeString(sWorld.GetUptime());
    uint32 updateTime = sWorld.GetUpdateTime();

    char const* full;
    char const* fullR2;
    if(m_session)
    {
        full = _FULLVERSION(REVISION_NR);
        fullR2 = _R2FULLVERSION(REVISION_DATE,REVISION_TIME,REVISION_R2,"|cffffffff|Hurl:" REVISION_ID "|h" REVISION_ID "|h|r");
    }
    else
    {
        full = _FULLVERSION(REVISION_NR);
        fullR2 = _R2FULLVERSION(REVISION_DATE,REVISION_TIME,REVISION_R2,REVISION_ID);
    }
    SendSysMessage(full);
    SendSysMessage(fullR2);

    if (sScriptMgr.IsScriptLibraryLoaded())
    {
        char const* ver = sScriptMgr.GetScriptLibraryVersion();
        if (ver && *ver)
            PSendSysMessage(LANG_USING_SCRIPT_LIB, ver);
        else
            SendSysMessage(LANG_USING_SCRIPT_LIB_UNKNOWN);
    }
    else
        SendSysMessage(LANG_USING_SCRIPT_LIB_NONE);

    PSendSysMessage(LANG_USING_WORLD_DB,sWorld.GetDBVersion());
    PSendSysMessage(LANG_USING_EVENT_AI,sWorld.GetCreatureEventAIVersion());
    PSendSysMessage(LANG_CONNECTED_USERS, activeClientsNum, maxActiveClientsNum, queuedClientsNum, maxQueuedClientsNum);
    PSendSysMessage(LANG_UPTIME, str.c_str());
    PSendSysMessage("Update time diff: %u", updateTime);

    return true;
}

bool ChatHandler::HandleDismountCommand(char* /*args*/)
{
    //If player is not mounted, so go out :)
    if (!m_session->GetPlayer( )->IsMounted())
    {
        SendSysMessage(LANG_CHAR_NON_MOUNTED);
        SetSentErrorMessage(true);
        return false;
    }

    if(m_session->GetPlayer( )->IsTaxiFlying())
    {
        SendSysMessage(LANG_YOU_IN_FLIGHT);
        SetSentErrorMessage(true);
        return false;
    }

    m_session->GetPlayer()->Unmount();
    m_session->GetPlayer()->RemoveSpellsCausingAura(SPELL_AURA_MOUNTED);
    return true;
}

bool ChatHandler::HandleSaveCommand(char* /*args*/)
{
    Player *player=m_session->GetPlayer();

    // save GM account without delay and output message (testing, etc)
    if(GetAccessLevel() > SEC_PLAYER)
    {
        player->SaveToDB();
        SendSysMessage(LANG_PLAYER_SAVED);
        return true;
    }

    // save or plan save after 20 sec (logout delay) if current next save time more this value and _not_ output any messages to prevent cheat planning
    uint32 save_interval = sWorld.getConfig(CONFIG_UINT32_INTERVAL_SAVE);
    if (save_interval==0 || (save_interval > 20*IN_MILLISECONDS && player->GetSaveTimer() <= save_interval - 20*IN_MILLISECONDS))
        player->SaveToDB();

    return true;
}

bool ChatHandler::HandleGMListIngameCommand(char* /*args*/)
{
    std::list< std::pair<std::string, bool> > names;

    {
        HashMapHolder<Player>::ReadGuard g(HashMapHolder<Player>::GetLock());
        HashMapHolder<Player>::MapType &m = sObjectAccessor.GetPlayers();
        for (HashMapHolder<Player>::MapType::const_iterator itr = m.begin(); itr != m.end(); ++itr)
        {
            AccountTypes itr_sec = itr->second->GetSession()->GetSecurity();
            if ((itr->second->isGameMaster() || (itr_sec > SEC_PLAYER && itr_sec <= (AccountTypes)sWorld.getConfig(CONFIG_UINT32_GM_LEVEL_IN_GM_LIST))) &&
                (!m_session || itr->second->IsVisibleGloballyFor(m_session->GetPlayer())))
                names.push_back(std::make_pair<std::string, bool>(GetNameLink(itr->second), itr->second->isAcceptWhispers()));
        }
    }

    if (!names.empty())
    {
        SendSysMessage(LANG_GMS_ON_SRV);

        char const* accepts = GetMangosString(LANG_GM_ACCEPTS_WHISPER);
        char const* not_accept = GetMangosString(LANG_GM_NO_WHISPER);
        for(std::list<std::pair< std::string, bool> >::const_iterator iter = names.begin(); iter != names.end(); ++iter)
            PSendSysMessage("%s - %s", iter->first.c_str(), iter->second ? accepts : not_accept);
    }
    else
        SendSysMessage(LANG_GMS_NOT_LOGGED);

    return true;
}

bool ChatHandler::HandleAccountPasswordCommand(char* args)
{
    // allow use from RA, but not from console (not have associated account id)
    if (!GetAccountId())
    {
        SendSysMessage (LANG_RA_ONLY_COMMAND);
        SetSentErrorMessage (true);
        return false;
    }

    // allow or quoted string with possible spaces or literal without spaces
    char *old_pass = ExtractQuotedOrLiteralArg(&args);
    char *new_pass = ExtractQuotedOrLiteralArg(&args);
    char *new_pass_c = ExtractQuotedOrLiteralArg(&args);

    if (!old_pass || !new_pass || !new_pass_c)
        return false;

    std::string password_old = old_pass;
    std::string password_new = new_pass;
    std::string password_new_c = new_pass_c;

    if (password_new != password_new_c)
    {
        SendSysMessage (LANG_NEW_PASSWORDS_NOT_MATCH);
        SetSentErrorMessage (true);
        return false;
    }

    if (!sAccountMgr.CheckPassword (GetAccountId(), password_old))
    {
        SendSysMessage (LANG_COMMAND_WRONGOLDPASSWORD);
        SetSentErrorMessage (true);
        return false;
    }

    AccountOpResult result = sAccountMgr.ChangePassword(GetAccountId(), password_new);

    switch(result)
    {
        case AOR_OK:
            SendSysMessage(LANG_COMMAND_PASSWORD);
            break;
        case AOR_PASS_TOO_LONG:
            SendSysMessage(LANG_PASSWORD_TOO_LONG);
            SetSentErrorMessage(true);
            return false;
        case AOR_NAME_NOT_EXIST:                            // not possible case, don't want get account name for output
        default:
            SendSysMessage(LANG_COMMAND_NOTCHANGEPASSWORD);
            SetSentErrorMessage(true);
            return false;
    }

    // OK, but avoid normal report for hide passwords, but log use command for anyone
    LogCommand(".account password *** *** ***");
    SetSentErrorMessage(true);
    return false;
}

bool ChatHandler::HandleAccountLockCommand(char* args)
{
    // allow use from RA, but not from console (not have associated account id)
    if (!GetAccountId())
    {
        SendSysMessage (LANG_RA_ONLY_COMMAND);
        SetSentErrorMessage (true);
        return false;
    }

    bool value;
    if (!ExtractOnOff(&args, value))
    {
        SendSysMessage(LANG_USE_BOL);
        SetSentErrorMessage(true);
        return false;
    }

    sAccountMgr.LockAccount(GetAccountId(), value);

    PSendSysMessage(value ? LANG_COMMAND_ACCLOCKLOCKED : LANG_COMMAND_ACCLOCKUNLOCKED);

    return true;
}

/// Display the 'Message of the day' for the realm
bool ChatHandler::HandleServerMotdCommand(char* /*args*/)
{
    PSendSysMessage(LANG_MOTD_CURRENT, sWorld.GetMotd());
    return true;
}

//wuzhu start query
bool ChatHandler::HandleWUZHUQuery(char*args)
{
	if(!*args)
	{
		PSendSysMessage(".wq [all]q/c/i/g/cl/gl/ggo/cgo querystr");
		return true;
	}

	char* qtype = strtok((char*)args, " ");
	char* msg = strtok(NULL, "");
	if(!qtype||!msg)
	{
		PSendSysMessage(".wq [all]q/c/i/g/cl/gl/ggo/cgo querystr");
		return true;
	}
	std::string qtstr = qtype;
	std::string qtname= msg;
	QueryResult* qr=NULL;
	uint32 outtype=0;
	if(qtstr=="q")
	{
		qr=WorldDatabase.PQuery("select entry,title_loc1,title_loc4,title_loc5 from locales_quest where title_loc5 like '%s%%' or  title_loc4 like '%s%%' or  title_loc1 like '%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if(qtstr=="c")
	{
		qr=WorldDatabase.PQuery("select entry,name_loc1,name_loc4,name_loc5 from locales_creature where name_loc5 like '%s%%' or name_loc4 like '%s%%' or name_loc1 like '%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if(qtstr=="i")
	{
		qr=WorldDatabase.PQuery("select entry,name_loc1,name_loc4,name_loc5 from locales_item where name_loc5 like '%s%%' or name_loc4 like '%s%%' or name_loc1 like '%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if(qtstr=="g")
	{
		qr=WorldDatabase.PQuery("select entry,name_loc1,name_loc4,name_loc5 from locales_gameobject where name_loc5 like '%s%%' or name_loc4 like '%s%%' or name_loc1 like '%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if (qtstr=="cl")
	{
		qr=WorldDatabase.PQuery("select guid,id,position_x,position_y,position_z,map from creature where id=%s",qtname.c_str());
		outtype=1;
	}
	else
	if (qtstr=="gl")
	{
		qr=WorldDatabase.PQuery("select guid,id,position_x,position_y,position_z,map from gameobject where id=%s",qtname.c_str());
		outtype=1;
	}
	else
	if(qtstr=="cgo")
	{
		qr=WorldDatabase.PQuery("select guid,id,position_x,position_y,position_z,map from creature where id=%s",qtname.c_str());
		outtype=2;
	}
	else
	if(qtstr=="ggo")
	{
		qr=WorldDatabase.PQuery("select guid,id,position_x,position_y,position_z,map from gameobject where id=%s",qtname.c_str());
		outtype=2;
	} 
	else
	if(qtstr=="allq")
	{
		qr=WorldDatabase.PQuery("select entry,title_loc1,title_loc4,title_loc5 from locales_quest where title_loc5 like '%%%s%%' or  title_loc4 like '%%%s%%' or  title_loc1 like '%%%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if(qtstr=="allc")
	{
		qr=WorldDatabase.PQuery("select entry,name_loc1,name_loc4,name_loc5 from locales_creature where name_loc5 like '%%%s%%' or name_loc4 like '%%%s%%' or name_loc1 like '%%%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if(qtstr=="alli")
	{
		qr=WorldDatabase.PQuery("select entry,name_loc1,name_loc4,name_loc5 from locales_item where name_loc5 like '%%%s%%' or name_loc4 like '%%%s%%' or name_loc1 like '%%%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	else
	if(qtstr=="allg")
	{
		qr=WorldDatabase.PQuery("select entry,name_loc1,name_loc4,name_loc5 from locales_gameobject where name_loc5 like '%%%s%%' or name_loc4 like '%%%s%%' or name_loc1 like '%%%s%%'",qtname.c_str(),qtname.c_str(),qtname.c_str());
	}
	if(!qr)
	{
		PSendSysMessage("No Result!!!");
		return true;
	}

	do 
	{
		Field *fields = qr->Fetch();
		if(!fields)
		{
			PSendSysMessage("No Record!!!");
			break;
		}
		if(outtype==0)
			PSendSysMessage("ID:%d %s %s.",fields[0].GetUInt32(),fields[1].GetString(),fields[2].GetString());
		else
		if(outtype==1)
		{
			PSendSysMessage("Guid:%d id:%d [X:%f Y:%f Z:%f M:%d].",fields[0].GetUInt32(),fields[1].GetUInt32(),fields[2].GetFloat(),fields[3].GetFloat(),fields[4].GetFloat(),fields[5].GetInt32());
		}
		else
		if(outtype==2)
		{
			PSendSysMessage("Guid/ID:%d id:%d [X:%f Y:%f Z:%f M:%d].",fields[0].GetUInt32(),fields[1].GetUInt32(),fields[2].GetFloat(),fields[3].GetFloat(),fields[4].GetFloat(),fields[5].GetInt32());
			m_session->GetPlayer()->TeleportTo(fields[5].GetInt32(),fields[2].GetFloat(),fields[3].GetFloat(),fields[4].GetFloat(),0);
			break;
		}
		continue;
	} while (qr->NextRow());

	delete qr;
	return true;

}

//wuzhu start gm
/*
111 	修理选中玩家装备
417 uvalue	修改选中玩家积分	
525	保存选中帐号GM和VIP等级到数据库
922 uvalue	设置选中玩家的VIP
982 	复活选中已经死亡的宠物
2201 fvalue	WUZHU_Player_AURA_Healing
2202 fvalue	WUZHU_Player_TMP_Damage
2203 fvalue	WUZHU_Player_PDA_Damage
2204 fvalue	WUZHU_Pet_TMP_Damage WUZHU_Pet_PDA_Damage
2205 fvalue	WUZHU_Health_Dungeon
2206 fvalue	WUZHU_Health_Raid_10Man
2207 fvalue	WUZHU_Health_Raid_25Man
2208 fvalue	WUZHU_Damage_Dungeon
2209 fvalue	WUZHU_Damage_Raid_10Man
2210 fvalue	WUZHU_Damage_Raid_25Man
8209 uvalue	设置选中玩家的GM			
9999	输出信息
*/
bool ChatHandler::HandleWUZHUGM(char*args)
{
	if(!*args)
		m_session->GetPlayer()->GetSession()->SendNotification("Dev.QQ:85267512");	
	else
	{
		uint32 operid;
		if (!ExtractUInt32(&args, operid))
			return false;
		float opervalue;
		if (!ExtractFloat(&args, opervalue))
		{
			opervalue = 1.0f;
		}
		int32 operuvalue=(uint32)opervalue;
		Player *selplayer=getSelectedPlayer();
		switch(operid)
		{
		case 111:
			//修改装备
			if(!selplayer)
				m_session->GetPlayer()->GetSession()->SendNotification("Not selected player.");	
			else
			{
				selplayer->DurabilityRepairAll(true,1,false);
				m_session->GetPlayer()->GetSession()->SendNotification("%s DurabilityRepairAll!",selplayer->GetName());
			}
		case 417:
			if(!selplayer)
				m_session->GetPlayer()->GetSession()->SendNotification("Not selected player.");	
			else
			{
				selplayer->ModifyVIP_Integral(operuvalue);
				m_session->GetPlayer()->GetSession()->SendNotification("%s Modify Integral:%d",selplayer->GetName(),operuvalue);	
			}
			return true;
		case 525:
			if(!selplayer)
				m_session->GetPlayer()->GetSession()->SendNotification("Not selected player.");	
			else
			{

				LoginDatabase.PExecute( "UPDATE account SET  gmlevel ='%d' WHERE id = '%d'",(int)selplayer->GetSession()->GetSecurity(),selplayer->GetSession()->GetAccountId());
				LoginDatabase.PExecute( "UPDATE account SET  vip ='%d' WHERE id = '%d'",selplayer->GetSession()->GetPlayer()->GetVIP(),selplayer->GetSession()->GetAccountId());
				m_session->GetPlayer()->GetSession()->SendNotification("%s GM:%d  VIP:%d",selplayer->GetName(),(int)selplayer->GetSession()->GetSecurity(),selplayer->GetSession()->GetPlayer()->GetVIP());	
			}
			return true;
		case 922:
			if(!selplayer)
				m_session->GetPlayer()->GetSession()->SendNotification("Not selected player.");	
			else
			{
				selplayer->SetVIP(operuvalue);		
				m_session->GetPlayer()->GetSession()->SendNotification("%s set VIP:%d",selplayer->GetName(),operuvalue);
			}
			return true;
		case 982:
			{
				//复活宠物
				Creature *sel=getSelectedCreature();
				if(!sel)
				{
					m_session->GetPlayer()->GetSession()->SendNotification("Not selected units!");	
					return false;
				}
				if(!sel->IsPet())
				{
					m_session->GetPlayer()->GetSession()->SendNotification("Select units are not pets!");	
					return false;
				}
				Pet *selpet=(Pet *)sel;
				if(selpet->getPetType()!=HUNTER_PET)
				{
					m_session->GetPlayer()->GetSession()->SendNotification("Select the pet is not a Hunter Pet!");	
					return false;
				}
				if(!selpet->isDead())
				{
					m_session->GetPlayer()->GetSession()->SendNotification("Select the pet is not Dead!");	
					return false;
				}
				selpet->SetDeathState(ALIVE);
				selpet->SetHealth(1);
				return true;
			}
		case 2201:
			sWorld.setWUZHUConfig(WUZHU_Player_AURA_Healing,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("AURA_Healing:%0.2f",opervalue);
			return true;
		case 2202:
			sWorld.setWUZHUConfig(WUZHU_Player_TMP_Damage,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("TMP_Damage:%0.2f",opervalue);
			return true;
		case 2203:
			sWorld.setWUZHUConfig(WUZHU_Player_PDA_Damage,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("PDA_Damage:%0.2f",opervalue);
			return true;
		case 2204:
			sWorld.setWUZHUConfig(WUZHU_Pet_TMP_Damage,opervalue);
			sWorld.setWUZHUConfig(WUZHU_Pet_PDA_Damage,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("Pet_Damage:%0.2f",opervalue);
			return true;
		case 2205:
			//omaxhealth=m_session->GetPlayer()->GetMaxHealth()/sWorld.getWUZHUConfig(WUZHU_Health);
			sWorld.setWUZHUConfig(WUZHU_Health_Dungeon,opervalue);
			//m_session->GetPlayer()->SetMaxHealth(omaxhealth*lev);
			m_session->GetPlayer()->GetSession()->SendNotification("Health Dungeon:%0.2f",opervalue);
			return true;
		case 2206:
			sWorld.setWUZHUConfig(WUZHU_Health_Raid_10Man,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("Health Raid_10Man:%0.2f",opervalue);
			return true;
		case 2207:
			sWorld.setWUZHUConfig(WUZHU_Health_Raid_25Man,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("Health Raid_25Man:%0.2f",opervalue);
			return true;
		case 2208:
			sWorld.setWUZHUConfig(WUZHU_Damage_Dungeon,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("WUZHU_Damage_Dungeon:%0.2f",opervalue);
			return true;
		case 2209:
			sWorld.setWUZHUConfig(WUZHU_Damage_Raid_10Man,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("WUZHU_Damage_Raid_10Man:%0.2f",opervalue);
			return true;
		case 2210:
			sWorld.setWUZHUConfig(WUZHU_Damage_Raid_25Man,opervalue);
			m_session->GetPlayer()->GetSession()->SendNotification("WUZHU_Damage_Raid_25Man:%0.2f",opervalue);
			return true;
		case 8209: 
			if(operuvalue>4)
				operuvalue=4;
			if(!selplayer)
				m_session->GetPlayer()->GetSession()->SendNotification("Not selected player.");	
			else
			{
				selplayer->GetSession()->SetSecurity(AccountTypes(operuvalue));
				m_session->GetPlayer()->GetSession()->SendNotification("%s set GM:%d",selplayer->GetName(),operuvalue);	
			}
			return true;
		case 9999:
			Player *player=m_session->GetPlayer();
			PSendSysMessage("IsDungeon=%d IsRaid=%d",player->GetMap()->IsDungeon(),player->GetMap()->IsRaid());
			PSendSysMessage("MaxPlayer=%d DamageRate=%0.2f",player->GetMap()->GetMaxPlayers(),player->WUZHU_GetDamageRate());
			break;
		}
		m_session->GetPlayer()->GetSession()->SendNotification("Dev.wuzhu.QQ:85267512");	
		
	}
	
	return true;
}
//wuzhu end

//wuzhu start 世界聊天
bool ChatHandler::HandleWorldCast(char* args)
{
	uint32 sjchatmoney=1000;
	if(!*args)
		return false;
	if(m_session->GetPlayer()->GetMoney()<sjchatmoney)//判断金钱是否满足
	{
		m_session->GetPlayer()->GetSession()->SendNotification("Your money less than %d!",sjchatmoney);
		return false;
	}	
	sWorld.SendWorldText(8209,m_session->GetPlayerName(),args);
	m_session->GetPlayer()->ModifyMoney(0-sjchatmoney);
	m_session->GetPlayer()->GetSession()->SendNotification(" 鏃犱富榄斿吔涓栫晫棰戦亾骞挎挱宸茬粡鍙戦�?鎵ｅ彇璐圭敤%d閾滃竵!",sjchatmoney);//无主魔兽世界频道广播已经发送,扣取费用%d铜币!
	return true;
}//wuzhu end