/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pguranda <pguranda@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:48 by afenzl            #+#    #+#             */
/*   Updated: 2023/04/07 13:50:28 by pguranda         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
#  define CHANNEL_HPP
# include "User.hpp"
# include "Request.hpp"
# include "Server.hpp"
# include <vector>
# include <list>

// FOR PRIVILIGED MEMBERS
//         INVITE  - Invite a client to an invite-only channel (mode +i)
//         KICK    - Eject a client from the channel
//         MODE    - Change the channel's mode, as well as
//                   members' privileges
//         PRIVMSG - Sending messages to the channel (mode +n, +m, +v)
//         TOPIC   - Change the channel topic in a mode +t channel

// /Channel operators are identified by the '@' symbol next to their
//    nickname whenever it is associated with a channel (i.e., replies to
//    the NAMES, WHO and WHOIS commands).

// Since channels starting with the character '+' as prefix do not
//    support channel modes, no member can therefore have the status of
//    channel operator.

// STANDART CHANNELS '#' or "&"
// These channels are created implicitly when the first user joins it,
//    and cease to exist when the last user leaves it.  While the channel
//    exists, any client can reference the channel using the name of the
//    channel.

// The various modes available for channels are as follows:

		// O - give "channel creator" status;
		// o - give/take channel operator privilege;
		// v - give/take the voice privilege;

		// a - toggle the anonymous channel flag;
		// i - toggle the invite-only channel flag;
		// m - toggle the moderated channel;
		// n - toggle the no messages to channel from clients on the
		//     outside;
		// q - toggle the quiet channel flag;
		// p - toggle the private channel flag;
		// s - toggle the secret channel flag;
		// r - toggle the server reop channel flag;
		// t - toggle the topic settable by channel operator only flag;

		// k - set/remove the channel key (password);
		// l - set/remove the user limit to channel;

		// b - set/remove ban mask to keep users out;
		// e - set/remove an exception mask to override a ban mask;
		// I - set/remove an invitation mask to automatically override
		//     the invite-only flag;
		//    Unless mentioned otherwise below, all these modes can be manipulated
		//    by "channel operators" by using the MODE command defined in "IRC
		//    Client Protocol" [IRC-CLIENT].

class Server;

class Channel
{
	private:
	std::string			_channelname;
	std::string			_topic;
	short				_modes;
	// int					_limit;
	
	std::list<User>		_users;
	// std::list<User>		_operators;
	// std::list<User>		_banned;
	
	public:
	// ------------- constructor --------------
	Channel(Request &request);
	~Channel();
	
	std::string     get_name();
	std::list<User> get_users();
	void	add_user(User *user);
	void	remove_user(User *user);
	Channel(std::string name, User first_user);
	
	// --------------- getters ----------------
	// --------------- setters ----------------
	// --------------- methods ----------------
};

#endif
	