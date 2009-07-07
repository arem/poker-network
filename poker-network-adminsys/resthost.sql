DROP TABLE IF EXISTS `resthost`;
CREATE TABLE `resthost` (
  `serial` int(10) unsigned NOT NULL auto_increment,
  `name` varchar(255) default NULL,
  `host` varchar(255) default NULL,
  `port` int(10) unsigned default NULL,
  `path` varchar(255) default NULL,
  PRIMARY KEY  (`serial`)
) ENGINE=MEMORY AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

INSERT INTO `resthost` VALUES (1,NULL,'poker1',19380,'/POKER_REST'),(3,NULL,'poker1',20380,'/POKER_REST'),(5,NULL,'poker2',19380,'/POKER_REST'),(7,NULL,'poker2',20380,'/POKER_REST');
