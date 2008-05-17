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

  The GLP is at the of this file.
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

$logspeed=round(microtime(true)*1000);

//As long as you don't remove the following lines you will always satisfy the
//requirements of the GPL.
    if($_REQUEST['source']==="show")
      {
	header('Content-type: application/xhtml+xml; charset=utf-8');
	echo '<?xml version="1.0" encoding="UTF-8"?>';
	echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">';
	echo '<html xmlns="http://www.w3.org/1999/xhtml"><head><title>Qpoker source</title></head>';
	echo '<body style="font-family:monospace;font-size:12px;white-space:pre">';
	$file=fopen(basename('Qpoker.php'),'r');
	$num=0;
	while(!feof($file))
	    {
	      echo '<b>'.str_pad(++$num,4,' ',STR_PAD_LEFT).':</b> ';
	      echo htmlspecialchars(strtr(fgets($file),array("\t"=>'        ')));
	      flush();
	    }
	die('</body></html>');
      }
    if($_REQUEST['source']==="download")
      {
	$file=fopen(basename('Qpoker.php'),'r');
	header('ContentType: application/x-php');
	header('Content-disposition: attachment; Qpoker.php');
	fpassthru($file);
	exit;
      }
//As long as you don't remove the priviose lines you will always satisfy the
//requirements of the GPL.


function prepare_input(&$var)
{
  if(is_array($var))
   {
    foreach($var as $key => $value)
     {
      prepare_input($var[$key]);
     }
   }
  else
   {
    if(get_magic_quotes_gpc()) $var=stripslashes($var);
    $var=str_replace("\r\n","\n",$var); //windows linefeeds
    $var=str_replace("\r","\n",$var); //mac linefeeds
   }
}

prepare_input($_POST); 
prepare_input($_GET);
prepare_input($_REQUEST);


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
      if(intval($_REQUEST['time'])<time()) { logspeed();die('reload');}
      logspeed();die(strval(intval($_REQUEST['time'])));
    case 'chat':
      if($t=@mysql_fetch_assoc(
	   @mysql_query(
	     'SELECT `table` FROM `'.$MySQLprefix.'player` '
	     .'WHERE `id`='.intval($_REQUEST['playerid']).' AND `table`!=0')))
	{
	  if($_REQUEST['chat']=='all') { logspeed();die("reload"); }
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
	  if($_REQUEST['chat']=='table') { logspeed();die("reload"); }
	  $t=@mysql_fetch_assoc(
	    @mysql_query(
	      'SELECT `id` FROM `'.$MySQLprefix.'chat` '
	      .'ORDER BY `id` DESC LIMIT 1'));
	}
      logspeed();die(strval(intval($t['id'])));
    case 'cards':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT MAX(`actiontime`) AS maxat '
	  .'FROM `'.$MySQLprefix.'table`,`'.$MySQLprefix.'player` '
	  .'WHERE `'.$MySQLprefix.'player`.`table`='.intval($_REQUEST['tableid'])));
      logspeed();die(strval(intval($t['maxat'])));
    case 'tables':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT MAX(`timein`) AS maxti,  MAX(`timeout`) AS maxto '
	  .'FROM `'.$MySQLprefix.'player`'));
      logspeed();die(strval(max($t['maxto'],$t['maxti'])));
    case 'players':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT MAX(`timein`) AS mti FROM `'.$MySQLprefix.'player` '
	  .'WHERE `table`='.intval($_REQUEST['tableid'])));
      logspeed();die(strval(intval($t['mti'])));
    case 'actions':
      $t=@mysql_fetch_assoc(
	@mysql_query(
	  'SELECT `onturn` FROM `'.$MySQLprefix.'table` '
	  .'WHERE `id`='.intval($_REQUEST['tableid'])));
      logspeed();die(strval(intval($t['onturn'])));
    }
}

if(isset($_REQUEST['sid'])) session_id($_REQUEST['sid']);

session_start();

$url='Qpoker.php?';
if(isset($_REQUEST['sid']))$url.='sid='.session_id().'&';

ini_set('error_reporting',E_ALL);

$variants=array();
$variants['texasholdem']=array(
  'name' => 'Texas Hold\'em',
  );

$limits=array();
$limits['limit']=array(
  'name' => 'Limit',
  );
$limits['potlimit']=array(
  'name' => 'Pot Limit',
  );
$limits['nolimit']=array(
  'name' => 'No Limit',
  );

$numsHTML=array('','Ace','2','3','4','5','6','7','8','9','10','Jack','Queen',' King','Ace');
$numsHTMLp=array('','Aces','2','3','4','5','6','7','8','9','10','Jacks','Queens',' Kings','Aces');
$nums2=array('','a','2','3','4','5','6','7','8','9','t','j','q','k','a');
$nums3=array_flip($nums2);
$colsHTML=array('&#x2660;','&#x2663;','&#x2665;','&#x2666;');
$cols2=array('s','c','h','d');
$cols3=array_flip($cols2);

if(isset($_REQUEST['logout']))
{
  unset($_SESSION['loggedin']);
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
    {
      if($player['zoom']!=intval($_REQUEST['zoom']))
	query('UPDATE `'.$MySQLprefix.'player` SET `zoom`='.intval($_REQUEST['zoom']).' WHERE `id`='.$player['id']);
      $player['zoom']=intval($_REQUEST['zoom']);
    }
}

$action=isset($_REQUEST['action'])?$_REQUEST['action']:NULL;
if(!isset($_REQUEST['frame']))$_REQUEST['frame']=false;

//$players=query('SELECT * FROM `'.$MySQLprefix.'player`');

$font1='font-family:monospace;font-size:'.zoom(14).'px;white-space:pre';
$font2='font-family:monospace;font-size:'.zoom(12).'px;white-space:pre';
$font3='font-family:sans-serif;font-size:'.zoom(12).'px;white-space:normal';
$font1g='color:#808080;'.$font1;
$font2g='color:#808080;'.$font2;
$font3g='color:#808080;'.$font3;
$font1='color:#000000;'.$font1;
$font2='color:#000000;'.$font2;
$font3='color:#000000;'.$font3;

if($_REQUEST['frame'] or !$player)
{
  header('Content-type: application/xhtml+xml; charset=utf-8');
  echo '<?xml version="1.0" encoding="UTF-8"?>';
  echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">';
  echo '<html xmlns="http://www.w3.org/1999/xhtml"><head><title>Qpoker</title></head>';
  echo '<body style="padding:0px;margin:0px">';
  echo '<form';
  echo ' id="theform"';
  echo ' method="post"';
  echo ' action="'.htmlspecialchars($url.(($_REQUEST['frame']=='chatdisplay')?'#chatbottom':'')).'" style="display:inline"';
  echo '>';
  echo '<div style="'.$font1.'"><input type="hidden" name="frame" value="'.$_REQUEST['frame'].'" />';
  $htmlend='</div></form></body></html>';
}

