/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:25:33 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/18 02:07:46 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_HPP
# define EXIT_HPP
typedef enum e_exit
{
	/// ! CHANNEL !
	EXIT_ERR_NEEDMOREPARAMS = 461,
	EXIT_ERR_NOSUCHCHANNEL = 403,
	EXIT_ERR_TOOMANYCHANNELS = 405,
	EXIT_ERR_CHANNELISFULL = 471,
	EXIT_ERR_INVITEONLYCHAN = 473,
	EXIT_ERR_BADCHANMASK = 476,
	EXIT_RPL_TOPICWHOTIME = 333,
	EXIT_ERR_NOTONCHANNEL = 442,
	/// ! PASSWORD/KEY !
	EXIT_ERR_BADCHANNELKEY = 475,
	EXIT_ERR_BANNEDFROMCHAN = 474,
	/// ! custom !
	/// ! SERVER !
	EXIT_ERR_NOTREGISTERED = 451,
	EXIT_ERR_ALREADYREGISTERED = 462,
	/// ! USER !
	EXIT_ERR_NOSUCHNICK = 401,
	/// ! RPL !
	EXIT_RPL_NAMREPLY = 353,
	EXIT_RPL_ENDOFNAMES = 366,
	EXIT_RPL_WHOREPLY = 352,
	EXIT_RPL_ENDOFWHO = 315,
	EXIT_RPL_LISTSTART = 321,
	EXIT_RPL_LIST = 322,
	EXIT_RPL_LISTEND = 323,
	EXIT_RPL_NOTOPIC = 331,
	EXIT_RPL_TOPIC = 332,
	// csutom
	EXIT_ERR_ALREADY_JOINED = 1000,
	EXIT_ERR_INVALID_CHANNEL_NAME = 1001,
	EXIT_LEFT_CHANNEL = 1002,
	EXIT_TOPIC_STRING = 1003,
	EXIT_CHANNEL_JOINED = 1004,
	EXIT_MODE_STRING = 1005
}	t_exit;
#endif
