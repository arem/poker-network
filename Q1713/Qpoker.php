<?php
// copyright 2007 Q

// Please give me some feedback. 
// Q1712 [at] online [dot] ms

/*
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


/*
because this file may be viewed by anybody keep your MySQL login in a seperate
file called QpokerMySQL.php containing this:
<?php
  $MySQLuser="user";
  $MySQLpass="pass";
  $MySQLhost="localhost";
  $MySQLdb="databasename";
  $MySQLtableprefix="qpoker_";
?>
*/

//As long as you don't remove the following lines you will satisfy the
//requirements of the GPL.
    if($_REQUEST['source']==="show")
      {
	echo '<html><head><title>Qpoker source</title></head>';
	echo '<body style="font-family:monospace;font-size:12px;white-space:pre">';
	$file=file('Qpoker.php');
	foreach($file as $num => $line)
	    {
	      echo '<b>'.str_pad($num+1,4,' ',STR_PAD_LEFT).':</b> ';
	      echo htmlentities(strtr($line,array("\t"=>'        ')));
	    }
	die('</body></html>');
      }
    if($_REQUEST['source']==="download")
      {
	$file=implode('',file('Qpoker.php'));
	header('ContentType: application/x-php');
	header('Content-disposition: attachment; Qpoker.php');
	die($file);  
      }
//As long as you don't remove the priviose lines you will satisfy the
//requirements of the GPL.



if(get_magic_quotes_gpc()) foreach($_REQUEST as &$r) if(is_string($r)) $r=stripslashes($r);
unset($r);


include('QpokerMySQL.php');
if(
  isset($_REQUEST['install'])
  or
  query('SELECT `id` FROM `'.$MySQLprefix.'chat` WHERE 0')===false
  or
  query('SELECT `id` FROM `'.$MySQLprefix.'player` WHERE 0')===false
  or
  query('SELECT `id` FROM `'.$MySQLprefix.'table` WHERE 0')===false
  ) install();


// the text/plain pages requested by the AJAX function
if(isset($_REQUEST['changed']))
{
  header('ContentType: text/plain');
  @mysql_connect($MySQLhost,
		 $MySQLuser,
		 $MySQLpass);
  @mysql_selectdb($MySQLdb);
  switch($_REQUEST['changed'])
    {
    case 'time':
      if(intval($_REQUEST['time'])<time())die('reload');
      die(strval(intval($_REQUEST['time'])));
    case 'chat':
      if($t=@mysql_fetch_assoc(
	   @mysql_query(
	     'SELECT `table` FROM `'.$MySQLprefix.'player` '
	     .'WHERE `id`='.intval($_REQUEST['playerid']).' AND `table`!=0')))
	{
	  if($_REQUEST['chat']=='all') die("reload");
	  $t=@mysql_fetch_assoc(
	    @mysql_query(
	      'SELECT `'.$MySQLprefix.'chat`.`id` '
	      .'FROM `'.$MySQLprefix.'chat`,`'.$MySQLprefix.'player` '
	      .'WHERE `'.$MySQLprefix.'chat`.`player`=`'.$MySQLprefix.'player`.`id` '
	      .'AND `'.$MySQLprefix.'player`.`table`='.intval($t['table']).' '
	      .'ORDER BY `'.$MySQLprefix.'chat`.`id` DESC LIMIT 1'));
	}
      else
	{
	  if($_REQUEST['chat']=='table') die("reload");
	  $t=@mysql_fetch_assoc(
	    @mysql_query(
	      'SELECT `id` FROM `'.$MySQLprefix.'chat` '
	      .'ORDER BY `id` DESC LIMIT 1'));
	}
      die(strval(intval($t['id'])));
    case 'cards':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT MAX(`actiontime`) AS maxat '
	  .'FROM `'.$MySQLprefix.'table`,`'.$MySQLprefix.'player` '
	  .'WHERE `'.$MySQLprefix.'player`.`table`='.intval($_REQUEST['tableid'])));
      die(strval(intval($t['maxat'])));
    case 'tables':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT MAX(`timein`) AS maxti,  MAX(`timeout`) AS maxto '
	  .'FROM `'.$MySQLprefix.'player`'));
      die(strval(max($t['maxto'],$t['maxti'])));
    case 'players':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT COUNT(`id`) AS cid FROM `'.$MySQLprefix.'player` '
	  .'WHERE `table`='.intval($_REQUEST['tableid'])));
      die(strval(intval($t['cid'])));
    case 'actions':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT `onturn` FROM `'.$MySQLprefix.'table` '
	  .'WHERE `id`='.intval($_REQUEST['tableid'])));
      die(strval(intval($t['onturn'])));
    }
}

if(isset($_REQUEST['sid'])) session_id($_REQUEST['sid']);

session_start();

//header('Content-Type: application/xhtml+xml; charset=utf-8');


$url='Qpoker.php?';
if(isset($_REQUEST['sid']))$url.='sid='.session_id().'&';

ini_set('error_reporting',E_ALL);


$nums=array(1=>' A',' 2',' 3',' 4',' 5',' 6',' 7',' 8',' 9','10',' J',' Q',' K',' A');
$nums2=array(1=>'A','2','3','4','5','6','7','8','9','1','J','Q','K','A');
$nums3=array_flip($nums2);
$nums4=array(1=>'a','2','3','4','5','6','7','8','9','10','j','q','k','a');
$cols=array('&#x2660;','&#x2663;','&#x2665;','&#x2666;');
$cols2=array('s','c','h','d');

if(isset($_REQUEST['logout']))
{
  unset($_SESSION);
}
if(isset($_REQUEST['login']) and $_REQUEST['login'])
$_SESSION['loggedin']=
l(query('SELECT `id` FROM `'.$MySQLprefix.'player`',
	'WHERE `name`="'.mes($_REQUEST['name']).'"',
	'AND `pass`="'.md5($_REQUEST['pass']).'"'),
  '<,id');

if(isset($_SESSION['loggedin'])) 
$player=
l(query('SELECT * FROM `'.$MySQLprefix.'player`',
	'WHERE `id`='.intval($_SESSION['loggedin'])));
else $player=NULL;

if($player) 
{
  if(isset($_REQUEST['zoom']) and intval($_REQUEST['zoom']))
    $zoom=intval($_REQUEST['zoom']);
  else
    $zoom=$player['zoom'];
  if($zoom!=$player['zoom'])
    query('UPDATE `'.$MySQLprefix.'player` SET `zoom`='.$zoom.' WHERE `id`='.$player['id']);
}
else
  $zoom=100;

$action=isset($_REQUEST['action'])?$_REQUEST['action']:NULL;
isset($_REQUEST['buyin']) and $buyin=intval($_REQUEST['buyin']) or $buyin=1000;
isset($_REQUEST['tablesize']) and $tablesize=intval($_REQUEST['tablesize']) or $tablesize=5;
isset($_REQUEST['sbstart']) and $sbstart=intval($_REQUEST['sbstart']) or $sbstart=20;
isset($_REQUEST['sbinc'  ]) and $sbinc  =intval($_REQUEST['sbinc'  ]) or $sbinc  =10;
isset($_REQUEST['sbend'  ]) and $sbend  =intval($_REQUEST['sbend'  ]) or $sbend =100;
if(!isset($_REQUEST['frame']))$_REQUEST['frame']=false;

$players=query('SELECT * FROM `'.$MySQLprefix.'player`');

$font='font-family:monospace;font-size:'.zoom(14).'px;white-space:pre';
$font2='font-family:monospace;font-size:'.zoom(12).'px;white-space:pre';
$font3='font-family:sans-serif;font-size:'.zoom(12).'px;white-space:normal';
$fontg='color:#808080;'.$font;
$font2g='color:#808080;'.$font2;
$font3g='color:#808080;'.$font3;
$font='color:#000000;'.$font;
$font2='color:#000000;'.$font2;
$font3='color:#000000;'.$font3;

if($_REQUEST['frame'] and $_REQUEST['frame']!='main' or !$player)
{
  echo '<html><head><title>Qpoker</title></head>';
  echo '<body style="padding:0px;margin:0px">';
  echo '<form';
  echo ' name="theform"';
  echo ' id="theform"';
  echo ' method="post"';
  echo ' action="'.$url.(($_REQUEST['frame']=='chatdisplay')?'#chatbottom':'').'" style="display:inline"';
  echo '>';
  echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
  echo '<span style="'.$font.'">';
  $htmlend='</span></form></body></html>';
}