if(isset($_REQUEST['register']))
{
  if(isset($_REQUEST['name']) and $_REQUEST['name'] and 
     !query('SELECT id FROM `'.$MySQLprefix.'player` WHERE name="'.mes($_REQUEST['name']).'"')
     and $_REQUEST['pass1'] and $_REQUEST['pass2'] 
     and $_REQUEST['pass1']==$_REQUEST['pass2'])
    {
      $_SESSION['loggedin']=
	query('INSERT INTO `'.$MySQLprefix.'player`',
	      'SET `name`="'.mes($_REQUEST['name']).'"',
	      ', `pass`="'.md5($_REQUEST['pass1']).'"');
      echo 'Thanks for registring '.htmlspecialchars($_REQUEST['name']).'.<br />';
      echo '<br /><a href="'.htmlspecialchars($url).'">login</a>';
    }
  else
    {
      if(isset($_REQUEST['name']) and $_REQUEST['name']
	 and $_REQUEST['pass1'] and $_REQUEST['pass2'] 
	 and $_REQUEST['pass1']==$_REQUEST['pass2'])
	echo htmlspecialchars($_REQUEST['name'])
	  .' is already used by someone else<br />';
      echo 'Name:    <input name="name" type="text" /><br />';
      if(isset($_REQUEST['name']) and $_REQUEST['name'] 
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
      echo '<div><label for="loginname">Name:</label><input id="loginname" name="name" type="text" value="'.htmlspecialchars($_REQUEST['name']).'" /></div>';
      echo '<div><label for="loginpass">Password:</label><input id="loginpass" name="pass" type="password" /></div>';
      echo '<div><input type="submit" name="login" value="login" /></div>';
      echo '<div><a href="?register=1">register</a></div>';

      //As long as you don't remove the following lines you will satisfy the
      //requirements of the GPL.
          echo '<span style="'.$font2.'">';
	  echo '<br /><br /><br />';
	  echo '<br />You may watch the <a href="'.htmlspecialchars($url).'source=show">source code</a> and <a href="'.htmlspecialchars($url).'source=download">download</a> a copy.';
	  echo '<br />You may also redistribute is under the terms of the GPL.';
	  echo '<br /><br /><br />';
	  echo '<br />This is an pre-alpha version!';
	  echo '<br />';
	  echo '<br />This program is distributed in the hope that it will be useful,';
	  echo '<br />but WITHOUT ANY WARRANTY; without even the implied warranty of';
	  echo '<br />MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the';
	  echo '<br /><a href="http://www.gnu.org/licenses/gpl.html">GNU General Public License</a> for more details.';
      //As long as you don't remove the priviouse lines you will satisfy the
      //requirements of the GPL.
	  echo '</span>';
    }
  else
    {
      echo 'login faild or session timeout.<br />';
      echo 'please <a target="_top" href="'.htmlspecialchars($url).'">login</a> again.';
    }
}
else if(!$_REQUEST['frame'])
{
  header('Content-type: application/xhtml+xml; charset=utf-8');
  echo '<?xml version="1.0" encoding="UTF-8"?>';
  echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">';
  echo '<html xmlns="http://www.w3.org/1999/xhtml"><head><title>Qpoker</title></head>';
  echo '<frameset cols="*,'.zoom(200).'" border="'.zoom(1).'">';
  echo '<frame';
  echo ' src="'.htmlspecialchars($url).'frame=main"';
  echo '></frame>';
  echo '<frameset rows="'.zoom(35).',*,'.zoom(40).'" border="'.zoom(1).'">';
  echo '<frame style="overflow:hidden"';
  echo ' src="'.htmlspecialchars($url).'frame=status"';
  echo '></frame>';
  echo '<frame';
  echo ' src="'.htmlspecialchars($url).'frame=chatdisplay#chatbottom"';
  echo '></frame>';
  echo '<frame style="overflow:hidden"';
  echo ' src="'.htmlspecialchars($url).'frame=chatwrite"';
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
  echo '<div><input style="border:'.zoom(1).'px solid #000;width:100%;height:'.zoom(18).'px;'.$font3.'" type="text" name="chatline" /></div>';
  echo '<div><input style="border:'.zoom(1).'px solid #000;width:100%;height:'.zoom(18).'px;'.$font2.'" type="submit" value="send" /></div>';
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
	echo '<b>'.htmlspecialchars($line['name']).'</b>: ';
	echo htmlspecialchars($line['text']).'<br />';
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
  echo htmlspecialchars($player['name']);
  echo ' ['.$player['money'].' DM]';
  echo ' <a target="_top" href="?logout=logout">logout</a>';
  echo '<br />Zoom: ';
  echo ' <a target="_top"';
  echo ' href="'.htmlspecialchars($url).'zoom='.($player['zoom']+10).'"';
  echo ' style="text-decoration:none"><b>+</b></a> ';
  echo $player['zoom'].'%';
  echo ' <a target="_top"';
  echo ' href="'.htmlspecialchars($url).'zoom='.($player['zoom']-10).'"';
  echo ' style="text-decoration:none"><b>-</b></a>';
}
else if($_REQUEST['frame']=='main')
{
  if(isset($_REQUEST['table']) and $_REQUEST['table']==='create')
    {
      $sbinc_select=array(
	'fast' =>'fast (short turnament)',
	'norm'=>'normal',
	'slow'=>'slow (long turnament)',
	'none'=>'no increment (very long turnament)'
	);

      $step=2;
      isset($_REQUEST['buyin'   ]) and $buyin   =intval($_REQUEST['buyin'   ]) 
	and $buyin<=$player['money']
	or $buyin =1000 and $step=1;
      isset($_REQUEST['sbstart' ]) and $sbstart =intval($_REQUEST['sbstart' ]) 
	and $sbstart<$buyin
	or $sbstart =10 and $step=1;
      isset($_REQUEST['sbinc'   ]) and isset($sbinc_select[$sbinc=$_REQUEST['sbinc']]) 
	or $sbinc   ='norm' and $step=1;
      isset($_REQUEST['payout'  ]) and $payout  =intval($_REQUEST['payout'  ]) 
	and $payout<=4
	or $payout   =1 and $step=1;
      isset($_REQUEST['size'    ]) and $size    =intval($_REQUEST['size'    ]) 
	or $size     =6 and $step=0;
      isset($_REQUEST['limit'   ]) and isset($limits  [$limit  =$_REQUEST['limit'  ]]) 
	or $limit  ='limit'       and $step=0;
      isset($_REQUEST['variant' ]) and isset($variants[$variant=$_REQUEST['variant']]) 
	or $variant='texasholdem' and $step=0;


      if($step<1)
	{
	  echo '<h1>New Table:</h1>';
	  echo '<p style="padding-left:'.zoom(40).'px">';
	  echo '<select name="limit">';
	  foreach($limits as $k => $limit)
	    echo '<option value="'.$k.'">'.$limit['name'].'</option>';
	  echo '</select> ';
	  
	  echo '<select name="variant">';
	  foreach($variants as $k => $variant)
	    echo '<option value="'.$k.'">'.$variant['name'].'</option>';
	  echo '</select> ';
	  
	  echo '<select name="size">';
	  for($i=2;$i<=16;$i++)
	    echo '<option value="'.$i.'">'.$i.' players</option>';
	  echo '</select> ';
	  
	  echo '<input type="checkbox" name="turnament" id="turnament" value="1" />';
	  echo '<label for="turnament"> turnament</label> ';
	  
	  echo '<input type="hidden" name="table" value="create" />';
	  echo '</p>';
	  echo '<div><input type="submit" value="continue" /></div>';
	}
      else if($step<2)
	{
	  echo '<h1>';
	  echo $limits[$_REQUEST['limit']]['name'].' ';
	  echo '<input type="hidden" name="limit" value="'.$_REQUEST['limit'].'" />';
	  echo $variants[$_REQUEST['variant']]['name'].' ';
	  echo '<input type="hidden" name="variant" value="'.$_REQUEST['variant'].'" />';
	  echo $_REQUEST['size'].' players';
	  echo '<input type="hidden" name="size" value="'.$_REQUEST['size'].'" />';
	  if(isset($_REQUEST['turnament']))
	    {
	      echo ' turnament';
	      echo '<input type="hidden" name="turnament" value="1" />';
	    }
	  echo ':</h1>';
	  echo '<p style="padding-left:'.zoom(40).'px">';

	  if(isset($_REQUEST['turnament']))
	    {
	      echo '<h2>buy-in:</h2>';
	      echo '<p style="padding-left:'.zoom(40).'px">';
	      echo '<select name="buyin">';
	      for($i=2;$i<4;$i++)
		{
		  echo '<option value="'.(1*pow(10,$i)).'">'.(1*pow(10,$i)).' DM</option>';
		  echo '<option value="'.(2*pow(10,$i)).'">'.(2*pow(10,$i)).' DM</option>';
		  echo '<option value="'.(5*pow(10,$i)).'">'.(5*pow(10,$i)).' DM</option>';
		}
	      echo '<option value="'.pow(10,$i).'">'.pow(10,$i).' DM</option>';
	      echo '</select>';
	      echo '</p>';
	      echo '<h2>small blind:</h2>';
	      echo '<p style="padding-left:'.zoom(40).'px">';
	      echo '<h3>start at</h3><p style="padding-left:'.zoom(40).'px"><input size="3" name="sbstart" value="10" /> DM</p>';
	      echo '<h3>increment small blind</h3>';
	      echo '<p style="padding-left:'.zoom(40).'px">';
	      echo makeselect('sbinc',$sbinc_select,$sbinc);
	      echo '</p>';
	      echo '</p>';
	      echo '<h2>payouts:</h2>';
// now
// 8,4,2,1
// 2 100%
// 3  67%, 33%
// 7  57%, 29%, 14%
// 14 53%, 27%, 13%, 7%

	      echo '<p style="padding-left:'.zoom(40).'px">';
	      echo '<select name="payout">';
	      echo '<option value="1">winner takes it all</option>';
	      if($_REQUEST['size']>=3)
		echo '<option value="2">1st (67%) and 2nd (33%)</option>';
	      if($_REQUEST['size']>=7)
		echo '<option value="3">1st (57%), 2nd (29%) and 3rd (14%)</option>';
	      if($_REQUEST['size']>=14)
		echo '<option value="4">1st (53%), 2nd (27%), 3rd (13%) and 4th (7%)</option>';
	      echo '</select>';
	      echo '</p>';
	    }
	  else if(0)
	    {
	      echo '<h2>buy-in:</h2> ';
	      echo '<p style="padding-left:'.zoom(40).'px">';
	      echo '<select name="buyinmin">';
	      for($i=2;$i<4;$i++)
		{
		  echo '<option value="'.(1*pow(10,$i)).'">min '.(1*pow(10,$i)).'</option>';
		  echo '<option value="'.(2*pow(10,$i)).'">min '.(2*pow(10,$i)).'</option>';
		  echo '<option value="'.(5*pow(10,$i)).'">min '.(5*pow(10,$i)).'</option>';
		}
	      echo '</select> ';
	      echo '<select name="buyinmax">';
	      for($i=2;$i<4;$i++)
		{
		  echo '<option value="'.(2*pow(10,$i)).'">max '.(2*pow(10,$i)).' DM</option>';
		  echo '<option value="'.(5*pow(10,$i)).'">max '.(5*pow(10,$i)).' DM</option>';
		  echo '<option value="'.(10*pow(10,$i)).'">max '.(10*pow(10,$i)).' DM</option>';
		}
	      echo '</select> ';
	      echo '</p>';

	      echo '<h2>blinds:</h2>';
	      echo '<p style="padding-left:'.zoom(40).'px">';
	      echo '<select name="sb">';
	      
	      for($i=0;$i<2;$i++)
		{
		  for($j=pow(10,$i);$j<pow(10,$i+1);$j+=pow(10,$i))
		    {
		      echo '<option value="'.$j.'">'.$j.' / '.(2*$j).' DM</option>';
		    }
		}
	      echo '</select> ';
	      echo '</p>';
	      
	    }
	  else echo 'error: not done yet';
	  echo '</p>';
	  echo '<div>';
	  echo '<input type="hidden" name="table" value="create" />';
	  echo '<input type="submit" value="continue" />';
	  echo '</div>';
	}
      else
	{
	  $_REQUEST['table']=
	    query('INSERT INTO `'.$MySQLprefix.'table`',
		  'SET `timecreate`='.time(),
		  ', `size`='.$size,
		  ', `limit`="'.$limit.'"',
		  ', `variant`="'.$variant.'"',
		  ', `payout`='.$payout,
		  ', `dealer`=0',
		  ', `onturn`=0',
		  ', `handcount`=0',
		  ', `buyin`='.$buyin,
		  ', `sb`='.$sbstart,
		  ', `sbstart`='.$sbstart,
		  ', `sbinc`="'.$sbinc.'"');
	  
	  $table=l(query('SELECT * FROM `'.$MySQLprefix.'table`',
			 'WHERE `id`='.intval($_REQUEST['table'])));
	  
	}
    }
  else if(isset($_REQUEST['table'])
	  and $table=l(query('SELECT * FROM `'.$MySQLprefix.'table`',
			     'WHERE `id`='.intval($_REQUEST['table'])))
	  )
    {
      $buyin=l(query('SELECT `buyin` FROM `'.$MySQLprefix.'table`',
		     'WHERE `id`='.intval($_REQUEST['table'])),'<,buyin');
      if($buyin>$player['money'] 
	 or count(query('SELECT `id` FROM `'.$MySQLprefix.'player`',
			'WHERE `table`='.$table['id']))
	 >$table['size']-1)
	{
	  if($buyin>$player['money'])
	    {
	      echo 'You can\'t join a table with a bigger buyin than ';
	      echo $player['money'].' DM<br />';
	    }
	  else
	    echo 'All Seats are allready taken.<br />';
	  echo '<input type="submit" value="back" />';
	  logspeed();die($htmlend);
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

      if(!isset($_REQUEST['table']) or $_REQUEST['table']!=='create')
	{
	  //table list
	  $wait=query('SELECT MAX(`timein`) AS maxti, MAX(`timeout`) AS maxto',
		      'FROM `'.$MySQLprefix.'player`');
	  $wait=max(l($wait,'<,maxto'),l($wait,'<,maxti'));
	  
	  makeAjax($wait,'tables');
	  
	  echo tablelist();
	  
	  echo '<input type="hidden" name="table" value="create" />';
	  echo '<input type="submit" value="create new table" />   ';
	}
    }
  else if(count($playersseated)<$table['size'])
    {
      if(isset($_REQUEST['table']) and $_REQUEST['table']==='leave')
	{
	  if(count(query('SELECT `id`',
			 'FROM `'.$MySQLprefix.'player`',
			 '`WHERE `table`='.$table['id']))==1)
	    {
	      query('DELETE FROM `'.$MySQLprefix.'table`',
		    'WHERE `id`='.$table['id']);
	    }
	  query('UPDATE `'.$MySQLprefix.'player`',
		'SET `table`=0',
		', `money`=`money`+`tablemoney`',
		', `tablemoney`=0',
		', `timeout`='.time(),
		'WHERE `id`='.$player['id']);
	}
      makeAjax(max(l($players,'timein')),'players&tableid='.$table['id']);

      echo 'Table '.str_pad($table['id'],6,'0',STR_PAD_LEFT).'<br /><br />';
      echo nl2br(htmlspecialchars(implode("\n",l($players,'*,name'))));
      echo '<br /><br />waiting for '.($table['size']-count($players)).' more Player(s).';
      echo '<br /><br /><input type="submit" name="table" value="leave" />';
    }
  else if(count($playersseated)>1)
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
      if($minbet<roundnice($table['sb'])*2){$minbet=roundnice($table['sb'])*2;}
	  
	  
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

      $playerHTML=playerHTML();

      echo '<table width="100%"><tr>';
      $c=0;
      $pid=nextplayer($player['id']);
      while ($pid!=$player['id'])
	{
	  echo '<td width="25%" style="'.$font2.'">'.$playerHTML[$pid].'</td>';
	  $pid=nextplayer($pid);
	  $c++;
	  if($c>=4)
	    {
	      echo '</tr></table><br />';
	      echo '<table width="100%"><tr>';
	      $c=0;
	    }
	}
      echo '<td width="25%" style="'.$font3.'">';
      echo nl2br(htmlspecialchars(implode("\n",l($playerswatching,'*,name'))));
      echo '</td>';

      for($c++;$c<4;$c++) echo '<td></td>';
      echo '</tr></table><br />';
	  
      echo '<table><tr>';
	  
      echo '<td><span style="'.$font2.'"> ';
      if($table['bettinground']==0 and $table['nextbettinground'])
	echo '<span style="background-color:#FFFF00">flop</span>';
      else
	echo 'flop';
      echo '<br />';
      if($table['bettinground']>0) 
	{
	  echo cardimgs(substr($ccards,0,6),70);
	}
      else echo cardimgs('xxxxxx',70);
      echo ' </span></td>';
	  
      echo '<td><span style="'.$font2.'"> ';
      if($table['bettinground']==1 and $table['nextbettinground'])
	echo '<span style="background-color:#FFFF00">turn</span>';
      else
	echo 'turn';
      echo '<br />';
      if($table['bettinground']>1) 
	echo cardimgs(substr($ccards,6,2),70);
      else 
	echo cardimgs('xx',70);
      echo ' </span></td>';
	  
      echo '<td><span style="'.$font2.'"> ';
      if($table['bettinground']==2 and $table['nextbettinground'])
	echo '<span style="background-color:#FFFF00">river</span>';
      else
	echo 'river';
      echo '<br />';
      if($table['bettinground']>2) echo cardimgs(substr($ccards,8,2),70);
      else echo cardimgs('xx',70);
      echo ' </span></td>';
	  
      echo '<td valign="bottom" align="right"><span style="'.$font2.'">';
      echo moneyimgs(intval($table['pot']));
      echo ' </span></td>';
      echo '<td width="'.zoom(25).'"></td>';
      echo '</tr></table><br /><br />';
	  
      echo $playerHTML[$player['id']];

      getGame();
	  
      do{
	$maxbetmoney=max(l($players,'*,betmoney'));
	    
	$brdone=true;
	if(!$table['nextbettinground'])
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
	    if(!$table['nextbettinground'])
	      {
		    
		collectbets();
		    
		if($table['bettinground']>=3) showdown();
		    
		query('UPDATE `'.$MySQLprefix.'table` ',
		      'SET `nextbettinground`='.($table['nextbettinground']=(time()+3)),
		      ', `onturn`=0',
		      'WHERE `id`='.$table['id']);
	      }
	    else if($table['nextbettinground']<time())
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
		      'SET `nextbettinground`=0',
		      ', `onturn`='.(count($playersactive)?nextactive($table['dealer']):'0'),
		      ', `bettinground`=`bettinground`+1',
		      'WHERE `id`='.$table['id']);
		$brdone=false;
		if($table['bettinground']>=3 or $table['deck']=='' or count($playersin)<=1)
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
      } while($brdone && !$table['nextbettinground']);
	  

      if($brdone)
	{
	  makeAjax($table['nextbettinground'],'time&time='.$table['nextbettinground']);
	}
      else
	{
	  $wait=l(query('SELECT MAX(`actiontime`) AS maxat',
			'FROM `'.$MySQLprefix.'player`',
			'WHERE `table`='.$table['id']),'<,maxat');
	      
	  makeAjax($wait,'cards&tableid='.$table['id']);
	}
	  
      $maxbetmoney=max(l($players,'*,betmoney'));
	      
      echo '<table cellpadding="0">';
      echo '<tr>';
	      
      echo '<td width="'.zoom(60).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.'">';
      if(!$brdone and $table['onturn']==$player['id'] 
	 and $player['betmoney']+$player['tablemoney']>$maxbetmoney)
	{
	  if($maxbetmoney)
	    {
	      echo '<span style="'.$font2.'">by </span>';
	      echo '<input style="border:'.zoom(1).'px solid #000;padding:'.zoom(1).'px;vertical-align:bottom;'.$font2.';width:'.zoom(40).'px;height:'.zoom(16).'px;text-align:right"';
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
	      echo ' /><br />';
	      echo '<span style="'.$font2.'">to </span>';
	      echo '<input style="border:'.zoom(1).'px solid #000;vertical-align:bottom;'.$font2.';width:'.zoom(40).'px;height:'.zoom(16).'px;text-align:right"';
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
	      echo '<input style="'.$font1.';width:'.zoom(60).'px;height:'.zoom(32).'px;text-align:right"';
	      echo ' type="text" name="money" value="'.$minbet.'" /><br />';
	    }
	}
      echo '</td>';
	      
      echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.';border:'.zoom(1).'px solid black;background-color:#d0d0d0">';
      if(!$brdone and $table['onturn']==$player['id'] 
	 and $player['betmoney']+$player['tablemoney']>$maxbetmoney)
	{
	  echo '<input style="'.$font1.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:'.zoom(1).'px solid black;background-color:#d0d0d0"';
	  echo ' type="submit" name="action" value="'.($maxbetmoney?'raise':'bet').'" />';
	}
      else echo $maxbetmoney?'raise':'bet';
      echo '</td>';
	      
      echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.';border:'.zoom(1).'px solid black;background-color:#d0d0d0">';
      if($brdone or $player['betmoney']>=$maxbetmoney) echo 'call';
      else if($table['onturn']!=$player['id'])
	{
	  echo '<input type="radio" name="preaction" value="call" id="pacall"';
	  if($_REQUEST['preaction']==='call')
	    echo ' checked="checked"';
	  echo ' /><label style="'.$font1.'" for="pacall"> call ';
	  if($player['betmoney']+$player['tablemoney']<=$maxbetmoney)
	    echo '(allin)';
	  else echo $maxbetmoney.' DM';
	  echo '<input type="hidden" name="pacallmoney" value="'.$maxbetmoney.'" />';
	  echo '</label>';
	}
      else
	{
	  echo '<input style="'.$font1.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:'.zoom(1).'px solid black;background-color:#d0d0d0"';
	  echo ' type="submit" name="action" value="call ';
	  if($player['betmoney']+$player['tablemoney']<=$maxbetmoney)
	    echo '(allin)';
	  else echo $maxbetmoney.' DM';
	  echo '" />';
	}
      echo '</td>';
	      
      echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.';border:'.zoom(1).'px solid black;background-color:#d0d0d0">';
      if($brdone or $player['betmoney']<$maxbetmoney) echo 'check';
      else if($table['onturn']!=$player['id'])
	{
	  echo '<input type="radio" name="preaction" value="check" id="pacheck"';
	  if($_REQUEST['preaction']==='check')
	    echo ' checked="checked"';
	  echo ' /><label style="'.$font1.'" for="pacheck"> check</label>';
	}
      else
	echo '<input style="'.$font1.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:'.zoom(1).'px solid black;background-color:#d0d0d0" type="submit" name="action" value="check" />';
      echo '</td>';
	      
      echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.';border:'.zoom(1).'px solid black;background-color:#d0d0d0">';
      if(!$brdone and $table['onturn']!=$player['id'])
	{
	  echo '<input type="radio" name="preaction" value="fold" id="pafold"';
	  if($_REQUEST['preaction']==='fold')
	    echo ' checked="checked"';
	  echo ' /><label style="'.$font1.'" for="pafold"> fold</label>';
	}
      else
	{
	  echo '<input style="'.$font1.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:'.zoom(1).'px solid black;background-color:#d0d0d0"';
	  echo ' type="submit" name="action" value="fold" />';
	}
      echo '</td>';
	      
      if(!$brdone and $table['onturn']!=$player['id'])
	{
	  echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.';border:'.zoom(1).'px solid black;background-color:#d0d0d0">';
	  echo '<input type="radio" name="preaction" value="none" id="panone"';
	  if($_REQUEST['preaction']==='none')
	    echo ' checked="checked"';
	  echo ' /><label style="'.$font1.'" for="panone"> No Action</label>';
	}
      else
	echo '<td width="'.zoom(120).'" height="'.zoom(32).'">&nbsp;';
      echo '</td>';
	      
      echo '<td width="'.zoom(120).'" height="'.zoom(32).'" valign="middle" align="center" style="'.$font1g.';border:'.zoom(1).'px solid black;background-color:#d0d0d0">';
      if(!$player['showcards'])
	echo '<input style="'.$font1.';width:'.zoom(120).'px;height:'.zoom(32).'px;border:'.zoom(1).'px solid black;background-color:#d0d0d0" type="submit" name="showcards" value="show cards" />';
      else echo 'show cards';
      echo '</td>';
	      
      echo '</tr>';
      echo '</table>';
	      
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
  else { logspeed();die('fatal error: wrong table'); }
}
function handname($hand)
{
  global $numsHTML,$numsHTMLp;
  $kind=$hand[0].$hand[1];

  switch($kind)
    {
    case 'rf': return 'Royal Flush';
    case 'sf': 
      if(strlen($hand)>2) 
	return 'Strait Flush '.$numsHTML[$hand[2]-4].' to '.$numsHTML[$hand[2]];
      return 'Strait Flush';
    case '4k':
      if(strlen($hand)>3) 
	return '4 of a kind '.$numsHTMLp[$hand[2]].' (kicker: '.$numsHTML[$hand[3]].')';
      if(strlen($hand)>2) 
	return '4 of a kind '.$numsHTMLp[$hand[2]].'';
      return '4 of a kind';
    case 'fh':
      if(strlen($hand)>3)
	return 'Full House '.$numsHTMLp[$hand[2]].' and '.$numsHTMLp[$hand[3]];
      return 'Full House';
    case 'fl':
      if(strlen($hand)>2)
	{
	  $r='Flush (including: '.$numsHTML[$hand[2]];
	  for($i=3;$i<strlen($hand);$i++)
	    $r.=', '.$numsHTML[$hand[$i]];
	  return $r.')';
	}
      return 'Flush';
    case 'st':
      if(strlen($hand)>2) 
	return 'Strait '.$numsHTML[$hand[2]-4].' to '.$numsHTML[$hand[2]];
      return 'Strait';
    case '2p':
      if(strlen($hand)>4) 
	return 'two pairs '.$numsHTMLp[$hand[2]].' and '.$numsHTMLp[$hand[3]].' (kicker: '.$numsHTML[$hand[4]].')';
      if(strlen($hand)>3)
	return 'two pairs '.$numsHTMLp[$hand[2]].' and '.$numsHTMLp[$hand[3]];
      return 'two pairs';
    case '2k':
      if(strlen($hand)>3)
	{
	  $r='one pair '.$numsHTMLp[$hand[2]].' (kicker: '.$numsHTML[$hand[3]];
	  for($i=4;$i<strlen($hand);$i++)
	    $r.=', '.$numsHTML[$hand[$i]];
	  return $r.')';
	}
      if(strlen($hand)>2) 
	return 'one pair '.$numsHTMLp[$hand[2]].'';
      return 'one pair';
    case 'hc':
      if(strlen($hand)>3)
	{
	  $r='High Card '.$numsHTML[$hand[2]].' (kicker: '.$numsHTML[$hand[3]];
	  for($i=4;$i<strlen($hand);$i++)
	    $r.=', '.$numsHTML[$hand[$i]];
	  return $r.')';
	}
      if(strlen($hand)>2) 
	return 'High Card '.$numsHTML[$hand[2]].'';
      return 'High Card';
    default: return '';
    }
}

function comphands($hand1,$hand2)
{
  $kind1=$hand1[0].$hand1[1];
  $kind2=$hand2[0].$hand2[1];
  if($kind1==$kind2)
    {
      for($i=2;$i<strlen($hand1) and $i<strlen($hand2);$i++)
	{
	  if($nums3[$hand1[$i]]>$nums3[$hand2[$i]]) return 1;
	  if($nums3[$hand1[$i]]<$nums3[$hand2[$i]]) return -1;
	}
    }
  else
    {
      if($kind1=='rf')return 1;
      if($kind2=='rf')return -1;
      if($kind1=='sf')return 1;
      if($kind2=='sf')return -1;
      if($kind1=='4k')return 1;
      if($kind2=='4k')return -1;
      if($kind1=='fh')return 1;
      if($kind2=='fh')return -1;
      if($kind1=='fl')return 1;
      if($kind2=='fl')return -1;
      if($kind1=='st')return 1;
      if($kind2=='st')return -1;
      if($kind1=='2p')return 1;
      if($kind2=='2p')return -1;
      if($kind1=='2k')return 1;
      if($kind2=='2k')return -1;
    }
  return 0;
}


// $hand is a string with up to 9 cards / 18 chars
// format:  "4das1hjh2d ... "
// returns a hands value
// format: "hcakt76" = high card Ace, kickers: King, 10, 7, 6   
//         "sfj" = straight flush Jack, 10, 9, 8, 7
//         ...   
function cards2hand($hand)
{
  global $nums,$nums2,$nums3,$cols,$cols2;

  /* what is it */
  $flush='x';
  if(substr_count($hand,'s')>=5)$flush='s';
  if(substr_count($hand,'d')>=5)$flush='d';
  if(substr_count($hand,'h')>=5)$flush='h';
  if(substr_count($hand,'c')>=5)$flush='c';
  
  /* rf */
  if(substr_count($hand,'a'.$flush)
     and 
     substr_count($hand,'k'.$flush)
     and 
     substr_count($hand,'q'.$flush)
     and 
     substr_count($hand,'j'.$flush)
     and 
     substr_count($hand,'t'.$flush))
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
    for($j=0;$j<strlen($hand)/2;$j++)
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
    for($j=0;$j<strlen($hand)/2;$j++)
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
    for($j=0;$j<strlen($hand)/2;$j++)
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
        for($k=0;$k<strlen($hand)/2;$k++)
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
    for($j=0;$j<strlen($hand)/2;$j++)
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
  for($j=0;$j<strlen($hand)/2;$j++)
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

  $n=$cards[0];
  if($n=='t')$n=10;
  $text=strtoupper($n);
  $c=$cards[1];
  $col='#000000';

  $r= $cards.'<svg height="193" id="svg2" version="1.0" width="143" x="0" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" y="0">';
  $r.= '<symbol>';
  switch($c)
    { 
    case 'c': 
      $r.= '<path d="M -1,1 L -4,12 L 4,12 L 1,1 C 3,1 7,7 9,7 C 15,7 15,-7 9,-7 C 7,-7 3,-1 1,-1 C 1,-4 5,-10 5,-13 C 5,-23 -5,-23 -5,-13 C -5,-10 -1,-4 -1,-1 C -3,-1 -7,-7 -9,-7 C -15,-7 -15,7 -9,7 C -7,7 -3,1 -1,1 z" id="symbolc" style="fill:#000000"/>';
      break;
    case 'h': 
      $r.= '<path d="M 0,-10 C 0,-25 14,-25 14,-10 C 14,-5 5,7 0,15 C -5,7 -14,-5 -14,-10 C -14,-25 0,-25 0,-10 z" id="symbolh" style="fill:#FF0000"/>';
      $col='#FF0000';
      break;
    case 'd': 
      $r.= '<path d="M 0,20 C -1,15 -10,0 -14,0 C -10,0 -1,-15 0,-20 C 1,-15 10,0 14,0 C 10,0 1,15 0,20 z" id="symbold" style="fill:#FF0000"/>';
      $col='#FF0000';
      break;
    case 's': 
      $r.= '<path d="M -1,1 L -4,12 L 4,12 L 1,-1 C 1,5 14,10 14,0 C 14,-5 5,-17 0,-25 C -5,-17 -14,-5 -14,0 C -14,10 -1,5 -1,0 z" id="symbols" style="fill:#000000"/>';
      break;
    }

  $r.= '<text x="0" y="0" id="num'.$text.'" text-anchor="middle" font-family="sans-serif" style="font-size:40px;font-weight:bold;fill:'.$col.';stroke:none">'.$text.'</text>';
  $r.= '</symbol>';
  
  $r.= '<rect x="5" y="5" width="138" height="188" rx="16" ry="16" style="fill:#000000;fill-opacity:0.5"/>';
  $r.= '<rect x="0" y="0" width="138" height="188" rx="16" ry="16" style="fill:#cccccc"/>';
  $r.= '<rect x="2" y="2" width="138" height="188" rx="16" ry="16" style="fill:#cccccc"/>';
  if($cards=='xx') $r.= '<rect x="1" y="1" width="138" height="188" rx="16" ry="16" style="fill:#000099"/>';
  else 
    {
      $r.= '<rect x="1" y="1" width="138" height="188" rx="16" ry="16" style="fill:#ffffff"/>';
  
      $r.= '<use x="28" y="40" xlink:href="#num'.$text.'" transform="scale(.75,1)" />';
      $r.= '<use x="28" y="40" xlink:href="#num'.$text.'" transform="rotate(180,70,95) scale(.75,1)" />';
  
      $r.= '<use x="20" y="70" xlink:href="#symbol'.$c.'" />';
      $r.= '<use x="20" y="70" xlink:href="#symbol'.$c.'" transform="rotate(180,70,95)" />';
  

      if($n>=8) $r.= '<use x="100" y="100" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=8) $r.= '<use x="180" y="100" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=4 && $n<=7) $r.= '<use x="100" y="110" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==3) $r.= '<use x="140" y="110" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=4 && $n<=7) $r.= '<use x="180" y="110" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==10|| $n==2) $r.= '<use x="140" y="125" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==7) $r.= '<use x="140" y="140" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=8) $r.= '<use x="100" y="170" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=8) $r.= '<use x="180" y="170" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==6) $r.= '<use x="100" y="190" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';

      if($n %2) $r.= '<use x="140" y="190" transform="scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n=='j'||$n=='q'||$n=='k') $r.= '<use x="47" y="60" xlink:href="#num'.$text.'" transform="scale(1.5,2)" />';
      if($n=='a') $r.= '<use x="35" y="49" transform="scale(2,2)" xlink:href="#symbol'.$c.'" />';

      if($n>=8) $r.= '<use x="100" y="100" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=8) $r.= '<use x="180" y="100" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=4 && $n<=7) $r.= '<use x="100" y="110" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==3) $r.= '<use x="140" y="110" transform="rotate(180,70,95) scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=4 && $n<=7) $r.= '<use x="180" y="110" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==10|| $n==2) $r.= '<use x="140" y="125" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==7) $r.= '<use x="140" y="140" transform="rotate(180,70,95) scale(0.5,0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=8) $r.= '<use x="100" y="170" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n>=8) $r.= '<use x="180" y="170" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
      if($n==6) $r.= '<use x="100" y="190" transform="rotate(180,70,95) scale(0.5)" xlink:href="#symbol'.$c.'" />';
    }
  $r.= '</svg>';
  return $r;
}
function moneyimgs($money)
{
  $chips=array(500=>"purple",100=>"black",25=>"green",10=>"blue",5=>"red",1=>"white");

  $tm=$money;
  $mychips=array();
  $r='<table cellpadding="'.zoom(2).'" cellspacing="0"><tr>';
  foreach($chips as $k => $v)
    {
      if($tm>=$k)
	{
	  $r.='<td valign="bottom">';
	  $r.='<table bgcolor="#888888" cellpadding="0" cellspacing="'.zoom(1).'">';
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
  return comphands(cards2hand($ccards.$pcards['down'][$p1['id']].$pcards['up'][$p1['id']]),cards2hand($ccards.$pcards['down'][$p2['id']].$pcards['up'][$p2['id']]));
}

function getGame($onlycheck=false)
{
  global $table,$players,$player,$playersin,$playersseated,$playersactive,$ccards,$pcards,$playerswatching;
  if(!$table=l(query('SELECT * FROM `'.$GLOBALS['MySQLprefix'].'table`',
		     'WHERE `id`='.intval($player['table'])))) return false;
  if($onlycheck) return true;
  $players=query('SELECT * FROM `'.$GLOBALS['MySQLprefix'].'player` WHERE `table`='.$table['id'].' ORDER BY `seat`');
  
  // the players represented by 5 arrays
  // players = all players tha are there (including guests)
  // playersseated = Players that have taken a Seat (after being out they have to stand up)
  // playerswatching = Players that not have taken a Seat or been out
  // playersin = Players that still in the pot (after flodding they arn't anymore)
  // playersactive = Players that may still perform an action (after going all-in they can't)

  $playersseated = array();
  $playerswatching = array();
  $playersin = array();
  $playersactive = array();

  foreach($players as $k => $v)
      {
	if(playerisseated($k))$playersseated[$k]=&$players[$k];
	else $playerswatching[$k]=&$players[$k];
      }
  foreach($players as $k => $v)if(playerisin($k))$playersin[$k]=&$players[$k];
  foreach($players as $k => $v)if(playerisactive($k))$playersactive[$k]=&$players[$k];

  $ccards=substr($table['deck'],0,10);
  $pcards=array('up'=>array(),'down'=>array());
  $i=10;
  foreach($players as $pid => $p)
      {
	$pcards['down'][$pid]=substr($table['deck'],$i,4);
	$pcards['up'][$pid]='';
	$i+=4;
      }
  return true;
}

function setGame()
{
  global $table,$player;//,$players
  $query1='UPDATE `'.$GLOBALS['MySQLprefix'].'table` SET';
  foreach($table as $name => $value)
      {
	$query1.=' `'.$name.'`="'.mes($value).'",';
      }
  $query1=substr($query,0,strlen($query1)-1).' WHERE `id`='.intval($player['table']);
  $query2='UPDATE `'.$GLOBALS['MySQLprefix'].'player` SET';
  foreach($player as $name => $value)
      {
	$query2.=' `'.$name.'`="'.mes($value).'",';
      }
  $query2=substr($query,0,strlen($query2)-1).' WHERE `id`='.intval($player['id']);
  
  /*
  $return=query($query1);
  foreach($players as $p)
      {
	$query2='UPDATE `'.$GLOBALS['MySQLprefix'].'player` SET';
	foreach($p as $name => $value)
	    {
	      $query2.=' `'.$name.'`="'.mes($value).'",';
	    }
	$query2=substr($query,0,strlen($query2)-1).' WHERE `id`='.intval($p['id']);
	$return=($return and query($query2));
      }
  return $return;
  */
  return (query($query1) and query($query2));
}

// players = all players that are there (including guests)
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

// playersin = Players that still in the pot (after flodding they arn't anymore)
function playerisin($pid)
{
  global $players;
  return (
    $players[$pid]['tablestatus']=="in" 
    and ($players[$pid]['tablemoney']+$players[$pid]['betmoney']+$players[$pid]['allinpot'])
    and $players[$pid]['action']!="fold"
    );
}
// playersseated = Players that have taken a Seat (after been out they have to stand up)
// playerswatching = Players that not have taken a Seat or been out
function playerisseated($pid)
{
  global $players;
  return (
    $players[$pid]['tablestatus']=="in" 
    and ($players[$pid]['tablemoney']+$players[$pid]['betmoney']+$players[$pid]['allinpot'])
    );
}
// playersactive = Players that may still perform an action (after going all-in thay can't)
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
function nextwatching($pid)
{
  do{ $pid=nextplayer($pid); }while(playerisseated($pid));
  return $pid;
}
function nextactive($pid)
{
  do{ $pid=nextplayer($pid); }while(!playerisactive($pid));
  return $pid;
}

// caching query() errors
// query() will performe the die()
function queryNoConnect()
{
  $r.= 'Could not connect to The MySQL database<br />';
  $r.= 'MySQL error: '.mysql_error().'<br /><br />';
  $r.= 'Please make sure that ur QpokerMySQL.php contains the right data.<br /><br />';
  $r.= 'example QpokerMySQL.php:<br />';
  $r.= '&lt;?php<br />';
  $r.= '  $MySQLuser="user";<br />';
  $r.= '  $MySQLpass="pass";<br />';
  $r.= '  $MySQLhost="localhost";<br />';
  $r.= '  $MySQLdb="databasename";<br />';
  $r.= '  $MySQLprefix="qpoker_";<br />';
  $r.= '?&gt;<br />';
  return $r;
}

// caching query() errors
// query() will performe the die()
function queryNoDB()
{
  return queryNoConnect();
}


// performs a mysql_query
// on first call it connects to the db
// returns info dependent on the statement:
// insert: autoincrement id
// update: affected rows (thats how it may be used inside a if statement)
// select: 2 dimentional array with all rows
//         if there is a field called id, it will be used as first array-key.
//                          (the order defined by 'ORDER BY' is NOT affected)
//         second-level array-key is the fieldname (mysql_fetch_array)
//         if no rows have been selected it returns an empty array 
//                 (converts to bool false inside an if statement)
// else: mysql_info()
// if there was a MySQL error it will always return bool(false)
function query()
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
// an example:
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
// this funktion is extremly useful to procces db results that are stored in an array s.a. query()
*/
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
      foreach($a as &$v)
	$v=l($v,$how);
      return $a;
    }
  return l($a[$do],$how);
}

