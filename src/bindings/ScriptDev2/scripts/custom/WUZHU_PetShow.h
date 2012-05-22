#include "precompiled.h"
#include "Database/DatabaseMysql.h"
#include "Database/DBCStores.h"
#include "../../../shared/Config/Config.h"
#include "sc_gossip.h"
#include "SharedDefines.h"
#include "Timer.h"
#include "Item.h"
#include "sc_creature.h"
#include "SpellMgr.h"
#include "Spell.h" 
#include "WUZHU.h"

struct WUZHU_PetShowData
{
	uint32 MenuClass;//菜单的类别
	//保留值0，为0值的菜单将做为首页菜单出现，其它值自定义。
	uint32 Icon;//菜单前面的图标(0-15)
	std::string Title;//菜单的文字内容
	uint32 PRG;//功能入口
	/*
	取值 功能说明 具体说明 这是一个比较关键的字值，取值1为父菜单，就是还有下一级子菜单的定义，当取此值时，此菜单被点击后将列出所有Action字段值等于MenuClass值代表的所有的子菜单，也就是我们常说的第二类及第三类子菜单等。
	所有功能对传送宝石和传送师都支持，只是两者的商店和技能商城的配置库和配置方法有所不同

	5 传送点 使用主数据库的game_tele表的的记录ID值等于本表Action的记录进行传送，本表拥有上千个传送点供大家自定义选择使用，足以传送到世界各个角落。不需要重定义表，这将很大程序减少系统资源的使用。 
	10 激活飞行点 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 
	20 银行 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 
	22 绑定出生炉石 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。
	40 修理装备耐久 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 
	70 宝石商城(只用于宝石)
	宝石商店物品分类在item_vendor表配置。当PRG为70时，Action值分别对应主数据库里的表item_vendor的entry值，每一个值代表一个分类，可以再添加分类，删除和修改等。	
	90 在线工资 可以规定等级，对应的money表示1分钟领取的资金，VIP，阵营，等作为作用条件，GM不受限止。 
	110 坐骑和临时技能 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 
	120 积分查看 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 	
	130 原地复活 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 	
	140 返回首页 返回首页的链接 ，只用于传送师

	200 版本信息 关于游戏版本 	此功能可以由普通菜单来完成　相当创建一个子菜单，点击菜单再返回上一及，菜单内容即为显示内容。
	80 技能商城
	或NPC技能师 可以规定等级，VIP，阵营，等作为条件，GM不受限止。当设置为宝石使用时在item_trainer表配置，当PRG为110时。item_trainer表的ID对应9个职业取值为：1，2，3，4，5，7，8，9，11。依次对应角色的9个职业,1战士(Warrior)，2圣骑士(Paladin)，3猎人(Hunter)，4盗贼(Rogue)，5牧师(Priest)，7萨满祭司(Shaman)，8法师(Mage)，9术师(Warlock)，11德鲁伊(Druid)
	当为NPC使用时在npc_trainer表添加NPC记录，与其它技能师相同，一个NPC只能教一类技能，多类配置需要多个ID的NPC 
	*/
	uint32 PetShowId;//PRG=110时有效 显示的ID;MenuClass<>0时为下一页的索引号;MenuClass==0时为首页标题，如果pageindex
	std::string Title;//标题信息
	std::string Message;//提示信息
};

struct PetShowHolder
{
	PetShowHolder( uint32 i,WUZHU_PetShowData p) : PetShowId(i),PetShow(p) {}
	uint32 PetShowId;
	WUZHU_PetShowData PetShow;
	bool operator <(PetShowHolder item)
	{
		if(PetShowId<item.PetShowId)
			return true;
		else
			return false;
	};
}; 


extern  UNORDERED_MAP<uint32, WUZHU_PetShowData> PetShow_Map;//传送 


