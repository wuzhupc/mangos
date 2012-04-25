mangos_wuzhu
============

在mangosrs和scriptdev2_R2基础上增加自己的一些设置项，比如：积分、传送宝石、宠物变身、多位置绑定等

///wuzhu 配置文件
enum WUZHUConfig
{
	WUZHU_Integral,//普通用户积分
	WUZHU_Create_Integral,//出生积分
	WUZHU_Player_AURA_Healing,//治疗倍数
	WUZHU_Player_TMP_Damage,//法术伤害倍数
	WUZHU_Player_PDA_Damage,//物理伤害倍数
	WUZHU_Pet_TMP_Damage,//宠物的法术伤害倍数
	WUZHU_Pet_PDA_Damage,//宠物的物理伤害倍数
	WUZHU_Pet_Scale,//宠物的大小倍数,默认值1
	WUZHU_MaxXP,//能获取的最多经验
	WUZHU_MaxReputation,//能获取最多声望
	WUZHU_MOB_MAXHEA,//最多多少血
	WUZHU_MOB_HEAADD,//增加原血量的百份之多少,当怪的血少于WUHZU_MOB_MAXHEA时，血量还是原来的，如果大于，最后怪的血为 WUHZU_MOB_MAXHEA+怪原来的血量*WUZHU_MOB_HEAADD/100
	WUHZU_MoveSpeed,//移动倍数
	WUZHU_SCALE_X,//角色大小倍数
	WUZHU_Max_Deal,//最大伤害值
	WUZHU_Damage_Raid,//RAID时伤害值 WUZHU.Damage.Raid
	WUZHU_ScalingStat,//传家宝参数倍数 默认1 WUZHU.ScalingStat
	MAX_WUZHUConfig
};