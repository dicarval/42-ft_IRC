
#include "../inc/Channel.hpp"

//*CONSTRUCTORS AND DESTRUCTORS

Channel::Channel()
: channelName(), channelCreation(/* funcao geral de definicao de tempo */), clients(), admins(), inviteOnly(false), key(false), \
password(), channelTopic(), topicRestriction(false), topicCreation(), limitOfClients(0), modes()
{}

Channel::Channel(const Channel &og)
{
	*this = og;
}

Channel& Channel::operator=(const Channel &og)
{
	if (this != &og)
	{
		channelName = og.channelName;
		channelCreation = og.channelCreation;
		clients = og.clients;
		admins = og.admins;
		inviteOnly = og.inviteOnly;
		key = og.key;
		password = og.password;
		channelTopic = og.channelTopic;
		topicRestriction = og.topicRestriction;
		topicCreation = og.topicCreation;
		limitOfClients = og.limitOfClients;
		modes = og.modes;
	}
	return *this;
}

Channel::~Channel()
{}


//*GETTERS

Client*	Channel::getClient(int fd) const
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client*	Channel::getAdmin(int fd) const
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

std::string Channel::getName() const
{
	return channelName;
}

std::string Channel::getChannelCreation() const
{
	return channelCreation;
}

std::string Channel::getPassword() const
{
	return password;
}

std::string Channel::getChannelTopic() const
{
	return channelTopic;
}

std::string Channel::getTopicCreation() const
{
	return topicCreation;
}

std::vector<Client> Channel::getClientList() const
{
	return clients;
}

std::vector<Client> Channel::getAdminList() const
{
	return admins;
}

bool Channel::getInviteOnly() const
{
	return inviteOnly;
}

bool Channel::getKey() const
{
	return key;
}

bool Channel::getTopicRestriction() const
{
	return topicRestriction;
}

unsigned int Channel::getLimitOfClients() const
{
	return limitOfClients;
}


//*SETTERS

void Channel::setName(std::string v)
{
	channelName = v;
}

void Channel::setChannelCreation(std::string v)
{
	channelCreation = v;
}

void Channel::setPassword(std::string v)
{
	password = v;
}

void Channel::setChannelTopic(std::string v)
{
	channelTopic = v;
}

void Channel::setTopicCreation(std::string v)
{
	topicCreation = v;
}

void Channel::setClientList(std::vector<Client> v)
{
	clients = v;
}

void Channel::setAdminList(std::vector<Client> v)
{
	admins = v;
}

void Channel::setInviteOnly(bool v)
{
	inviteOnly = v;
}

void Channel::setKey(bool v)
{
	key = v;
}

void Channel::setTopicRestriction(bool v)
{
	topicRestriction = v;
}

void Channel::setLimitOfClients(unsigned int v)
{
	limitOfClients = v;
}


//*METHODS

void	Channel::addClient(Client newClient)
{
	clients.push_back(newClient);
}

void	Channel::addAdmin(Client newClient)
{
	admins.push_back(newClient);
}

void	Channel::removeClient(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			clients.erase(it);
			break ;
		}
	}
}

void	Channel::removeAdmin(int fd)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			admins.erase(it);
			break ;
		}
	}
}

bool	Channel::clientToAdmin(std::string &nick)
{
	size_t i = 0;
	for(; i < clients.size(); ++i)
	{
		if (clients[i].getNickName() == nick)
			break;
	}
	if (i < clients.size())
	{
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

bool	Channel::adminToClient(std::string &nick)
{
	size_t i = 0;
	for(; i < admins.size(); ++i)
	{
		if (admins[i].getNickName() == nick)
			break;
	}
	if (i < admins.size())
	{
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;
}

void	Channel::sendToAll(std::string repl)
{
	for (size_t i = 0; i < admins.size(); ++i)
		if (send(admins[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
			std::cout << "send() failed" << std::endl;
	for (size_t i = 0; i < clients.size(); ++i)
		if (send(clients[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
			std::cout << "send() failed" << std::endl;
}

void	Channel::sendToEveryoneElse(std::string repl, int fd)
{
	for (size_t i = 0; i < admins.size(); ++i)
	{
		if (admins[i].getFd() != fd)
			if (send(admins[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
				std::cout << "send() failed" << std::endl;
	}
	for (size_t i = 0; i < clients.size(); ++i)
	{
		if (clients[i].getFd() != fd)
			if (send(clients[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
				std::cout << "send() failed" << std::endl;
	}
}
