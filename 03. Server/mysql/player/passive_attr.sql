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

 Date: 12/06/2026 00:12:32
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for passive_attr
-- ----------------------------
DROP TABLE IF EXISTS `passive_attr`;
CREATE TABLE `passive_attr`  (
  `apply` enum('HIT_BUFF_SUNGMA_STR','HIT_BUFF_SUNGMA_HP','HIT_BUFF_SUNGMA_MOVE','HIT_BUFF_SUNGMA_IMMUNE','HIT_AUTO_HP_RECOVERY','HIT_AUTO_SP_RECOVERY','HIT_STONE_DEF_GRADE_BONUS','HIT_STONE_ATTBONUS_STONE','KILL_BOSS_ITEM_BONUS','MOUNT_MELEE_MAGIC_ATTBONUS_PER','DISMOUNT_MOVE_SPEED_BONUS_PER','MOUNT_NO_KNOCKBACK','USE_SKILL_COOLTIME_DECREASE_ALL','AUTO_PICKUP','NO_DEATH_AND_HP_RECOVERY30','MOB_HIT_MOB_AGGRESSIVE') CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `prob` int NOT NULL DEFAULT 100,
  `cooldown_sec` int NOT NULL DEFAULT 120,
  `lv1` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv2` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv3` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv4` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv5` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv6` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv7` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv8` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv9` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  `lv10` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL DEFAULT '',
  PRIMARY KEY (`apply`) USING BTREE
) ENGINE = MyISAM AUTO_INCREMENT = 1 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_unicode_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of passive_attr
-- ----------------------------
INSERT INTO `passive_attr` VALUES ('HIT_BUFF_SUNGMA_STR', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '10', '');
INSERT INTO `passive_attr` VALUES ('HIT_BUFF_SUNGMA_HP', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '10', '');
INSERT INTO `passive_attr` VALUES ('HIT_BUFF_SUNGMA_MOVE', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '10', '');
INSERT INTO `passive_attr` VALUES ('HIT_BUFF_SUNGMA_IMMUNE', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '10', '');
INSERT INTO `passive_attr` VALUES ('HIT_AUTO_HP_RECOVERY', 100, 300, '1', '1', '1', '1', '1', '1', '1', '1', '1', '');
INSERT INTO `passive_attr` VALUES ('HIT_AUTO_SP_RECOVERY', 100, 300, '1', '1', '1', '1', '1', '1', '1', '1', '1', '');
INSERT INTO `passive_attr` VALUES ('HIT_STONE_DEF_GRADE_BONUS', 100, 120, '1', '2', '3', '4', '5', '6', '8', '10', '', '');
INSERT INTO `passive_attr` VALUES ('HIT_STONE_ATTBONUS_STONE', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '10', '');
INSERT INTO `passive_attr` VALUES ('KILL_BOSS_ITEM_BONUS', 100, 0, '1', '2', '3', '4', '5', '6', '7', '8', '10', '');
INSERT INTO `passive_attr` VALUES ('MOUNT_MELEE_MAGIC_ATTBONUS_PER', 100, 120, '1', '2', '3', '4', '5', '', '', '', '', '');
INSERT INTO `passive_attr` VALUES ('DISMOUNT_MOVE_SPEED_BONUS_PER', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '9', '10');
INSERT INTO `passive_attr` VALUES ('MOUNT_NO_KNOCKBACK', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '9', '10');
INSERT INTO `passive_attr` VALUES ('USE_SKILL_COOLTIME_DECREASE_ALL', 100, 120, '1', '2', '3', '4', '5', '6', '7', '9', '15', '');
INSERT INTO `passive_attr` VALUES ('AUTO_PICKUP', 100, 300, '1', '2', '3', '4', '5', '6', '7', '9', '15', '');
INSERT INTO `passive_attr` VALUES ('NO_DEATH_AND_HP_RECOVERY30', 100, 600, '5', '7', '11', '15', '20', '30', '50', '', '', '');
INSERT INTO `passive_attr` VALUES ('MOB_HIT_MOB_AGGRESSIVE', 100, 120, '1', '2', '3', '4', '5', '6', '7', '8', '9', '10');

SET FOREIGN_KEY_CHECKS = 1;
