/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
* This program is free software licensed under GPL version 2
* Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_SYSTEM_H
#define SC_SYSTEM_H

extern DatabaseType SD2Database;
extern std::string  strSD2Version;                          //version info: database entry and revision

#define TEXT_SOURCE_RANGE -1000000                          //the amount of entries each text source has available

#define TEXT_SOURCE_TEXT_START      TEXT_SOURCE_RANGE
#define TEXT_SOURCE_TEXT_END        TEXT_SOURCE_RANGE*2 + 1

#define TEXT_SOURCE_CUSTOM_START    TEXT_SOURCE_RANGE*2
#define TEXT_SOURCE_CUSTOM_END      TEXT_SOURCE_RANGE*3 + 1

#define TEXT_SOURCE_GOSSIP_START    TEXT_SOURCE_RANGE*3
#define TEXT_SOURCE_GOSSIP_END      TEXT_SOURCE_RANGE*4 + 1

struct ScriptPointMove
{
    uint32 uiCreatureEntry;
    uint32 uiPointId;
    float  fX;
    float  fY;
    float  fZ;
    uint32 uiWaitTime;
};

struct StringTextData
{
    uint32 uiSoundId;
    uint8  uiType;
    uint32 uiLanguage;
    uint32 uiEmote;
};

//wuzhu start
struct WUZHU_GameTele
{
	uint32 target_mapId;
	float  target_X;
	float  target_Y;
	float  target_Z;
	float  target_Orientation;
};

struct WUZHU_Teleport_Template
{	
	uint32 Type;//Type 取值 功能说明 同一个菜单当Type设置为3时，将在传送师与宝石里同时出现
	//0 注销本菜单  		1 传送师专用菜单  		2 宝石专用菜单  		3 传送师与宝石公用菜单 
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
	111 宠物外观 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 
	120 积分查看 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 	
	130 原地复活 可以规定等级，消费的资金，VIP，阵营，等作为作用条件，GM不受限止。 	
	140 返回首页 返回首页的链接 ，只用于传送师

	200 版本信息 关于游戏版本 	此功能可以由普通菜单来完成　相当创建一个子菜单，点击菜单再返回上一及，菜单内容即为显示内容。
	80 技能商城
	或NPC技能师 可以规定等级，VIP，阵营，等作为条件，GM不受限止。当设置为宝石使用时在item_trainer表配置，当PRG为110时。item_trainer表的ID对应9个职业取值为：1，2，3，4，5，7，8，9，11。依次对应角色的9个职业,1战士(Warrior)，2圣骑士(Paladin)，3猎人(Hunter)，4盗贼(Rogue)，5牧师(Priest)，7萨满祭司(Shaman)，8法师(Mage)，9术师(Warlock)，11德鲁伊(Druid)
	当为NPC使用时在npc_trainer表添加NPC记录，与其它技能师相同，一个NPC只能教一类技能，多类配置需要多个ID的NPC 
	*/
	uint32 Action;//多功能指向，其值的含意取决于PRG值
	//当PRG为1时，代表一个系列MenuClass值相等的子菜单类，当PRG为5时代表一个传送点，此传送点的具体信息参见主数据库的game_tele表。当PRG为70时代表一系列商品的分类，具体参见主数据库item_vendor表
	uint32 Level;//使用此菜单的最小等级控制，全局有效
	std::string Message;//当Money或Integral大于0时有效
	uint32 Money;//功能收取金币，全局有效
	uint32 Integral;//功能收取积分，当Money值为0时生效并全局有效
	uint32 PTeam;//功能阵营控制，对所有菜单有效 取值为：0所有阵营，469联盟，67部落。其它值无效，不属于角色阵营的菜单将不被列出 
	uint32 VIP;//功能的VIP控制，0不限止，1仅对VIP开放，当限止仅为VIP使用的菜单或功能，普通玩家将看不到，GM不受限止
	uint32 SpellID;//PRG值为110或111时本字段有效,110时为技能编号，111为宠物外观模型
	uint32 P_class;//职业限止，全局有效P_class; 
	//取值范围：1，2，3，4，5，7，8，9，11。依次对应角色的9个职业,1战士(Warrior)，2圣骑士(Paladin)，3猎人(Hunter)，4盗贼(Rogue)，5牧师(Priest)，7萨满祭司(Shaman)，8法师(Mage)，9术师(Warlock)，11德鲁伊(Druid)	
};

