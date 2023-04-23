/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:25:33 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/23 13:20:36 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP
typedef enum e_res
{
	/** General */
	RES_ERR_NEEDMOREPARAMS = 461,
	RES_ERR_NOSUCHCHANNEL = 403,
	RES_ERR_NOTONCHANNEL = 442,
	RES_ERR_CHANNOPRIVSNEEDED = 482,
	RES_ERR_BADCHANNAME = 479,

	/** JOIN */
	RES_ERR_NOTREGISTERED = 451,
	RES_ERR_BADCHANNELKEY = 475,
	RES_ERR_BANNEDFROMCHAN = 474,
	RES_ERR_CHANNELISFULL = 471,

	/** LIST */
	RES_RPL_LISTSTART = 321,
	RES_RPL_LIST = 322,
	RES_RPL_LISTEND = 323,
	RES_RPL_CHANNELMODEIS = 324,

	/** TOPIC */
	RES_RPL_NOTOPIC = 331,
	RES_RPL_TOPIC = 332,

	/** WHO */
	RES_ERR_NOSUCHNICK = 401,
	RES_RPL_ENDOFWHO = 315,
	RES_RPL_WHOREPLY = 352,
	
	/** MODE */
	RES_ERR_USERSDONTMATCH = 502,
	RES_ERR_USERNOTINCHANNEL = 441,
	RES_ERR_UNKNOWNMODE = 472,

	/** Custom */
	RES_ERR_CHANNELALREADYJOINED = 1000,
	RES_CHANNELJOINED = 1001,
	RES_CHANNELLEFT = 1002,
	RES_MODE = 1003,

	/** INVITE */
	RES_ERR_USERONCHANNEL = 443,
	RES_ERR_USERNOTREGISTERED = 451,
	RES_ERR_INVITEONLYCHAN = 473,

	RES_INVITED = 341,
	
	RES_ERR_NOTANOPERATOR = 1004,
	RES_ERR_ALREADYANOPERATOR = 1005
}	t_res;
#endif