if(isset($_REQUEST['register']))
{
  if($_REQUEST['name'] and 
     !query('SELECT id FROM `'.$MySQLprefix.'player` WHERE name="'.mes($_REQUEST['name']).'"')
     and $_REQUEST['pass1'] and $_REQUEST['pass2'] 
     and $_REQUEST['pass1']==$_REQUEST['pass2'])
    {
      $_SESSION['loggedin']=
	query('INSERT INTO `'.$MySQLprefix.'player`',
	      'SET `name`="'.mes($_REQUEST['name']).'"',
	      ', `pass`="'.md5($_REQUEST['pass1']).'"');
      echo 'Thanks for registring '.htmlentities($_REQUEST['name']).'.<br />';
      echo '<br /><a href="'.$url.'">login</a>';
    }
  else
    {
      if($_REQUEST['name'] 
	 and $_REQUEST['pass1'] and $_REQUEST['pass2'] 
	 and $_REQUEST['pass1']==$_REQUEST['pass2'])
	echo htmlentities($_REQUEST['name'])
	  .' is already used by someone else<br />';
      echo 'Name:    <input name="name" type="text" /><br />';
      if($_REQUEST['name'] 
	 and (!$_REQUEST['pass1'] or !$_REQUEST['pass2'] 
	      or $_REQUEST['pass1']!=$_REQUEST['pass2']))
	echo 'passwords didn\'t match<br />';
      echo 'Password:<input name="pass1" type="password" /><br />';
      echo 'Password:<input name="pass2" type="password" /><br />';
      echo '<input type="submit" name="register" value="register" />';
    }
}
else if(!$player)
{
  if(!$_REQUEST['frame'])
    {
      if(isset($_REQUEST['login'])) echo 'Login failed<br />';
      if(!isset($_REQUEST['name']))$_REQUEST['name']='';
      echo 'Name:    <input name="name" type="text" value="'.htmlentities($_REQUEST['name']).'" /><br />';
      echo 'Password:<input name="pass" type="password" /><br />';
      echo '<input type="submit" name="login" value="login" /><br />';
      echo '<br /><a href="?register=1">register</a><br />';

      //As long as you don't remove the following lines you will satisfy the
      //requirements of the GPL.
          echo '<span style="'.$font2.'">';
	  echo '<br /><br /><br /><br />';
	  echo '<br />You may watch the <a href="'.$url.'source=show">source code</a> and <a href="'.$url.'source=download">download</a> a copy.';
	  echo '<br />You may also redistribute is under the terms of the GPL.';
	  echo '<br />';
	  echo '<br />';
	  echo '<br />';
	  echo '<br />This program is free software; you can redistribute it and/or modify';
	  echo '<br />it under the terms of the GNU General Public License as published by';
	  echo '<br />the Free Software Foundation; either version 2 of the License, or';
	  echo '<br />(at your option) any later version.';
	  echo '<br /> ';
	  echo '<br />This program is distributed in the hope that it will be useful,';
	  echo '<br />but WITHOUT ANY WARRANTY; without even the implied warranty of';
	  echo '<br />MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the';
	  echo '<br /><a target="blank" href="http://www.gnu.org/licenses/gpl.html">GNU General Public License</a> for more details.';
	  echo '<br />';
	  echo '<br />';
	  echo '<br />All images are part of <a href="http://www.openclipart.org">openclipart</a>';
	  echo '<br />and distributed under the terms of the';
	  echo '<br /><a target="blank" href="http://www.gnu.org/licenses/fdl.html">GNU Free Documentation License</a> (GNU FDL).';
	  echo '<br />';
	  echo '<br /><a href="QpokerCards.tar.gz">download</a> the images.';
	  echo '</span>';
      //As long as you don't remove the priviouse lines you will satisfy the
      //requirements of the GPL.
          echo '<span style="'.$font2.'">';
	  echo '<br /><br /><br /><br />';
	  echo '<br />This is an alph version!';
	  echo '<br />';
	  echo '<br />To Do:';
	  echo '<br />- basics';
	  echo '<br />  - test and inprove payout';
	  echo '<br />  - leaving a table';
	  echo '<br />  - auto fold for players that left a turnament';
	  echo '<br />  - less reloading of the tablelist';
	  echo '<br />- table settings';
	  echo '<br />  - putting them on a seperate screen';
	  echo '<br />  - a maximum reaction time';
	  echo '<br />  - blind controll';
	  echo '<br />  - payouts for 2nd, 3tr ...';
	  echo '<br />- more poker variants (for now just NL Hold\'em 1 table turnaments)';
	  echo '<br />  - none turnament tables';
	  echo '<br />  - Limit Poker';
	  echo '<br />  - Pot Limit Poker';
	  echo '<br />  - other variants than Texas Hold\'em ';
	  echo '<br />- optimizations';
	  echo '<br />  - better random algorithem';
	  echo '<br />  - MySQL optimizations';
	  echo '<br />  - speed optimizations';
	  echo '<br />- even more';
	  echo '<br />  - alow a guest access to the tables';
	  echo '<br />  - spellchecking';
	  echo '<br />';
	  echo '<br />as the alpha testing is in process there will be a need for beta testing soon';
	  echo '<br />everybody who likes to put this on his server for beta testing would be a big help';
	  echo '</span>';
    }
  else
    {
      echo 'login faild or session timeout.<br />';
      echo 'please <a target="_top" href="'.$url.'">login</a> again.';
    }
}
else if(!$_REQUEST['frame'])
{
  echo '<html><head><title>Qpoker</title></head>';
  echo '<frameset cols="*,'.zoom(200).'" border="1">';
  echo '<frame';
  echo ' src="'.$url.'frame=main"';
  echo '></frame>';
  echo '<frameset rows="'.zoom(35).',*,'.zoom(40).'" border="1">';
  echo '<frame style="overflow:hidden"';
  echo ' src="'.$url.'frame=status"';
  echo '></frame>';
  echo '<frame';
  echo ' src="'.$url.'frame=chatdisplay#chatbottom"';
  echo '></frame>';
  echo '<frame style="overflow:hidden"';
  echo ' src="'.$url.'frame=chatwrite"';
  echo '></frame>';
  echo '</frameset>';
  echo '</frameset>';
  $htmlend='</html>';
}
else if($_REQUEST['frame']=='chatwrite')
{
  if(isset($_REQUEST['chatline']) and strlen($_REQUEST['chatline']))
    {
      query('INSERT INTO `'.$MySQLprefix.'chat`',
	    'SET `player`='.$player['id'].',',
	    '    `text`="'.mes($_REQUEST['chatline']).'",',
	    '    `time`='.time());
    }
  echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
  echo '<input style="width:100%;height:'.zoom(20).'px;'.$font3.'" type="text" name="chatline" /><br />';
  echo '<input style="width:100%;height:'.zoom(20).'px;'.$font2.'" type="submit" value="send" />';
}
else if($_REQUEST['frame']=='chatdisplay')
{
  $chatwhere=$chatfrom='';
  if(getGame(true))
    {
      $chatwhere=' AND `'.$MySQLprefix.'chat`.`player`=`'.$MySQLprefix.'player`.`id` AND `'.$MySQLprefix.'player`.`table`='.$table['id'];
      $chatfrom=',`'.$MySQLprefix.'player`';
    }
  $lastchatid=l(query('SELECT `'.$MySQLprefix.'chat`.`id` FROM `'.$MySQLprefix.'chat`'.$chatfrom,
		    'WHERE 1'.$chatwhere,
		    'ORDER BY `'.$MySQLprefix.'chat`.`id` DESC LIMIT 15,1'),'<,id');
  $lastchatid2=l(query('SELECT `'.$MySQLprefix.'chat`.`id` FROM `'.$MySQLprefix.'chat`'.$chatfrom,
		    'WHERE 1'.$chatwhere,
		    'ORDER BY `'.$MySQLprefix.'chat`.`id` DESC LIMIT 40,1'),'<,id');
  $verylastchatid=l(query('SELECT `'.$MySQLprefix.'chat`.`id` FROM `'.$MySQLprefix.'chat`'.$chatfrom,
		    'WHERE 1'.$chatwhere,
		    'ORDER BY `'.$MySQLprefix.'chat`.`id` DESC LIMIT 1'),'<,id');
  $chat=query('SELECT `'.$MySQLprefix.'chat`.`id`, `'.$MySQLprefix.'chat`.`text`, `'.$MySQLprefix.'player`.`name`',
	      'FROM `'.$MySQLprefix.'chat`,`'.$MySQLprefix.'player`',
	      'WHERE `'.$MySQLprefix.'player`.`id`=`'.$MySQLprefix.'chat`.`player`',
	      $chatwhere,
	      ' AND ((',
	      '`'.$MySQLprefix.'chat`.`time`>'.(time()-15*60),
              ' OR (`'.$MySQLprefix.'chat`.`id`>'.intval($lastchatid),
	      ' AND `'.$MySQLprefix.'chat`.`time`>'.(time()-60*60).')))',
              ' AND `'.$MySQLprefix.'chat`.`id`>'.intval($lastchatid2),
	      'ORDER BY `'.$MySQLprefix.'chat`.`id`');
  echo '<span style="'.$font3.'">';
  foreach($chat as $line)
      {
	echo '<b>'.$line['name'].'</b>: ';
	echo $line['text'].'<br />';
      }
  echo '<a name="chatbottom" id="chatbottom"></a></span>';
  if($table)
    makeAjax($verylastchatid,'chat&chat=table&playerid='.$player['id']);
  else
    makeAjax($verylastchatid,'chat&chat=all&playerid='.$player['id']);
}
else if($_REQUEST['frame']=='status')
{
  echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
  echo htmlentities($player['name']);
  echo ' ['.$player['money'].' DM]';
  echo ' <a target="_top" href="?logout=logout">logout</a>';
  echo '<br />Zoom: ';
  echo ' <a target="_top"';
  echo ' href="'.$url.'zoom='.($zoom+10).'"';
  echo ' style="text-decoration:none"><b>+</b></a> ';
  echo $zoom.'%';
  echo ' <a target="_top"';
  echo ' href="'.$url.'zoom='.($zoom-10).'"';
  echo ' style="text-decoration:none"><b>-</b></a>';
}
else if($_REQUEST['frame']=='main')
{
  if(isset($_REQUEST['table'])
     and $_REQUEST['table']!=='leave' 
     and ($_REQUEST['table']==='create' 
	  or $table=l(query('SELECT * FROM `'.$MySQLprefix.'table`',
			    'WHERE `id`='.intval($_REQUEST['table'])))
	  ))
    {
      if($_REQUEST['table']==='create')
	{
	  if($_REQUEST['buyin']>$player['money'])
	    {
	      echo '<html><head><title>Qpoker</title></head><body style="padding:0px;margin:0px">';
	      echo '<form';
	      echo ' name="theform"';
	      echo ' id="theform"';
	      echo ' method="post"';
	      echo ' action="'.$url.'" style="display:inline"';
	      echo '>';
	      echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
	      echo '<span style="'.$font.'">';
	      echo 'You can\'t create a table with a bigger buyin than ';
	      echo $player['money'].' DM<br />';
	      echo '<input type="submit" value="back" />';
	      echo '</span></form></body></html>';
	      exit;
	    }
	  else
	    {
	      $_REQUEST['table']=query('INSERT INTO `'.$MySQLprefix.'table`',
				       'SET admin='.$player['id'],
				       ', `timecreate`='.time(),
				       ', `size`='.$tablesize,
				       ', `dealer`='.$player['id'],
				       ', `onturn`='.$player['id'],
				       ', `buyin`='.$buyin,
				       ', `sb`='.$sbstart,
				       ', `sbstart`='.$sbstart,
				       ', `sbinc`='.$sbinc,
				       ', `sbend`='.$sbend);
	      
	      $table=l(query('SELECT * FROM `'.$MySQLprefix.'table`',
			     'WHERE `id`='.intval($_REQUEST['table'])));
	    }
	}
      $buyin=l(query('SELECT `buyin` FROM `'.$MySQLprefix.'table`',
		     'WHERE `id`='.intval($_REQUEST['table'])),'<,buyin');
      if($buyin>$player['money'] 
	 or count(query('SELECT `id` FROM `'.$MySQLprefix.'player`',
			'WHERE `table`='.$table['id']))
		  >$table['size']-1)
	{
	  echo '<html><head><title>Qpoker</title></head><body style="padding:0px;margin:0px">';
	  echo '<form';
	  echo ' name="theform"';
	  echo ' id="theform"';
	  echo ' method="post"';
	  echo ' action="'.$url.'" style="display:inline"';
	  echo '>';
	  echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
	  echo '<span style="'.$font.'">';
	  if($buyin>$player['money'])
	    {
	      echo 'You can\'t join a table with a bigger buyin than ';
	      echo $player['money'].' DM<br />';
	    }
	  else
	    echo 'All Seats are allready taken.<br />';
	  echo '<input type="submit" value="back" />';
	  echo '</span></form></body></html>';
	  exit;
	}
      else
	{
	  query('UPDATE `'.$MySQLprefix.'player`',
		'SET `table`='.$table['id'],
		', `seat`='.(($_REQUEST['table']==='create')?'0':
		  l(query('SELECT MAX(`seat`) AS maxseat',
			  'FROM `'.$MySQLprefix.'player`',
			  'WHERE `table`='.intval($table['id'])),
		    '<,maxseat')+1),
		', `money`=`money`-'.$buyin,
		', `tablemoney`='.intval($buyin),
		', `betmoney`=0',
		', `allinpot`=0',
		', `action`="none"',
		', `actiontime`=0',
		', `tablestatus`="in"',
		', `timein`='.time(),
		', `timeout`=0',
		'WHERE `id`='.$player['id']);
	}
    }
  if(!getGame())
    {
      //table list
      $wait=query('SELECT MAX(`timein`) AS maxti, MAX(`timeout`) AS maxto',
		  'FROM `'.$MySQLprefix.'player`');
      $wait=max(l($wait,'<,maxto'),l($wait,'<,maxti'));

      echo '<html><head><title>Qpoker</title></head><body style="padding:0px;margin:0px">';
      echo '<form';
      echo ' name="theform"';
      echo ' id="theform"';
      echo ' method="post"';
      echo ' action="'.$url.'" style="display:inline"';
      echo '>';
      echo '<span style="'.$font.'">';
      echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
      $htmlend='</span></form></body></html>';

      makeAjax($wait,'tables');

      $notable=true;
      $tables=query('SELECT * FROM `'.$MySQLprefix.'table` WHERE `timeclosed`=0');
      foreach($tables as $table)
	  {
	    $p=
	      query('SELECT *',
		    'FROM `'.$MySQLprefix.'player`',
		    'WHERE `table`='.intval($table['id']),
		    'ORDER BY `seat`');
	    if(count($p)<$table['size'])
	      {
		if($notable)
		  {
		    echo 'Select a Table:<br />';
		    $notable=false;
		  }
		echo '<input style="'.$font.'" type="submit" name="table" value="';
		echo str_pad($table['id'],6,'0',STR_PAD_LEFT).' ';
		echo str_pad($table['buyin'].' DM',9,' ',STR_PAD_LEFT).' ';
		echo str_pad(
		  $table['sbstart'].'-'.$table['sbend'].' DM',12,' ',STR_PAD_LEFT);
		echo str_pad(
		  '   '.count($p).'/'.$table['size']
		  .' ('.htmlentities(implode(', ',l($p,'*,name'))).')',
		  70);
		echo '" /><br />';
	      }
	  }
      if(!$notable)
	echo '(Number, Buy-In, Small-Blind, Players)<br /><br />';
      echo 'New Table:<br />';
      echo ' Size:       <input size="2" name="tablesize" value="5" /> Players<br />';
      echo ' Buy-In:     <input size="4" name="buyin" value="1000" /> DM<br />';
      echo ' Small Bind:<br />';
      echo '  Start:     <input size="3" name="sbstart" value="20" /> DM<br />';
      echo '  Increment: <input size="3" name="sbinc" value="10" /> DM<br />';
      echo '  End:       <input size="3" name="sbend" value="100" /> DM<br />';
      echo '<input type="submit" name="table" value="create" />   ';

    }
  else if(count($players)<$table['size'])
    {
      if(isset($_REQUEST['table']) and $_REQUEST['table']==='leave')
	{
	  if($player['id']==$table['admin'])
	    {
	      foreach($players as $p)
		  {
		    query('UPDATE `'.$MySQLprefix.'player` SET `table`=0',
			  ', `money`=`money`+`tablemoney`',
			  ', `tablemoney`=0',
			  ', `timeout`='.time(),
			  'WHERE `id`='.$p['id']);
		  }
	      query('DELETE FROM `'.$MySQLprefix.'table`',
		    'WHERE `id`='.$table['id']);
	    }
	  else
	    {
	      query('UPDATE `'.$MySQLprefix.'player`',
		    'SET `table`=0',
		    ', `money`=`money`+`tablemoney`',
		    ', `tablemoney`=0',
		    ', `timeout`='.time(),
		    'WHERE `id`='.$player['id']);
	    }
	}
      echo '<html><head><title>Qpoker</title></head><body style="padding:0px;margin:0px">';
      echo '<form';
      echo ' name="theform"';
      echo ' id="theform"';
      echo ' method="POST"';
      echo ' action="'.$url.'" style="display:inline"';
      echo '>';
      echo '<span style="'.$font.'">';
      echo '<input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
      $htmlend='</span></form></body></html>';

      makeAjax(count($players),'players&tableid='.$table['id']);

      echo 'Table '.str_pad($table['id'],6,'0',STR_PAD_LEFT).'<br /><br />';
      echo nl2br(htmlentities(implode("\n",l($players,'*,name'))));
      echo '<br /><br />waiting for '.($table['size']-count($players)).' more Player(s).';
      echo '<br /><br /><input type="submit" name="table" value="leave" />';
    }
  else
    {
      echo '<html><head><title>Qpoker</title></head>';
      echo '<frameset rows="*,'.zoom(40).'" border="1">';
      echo '<frame src="'.$url.'frame=cards"></frame>';
      echo '<frame src="'.$url.'frame=actions"></frame>';
      echo '</frameset>';
      $htmlend='</html>';
    }
}
else if($_REQUEST['frame']=='cards')
{
  if(getGame())
    {
      $wait=l(query('SELECT MAX(`actiontime`) AS maxat',
		   'FROM `'.$MySQLprefix.'player`',
		   'WHERE `table`='.$table['id']),'<,maxat');

      makeAjax($wait,'cards&tableid='.$table['id']);

      foreach($players as $pid => $p)
	{
	  $thishand='';
	  $actionhtml=
	    array(
	      'none'=>'',
	      'sb'=>'small blind ',
	      'bb'=>'big blind ',
	      'fold'=>'<b>fold</b> ',
	      'call'=>'<b>call</b> ',
	      'check'=>'check ',
	      'allin'=>'<b>all-in</b> ',
	      'bet'=>'<b>bet</b> ',
	      'raise'=>'<b>raise</b> ',
	      );
	  if($p['tablestatus']=="out")
	    $thishand='<b>out</b> '.$thishand;
	  else if($table['onturn']==$pid) 
	    $thishand=$actionhtml[$p['action']].$thishand;

	  $other[$pid]=' ';
	  if($table['onturn']==$pid) 
	    $other[$pid].='<span style="background-color:#FFFF00">';
	  $other[$pid].=$p['name'];
	  if($pid==$table['dealer']) $other[$pid].=' (D)';
	  $other[$pid].=' ['.intval($p['tablemoney']).' DM]';
	  if($table['onturn']==$pid) 
	    $other[$pid].='</span>';
	  $other[$pid].= '<table><tr><td><pre>';
	  if($p['showcards'])
	    {
	      $other[$pid].=cardimgs($pcards[$pid],($pid==$player['id'])?100:70);
	    }
	  else
	    {
	      if($pid==$player['id'])// and in_array($pid,$seated))
		{
		  $other[$pid].=cardimgs($pcards[$pid],100);
		}
	      else
		{
		  $other[$pid].= cardimgs('xxxx',($pid==$player['id'])?100:70);
		}
	    }
	  $other[$pid].=' </pre></td>';
	  $other[$pid].='<td valign="bottom" align="right" style="'.$font2.'">';
	  $other[$pid].=moneyimgs(intval($p['betmoney']));
	  $other[$pid].='</td>';
	  $other[$pid].='<td width="25"></td></tr></table>';
	  $other[$pid].=' '.$thishand;
	  
	} 
      
      
      echo '<table><tr>';
      $pid=nextplayer($player['id']);
      while ($pid!=$player['id'])
	{
	  echo '<td style="'.$font2.'">'.$other[$pid].'</td>';
	  $pid=nextplayer($pid);
	}
      echo '</tr></table><br />';
      
      echo '<table><tr>';
      
      echo '<td><span style="'.$font2.'"> ';
      if($table['betround']==0 and $table['nextbetround'])
	echo '<span style="background-color:#FFFF00">flop</span>';
      else
	echo 'flop';
      echo '<br />';
      if($table['betround']>0) 
	{
	  echo cardimgs(substr($ccards,0,6),70);
	}
      else echo cardimgs('xxxxxx',70);
      echo ' </span></td>';
      
      echo '<td><span style="'.$font2.'"> ';
      if($table['betround']==1 and $table['nextbetround'])
	echo '<span style="background-color:#FFFF00">turn</span>';
      else
	echo 'turn';
      echo '<br />';
      if($table['betround']>1) 
	echo cardimgs(substr($ccards,6,2),70);
      else 
	echo cardimgs('xx',70);
      echo ' </span></td>';
      
      echo '<td><span style="'.$font2.'"> ';
      if($table['betround']==2 and $table['nextbetround'])
	echo '<span style="background-color:#FFFF00">river</span>';
      else
	echo 'river';
      echo '<br />';
      if($table['betround']>2) echo cardimgs(substr($ccards,8,2),70);
      else echo cardimgs('xx',70);
      echo ' </span></td>';
      
      echo '<td valign="bottom" align="right"><span style="'.$font2.'">';
      echo moneyimgs(intval($table['pot']));
      echo ' </span></td>';
      echo '<td width="25"></td>';
      echo '</tr></table><br /><br />';
      
      echo $other[$player['id']];
    }
  else die('fatal error: wrong table');
}
else if($_REQUEST['frame']=='actions')
{
  if(getGame() and count($playersseated)>1)
    {
      $maxbetmoney=max(l($players,'*,betmoney'));

      $maxbetmoney2=0;
      foreach($players as $p)
	  {
	    if($p['tablemoney'] 
	       and $p['betmoney']<$maxbetmoney 
	       and $p['betmoney']>$maxbetmoney2
	       ) $maxbetmoney2=$p['betmoney'];
	  }

      $minbet=2*$maxbetmoney-$maxbetmoney2;
      if($minbet<$table['sb']*2){$minbet=$table['sb']*2;}
      

      if(!isset($_REQUEST['preaction']) 
	 or $_REQUEST['preaction']==='call' 
	 and $_REQUEST['pacallmoney']!=$maxbetmoney
	 or $_REQUEST['preaction']==='check'
	 and $player['betmoney']<$maxbetmoney)
	{
	  $_REQUEST['preaction']='none';
	}
      if(!isset($_REQUEST['action']))$_REQUEST['action']=false;
      if($table['onturn']==$player['id'])
	{
	  if($_REQUEST['preaction']==='call') $_REQUEST['action']='call';
	  if($_REQUEST['preaction']==='check') $_REQUEST['action']='check';
	  if($_REQUEST['preaction']==='fold')
	    {
	      if($player['betmoney']>=$maxbetmoney) $_REQUEST['action']='check';
	      else $_REQUEST['action']='fold';
	    }
	
	  if(substr($_REQUEST['action'],0,4)=='call') $_REQUEST['action']='call';

	  if($_REQUEST['action']=='call')
	    {
	      $_REQUEST['money']=$maxbetmoney;
	    }
	  if($_REQUEST['action']=='bet' or $_REQUEST['action']=='call' or $_REQUEST['action']=='raise')
	    {
	      if($_REQUEST['money']>=$player['tablemoney']+$player['betmoney'])
		{
		  $_REQUEST['money']=$player['tablemoney']+$player['betmoney'];
		  $_REQUEST['action']="allin";
		}
	      if($_REQUEST['action']!='call' and $_REQUEST['action']!="allin" and $_REQUEST['money']<$minbet)
		{
		  echo 'Du Depp der Mindesteinsatz ist doch '.$minbet.'. <br />';
		  echo 'Oder warum stand das wohl in dem Feld. <br />';
		  echo 'Sch&auml;m dich und stell dich in die Ecke. <br />';
		  echo 'Pocker spielen kanst du ja so wie so nicht.';
		}
	      else
		{
		  query('UPDATE `'.$MySQLprefix.'player`',
			'SET `action`="'.mes($_REQUEST['action']).'"',
			', `actiontime`='.time(),
			', `betmoney`='.$_REQUEST['money'],
			', `tablemoney`=`tablemoney`-'.(
			  $_REQUEST['money']
			  -$player['betmoney']),
			'WHERE `id`='.$player['id']);
		  query('UPDATE `'.$MySQLprefix.'table` SET `onturn`='.nextactive($player['id']),
			'WHERE `id`='.$table['id']);
		}
	      getGame();
	    }
	  if($_REQUEST['action']=='fold' and $player['betmoney']>=$maxbetmoney) 
	    $_REQUEST['action']='check';
	  if($_REQUEST['action']=='check' and $player['betmoney']>=$maxbetmoney) 
	    {
	      query('UPDATE `'.$MySQLprefix.'player`',
		    'SET `action`="'.mes($_REQUEST['action']).'"',
		    ', `actiontime`='.time(),
		    'WHERE `id`='.$player['id']);
	      query('UPDATE `'.$MySQLprefix.'table` SET `onturn`='.nextactive($player['id']),
		    'WHERE `id`='.$table['id']);
	      getGame();
	    }
	  if($_REQUEST['action']=='fold')
	    {
	      query('UPDATE `'.$MySQLprefix.'player`',
		    'SET `action`="'.mes($_REQUEST['action']).'"',
		    ', `actiontime`='.time(),
		    'WHERE `id`='.$player['id']);
	      query('UPDATE `'.$MySQLprefix.'table` SET `onturn`='.nextactive($player['id']),
		    'WHERE `id`='.$table['id']);
	      getGame();
	    }
	}  
      if(isset($_REQUEST['showcards']))
	{
	  query('UPDATE `'.$MySQLprefix.'player`',
		'SET `showcards`=1',
		', `actiontime`='.time(),
		'WHERE `id`='.$player['id']);
	}
      if(count($playersactive)<=1 and in_array('allin',l($players,'*,action')))
	{
	  foreach($players as $pid => $p)
	      {
		if($p['action']=='allin' or playerisactive($pid) and !$p['showcards'])
		  query('UPDATE `'.$MySQLprefix.'player`',
			'SET `showcards`=1',
			', `actiontime`='.time(),
			'WHERE `id`='.$pid);
	      }
	}
      getGame();
      
      do{
	$maxbetmoney=max(l($players,'*,betmoney'));
	
	$brdone=true;
	if(!$table['nextbetround'])
	  {
	    foreach($players as $p)
		{
		  if( $p['tablemoney'] and (
			($p['betmoney']<$maxbetmoney and $p['action']!="fold") 
			or 
			($p['action']=="none" or $p['action']=="bb")
			)) 
		    $brdone=false;
		}
	    
	    if(count($playersactive)<=1) $brdone=true;
	  }
	if(!$table['deck'])$brdone=true;
	
	if($brdone)
	  {
	    if(!$table['nextbetround'])
	      {
		
		collectbets();
		
		if($table['betround']>=3) showdown();
		
		query('UPDATE `'.$MySQLprefix.'table` ',
		      'SET `nextbetround`='.($table['nextbetround']=(time()+5)),
		      ', `onturn`=0',
		      'WHERE `id`='.$table['id']);
	      }
	    else if($table['nextbetround']<time())
	      {
		foreach($players as $p)
		    {
		      query('UPDATE `'.$MySQLprefix.'player`',
			    'SET `action`="'.
			    (($p['action']=="fold")?"fold":
			     (($p['action']=="allin")?"allin":"none")).'"',
			    ', `actiontime`='.time(),
			    'WHERE `id`='.$p['id']);
		    }
		getGame();
		query('UPDATE `'.$MySQLprefix.'table`',
		      'SET `nextbetround`=0',
		      ', `onturn`='.(count($playersactive)?nextactive($table['dealer']):'0'),
		      ', `betround`=`betround`+1',
		      'WHERE `id`='.$table['id']);
		$brdone=false;
		if($table['betround']>=3 or $table['deck']=='' or count($playersin)<=1)
		  {
		    if($table['deck']=='')
		      firsthand();
		    else
		      nexthand();
		    
		    postblinds();
		  }
	      }
	    getGame();
	  }
      } while($brdone && !$table['nextbetround']);

      if($brdone)
	{
	  makeAjax($table['nextbetround'],'time&time='.$table['nextbetround']);
	  /*
	  echo 'waiting for ';
	  switch ($table['betround'])
	    {
	    case 0: echo 'the flop'; break;
	    case 1: echo 'the turn'; break;
	    case 2: echo 'the river'; break;
	    case 3: echo 'new cards'; break;
	    }
	  */
	}
      else
	{
	  makeAjax($table['onturn'],'actions&tableid='.$table['id']);
	}
	{
	  $maxbetmoney=max(l($players,'*,betmoney'));
	  
	  echo '<table cellpadding="0">';
	  echo '<tr>';
	  
	  echo '<td width="'.zoom(60).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.'">';
	  if(!$brdone and $table['onturn']==$player['id'] 
	     and $player['betmoney']+$player['tablemoney']>$maxbetmoney)
	    {
	      if($maxbetmoney)
		{
		  echo '<span style="'.$font2.'">by </span>';
		  echo '<input style="border:1px solid #000;padding:1px;vertical-align:bottom;'.$font2.';width:'.zoom(40).'px;height:'.zoom(16).'px;text-align:right"';
		  echo ' type="text" name="money2" value="'.($minbet-$maxbetmoney).'" id="moneyby"';
		  echo ' onkeyup="document.getElementById(&quot;moneyto&quot;).value';
		  echo '=Number(this.value)+'.$player['betmoney'];
		  echo '"';
		  echo ' onchange="';
		  echo 'if(Number(this.value)<'.($minbet-$maxbetmoney).')this.value='.($minbet-$maxbetmoney).';';
		  echo 'if(Number(this.value)>'.$player['tablemoney'].')this.value='.$player['tablemoney'].';';
		  echo 'document.getElementById(&quot;moneyto&quot;).value';
		  echo '=Number(this.value)+'.$player['betmoney'];
		  echo '"';
		  echo ' /><br>';
		  echo '<span style="'.$font2.'">to </span>';
		  echo '<input style="border:1px solid #000;vertical-align:bottom;'.$font2.';width:'.zoom(40).'px;height:'.zoom(16).'px;text-align:right"';
		  echo ' type="text" name="money"  value="'.$minbet.'" id="moneyto"';
		  echo ' onkeyup="document.getElementById(&quot;moneyby&quot;).value';
		  echo '=Number(this.value)-'.$player['betmoney'];
		  echo '"';
		  echo ' onchange="';
		  echo 'if(Number(this.value)<'.intval($minbet).')this.value='.intval($minbet).';';
		  echo 'if(Number(this.value)>'.($player['tablemoney']+$player['betmoney']).')this.value='.($player['tablemoney']+$player['betmoney']).';';
		  echo 'document.getElementById(&quot;moneyby&quot;).value';
		  echo '=Number(this.value)-'.$player['betmoney'];
		  echo '"';
		  echo ' />';
		}
	      else
		{
		  echo '<input style="'.$font.';width:'.zoom(60).'px;height:'.zoom(32).'px;text-align:right"';
		  echo ' type="text" name="money" value="'.$minbet.'" /><br>';
		}
	    }
	  echo '</td>';
	  
	  echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.';border:1px solid black;background-color:#d0d0d0">';
	  if(!$brdone and $table['onturn']==$player['id'] 
	     and $player['betmoney']+$player['tablemoney']>$maxbetmoney)
	    {
	      echo '<input style="'.$font.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:1px solid black;background-color:#d0d0d0"';
	      echo ' type="submit" name="action" value='.($maxbetmoney?'raise':'bet').' />';
	    }
	  else echo $maxbetmoney?'raise':'bet';
	  echo '</td>';
	  
	  echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.';border:1px solid black;background-color:#d0d0d0">';
	  if($brdone or $player['betmoney']>=$maxbetmoney) echo 'call';
	  else if($table['onturn']!=$player['id'])
	    {
	      echo '<input type="radio" name="preaction" value="call" id="pacall"';
	      if($_REQUEST['preaction']==='call')
		echo ' checked="checked"';
	      echo ' /><label style="'.$font.'" for="pacall"> call ';
	      if($player['betmoney']+$player['tablemoney']<=$maxbetmoney)
		echo '(allin)';
	      else echo $maxbetmoney.' DM';
	      echo '<input type="hidden" name="pacallmoney" value="'.$maxbetmoney.'" />';
	      echo '</label>';
	    }
	  else
	    {
	      echo '<input style="'.$font.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:1px solid black;background-color:#d0d0d0"';
	      echo ' type="submit" name="action" value="call ';
	      if($player['betmoney']+$player['tablemoney']<=$maxbetmoney)
		echo '(allin)';
	      else echo $maxbetmoney.' DM';
	      echo '" />';
	    }
	  echo '</td>';
	  
	  echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.';border:1px solid black;background-color:#d0d0d0">';
	  if($brdone or $player['betmoney']<$maxbetmoney) echo 'check';
	  else if($table['onturn']!=$player['id'])
	    {
	      echo '<input type="radio" name="preaction" value="check" id="pacheck"';
	      if($_REQUEST['preaction']==='check')
		echo ' checked="checked"';
	      echo ' /><label style="'.$font.'" for="pacheck"> check</label>';
	    }
	  else
	    echo '<input style="'.$font.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:1px solid black;background-color:#d0d0d0" type="submit" name="action" value="check" />';
	  echo '</td>';
	  
	  echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.';border:1px solid black;background-color:#d0d0d0">';
	  if(!$brdone and $table['onturn']!=$player['id'])
	    {
	      echo '<input type="radio" name="preaction" value="fold" id="pafold"';
	      if($_REQUEST['preaction']==='fold')
		echo ' checked="checked"';
	      echo ' /><label style="'.$font.'" for="pafold"> fold</label>';
	    }
	  else
	    {
	      echo '<input style="'.$font.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:1px solid black;background-color:#d0d0d0"';
	      echo ' type="submit" name="action" value="fold" />';
	    }
	  echo '</td>';
	  
	  if(!$brdone and $table['onturn']!=$player['id'])
	    {
	      echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.';border:1px solid black;background-color:#d0d0d0">';
	      echo '<input type="radio" name="preaction" value="none" id="panone"';
	      if($_REQUEST['preaction']==='none')
		echo ' checked="checked"';
	      echo ' /><label style="'.$font.'" for="panone"> No Action</label>';
	    }
	  else
	    echo '<td width="'.zoom(120).'" height="'.zoom(32).'">&nbsp;';
	  echo '</td>';
      
	  echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$fontg.';border:1px solid black;background-color:#d0d0d0">';
	  if(!$player['showcards'])
	    echo '<input style="'.$font.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:1px solid black;background-color:#d0d0d0" type="submit" name="showcards" value="show cards" />';
	  else echo 'show cards';
	  echo '</td>';
	  
	  echo '</tr>';
	  echo '</table>';
        }
      if(isset($bug)) echo $bug;
    }
  else if(isset($table))
    {
      if(count($playersseated)==1)
	{
	  query('UPDATE `'.$MySQLprefix.'player`',
		'SET `status`="winner"',
		', `money`=`tablemoney`',
		', `tablemoney`=0',
		'WHERE `id`='.l($playersseated,'<,id'));
	  query('UPDATE `'.$MySQLprefix.'table`',
		'SET `timeclosed`='.time(),
		'WHERE `id`='.$table['id']);
	}
      if($player['id']==l($playersseated,'<,id') or $player['status']=='winner') echo 'you won!';
    }
  else die('fatal error: wrong table');

// echo '<script language="JavaScript">setTimeout("document.getElementById(\'theform\').submit()",5000);</script>';
// echo '<td width="'.zoom(80).'"><input style="'.$font.'" type="submit" name="action" value="reload"></td>';
}
function hands()
{
  global $nums,$nums2,$nums3,$nums4,$cols,$cols2,$handnames,$handranks;

  $handnames=array();

  $handnames['rf']['name']='Royal Flush';
  $handnames['sf']['name']='Strait Flush';
  for($n=13;$n>=5;$n--)
    {
      $handnames['sf'.$nums2[$n]]['name']='Strait Flush '.$nums[$n];
    }
  $handnames['4k']['name']='4 of a kind';
  for($n1=14;$n1>1;$n1--)
    {
      $handnames['4k'.$nums2[$n1]]['name']='4 of a kind '.$nums[$n1].'';
      for($n2=14;$n2>2;$n2--)if($n1!=$n2)
	{
	  $handnames['4k'.$nums2[$n1].$nums2[$n2]]['name']='4 of a kind '.$nums[$n1].' (Kicker: '.$nums[$n2].')';
	}
    }
  $handnames['fh']['name']='Full House';
  for($n1=14;$n1>1;$n1--)for($n2=14;$n2>1;$n2--)if($n1!=$n2)
    {
      $handnames['fh'.$nums2[$n1].$nums2[$n2]]['name']='Full House '.$nums[$n1].', '.$nums[$n2];
    }
  
  $handnames['fl']['name']='Flush';
  for($n1=14;$n1>6;$n1--)
    {
      $handnames['fl'.$nums2[$n1]]['name']='Flush (including: '.$nums[$n1].')';
      for($n2=13;$n2>4;$n2--)if($n1>$n2)
	{
	  $handnames['fl'.$nums2[$n1].$nums2[$n2]]['name']='Flush (including: '.$nums[$n1].' '.$nums[$n2].')';
	  for($n3=12;$n3>3;$n3--)if($n2>$n3)
	    {
	      $handnames['fl'.$nums2[$n1].$nums2[$n2].$nums2[$n3]]['name']='Flush (including: '.$nums[$n1].' '.$nums[$n2].', '.$nums2[$n3].')';
	      for($n4=11;$n4>2;$n4--)if($n3>$n4)
		{
		  $handnames['fl'.$nums2[$n1].$nums2[$n2].$nums2[$n3].$nums2[$n4]]['name']='Flush (including: '.$nums[$n1].' '.$nums[$n2].', '.$nums2[$n3].', '.$nums2[$n4].')';
		  for($n5=9;$n5>1;$n5--)if($n4>$n5 and $n1>$n5+4)
		    {
		      $handnames['fl'.$nums2[$n1].$nums2[$n2].$nums2[$n3].$nums2[$n4].$nums2[$n5]]['name']='Flush (including: '.$nums[$n1].' '.$nums[$n2].', '.$nums[$n3].', '.$nums[$n4].', '.$nums[$n5].')';
		    }
		}
	    }
	}
    }
  
  $handnames['st']['name']='Strait';
  for($n=14;$n>=5;$n--)
    {
      $handnames['st'.$nums2[$n]]['name']='Strait '.$nums[$n];
    }
  
  $handnames['3k']['name']='3 of a kind';
  for($n1=14;$n1>1;$n1--)
    {
      $handnames['3k'.$nums2[$n1]]['name']='3 of a kind '.$nums[$n1].'';
      for($n2=14;$n2>2;$n2--)if($n1!=$n2)
	{
	  $handnames['3k'.$nums2[$n1].$nums2[$n2]]['name']='3 of a kind '.$nums[$n1].' (Kicker: '.$nums[$n2].')';
	  for($n3=13;$n3>1;$n3--)if($n1!=$n3 and $n2>$n3)
	    {
	      $handnames['3k'.$nums2[$n1].$nums2[$n2].$nums2[$n3]]['name']='3 of a kind '.$nums[$n1].' (Kicker: '.$nums[$n2].', '.$nums[$n3].')';
	    }
	}
    }
  
  $handnames['2p']['name']='two pairs';
  for($n1=14;$n1>1;$n1--)
    {
      $handnames['2p'.$nums2[$n1]]['name']='two pairs '.$nums[$n1].'';
      for($n2=14;$n2>1;$n2--)if($n1>$n2)
	{
	  $handnames['2p'.$nums2[$n1].$nums2[$n2]]['name']='two pairs '.$nums[$n1].', '.$nums2[$n2].'';
	  for($n3=14;$n3>1;$n3--)if($n1!=$n3 and $n2!=$n3)
	    {
	      $handnames['2p'.$nums2[$n1].$nums2[$n2].$nums2[$n3]]['name']='two pairs '.$nums[$n1].', '.$nums[$n2].' (Kicker: '.$nums[$n3].')';
	    }
	}
    }
  
  $handnames['2k']['name']='one pair';
  for($n1=14;$n1>1;$n1--)
    {
      $handnames['2k'.$nums2[$n1]]['name']='one pair '.$nums[$n1].'';
      for($n2=14;$n2>3;$n2--)if($n1!=$n2)
	{
	  $handnames['2k'.$nums2[$n1].$nums2[$n2]]['name']='one pair '.$nums[$n1].' (Kicker: '.$nums[$n2].')';
	  for($n3=13;$n3>2;$n3--)if($n1!=$n3 and $n2>$n3)
	    {
	      $handnames['2k'.$nums2[$n1].$nums2[$n2].$nums2[$n3]]['name']='one pair '.$nums[$n1].' (Kicker: '.$nums[$n2].', '.$nums2[$n3].')';
	      for($n4=12;$n4>1;$n4--)if($n1!=$n4 and $n3>$n4)
		{
		  $handnames['2k'.$nums2[$n1].$nums2[$n2].$nums2[$n3].$nums2[$n4]]['name']='one pair '.$nums[$n1].' (Kicker: '.$nums[$n2].', '.$nums[$n3].', '.$nums[$n4].')';
		}
	    }
	}
    }
  
  $handnames['hc']['name']='High Card';
  for($n1=14;$n1>8;$n1--)
    {
      $handnames['hc'.$nums2[$n1]]['name']='High Card '.$nums[$n1];
      for($n2=13;$n2>6;$n2--)if($n1>$n2)
	{
	  $handnames['hc'.$nums2[$n1].$nums2[$n2]]['name']='High Card '.$nums[$n1].' (Kicker: '.$nums[$n2].')';
	  for($n3=12;$n3>5;$n3--)if($n2>$n3)
	    {
	      $handnames['hc'.$nums2[$n1].$nums2[$n2].$nums2[$n3]]['name']='High Card '.$nums[$n1].' (Kicker: '.$nums[$n2].', '.$nums2[$n3].')';
	      for($n4=11;$n4>4;$n4--)if($n3>$n4)
		{
		  $handnames['hc'.$nums2[$n1].$nums2[$n2].$nums2[$n3].$nums2[$n4]]['name']='High Card '.$nums[$n1].' (Kicker: '.$nums[$n2].', '.$nums2[$n3].', '.$nums2[$n4].')';
		  for($n5=9;$n5>3;$n5--)if($n4>$n5 and $n1>$n5+4)
		    {
		      $handnames['hc'.$nums2[$n1].$nums2[$n2].$nums2[$n3].$nums2[$n4].$nums2[$n5]]['name']='High Card '.$nums[$n1].' (Kicker: '.$nums[$n2].', '.$nums[$n3].', '.$nums[$n4].', '.$nums[$n5].')';
		    }
		}
	    }
	}
    }
  $handranks=array();$i=0;
  foreach($handnames as $k => &$foo) $handranks[$k]=$i++;
}

