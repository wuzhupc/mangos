UPDATE creature_template SET ScriptName='' WHERE entry=23336;
UPDATE creature_template SET ScriptName='' WHERE entry=23069;
UPDATE creature_template SET ScriptName='' WHERE entry=23259;
UPDATE gameobject_template SET ScriptName='' WHERE entry=185916;

Delete from creature where id=23089;
DELETE FROM creature_linking_template WHERE entry IN (23089, 23197, 23226, 23498, 22996, 22997, 23375);
INSERT INTO creature_linking_template (entry, map, master_entry, flag, search_range) VALUES
(23089, 564, 22917, 4096, 0),
(23197, 564, 22917, 4096, 0),
(23226, 564, 22917, 4112, 0),
(23498, 564, 22917, 4112, 0),
(22996, 564, 22917, 4096, 0),
(22997, 564, 22917, 4096, 0),
(23375, 564, 22917, 4096, 0);
delete from spell_script_target where entry in (39635,39849);
insert into spell_script_target values
(39635, 1, 23448),
(39849, 1, 23448);
-- cage spells
delete from spell_script_target where entry in (40704, 40707, 40708, 40709, 40710, 40711, 40712, 40713);
insert into spell_script_target values
(40704, 1, 23292),
(40707, 1, 23293),
(40708, 1, 23294),
(40709, 1, 23295),
(40710, 1, 23296),
(40711, 1, 23297),
(40712, 1, 23298),
(40713, 1, 23299);