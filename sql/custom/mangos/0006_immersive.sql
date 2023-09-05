-- backup
drop table if exists creature_template_backup;
create table creature_template_backup
SELECT * FROM creature_template where TrainerTemplateId <> 0 and TrainerClass <> 0 and GossipMenuId = 0;

-- cleanup
DELETE FROM `gossip_menu` where entry > 60000;
DELETE FROM `gossip_menu_option` where menu_id in (60000, 60001, 60002, 60003, 60004);
DELETE FROM `gossip_menu_option` where option_text = 'Manage attributes';
DELETE FROM `locales_gossip_menu_option` where menu_id in (60000, 60001, 60002, 60003, 60004);
DELETE FROM `locales_gossip_menu_option` where option_text_loc6 = 'Administrat atributos';

SET @TEXT_ID := 50800;
DELETE FROM `npc_text` WHERE `ID` in (@TEXT_ID, @TEXT_ID+1, @TEXT_ID+2);
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES
(@TEXT_ID, "Ah, $N, it appears our journey has brought us to this crucial point. Let's review your attributes to ensure you are prepared for the challenges ahead."),
(@TEXT_ID+1, "Are you sure you want to unlearn all your attributes?"),
(@TEXT_ID+2, "Interesting... Reducing your attributes will make your experience more challenging. Are you sure?");

DELETE FROM `locales_npc_text` WHERE `entry` in (@TEXT_ID, @TEXT_ID+1, @TEXT_ID+2);
INSERT INTO `locales_npc_text` (`entry`, `text0_0_loc6`) VALUES
(@TEXT_ID, "Ah, $N, parece que nuestro viaje nos ha llevado a este punto crucial. Revisemos tus atributos para asegurarnos de que estas preparados para los desafíos que se avecinan."),
(@TEXT_ID+1, "¿Estás seguro de que quieres reiniciar todos tus atributos?"),
(@TEXT_ID+2, "Interesante... Reducir tus atributos hará que tu experiencia sea más desafiante. ¿Estas seguro?");

-- root menu
INSERT INTO `gossip_menu` (`entry`, `text_id`, `script_id`, `condition_id`)
VALUES (60001, @TEXT_ID, 0, 0);

INSERT INTO `gossip_menu` (`entry`, `text_id`, `script_id`, `condition_id`)
VALUES (60002, @TEXT_ID+1, 0, 0);

INSERT INTO `gossip_menu` (`entry`, `text_id`, `script_id`, `condition_id`)
VALUES (60003, @TEXT_ID+2, 0, 0);

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 0, 7, 'Check Current Attributes', 18, 16, 0, 0, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 0, 'Ver Atributos Actuales', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 1, 3, 'Improve Strength', 18, 16, 0, 1, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 1, 'Mejorar Fuerza', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 2, 3, 'Improve Agility', 18, 16, 0, 2, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 2, 'Mejorar Agilidad', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 3, 3, 'Improve Stamina', 18, 16, 0, 3, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 3, 'Mejorar Aguante', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 4, 3, 'Improve Intellect', 18, 16, 0, 4, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 4, 'Mejorar Intelecto', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 5, 3, 'Improve Spirit', 18, 16, 0, 5, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 5, 'Mejorar Espiritu', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 7, 4, 'Unlearn all attributes', 1, 16, 60002, 0, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 7, 'Reiniciar todos los atributos', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60002, 6, 4, 'I am sure I do want to unlearn all attributes', 18, 16, 0, 6, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60002, 6, 'Estoy seguro que quiero reiniciar mis atributos', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60001, 8, 4, 'Temporary reduce attributes', 1, 16, 60003, 0, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60001, 8, 'Reducir los atributos temporalmente', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 11, 3, 'Remove Reduction', 18, 16, 0, 11, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 11, 'Quitar reducción de atributos', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 12, 3, 'Reduce by 90%', 18, 16, 0, 12, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 12, 'Reducir un 90%', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 14, 3, 'Reduce by 70%', 18, 16, 0, 14, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 14, 'Reducir un 70%', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 16, 3, 'Reduce by 50%', 18, 16, 0, 16, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 16, 'Reducir un 50%', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 17, 3, 'Reduce by 30%', 18, 16, 0, 18, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 17, 'Reducir un 30%', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 18, 3, 'Reduce by 20%', 18, 16, 0, 18, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 18, 'Reducir un 20%', '');

INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`,
`box_money`, `box_text`, `condition_id`) VALUES (60003, 19, 3, 'Reduce by 10%', 18, 16, 0, 20, 0, 0, 0, '', 0);

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
VALUES (60003, 19, 'Reducir un 10%', '');

-- add to trainers
INSERT INTO `gossip_menu_option`
(`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `action_script_id`, `box_coded`, `box_money`, `box_text`, `condition_id`) 
SELECT menu_id, 61, 3, 'Manage attributes', 1, 16, 60001, 0, 0, 0, 0, '', 0 FROM `gossip_menu_option` where `action_menu_id` = 4461;

INSERT INTO `locales_gossip_menu_option` (`menu_id`, `id`, `option_text_loc6`, `box_text_loc6`) 
SELECT menu_id, id, 'Administrat atributos', '' FROM `gossip_menu_option` WHERE option_text = 'Manage attributes';

-- add missing gossips to trainers
update creature_template set GossipMenuId = 4537 where entry in (select entry from creature_template_backup);
update gossip_menu_option set condition_id = 0 where action_menu_id = 4461;

-- chat messages
DELETE FROM `mangos_string` where entry in (12100, 12101, 12102, 12103, 12104, 12105, 12106, 12107, 12108, 12109, 12110, 12111, 12112, 12113, 12114, 12115, 12116, 12117, 12118, 12119, 12120, 12121);
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12118, '|cffffff00You have %u attribute points available.\n|cffffff00Speak with your class trainer to use them.', '|cffffff00Tienes %u puntos de atributos disponibles.\n|cffffff00Habla con tu entrenador de clase para usarlos.');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12119, '%s gained', '%s ganado');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12120, '%u reputation gained', '%u reputación ganada');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12121, '%s completed', '%s completado');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12117, '%u experience gained', '%u experiencia ganada');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12116, '|cffffff00Your attributes have been reset. You have |cff00ff00%u|cffffff00 points available', '|cffffff00Se han reiniciado tus atributos. Tienes |cff00ff00%u|cffffff00 puntos disponibles');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12115, '|cffffff00You have gained |cff00ff00+%u |cffffff00%s. |cff00ff00%u|cffffff00 points left. (|cffffff00%s|cffffff00 spent)', '|cffffff00Has ganado |cff00ff00+%u |cffffff00%s. |cff00ff00%u|cffffff00 puntos restantes. (|cffffff00%s|cffffff00 gastados)');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12114, '|cffffa0a0You don\'t have enough gold', '|cffffa0a0No tienes suficiente dinero');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12113, '|cffffa0a0You have no attribute points left', '|cffffa0a0No tienes mas puntos de atributos');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12112, '(disabled)', '(deshabilitado)');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12111, '|cffffff00You have changed your attribute modifier to |cff00ff00%s|cffffff00', '|cffffff00Has cambiado el modificador de atributos a |cff00ff00%s|cffffff00');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12110, '|cffffff00Suggested Attributes:\n%s', '|cffffff00Atributos Sugeridos:\n%s');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12109, '(|cff00ff00%s|cffffff00 modifier)', '(|cff00ff00%s|cffffff00 modificador)');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12108, '|cffffff00Current Attributes:\n%s', '|cffffff00Atributos Actuales:\n%s');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12107, '|cffffff00Points Available: |cff00ff00%u|cffffff00 (Cost: |cffffff00%s|cffffff00)', '|cffffff00Puntos disponibles: |cff00ff00%u|cffffff00 (Coste: |cffffff00%s|cffffff00)');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12106, 'Spirit', 'Espiritu');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12105, 'Intellect', 'Intelecto');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12104, 'Stamina', 'Aguante');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12103, 'Agility', 'Agilidad');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12102, 'Strength', 'Fuerza');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12101, '|cffffff00You have lost these attributes: %s', '|cffffff00Has perdido estos atributos: %s');
INSERT INTO `mangos_string` (`entry`, `content_default`, `content_loc6`) VALUES (12100, '|cffffff00Manual attributes are disabled.', '|cffffff00Los atributos manuales están deshabilitados.');
