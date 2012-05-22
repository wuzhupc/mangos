#include "precompiled.h"
#include "sc_gossip.h"
#include "SharedDefines.h"
#include "Item.h"
#include "sc_creature.h"
#include "SpellMgr.h"
#include "Spell.h"
#include "WUZHU.h"
#include "Timer.h"
#include "../../system/system.h"

/*
部落
1~10  
  血精灵	紫色陆行鸟		http://db.duowan.com/wow/item-29222.html	35018
  牛头人	灰色科多兽		http://db.duowan.com/wow/item-15277.html	18989   
  兽人		暗棕狼号角		http://db.duowan.com/wow/item-5668.html		6654
  亡灵		蓝色骸骨军马	http://db.duowan.com/wow/item-13332.html	17463
  巨魔		紫色迅猛龙之哨	http://db.duowan.com/wow/item-8592.html		10799
10~20
  血精灵	迅捷粉色陆行鸟	http://db.duowan.com/wow/item-28936.html	33660
  牛头人	大型白色科多兽	http://db.duowan.com/wow/item-18793.html	23247
  兽人		迅捷森林狼号角	http://db.duowan.com/wow/item-18797.html	23251
  亡灵		紫色骷髅战马	http://db.duowan.com/wow/item-18791.html	23246
  巨魔		迅捷橙色迅猛龙	http://db.duowan.com/wow/item-18790.html	23243
20~40公用 黑色军用迅猛龙哨 http://db.duowan.com/wow/item-29472.html	22721
40~60 蓝色驭风者 http://db.duowan.com/wow/item-25475.html				32244
60~70 黑色灵翼幼龙的缰绳http://db.duowan.com/wow/item-32857.html		41513 
70凤凰/虚空幼龙														37015 32345

联盟
1~10  
人类	栗色马缰绳		http://db.duowan.com/wow/item-5655.html		6648
矮人	白山羊			http://db.duowan.com/wow/item-5873.html		6898   
侏儒	红色机械陆行鸟	http://db.duowan.com/wow/item-8563.html		10873	
精灵	条纹夜刃豹缰绳	http://db.duowan.com/wow/item-8629.html		10793
德莱尼	紫色雷象		http://db.duowan.com/wow/item-29743.html	35711

10~20
人类	迅捷白马		http://db.duowan.com/wow/item-18778.html	23228
矮人	迅捷白山羊		http://db.duowan.com/wow/item-18785.html	23240 
侏儒	迅捷白色机械陆行鸟	http://db.duowan.com/wow/item-18773.html	23223
精灵	迅捷霜刃豹缰绳	http://db.duowan.com/wow/item-18766.html	23221
德莱尼	重型紫色雷象	http://db.duowan.com/wow/item-29747.html	35714
20~40公用 黑色军用座虎缰绳 http://db.duowan.com/wow/item-18766.html		22723
40~60  雪白狮鹫兽 http://db.duowan.com/wow/item-25472.html			32240
60~70 红色骑乘虚空鳐 http://db.duowan.com/wow/item-32317.html		39800	
70凤凰/虚空幼龙													37015 32345
*/ 


