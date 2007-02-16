
CREATE TABLE config (
  gameid int NOT NULL default 0,
  PRIMARY KEY  (gameid)
);

INSERT INTO config VALUES (0);

CREATE TABLE users (
  userid serial,
  username varchar(32) NOT NULL default '',
  passwd varchar(32) NOT NULL default '',
  games_played int NOT NULL default '0',
  last_game int NOT NULL default '0',
  bankroll int NOT NULL default '0',
  wins int NOT NULL default '0',
  ties int NOT NULL default '0',
  loses int NOT NULL default '0',
  folds int NOT NULL default '0',
  sd int NOT NULL default '0',
  PRIMARY KEY  (userid)
);

INSERT INTO users VALUES (1,'player1','5ebe2294ecd0e0f08eab7690d2a6ee69',0,0,0,0,0,0,0,0);
INSERT INTO users VALUES (2,'player2','5ebe2294ecd0e0f08eab7690d2a6ee69',0,0,0,0,0,0,0,0);
