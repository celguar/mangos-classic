-- Fix Staghelm Point (Silithus) exploration
UPDATE `achievement_criteria_dbc` SET `Asset_Id`='1022' WHERE  `ID`=1472;
-- Delete new zones from exploration
-- EK
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=1550;
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=1551;
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=1796;
-- Kalimdor
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=1600;
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=1601;
-- Ruins of Scarlet Enclave
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=8749;
-- Add Pathfinder Achievement for exploring old world
-- Set World Explorer achievement to wrath
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=46;
-- ghostlands
UPDATE `achievement_dbc` SET `patch`='01' WHERE  `ID`=858;
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Supercedes`, `Title_Lang_enUS`, `Title_Lang_enGB`, `Title_Lang_koKR`, `Title_Lang_frFR`, `Title_Lang_deDE`, `Title_Lang_enCN`, `Title_Lang_zhCN`, `Title_Lang_enTW`, `Title_Lang_zhTW`, `Title_Lang_esES`, `Title_Lang_esMX`, `Title_Lang_ruRU`, `Title_Lang_ptPT`, `Title_Lang_ptBR`, `Title_Lang_itIT`, `Title_Lang_Unk`, `Title_Lang_Mask`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Category`, `Points`, `Ui_Order`, `Flags`, `IconID`, `Reward_Lang_enUS`, `Reward_Lang_enGB`, `Reward_Lang_koKR`, `Reward_Lang_frFR`, `Reward_Lang_deDE`, `Reward_Lang_enCN`, `Reward_Lang_zhCN`, `Reward_Lang_enTW`, `Reward_Lang_zhTW`, `Reward_Lang_esES`, `Reward_Lang_esMX`, `Reward_Lang_ruRU`, `Reward_Lang_ptPT`, `Reward_Lang_ptBR`, `Reward_Lang_itIT`, `Reward_Lang_Unk`, `Reward_Lang_Mask`, `Minimum_Criteria`, `Shares_Criteria`, `patch`) VALUES
(47, -1, -1, 0, 'Pathfinder', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 'Explore Eastern Kingdoms and Kalimdor.', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 97, 50, 5, 0, 3720, '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 0, 0, 0);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('93', '47', '8', '42', '1', 'Eastern Kingdoms', '16712190', '1');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('92', '47', '8', '43', '1', 'Kalimdor', '16712190', '2');
-- Fix INVALID CRITERIA in Explore due to wrong order
-- EK
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='13' WHERE  `ID`=1291;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='14' WHERE  `ID`=1292;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='15' WHERE  `ID`=1293;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='16' WHERE  `ID`=1294;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='17' WHERE  `ID`=1295;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='18' WHERE  `ID`=1297;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='19' WHERE  `ID`=1298;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='20' WHERE  `ID`=1282;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='21' WHERE  `ID`=1296;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='22' WHERE  `ID`=1286;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='4' WHERE  `ID`=1489;
-- Kalimdor
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='5' WHERE  `ID`=1490;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='6' WHERE  `ID`=1491;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='7' WHERE  `ID`=1492;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='8' WHERE  `ID`=1493;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='9' WHERE  `ID`=1494;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='20' WHERE  `ID`=1495;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='11' WHERE  `ID`=1496;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='10' WHERE  `ID`=1495;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='12' WHERE  `ID`=1497;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='13' WHERE  `ID`=1498;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='14' WHERE  `ID`=1499;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='15' WHERE  `ID`=1500;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='16' WHERE  `ID`=1501;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='17' WHERE  `ID`=1502;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='18' WHERE  `ID`=1503;
-- Make PvP Rank achievements normal and superceded
-- UPDATE `achievement_dbc` SET `Supercedes`='442' WHERE  `ID`=470;
-- UPDATE `achievement_dbc` SET `Supercedes`='470' WHERE  `ID`=471;
-- UPDATE `achievement_dbc` SET `Supercedes`='471' WHERE  `ID`=441;
-- UPDATE `achievement_dbc` SET `Supercedes`='441' WHERE  `ID`=440;
-- UPDATE `achievement_dbc` SET `Supercedes`='440' WHERE  `ID`=439;

-- Add Custom 6 Bank Slots achievement
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`, `Reward_Lang_Mask`) VALUES ('548', '-1', '-1', 'Bank You Very Much', 'Buy 6 additional bank slots.', '92', '10', '11', '2', '16712174');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Flags`, `Ui_Order`) VALUES ('751', '548', '45', '6', 'Purchase 6 bank slots', '16712190', '1', '1');
-- Lock out 7 Bank Slots to wrath
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=546;
-- Cenarius Circle and Refuge reputation lock to tbc+
UPDATE `classicmangos`.`achievement_dbc` SET `patch`='1' WHERE  `ID`=953;

