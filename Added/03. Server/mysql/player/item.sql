/*
 Navicat Premium Dump SQL

 Source Server         : OfficialFiles
 Source Server Type    : MySQL
 Source Server Version : 80041 (8.0.41)
 Source Host           : 192.168.1.163:3306
 Source Schema         : player

 Target Server Type    : MySQL
 Target Server Version : 80041 (8.0.41)
 File Encoding         : 65001

 Date: 12/06/2026 00:14:30
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for item
-- ----------------------------
DROP TABLE IF EXISTS `item`;
CREATE TABLE `item`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `owner_id` int UNSIGNED NOT NULL DEFAULT 0,
  `window` enum('INVENTORY','EQUIPMENT','WEAR_EQUIPMENT_2','WEAR_COSTUME','WEAR_DRAGONSOUL','WEAR_PASSIVE_ATTR','WEAR_UNIQUE','SAFEBOX','MALL','DRAGON_SOUL_INVENTORY','BELT_INVENTORY','BELT_INVENTORY_2','GUILDBANK','MAIL','NPC_STORAGE','PREMIUM_PRIVATE_SHOP','ACCEREFINE','GROUND','PET_FEED','CHANGELOOK','AURA_REFINE','CUBE_WINDOW') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NULL DEFAULT 'INVENTORY',
  `pos` smallint UNSIGNED NOT NULL DEFAULT 0,
  `vnum` int UNSIGNED NOT NULL DEFAULT 0,
  `count` smallint UNSIGNED NOT NULL DEFAULT 0,
  `soulbind` bigint NOT NULL DEFAULT 0,
  `socket0` int UNSIGNED NOT NULL DEFAULT 0,
  `socket1` int UNSIGNED NOT NULL DEFAULT 0,
  `socket2` int UNSIGNED NOT NULL DEFAULT 0,
  `socket3` int UNSIGNED NOT NULL DEFAULT 0,
  `socket4` int UNSIGNED NOT NULL DEFAULT 0,
  `socket5` int UNSIGNED NOT NULL DEFAULT 0,
  `attrtype0` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue0` smallint NOT NULL DEFAULT 0,
  `attrtype1` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue1` smallint NOT NULL DEFAULT 0,
  `attrtype2` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue2` smallint NOT NULL DEFAULT 0,
  `attrtype3` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue3` smallint NOT NULL DEFAULT 0,
  `attrtype4` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue4` smallint NOT NULL DEFAULT 0,
  `attrtype5` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue5` smallint NOT NULL DEFAULT 0,
  `attrtype6` smallint UNSIGNED NOT NULL DEFAULT 0,
  `attrvalue6` smallint NOT NULL DEFAULT 0,
  `transmutation` int UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_apply_type` smallint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_grade` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_value0` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_value1` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_value2` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_bonus_value0` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_bonus_value1` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `refine_element_bonus_value2` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `apply_type0` smallint UNSIGNED NOT NULL DEFAULT 0,
  `apply_value0` smallint NOT NULL DEFAULT 0,
  `apply_path0` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `apply_type1` smallint UNSIGNED NOT NULL DEFAULT 0,
  `apply_value1` smallint NOT NULL DEFAULT 0,
  `apply_path1` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `apply_type2` smallint UNSIGNED NOT NULL DEFAULT 0,
  `apply_value2` smallint NOT NULL DEFAULT 0,
  `apply_path2` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `apply_type3` smallint UNSIGNED NOT NULL DEFAULT 0,
  `apply_value3` smallint NOT NULL DEFAULT 0,
  `apply_path3` tinyint UNSIGNED NOT NULL DEFAULT 0,
  `set_value` tinyint UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `owner_id_idx`(`owner_id`) USING BTREE,
  INDEX `item_vnum_index`(`vnum`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 2000000000 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = FIXED;

-- ----------------------------
-- Records of item
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
