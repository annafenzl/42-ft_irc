# 42-ft_irc
#### *A Internet Relay Chat Server*
This is a project for [42Heilbronn](https://www.42heilbronn.de/en/curriculum/) school's curriculum.

The goal of the project is to write a IRC-Server.

## TO DO
◦ You must be able to authenticate, set a nickname, a username, join a channel,
send and receive private messages using your reference client.

__mostly done, need to be tested, maybe need to add more error-checks__
- PING
- CAP
- PASS
- NICK
- USER
- PRIVMSG

__not done yet__

- OPER

__don't know if needed__
- QUIT
- SERVICE
- NOTICE
- GETFILE
- SENDFILE
...

==> Channel Operations:

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


◦ All the messages sent from one client to a channel have to be forwarded to
every other client that joined the channel.

◦ You must have operators and regular users.

◦ Then, you have to implement the commands that are specific to operators.
