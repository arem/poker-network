#!/usr/bin/php
<?
/**
 * Ugly Flash socket policy file service. This script must be run as root from
 * the command line. It binds to port 843 on all interfaces and waits
 * indefinitely for connections. When a connection is detected, the script spits
 * out a chunk of xml and disconnects. It can only serve one request at a time,
 * but that shouldn't be much of a problem.
 *
 * One potential problem with this script is that you can easily lock port 843
 * up for an indeterminate amount of time if the script doesn't exit cleanly.
 * The OS should clear the port up for you eventually, but you could be stuck
 * playing the waiting game.
 *
 * This particular version of the script has only been tested very lightly.
 * Deploy at your own peril ;) YMMV.
 *
 * - Ammon Lauritzen [12/13/07]
 */

// define the xml policy "file"
$policy_file =
    '<'.'?xml version="1.0" encoding="UTF-8"?'.'>'.
    '<cross-domain-policy xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="http://www.adobe.com/xml/schemas/PolicyFileSocket.xsd">'.
        '<allow-access-from domain="*" to-ports="*" secure="false" />'.
        '<site-control permitted-cross-domain-policies="master-only" />'.
    '</cross-domain-policy>';

// make sure everything launches correctly
if( posix_getuid() != 0 )
    die( "You must run this script as root.\n" );

$sock = @socket_create( AF_INET, SOCK_STREAM, SOL_TCP );
if( !$sock )
    die( "Unable to create socket.\n" );

$succ = @socket_bind( $sock, "0.0.0.0", 843 );
if( !$succ )
    die( "Unable to bind to port 843.\n" );

$succ = @socket_listen( $sock );
if( !$succ )
    die( "Unable to start listening.\n" );

// start serving policies
while( true ) {
    $r = $w = $e = array( $sock );
    if( @socket_select( $r, $w, $e, null ) !== false ) {
        $conn = @socket_accept( $sock );
        if( $conn !== false ) {
            // somebody connected, just dump the xml and close
            socket_write( $conn, $policy_file );
            socket_close( $conn );
        } else {
            echo "socket_accept() failed?\n";
            break;
        }
    } else {
        echo "socket_select() failed?\n";
        break;
    }
}// end: listen forever

// clean up
socket_close( $sock );
?>