bool GossipHello_Item(Player *pPlayer, Item* _Item, SpellCastTargets const& targets)
{ 
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-55,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
		//return true;
	}
	if(pPlayer->IsTaxiFlying()||pPlayer->IsMounted())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-56,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
		//return false;
	}
	else
	{
		if (!pPlayer->isDead())
		{ 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-57,pPlayer),1,GOSSIP_ACTION_INFO_DEF + 1);
			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-328,pPlayer),1,GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-58,pPlayer),1,GOSSIP_ACTION_INFO_DEF + 2);			
			if(pPlayer->getLevel()<=20)
			switch(pPlayer->getRace())
			{
			case RACE_HUMAN:
				if(pPlayer->getLevel()<=10)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-59,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_RL1);
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-60,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_RL20);
				break;
			case RACE_ORC:
				if(pPlayer->getLevel()<=10)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-61,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_SR1);
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-62,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_SR20);
				break;
			case RACE_DWARF:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-63,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_AR1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-64,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_AR20);
				break;
			case RACE_NIGHTELF:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-65,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_JL1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-66,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_JL20);
				break;
			case RACE_UNDEAD:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-67,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_WN1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-68,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_WN20);
				break;
			case RACE_TAUREN:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-69,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_NTR1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-70,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_NTR20);
				break;
			case RACE_GNOME:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-71,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_ZR1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-72,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_ZR20);
				break;
			case RACE_TROLL:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-73,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_JM1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-74,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_JM20);
				break;
			case RACE_BLOODELF:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-75,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_XJN1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-76,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL_XJN20);
				break;
			case RACE_DRAENEI:
				if(pPlayer->getLevel()<=10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-77,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_DLN1);
				else
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-78,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM_DLN20);
				break;
			}
				//0~20
				//21~40 1s40 1t40 1s60
				//41~60 1s40 1t40 1t60 1s60
				//>61 1t60 1s60 1t80 1s80
				WUZHU_FLY_DATA *S40=NULL;
				WUZHU_FLY_DATA *S60=NULL;
				WUZHU_FLY_DATA *S80=NULL;
				WUZHU_FLY_DATA *T40=NULL;
				WUZHU_FLY_DATA *T60=NULL;
				WUZHU_FLY_DATA *T80=NULL;
				if(pPlayer->getLevel()>=21&&pPlayer->getLevel()<=40)
				{
					S40=pSystemMgr.GetFlyData(40,0);					
					S60=pSystemMgr.GetFlyData(60,0);
					T40=pSystemMgr.GetFlyData(40,1);					
				}
				if(pPlayer->getLevel()>=41&&pPlayer->getLevel()<=60)
				{
					S40=pSystemMgr.GetFlyData(40,0);
					S60=pSystemMgr.GetFlyData(60,0);
					T40=pSystemMgr.GetFlyData(40,1);
					T60=pSystemMgr.GetFlyData(60,1);
				}
				if(pPlayer->getLevel()>=61&&pPlayer->getLevel()<=70)
				{
					S60=pSystemMgr.GetFlyData(60,0);
					S80=pSystemMgr.GetFlyData(80,0);
					T40=pSystemMgr.GetFlyData(40,1);
					T60=pSystemMgr.GetFlyData(60,1);
				}
				if(pPlayer->getLevel()>=71||pPlayer->isGameMaster())
				{
					S60=pSystemMgr.GetFlyData(60,0);
					S80=pSystemMgr.GetFlyData(80,0);
					T60=pSystemMgr.GetFlyData(60,1);
					T80=pSystemMgr.GetFlyData(80,1);
				}

				if(S40)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,"|cff8000FF"+S40->name+"|r",170,GOSSIP_ACTION_INFO_DEF + S40->spell);
				}
				if(S60)
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,"|cff800099"+S60->name+"|r",170,GOSSIP_ACTION_INFO_DEF + S60->spell);
				}
				if(S80)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,"|cff800066"+S80->name+"|r",170,GOSSIP_ACTION_INFO_DEF + S80->spell);

				if(T40)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,"|cff800055"+T40->name+"|r",170,GOSSIP_ACTION_INFO_DEF + T40->spell);
				if(T60)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,"|cff800044"+T60->name+"|r",170,GOSSIP_ACTION_INFO_DEF + T60->spell);
				if(T80)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,"|cff800022"+T80->name+"|r",170,GOSSIP_ACTION_INFO_DEF + T80->spell);
				/*if(isbl(pPlayer))
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-79,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL20);
					if(pPlayer->getLevel()>=30&&pPlayer->getLevel()<50)
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-80,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL40);
					if(pPlayer->getLevel()>=50)
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-81,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_BL60);
				}
				else
				{					
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-82,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM20);
					if(pPlayer->getLevel()>=30&&pPlayer->getLevel()<50)
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-83,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM40);
					if(pPlayer->getLevel()>=50)
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-84,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LM60);
				}*/
			//else
			//{
			//	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-85,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LMBL70_1);
			//	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-86,pPlayer),110,GOSSIP_ACTION_INFO_DEF + SFM_LMBL70_2);
			//}
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-87,pPlayer),1,GOSSIP_ACTION_INFO_DEF+3);
			//if(pPlayer->isGameMaster()||pPlayer->getLevel()<=60)
			//{
			//	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-88,pPlayer),90,GOSSIP_ACTION_INFO_DEF);//领取在线工资			
			//}
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-89,pPlayer),40,GOSSIP_ACTION_INFO_DEF);			
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-90,pPlayer),20,GOSSIP_ACTION_INFO_DEF);//银行
			if(pPlayer->GetGuildId()&&pPlayer->isGameMaster())
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-345,pPlayer),21,GOSSIP_ACTION_INFO_DEF);//公会银行				
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-91,pPlayer), 22, GOSSIP_ACTION_INFO_DEF + 22);//绑定

		}
		else 
			if (pPlayer->isDead())
			{
				uint32 v_map = pPlayer->GetMapId();				
				if(pPlayer->isGameMaster()||pPlayer->GetVIP()==5||v_map == 530 || v_map == 571||v_map == 0||v_map == 1)
					pPlayer-> GetSession()->SendSpiritResurrect();
					//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-92,pPlayer), 130, GOSSIP_ACTION_INFO_DEF + 130); //灵魂医者复活 
			}
		pPlayer->SEND_GOSSIP_MENU(90000,_Item->GetObjectGuid());
	}

	return true;
}