// shortcut for mysql_escape_string()
// i know i'm laisy
// maybe it will have real usage when sombody trys to make a non MySQL port
function mes($s)
{
  return mysql_escape_string($s);
}

// uses the $player['zoom']
// may also take a float as parameter
// always returns an int >=1
function zoom($i)
{
  return max(1,intval($i*(isset($GLOBALS['player']['zoom'])?$GLOBALS['player']['zoom']/100:1)));
}

// returns a random string with a length of 104
// the format is: "4dasthjh2d ... "
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

// creates that little R at the right. checks for changes and submits
// theform if there are any or on clicking the R
function makeAjax($wait,$post,$speed=NULL)
{
  if($speed===NULL)$speed=1000;
  $wait=intval($wait);
//echo '<a href="'.htmlspecialchars($GLOBALS['url'].'changed='.$post).'">&quot;'.$wait.'&quot;</a>';
  echo '<input style="';
  echo   $GLOBALS['font2g'].';background:none;border:none;width:'.zoom(24).'px;height:'.zoom(20).'px;';
  echo   'position:absolute;z-index:99;right:0px;top:0px';
  echo '" type="submit" value="R" title="reload frame manualy" id="reloadbutton" />';
  echo '<script language="JavaScript">';
  
  echo htmlspecialchars(<<<JS
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
	    var zeit = new Date();
	    var nextTO = zeit.getMilliseconds()%$speed;
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
	    ajaxReq.open("post","$GLOBALS[url].'",true);
	    ajaxReq.setRequestHeader("Content-Type", 
				     "application/x-www-form-urlencoded");
	    ajaxReq.send("changed=$post");
	  }
	else 
	  setTimeout('document.getElementById("theform").submit()',${speed}0);
      }
  setTimeout('makeReq()',$speed);
