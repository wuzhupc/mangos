#include "precompiled.h"
#include "sc_gossip.h"
#include "SharedDefines.h"
#include "Timer.h"
#include "WUZHU.h"

bool GossipHello_Integral(Player *pPlayer, Creature *_Creature)
{ 
	if(pPlayer->isInCombat())
	{ 
		pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-4,pPlayer));//" |cffff0000无法在战斗中访问，请停止战斗再访问.|r ");
		pPlayer->CLOSE_GOSSIP_MENU();
		//return false;
	}
	else
	{
		if (!pPlayer->isDead())
		{ 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-5,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-6,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 7);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-7,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-8,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-9,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-10,pPlayer), 160, GOSSIP_ACTION_INFO_DEF);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-11,pPlayer), 150, GOSSIP_ACTION_INFO_DEF);
			pPlayer->SEND_GOSSIP_MENU(88800,_Creature->GetObjectGuid());
		}
	}
	return true;
}

void SendDefaultMenu_Integral(Player *pPlayer, Creature *_Creature, uint32 action)
{
	switch(action) {
		case GOSSIP_ACTION_INFO_DEF+3://降级
			if(pPlayer->getLevel()>10)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-12,pPlayer), 155, GOSSIP_ACTION_INFO_DEF + 10);
			if(pPlayer->getLevel()>1)
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-13,pPlayer), 155, GOSSIP_ACTION_INFO_DEF + 1);
			else
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-14,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
			}
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-15,pPlayer), 140, GOSSIP_ACTION_INFO_DEF);
			pPlayer->SEND_GOSSIP_MENU(88803,_Creature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF+4://物品兑换积分 一公正徽章换一千积分
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-16,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 1);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-18,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 2);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-17,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-19,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-20,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-21,pPlayer), 151, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-15,pPlayer), 140, GOSSIP_ACTION_INFO_DEF);
			pPlayer->SEND_GOSSIP_MENU(88803,_Creature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF+5://积分购买荣誉点数
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-22,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 1);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-23,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-24,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 2);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-25,pPlayer), 152, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-15,pPlayer), 140, GOSSIP_ACTION_INFO_DEF);
			pPlayer->SEND_GOSSIP_MENU(88803,_Creature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF+6://积分购买技能点数
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-27,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 1);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-28,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 2);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-29,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 3);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-30,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 4);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-31,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 5);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-32,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 6);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-33,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 7);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-34,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 8);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-35,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 9);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-36,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 10);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-37,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 11);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-38,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 12);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-39,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 13);
			if(pPlayer->getClass()==CLASS_ROGUE)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-40,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 21);
			}
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-41,pPlayer), 153, GOSSIP_ACTION_INFO_DEF + 22);					
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-15,pPlayer), 140, GOSSIP_ACTION_INFO_DEF);
			pPlayer->SEND_GOSSIP_MENU(88803,_Creature->GetObjectGuid());
			break;
		case GOSSIP_ACTION_INFO_DEF+7://积分升级
			if(pPlayer->getLevel()>=80)
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-42,pPlayer), 1, GOSSIP_ACTION_INFO_DEF + 3);
			}
			else
			{
				//升级所需积分：(升级后等级-目前等级)*(80-目前等级)*1000						
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
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-15,pPlayer), 140, GOSSIP_ACTION_INFO_DEF);
			pPlayer->SEND_GOSSIP_MENU(88803,_Creature->GetObjectGuid());
			break;
				
	}
}

