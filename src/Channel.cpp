/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: katchogl <katchogl@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:22:22 by afenzl            #+#    #+#             */
/*   Updated: 2023/03/22 18:24:39 by katchogl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/Channel.hpp"

// Constructors && Destructor
Channel::Channel( std::string name, std::string topic ): _name(name), _topic(topic) {}
Channel::~Channel( void ) {}

// Getters
const std::string &Channel::getName( void ) { return (_name ); }
const std::string &Channel::getTopic( void ) { return (_topic ); }

// Utility