JS
);
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
	  `timecreate` int(11) NOT NULL,
	  `timestart` int(11) NOT NULL,
	  `timeclosed` int(11) NOT NULL,
	  `deck` VARCHAR (104) NOT NULL,
	  `dealer` int(11) NOT NULL,
	  `buyin` int(11) NOT NULL,
	  `sbstart` int(11) NOT NULL,
	  `sbinc` enum('none','slow','norm','fast') NOT NULL default 'norm',
	  `sb` int(11) NOT NULL,
	  `pot` int(11) NOT NULL,
	  `onturn` int(11) NOT NULL,
	  `bettinground` int(3) NOT NULL,
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
  if($players[$pid]['tablemoney']<=roundnice($table['sb']))
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
	    'SET `tablemoney`=`tablemoney`-'.roundnice($table['sb']),
	    ', `betmoney`='.roundnice($table['sb']),
	    ', `action`="sb"',
	    ', `actiontime`='.time(),
	    'WHERE `id`='.$pid);
    }
  $pid=nextactive($pid);
  if($players[$pid]['tablemoney']<=roundnice($table['sb'])*2)
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
	    'SET `tablemoney`=`tablemoney`-'.(roundnice($table['sb'])*2),
	    ', `betmoney`='.(roundnice($table['sb'])*2),
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
      $thishand=cards2hand($ccards.$pcards['down'][$pid].$pcards['up'][$pid]);
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
  if(!$table['handcount']%$table['size'] and !$firsthand)
    {
      if($table['inc']=='slow')
	$table['sb']=round($table['sb']+$table['sbstart']*0.5);
      if($table['inc']=='norm')
	$table['sb']=round($table['sb']*1.5);
      if($table['inc']=='fast')
	$table['sb']=round(exp($table['sb'],1.5));
    }
  getGame();
  
  mt_srand();
  $table['dealer']=nextseated($table['dealer']);
  query('UPDATE `'.$GLOBALS['MySQLprefix'].'table`',
	'SET `dealer`='.$table['dealer'],
	', `deck`="'.randdeck().'"',
	', `bettinground`=0',
	', `onturn`='.nextactive($table['dealer']),
	', `sb`='.$table['sb'],
	'WHERE `id`='.$table['id']);
  
  getGame();
}