void SendDefaultMenu_Item(Player *pPlayer, Item *_Item, uint32 action)
{
	switch(action) {
		// 主城
				case GOSSIP_ACTION_INFO_DEF + 1 :
					if(isbl(pPlayer)||(pPlayer->GetVIP()==6&&!isbl(pPlayer)))
					{
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-93,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 4,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-94,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 5,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-95,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 6,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-96,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 11,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						if(pPlayer->getLevel()>=68)
						{
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-321,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 13,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-320,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 14,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						}

					}
					else
					{
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-97,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 1,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-98,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 2,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-99,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 3,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-100,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 12,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						if(pPlayer->getLevel()>=68)
						{
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-323,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 16,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-322,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 17,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
						}
					}
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-101,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 7,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-102,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 8,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-103,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 9,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-104,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 10,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-105,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 7210,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					if(pPlayer->getLevel()<71)
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-106,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 7700,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-107,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 15,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-108,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 715,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					if(pPlayer->getLevel()>=71)
					{
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-324,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 714,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_MainCity,false);
					}
					//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<<<返回首页>>>  ", 140, GOSSIP_ACTION_INFO_DEF);
					pPlayer->SEND_GOSSIP_MENU(90001,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+2://商店
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-109,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 200);					
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-110,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 1);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-111,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 2);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-112,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-113,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 4);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-114,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 5);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-115,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 6);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-116,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 7);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-117,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 8);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-118,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 9);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-119,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 10);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-120,pPlayer), 70, GOSSIP_ACTION_INFO_DEF + 11);					
					pPlayer->SEND_GOSSIP_MENU(90002,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+3://积分
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-6,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 7);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-7,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 4);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-8,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 5);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-9,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 6);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-10,pPlayer), 160, GOSSIP_ACTION_INFO_DEF);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-11,pPlayer), 150, GOSSIP_ACTION_INFO_DEF);
					pPlayer->SEND_GOSSIP_MENU(90003,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+4://物品兑换积分 一公正徽章换一千积分
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-16,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 1);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-17,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 2);
					//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-19,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-20,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 4);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-131,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->SEND_GOSSIP_MENU(90004,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+5://积分购买荣誉点数
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-22,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 1);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-23,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 2);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-24,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-131,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->SEND_GOSSIP_MENU(90005,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+6://积分购买技能点数
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-121,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 1);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-122,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 2);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-123,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-124,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 4);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-125,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 5);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-126,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 6);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-127,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 7);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-128,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 8);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-129,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 9);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-130,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 10);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-132,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 11);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-133,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 12);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-134,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 13);
					if(pPlayer->getClass()==CLASS_ROGUE)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-135,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 21);
					}
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-136,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 22);					
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-131,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->SEND_GOSSIP_MENU(90006,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+7://积分升级
					if(pPlayer->getLevel()>=80)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-42,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
					}
					else
					{
						//升级所需积分：(升级后等级-目前等级)*(70-目前等级)*1000						
						uint32 nowlevel=pPlayer->getLevel();
						uint32 newlevel=nowlevel+1;
						uint32 needintegral=(nowlevel+1)*UPLevelIntegral;//需要的积分
						std::string str=pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-43,pPlayer);
						char *tempchs=new char[8];
						itoa(needintegral,tempchs,10);
						str+=tempchs;
						str+=pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-44,pPlayer);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, str.c_str(), 154, GOSSIP_ACTION_INFO_DEF);

					}
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-131,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
					pPlayer->SEND_GOSSIP_MENU(90007,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+8:
					if(pPlayer->getLevel()>=80)
					{
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-329,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 5191,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Extent_Boss_Copy,false);
						pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-331,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 5192,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Extent_Boss_Copy,false);
					}
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-330,pPlayer), 999, GOSSIP_ACTION_INFO_DEF);
					pPlayer->SEND_GOSSIP_MENU(90008,_Item->GetObjectGuid());
					break;
				case GOSSIP_ACTION_INFO_DEF+22://绑定二级菜单
					{					
					//读取数据库数据							0	1		2			3			4			5	6		7
					//result = SD2Database.PQuery("SELECT id, guid, position_x, position_y, position_z, map, Zone, maptext FROM wuzhu_bind where guid=%u order by id",pPlayer->GetGUID());
					QueryResult* result=pPlayer->WUZHU_GetBind();
					int recount=0;
					if (result)
					{
						do 
						{
							recount++;
							Field *fields = result->Fetch();
							if(fields[0].GetInt32()<recount)
								continue;
							if(fields[0].GetInt32()>recount)
							{
								if(fields[0].GetInt32()<=Max_BindCount)
								{
									for(int i=recount;i<fields[0].GetInt32();i++)
									{
										pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-337,pPlayer), 23, GOSSIP_ACTION_INFO_DEF+220 + i);
									}
									recount=fields[0].GetInt32();
								}
								else
								{
									for(int i=recount;i<=Max_BindCount;i++)
									{
										pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-337,pPlayer), 23, GOSSIP_ACTION_INFO_DEF+220 + i);
									}
									recount=Max_BindCount;
									break;
								}

							}
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, fields[7].GetString(), 23, GOSSIP_ACTION_INFO_DEF+220 + recount);


						} while (result->NextRow()&&recount<Max_BindCount);

						delete result;
					}	
					//如果不足10个
					for(int i=recount+1;i<=Max_BindCount;i++)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-337,pPlayer), 23, GOSSIP_ACTION_INFO_DEF+220 + i);						
					}
					pPlayer->SEND_GOSSIP_MENU(90022,_Item->GetObjectGuid());
					}
					break;
				case GOSSIP_ACTION_INFO_DEF+200://显示关于商场
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-137,pPlayer), 160, GOSSIP_ACTION_INFO_DEF);
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-131,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 22);
					pPlayer->SEND_GOSSIP_MENU(90200,_Item->GetObjectGuid());
					break;
	}
	if(action>GOSSIP_ACTION_INFO_DEF+220&&action<=GOSSIP_ACTION_INFO_DEF+220+Max_BindCount)//绑定三级菜单
	{
		int index=action-GOSSIP_ACTION_INFO_DEF-220;//这个就是索引值
		if(index>0&&index<=Max_BindCount)
		{
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-338,pPlayer), 24, GOSSIP_ACTION_INFO_DEF+index);//绑定
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-339,pPlayer), 25, GOSSIP_ACTION_INFO_DEF+index);//传送
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-340,pPlayer), 26, GOSSIP_ACTION_INFO_DEF+index);//删除
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-346,pPlayer), 27, GOSSIP_ACTION_INFO_DEF+index);//位置查看
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "             ", 23, action);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-131,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 22);

		}
		pPlayer->SEND_GOSSIP_MENU(90023,_Item->GetObjectGuid());
	}
}

