<?php
  //
  // Copyright (C) 2006, 2007, 2008 Loic Dachary <loic@dachary.org>
  // Copyright (C) 2005, 2006 Mekensleep <licensing@mekensleep.com>
  //                          24 rue vieille du temple,75004 Paris
  //
  // This software's license gives you freedom; you can copy, convey,
  // propagate, redistribute and/or modify this program under the terms of
  // the GNU Affero General Public License (AGPL) as published by the Free
  // Software Foundation (FSF), either version 3 of the License, or (at your
  // option) any later version of the AGPL published by the FSF.
  //
  // This program is distributed in the hope that it will be useful, but
  // WITHOUT ANY WARRANTY; without even the implied warranty of
  // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero
  // General Public License for more details.
  //
  // You should have received a copy of the GNU Affero General Public License
  // along with this program in a file in the toplevel directory called
  // "AGPLv3".  If not, see <http://www.gnu.org/licenses/>.
  //
  // Authors:
  //  Morgan Manach <akshell@free.fr>
  //  Loic Dachary <loic@dachary.org>
  //
require_once 'common.php';
require_once 'birthday.php';

$birthday = new birthday();

if(_post_string('submit')) {
  $account = array('name' => _post_string('name'),
                   'password' => _post_string('password'),
                   'password2' => _post_string('password2'),
                   'email' => _post_string('email'),
                   'firstname' => _post_string('firstname'),
                   'lastname' => _post_string('lastname'),
                   'addr_street' => _post_string('addr_street'),
                   'addr_street2' => _post_string('addr_street2'),
                   'addr_zip' => _post_string('addr_zip'),
                   'addr_town' => _post_string('addr_town'),
                   'addr_state' => _post_string('addr_state'),
                   'addr_country' => _post_string('addr_country'),
                   'phone' => _post_string('phone'),
                   'gender' => _post_string('gender'),
                   'birthdate' => $birthday->as_string()
                   );

  try {
    if($account['password'] != $account['password2'])
      throw new Exception('Password and confirmation must be the same.');

    if($account['email'] == '')
      throw new Exception('Email is mandatory');

    $poker->send(array('type' => 'PacketPokerCreateAccount', 
                       'name' => $account['name'], 
                       'password' => $account['password'],
                       'email' => $account['email'],
                       'firstname' => $account['firstname'],
                       'lastname' => $account['lastname'],
                       'addr_street' => $account['addr_street'],
                       'addr_street2' => $account['addr_street2'],
                       'addr_zip' => $account['addr_zip'],
                       'addr_town' => $account['addr_town'],
                       'addr_state' => $account['addr_state'],
                       'addr_country' => $account['addr_country'],
                       'phone' => $account['phone'],
                       'gender' => $account['gender'],
                       'birthdate' => $account['birthdate'],
                       'affiliate' => 4242
                       ));

    $poker->login($account['name'], $account['password']);
    header('Location: index.php?comment=Account%20created.');
    die();
  } catch(Exception $e) {
    $poker_error = $e->getMessage();
  }
} else {
  $account = array('name' => _get_string('name'));
}


	hci_header();

if($poker_error) {
  print "<!-- CREATE ACCOUNT ERROR PAGE " . $login . " -->";
  print "<h3>" . $poker_error . "</h3>";
}

?>
<!-- CREATE ACCOUNT <?php echo $login ?> -->

	<form method="post">
		<table>
			<tr>
				<td></td>
				<td><?php echo _('Create Account') ?></td>
			</tr>
			<tr>
				<td><b><?php echo _('Login') ?><sup>*</sup>:</b></td>
				<td><input type="text" size="20" maxlength="32" id="name" name="name" value="<?php print $account['name'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Password') ?><sup>*</sup>:</b></td>
				<td><input type="password" size="20" maxlength="32" id="password" name="password" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Password confirmation') ?><sup>*</sup>:</b></td>
				<td><input type="password" size="20" maxlength="32" id="password2" name="password2" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Email') ?><sup>*</sup>:</b></td>
				<td><input type="text" size="32" maxlength="128" id="email" name="email" value="<?php print $account['email'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Phone') ?>:</b></td>
				<td><input type="text" size="40" maxlength="64" id="phone" name="phone" value="<?php print $account['phone'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('First Name') ?>:</b></td>
				<td><input type="text" size="32" maxlength="32" id="firstname" name="firstname" value="<?php print $account['firstname'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Last Name') ?>:</b></td>
				<td><input type="text" size="32" maxlength="32" id="lastname" name="lastname" value="<?php print $account['lastname'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Gender') ?>:</b></td>
				<td>
				<select id="gender" name="gender">
                                <option value="M" <?php $account['gender'] == 'M' ? 'selected="selected"' : '' ?>><?php echo _('Mr') ?></option>
                                <option value="F" <?php $account['gender'] == 'F' ? 'selected="selected"' : '' ?>><?php echo _('Miss') ?></option>
                                </select>
                                </td>
			</tr>
			<tr>
			  <td><b><?php echo _('Birthday') ?>:</b></td>
                          <td><?php print $birthday->form(); ?></td>
			</tr>
			<tr>
				<td><b><?php echo _('Street') ?>:</b></td>
				<td><textarea id="addr_street" name="addr_street" cols="30" rows="3"><?php print $account['addr_street'] ?></textarea></td>
			</tr>
			<tr>
				<td><b><?php echo _('Street') ?> 2:</b></td>
				<td><textarea id="addr_street2" name="addr_street2" cols="30" rows="3"><?php print $account['addr_street2'] ?></textarea></td>
			</tr>
			<tr>
				<td><b><?php echo _('Zip code') ?>:</b></td>
				<td><input type="text" size="20" maxlength="64" id="addr_zip" name="addr_zip" value="<?php print $account['addr_zip'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Town') ?>:</b></td>
				<td><input type="text" size="50" maxlength="64" id="addr_town" name="addr_town" value="<?php print $account['addr_town'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('State') ?>:</b></td>
				<td><input type="text" size="50" maxlength="128" id="addr_state" name="addr_state" value="<?php print $account['addr_state'] ?>" /></td>
			</tr>
			<tr>
				<td><b><?php echo _('Country') ?>:</b></td>
				<td>
					<select id="addr_country" name="addr_country">
<?php

	$countries = file('country.txt');
	foreach ($countries as $country) {
		list ($code, $name) = explode (';', $country);
		$name = str_replace("\r", '', $name);
		$name = str_replace("\n", '', $name);
		echo '<option value="'.$name.'"'.($name == $account['addr_country']?
			' selected="selected"':'').'>'.$name.'</option>'."\r\n";
	}
?>
					</select>
				</td>
			</tr>
			<tr>
				<td></td>
				<td><input type="submit" name='submit' value="<?php echo _('Ok') ?>" /></td>
			</tr>
		</table>
	</form>
</div>
<?php
	hci_footer();
?>