// just for debuging. it always returns the given value.
// e.g. if u wanna see a malefunctioning SQL statement just as it inside query():
// query(echo_r("SELECT .... "));
function echo_r($var)
{
  if(func_num_args()>1)
    {
      $var=func_get_args();
      $var=implode(' ',$var);
    }
  if(is_string($var))echo nl2br("\n\"".mes($var)."\"\n");
  else echo nl2br("\n".rprint_r($var)."\n");
  return $var;
}

// used like rand(), no srand() needed.
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

function playerHTML()
{
  global $player, $players, $pcards, $table;
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
	
	$html[$pid]=' ';
	if($table['onturn']==$pid) 
	  $html[$pid].='<span style="background-color:#FFFF00">';
	$html[$pid].=$p['name'];
	if($pid==$table['dealer']) $html[$pid].=' (D)';
	$html[$pid].=' ['.intval($p['tablemoney']).' DM]';
	if($table['onturn']==$pid) 
	  $html[$pid].='</span>';
	$html[$pid].= '<table><tr><td><pre>';
	if($p['showcards'])
	  {
	    $html[$pid].=cardimgs($pcards['down'][$pid].$pcards['up'][$pid],($pid==$player['id'])?100:70);
	  }
	else
	  {
	    if($pid==$player['id'])// and in_array($pid,$seated))
	      {
		$html[$pid].=cardimgs($pcards['down'][$pid].$pcards['up'][$pid],100);
	      }
	    else
	      {
		$html[$pid].= cardimgs(str_repeat('x',strlen($pcards['down'][$pid]))
					.$pcards['up'][$pid]
					,($pid==$player['id'])?100:70);
	      }
	  }
	$html[$pid].=' </pre></td>';
	$html[$pid].='<td valign="bottom" align="right" style="'.$GLOBALS['font2'].'">';
	$html[$pid].=moneyimgs(intval($p['betmoney']));
	$html[$pid].='</td>';
	$html[$pid].='<td width="'.zoom(25).'"></td></tr></table>';
	$html[$pid].=' '.$thishand;
	
      } 
  return $html;
}

