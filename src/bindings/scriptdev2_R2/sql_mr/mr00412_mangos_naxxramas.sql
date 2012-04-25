-- ----------------------------- Achievemets: --------------------------------
-- Arachnophobia
DELETE FROM `achievement_criteria_requirement` WHERE `criteria_id` IN(7129,7128);
INSERT INTO `achievement_criteria_requirement` VALUES(7128,12,0,0),(7129,12,1,0);

-- And The Would All Go Down Together
DELETE FROM `achievement_criteria_requirement` WHERE `criteria_id` IN(7600,7601);
INSERT INTO `achievement_criteria_requirement` VALUES(7600,12,0,0),(7600,18,0,0),(7601,12,1,0),(7601,18,0,0);

-- The Military Quarter (Horsemen)
DELETE FROM `achievement_criteria_requirement` WHERE `criteria_id` IN(7192,7193);
INSERT INTO `achievement_criteria_requirement` VALUES(7192,12,0,0),(7193,12,1,0);

-- Make Quick Werk of Him
DELETE FROM `achievement_criteria_requirement` WHERE `criteria_id` IN(7126,7127);
INSERT INTO `achievement_criteria_requirement` VALUES(7126,12,0,0),(7127,12,1,0);