bool GossipHello_ItemPetShow(Player *player, Item* _Item, SpellCastTargets const& targets)
{
	if(player->isInCombat())
	{ 
		player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在战斗中使用宝石.|r ");
		player->CLOSE_GOSSIP_MENU();
	}
	else
	if(player->isInFlight()||player->IsMounted())
	{ 
		player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在坐骑中使用宝石.|r ");
		player->CLOSE_GOSSIP_MENU();
	}
	else
	{
		if (!player->isDead()&&player->getClass()=CLASS_HUNTER)
		{ 
			UNORDERED_MAP<uint32, WUZHU_PetShowData>::iterator i;
			std::list<PetShowHolder> PetShowList;
			PetShowList.clear();

			uint32 count=0;//超过16个就不处理
			bool baddmenu=true;
			for (i = PetShow_Map.begin(); i != PetShow_Map.end(); ++i)
			{
				//outstring_log("wuzhu %d %s",(*i).first,(*i).second.Title);
				baddmenu=true;
				if(count>=16)
					break;				
				if ((*i).second.MenuClass==0&&(*i).second.PRG!=130&&((*i).second.Type==2||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
				{
					if((*i).second.VIP>0)
					{
						if(player->GetVIP()==0)
							baddmenu=false;
					}
					if((*i).second.Level>player->getLevel())
						baddmenu=false;
					if((*i).second.P_class>0)
					{
						if((*i).second.P_class!=player->getClass())
							baddmenu=false;
					}
					if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
					{
						if(!isbl(player))
							baddmenu=false;
					}
					else
					if((*i).second.PTeam==469)
					{
						if(isbl(player))
							baddmenu=false;
					}	
					if(baddmenu||player->isGameMaster())
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
						player->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
					else
					{
						if(th->Teleport.PRG==110)
							player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money,false);
						else
							player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money,false);
					}
				}	
			}
		}
		player->SEND_GOSSIP_MENU(80000,_Item->GetGUID());
	}

	return true;
}

void SendDefaultMenu_ItemPetShow(Player *player, Item *_Item, uint32 action)
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if(player->isInCombat())
	{ 
		player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在战斗中使用宝石.|r ");
		player->CLOSE_GOSSIP_MENU();
	}
	else
	if(player->isInFlight()||player->IsMounted())
	{ 
		player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在坐骑中使用宝石.|r ");
		player->CLOSE_GOSSIP_MENU();
	}
	else
	{
		if (!player->isDead())
		{ 
			UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
			std::list<TeleportHolder> TeleportList;
			TeleportList.clear();
			uint32 count=0;//超过16个就不处理
			bool baddmenu=true;
			for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
			{
				baddmenu=true;
				if(count>=16)
					break;				
				if ((*i).second.MenuClass==baseaction&&(*i).second.PRG!=130&&((*i).second.Type==2||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
				{
					if((*i).second.VIP>0)
					{
						if(player->GetVIP()==0)
							baddmenu=false;
						//outstring_log("wuzhu vip %d %d",(*i).second.VIP,player->GetVIP());
					}
					if((*i).second.Level>player->getLevel())
					{
						baddmenu=false;
						//outstring_log("wuzhu Level %d %d",(*i).second.Level,player->getLevel());
					}
					if((*i).second.P_class>0)
					{
						if((*i).second.P_class!=player->getClass())
							baddmenu=false;
						//outstring_log("wuzhu P_class %d %d",(*i).second.P_class,player->getClass());
					}
					if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
					{
						if(!isbl(player))
							baddmenu=false;
					}
					else
					{
						if((*i).second.PTeam==469)
						{
							if(isbl(player))
								baddmenu=false;
						}	
					}
					//outstring_log("wuzhu PTeam %d %d",(*i).second.PTeam,player->getRace());
					if(baddmenu||player->isGameMaster())
					{
						++count;
						TeleportList.push_back(TeleportHolder((*i).first, (*i).second));
						/*if((*i).second.PRG==90)
							player->ADD_GOSSIP_ITEM((*i).second.Icon,(*i).second.Title,(*i).second.PRG,GOSSIP_ACTION_INFO_DEF+(*i).second.Action);
						else
						{
							if((*i).second.PRG==110)
								player->ADD_GOSSIP_ITEM_EXTENDED((*i).second.Icon,(*i).second.Title,(*i).second.PRG,GOSSIP_ACTION_INFO_DEF+(*i).second.SpellID,(*i).second.Message,(*i).second.Money);
							else
								player->ADD_GOSSIP_ITEM_EXTENDED((*i).second.Icon,(*i).second.Title,(*i).second.PRG,GOSSIP_ACTION_INFO_DEF+(*i).second.Action,(*i).second.Message,(*i).second.Money);
						}*/
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
						player->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
					else
					{
						if(th->Teleport.PRG==110)
							player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money,false);
						else
							player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money,false);
					}
				}	
			}
		}		
		player->SEND_GOSSIP_MENU(80006,_Item->GetGUID());
	}
}