function whathand($hand)
{
  global $nums,$nums2,$nums3,$nums4,$cols,$cols2;

  /* what is it */
  $flush='x';
  if(substr_count($hand,'s')>=5)$flush='s';
  if(substr_count($hand,'d')>=5)$flush='d';
  if(substr_count($hand,'h')>=5)$flush='h';
  if(substr_count($hand,'c')>=5)$flush='c';
  
  /* rf */
  if(substr_count($hand,'A'.$flush)
     and 
     substr_count($hand,'K'.$flush)
     and 
     substr_count($hand,'Q'.$flush)
     and 
     substr_count($hand,'J'.$flush)
     and 
     substr_count($hand,'1'.$flush))
   {return 'rf';}
  
  /* sf */
  for($i=13;$i>=5;$i--)
  if(substr_count($hand,$nums2[$i  ].$flush)
     and 
     substr_count($hand,$nums2[$i-1].$flush)
     and 
     substr_count($hand,$nums2[$i-2].$flush)
     and 
     substr_count($hand,$nums2[$i-3].$flush)
     and 
     substr_count($hand,$nums2[$i-4].$flush))
   {return 'sf'.$nums2[$i-0];}
  
  /* 4k */
  for($i=14;$i>1;$i--)
  if(substr_count($hand,$nums2[$i])==4)
   {
    $kicker=0;
    for($j=0;$j<7;$j++)
     {
      if($nums3[$hand[$j*2]]!=$i and $nums3[$hand[$j*2]]>$kicker)$kicker=$nums3[$hand[$j*2]];
     }
    return '4k'.$nums2[$i].$nums2[$kicker];
   }
  
  /* fh */
  for($i=14;$i>1;$i--)
  if(substr_count($hand,$nums2[$i])==3)
   {
    for($j=14;$j>1;$j--)
     {
      if($i!=$j and substr_count($hand,$nums2[$j])>=2)
       {
        return 'fh'.$nums2[$i].$nums2[$j];
       }
     }
   }
  
  /* fl */
  if($flush!='x')
   {
    $kicker1=0;
    $kicker2=0;
    $kicker3=0;
    $kicker4=0;
    $kicker5=0;
    for($j=0;$j<7;$j++)
     {
      if($nums3[$hand[$j*2]+1]==$flush)
       {
        if($nums3[$hand[$j*2]]>$kicker1)
         {
          $kicker5=$kicker4;
          $kicker4=$kicker3;
          $kicker3=$kicker2;
          $kicker2=$kicker1;
          $kicker1=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker2)
         {
          $kicker5=$kicker4;
          $kicker4=$kicker3;
          $kicker3=$kicker2;
          $kicker2=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker3)
         {
          $kicker5=$kicker4;
          $kicker4=$kicker3;
          $kicker3=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker4)
         {
          $kicker5=$kicker4;
          $kicker4=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker5)
         {
          $kicker5=$nums3[$hand[$j*2]];
         }
       }
     }
    return 'fl'.$nums2[$kicker1].$nums2[$kicker2].$nums2[$kicker3].$nums2[$kicker4].$nums2[$kicker5];
   }
  
  /* st */
  for($i=14;$i>=5;$i--)
  if(substr_count($hand,$nums2[$i  ])
     and 
     substr_count($hand,$nums2[$i-1])
     and 
     substr_count($hand,$nums2[$i-2])
     and 
     substr_count($hand,$nums2[$i-3])
     and 
     substr_count($hand,$nums2[$i-4]))
   {return 'st'.$nums2[$i-0];}
  
  /* 3k */
  for($i=14;$i>1;$i--)
  if(substr_count($hand,$nums2[$i])==3)
   {
    $kicker1=0;
    $kicker2=0;
    for($j=0;$j<7;$j++)
     {
      if($nums3[$hand[$j*2]]!=$i)
       {
        if($nums3[$hand[$j*2]]>$kicker1)
         {
          $kicker2=$kicker1;
          $kicker1=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker2)
         {
          $kicker2=$nums3[$hand[$j*2]];
         }
       }
     }
    return '3k'.$nums2[$i].$nums2[$kicker1].$nums2[$kicker2];
   }

  /* 2p */
  for($i=14;$i>1;$i--)
  if(substr_count($hand,$nums2[$i])==2)
   {
    for($j=14;$j>1;$j--)
     {
      if($i!=$j and substr_count($hand,$nums2[$j])==2)
       {
        $kicker=0;
        for($k=0;$k<7;$k++)
         {
          if($nums3[$hand[$k*2]]!=$i and $nums3[$hand[$k*2]]!=$j and $nums3[$hand[$k*2]]>$kicker)$kicker=$nums3[$hand[$k*2]];
         }
        return '2p'.$nums2[$i].$nums2[$j].$nums2[$kicker];
       }
     }
   }

  /* 2k */
  for($i=14;$i>1;$i--)
  if(substr_count($hand,$nums2[$i])==2)
   {
    $kicker1=0;
    $kicker2=0;
    $kicker3=0;
    for($j=0;$j<7;$j++)
     {
      if($nums3[$hand[$j*2]]!=$i)
       {
        if($nums3[$hand[$j*2]]>$kicker1)
         {
          $kicker3=$kicker2;
          $kicker2=$kicker1;
          $kicker1=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker2)
         {
          $kicker3=$kicker2;
          $kicker2=$nums3[$hand[$j*2]];
         }
        else if($nums3[$hand[$j*2]]>$kicker3)
         {
          $kicker3=$nums3[$hand[$j*2]];
         }
       }
     }
    return '2k'.$nums2[$i].$nums2[$kicker1].$nums2[$kicker2].$nums2[$kicker3];
   }
  
  /* hc */
  $kicker1=0;
  $kicker2=0;
  $kicker3=0;
  $kicker4=0;
  $kicker5=0;
  for($j=0;$j<7;$j++)
   {
    if($nums3[$hand[$j*2]]>$kicker1)
     {
      $kicker5=$kicker4;
      $kicker4=$kicker3;
      $kicker3=$kicker2;
      $kicker2=$kicker1;
      $kicker1=$nums3[$hand[$j*2]];
     }
    else if($nums3[$hand[$j*2]]>$kicker2)
     {
      $kicker5=$kicker4;
      $kicker4=$kicker3;
      $kicker3=$kicker2;
      $kicker2=$nums3[$hand[$j*2]];
     }
    else if($nums3[$hand[$j*2]]>$kicker3)
     {
      $kicker5=$kicker4;
      $kicker4=$kicker3;
      $kicker3=$nums3[$hand[$j*2]];
     }
    else if($nums3[$hand[$j*2]]>$kicker4)
     {
      $kicker5=$kicker4;
      $kicker4=$nums3[$hand[$j*2]];
     }
    else if($nums3[$hand[$j*2]]>$kicker5)
     {
      $kicker5=$nums3[$hand[$j*2]];
     }
   }
  return 'hc'.$nums2[$kicker1].$nums2[$kicker2].$nums2[$kicker3].$nums2[$kicker4].$nums2[$kicker5];
}