-- Emblem achievements are for wotlk
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID` IN (4785,4784,4729,4730,3838,3839,3840,3841,3842,3843,3844,3876,4316);

-- Champ of Frozen Wastes
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=1658;

-- Glory of Wotlk PvE
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=2136;
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=2137;
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=2138;

-- Visit Barbershop
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=545;

-- Grand Master Proffs (wotlk)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID` IN (125,130,135,734,1414,1415,1416,1417,1418,1419,1420,1421,1422,1423,1424,1425,1426,1427);
-- Master Proffs (tbc)
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID` IN (124,129,134,733);
-- Master Angler (booty bay or wotlk stuff)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=306;

-- Grand Master in 2 proff
-- lock wotlk achievement
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=735;
-- add new
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`, `Minimum_Criteria`) VALUES ('737', '-1', '-1', 'Working Overtime', 'Become an Artisan in two professions.', '169', '10', '6', '2840', '2');
-- add new criteria
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (9, 737, 40, 171, 6, 0, 0, 0, 0, 'Alchemy', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 1);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (10, 737, 40, 164, 6, 0, 0, 0, 0, 'Blacksmithing', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 2);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (11, 737, 40, 333, 6, 0, 0, 0, 0, 'Enchanting', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 3);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (12, 737, 40, 202, 6, 0, 0, 0, 0, 'Engineering', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 4);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (13, 737, 40, 182, 6, 0, 0, 0, 0, 'Herbalism', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 5);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (14, 737, 40, 165, 6, 0, 0, 0, 0, 'Leatherworking', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 6);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (15, 737, 40, 186, 6, 0, 0, 0, 0, 'Mining', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 7);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (16, 737, 40, 393, 6, 0, 0, 0, 0, 'Skinning', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 8);
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Start_Event`, `Start_Asset`, `Fail_Event`, `Fail_Asset`, `Description_Lang_enUS`, `Description_Lang_enGB`, `Description_Lang_koKR`, `Description_Lang_frFR`, `Description_Lang_deDE`, `Description_Lang_enCN`, `Description_Lang_zhCN`, `Description_Lang_enTW`, `Description_Lang_zhTW`, `Description_Lang_esES`, `Description_Lang_esMX`, `Description_Lang_ruRU`, `Description_Lang_ptPT`, `Description_Lang_ptBR`, `Description_Lang_itIT`, `Description_Lang_Unk`, `Description_Lang_Mask`, `Flags`, `Timer_Start_Event`, `Timer_Asset_Id`, `Timer_Time`, `Ui_Order`) VALUES (17, 737, 40, 197, 6, 0, 0, 0, 0, 'Tailoring', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 16712190, 2, 0, 0, 0, 9);

-- Skills to pay the bills (grand master fish, first aid, cook)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=730;

-- First Aid
-- Stock Up (frost bandage)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=137;
-- Replace Stock up
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`) VALUES ('138', '-1', '-1', 'Bandage Dispenser', 'Create 500 Heavy Runecloth Bandages.', '172', '10', '6', '1621');
-- criteria
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Flags`, `Ui_Order`) VALUES ('1833', '138', '29', '18630', '500', 'Create 500 Heavy Runecloth Bandages', '16712190', '1', '1');
-- Ultimate Triage (frost bandage)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=141;
-- Replace
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`) VALUES ('142', '-1', '-1', 'Medic!', 'Use a Heavy Runecloth Bandage to heal another player or yourself with less than 5% health.', '172', '10', '7', '3840');
-- criteria
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Flags`, `Ui_Order`) VALUES ('6801', '142', '110', '18610', '1', 'First Aid Below 5%', '16712190', '2', '1');
-- criteria data
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`) VALUES ('6801', '3', '5');