bool GossipSelect_ItemPetShow(Player *player, Item *_Item, uint32 sender, uint32 action )
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if (!isstone(player))
	{
		switch(sender) {
			// 主选单
				case GOSSIP_SENDER_MAIN :
					SendDefaultMenu_DBItem(player, _Item, action);
					break;
			//传送 
				case 5 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
								if((*i).second.Integral>0)
								{
									if(player->GetVIP_Integral() >= (*i).second.Integral)
									{
										btelete=2;//需要积分
									}
									else
									{
										player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
										break;
									}
								}
								UNORDERED_MAP<uint32, WUZHU_GameTele>::iterator j = GameTele_Map.find(baseaction);
								if(j!=GameTele_Map.end())
								{
									player->TeleportTo((*j).second.target_mapId,(*j).second.target_X,
										(*j).second.target_Y,(*j).second.target_Z,(*j).second.target_Orientation);
									if(btelete==1)
										player->ModifyMoney(0-(*i).second.Money);
									else
										if(btelete==2)
											player->ModifyVIP_Integral(0-(*i).second.Integral);
								} 
								break;
							}
						}
					}
					player->CLOSE_GOSSIP_MENU();
					break;
					//开飞行点
				case 10:
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
								 player->SetTaxiCheater(true);
								 if(btelete==1)
								 {
									player->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
									 if(btelete==2)
								 {
									 player->ModifyVIP_Integral(0-(*i).second.Integral);
								 }
								 player->GetSession()->SendAreaTriggerMessage(" |cff2040CC已经开通所有飞行点.|r ");
								 break;
							}
						}
					}
					player->CLOSE_GOSSIP_MENU();
					break;
					//银行
				case 20 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									player->SEND_BANKERLIST(_Item->GetGUID() );
									if(btelete==1)
								 {
									 player->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 player->ModifyVIP_Integral(0-(*i).second.Integral);
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
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									player->GetSession()->SendBindPoint();
									if(btelete==1)
								 {
									 player->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 player->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					player->CLOSE_GOSSIP_MENU();
					break;	
				case 40://修理装备
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									player->DurabilityRepairAll(true,1,false);
									if(btelete==1)
								 {
									 player->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 player->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					player->CLOSE_GOSSIP_MENU();
					break;
				case 70://商店
					player->GetSession()->OpenShop(_Item->GetGUID(),action-GOSSIP_ACTION_INFO_DEF);
					player->CLOSE_GOSSIP_MENU();
					break;
				case 90://领取工资
					//获取当前时间
					{
						uint32 ingametime=getMSTimeDiff(player->GetInGameTime(),getMSTime())/1000;
						if(ingametime > 60) // 条件检查你在线时间大于1分钟．这可以自己改(单位:秒)
						{
							UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
							for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
							{
								if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
								{
									float money = 0;
									//TODO:工资系统，根据游戏时间来发工资
									//规则：工资=1分钟1G
									money = (float)(ingametime / 60 *(*i).second.Money);

									player->ModifyMoney(int32(money));      //满足条件获得金币
									player->SetInGameTime(getMSTime());   //将在玩家在线时间归零
									player->GetSession()->SendAreaTriggerMessage(" |cff7000ff领取了工资：%d金币.|r ",int32(money)/10000);
									break;

								}
							}
						}
						else
						{
							player->GetSession()->SendAreaTriggerMessage(" |cffff0000领取工资时间间隔太短.|r ");
						}
						player->CLOSE_GOSSIP_MENU();
						break;
					}
				case 110://飞行坐骑
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.SpellID==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									player->CastSpell(player,(*i).second.SpellID,true);
									if(btelete==1)
								 {
									 player->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 player->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					player->CLOSE_GOSSIP_MENU();
					break;
				case 120://积分查询
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									player->GetSession()->SendAreaTriggerMessage(" |cffaa0080你当前共有积分：%u.|r ",player->GetVIP_Integral());
									if(btelete==1)
									{
										player->ModifyMoney(0-(*i).second.Money);									 
									}
									else
									{
										if(btelete==2)
										{
											player->ModifyVIP_Integral(0-(*i).second.Integral);
										}
									}
									break;
							}
						}
					}					
					player->CLOSE_GOSSIP_MENU();
					break;
				case 130://灵魂使者
					{
						if(!player->isDead())
							break;
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==2||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										WUZHU_Common::SendMoneyError(player);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									player-> GetSession()->SendSpiritResurrect();
									if(btelete==1)
								 {
									 player->ModifyMoney(0-(*i).second.Money);									 
								 }
								 else
								 {
									 if(btelete==2)
									 {
										 player->ModifyVIP_Integral(0-(*i).second.Integral);
									 }
								 }
									break;
							}
						}
					}					
					player->CLOSE_GOSSIP_MENU();
					break;
		}
	}
	return true;
}