function cardimgs($cards='xx',$size=100)
{
  if(strlen($cards)>2)
    {
      $r='';
      for($i=0;$i<strlen($cards);$i+=2)
	$r.=($i?' ':'').cardimgs(substr($cards,$i,2),$size);
      return $r;
    }
  return
    '<img alt="'.$cards.'" '
        .'src="cards/'.$cards.'.png" '
        .'width="'.zoom(70/100*$size).'" '
        .'height="'.zoom(95/100*$size).'" '
    .'/>';
}
function moneyimgs($money)
{
  $chips=array(500=>"purple",100=>"black",25=>"green",10=>"blue",5=>"red",1=>"white");

  $tm=$money;
  $mychips=array();
  $r='<table cellpadding="2" cellspacing="0"><tr>';
  foreach($chips as $k => $v)
    {
      if($tm>=$k)
	{
	  $r.='<td valign="bottom">';
	  $r.='<table bgcolor="#888888" cellpadding="0" cellspacing="1">';
	  while($tm>=$k)
	    {
	      $r.='<tr><td width="'.zoom(10).'" ';
	      $r.='height="'.zoom(5).'" bgcolor="'.$v.'">';
	      $r.='</td></tr>';
	      $tm-=$k;
	    }
	  $r.='</table></td>';
	}
    }
  $r.='</tr></table>';
  if($money) $r.=$money.' DM';
  return $r;
}
function sortbyhand($p1,$p2)
{
  global $ccards,$pcards;
  return comphands(whathand($ccards.$pcards[$p1['id']]),whathand($ccards.$pcards[$p1['id']]));
}
function comphands($c1,$c2)
{
  global $handranks;
  if($c1==$c2) return 0;
  if($handranks[$c1]>$handranks[$c2]) return 1;
  return -1;
 }