bool GossipWUZHUItemSelect(Player *pPlayer, Item *_Item, uint32 sender, uint32 action )
{
	if (!isstone(pPlayer))
	{
		switch(sender) {
			// 主选单
				case GOSSIP_SENDER_MAIN :
				case 22://绑定2级菜单
				case 23://绑定3级菜单
					SendDefaultMenu_Item(pPlayer, _Item, action);
					break;
					//传送 
				case 5 :
					switch(action) 
					{
						/*统一格式说明：(MapID, X, Y, Z, 0);

						case GOSSIP_ACTION_INFO_DEF + 9 :
						pPlayer->TeleportTo(533,3700.35,-5185.92,143.957,4.403038,0);
						break;
						*/
					case GOSSIP_ACTION_INFO_DEF +1://	联盟 暴风城
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
							pPlayer->TeleportTo(0,	-9065.0f,	434.0f,	94.0f,	0.0f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +2://	联盟 铁炉堡
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(0,	-5032.0f,	-819.0f,	495.0f,	0.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +3://	联盟 达纳苏斯	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(1,	9961.0f,	2055.0f,	1329.0f,	0.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +4://	部落 奥格瑞玛	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(1,	1317.0f	,-4383.0f,	27.0f,	0.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +5://	部落 雷霆崖	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(1,	-1391.0f,	140.0f,	23,	0.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +6://	部落 幽暗城	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(0,	1909.0f,	235.0f,	53.0f,	0.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +7://	中立 棘齿城	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{pPlayer->TeleportTo(1,	-977.0f,	-3788.0f,	6.0f,	0.0f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}						
						break;
					case GOSSIP_ACTION_INFO_DEF +8://	中立 藏宝海湾	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{pPlayer->TeleportTo(0,	-14302.0f,	518.0f,	9.0f,	0.0f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}						
						break;
					case GOSSIP_ACTION_INFO_DEF +9://	中立 冬泉谷	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{pPlayer->TeleportTo(1,	6723.47f,	-4649.3f,	720.981f,	4.65348f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}						
						break;
					case GOSSIP_ACTION_INFO_DEF +10://	中立 加基森	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(1,	-7177.1f	,-3788.08f,	8.37018f,	3.62134f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +11://	部落 银月城	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(530,	9301.71f	,-7268.83f	,15.2522f	,0.38392f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +12://	联盟 埃索达	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(530,	-4073.83f,	-12030.6f	,1.59961f	,1.40306f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +13://	凜風峽灣：復仇臺地  X: 2018.871216 Y: -6175.680176 Z: 15.392685 Map: 571 or:1.040551	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
							pPlayer->TeleportTo(571,	2018.871216f, -6175.680176f	,15.392685f	,1.040551f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +14://	北風凍原：戰歌堡	X: 2915.007568 Y: 6256.863770 Z: 208.750229 Map: 571 or :0.575959
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
							pPlayer->TeleportTo(571,	2915.007568f,	6256.863770f	,208.750229f	,0.575959f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +15://	中立 奎尔丹纳斯岛 	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
						pPlayer->TeleportTo(530,	13011.5f,	-6900.8f,	10.59f,	3.803f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +16://	凜風峽灣：瓦爾加德 	X: 589.297852 Y: -5104.841309 Z: 5.260796 Map: 571 or:1.251140
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
							pPlayer->TeleportTo(571,	589.297852f,	-5104.841309f,	5.260796f,	1.251140f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +17://	北風凍原：驍勇要塞X: 2231.822754 Y: 5131.532227 Z: 5.344474 Map: 571 or:1.570800	 
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
							pPlayer->TeleportTo(571,	2231.822754f,	5131.532227f,	5.344474f,	1.5708f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF + 714 ://达拉然
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{
							pPlayer->TeleportTo(571,5813.874512f,448.283173f,658.752319f,1.008073f);
							pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +715://	黑暗之门	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{pPlayer->TeleportTo(530	,-246.6f	,946.0f,84.4f,0.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF+5191://naxx 533	3005.92	-3414.68	297.074	1.57674

						if(pPlayer->GetMoney() >= Cost_Extent_Boss_Copy)
						{pPlayer->TeleportTo(533,	3005.92f,	-3414.68f,	297.074f,	1.57674f);
						pPlayer->ModifyMoney(-Cost_Extent_Boss_Copy);
						}						else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF+5192://阿尔卡冯的宝库 571 5405.26953 2840.214111 418.675201 0.20423

						if(pPlayer->GetMoney() >= Cost_Extent_Boss_Copy)
						{pPlayer->TeleportTo(571 ,5405.26953f ,2840.214111f ,418.675201f ,0.020423f);
						pPlayer->ModifyMoney(-Cost_Extent_Boss_Copy);
						}						else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +7210://	中立 沙塔斯城	
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{pPlayer->TeleportTo(530,	-1813.51f,	5413.34f	,-12.4281f,	1.0f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +7700://52
						//中立 52区X: 3027.937988 Y: 3571.022461 Z: 143.480347 Map: 530 Or5.754831
						if(pPlayer->GetMoney() >= Cost_MainCity)
						{pPlayer->TeleportTo(530,3027.937988f,3571.022461f,143.480347f,5.754831f);
						pPlayer->ModifyMoney(-Cost_MainCity);
						}else{
							WUZHU_Common::SendMoneyError(pPlayer);
						}
						break;
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;					
					//银行
				case 20 :
					pPlayer->SEND_BANKERLIST(_Item->GetObjectGuid() );
					break;
				case 21://公会银行
					if(!pPlayer->WUZHU_OpenGuildBank())
						pPlayer->CLOSE_GOSSIP_MENU();
					break;

				//case 22://绑定					
				//	pPlayer->GetSession()->SendBindPoint();
				//	pPlayer->CLOSE_GOSSIP_MENU();
				//	break;
				case 24://绑定-绑定
					{
						uint32 v_map = pPlayer->GetMapId();				
						if(pPlayer->isGameMaster()||pPlayer->GetVIP()==5||v_map == 530 || v_map == 571||v_map == 0||v_map == 1)
						{
							uint32 index=action-GOSSIP_ACTION_INFO_DEF;
							//先删除数据
							pPlayer->WUZHU_DelBind(index);
							//再插入数据	
							//SD2Database.PExecute("INSERT INTO wuzhu_bind (id,guid,map,zone,position_x,position_y,position_z,maptext) VALUES ('%d','%u', '%u', '%u', '%f', '%f', '%f','%d-%d')", 
							//index,pPlayer->GetGUID(), pPlayer->GetMapId(), (uint32)pPlayer->GetZoneId(), pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(),pPlayer->GetMapId(),pPlayer->GetZoneId());
							if(pPlayer->WUZHU_InsertBind(index))
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-341,pPlayer));
							}
							pPlayer->CLOSE_GOSSIP_MENU();
						}
						else
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-344,pPlayer));						
							pPlayer->CLOSE_GOSSIP_MENU();
						}
					}
					break;
				case 25://绑定-传送
					{
						//uint32 v_map = pPlayer->GetMapId();				
						//if(pPlayer->isGameMaster()||pPlayer->GetVIP()==5||v_map == 530 || v_map == 571||v_map == 0||v_map == 1)
						//传送不限制
						//{
							uint32 index=action-GOSSIP_ACTION_INFO_DEF;
							//												0		1	2			3			4			5	6		7							
							//SD2Database.PQuery("SELECT id, guid, position_x, position_y, position_z, map, Zone, maptext FROM wuzhu_bind where guid=%u and id=%d",pPlayer->GetGUID(),index);
							if(QueryResult* result = pPlayer->WUZHU_GetOneBind(index))
							{
								Field *fields = result->Fetch();
								pPlayer->TeleportTo(fields[5].GetInt32(),fields[2].GetFloat(),	fields[3].GetFloat(), fields[4].GetFloat(),	1.0f);
								delete result;
							}
							else
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-343,pPlayer));							
							pPlayer->CLOSE_GOSSIP_MENU();
						//}
						//else
						//{
						//	pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-344,pPlayer));						
						//	pPlayer->CLOSE_GOSSIP_MENU();
						//}

					}
					break;
				case 26://绑定-删除
					{
					uint32 index=action-GOSSIP_ACTION_INFO_DEF;
					//删除数据
						//SD2Database.PExecute("delete from wuzhu_bind where id=%d and guid='%u'",index,pPlayer->GetGUID());
					if(pPlayer->WUZHU_DelBind(index))
					{
						pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-342,pPlayer));
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					}
					break;
				case 27://绑定-位置查看
					{
						uint32 index=action-GOSSIP_ACTION_INFO_DEF;
						//位置查看
						//							 0		1	2			3			4			5	6		7							
						//SD2Database.PQuery("SELECT id, guid, position_x, position_y, position_z, map, Zone, maptext FROM wuzhu_bind where guid=%u and id=%d",pPlayer->GetGUID(),index);
						
						if(QueryResult* result = pPlayer->WUZHU_GetOneBind(index))
							{
								Field *fields = result->Fetch();
								//要判断是map
								if(fields[5].GetUInt32()==pPlayer->GetMapId())
									pPlayer->WUZHU_SendPointOfInterest(fields[2].GetFloat(),	fields[3].GetFloat(),fields[7].GetString());
								else
									pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-347,pPlayer));
								delete result;
							}
							else
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-343,pPlayer));
						pPlayer->CLOSE_GOSSIP_MENU();
					}
					break;
				case 40://修理装备
					pPlayer->DurabilityRepairAll(true,1,false);
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 70://商店
					pPlayer->GetSession()->OpenShop(_Item->GetObjectGuid(),action-GOSSIP_ACTION_INFO_DEF);
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 90://领取工资
					//获取当前时间
					{
						//uint32 ingametime=WorldTimer::getMSTimeDiff(pPlayer->GetInGameTime(),WorldTimer::getMSTime())/1000;
						//if(ingametime > 60) // 条件检查你在线时间大于1分钟．这可以自己改(单位:秒)
						//{
						//	float money = 0;
						//	//TODO:工资系统，根据游戏时间来发工资
						//	//规则：工资=1分钟1G
						//	money = (float)(ingametime / 60 *WUZHU_Wages_Money);

						//	pPlayer->ModifyMoney(int32(money));      //满足条件获得金币
						//	pPlayer->SetInGameTime(WorldTimer::getMSTime());   //将在玩家在线时间归零
						//	pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-138,pPlayer));
						//	pPlayer->CLOSE_GOSSIP_MENU();
						//}
						//else
						//{
						//	pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-139,pPlayer));
						//	pPlayer->CLOSE_GOSSIP_MENU();
						//}
						pPlayer->CLOSE_GOSSIP_MENU();
						break;
					}
				case 110:
					//飞行坐骑
					//if((pPlayer->GetMapId()==564||pPlayer->GetMapId()==580)&&pPlayer->getLevel()<70)//两个副本禁止用坐骑
					//{
					//	pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-140,pPlayer));
					//	pPlayer->CLOSE_GOSSIP_MENU();
					//	return false;
					//}
					if(!pPlayer->IsNonMeleeSpellCasted(false))
					{
						switch(action) 
						{
						case  GOSSIP_ACTION_INFO_DEF+SFM_LM_RL20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM_RL20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;

						case  GOSSIP_ACTION_INFO_DEF+SFM_LM_AR20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM_AR20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_LM_ZR20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM_ZR20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_LM_JL20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM_JL20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_LM_DLN20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM_DLN20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_BL_SR20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL_SR20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_BL_NTR20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL_NTR20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_BL_WN20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL_WN20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_BL_JM20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL_JM20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_BL_XJN20:
							if(pPlayer->getLevel()>=10)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL_XJN20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-141,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_BL20:
							if(pPlayer->getLevel()>=20)
							{
							//pPlayer->StopMoving();
							pPlayer->CastSpell(pPlayer,SFM_BL20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-142,pPlayer));								
							}
							break;
						case  GOSSIP_ACTION_INFO_DEF+SFM_LM20:
							if(pPlayer->getLevel()>=20)
							{
							//pPlayer->StopMoving();
							pPlayer->CastSpell(pPlayer,SFM_LM20,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-142,pPlayer));								
							}
							break;
						case GOSSIP_ACTION_INFO_DEF+SFM_BL40:
							if(pPlayer->getLevel()>=30)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL40,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-143,pPlayer));								
							}
							break;
						case GOSSIP_ACTION_INFO_DEF+SFM_LM40:
							if(pPlayer->getLevel()>=30)
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM40,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-143,pPlayer));								
							}
							break;
						case GOSSIP_ACTION_INFO_DEF+SFM_BL60:
							if(pPlayer->getLevel()>=50||pPlayer->isGameMaster())
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_BL60,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-144,pPlayer));								
							}
							break;
						case GOSSIP_ACTION_INFO_DEF+SFM_LM60:
							if(pPlayer->getLevel()>=50||pPlayer->isGameMaster())
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LM60,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-144,pPlayer));
								
							}
							break;
						case GOSSIP_ACTION_INFO_DEF+SFM_LMBL70_1:
							if(pPlayer->getLevel()>=60||pPlayer->isGameMaster())
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LMBL70_1,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-145,pPlayer));
								
							}
							break;
						case GOSSIP_ACTION_INFO_DEF+SFM_LMBL70_2:
							if(pPlayer->getLevel()>=70||pPlayer->isGameMaster())
							{
								//pPlayer->StopMoving();
								pPlayer->CastSpell(pPlayer,SFM_LMBL70_2,true);
							}
							else
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-146,pPlayer));
								
							}
							break;
						default:
							if(action>GOSSIP_ACTION_INFO_DEF)
							{
							//pPlayer->StopMoving();
							pPlayer->CastSpell(pPlayer,action-GOSSIP_ACTION_INFO_DEF,true);
							}
							break;
						}
						pPlayer->CLOSE_GOSSIP_MENU();
					}					
					break;
				case 130://灵魂使者
					pPlayer-> GetSession()->SendSpiritResurrect();
					pPlayer->CLOSE_GOSSIP_MENU();
					break;					
				case 140://返回主菜单
					pPlayer->CLOSE_GOSSIP_MENU();
					//SpellCastTargets targets;
					//GossipHello_Item(pPlayer,_Item,targets);
					break;
				case 150://积分查询					
					pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-45,pPlayer),pPlayer->GetVIP_Integral());
					//pPlayer->GetSession()->SendAreaTriggerMessage(" |cff6000ff%u|r ",pPlayer->GetVIP_Integral());					
					//pPlayer->GetSession()->SendAreaTriggerMessage(" |cff6000ff你当前共有积分为:|r ");					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 151://物品换积分
					{
						uint32 wpid=0;
						uint32 wpcount=0;
						uint32 addintegral=0;
						switch(action)
						{
							case GOSSIP_ACTION_INFO_DEF+1://英雄纹章10换10000
								wpid=WP_YQWZID;
								wpcount=10;
								addintegral=10000;
							break;
							case GOSSIP_ACTION_INFO_DEF+2://勇气纹章10换20000
								wpid=WP_YXWZID;
								wpcount=10;
								addintegral=20000;
								break;
							case GOSSIP_ACTION_INFO_DEF+4://积分币10换20000
								wpid=WP_JFBID;
								wpcount=10;
								addintegral=20000;
								break;
						}
						if(pPlayer->GetItemCount(wpid)<wpcount)
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-46,pPlayer));
						}
						else
						{
							pPlayer->DestroyItemCount(wpid,wpcount,true);
							pPlayer->ModifyVIP_Integral(addintegral);
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-47,pPlayer),addintegral);

						}
						//pPlayer->GetSession()->SendAreaTriggerMessage(" |cffaa0080本功能即将开放.|r ");
						pPlayer->CLOSE_GOSSIP_MENU();
						break;
					}
					
				case 152://积分买荣誉点数
					//pPlayer->GetSession()->SendAreaTriggerMessage(" |cffaa0080本功能即将开放.|r ");
					{
					uint32 needintegral=0;
					uint32 addHoner=0;
					switch(action)
					{
					case GOSSIP_ACTION_INFO_DEF+1://1000->100
						needintegral=1000;
						addHoner=100;
						break;
					case GOSSIP_ACTION_INFO_DEF+2://1w->1000
						needintegral=10000;
						addHoner=1000;
						break;
					case GOSSIP_ACTION_INFO_DEF+3://10W->1W
						needintegral=100000;
						addHoner=10000;
						break;

					}
					if(needintegral!=0&&addHoner!=0)
					{
						if(needintegral>pPlayer->GetVIP_Integral())
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
						}
						else
						{
							pPlayer->ModifyVIP_Integral(-(int32)needintegral);
							pPlayer->ModifyHonorPoints(addHoner);
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-49,pPlayer),addHoner,pPlayer->GetHonorPoints());
							pPlayer->CLOSE_GOSSIP_MENU();

						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					}
					break;
				case 153://积分换技能点数
					if(pPlayer->GetVIP_Integral()<UPSkillIntegral_10)
					{
						pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
					}
					else
					{
						bool baddsuccess=false;
						switch (action)
						{
						case GOSSIP_ACTION_INFO_DEF+1://增加采矿186 SKILL_MINING 参考\game\SharedDefines.h enum SkillType
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_MINING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+2://增加炼金171
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_ALCHEMY,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+3://增加锻造164
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_BLACKSMITHING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+4://增加裁缝197
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_TAILORING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+5://增加烹饪185
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_COOKING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+6://增加制皮165
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_LEATHERWORKING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+7://增加钓鱼356
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_FISHING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+8://增加附魔333
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_ENCHANTING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+9://增加珠宝加工755
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_JEWELCRAFTING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+10://增加工程学202
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_ENGINEERING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+11://增加急救129
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_FIRST_AID,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+12://增加草药182
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_HERBALISM,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+13://增加剥皮393
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_SKINNING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+21://增加开锁633
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_LOCKPICKING,10);
							break;
						case GOSSIP_ACTION_INFO_DEF+22://增加武器和防御
							{
								uint16 successcount=0;
								if(WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_DEFENSE,10))
								{
									successcount++;
									// update dependent from defense skill part
									pPlayer->UpdateDefenseBonusesMod();
								}
								for(int i=0;i<MAX_ITEM_SUBCLASS_WEAPON;i++)
								{
									if(item_weapon_skills[i]!=0)
										if(WUZHU_Common::WZ_UpdateSkill(pPlayer,item_weapon_skills[i],10))
											successcount++;
								}
								if(successcount)
									baddsuccess=true;
							}
							
							break;
						}
						//pPlayer->GetSession()->SendAreaTriggerMessage(" |cffaa0080你当前共有积分：%u.|r ",pPlayer->GetVIP_Integral());
						if(baddsuccess)
						{
							pPlayer->ModifyVIP_Integral(-UPSkillIntegral_10);
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-50,pPlayer),pPlayer->GetVIP_Integral());
						}
						else
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-51,pPlayer));
						}
						
					}					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 154://积分换等级 					
					if(pPlayer->getLevel()>=80)
						pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-42,pPlayer));
					else
					{
						uint32 newlevel=pPlayer->getLevel()+1;
						uint32 needintegral=(newlevel)*UPLevelIntegral;//需要的积分
						if(pPlayer->GetVIP_Integral()<needintegral)
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
						}
						else
						{
							pPlayer->ModifyVIP_Integral(-(int32)needintegral);
							pPlayer->GiveLevel(newlevel);
							pPlayer->InitTalentForLevel();
							pPlayer->SetUInt32Value(PLAYER_XP,0);
							//pPlayer->GetSession()->SendAreaTriggerMessage(" |cffaa0080你当前共有积分：%u.|r ",pPlayer->GetVIP_Integral());
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-52,pPlayer),pPlayer->GetVIP_Integral());
						}
						
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 160://购买积分	提示
					pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-54,pPlayer));
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 170://自义坐骑
					if(!pPlayer->IsNonMeleeSpellCasted(false))
					{
						pPlayer->CastSpell(pPlayer,action-GOSSIP_ACTION_INFO_DEF,true);						
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 999://关闭菜单
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
		} 
	}
		else
		{
			pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-147,pPlayer));
			pPlayer->CLOSE_GOSSIP_MENU();
		}

	return true;
} 

void AddSC_WUZHU_Item() 
{ 
	Script *newscript; 

	newscript = new Script; 
	newscript->Name="WUZHUItem"; 
	newscript->pItemUse= &GossipHello_Item; 
	newscript->pGossipItemSelect = &GossipWUZHUItemSelect; 
	newscript->RegisterSelf(); 
}