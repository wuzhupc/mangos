/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_GOSSIP_H
#define SC_GOSSIP_H

#include "Player.h"
#include "GossipDef.h"
#include "QuestDef.h"

#define WUZHU_TEXT_SOURCE_RANGE -6000000//wuzhu

// Gossip Item Text
#define GOSSIP_TEXT_BROWSE_GOODS        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-259,pPlayer))//"我想看看你的商品. "//"I'd like to browse your goods."
#define GOSSIP_TEXT_TRAIN               (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-260,pPlayer))//"训练我! "//"Train me!"

#define GOSSIP_TEXT_BANK (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-261,pPlayer))//"银行 "
#define GOSSIP_TEXT_IRONFORGE_BANK      (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-262,pPlayer))//"铁炉堡银行 "
#define GOSSIP_TEXT_STORMWIND_BANK      (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-263,pPlayer))//"暴风城银行 "
#define GOSSIP_TEXT_WINDRIDER (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-264,pPlayer))//"双足飞龙管理员 "
#define GOSSIP_TEXT_GRYPHON             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-265,pPlayer))//"狮鹫管理员 "
#define GOSSIP_TEXT_BATHANDLER          (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-266,pPlayer))//"蝙蝠管理员 "
#define GOSSIP_TEXT_HIPPOGRYPH          (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-267,pPlayer))//"角鹰兽管理员 "
#define GOSSIP_TEXT_ZEPPLINMASTER       (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-268,pPlayer))//"飞艇管理员 "
#define GOSSIP_TEXT_DEEPRUNTRAM         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-269,pPlayer))//"地铁 "
#define GOSSIP_TEXT_FERRY               (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-270,pPlayer))//"鲁瑟兰村码头 "
#define GOSSIP_TEXT_FLIGHTMASTER        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-271,pPlayer))//"飞行管理员 "
#define GOSSIP_TEXT_AUCTIONHOUSE        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-272,pPlayer))//"拍卖行 "
#define GOSSIP_TEXT_GUILDMASTER         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-273,pPlayer))//"公会注册管理员 "
#define GOSSIP_TEXT_INN                 (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-274,pPlayer))//"旅店 "
#define GOSSIP_TEXT_MAILBOX             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-275,pPlayer))//"邮箱 "
#define GOSSIP_TEXT_STABLEMASTER        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-276,pPlayer))//"宠物管理员 "
#define GOSSIP_TEXT_WEAPONMASTER        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-277,pPlayer))//"武器大师 "
#define GOSSIP_TEXT_OFFICERS            (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-278,pPlayer))//"军官休息室 "
#define GOSSIP_TEXT_BATTLEMASTER        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-279,pPlayer))//"战场军官 "
#define GOSSIP_TEXT_BARBER              (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-280,pPlayer))//"理发师 "
#define GOSSIP_TEXT_CLASSTRAINER        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-281,pPlayer))//"种族训练师 "
#define GOSSIP_TEXT_PROFTRAINER         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-282,pPlayer))//"职业训练师 "
#define GOSSIP_TEXT_LEXICON             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-283,pPlayer))//"魔法力量宝典 "

#define GOSSIP_TEXT_ALTERACVALLEY       (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-284,pPlayer))//"奥特兰克山谷 "
#define GOSSIP_TEXT_ARATHIBASIN         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-285,pPlayer))//"阿拉希盆地 " 
#define GOSSIP_TEXT_WARSONGULCH         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-286,pPlayer))//"战歌峡谷 "
#define GOSSIP_TEXT_ARENA               (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-287,pPlayer))//"竞技场 "
#define GOSSIP_TEXT_EYEOFTHESTORM       (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-288,pPlayer))//"风暴之眼 "
#define GOSSIP_TEXT_STRANDOFANCIENT     (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-289,pPlayer))//"远祖滩头"//"Strand of the Ancients"

#define GOSSIP_TEXT_DEATH_KNIGHT        (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-290,pPlayer))//"死亡骑士"
#define GOSSIP_TEXT_DRUID               (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-291,pPlayer))//"德鲁伊 " 
#define GOSSIP_TEXT_HUNTER              (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-292,pPlayer))//"猎人 "
#define GOSSIP_TEXT_PRIEST              (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-293,pPlayer))//"牧师 "
#define GOSSIP_TEXT_ROGUE               (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-294,pPlayer))//"盗贼 "
#define GOSSIP_TEXT_WARRIOR             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-295,pPlayer))//"战士 "
#define GOSSIP_TEXT_PALADIN             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-296,pPlayer))//"圣骑士 "
#define GOSSIP_TEXT_SHAMAN              (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-297,pPlayer))//"萨满祭司 "
#define GOSSIP_TEXT_MAGE                (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-298,pPlayer))//"法师 "
#define GOSSIP_TEXT_WARLOCK             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-299,pPlayer))//"术土 "

#define GOSSIP_TEXT_ALCHEMY             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-301,pPlayer))//"炼金术 "
#define GOSSIP_TEXT_BLACKSMITHING       (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-302,pPlayer))//"锻造 "
#define GOSSIP_TEXT_COOKING             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-303,pPlayer))//"烹饪 "
#define GOSSIP_TEXT_ENCHANTING          (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-304,pPlayer))//"附魔 "
#define GOSSIP_TEXT_ENGINEERING         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-305,pPlayer))//"工程学 "
#define GOSSIP_TEXT_FIRSTAID            (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-306,pPlayer))//"急救 "
#define GOSSIP_TEXT_HERBALISM           (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-307,pPlayer))//"草药学 "
#define GOSSIP_TEXT_LEATHERWORKING      (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-308,pPlayer))//"制皮 "
#define GOSSIP_TEXT_TAILORING           (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-309,pPlayer))//"裁缝 "
#define GOSSIP_TEXT_MINING              (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-310,pPlayer))//"采矿 "
#define GOSSIP_TEXT_FISHING             (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-311,pPlayer))//"钓鱼 "
#define GOSSIP_TEXT_SKINNING            (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-312,pPlayer))//"剥皮 "
#define GOSSIP_TEXT_JEWELCRAFTING       (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-313,pPlayer))//"珠宝加工 "
#define GOSSIP_TEXT_INSCRIPTION         (pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-314,pPlayer))//"铭文"//"Inscription"

