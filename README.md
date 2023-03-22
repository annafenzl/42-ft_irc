# 42-ft_irc
#### *A Internet Relay Chat Server*
This is a project for [42Heilbronn](https://www.42heilbronn.de/en/curriculum/) school's curriculum.

The goal of the project is to write a IRC-Server.

## Helpfull Ressource
[irc-ressource](https://github.com/barimehdi77/ft_irc)

beej'is guide for learning about sockets

irc-specific guide: https://ircgod.com/

And the IRC protocols are helpfull:)

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
+ be able to send PRIVMSG to channels
  -- also how about naming conventions, channelnames should be prefixed by usally # or maybe &, but there is more possiblities
+ add operators (should we implement the creator operator?)
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

-- CHANNEL CLASS

- JOIN
- Part
- MODE
- TOPIC
- NAMES
- WHO
- WHOIS
- LIST
- INVITE
- KICK

__don't know if needed__
- QUIT
- SERVICE
- NOTICE
- GETFILE
- SENDFILE
...