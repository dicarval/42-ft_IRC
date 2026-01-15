#include "../../inc/Server.hpp"


std::string Server::splitQuit(std::vector<std::string> &tokens)
{
	if (tokens.size() == 1)
		return ("Quit");
	else
		return (findMsg(tokens));
}

void	Server::quit(std::vector<std::string> &tokens, int &fd)
{
	std::string reason;

	reason = splitQuit(tokens) + CRLF;
	while (this->_channels.size() != 0)
	{
		if (this->_channels[0].getClient(fd))
		{
			this->_channels[0].removeClient(fd);
			if (this->_channels[0].getNumberOfClients() == 0)
				removeChannel(this->_channels[0].getName());
			else
				this->_channels[0].sendToAll(":" + this->getClientFd(fd)->getNickName() + \
				"!~" + this->getClientFd(fd)->getUserName() + "@localhost QUIT " + reason);
		}
		if (this->_channels[0].getAdmin(fd))
		{
			this->_channels[0].removeAdmin(fd);
			if (this->_channels[0].getNumberOfClients() == 0)
				removeChannel(this->_channels[0].getName());
			else
				this->_channels[0].sendToAll(":" + this->getClientFd(fd)->getNickName() + \
				"!~" + this->getClientFd(fd)->getUserName() + "@localhost QUIT " + reason);
		}
	}
	this->endConnection(fd);
}
