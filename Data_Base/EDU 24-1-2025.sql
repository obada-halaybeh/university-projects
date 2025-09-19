-- MySQL dump 10.13  Distrib 8.0.33, for Win64 (x86_64)
--
-- Host: localhost    Database: edu
-- ------------------------------------------------------
-- Server version	8.0.33

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `donations`
--

DROP TABLE IF EXISTS `donations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `donations` (
  `donation_id` int NOT NULL AUTO_INCREMENT,
  `amount` decimal(10,2) NOT NULL,
  `donation_date` date NOT NULL,
  `donor_id` int DEFAULT NULL,
  `event_id` int DEFAULT NULL,
  PRIMARY KEY (`donation_id`),
  KEY `donor_id` (`donor_id`),
  KEY `event_id` (`event_id`),
  CONSTRAINT `donations_ibfk_1` FOREIGN KEY (`donor_id`) REFERENCES `donor` (`donor_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `donations_ibfk_2` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `donations`
--

LOCK TABLES `donations` WRITE;
/*!40000 ALTER TABLE `donations` DISABLE KEYS */;
INSERT INTO `donations` VALUES (1,1200.00,'2025-01-01',1,1),(2,500.00,'2025-01-02',1,1),(3,1000.00,'2025-01-02',7,1),(4,750.00,'2025-02-16',2,2),(5,250.00,'2025-02-16',4,2),(6,5000.00,'2025-02-17',10,2),(7,500.00,'2025-06-20',2,3),(8,250.00,'2025-06-20',3,3),(9,300.00,'2025-06-20',3,3),(10,1000.00,'2025-04-15',4,4),(11,2500.00,'2025-04-16',9,4),(12,350.00,'2025-04-16',6,4),(13,1100.00,'2025-11-01',5,5),(14,400.00,'2025-11-02',5,5),(15,1000.00,'2025-11-03',8,5),(16,500.00,'2025-05-02',6,6),(17,700.00,'2025-05-04',8,6),(18,200.00,'2025-05-09',10,6),(19,500.00,'2025-05-15',1,6),(20,1000.00,'2025-05-20',7,6),(21,200.00,'2025-06-01',9,6),(22,400.00,'2025-08-01',2,7),(23,1250.00,'2025-08-04',5,7),(24,100.00,'2025-08-05',3,7),(25,200.00,'2025-08-09',8,7);
/*!40000 ALTER TABLE `donations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `donor`
--

DROP TABLE IF EXISTS `donor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `donor` (
  `donor_id` int NOT NULL AUTO_INCREMENT,
  `first_name` varchar(50) DEFAULT NULL,
  `last_name` varchar(50) DEFAULT NULL,
  `type` varchar(50) NOT NULL,
  `email` varchar(100) NOT NULL,
  `staff_id` int DEFAULT NULL,
  PRIMARY KEY (`donor_id`),
  UNIQUE KEY `email` (`email`),
  KEY `staff_id` (`staff_id`),
  CONSTRAINT `donor_ibfk_1` FOREIGN KEY (`staff_id`) REFERENCES `staff` (`staff_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `donor`
--

LOCK TABLES `donor` WRITE;
/*!40000 ALTER TABLE `donor` DISABLE KEYS */;
INSERT INTO `donor` VALUES (1,'Michael','Brown','Individual','Michael.Brown@gmail.com',1),(2,'Eva','Davis','Individual','Eva.Davis@gmail.com',1),(3,'David','Wilson','Individual','David.Wilson@gmail.com',1),(4,'Sophia','Martinez','Individual','Sophia.Martinez@gmail.com',4),(5,'James','Anderson','Individual','James.Anderson@gmail.com',4),(6,'Olivia','Thomas','Individual','Olivia.Thomas@gmail.com',5),(7,NULL,NULL,'organization','steev.organization@gmail.com',2),(8,NULL,NULL,'organization','brox.organization@gmail.com',2),(9,NULL,NULL,'organization','fam.organization@gmail.com',3),(10,NULL,NULL,'organization','dov.organization@gmail.com',3);
/*!40000 ALTER TABLE `donor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `donor_address`
--

DROP TABLE IF EXISTS `donor_address`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `donor_address` (
  `donor_id` int NOT NULL,
  `state` varchar(50) NOT NULL,
  `city` varchar(50) NOT NULL,
  `street` varchar(50) NOT NULL,
  PRIMARY KEY (`donor_id`,`state`,`city`,`street`),
  CONSTRAINT `donor_address_ibfk_1` FOREIGN KEY (`donor_id`) REFERENCES `donor` (`donor_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `donor_address`
--

LOCK TABLES `donor_address` WRITE;
/*!40000 ALTER TABLE `donor_address` DISABLE KEYS */;
INSERT INTO `donor_address` VALUES (1,'Florida','Pompano Beach','1575 Foley Street'),(1,'New York','Delmar','1037 Hardesty Street'),(2,'Kentucky','Lexington','120 North Bend River Road'),(3,'Maryland','Columbia','4062 Hewes Avenue'),(4,'North Carolina','Raleigh','4122 Johnson Street'),(4,'Virginia','Herndon','746 Sunrise Road'),(5,'Indiana','Evansville','1980 Stratford Park'),(5,'Oregon','Portland','3288 Mattson Street'),(6,'Michigan','Grand Rapids','171 Echo Lane'),(7,'California','Fremont','1617 Ella Street'),(8,'New Jersey','New Brunswick','1889 Williams Mine Road'),(9,'New York','Albany','2611 Golden Ridge Road'),(10,'Pennsylvania','Philadelphia','2577 Berkley Street');
/*!40000 ALTER TABLE `donor_address` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `donor_contact_information`
--

DROP TABLE IF EXISTS `donor_contact_information`;
/*!50001 DROP VIEW IF EXISTS `donor_contact_information`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `donor_contact_information` AS SELECT 
 1 AS `full_name`,
 1 AS `email`,
 1 AS `state`,
 1 AS `city`,
 1 AS `street`,
 1 AS `phone`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `donor_contribution`
--

DROP TABLE IF EXISTS `donor_contribution`;
/*!50001 DROP VIEW IF EXISTS `donor_contribution`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `donor_contribution` AS SELECT 
 1 AS `donor_id`,
 1 AS `full_name`,
 1 AS `type`,
 1 AS `email`,
 1 AS `staff_id`,
 1 AS `total_donations`,
 1 AS `donation_count`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `donor_event`
--

DROP TABLE IF EXISTS `donor_event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `donor_event` (
  `donor_id` int NOT NULL,
  `event_id` int NOT NULL,
  PRIMARY KEY (`donor_id`,`event_id`),
  KEY `event_id` (`event_id`),
  CONSTRAINT `donor_event_ibfk_1` FOREIGN KEY (`donor_id`) REFERENCES `donor` (`donor_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `donor_event_ibfk_2` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `donor_event`
--

LOCK TABLES `donor_event` WRITE;
/*!40000 ALTER TABLE `donor_event` DISABLE KEYS */;
INSERT INTO `donor_event` VALUES (1,1),(3,1),(7,1),(8,1),(9,1),(10,1),(2,2),(4,2),(7,2),(8,2),(9,2),(10,2),(2,3),(3,3),(5,3),(7,3),(8,3),(9,3),(10,3),(1,4),(4,4),(5,4),(6,4),(7,4),(8,4),(9,4),(10,4),(4,5),(7,5),(8,5),(9,5),(10,5),(1,6),(2,6),(3,6),(4,6),(6,6),(7,6),(8,6),(9,6),(10,6),(1,7),(2,7),(3,7),(4,7),(5,7),(7,7),(8,7),(9,7),(10,7);
/*!40000 ALTER TABLE `donor_event` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `donor_phone`
--

DROP TABLE IF EXISTS `donor_phone`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `donor_phone` (
  `donor_id` int NOT NULL,
  `phone` varchar(15) NOT NULL,
  PRIMARY KEY (`donor_id`,`phone`),
  CONSTRAINT `donor_phone_ibfk_1` FOREIGN KEY (`donor_id`) REFERENCES `donor` (`donor_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `donor_phone`
--

LOCK TABLES `donor_phone` WRITE;
/*!40000 ALTER TABLE `donor_phone` DISABLE KEYS */;
INSERT INTO `donor_phone` VALUES (1,'+1 561-929-1270'),(1,'+1 845-200-9047'),(2,'+1 502-488-2827'),(3,'+1 919-809-5336'),(4,'+1 503-515-6733'),(4,'+1 703-282-3635'),(5,'+1 616-478-1457'),(5,'+1 812-305-1237'),(6,'+1 267-752-9808'),(7,'+1 817-320-3013'),(8,'+1 508-973-3274'),(9,'+1 502-802-8790'),(10,'+1 813-765-7392');
/*!40000 ALTER TABLE `donor_phone` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `event_donations`
--

DROP TABLE IF EXISTS `event_donations`;
/*!50001 DROP VIEW IF EXISTS `event_donations`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `event_donations` AS SELECT 
 1 AS `event_name`,
 1 AS `start_date`,
 1 AS `end_date`,
 1 AS `location`,
 1 AS `total_donations`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `event_location`
--

DROP TABLE IF EXISTS `event_location`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `event_location` (
  `event_id` int NOT NULL,
  `location` varchar(100) NOT NULL,
  PRIMARY KEY (`event_id`,`location`),
  CONSTRAINT `event_location_ibfk_1` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `event_location`
--

LOCK TABLES `event_location` WRITE;
/*!40000 ALTER TABLE `event_location` DISABLE KEYS */;
INSERT INTO `event_location` VALUES (1,'Central Park, NY'),(2,'Broadway Theater'),(3,'Miami Beach, FL'),(4,'Chicago Botanic Garden'),(5,'Denver City Park'),(6,'Blue Ridge Parkway'),(6,'Grand Canyon'),(6,'Route 66'),(6,'Washington Monument'),(6,'Yellowstone Park'),(7,'Los Angeles, CA'),(7,'San Diego, CA'),(7,'San Francisco, CA');
/*!40000 ALTER TABLE `event_location` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `events`
--

DROP TABLE IF EXISTS `events`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `events` (
  `event_id` int NOT NULL AUTO_INCREMENT,
  `event_name` varchar(50) NOT NULL,
  `start_date` date NOT NULL,
  `end_date` date NOT NULL,
  PRIMARY KEY (`event_id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `events`
--

LOCK TABLES `events` WRITE;
/*!40000 ALTER TABLE `events` DISABLE KEYS */;
INSERT INTO `events` VALUES (1,'Winter Festival','2025-01-01','2025-01-02'),(2,'School of Hop','2025-02-15','2025-02-17'),(3,'Summer Festival','2025-06-20','2025-06-22'),(4,'Spring Festival','2025-04-15','2025-04-17'),(5,'Fall Festival','2025-11-01','2025-11-03'),(6,'Country Run','2025-05-01','2025-06-01'),(7,'Down Town','2025-08-01','2025-08-10');
/*!40000 ALTER TABLE `events` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `staff`
--

DROP TABLE IF EXISTS `staff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `staff` (
  `staff_id` int NOT NULL AUTO_INCREMENT,
  `first_name` varchar(50) NOT NULL,
  `last_name` varchar(50) NOT NULL,
  `role` varchar(50) NOT NULL DEFAULT 'employee',
  `email` varchar(100) NOT NULL,
  `annual_salary` decimal(10,2) NOT NULL,
  PRIMARY KEY (`staff_id`),
  UNIQUE KEY `email` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `staff`
--

LOCK TABLES `staff` WRITE;
/*!40000 ALTER TABLE `staff` DISABLE KEYS */;
INSERT INTO `staff` VALUES (1,'Issac','Hubbard','employee','issac.hubbard@gmail.com',50000.00),(2,'luna','snow','manager','luna.snow@gmail.com',75000.00),(3,'Kayla','Frost','manager','Kayla.Frost@gmail.com',80000.00),(4,'Frank','Miller','employee','Frank.Miller@gmail.com',45000.00),(5,'Grace','Wilson','employee','Grace.Wilson@gmail.com',30000.00);
/*!40000 ALTER TABLE `staff` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary view structure for view `staff_contribution_report`
--

DROP TABLE IF EXISTS `staff_contribution_report`;
/*!50001 DROP VIEW IF EXISTS `staff_contribution_report`*/;
SET @saved_cs_client     = @@character_set_client;
/*!50503 SET character_set_client = utf8mb4 */;
/*!50001 CREATE VIEW `staff_contribution_report` AS SELECT 
 1 AS `staff_id`,
 1 AS `full_name`,
 1 AS `role`,
 1 AS `email`,
 1 AS `donor_count`,
 1 AS `total_donations`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `staff_event`
--

DROP TABLE IF EXISTS `staff_event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `staff_event` (
  `staff_id` int NOT NULL,
  `event_id` int NOT NULL,
  PRIMARY KEY (`staff_id`,`event_id`),
  KEY `event_id` (`event_id`),
  CONSTRAINT `staff_event_ibfk_1` FOREIGN KEY (`staff_id`) REFERENCES `staff` (`staff_id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `staff_event_ibfk_2` FOREIGN KEY (`event_id`) REFERENCES `events` (`event_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `staff_event`
--

LOCK TABLES `staff_event` WRITE;
/*!40000 ALTER TABLE `staff_event` DISABLE KEYS */;
INSERT INTO `staff_event` VALUES (1,1),(2,1),(3,2),(4,2),(2,3),(5,3),(1,4),(3,4),(1,5),(3,5),(2,6),(5,6),(3,7),(4,7);
/*!40000 ALTER TABLE `staff_event` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `staff_phone`
--

DROP TABLE IF EXISTS `staff_phone`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `staff_phone` (
  `staff_id` int NOT NULL,
  `phone` varchar(15) NOT NULL,
  PRIMARY KEY (`staff_id`,`phone`),
  CONSTRAINT `staff_phone_ibfk_1` FOREIGN KEY (`staff_id`) REFERENCES `staff` (`staff_id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `staff_phone`
--

LOCK TABLES `staff_phone` WRITE;
/*!40000 ALTER TABLE `staff_phone` DISABLE KEYS */;
INSERT INTO `staff_phone` VALUES (1,'+1 305-241-8749'),(1,'+1 719-200-4137'),(2,'+1 503-341-4061'),(3,'+1 906-458-5993'),(4,'+1 361-462-3441'),(4,'+1 660-247-0609'),(5,'+1 252-282-0855');
/*!40000 ALTER TABLE `staff_phone` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'edu'
--
/*!50003 DROP PROCEDURE IF EXISTS `AddDonation` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AddDonation`(IN donation_amount DECIMAL(10,2), IN donation_date DATE, IN donor_id INT, IN event_id INT)
BEGIN
    INSERT INTO donations (amount, donation_date, donor_id, event_id)
    VALUES (donation_amount, donation_date, donor_id, event_id);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AddDonor` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AddDonor`(IN f_name VARCHAR(50), IN l_name VARCHAR(50), IN donor_type VARCHAR(50), IN donor_email VARCHAR(100), IN s_id INT)
BEGIN
    INSERT INTO donor (first_name, last_name, type, email, staff_id)
    VALUES (f_name, l_name, donor_type, donor_email, s_id);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `AssignStaffToEvent` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `AssignStaffToEvent`(IN staff_id INT, IN event_id INT)
BEGIN
    INSERT INTO staff_event (staff_id, event_id)
    VALUES (staff_id, event_id);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UpdateDonorEmail` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `UpdateDonorEmail`(IN donor_id INT, IN new_email VARCHAR(100))
BEGIN
    UPDATE donor SET email = new_email WHERE donor_id = donor_id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `donor_contact_information`
--

/*!50001 DROP VIEW IF EXISTS `donor_contact_information`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `donor_contact_information` AS select concat(`d`.`first_name`,' ',`d`.`last_name`) AS `full_name`,`d`.`email` AS `email`,`da`.`state` AS `state`,`da`.`city` AS `city`,`da`.`street` AS `street`,`dp`.`phone` AS `phone` from ((`donor` `d` left join `donor_address` `da` on((`d`.`donor_id` = `da`.`donor_id`))) left join `donor_phone` `dp` on((`d`.`donor_id` = `dp`.`donor_id`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `donor_contribution`
--

/*!50001 DROP VIEW IF EXISTS `donor_contribution`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `donor_contribution` AS select `d`.`donor_id` AS `donor_id`,concat(`d`.`first_name`,' ',`d`.`last_name`) AS `full_name`,`d`.`type` AS `type`,`d`.`email` AS `email`,`d`.`staff_id` AS `staff_id`,sum(`da`.`amount`) AS `total_donations`,count(`da`.`donation_id`) AS `donation_count` from (`donor` `d` left join `donations` `da` on((`d`.`donor_id` = `da`.`donor_id`))) group by `d`.`donor_id` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `event_donations`
--

/*!50001 DROP VIEW IF EXISTS `event_donations`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `event_donations` AS select `e`.`event_name` AS `event_name`,`e`.`start_date` AS `start_date`,`e`.`end_date` AS `end_date`,`el`.`location` AS `location`,sum(`da`.`amount`) AS `total_donations` from ((`events` `e` left join `event_location` `el` on((`e`.`event_id` = `el`.`event_id`))) left join `donations` `da` on((`e`.`event_id` = `da`.`event_id`))) group by `e`.`event_id`,`e`.`event_name`,`e`.`start_date`,`e`.`end_date`,`el`.`location` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `staff_contribution_report`
--

/*!50001 DROP VIEW IF EXISTS `staff_contribution_report`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `staff_contribution_report` AS select `s`.`staff_id` AS `staff_id`,concat(`s`.`first_name`,' ',`s`.`last_name`) AS `full_name`,`s`.`role` AS `role`,`s`.`email` AS `email`,count(distinct `d`.`donor_id`) AS `donor_count`,sum(`da`.`amount`) AS `total_donations` from ((`staff` `s` left join `donor` `d` on((`s`.`staff_id` = `d`.`staff_id`))) left join `donations` `da` on((`d`.`donor_id` = `da`.`donor_id`))) group by `s`.`staff_id`,`s`.`first_name`,`s`.`last_name`,`s`.`role`,`s`.`email` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-01-24 14:14:28