bool GossipHello_NPCPetShow(Player *player, Creature *_Creature)
{
	if(player->isInCombat())
	{ 
		player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在战斗中使用宝石.|r ");
		player->CLOSE_GOSSIP_MENU();
	}
	else
		if(player->isInFlight()||player->IsMounted())
		{ 
			player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在坐骑中使用宝石.|r ");
			player->CLOSE_GOSSIP_MENU();
		}
		else
		{
			if (!player->isDead())
			{ 
				UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
				std::list<TeleportHolder> TeleportList;
				TeleportList.clear();
				uint32 count=0;//超过16个就不处理
				bool baddmenu=true;
				for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
				{
					if(count>=16)
						break;				
					if ((*i).second.MenuClass==0&&(*i).second.PRG!=130&&((*i).second.Type==1||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
					{
						baddmenu=true;
						if((*i).second.VIP>0)
						{
							if(player->GetVIP()==0)
								baddmenu=false;
						}
						if((*i).second.Level>player->getLevel())
							baddmenu=false;
						if((*i).second.P_class>0)
						{
							if((*i).second.P_class!=player->getClass())
								baddmenu=false;
						}
						if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
						{
							if(!isbl(player))
								baddmenu=false;
						}
						else
							if((*i).second.PTeam==469)
							{
								if(isbl(player))
									baddmenu=false;
							}	
							if(baddmenu||player->isGameMaster())
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
							player->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
						else
						{
							if(th->Teleport.PRG==110)									
								player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money,false);
							else									
								player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money,false);
						}
					}
				}
				
			}
			else 
				if(player->isDead())
				{
					UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;

					bool baddmenu=true;
					for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
					{
						if ((*i).second.MenuClass==0&&(*i).second.PRG==130&&((*i).second.Type==1||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
						{
							baddmenu=true;
							if((*i).second.VIP>0)
							{
								if(player->GetVIP()==0)
									baddmenu=false;
							}
							if((*i).second.Level>player->getLevel())
								baddmenu=false;
							if((*i).second.P_class>0)
							{
								if((*i).second.P_class!=player->getClass())
									baddmenu=false;
							}
							if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
							{
								if(!isbl(player))
									baddmenu=false;
							}
							else
								if((*i).second.PTeam==469)
								{
									if(isbl(player))
										baddmenu=false;
								}	
								if(baddmenu||player->isGameMaster())
								{
									player->ADD_GOSSIP_ITEM_EXTENDED((*i).second.Icon,(*i).second.Title,(*i).second.PRG,GOSSIP_ACTION_INFO_DEF+(*i).second.Action,(*i).second.Message,(*i).second.Money,false);
								}

								break;
						}
					}
				} 
				player->SEND_GOSSIP_MENU(80001,_Creature->GetGUID());
		}

		return true;
}

void SendDefaultMenu_NPCPetShow(Player *player, Creature *_Creature, uint32 action)
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if(player->isInCombat())
	{ 
		player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在战斗中使用宝石.|r ");
		player->CLOSE_GOSSIP_MENU();
	}
	else
		if(player->isInFlight()||player->IsMounted())
		{ 
			player->GetSession()->SendAreaTriggerMessage(" |cffff0000无法在坐骑中使用宝石.|r ");
			player->CLOSE_GOSSIP_MENU();
		}
		else
		{
			if (!player->isDead())
			{ 
				UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
				std::list<TeleportHolder> TeleportList;
				TeleportList.clear();
				uint32 count=0;//超过16个就不处理
				bool baddmenu=true;
				for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
				{
					baddmenu=true;
					if(count>=16)
						break;				
					if ((*i).second.MenuClass==baseaction&&(*i).second.PRG!=130&&((*i).second.Type==1||(*i).second.Type==3))//当MC=0并用type=1和3时才显示
					{
						if((*i).second.VIP>0)
						{
							if(player->GetVIP()==0)
								baddmenu=false;
						}
						if((*i).second.Level>player->getLevel())
							baddmenu=false;
						if((*i).second.P_class>0)
						{
							if((*i).second.P_class!=player->getClass())
								baddmenu=false;
						}
						if((*i).second.PTeam==67)//469联盟，67部落 其它值则认为是全部都允许
						{
							if(!isbl(player))
								baddmenu=false;
						}
						else
							if((*i).second.PTeam==469)
							{
								if(isbl(player))
									baddmenu=false;
							}	
							if(baddmenu||player->isGameMaster())
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
							player->ADD_GOSSIP_ITEM(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action);
						else
						{
							if(th->Teleport.PRG==110)
								player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.SpellID,th->Teleport.Message,th->Teleport.Money,false);
							else
								player->ADD_GOSSIP_ITEM_EXTENDED(th->Teleport.Icon,th->Teleport.Title,th->Teleport.PRG,GOSSIP_ACTION_INFO_DEF+th->Teleport.Action,th->Teleport.Message,th->Teleport.Money,false);
						}
					}
				}
			}		
			player->SEND_GOSSIP_MENU(80007,_Creature->GetGUID());
		}
}

