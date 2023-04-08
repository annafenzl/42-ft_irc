/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errco.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 14:25:33 by katchogl          #+#    #+#             */
/*   Updated: 2023/04/08 14:40:52 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRCO_H
# define ERRCO_H
typedef enum e_errco
{
	ERR_NONE = 0,
	/// ! CHANNEL JOIN !
	ERR_NEEDMOREPARAMS = 461,
	ERR_NOSUCHCHANNEL = 403,
	ERR_TOOMANYCHANNELS = 405,
	ERR_BADCHANNELKEY = 475,
	ERR_BANNEDFROMCHAN = 474,
	ERR_CHANNELISFULL = 471,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANMASK = 476,
	RPL_TOPIC = 332,
	RPL_TOPICWHOTIME = 333,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366
}	t_errco;
#endif
