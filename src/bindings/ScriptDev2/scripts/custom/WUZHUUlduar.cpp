#include "precompiled.h"
#include "sc_gossip.h"
#include "SharedDefines.h"
#include "Timer.h"
#include "WUZHU.h"
//#include "InstanceSaveMgr.h"

bool GossipHello_WZUlduar(Player *pPlayer, Creature *_Creature)
{ 
	if(!pPlayer||!_Creature)
		return false;
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-153,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
	{
		if (!pPlayer->isDead())
		{ 
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-332,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 1,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Raid,false);
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-333,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 2,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Raid,false);
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-334,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 3,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Raid,false);
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-335,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 4,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Raid,false);
			pPlayer->ADD_GOSSIP_ITEM_EXTENDED( GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-336,pPlayer), 5, GOSSIP_ACTION_INFO_DEF + 5,pPlayer->GetWUZHUString(Cost_BOXMessage_ID,pPlayer),Cost_Raid,false);
		}
		pPlayer->SEND_GOSSIP_MENU(80000,_Creature->GetObjectGuid());
	}
	return true;
}

bool GossipSelect_WZUlduar(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
{
	if (!isstone(pPlayer))
	{
		switch(sender) {
					//传送 
				case 5 :
					switch(action) 
					{
						/*统一格式说明：(MapID, X, Y, Z, 0);

						case GOSSIP_ACTION_INFO_DEF + 9 :
						pPlayer->TeleportTo(533,3700.35,-5185.92,143.957,4.403038,0);
						break;
						*/
					case GOSSIP_ACTION_INFO_DEF +1://奧杜亞攻城區 -707.120789 -92.624481 430.275574 603 6.244293
						if(pPlayer->GetMoney() >= Cost_Raid)
						{
							pPlayer->TeleportTo(603,-707.120789f,-92.624481f,430.275574f,6.244293f);
							pPlayer->ModifyMoney(-Cost_Raid);
						}else{
							pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +2://前厅 .go 1498.265503 -24.096891 421.366669 603   0.007446
						if(pPlayer->GetMoney() >= Cost_Raid)
						{
						pPlayer->TeleportTo(603,1498.265503f,-24.096891f,421.366669f,0.007446f);
						pPlayer->ModifyMoney(-Cost_Raid);
						}else{
							pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +3://巧思回廊 .go 2289.833008 287.100616 423.343384  603   3.068926
						if(pPlayer->GetMoney() >= Cost_Raid)
						{
						pPlayer->TeleportTo(603,2289.833008f,287.100616f,423.343384f,3.068926f);
						pPlayer->ModifyMoney(-Cost_Raid);
						}else{
							pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +4://创思之厅：.go 2349.012939 2570.679932 419.632355 603     6.212879
						if(pPlayer->GetMoney() >= Cost_Raid)
						{
						pPlayer->TeleportTo(603,2349.012939f,2570.679932f,419.632355f,6.212879f);
						pPlayer->ModifyMoney(-Cost_Raid);
						}else{
							pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
						}
						break;
					case GOSSIP_ACTION_INFO_DEF +5://癫狂斜廊：.go 1802.27002 124.956 342.377991 603   1.609490
						if(pPlayer->GetMoney() >= Cost_Raid)
						{
						pPlayer->TeleportTo(603,1802.27002f,124.956f,342.377991f,1.609490f);
						pPlayer->ModifyMoney(-Cost_Raid);
						}else{
							pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
						}
						break;
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
		} 
	}
	return true;
}


void AddSC_WUZHUUlduar() 
{ 
	Script *newscript; 

	newscript = new Script; 
	newscript->Name="WUZHUUlduar"; 
	newscript->pGossipHello = &GossipHello_WZUlduar; 
	newscript->pGossipSelect = &GossipSelect_WZUlduar;
	newscript->RegisterSelf(); 
}