function getGame($onlycheck=false)
{
  global $table,$players,$player,$playersin,$playersseated,$playersactive,$ccards,$pcards;
  if(!$table=l(query('SELECT `'.$GLOBALS['MySQLprefix'].'table`.* FROM `'.$GLOBALS['MySQLprefix'].'player`,`'.$GLOBALS['MySQLprefix'].'table`',
		     'WHERE `'.$GLOBALS['MySQLprefix'].'table`.`id`=`'.$GLOBALS['MySQLprefix'].'player`.`table`',
		     ' AND `'.$GLOBALS['MySQLprefix'].'player`.`id`='.intval($player['id']),
		     ' AND `timeclosed`=0',
		     ' AND `tablestatus`!="out"'))) return false;
  if($onlycheck) return true;
  $players=query('SELECT * FROM `'.$GLOBALS['MySQLprefix'].'player` WHERE `table`='.$table['id'].' ORDER BY `seat`');
  
  foreach($players as $k => $v)if(playerisseated($k))$playersseated[$k]=$v;
  foreach($players as $k => $v)if(playerisin($k))$playersin[$k]=$v;
  foreach($players as $k => $v)if(playerisactive($k))$playersactive[$k]=$v;
  $ccards=substr($table['deck'],0,10);
  $pcards=array();
  $i=10;
  foreach($players as $pid => $p)
      {
	$pcards[$pid]=substr($table['deck'],$i,4);
	$i+=4;
      }
  return true;
}

