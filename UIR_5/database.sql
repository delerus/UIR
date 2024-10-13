-- Удалить старую базу и аккаунт, если они существуют
DROP DATABASE IF EXISTS uir5;
DROP USER IF EXISTS uir5User;

-- Создать новую базу и пользователя
CREATE DATABASE uir5;
CREATE USER uir5User IDENTIFIED BY 'uir5Password';
GRANT ALL PRIVILEGES ON uir5.* TO uir5User;

-- Использовать созданную базу
USE uir5;

-- Создать таблицу без партицирования
CREATE TABLE IF NOT EXISTS `articles_no_part` (
    `id` INTEGER NOT NULL AUTO_INCREMENT,
    `title` VARCHAR(256) NOT NULL,
    `price` DECIMAL(10, 2) NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE = InnoDB;

-- Создать таблицу с партицированием
CREATE TABLE IF NOT EXISTS `articles_part` (
    `id` INTEGER NOT NULL AUTO_INCREMENT,
    `title` VARCHAR(256) NOT NULL,
    `price` INTEGER NOT NULL,
    PRIMARY KEY (`id`, `price`)
) ENGINE = InnoDB
  PARTITION BY RANGE (`price`) (
     PARTITION p0 VALUES LESS THAN (100),
     PARTITION p1 VALUES LESS THAN (200),
     PARTITION p2 VALUES LESS THAN (300),
     PARTITION p3 VALUES LESS THAN (400),
     PARTITION p4 VALUES LESS THAN MAXVALUE
  );



-- Создать процедуру для генерации данных
DELIMITER //
CREATE PROCEDURE `generate_data` (IN items_count INT)
BEGIN
    DECLARE i INT DEFAULT 0;
    WHILE i < items_count DO
        INSERT INTO `articles_no_part` (`title`, `price`) VALUES (SUBSTRING(MD5(RAND()) FROM 1 FOR 8), RAND() * 1000);
        SET i = i + 1;
    END WHILE;
    -- Копировать те же данные в таблицу с партицированием
    INSERT INTO `articles_part`
        SELECT * FROM `articles_no_part`;
END//
DELIMITER ;

-- Заполнить таблицу данными
CALL generate_data(2000);

-- Удалить процедуру
DROP PROCEDURE `generate_data`;
