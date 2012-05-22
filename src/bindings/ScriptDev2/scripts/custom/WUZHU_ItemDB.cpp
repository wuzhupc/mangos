#include "precompiled.h"
#include "Database/DatabaseMysql.h"
#include "DBCStores.h"
#include "../../../shared/Config/Config.h"
#include "sc_gossip.h"
#include "SharedDefines.h"
#include "Timer.h"
#include "Item.h"
#include "sc_creature.h"
#include "SpellMgr.h"
#include "Spell.h" 
#include "WUZHU.h"
#include "../system/system.h"


//extern  UNORDERED_MAP<uint32, WUZHU_GameTele> GameTele_Map;//传送
//extern  UNORDERED_MAP<uint32, WUZHU_Teleport_Template> Teleport_Template_Map;


bool GossipDBItemHello(Player *pPlayer, Item* _Item, SpellCastTargets const& targets)
{
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-55,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
	if(pPlayer->IsTaxiFlying()||pPlayer->IsMounted())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-56,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
	{
		if (!pPlayer->isDead())
		{ 
			UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
			std::list<TeleportHolder> TeleportList;
			TeleportList.clear();

			uint32 count=0;//超过16个就不处理
			bool baddmenu=true;
			for (i =pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
			{
				//outstring_log("wuzhu %d %s",(*i).first,(*i).second.Title);
				baddmenu=true;
				if(count>=16)
					break;				
				if ((*i).second.MenuClass==0&&(*i).second.PRG!=130&&((*i).second.Type==2||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
				{
					if((*i).second.VIP>0)
					{
						if(pPlayer->GetVIP()==0)
							baddmenu=false;
					}
					if((*i).second.Level>pPlayer->getLevel())
						baddmenu=false;
					if((*i).second.P_class>0)
					{
						if((*i).second.P_class!=pPlayer->getClass())
							baddmenu=false;
					}
					if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
					{
						if(!isbl(pPlayer))
							baddmenu=false;
					}
					else
					if((*i).second.PTeam==469)
					{
						if(isbl(pPlayer))
							baddmenu=false;
					}	
					if(baddmenu||pPlayer->isGameMaster())
					{
						++count;
						TeleportList.push_back(TeleportHolder((*i).first, (*i).second));
						
					}
				}
			}
			if(!TeleportList.empty())
			{
				count=0;
				TeleportList.sort();
				for(std::list<TeleportHolder>::iterator th = TeleportList.begin();th!=TeleportList.end();++th)
				{
					++count;
					if(count>=16)
						break;
					if(th->Teleport.PRG==90)
						pPlayer->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
					else
					{
						if(th->Teleport.PRG==110||th->Teleport.PRG==111)
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
						else
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
					}
				}	
			}
		}
		else 
		if(pPlayer->isDead())
		{
			UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;

			bool baddmenu=true;
			for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
			{
				if ((*i).second.MenuClass==0&&(*i).second.PRG==130&&((*i).second.Type==2||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
				{
					baddmenu=true;
					if((*i).second.VIP>0)
					{
						if(pPlayer->GetVIP()==0)
							baddmenu=false;
					}
					if((*i).second.Level>pPlayer->getLevel())
						baddmenu=false;
					if((*i).second.P_class>0)
					{
						if((*i).second.P_class!=pPlayer->getClass())
							baddmenu=false;
					}
					if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
					{
						if(!isbl(pPlayer))
							baddmenu=false;
					}
					else
						if((*i).second.PTeam==469)
						{
							if(isbl(pPlayer))
								baddmenu=false;
						}	
						if(baddmenu||pPlayer->isGameMaster())
						{
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED((*i).second.Icon,(*i).second.Title,(*i).second.PRG,GOSSIP_ACTION_INFO_DEF+(*i).second.Action,(*i).second.Message,(*i).second.Money?(*i).second.Money:(*i).second.Integral,false);
						}

					break;
				}
			}
		} 
		pPlayer->SEND_GOSSIP_MENU(80000,_Item->GetObjectGuid());
	}

	return true;
}

void SendDefaultDBItemMenu(Player *pPlayer, Item *_Item, uint32 action)
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-55,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
	if(pPlayer->IsTaxiFlying()||pPlayer->IsMounted())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-56,pPlayer));
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
	{
		if (!pPlayer->isDead())
		{ 
			UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
			std::list<TeleportHolder> TeleportList;
			TeleportList.clear();
			uint32 count=0;//超过16个就不处理
			bool baddmenu=true;
			for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
			{
				baddmenu=true;
				if(count>=16)
					break;				
				if ((*i).second.MenuClass==baseaction&&(*i).second.PRG!=130&&((*i).second.Type==2||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
				{
					if((*i).second.VIP>0)
					{
						if(pPlayer->GetVIP()==0)
							baddmenu=false;
						//outstring_log("wuzhu vip %d %d",(*i).second.VIP,pPlayer->GetVIP());
					}
					if((*i).second.Level>pPlayer->getLevel())
					{
						baddmenu=false;
						//outstring_log("wuzhu Level %d %d",(*i).second.Level,pPlayer->getLevel());
					}
					if((*i).second.P_class>0)
					{
						if((*i).second.P_class!=pPlayer->getClass())
							baddmenu=false;
						//outstring_log("wuzhu P_class %d %d",(*i).second.P_class,pPlayer->getClass());
					}
					if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
					{
						if(!isbl(pPlayer))
							baddmenu=false;
					}
					else
					{
						if((*i).second.PTeam==469)
						{
							if(isbl(pPlayer))
								baddmenu=false;
						}	
					}
					//outstring_log("wuzhu PTeam %d %d",(*i).second.PTeam,pPlayer->getRace());
					if(baddmenu||pPlayer->isGameMaster())
					{
						++count;
						TeleportList.push_back(TeleportHolder((*i).first, (*i).second));
					}
				}
			}
			if(!TeleportList.empty())
			{
				count=0;
				TeleportList.sort();
				for(std::list<TeleportHolder>::iterator th = TeleportList.begin();th!=TeleportList.end();++th)
				{
					++count;
					if(count>=16)
						break;
					if(th->Teleport.PRG==90)
						pPlayer->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
					else
					{
						if(th->Teleport.PRG==110||th->Teleport.PRG==111)
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
						else
							pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
					}
				}	
			}
		}		
		pPlayer->SEND_GOSSIP_MENU(80006,_Item->GetObjectGuid());
	}
}

bool GossipDBItemSelect(Player *pPlayer, Item *_Item, uint32 sender, uint32 action )
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if (!isstone(pPlayer))
	{
		switch(sender) {
			// 主选单
				case GOSSIP_SENDER_MAIN :
					SendDefaultDBItemMenu(pPlayer, _Item, action);
					break;
			//传送 
				case 5 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
								if((*i).second.Integral>0)
								{
									if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
									{
										btelete=2;//需要积分
									}
									else
									{
										pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
										break;
									}
								}
								UNORDERED_MAP<uint32, WUZHU_GameTele>::iterator j = pSystemMgr.GameTele_Map.find(baseaction);
								if(j!=pSystemMgr.GameTele_Map.end())
								{
									pPlayer->TeleportTo((*j).second.target_mapId,(*j).second.target_X,
										(*j).second.target_Y,(*j).second.target_Z,(*j).second.target_Orientation);
									if(btelete==1)
										pPlayer->ModifyMoney(0-(*i).second.Money);
									else
										if(btelete==2)
											pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
								} 
								break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
					//开飞行点
				case 10:
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
								 pPlayer->SetTaxiCheater(true);
								 if(btelete==1)
								 {
									pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
									 if(btelete==2)
								 {
									 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
								 }
								 pPlayer->GetSession()->SendAreaTriggerMessage(" pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-148,pPlayer) ");
								 break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
					//银行
				case 20 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->SEND_BANKERLIST(_Item->GetObjectGuid() );
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
								break;
							}
						}
					}	
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 22://绑定
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->GetSession()->SendBindPoint();
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;	
				case 40://修理装备
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->DurabilityRepairAll(true,1,false);
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 70://商店
					pPlayer->GetSession()->OpenShop(_Item->GetObjectGuid(),action-GOSSIP_ACTION_INFO_DEF);					
					break;
				case 90://领取工资
					//获取当前时间
					{
						//uint32 ingametime=WorldTimer::getMSTimeDiff(pPlayer->GetInGameTime(),getMSTime())/1000;
						//if(ingametime > 60) // 条件检查你在线时间大于1分钟．这可以自己改(单位:秒)
						//{
						//	UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						//	for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						//	{
						//		if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
						//		{
						//			float money = 0;
						//			//TODO:工资系统，根据游戏时间来发工资
						//			//规则：工资=1分钟1G
						//			money = (float)(ingametime / 60 *(*i).second.Money);

						//			pPlayer->ModifyMoney(int32(money));      //满足条件获得金币
						//			pPlayer->SetInGameTime(getMSTime());   //将在玩家在线时间归零
						//			pPlayer->GetSession()->SendAreaTriggerMessage(" |cff7000ff领取了工资：%d金币.|r ",int32(money)/10000);
						//			break;

						//		}
						//	}
						//}
						//else
						//{
						//	pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000领取工资时间间隔太短.|r ");
						//}
						pPlayer->CLOSE_GOSSIP_MENU();
						break;
					}
				case 110://飞行坐骑
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.SpellID==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->CastSpell(pPlayer,(*i).second.SpellID,true);
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 111://宠物模型
					{
						if(pPlayer->getClass()==CLASS_HUNTER)
						{
							Pet* pet = pPlayer->GetPet();
							if(!pet||pet->getPetType()!=HUNTER_PET)
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000你没开玩笑吧，宠物呢?|r ");
								break;
							}
							if(!pPlayer->IsWithinDistInMap(pet, MAX_VISIBILITY_DISTANCE))
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000你的宠物太害羞，跑得太远了.|r ");
								break;
							}
							UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
							for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
							{
								if ((*i).second.SpellID==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
								{
									uint32 btelete=0;//不能传送
									if((*i).second.Money>0)
									{
										if(pPlayer->GetMoney() >= (*i).second.Money)
										{
											btelete=1;//需要money
										}
										else
										{
											WUZHU_Common::SendMoneyError(pPlayer);
											break;
										}
									}
									else
										if((*i).second.Integral>0)
										{
											if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
											{
												btelete=2;//需要积分
											}
											else
											{
												pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
												break;
											}
										}
										pet->SetDisplayId((*i).second.SpellID);
										pet->SetNativeDisplayId((*i).second.SpellID);
										pet->SetByteValue(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);
										//pPlayer->CastSpell(pPlayer,(*i).second.SpellID,true);
										if(btelete==1)
										{
											pPlayer->ModifyMoney(0-(*i).second.Money);									 
										}
										else
										{
											if(btelete==2)
											{
												pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
											}
										}
										break;
								}
							}
						}
						else
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000目前此项服务只对猎人开放.|r ");
							break;
						}						
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 120://积分查询
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->GetSession()->SendAreaTriggerMessage(" |cffaa0080你当前共有积分：%u.|r ",pPlayer->GetVIP_Integral());
									if(btelete==1)
									{
										pPlayer->ModifyMoney(0-(*i).second.Money);									 
									}
									else
									{
										if(btelete==2)
										{
											pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
										}
									}
									break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 130://灵魂使者
					{
						if(!pPlayer->isDead())
							break;
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(pPlayer);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer-> GetSession()->SendSpiritResurrect();
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
		}
	}
	return true;
}


bool GossipDBTeleportHello(Player *pPlayer, Creature *_Creature)
{
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在战斗中使用本功能.|r ");
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
		if(pPlayer->IsTaxiFlying()||pPlayer->IsMounted())
		{ 
			pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在坐骑中使用本功能.|r ");
			pPlayer->CLOSE_GOSSIP_MENU();
		}
		else
		{
			if (!pPlayer->isDead())
			{ 
				UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
				std::list<TeleportHolder> TeleportList;
				TeleportList.clear();
				uint32 count=0;//超过16个就不处理
				bool baddmenu=true;
				for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
				{
					if(count>=16)
						break;				
					if ((*i).second.MenuClass==0&&(*i).second.PRG!=130&&((*i).second.Type==1||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
					{
						baddmenu=true;
						if((*i).second.VIP>0)
						{
							if(pPlayer->GetVIP()==0)
								baddmenu=false;
						}
						if((*i).second.Level>pPlayer->getLevel())
							baddmenu=false;
						if((*i).second.P_class>0)
						{
							if((*i).second.P_class!=pPlayer->getClass())
								baddmenu=false;
						}
						if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
						{
							if(!isbl(pPlayer))
								baddmenu=false;
						}
						else
							if((*i).second.PTeam==469)
							{
								if(isbl(pPlayer))
									baddmenu=false;
							}	
							if(baddmenu||pPlayer->isGameMaster())
							{
								++count;
								TeleportList.push_back(TeleportHolder((*i).first, (*i).second));								
							}
					}
				}

				if(!TeleportList.empty())
				{
					count=0;
					TeleportList.sort();
					for(std::list<TeleportHolder>::iterator th = TeleportList.begin();th!=TeleportList.end();++th)
					{
						++count;
						if (count>=16)
						{
							break;
						}
						if(th->Teleport.PRG==90)
							pPlayer->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
						else
						{
							if(th->Teleport.PRG==110||th->Teleport.PRG==111)								
								pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
							else									
								pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
						}
					}
				}
				
			}
			else 
				if(pPlayer->isDead())
				{
					UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;

					bool baddmenu=true;
					for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
					{
						if ((*i).second.MenuClass==0&&(*i).second.PRG==130&&((*i).second.Type==1||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
						{
							baddmenu=true;
							if((*i).second.VIP>0)
							{
								if(pPlayer->GetVIP()==0)
									baddmenu=false;
							}
							if((*i).second.Level>pPlayer->getLevel())
								baddmenu=false;
							if((*i).second.P_class>0)
							{
								if((*i).second.P_class!=pPlayer->getClass())
									baddmenu=false;
							}
							if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
							{
								if(!isbl(pPlayer))
									baddmenu=false;
							}
							else
								if((*i).second.PTeam==469)
								{
									if(isbl(pPlayer))
										baddmenu=false;
								}	
								if(baddmenu||pPlayer->isGameMaster())
								{
									pPlayer->ADD_GOSSIP_ITEM_EXTENDED((*i).second.Icon,(*i).second.Title,(*i).second.PRG,GOSSIP_ACTION_INFO_DEF+(*i).second.Action,(*i).second.Message,(*i).second.Money?(*i).second.Money:(*i).second.Integral,false);
								}

								break;
						}
					}
				} 
				pPlayer->SEND_GOSSIP_MENU(80001,_Creature->GetObjectGuid());
		}

		return true;
}

void SendDefaultDBTeleportMenu(Player *pPlayer, Creature *_Creature, uint32 action)
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在战斗中使用本功能.|r ");
		pPlayer->CLOSE_GOSSIP_MENU();
	}
	else
		if(pPlayer->IsTaxiFlying()||pPlayer->IsMounted())
		{ 
			pPlayer->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在坐骑中使用本功能.|r ");
			pPlayer->CLOSE_GOSSIP_MENU();
		}
		else
		{
			if (!pPlayer->isDead())
			{ 
				UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
				std::list<TeleportHolder> TeleportList;
				TeleportList.clear();
				uint32 count=0;//超过16个就不处理
				bool baddmenu=true;
				for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
				{
					baddmenu=true;
					if(count>=16)
						break;				
					if ((*i).second.MenuClass==baseaction&&(*i).second.PRG!=130&&((*i).second.Type==1||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
					{
						if((*i).second.VIP>0)
						{
							if(pPlayer->GetVIP()==0)
								baddmenu=false;
						}
						if((*i).second.Level>pPlayer->getLevel())
							baddmenu=false;
						if((*i).second.P_class>0)
						{
							if((*i).second.P_class!=pPlayer->getClass())
								baddmenu=false;
						}
						if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
						{
							if(!isbl(pPlayer))
								baddmenu=false;
						}
						else
							if((*i).second.PTeam==469)
							{
								if(isbl(pPlayer))
									baddmenu=false;
							}	
							if(baddmenu||pPlayer->isGameMaster())
							{
								++count;
								TeleportList.push_back(TeleportHolder((*i).first, (*i).second));
								
							}
					}
				}
				if(!TeleportList.empty())
				{
					count=0;
					TeleportList.sort();
					for(std::list<TeleportHolder>::iterator th = TeleportList.begin();th!=TeleportList.end();++th)
					{
						++count;
						if (count>=16)
						{
							break;
						}
						if(th->Teleport.PRG==90)
							pPlayer->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
						else
						{
							if(th->Teleport.PRG==110||th->Teleport.PRG==111)
								pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
							else
								pPlayer->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money?th->Teleport.Money:th->Teleport.Integral,false);
						}
					}
				}
			}		
			pPlayer->SEND_GOSSIP_MENU(80007,_Creature->GetObjectGuid());
		}
}

bool GossipDBTeleportSelect(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if (!isstone(pPlayer))
	{
		switch(sender) {
			// 主选单
				case GOSSIP_SENDER_MAIN :
					SendDefaultDBTeleportMenu(pPlayer, _Creature, action);
					break;
					//传送 
				case 5 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									UNORDERED_MAP<uint32, WUZHU_GameTele>::iterator j = pSystemMgr.GameTele_Map.find(baseaction);
									if(j!=pSystemMgr.GameTele_Map.end())
									{
										pPlayer->TeleportTo((*j).second.target_mapId,(*j).second.target_X,
											(*j).second.target_Y,(*j).second.target_Z,(*j).second.target_Orientation);
										if(btelete==1)
											pPlayer->ModifyMoney(0-(*i).second.Money);
										else
											if(btelete==2)
												pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									} 
									break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
					//开飞行点
				case 10:
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->SetTaxiCheater(true);
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
									 pPlayer->GetSession()->SendAreaTriggerMessage(" pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-148,pPlayer) ");
									 break;
							}
						}
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
					//银行
				case 20 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->SEND_BANKERLIST(_Creature->GetObjectGuid() );
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					break;
				case 22://绑定
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->GetSession()->SendBindPoint();
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;	
				case 40://修理装备
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->DurabilityRepairAll(true,1,false);
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 90://领取工资
					//获取当前时间
					{
						//uint32 ingametime=WorldTimer::getMSTimeDiff(pPlayer->GetInGameTime(),getMSTime())/1000;
						//if(ingametime > 60) // 条件检查你在线时间大于1分钟．这可以自己改(单位:秒)
						//{
						//	UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						//	for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						//	{
						//		if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
						//		{
						//			float money = 0;
						//			//TODO:工资系统，根据游戏时间来发工资
						//			//规则：工资=1分钟1G
						//			money = (float)(ingametime / 60 *(*i).second.Money);

						//			pPlayer->ModifyMoney(int32(money));      //满足条件获得金币
						//			pPlayer->SetInGameTime(getMSTime());   //将在玩家在线时间归零
						//			pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-149,pPlayer),int32(money)/10000);
						//			break;

						//		}
						//	}
						//}
						//else
						//{
						//	pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-139,pPlayer));
						//}
						pPlayer->CLOSE_GOSSIP_MENU();
						break;
					}
				case 110://飞行坐骑
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.SpellID==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->CastSpell(pPlayer,(*i).second.SpellID,true);
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 111://宠物模型
					{
						if(pPlayer->getClass()==CLASS_HUNTER)
						{
							Pet* pet = pPlayer->GetPet();
							if(!pet||pet->getPetType()!=HUNTER_PET)
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-150,pPlayer));
								break;
							}
							if(!pPlayer->IsWithinDistInMap(pet, MAX_VISIBILITY_DISTANCE))
							{
								pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-151,pPlayer));
								break;
							}
							UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
							for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
							{
								if ((*i).second.SpellID==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
								{
									uint32 btelete=0;//不能传送
									if((*i).second.Money>0)
									{
										if(pPlayer->GetMoney() >= (*i).second.Money)
										{
											btelete=1;//需要money
										}
										else
										{
											WUZHU_Common::SendMoneyError(pPlayer);
											break;
										}
									}
									else
										if((*i).second.Integral>0)
										{
											if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
											{
												btelete=2;//需要积分
											}
											else
											{
												pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
												break;
											}
										}
										pet->SetDisplayId((*i).second.SpellID);
										pet->SetNativeDisplayId((*i).second.SpellID);
										pet->SetByteValue(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED);
										//pPlayer->CastSpell(pPlayer,(*i).second.SpellID,true);
										if(btelete==1)
										{
											pPlayer->ModifyMoney(0-(*i).second.Money);									 
										}
										else
										{
											if(btelete==2)
											{
												pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
											}
										}
										break;
								}
							}
						}
						else
						{
							pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-152,pPlayer));
							break;
						}						
					}
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 120://积分查询
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-45,pPlayer),pPlayer->GetVIP_Integral());
									if(btelete==1)
									{
										pPlayer->ModifyMoney(0-(*i).second.Money);									 
									}
									else
									{
										if(btelete==2)
										{
											pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
										}
									}
									break;
							}
						}
					}					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 130://灵魂使者
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = pSystemMgr.Teleport_Template_Map.begin(); i != pSystemMgr.Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(pPlayer->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										pPlayer->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(pPlayer->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
											break;
										}
									}
									pPlayer-> GetSession()->SendSpiritResurrect();
									if(btelete==1)
								 {
									 pPlayer->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 pPlayer->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;
				case 140://返回主菜单
					GossipDBTeleportHello(pPlayer,_Creature);
					break;
		}
	}
	return true;
}



void AddSC_WUZHU_DBTeleport() 
{ 
	Script *newscript; 

	newscript = new Script; 
	newscript->Name="WUZHUDBTeleport"; 
	newscript->pGossipHello = &GossipDBTeleportHello; 
	newscript->pGossipSelect = &GossipDBTeleportSelect;
	newscript->RegisterSelf(); 


	newscript = new Script; 
	newscript->Name="WUZHUDBItem"; 
	newscript->pItemUse= &GossipDBItemHello; 
	newscript->pGossipItemSelect = &GossipDBItemSelect; 
	newscript->RegisterSelf(); 
}