function tablelist()
{
  $notable=true;
  $tables=query('SELECT * FROM `'.$GLOBALS['MySQLprefix'].'table`');// WHERE `timeclosed`=0');
  $style='border:1px solid #000;'.$GLOBALS['font1'];
  foreach($tables as $table)
      {
	$p=
	  query('SELECT *',
		'FROM `'.$GLOBALS['MySQLprefix'].'player`',
		'WHERE `table`='.intval($table['id']),
		'ORDER BY `seat`');
	if(1 or count($p)<$table['size'])
	  {
	    if($notable)
	      {
		$r= 'Select a Table:';
		$r.= '</div></form>';
		$notable=false;
	      }
	    $r.= '<form method="post" action="'.$GLOBALS['url'].'">';
	    $r.= '<div style="'.$GLOBALS['font1'].'">';
	    $r.= '<input type="hidden" name="frame" value="main" />';
	    $r.= '<input type="hidden" name="table" value="'.$table['id'].'" />';
	    $r.= '<input style="'.$style.'" type="submit" name="tableid" value="';
	    $r.= str_pad($table['id'],6,'0',STR_PAD_LEFT).'" />';
	    $r.= '<input style="'.$style.'" type="submit" name="tablebuyin" value="';
	    $r.= str_pad($table['buyin'].' DM',9,' ',STR_PAD_LEFT).'" />';
	    $r.= '<input style="'.$style.'" type="submit" name="tablesb" value="';
	    $r.= str_pad(
	      $table['sbstart'].' '.$table['sbinc'].' DM',12,' ',STR_PAD_LEFT).'" />';
	    $r.= '<input style="'.$style.'" type="submit" name="tableplayers" value="';
	    $r.= str_pad(
	      '   '.count($p).'/'.$table['size']
	      .' ('.htmlspecialchars(implode(', ',l($p,'*,name'))).')',
	      70).'" />';
	    $r.= '</div></form>';
	  }
      }
  if(!$notable)
    {
      $r.= '<div style="'.$GLOBALS['font1'].'">';
      $r.= '(Number, Buy-In, Small-Blind, Players)<br /><br />';
      $r.= '</div>';
      $r.= '<form method="post" action="'.$GLOBALS['url'].'"><div style="'.$GLOBALS['font1'].'">';
      $r.= '<input type="hidden" name="frame" value="main" />';
    }
  return $r;
}

