#include "Server.hpp"

void	findReason(std::vector<std::string> &tokens, std::string &reason)
{
	std::string temp;
	for (int i = 1; i < tokens.size(); i++)
	{
		if (tokens.size() == i + 1)
			temp += tokens[i];
		else
			temp += tokens[i] + " ";
	}
	reason = temp;
}

int	Server::splitPart(std::vector<std::string> &tokens, std::string &reason, int fd)
{
	if (tokens.size() >= 2)
		findReason(tokens, reason);
	else
		reason = "";
}

void	Server::part(std::vector<std::string> tokens, int fd)
{
	std::string reason;
	if(!splitPart(tokens, reason, fd))
		return (sendResponse(ERR_NOTENOUGHPARAM(getClientFd(fd)->getNickName()), fd);

}
