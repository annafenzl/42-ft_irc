/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:25:33 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/10 01:54:18 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXIT_H
# define EXIT_H
typedef enum e_exit
{
	EXIT_INFO_ONLY = -1,
	EXIT_DEFAULT = 0,
	EXIT_LEFT_CHANNEL = 0,
	EXIT_CHANNEL_JOINED = 332,
	/// ! CHANNEL !
	EXIT_ERR_NEEDMOREPARAMS = 461,
	EXIT_ERR_NOSUCHCHANNEL = 403,
	EXIT_ERR_TOOMANYCHANNELS = 405,
	EXIT_ERR_BADCHANNELKEY = 475,
	EXIT_ERR_BANNEDFROMCHAN = 474,
	EXIT_ERR_CHANNELISFULL = 471,
	EXIT_ERR_INVITEONLYCHAN = 473,
	EXIT_ERR_BADCHANMASK = 476,
	EXIT_RPL_TOPIC = 332,
	EXIT_RPL_TOPICWHOTIME = 333,
	EXIT_RPL_NAMREPLY = 353,
	EXIT_RPL_ENDOFNAMES = 366,
	EXIT_ERR_NOTONCHANNEL = 442,
	EXIT_ERR_ALREADY_JOINED = 1000,
	EXIT_ERR_INVALID_CHANNEL_NAME = 1001,
}	t_exit;
#endif
