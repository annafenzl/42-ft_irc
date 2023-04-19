/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:25:33 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/19 05:55:53 by katchogl         ###   ########.fr       */
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
	RES_ERR_NOTREGISTERED_CHAN = 451,
	RES_ERR_BADCHANNELKEY = 475,
	RES_ERR_BANNEDFROMCHAN = 474,

	/** LIST */
	RES_RPL_LISTSTART = 321,
	RES_RPL_LIST = 322,
	RES_RPL_LISTEND = 323,

	/** TOPIC */
	RES_RPL_NOTOPIC = 331,
	RES_RPL_TOPIC = 332,

	/** WHO */
	RES_ERR_NOSUCHNICK = 401,
	RES_RPL_ENDOFWHO = 315,
	RES_RPL_WHOREPLY = 352,

	/** Custom */
	RES_ERR_CHANNELALREADYJOINED = 1000,
	RES_CHANNELJOINED = 1001,
	RES_CHANNELLEFT = 1002,
	RES_MODE = 1003
}	t_res;
#endif
