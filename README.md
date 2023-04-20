# 42-ft_irc - A Internet Relay Chat Server

This is a project for [42Heilbronn](https://www.42heilbronn.de/en/curriculum/) school's curriculum. The goal of the project is to write a IRC-Server.

## Helpful Ressource

1. beej'is guide for learning about sockets - [irc-ressource](https://github.com/barimehdi77/ft_irc)
2. irc-specific guide - https://ircgod.com/
3. IRC bible - https://modern.ircdocs.horse/ (also here, https://datatracker.ietf.org/doc/html/rfc2812 https://datatracker.ietf.org/doc/html/rfc1459)
4. Doc on servers and clients in C/C++ - https://www.bogotobogo.com/cplusplus/sockets_server_client.php
5. Reference client: http://www.kvirc.net/?id=releases&platform=macosx&version=5.0.0&lang=en

## TO DO
◦ You must be able to authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.

◦ All the messages sent from one client to a channel have to be forwarded to
every other client that joined the channel.

◦ You must have operators and regular users.

◦ Then, you have to implement the commands that are specific to operators.

__mostly done, need to be tested, maybe need to add more error-checks__
- PING
- CAP
- PASS
- NICK
- USER
- PRIVMSG
- OPER (admin, 42)
- KILL (only operators, to add channels)
- GLOBOPS (only operators, messages all operators)
- NOTICE 
- QUIT (giver Server ERROR)
- DDC (file transfer, handled by default)
- BOT (showing current time)

__not done yet__

==> Channel Operations:
+ need to make a channel class;
+ add it to the server class, maybe as a std::map<std:string, Channel> (the key is the channel name)
+ add channel modes
  - a - user is flagged as away;
  - i - marks a users as invisible;
  - w - user receives wallops;
  - r - restricted user connection;
  - o - operator flag;
  - O - local operator flag;
  - s - marks a user for receipt of server notices.
  - there may be more
+ add functions

## 'Don't know if needed

- SERVICE
<<<<<<< HEAD
<<<<<<< HEAD
- NOTICE
- GETFILE
- SENDFILE

<!--what to do in case of ctrl C -->
=======
>>>>>>> origin/katchogl
=======

<!-- ODE :Unknown command>

-> Parsing |PART #channel2 :No boundaries on the net!| <-
--------- after parsing ----------
CMD: |PART|
PARAMS: <#channel2> <No boundaries on the net!> 
----------------------------------
RESPONSE IS <:sdasd!sda@:ircserv.com PART #channel2>



-> Parsing |PART #channel1 :No boundaries on the net!| <-
--------- after parsing ----------
CMD: |PART|
PARAMS: <#channel1> <No boundaries on the net!> 
----------------------------------
RESPONSE IS <:ircserv.com 442 sdasd :not on channel> -->
>>>>>>> origin/katchogl

// prevent gsl dangling pointer issue, need to make copy get_params ()
	// else params returned by functon call are destroyed, and begin or end its are invalid!