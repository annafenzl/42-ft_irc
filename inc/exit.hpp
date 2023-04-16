/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:25:33 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/16 13:06:49 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_HPP
# define EXIT_HPP
typedef enum e_exit
{
	EXIT_INFO_ONLY = -1,
	EXIT_LEFT_CHANNEL = 0,
	EXIT_CHANNEL_JOINED = 332,
	/// ! CHANNEL !
	EXIT_ERR_NEEDMOREPARAMS = 461,
	EXIT_ERR_NOSUCHCHANNEL = 403,
	EXIT_ERR_TOOMANYCHANNELS = 405,
	EXIT_ERR_CHANNELISFULL = 471,
	EXIT_ERR_INVITEONLYCHAN = 473,
	EXIT_ERR_BADCHANMASK = 476,
	EXIT_RPL_TOPIC = 332,
	EXIT_RPL_TOPICWHOTIME = 333,
	EXIT_ERR_NOTONCHANNEL = 442,
	/// ! PASSWORD/KEY !
	EXIT_ERR_BADCHANNELKEY = 475,
	EXIT_ERR_BANNEDFROMCHAN = 474,
	/// ! custom !
	EXIT_ERR_ALREADY_JOINED = 1000,
	EXIT_ERR_INVALID_CHANNEL_NAME = 1001,
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
}	t_exit;
#endif
