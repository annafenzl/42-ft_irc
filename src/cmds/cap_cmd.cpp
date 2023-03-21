/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cap_cmp.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afenzl <afenzl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:35:55 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/20 11:39:27 by afenzl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/Server.hpp"

void Server::cap_command(Request request)
{
	std::string response;

	if (request.get_params()[0] == "LS")
	{
		response = (SERVER_NAME " CAP * LS :End of CAP LS negotiation");
		send_message(response, request.get_user()->get_fd());
	}	
}