function nextplayer($pid)
{
  global $players;
  $found=false;
  foreach($players as $p)
    {
      if($found==true) return $p['id'];
      if($p['id']==$pid)$found=true;
    }
  return l(reset($players),'id');
}

function playerisin($pid)
{
  global $players;
  return (
    $players[$pid]['tablestatus']=="in" 
    and ($players[$pid]['tablemoney']+$players[$pid]['betmoney']+$players[$pid]['allinpot'])
    and $players[$pid]['action']!="fold"
    );
}
function playerisseated($pid)
{
  global $players;
  return (
    $players[$pid]['tablestatus']=="in" 
    and ($players[$pid]['tablemoney']+$players[$pid]['betmoney']+$players[$pid]['allinpot'])
    );
}
function playerisactive($pid)
{
  global $players;
  return (
    $players[$pid]['tablestatus']=="in" 
    and $players[$pid]['tablemoney'] 
    and $players[$pid]['action']!="fold"
    and $players[$pid]['action']!="allin"
    );
}
function nextin($pid)
{
  do{ $pid=nextplayer($pid); }while(!playerisin($pid));
  return $pid;
}
function nextseated($pid)
{
  do{ $pid=nextplayer($pid); }while(!playerisseated($pid));
  return $pid;
}
function nextactive($pid)
{
  do{ $pid=nextplayer($pid); }while(!playerisactive($pid));
  return $pid;
}

function queryNoConnect()
{
  $r.= 'Could not connect to The MySQL database<br />';
  $r.= 'MySQL error: '.mysql_error().'<br /><br />';
  $r.= 'Please make sure that ur QpokerMySQL.php contains the right data.<br /><br />';
  $r.= 'example QpokerMySQL.php:<br />';
  $r.= '<?php<br />';
  $r.= '  $MySQLuser="user";<br />';
  $r.= '  $MySQLpass="pass";<br />';
  $r.= '  $MySQLhost="localhost";<br />';
  $r.= '  $MySQLdb="databasename";<br />';
  $r.= '  $MySQLprefix="qpoker_";<br />';
  $r.= '?><br />';
  return $r;
}

function queryNoDB()
{
  return queryNoConnect();
}


function query($q,$q2='',$q3='',$q4='',$q5='',$q6='',$q7='',$q8='',$q9='')
{
  static $db;
  if(!$db)
    {
      @mysql_connect(
	$GLOBALS['MySQLhost'],
	$GLOBALS['MySQLuser'],
	$GLOBALS['MySQLpass'])
	or die(queryNoConnect());
      $db=@mysql_selectdb($GLOBALS['MySQLdb']) or die(queryNoDB());
    }
  $q=func_get_args();
  $q=implode(' ',$q);
  $r=@mysql_query($q);
  if(1 and $e=mysql_error()) echo $e.' '.$q;
  if(!$r) return false;
  if(strtolower(substr($q,0,6))=='select')
    {
      $a=array();
      $byid=false;
      if($row=@mysql_fetch_assoc($r))
	{
	  if(isset($row['id'])) $byid=true;
	  do
	    {
	      if($byid) $a[$row['id']]=$row;
	      else $a[]=$row;
	    }
	  while($row=@mysql_fetch_array($r));
	}
      return $a;
    }
  if(strtolower(substr($q,0,6))=='insert')
    return @mysql_insert_id();
  if(strtolower(substr($q,0,6))=='update')
    return @mysql_affected_rows();
  return @mysql_info();
}


// l() is a funktion simiular to the list construct but used on the other side
//   $scalar=l($array) is the same as list($scalar)=$array
// but l takes a second parameter specefiing wich key to use (number or string)
//   e.g. $scalar=l($array,2) instead of list(,,$scalar)=$array
// and l is recursive (for mulitidemensional array processing) 
// taking a array or as short form a komma seperated list
//   l($array,array(2,'id')) is the same as l($array,'2,id')
// there are 2 special values
//   '<' the first array element
//   '*' all elements (only makes sence when more values are used for recurion)
// a complex example:
/*
 $companys = array(
   '01457' => array(
     'name' => 'Small One Co. KG',
     'managers' => array(
        'Mr. Owner'
       )
    ),
   '02351' => array(
     'name' => 'Whatever LtD',
     'managers' => array(
        'Mr. Somebody',
        'Mrs. Anyone'
       )
    )
 )

//to get a list of the first listed managers of each company do:
$managers = l($companys,'*,managers,<');
//to get the all managers of a company do:
$managers = l($companys,'00235,managers');
//to get a direct companynummber to name array do:
$companynames = l($companys,'*,name');
*/
// this funktion is extremly useful to procces db results that are stored in an array s.a. query()
function l($a,$how=array('<'))
{
  if(!is_array($how))$how=explode(',',$how);
  if(!count($how)) return $a;
  if(!is_array($a)) return NULL;
  $do=array_shift($how);
  if($do==='<')
      return l(reset($a),$how);
  if($do==='*')
    {
      foreach($a as &$k)
	$k=l($k,$how);
      return $a;
    }
  return l($a[$do],$how);
}

