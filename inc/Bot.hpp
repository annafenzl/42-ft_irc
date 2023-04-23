#ifndef BOT_HPP
# define BOT_HPP

# include <iostream>
# include <string>

#include "User.hpp"
#include "Server.hpp"

class Bot : public User 
{
	public:
		Bot(int fd, const std::string& hostmask);
		~Bot();
};


#endif /* ************************************************************* BOT_H */