-- COOKING
-- tbc
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID` IN (1800);
-- wotlk achievements
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID` IN (1563,3296,2002,2001,2000,1999,1998,1801,1801,1784,1781,1782,1783,1777,1778,1779,906,877,1780);

-- FISHING
-- tbc
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID` IN (144,726,905,1225,1243,1257,1836,1837);
-- wotlk
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID` IN (1516,1517,1957,1958,2094,2095,2096,3217,3218);
-- TODO copy 1257 achievement with removed 1 criteroa that is TBC

-- QUESTS
-- Daily quest tbc
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID` IN (97,759,973,974,975,976,977,1525,1526,31,759);
-- Nesingwary
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=941;
-- Nagrand + Zul Drak arena
-- TODO Separate for tbc
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=1576;
-- Loremaster
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=1681;
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=1682;

-- FIRST REALM
-- First BE 80
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=1405;

-- PVP
-- Kill every class - remove DK and fix order
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=2359;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='1' WHERE  `ID`=2360;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='2' WHERE  `ID`=2361;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='3' WHERE  `ID`=2362;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='4' WHERE  `ID`=2363;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='5' WHERE  `ID`=2364;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='6' WHERE  `ID`=2365;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='7' WHERE  `ID`=2366;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='8' WHERE  `ID`=2367;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='9' WHERE  `ID`=2368;
-- Kill every race - remove blood elf
UPDATE `achievement_dbc` SET `Description_Lang_enUS`='Get an honorable, killing blow on four different races.' WHERE  `ID`=246;
UPDATE `achievement_dbc` SET `Description_Lang_enUS`='Get an honorable, killing blow on four different races.' WHERE  `ID`=1005;
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=2369;
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=2374;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='1' WHERE  `ID`=2370;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='2' WHERE  `ID`=2371;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='3' WHERE  `ID`=2372;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='4' WHERE  `ID`=2373;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='1' WHERE  `ID`=2375;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='2' WHERE  `ID`=2376;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='3' WHERE  `ID`=2377;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='4' WHERE  `ID`=2378;
-- Kill in every city - remove tbc cities
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=6639;
DELETE FROM `achievement_criteria_dbc` WHERE  `ID`=6634;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='3' WHERE  `ID`=6640;
UPDATE `achievement_criteria_dbc` SET `Ui_Order`='2' WHERE  `ID`=6635;
-- Kill Blood elf leader
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=613;
-- Kill Draenei leader
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=618;
-- Kill Varian
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=615;
-- New Achievement Kill Bolvar (classic)
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Title_Lang_Mask`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Category`, `Points`, `Ui_Order`, `IconID`, `Reward_Lang_Mask`) VALUES ('620', '0', '0', 'Do I smell fire?', '16712190', 'Kill Highlord Bolvar Fordragon.', '16712190', '95', '10', '26', '3671', '16712174');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('5204', '620', '0', '1748', '1', 'Highlord Bolvar Fordragon', '2', '1');
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`) VALUES ('5204', '0', '0');
-- Grizzled Veteran (wotlk)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=2016;
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=2017;
-- Kill Enemy Leaders - set to tbc+
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=619;
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=614;
-- New achievement Kill Enemy Leaders (classic)
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Title_Lang_Mask`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Category`, `Points`, `Ui_Order`, `IconID`, `Reward_Lang_Mask`) VALUES ('702', '0', '-1', 'For The Horde!', '16712190', 'Slay the leaders of the Alliance.', '16712190', '95', '20', '30', '1703', '16712190');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('574', '702', '8', '620', '0', 'Do I smell fire?', '0', '1');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('575', '702', '8', '616', '0', 'Death to the King!', '0', '2');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('576', '702', '8', '617', '0', 'Immortal No More', '0', '3');
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Title_Lang_Mask`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Category`, `Points`, `Ui_Order`, `IconID`, `Reward_Lang_Mask`) VALUES ('703', '1', '-1', 'For The Alliance!', '16712190', 'Slay the leaders of the Horde.', '16712190', '95', '20', '25', '1704', '16712190');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('577', '703', '8', '610', '0', 'Death to the Warchief!', '0', '1');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('578', '703', '8', '611', '0', 'Bleeding Bloodhoof', '0', '2');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('579', '703', '8', '612', '0', 'Downing the Dark Lady', '0', '3');