struct TeleportHolder
{
	TeleportHolder( uint32 i,WUZHU_Teleport_Template p) : TeleportId(i),Teleport(p) {}
	uint32 TeleportId;
	WUZHU_Teleport_Template Teleport;
	bool operator <(TeleportHolder item)
	{
		if(TeleportId<item.TeleportId)
			return true;
		else
			return false;
	};
}; 

struct WUZHU_FLY_DATA
{
	uint32 entry;
	uint32 minlevel;
	uint32 maxlevel;
	std::string name;
	uint32 Speed;
	uint32 Flags;
	uint32 AllowableClass;
	uint32 AllowableRace;
	uint32 itemid;
	uint32 spell;
};
//wuzhu end

#define pSystemMgr SystemMgr::Instance()

class SystemMgr
{
    public:
        SystemMgr();
        ~SystemMgr() {}

        static SystemMgr& Instance();

        //Maps and lists
        typedef UNORDERED_MAP<int32, StringTextData> TextDataMap;
        typedef UNORDERED_MAP<uint32, std::vector<ScriptPointMove> > PointMoveMap;
		//wuzhu start
		 UNORDERED_MAP<uint32, WUZHU_GameTele> GameTele_Map;//传送
		 UNORDERED_MAP<uint32, WUZHU_Teleport_Template> Teleport_Template_Map;//宝石菜单等
		//GameTele_Map    m_mGameTele_Map;
		//Teleport_Template_Map m_mTeleport_Template_Map;
		//wuzhu end

        //Database
        void LoadVersion();
        void LoadScriptTexts();
        void LoadScriptTextsCustom();
        void LoadScriptGossipTexts();
        void LoadScriptWaypoints();
		//wuzhu start
		void LoadWUZHU_texts();
		void LoadGameTele_Map();
		void LoadTeleport_Template_Map();
		void LoadWUZHU_Fly_Data();
		uint32 LoadWUZHU_Fly_Data(int minlevel,int maxlevel,int flags);
		void FreeFly_Data();

		WUZHU_FLY_DATA *GetFlyData(uint32 level,int flags);
		//wuzhu end

        //Retrive from storage
        StringTextData const* GetTextData(int32 uiTextId) const
        {
            TextDataMap::const_iterator itr = m_mTextDataMap.find(uiTextId);

            if (itr == m_mTextDataMap.end())
                return NULL;

            return &itr->second;
        }

        std::vector<ScriptPointMove> const &GetPointMoveList(uint32 uiCreatureEntry) const
        {
            static std::vector<ScriptPointMove> vEmpty;

            PointMoveMap::const_iterator itr = m_mPointMoveMap.find(uiCreatureEntry);

            if (itr == m_mPointMoveMap.end())
                return vEmpty;

            return itr->second;
        }

    protected:
        TextDataMap     m_mTextDataMap;                     //additional data for text strings
        PointMoveMap    m_mPointMoveMap;                    //coordinates for waypoints

		//wuzhu start 100903
		WUZHU_FLY_DATA *m_mFlyData_S40;//40前坐骑
		uint32 m_uFlyDataMap_S40Count;
		WUZHU_FLY_DATA *m_mFlyData_S60;//60前坐骑
		uint32 m_uFlyDataMap_S60Count;
		WUZHU_FLY_DATA *m_mFlyData_S80;//80前坐骑
		uint32 m_uFlyDataMap_S80Count;
		WUZHU_FLY_DATA *m_mFlyData_T40;//40前飞行坐骑
		uint32 m_uFlyDataMap_T40Count;
		WUZHU_FLY_DATA *m_mFlyData_T60;//60前飞行坐骑
		uint32 m_uFlyDataMap_T60Count;
		WUZHU_FLY_DATA *m_mFlyData_T80;//80前飞行坐骑
		uint32 m_uFlyDataMap_T80Count;
		//wuzhu end
};

#endif