// makes nice to view nums within the range given by $error
// e.g. 314 -> 300, 321 -> 320, 344 -> 350 ...
function roundnice($num,$digits=0,$error=.05,$errorTo5=NULL)
{
  if($errorTo5===NULL)$errorTo5=$error/2;
  $digits=pow(10,$digits);
  $num*=$digits;
  $max=$num*(1+$error);
  $min=$num*(1-$error);
  $maxTo5=$num*(1+$errorTo5);
  $minTo5=$num*(1-$errorTo5);
  $len=1;
  while($num>=pow(10,$len))$len++;

  for($i=$len-1;$i>0;$i--)
    {
      $round=round($num/pow(10,$i)/5)*pow(10,$i)*5;
      if($round<$maxTo5 and $round>$minTo5) return $round/$digits;
      $round=round($num/pow(10,$i))*pow(10,$i);
      if($round<$max and $round>$min) return $round/$digits;
    }
  $round=round($num/pow(10,$i)/5)*pow(10,$i)*5;
  if($round<$maxTo5 and $round>$minTo5) return $round/$digits;
  return round($num)/$digits;
}

function logspeed()
{
  $logspeed=(round(microtime(true)*1000)-$GLOBALS['logspeed'])/1000;
  if(isset($_REQUEST['changed']))
    {/*
      $fp=fopen('QpokerSpeedAjax.log','a')
	and (fwrite($fp,str_pad($_REQUEST['changed'],8)."\t".time()."\t".$logspeed."\n")
	xor fclose($fp));
     */}
  else
    { 
      $fp=fopen('QpokerSpeed.log','a')
	and (fwrite($fp,time()."\t".str_pad($logspeed,5,'0')."\t".$_SERVER['QUERY_STRING']."\n")
	     xor fclose($fp));
    }
}
function makeselect ($name, $array, $default = NULL)
{
  $r = '<select name="'.htmlspecialchars($name).'">';
  foreach($array as $value => $displayed)
      {
	$r.='<option value="'.htmlspecialchars($value).'"';
	if($default == $value) {
	  $r.=' selected="selected"';
	}
	$r.='>'.htmlspecialchars($displayed).'</option>';
      }
  $r .= '</select>';
  return $r;
}

/*
for($i=0;$i<1000;$i++) echo $i."\t".roundnice($i)."\n";
echo round(10000/3)."\n";
echo roundnice(10000/3)."\n";
echo round(985)."\n";
echo roundnice(985)."\n";
echo round(57777)."\n";
echo roundnice(57777)."\n";
echo round(46666)."\n";
echo roundnice(46666)."\n";
echo round(10000/6)."\n";
echo roundnice(10000/6)."\n";
echo round(10000/7)."\n";
echo roundnice(10000/7)."\n";
echo round(10000/9)."\n";
echo roundnice(10000/9)."\n";
echo round(10000/11)."\n";
echo roundnice(10000/11)."\n";
echo round(10000/12)."\n";
echo roundnice(10000/12)."\n";
echo round(10000/13)."\n";
echo roundnice(10000/13)."\n";
echo round(10000/14)."\n";
echo roundnice(10000/14)."\n";
echo round(314156)."\n";
echo roundnice(314156)."\n";
/**/
//setGame();

logspeed();
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
