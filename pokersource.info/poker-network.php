<?php
$root = ".";
$title = "poker-network";
require_once('header.php')
?>

<blockquote>

<h3>poker-network: poker server and client library</h3>

<p>
The server deals the cards and checks the user actions using a poker
game engine (poker-engine). It listens on a TCP/IP port and
understands a poker-network specific protocol. The persistent
informations (accounts, hand history etc.) are stored in a MySQL
database. The server was designed and tested to gracefully handle 1000
simultaneous players on a single machine also running the MySQL
server.
</p>
<p>
The client library implements the poker-network protocol for
the client. It runs a poker-engine identical to the one used by
the server and uses it to simplify the implementation of a client.
For instance it creates an event indicating that the player lost
position although the server does not send such a packet. A simple
minded bot is provided as an example usage of the client library.
</p>
<p>
The abstract user interface library provides a framework based on the
client library and suited to implement a user friendly client. A
display is fed with events such as give seat S to player P or get
amount A from side pot P to player P so that the rendering part of the
user interface does not need to maintain contextual game
information. A toolkit is fed with high level interaction actions such
as ask login and password or display the following holdem tables. An
animation module is fed with events that can trigger animations or
sounds such as player P timeouted or player P wins the pot.
</p>
<p>
The client based on the user interface library uses GTK. 
</p>

</blockquote>

<?php require_once('footer.php') ?>