function mes($s)
{
  return mysql_escape_string($s);
}

function zoom($i)
{
  return max(1,intval($i/100*$GLOBALS['zoom']));
}
function randdeck()
{
  global $nums2,$cols2;
  $usedcards=array();
  $deck='';
  for($i=0;$i<52;$i++)
    {
      $rand=Qrand(0,51-$i);
      sort($usedcards);
      foreach($usedcards as $usedcard)
	if($usedcard<=$rand)$rand++;
      $usedcards[]=$rand;
      $deck.=$nums2[$rand%13+1].$cols2[$rand%4];
    }
  return $deck;
} 

function randcards()
{
  global $ccards,$pcards,$players,$nums2,$cols2;
  mt_srand($GLOBALS['table']['handid']);
  
  do
    {
      $all=$ccards=
	$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)].
	$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)].
	$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)].
	$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)].
	$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)];

      $pcards=array();
      foreach($players as $pid => $p)
	$all.=$pcards[$pid]=$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)].$nums2[mt_rand(1,13)].$cols2[mt_rand(0,3)];
      
      $unique=true;
      for($i=0;$i<strlen($all);$i+=2)
	{
	  if(substr_count($all,$all[$i].$all[$i+1])>1)
	    {
	      $unique=false;
	      break;
	    }
	}
    } 
  while(!$unique);
}      

function makeAjax($wait,$post,$speed=NULL)
{
  if($speed===NULL)$speed=1000;
  $wait=intval($wait);
//echo '<a href="'.$GLOBALS['url'].'changed='.$post.'">&quot;'.$wait.'&quot;</a>';
  echo '<input style="';
  echo   $GLOBALS['font2g'].';background:none;border:none;width:'.zoom(24).'px;height:'.zoom(20).'px;';
  echo   'position:absolute;z-index:99;right:0px;top:0px';
  echo '" type="submit" value="R" title="reload frame manualy" id="reloadbutton" />';
  echo '<script language="JavaScript">';
  
  echo <<<JS
    var ajaxReq;
  var ajaxCompare="$wait";
  if (window.XMLHttpRequest)
    ajaxReq = new XMLHttpRequest();
  else if (window.ActiveXObject)
    {
      try
	  {
	    ajaxReq = new ActiveXObject("Msxml2.XMLHTTP");
	  }
      catch(e)
	  {
	    try
		{
		  ajaxReq = new ActiveXObject("Microsoft.XMLHTTP");
		}
	    catch(e){}
	  }
    }
  var tOut;
  function ajaxCheck()
      {
	if(ajaxReq.readyState == 4)
	  {
	    if(ajaxReq.status != 200)
	      {
		if(!tOut) tOut=setTimeout(
		  'document.getElementById("theform").submit()',${speed}0);
		setTimeout("makeReq()",$speed);
	      }
	    else 
	      {
		document.getElementById("reloadbutton").style.color="#808080";
		if(tOut) clearTimeout(tOut);
		if(ajaxReq.responseText==ajaxCompare)
		  setTimeout("makeReq()",$speed);
		else
		  document.getElementById("theform").submit();
	      }
	  }
      }
  
  function makeReq()
      {
	if(ajaxReq)
	  {
	    document.getElementById("reloadbutton").style.color="#000";
	    ajaxReq.onreadystatechange=ajaxCheck;
	    ajaxReq.open("POST","$GLOBALS[url].'",true);
	    ajaxReq.setRequestHeader("Content-Type", 
				     "application/x-www-form-urlencoded");
	    ajaxReq.send("changed=$post");
	  }
	else 
	  setTimeout('document.getElementById("theform").submit()',${speed}0);
      }
  setTimeout('makeReq()',$speed);
JS;
  echo '</script>';
}

function install()
{
  if($_REQUEST['install']=='tables')
    {
      if(
      query("
	CREATE TABLE `".$GLOBALS['MySQLprefix']."chat` (
	  `id` int(11) NOT NULL auto_increment,
	  `player` int(11) NOT NULL,
	  `text` text collate utf8_unicode_ci NOT NULL,
	  `time` int(11) NOT NULL,
	  PRIMARY KEY  (`id`)
	  ) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci
      ")!==false and
      query("
	CREATE TABLE `".$GLOBALS['MySQLprefix']."player` (
	  `id` int(11) NOT NULL auto_increment,
	  `name` varchar(32) collate utf8_unicode_ci NOT NULL,
	  `pass` varchar(32) collate utf8_unicode_ci NOT NULL,
	  `zoom` int(11) NOT NULL default '100',
	  `money` int(11) NOT NULL default '10000',
	  `lastseen` int(11) NOT NULL,
	  `loggedin` tinyint(1) NOT NULL,
	  `table` int(11) NOT NULL,
	  `seat` int(11) NOT NULL,
	  `tablemoney` int(11) NOT NULL,
	  `betmoney` int(11) NOT NULL,
	  `allinpot` int(11) NOT NULL,
	  `action` enum('none','call','bet','raise','check','fold','bb','sb','allin') collate utf8_unicode_ci NOT NULL default 'none',
	  `showcards` tinyint(1) NOT NULL,
	  `actiontime` int(11) NOT NULL,
	  `tablestatus` enum('in','out','guest') collate utf8_unicode_ci NOT NULL default 'in',
	  `timein` int(11) NOT NULL,
	  `timeout` int(11) NOT NULL,
	  PRIMARY KEY  (`id`)
	  ) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci
      ")!==false and
      query("
	CREATE TABLE `".$GLOBALS['MySQLprefix']."table` (
	  `id` int(11) NOT NULL auto_increment,
	  `size` int(6) NOT NULL,
	  `admin` int(11) NOT NULL,
	  `timecreate` int(11) NOT NULL,
	  `timestart` int(11) NOT NULL,
	  `timeclosed` int(11) NOT NULL,
	  `deck` VARCHAR (104) NOT NULL,
	  `dealer` int(11) NOT NULL,
	  `buyin` int(11) NOT NULL,
	  `sbstart` int(11) NOT NULL,
	  `sbinc` int(11) NOT NULL,
	  `sbend` int(11) NOT NULL,
	  `sb` int(11) NOT NULL,
	  `pot` int(11) NOT NULL,
	  `onturn` int(11) NOT NULL,
	  `betround` int(3) NOT NULL,
	  PRIMARY KEY  (`id`)
	  ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci
      ")!== false
      ) echo '<br />Tables created.';
      else echo '<br />MySQL error: '.mysql_error();
      echo '<br /><br /><a href="'.$GLOBALS['url'].'">Go back</a>';
      exit;
    }
  else
    {
      echo '<br />It looks as if you haven\'t set up the MySQL Tables yet.';
      echo '<br /><a href="'.$GLOBALS['url'].'?install=tables">Make Tables</a>.';
    }
  exit;
}
function postblinds()
{
  global $table,$players,$playersin,$playersseated;
  if(count($playersseated)>2)
    $pid=$table['onturn'];
  else
    $pid=$table['dealer'];
  if($players[$pid]['tablemoney']<=$table['sb'])
    {
      query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	    'SET `betmoney`=`tablemoney`',
	    ', `tablemoney`=0',
	    ', `action`="allin"',
	    ', `actiontime`='.time(),
	    'WHERE `id`='.$pid);
    }
  else
    {
      query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	    'SET `tablemoney`=`tablemoney`-'.$table['sb'],
	    ', `betmoney`='.$table['sb'],
	    ', `action`="sb"',
	    ', `actiontime`='.time(),
	    'WHERE `id`='.$pid);
    }
  $pid=nextactive($pid);
  if($players[$pid]['tablemoney']<=$table['sb']*2)
    {
      query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	    'SET `betmoney`=`tablemoney`',
	    ', `tablemoney`=0',
	    ', `action`="allin"',
	    ', `actiontime`='.time(),
	    'WHERE `id`='.$pid);
    }
  else
    {
      query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	    'SET `tablemoney`=`tablemoney`-'.($table['sb']*2),
	    ', `betmoney`='.($table['sb']*2),
	    ', `action`="bb"',
	    ', `actiontime`='.time(),
	    'WHERE `id`='.$pid);
    }
  query('UPDATE `'.$GLOBALS['MySQLprefix'].'table`',
	'SET `onturn`='.nextactive($pid),
	'WHERE `id`='.$table['id']);
  getGame();
}
function collectbets()
{
  global $players,$table;
  foreach($players as &$p)
      {
	if($p['action']=="allin")
	  {
	    if(!$p['allinpot'])
	      {
		$p['allinpot']=$table['pot'];
		foreach($players as $p2)
		  $p['allinpot']+=min($p['betmoney'],$p2['betmoney']);
	      }
	  }
	query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	      'SET `betmoney`=0',
	      ', `allinpot`='.$p['allinpot'],
	      'WHERE `id`='.$p['id']);
      }
  query('UPDATE `'.$GLOBALS['MySQLprefix'].'table` ',
	'SET `pot`=`pot`+'.array_sum(l($players,'*,betmoney')),
	'WHERE `id`='.$table['id']);
  getGame();
}

