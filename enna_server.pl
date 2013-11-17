#!/usr/bin/env perl
use strict;
use warnings;
use IO::Socket::INET;
use XML::Parser;
 
# auto-flush on socket
$| = 1;
 
# creating a listening socket
my $socket = new IO::Socket::INET (
    LocalHost => '0.0.0.0',
    LocalPort => '7777',
    Proto => 'tcp',
    Listen => 5,
    Reuse => 1
);
die "cannot create socket $!\n" unless $socket;
print "server waiting for client connection on port 7777\n";
 
while(1)
{
    # waiting for a new client connection
    my $client_socket = $socket->accept();
 
    # get information about a newly connected client
    my $client_address = $client_socket->peerhost();
    my $client_port = $client_socket->peerport();
    print "connection from $client_address:$client_port\n";
 
    # read up to 1024 characters from the connected client
    my $request = "";
    $client_socket->recv($request, 1024);
    print "received data: $request\n";
 
    # initialize parser object and parse the string
    my $parser = XML::Parser->new( ErrorContext => 2 );
    eval { $parser->parse($request); };

    # report any error that stopped parsing, or announce success
    my $response = '';
    if ($@) {
        $@ =~ s/at \/.*?$//s;               # remove module line number
        $response = "ERROR in request:\n$@\n";
    } else {
        $response = "request is well-formed\n";
    }
    print STDERR "\n$response";

    # write response data to the connected client
    $client_socket->send($response);
 
    # notify client that response has been sent
    shutdown($client_socket, 1);
}
 
$socket->close();
