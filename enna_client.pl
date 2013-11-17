#!/usr/bin/env perl
use strict;
use warnings;
use IO::Socket::INET;
use XML::Simple;
use Data::Dumper;
 
# auto-flush on socket
$| = 1;
 
# my $request = {action => 'delete', selection => 'alarms', alarms => ['ABS', 
# 'Tar fgj']};
# my $request = {action => 'delete', selection => 'alarms', alarms => ['ABS']};
# my $request = {action => 'delete', selection => 'oldest', number_of_alarms => 5};
# my $request = {action => 'delete', selection => 'all'};
my @requests;
push @requests, {action => 'delete', selection => 'alarms', alarms => ['ABS', 'Tar fgj']};
push @requests, {action => 'delete', selection => 'alarms', alarms => ['ABS']};
push @requests, {action => 'delete', selection => 'oldest', number_of_alarms => 5};
push @requests, {action => 'delete', selection => 'all'};

my $socket;
for my $request (@requests) {
    # create a connecting socket
    $socket = new IO::Socket::INET (
        # PeerHost => '192.168.1.10',
        PeerHost => 'localhost',
        PeerPort => '7777',
        Proto => 'tcp',
        Reuse => 1
    );
    die "cannot connect to the server $!\n" unless $socket;
    print "connected to the server\n";
 
    # print Dumper($request);
    # data to send to a server
    # my $req = $ARGV[0] ? $ARGV[0] : 'hello world';
    my $XMLreq = XMLout($request);
    # $XMLreq = "Test invalid XML";
    my $size = $socket->send($XMLreq);
    print "sent data of length $size\n";
     
    # notify server that request has been sent
    # shutdown($socket, 1);
     
    # receive a response of up to 1024 characters from server
    my $response = "";
    $socket->recv($response, 1024);
    print "received response: $response\n";
    # $socket->recv($response, 1024);
    # print "received response: $response\n";
} 
$socket->close();