bool GossipSelect_NPCPetShow(Player *player, Creature *_Creature, uint32 sender, uint32 action )
{
	uint32 baseaction=action-GOSSIP_ACTION_INFO_DEF;
	if (!isstone(player))
	{
		switch(sender) {
			// 主选单
				case GOSSIP_SENDER_MAIN :
					SendDefaultMenu_NPCPetShow(player, _Creature, action);
					break;
					//传送 
				case 5 :
					{
						UNORDERED_MAP<uint32, WUZHU_Teleport_Template>::iterator i;
						for (i = Teleport_Template_Map.begin(); i != Teleport_Template_Map.end(); ++i)
						{
							if ((*i).second.Action==baseaction&&((*i).second.Type==1||(*i).second.Type==3))
							{
								uint32 btelete=0;//不能传送
								if((*i).second.Money>0)
								{
									if(player->GetMoney() >= (*i).second.Money)
									{
										btelete=1;//需要money
									}
									else
									{
										player->SendBuyError( BUY_ERR_NOT_ENOUGHT_MONEY, _Creature, 0, 0);
										break;
									}
								}
								else
									if((*i).second.Integral>0)
									{
										if(player->GetVIP_Integral() >= (*i).second.Integral)
										{
											btelete=2;//需要积分
										}
										else
										{
											player->GetSession()->SendAreaTriggerMessage(" |cffff0000你的积分不够.|r ");
											break;
										}
									}
									UNORDERED_MAP<uint32, WUZHU_GameTele>::iterator j = GameTele_Map.find(baseaction);
									if(j!=GameTele_Map.end())
									{
										player->TeleportTo((*j).second.target_mapId,(*j).second.target_X,
											(*j).second.target_Y,(*j).second.target_Z,(*j).second.target_Orientation);
										if(btelete==1)
											player->ModifyMoney(0-(*i).second.Money);
										else
											if(btelete==2)
												player->ModifyVIP_Integral(0-(*i).second.Integral);
									} 
									break;
							}
						}
					}
					player->CLOSE_GOSSIP_MENU();
					break;
				case 140://返回主菜单
					GossipHello_NPCPetShow(player,_Creature);
					break;
		}
	}
	return true;
}



void AddSC_WUZHU_PetShow() 
{ 
	Script *newscript; 

	newscript = new Script; 
	newscript->Name="WUZHU_ItemPetShow"; 
	newscript->pGossipHello = &GossipHello_ItemPetShow; 
	newscript->pGossipSelect = &GossipSelect_ItemPetShow;
	newscript->RegisterSelf(); 


	newscript = new Script; 
	newscript->Name="WUZHU_NPCPetShow"; 
	newscript->pItemUse= &GossipHello_NPCPetShow; 
	newscript->pGossipSelect_Item = &GossipSelect_NPCPetShow; 
	newscript->RegisterSelf(); 
}