bool GossipSelect_Integral(Player *pPlayer, Creature *_Creature, uint32 sender, uint32 action )
{
	if (!isstone(pPlayer))
	{
		switch(sender) {
			// 主选单
				case GOSSIP_SENDER_MAIN :
					SendDefaultMenu_Integral(pPlayer, _Creature, action);
					break;					
					
				case 140://返回主菜单
					GossipHello_Integral(pPlayer,_Creature);
					break;
				case 150://积分查询					
					pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-45,pPlayer),pPlayer->GetVIP_Integral());			
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
							wpid=WP_YXWZID;
							wpcount=10;
							addintegral=10000;
							break;
						case GOSSIP_ACTION_INFO_DEF+2://英雄纹章100换100000
							wpid=WP_YXWZID;
							wpcount=100;
							addintegral=100000;
							break;
						case GOSSIP_ACTION_INFO_DEF+3://勇气纹章10换20000
							wpid=WP_YQWZID;
							wpcount=10;
							addintegral=20000;
							break;
						case GOSSIP_ACTION_INFO_DEF+4://勇气纹章100换200000
							wpid=WP_YQWZID;
							wpcount=100;
							addintegral=200000;
							break;
						case GOSSIP_ACTION_INFO_DEF+5://积分币10换20000
							wpid=WP_JFBID;
							wpcount=10;
							addintegral=20000;
							break;
						case GOSSIP_ACTION_INFO_DEF+6://积分币100换200000
							wpid=WP_JFBID;
							wpcount=100;
							addintegral=200000;
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
							addHoner=200;
							break;
						case GOSSIP_ACTION_INFO_DEF+2://10w->1w
							needintegral=100000;
							addHoner=20000;
							break;
						case GOSSIP_ACTION_INFO_DEF+3://100W->10W
							needintegral=1000000;
							addHoner=200000;
							break;
						case GOSSIP_ACTION_INFO_DEF+4://1w->1000
							needintegral=10000;
							addHoner=2000;
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
					if(pPlayer->GetVIP_Integral()<UPSkillIntegral_50)
					{
						pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-48,pPlayer));
					}
					else
					{
						bool baddsuccess=false;
						switch (action)
						{
						case GOSSIP_ACTION_INFO_DEF+1://增加采矿186 SKILL_MINING 参考\game\SharedDefines.h enum SkillType
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_MINING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+2://增加炼金171
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_ALCHEMY,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+3://增加锻造164
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_BLACKSMITHING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+4://增加裁缝197
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_TAILORING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+5://增加烹饪185
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_COOKING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+6://增加制皮165
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_LEATHERWORKING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+7://增加钓鱼356
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_FISHING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+8://增加附魔333
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_ENCHANTING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+9://增加珠宝加工755
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_JEWELCRAFTING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+10://增加工程学202
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_ENGINEERING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+11://增加急救129
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_FIRST_AID,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+12://增加草药182
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_HERBALISM,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+13://增加剥皮393
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_SKINNING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+21://增加开锁633
							baddsuccess=WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_LOCKPICKING,50);
							break;
						case GOSSIP_ACTION_INFO_DEF+22://增加武器和防御
							{
								uint16 successcount=0;
								if(WUZHU_Common::WZ_UpdateSkill(pPlayer,SKILL_DEFENSE,50))
								{
									successcount++;
									// update dependent from defense skill part
									pPlayer->UpdateDefenseBonusesMod();
								}
								for(int i=0;i<MAX_ITEM_SUBCLASS_WEAPON;i++)
								{
									if(item_weapon_skills[i]!=0)
										if(WUZHU_Common::WZ_UpdateSkill(pPlayer,item_weapon_skills[i],50))
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
							if(!pPlayer->isGameMaster())
								pPlayer->ModifyVIP_Integral(-UPSkillIntegral_50);
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
				case 155://积分降级
					if(pPlayer->getLevel()<=1)
					{
						pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-14,pPlayer));
					}
					else
					{
						uint32 newlevel=pPlayer->getLevel();
						uint32 nowlevle=newlevel;
						switch (action)
						{
						case GOSSIP_ACTION_INFO_DEF+1:
							newlevel=1;
							break;
						case GOSSIP_ACTION_INFO_DEF+10:
							if(pPlayer->getLevel()>10)
								newlevel=pPlayer->getLevel()-10;
							else
								newlevel=1;
							break;
						}
						pPlayer->ModifyVIP_Integral((nowlevle-newlevel)*15);//1等级获得15积分
						//if(newlevel==1)//当为1级时，直接设置为非VIP
						//	pPlayer->SetVIP(0);
						pPlayer->GiveLevel(newlevel);
						pPlayer->InitTalentForLevel();
						pPlayer->SetUInt32Value(PLAYER_XP,0);
						pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-53,pPlayer),pPlayer->getLevel());						
					}
					break;
				case 160://购买积分	提示
					pPlayer->GetSession()->SendAreaTriggerMessage(pPlayer->GetWUZHUString(WUZHU_TEXT_SOURCE_RANGE-54,pPlayer));					
					pPlayer->CLOSE_GOSSIP_MENU();
					break;

		} 
	}
	return true;
}


void AddSC_WUZHU_Integral() 
{ 
	Script *newscript; 

	newscript = new Script; 
	newscript->Name="WUZHUIntegral"; 
	newscript->pGossipHello = &GossipHello_Integral; 
	newscript->pGossipSelect = &GossipSelect_Integral;
	newscript->RegisterSelf(); 
}