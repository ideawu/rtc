# Roles:

* M: Manager
* S: Server
* C: Client

# Data Structures

class Packet{
	type
	seq
	data
};

# Interactions

## Basic Commands

noop
	roles: * <-> *

resp
	roles: * <-> *
	set seq as the sequence number this resp is to
	return ok|fail

## Room Management

open
	roles: M -> S
	params: feed_queue_size
	return: ok|fail room_id

close
	roles: M -> S
	params: room_id
	return: ok|fail

sign
	roles: M -> S
	params: room_id
	return: ok|fail token

join
	roles: C -> S
	params: room_id token
	return ok|fail

quit
	roles: C -> S
	params: room_id
	return ok|fail

## Subscription

sub
	roles: C -> S
	with_seq
	params: room_id
	return ok|fail

unsub
	roles: C -> S
	params: room_id
	return ok|fail

pub
	roles: C -> S
	with_seq
	params: room_id data
	return ok|fail

data
	roles: S -> C
	with_seq
	params: room_id data

retrx
	roles: C <-> S
	params: room_id seq








