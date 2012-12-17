-- ------------------------------------- --
-- ------- I am a temporary file ------- --
-- After a month my data will be deleted --
-- ------------------------------------- --
-- ===================================== --

-- ---------- --
-- 2012-11-26 --
-- ---------- --
/*
mob_erekem_guard -> npc_erekem_guard
npc_violet_portal -> npc_teleportation_portal
npc_door_seal_vh -> npc_door_seal
mob_soul_fragment -> npc_corrupted_soul_fragment
boss_ragefire -> boss_saviana
boss_colossus -> boss_drakkari_colossus
boss_drakari_elemental -> boss_drakkari_elemental
*/
UPDATE creature_template SET ScriptName = 'npc_erekem_guard' WHERE `entry`= 29395;
UPDATE creature_template SET ScriptName = 'npc_teleportation_portal' WHERE `entry` = 31011;
UPDATE creature_template SET ScriptName = 'npc_door_seal' WHERE `entry` = 30896;
UPDATE creature_template SET ScriptName = 'npc_corrupted_soul_fragment' WHERE `entry` = 36535;
UPDATE creature_template SET ScriptName = 'boss_saviana' WHERE `entry` = 39747;
UPDATE creature_template SET ScriptName = 'boss_drakkari_colossus' WHERE `entry` = 29307;
UPDATE creature_template SET ScriptName = 'boss_drakkari_elemental' WHERE `entry` = 29573;

-- ---------- --
-- 2012-11-27 --
-- ---------- --
-- sd2_mr0518
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = '' WHERE `entry` = 28822;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = '' WHERE `entry` = 28819;
UPDATE `creature_template` SET `AIName` = '', `ScriptName` = '' WHERE `entry` = 28891;
UPDATE `item_template` SET `ScriptName` = '' WHERE `entry` = 39253;

-- sd2_mr0519
-- Mirror Immage
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry` = 31216;
-- DK Gargoyle
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = '27829';

-- ---------- --
-- 2012-11-28 --
-- ---------- --
-- sd2_mr0522
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` IN (27894, 28781);

-- [2766]
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 29175;

-- sd2_mr0523
UPDATE `gameobject_template` SET `data0` = 58420 WHERE `entry` = 193053;
UPDATE `gameobject_template` SET `data0` = 58418 WHERE `entry` = 193052;
UPDATE `gameobject_template` SET `ScriptName` = '' WHERE `entry` IN (193052, 193053);

-- sd2_mr0528
DELETE FROM `spell_script_target` WHERE `entry` = 45063;

-- ---------- --
-- 2012-12-01 --
-- ---------- --
-- sd2_mr0530
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` IN(33051, 33705);

-- ---------- --
-- 2012-12-09 --
-- ---------- --
-- sd2_mr0536
UPDATE `creature_model_info` SET `bounding_radius` = 0.31, `combat_reach` = 20 WHERE `modelid` = 28638; -- temp