enum
{
    // Skill defines
    TRADESKILL_ALCHEMY             = 1,
    TRADESKILL_BLACKSMITHING       = 2,
    TRADESKILL_COOKING             = 3,
    TRADESKILL_ENCHANTING          = 4,
    TRADESKILL_ENGINEERING         = 5,
    TRADESKILL_FIRSTAID            = 6,
    TRADESKILL_HERBALISM           = 7,
    TRADESKILL_LEATHERWORKING      = 8,
    TRADESKILL_POISONS             = 9,
    TRADESKILL_TAILORING           = 10,
    TRADESKILL_MINING              = 11,
    TRADESKILL_FISHING             = 12,
    TRADESKILL_SKINNING            = 13,
    TRADESKILL_JEWLCRAFTING        = 14,
    TRADESKILL_INSCRIPTION         = 15,

    TRADESKILL_LEVEL_NONE          = 0,
    TRADESKILL_LEVEL_APPRENTICE    = 1,
    TRADESKILL_LEVEL_JOURNEYMAN    = 2,
    TRADESKILL_LEVEL_EXPERT        = 3,
    TRADESKILL_LEVEL_ARTISAN       = 4,
    TRADESKILL_LEVEL_MASTER        = 5,
    TRADESKILL_LEVEL_GRAND_MASTER  = 6,

    // Gossip defines
    GOSSIP_ACTION_TRADE            = 1,
    GOSSIP_ACTION_TRAIN            = 2,
    GOSSIP_ACTION_TAXI             = 3,
    GOSSIP_ACTION_GUILD            = 4,
    GOSSIP_ACTION_BATTLE           = 5,
    GOSSIP_ACTION_BANK             = 6,
    GOSSIP_ACTION_INN              = 7,
    GOSSIP_ACTION_HEAL             = 8,
    GOSSIP_ACTION_TABARD           = 9,
    GOSSIP_ACTION_AUCTION          = 10,
    GOSSIP_ACTION_INN_INFO         = 11,
    GOSSIP_ACTION_UNLEARN          = 12,
    GOSSIP_ACTION_INFO_DEF         = 1000,

    GOSSIP_SENDER_MAIN             = 1,
    GOSSIP_SENDER_INN_INFO         = 2,
    GOSSIP_SENDER_INFO             = 3,
    GOSSIP_SENDER_SEC_PROFTRAIN    = 4,
    GOSSIP_SENDER_SEC_CLASSTRAIN   = 5,
    GOSSIP_SENDER_SEC_BATTLEINFO   = 6,
    GOSSIP_SENDER_SEC_BANK         = 7,
    GOSSIP_SENDER_SEC_INN          = 8,
    GOSSIP_SENDER_SEC_MAILBOX      = 9,
    GOSSIP_SENDER_SEC_STABLEMASTER = 10
};

extern uint32 GetSkillLevel(Player* pPlayer, uint32 uiSkill);

// Defined fuctions to use with player.

// This fuction add's a menu item,
// Icon Id
// Text
// Sender(this is to identify the current Menu with this item)
// Option id (identifies this Menu Item)
// Text to be displayed in pop up box
// Money value in pop up box
// Coded
#define ADD_GOSSIP_ITEM(uiIcon, chrText, uiSender, uiOptionId)   PlayerTalkClass->GetGossipMenu().AddMenuItem(uiIcon, chrText, uiSender, uiOptionId, "", 0)
#define ADD_GOSSIP_ITEM_ID(uiIcon, iTextId, uiSender, uiOptionId)   PlayerTalkClass->GetGossipMenu().AddMenuItem(uiIcon, iTextId, uiSender, uiOptionId, 0, 0)
#define ADD_GOSSIP_ITEM_EXTENDED(uiIcon, chrText, uiSender, uiOptionId, chrBoxMessage, uiBoxMoney, bCode)   PlayerTalkClass->GetGossipMenu().AddMenuItem(uiIcon, chrText, uiSender, uiOptionId, chrBoxMessage, uiBoxMoney, bCode)

// This fuction Sends the current menu to show to client
// uiTextId - NPCTEXTID (uint32)
// guid - npc guid (ObjectGuid)
#define SEND_GOSSIP_MENU(uiTextId, guid)      PlayerTalkClass->SendGossipMenu(uiTextId, guid)

// Closes the Menu
#define CLOSE_GOSSIP_MENU()        PlayerTalkClass->CloseGossip()

// Fuctions to send NPC lists
// a - is always the npc guid (ObjectGuid)
#define SEND_VENDORLIST(a)         GetSession()->SendListInventory(a)
#define SEND_TRAINERLIST(a)        GetSession()->SendTrainerList(a)
#define SEND_BANKERLIST(a)         GetSession()->SendShowBank(a)
#define SEND_TABARDLIST(a)         GetSession()->SendTabardVendorActivate(a)
#define SEND_TAXILIST(a)           GetSession()->SendTaxiStatus(a)

#endif