function showdown()
{
  hands();
  global $ccards,$pcards,$players,$table,$playersin;
  $winners=array();
  $pid=$table['onturn'] or $pid=nextin($table['dealer']);
  do
    {
      $thishand=whathand($ccards.$pcards[$pid]);
      if(!isset($maxwinnerhand) 
	 or comphands($thishand,$maxwinnerhand)+1)
	{
	  $maxwinnerhand=$thishand;
	  if(count($playersin)>1)
	    query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
		  'SET `showcards`='.($players[$pid]['showcards']=1),
		  'WHERE `id`='.$pid);
	}

      $winners[$pid]=$thishand;

      $pid=nextin($pid);
    } while($pid!=$table['onturn']);
  $handscount=array_count_values($winners);
  uasort($winners,'comphands');
  
  foreach($players as &$p)
      {
	if(!$p['allinpot'])$p['allinpot']=$table['pot'];
      }
  unset($p);

  while(count($winners))
      {
	reset($winners);
	$pid=key($winners);
	$hand=$winners[$pid];
	$p=&$players[$pid];
	if($handscount[$hand]==1)
	  {
	    $p['tablemoney']+=$p['allinpot'];
	    $table['pot']-=$p['allinpot'];
	    foreach($winners as $opid => $ohand)
		{
		  if($players[$opid]['allinpot']<=$p['allinpot'])
		    unset($winners[$opid]);
		  else
		    $players[$opid]['allinpot']-=$p['allinpot'];
		}
	  }
	else
	  {
	    $temppot=$p['allinpot'];
	    foreach($winners as $opid => $ohand)
		{
		  $op=&$players[$opid];
		  if($hand==$ohand)
		    {
		      if($p['allinpot']>$op['allinpot'])$temppot+=$p['allinpot']-$op['allinpot'];
		    }
		  unset($op);
		}
	    $p['tablemoney']+=intval($temppot/$handscount[$hand]);
	    $table['pot']-=intval($temppot/$handscount[$hand]);
	    $handscount[$hand]--;
	    unset($winners[$pid]);
	  }
	query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	      'SET `tablemoney`='.$p['tablemoney'],
	      ', `allinpot`=0',
	      'WHERE `id`='.$pid);
	unset($p);
      }
  query('UPDATE `'.$GLOBALS['MySQLprefix'].'table` SET `pot`='.$table['pot'],
	'WHERE `id`='.$table['id']);
  getGame();  
}
function firsthand()
{
  nexthand(true);
}
function nexthand($firsthand=false)
{
  global $players,$table;
  foreach($players as $p)
      {
	query('UPDATE `'.$GLOBALS['MySQLprefix'].'player`',
	      'SET `action`="none"',
	      ', `allinpot`=0',
	      ($p['tablemoney']?'':', `status="out", timeout='.time()),
	      ', `showcards`=0',
	      ', `actiontime`='.time(),
	      'WHERE `id`='.$p['id']);
      }
  if($table['dealer']==$table['admin'] and  !$firsthand)
    $table['sb']=min($table['sb']+$table['sbinc'],$table['sbend']);
  getGame();
  
  mt_srand();
  $table['dealer']=nextseated($table['dealer']);
  query('UPDATE `'.$GLOBALS['MySQLprefix'].'table`',
	'SET `dealer`='.$table['dealer'],
	', `deck`="'.randdeck().'"',
	', `betround`=0',
	', `onturn`='.nextactive($table['dealer']),
	', `sb`='.$table['sb'],
	'WHERE `id`='.$table['id']);
  
  getGame();
}
function echo_r($var)
{
  if(func_num_args()>1)
    {
      $var=func_get_args();
      $var=implode(' ',$var);
    }
  if(is_string($var))echo nl2br("\n\"".mes($var)."\"\n");
  else echo nl2br("\n".print_r($var)."\n");
  return $var;
}

function Qrand($from,$to)
{
  static $srand=false;
  static $urand=true;
  static $drand=true;
  if($from>$to) return false;
  if($from==$to) return $from;
  $range=$to-$from+1;
  if($drand and $drand=fopen('/dev/random','rb') and stream_set_blocking($drand, 0))
    {
      for($i=1;$range>pow(256,$i-1)-1;$i++) $chars=$i;
      $max=pow(256,$chars)-1;
      $max-=$max%($range);
      do
	{
	  $rand=0;
	  $char='dummy';
	  for($i=1;$i<=$chars and strlen($char);$i++)
	    {
	      if(strlen($char=fgetc($drand)))
		$rand+=ord($char)*pow(256,$i-1);
	    }
	}
      while($rand>$max and strlen($char));
      fclose($drand);
      if(!strlen($char))$drand=false;
      else return $rand%($range)+$from;
    }
  if($urand and $urand=fopen('/dev/urandom','rb'))
    {
      for($i=1;$range>pow(256,$i-1)-1;$i++) $chars=$i;
      $max=pow(256,$chars)-1;
      $max-=$max%($range);
      do
	{
	  $rand=0;
	  for($i=1;$i<=$chars;$i++) 
	    $rand+=ord(fgetc($urand))*pow(256,$i-1);
	}
      while($rand>$max);
      fclose($urand);
      return $rand%($range)+$from;
    }
  if(!$srand)
    {
      mt_srand();
      $srand=true;
    }
  return mt_rand($from,$to);
}


echo $htmlend; 



/*

		    GNU GENERAL PUBLIC LICENSE
		       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

			    Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
License is intended to guarantee your freedom to share and change free
software--to make sure the software is free for all its users.  This
General Public License applies to most of the Free Software
Foundation's software and to any other program whose authors commit to
using it.  (Some other Free Software Foundation software is covered by
the GNU Lesser General Public License instead.)  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
this service if you wish), that you receive source code or can get it
if you want it, that you can change the software or use pieces of it
in new free programs; and that you know you can do these things.

  To protect your rights, we need to make restrictions that forbid
anyone to deny you these rights or to ask you to surrender the rights.
These restrictions translate to certain responsibilities for you if you
distribute copies of the software, or if you modify it.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must give the recipients all the rights that
you have.  You must make sure that they, too, receive or can get the
source code.  And you must show them these terms so they know their
rights.

  We protect your rights with two steps: (1) copyright the software, and
(2) offer you this license which gives you legal permission to copy,
distribute and/or modify the software.

  Also, for each author's protection and ours, we want to make certain
that everyone understands that there is no warranty for this free
software.  If the software is modified by someone else and passed on, we
want its recipients to know that what they have is not the original, so
that any problems introduced by others will not reflect on the original
authors' reputations.

  Finally, any free program is threatened constantly by software
patents.  We wish to avoid the danger that redistributors of a free
program will individually obtain patent licenses, in effect making the
program proprietary.  To prevent this, we have made it clear that any
patent must be licensed for everyone's free use or not licensed at all.

  The precise terms and conditions for copying, distribution and
modification follow.

		    GNU GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License applies to any program or other work which contains
a notice placed by the copyright holder saying it may be distributed
under the terms of this General Public License.  The "Program", below,
refers to any such program or work, and a "work based on the Program"
means either the Program or any derivative work under copyright law:
that is to say, a work containing the Program or a portion of it,
either verbatim or with modifications and/or translated into another
language.  (Hereinafter, translation is included without limitation in
the term "modification".)  Each licensee is addressed as "you".

Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running the Program is not restricted, and the output from the Program
is covered only if its contents constitute a work based on the
Program (independent of having been made by running the Program).
Whether that is true depends on what the Program does.

  1. You may copy and distribute verbatim copies of the Program's
source code as you receive it, in any medium, provided that you
conspicuously and appropriately publish on each copy an appropriate
copyright notice and disclaimer of warranty; keep intact all the
notices that refer to this License and to the absence of any warranty;
and give any other recipients of the Program a copy of this License
along with the Program.

You may charge a fee for the physical act of transferring a copy, and
you may at your option offer warranty protection in exchange for a fee.

  2. You may modify your copy or copies of the Program or any portion
of it, thus forming a work based on the Program, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

    a) You must cause the modified files to carry prominent notices
    stating that you changed the files and the date of any change.

    b) You must cause any work that you distribute or publish, that in
    whole or in part contains or is derived from the Program or any
    part thereof, to be licensed as a whole at no charge to all third
    parties under the terms of this License.

    c) If the modified program normally reads commands interactively
    when run, you must cause it, when started running for such
    interactive use in the most ordinary way, to print or display an
    announcement including an appropriate copyright notice and a
    notice that there is no warranty (or else, saying that you provide
    a warranty) and that users may redistribute the program under
    these conditions, and telling the user how to view a copy of this
    License.  (Exception: if the Program itself is interactive but
    does not normally print such an announcement, your work based on
    the Program is not required to print an announcement.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Program,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Program.

In addition, mere aggregation of another work not based on the Program
with the Program (or with a work based on the Program) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may copy and distribute the Program (or a work based on it,
under Section 2) in object code or executable form under the terms of
Sections 1 and 2 above provided that you also do one of the following:

    a) Accompany it with the complete corresponding machine-readable
    source code, which must be distributed under the terms of Sections
    1 and 2 above on a medium customarily used for software interchange; or,

    b) Accompany it with a written offer, valid for at least three
    years, to give any third party, for a charge no more than your
    cost of physically performing source distribution, a complete
    machine-readable copy of the corresponding source code, to be
    distributed under the terms of Sections 1 and 2 above on a medium
    customarily used for software interchange; or,

    c) Accompany it with the information you received as to the offer
    to distribute corresponding source code.  (This alternative is
    allowed only for noncommercial distribution and only if you
    received the program in object code or executable form with such
    an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for
making modifications to it.  For an executable work, complete source
code means all the source code for all modules it contains, plus any
associated interface definition files, plus the scripts used to
control compilation and installation of the executable.  However, as a
special exception, the source code distributed need not include
anything that is normally distributed (in either source or binary
form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component
itself accompanies the executable.

If distribution of executable or object code is made by offering
access to copy from a designated place, then offering equivalent
access to copy the source code from the same place counts as
distribution of the source code, even though third parties are not
compelled to copy the source along with the object code.

  4. You may not copy, modify, sublicense, or distribute the Program
except as expressly provided under this License.  Any attempt
otherwise to copy, modify, sublicense or distribute the Program is
void, and will automatically terminate your rights under this License.
However, parties who have received copies, or rights, from you under
this License will not have their licenses terminated so long as such
parties remain in full compliance.

  5. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Program or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Program (or any work based on the
Program), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

  6. Each time you redistribute the Program (or any work based on the
Program), the recipient automatically receives a license from the
original licensor to copy, distribute or modify the Program subject to
these terms and conditions.  You may not impose any further
restrictions on the recipients' exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties to
this License.

  7. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Program at all.  For example, if a patent
license would not permit royalty-free redistribution of the Program by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Program.

If any portion of this section is held invalid or unenforceable under
any particular circumstance, the balance of the section is intended to
apply and the section as a whole is intended to apply in other
circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system, which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  8. If the distribution and/or use of the Program is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Program under this License
may add an explicit geographical distribution limitation excluding
those countries, so that distribution is permitted only in or among
countries not thus excluded.  In such case, this License incorporates
the limitation as if written in the body of this License.

  9. The Free Software Foundation may publish revised and/or new versions
of the General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

Each version is given a distinguishing version number.  If the Program
specifies a version number of this License which applies to it and "any
later version", you have the option of following the terms and conditions
either of that version or of any later version published by the Free
Software Foundation.  If the Program does not specify a version number of
this License, you may choose any version ever published by the Free Software
Foundation.

  10. If you wish to incorporate parts of the Program into other free
programs whose distribution conditions are different, write to the author
to ask for permission.  For software which is copyrighted by the Free
Software Foundation, write to the Free Software Foundation; we sometimes
make exceptions for this.  Our decision will be guided by the two goals
of preserving the free status of all derivatives of our free software and
of promoting the sharing and reuse of software generally.

			    NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

		     END OF TERMS AND CONDITIONS

	    How to Apply These Terms to Your New Programs

  If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

  To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
convey the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

Also add information on how to contact you by electronic and paper mail.

If the program is interactive, make it output a short notice like this
when it starts in an interactive mode:

    Gnomovision version 69, Copyright (C) year name of author
    Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, the commands you use may
be called something other than `show w' and `show c'; they could even be
mouse-clicks or menu items--whatever suits your program.

You should also get your employer (if you work as a programmer) or your
school, if any, to sign a "copyright disclaimer" for the program, if
necessary.  Here is a sample; alter the names:

  Yoyodyne, Inc., hereby disclaims all copyright interest in the program
  `Gnomovision' (which makes passes at compilers) written by James Hacker.

  <signature of Ty Coon>, 1 April 1989
  Ty Coon, President of Vice

This General Public License does not permit incorporating your program into
proprietary programs.  If your program is a subroutine library, you may
consider it more useful to permit linking proprietary applications with the
library.  If this is what you want to do, use the GNU Lesser General
Public License instead of this License.

*/?>
