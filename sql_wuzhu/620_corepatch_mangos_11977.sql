ALTER TABLE db_version CHANGE COLUMN required_11947_01_mangos_dbscripts required_11953_01_mangos_playercreateinfo_spell bit;

DELETE FROM playercreateinfo_spell WHERE Spell=21009;
INSERT INTO playercreateinfo_spell (race,class,Spell,Note) VALUES
(4,6,21009,'Elusiveness'),
(4,11,21009,'Elusiveness');

ALTER TABLE db_version CHANGE COLUMN required_11953_01_mangos_playercreateinfo_spell required_11955_01_mangos_mangos_string bit;

DELETE FROM mangos_string WHERE entry IN (555,556);

ALTER TABLE db_version CHANGE COLUMN required_11955_01_mangos_mangos_string required_11955_02_mangos_command bit;

DELETE FROM command WHERE name LIKE 'hover';

ALTER TABLE db_version CHANGE COLUMN required_11955_02_mangos_command required_11958_01_mangos_mangos_string bit;

DELETE FROM mangos_string WHERE entry=817;
INSERT INTO mangos_string VALUES (817,'Warning: You\'ve entered a no-fly zone and are about to be dismounted!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

# 11964_01_mangos_conditions.sql
ALTER TABLE db_version CHANGE COLUMN required_11958_01_mangos_mangos_string required_11964_01_mangos_conditions bit;

DROP TABLE IF EXISTS `conditions`;
CREATE TABLE `conditions` (
  `condition_entry` mediumint(8) unsigned NOT NULL auto_increment COMMENT 'Identifier',
  `type` tinyint(3) signed NOT NULL DEFAULT '0' COMMENT 'Type of the condition',
  `value1` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'data field one for the condition',
  `value2` mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'data field two for the condition',
  PRIMARY KEY  (`condition_entry`),
  CONSTRAINT unique_conditions UNIQUE (type,value1,value2)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=FIXED COMMENT='Condition System';

ALTER TABLE gossip_menu_option ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER cond_3_val_2;
ALTER TABLE gossip_menu ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER cond_2_val_2;

ALTER TABLE reference_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE creature_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE gameobject_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE pickpocketing_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE item_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE fishing_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE skinning_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE disenchant_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE mail_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE prospecting_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE spell_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;
ALTER TABLE milling_loot_template ADD COLUMN `condition_id` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER condition_value2;

# 11968_01_mangos_creature_linking_template.sql
ALTER TABLE db_version CHANGE COLUMN required_11964_01_mangos_conditions required_11968_01_mangos_creature_linking_template bit;
ALTER TABLE creature_linking_template ADD COLUMN search_range MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0'  AFTER flag;


# SD2_2534
UPDATE creature_template SET ScriptName='' WHERE entry=2708;
UPDATE creature_template SET ScriptName='' WHERE entry=4488;
UPDATE creature_template SET ScriptName='' WHERE entry IN (11056,11057);
UPDATE creature_template SET ScriptName='' WHERE entry=20201;
DELETE FROM scripted_event_id WHERE id=4884;
INSERT INTO scripted_event_id VALUES
(4884,'event_spell_altar_emberseer');
UPDATE creature_template SET ScriptName='spell_dummy_npc' WHERE entry IN (8888,9299,26924);
UPDATE creature_template SET ScriptName='npc_spirit_shade' WHERE entry=15261;
UPDATE creature_template SET ScriptName='npc_spring_rabbit' WHERE entry=32791;
UPDATE creature_template SET ScriptName='npc_redemption_target' WHERE entry IN (6172,6177,17542,17768);