-- PVE
-- Naxxramas
-- Achievement Kill KT
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`, `Minimum_Criteria`) VALUES ('715', '-1', 'Naxxramas', 'Defeat Kel\'Thuzad', '14808', '10', '26', '4075', '1');
-- Move Leeroy Achievement after Naxx
UPDATE `achievement_dbc` SET `Ui_Order`='27' WHERE  `ID`=2188;
-- criteria
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('593', '715', '15990', '1', 'Kel\'Thuzad', '2', '1');
-- add Naxx to Classic Raider
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Description_Lang_enUS`, `Ui_Order`) VALUES ('4008', '1285', '8', '715', 'Naxxramas', '6');
-- tbc achievements
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID` IN (4476,4477,4478);

-- GENERAL
-- Riding skill flying
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=890;
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=892;
-- Higher Learning (wotlk)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=1956;
-- wotlk achievements
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID` IN (2076,2078,2081,2084,2097,2557,2716);

-- REPUTATION
-- Argent Champion (Wotlk faction)
UPDATE `achievement_dbc` SET `patch`='2' WHERE  `ID`=945;
-- The Diplomat (TBC factions)
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=942;
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=943;
-- Ambassador Achievement
-- ALLIANCE
-- old achievement set patch to tbc+
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=948;
-- new achievement (Agent of the Alliance)
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`) VALUES ('1001', '1', '-1', 'Agent of the Alliance', 'Earn exalted reputation with 4 home cities.', '201', '10', '11', '3375');
-- new criteria
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('5', '1001', '46', '72', '42000', 'Exalted Stormwind', '16712190', '1');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('6', '1001', '46', '47', '42000', 'Exalted Ironforge', '16712190', '2');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('7', '1001', '46', '69', '42000', 'Exalted Darnassus', '16712190', '3');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('8', '1001', '46', '54', '42000', 'Exalted Gnomeregan Exiles', '16712190', '4');
-- HORDE
-- old achievement set patch to tbc+
UPDATE `achievement_dbc` SET `patch`='1' WHERE  `ID`=762;
-- new achievement (Agent of the Horde)
INSERT INTO `achievement_dbc` (`ID`, `Instance_Id`, `Title_Lang_enUS`, `Description_Lang_enUS`, `Category`, `Points`, `Ui_Order`, `IconID`) VALUES ('1002', '-1', 'Agent of the Horde', 'Earn exalted reputation with 4 home cities.', '201', '10', '10', '3374');
-- new criteria
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('1', '1002', '46', '76', '42000', 'Exalted Orgrimmar', '16712190', '1');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('2', '1002', '46', '81', '42000', 'Exalted Thunder Bluff', '16712190', '2');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('3', '1002', '46', '68', '42000', 'Exalted Undercity', '16712190', '3');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Ui_Order`) VALUES ('4', '1002', '46', '530', '42000', 'Exalted Darkspear Trolls', '16712190', '4');

-- Custom
-- Ironman Challenge
INSERT INTO `achievement_dbc` (`ID`, `Faction`, `Instance_Id`, `Title_Lang_enUS`, `Title_Lang_Mask`, `Description_Lang_enUS`, `Description_Lang_Mask`, `Category`, `Points`, `Ui_Order`, `IconID`, `Reward_Lang_Mask`) VALUES ('704', '-1', '-1', 'I Can\'t Believe You Have Done That!', '16712190', 'Reach max level without dying once.', '16712190', '81', '0', '161', '3582', '16712190');
INSERT INTO `achievement_criteria_dbc` (`ID`, `Achievement_Id`, `Type`, `Asset_Id`, `Quantity`, `Description_Lang_enUS`, `Flags`, `Ui_Order`) VALUES ('580', '704', '5', '0', '100', 'Reach max level without dying once.', '0', '1');