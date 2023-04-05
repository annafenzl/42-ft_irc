# 42-ft_irc - A Internet Relay Chat Server

This is a project for [42Heilbronn](https://www.42heilbronn.de/en/curriculum/) school's curriculum. The goal of the project is to write a IRC-Server.

## Helpful Ressource

1. beej'is guide for learning about sockets - [irc-ressource](https://github.com/barimehdi77/ft_irc)
2. irc-specific guide - https://ircgod.com/
3. IRC'yclopedia - https://modern.ircdocs.horse/
4. Doc on servers and clients in C/C++ - https://www.bogotobogo.com/cplusplus/sockets_server_client.php

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

__not done yet__

- OPER

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

## Channel Messages to implement:

1. JOIN
2. Part
3. MODE
4. TOPIC
5. NAMES
6. WHO
7. WHOIS
8. LIST
9. INVITE
10. KICK

## 'Don't know if needed

- QUIT
- SERVICE
- NOTICE
- GETFILE
- SENDFILE
