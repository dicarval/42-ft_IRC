
#include "../inc/Channel.hpp"

//*CONSTRUCTORS AND DESTRUCTORS

Channel::Channel()
: _channelName(), _channelCreation(/* funcao geral de definicao de tempo */), _clients(), _admins(), _inviteOnly(false), _key(false), \
_password(), _channelTopic(), _topicRestriction(false), _topicCreation(), _limitOfClients(0), _modes()
{}

Channel::Channel(const Channel &og)
{
	*this = og;
}

Channel& Channel::operator=(const Channel &og)
{
	if (this != &og)
	{
		_channelName = og._channelName;
		_channelCreation = og._channelCreation;
		_clients = og._clients;
		_admins = og._admins;
		_inviteOnly = og._inviteOnly;
		_key = og._key;
		_password = og._password;
		_channelTopic = og._channelTopic;
		_topicRestriction = og._topicRestriction;
		_topicCreation = og._topicCreation;
		_limitOfClients = og._limitOfClients;
		_modes = og._modes;
	}
	return *this;
}

Channel::~Channel()
{}


//*GETTERS

Client*	Channel::getClient(int fd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client*	Channel::getAdmin(int fd)
{
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
	{
		if (it->getFd() == fd)
			return &(*it);
	}
	return NULL;
}

Client*	Channel::getClientInChannel(std::string name)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
	{
		if (it->getNickName() == name)
			return &(*it);
	}
	return NULL;
}

std::string Channel::getName() const
{
	return _channelName;
}

std::string Channel::getChannelCreation() const
{
	return _channelCreation;
}

std::string Channel::getPassword() const
{
	return _password;
}

std::string Channel::getChannelTopic() const
{
	return _channelTopic;
}

std::string Channel::getTopicCreation() const
{
	return _topicCreation;
}

std::vector<Client> Channel::getClientList() const
{
	return _clients;
}

std::vector<Client> Channel::getAdminList() const
{
	return _admins;
}

bool Channel::getInviteOnly() const
{
	return _inviteOnly;
}

bool Channel::getKey() const
{
	return _key;
}

bool Channel::getTopicRestriction() const
{
	return _topicRestriction;
}

unsigned int Channel::getNumberOfClients() const
{
	return this->_clients.size() + this->_admins.size();
}

unsigned int Channel::getLimitOfClients() const
{
	return _limitOfClients;
}


//*SETTERS

void Channel::setName(std::string v)
{
	_channelName = v;
}

void Channel::setChannelCreation(std::string v)
{
	_channelCreation = v;
}

void Channel::setPassword(std::string v)
{
	_password = v;
}

void Channel::setChannelTopic(std::string v)
{
	_channelTopic = v;
}

void Channel::setTopicCreation(std::string v)
{
	_topicCreation = v;
}

void Channel::setClientList(std::vector<Client> v)
{
	_clients = v;
}

void Channel::setAdminList(std::vector<Client> v)
{
	_admins = v;
}

void Channel::setInviteOnly(bool v)
{
	_inviteOnly = v;
}

void Channel::setKey(bool v)
{
	_key = v;
}

void Channel::setTopicRestriction(bool v)
{
	_topicRestriction = v;
}

void Channel::setLimitOfClients(unsigned int v)
{
	_limitOfClients = v;
}


//*METHODS

void	Channel::addClient(Client newClient)
{
	_clients.push_back(newClient);
}

void	Channel::addAdmin(Client newClient)
{
	_admins.push_back(newClient);
}

void	Channel::removeClient(int fd)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			_clients.erase(it);
			break ;
		}
	}
}

void	Channel::removeAdmin(int fd)
{
	for (std::vector<Client>::iterator it = _admins.begin(); it != _admins.end(); ++it)
	{
		if (it->getFd() == fd)
		{
			_admins.erase(it);
			break ;
		}
	}
}

bool	Channel::clientToAdmin(std::string &nick)
{
	size_t i = 0;
	for(; i < _clients.size(); ++i)
	{
		if (_clients[i].getNickName() == nick)
			break;
	}
	if (i < _clients.size())
	{
		_admins.push_back(_clients[i]);
		_clients.erase(i + _clients.begin());
		return true;
	}
	return false;
}

bool	Channel::adminToClient(std::string &nick)
{
	size_t i = 0;
	for(; i < _admins.size(); ++i)
	{
		if (_admins[i].getNickName() == nick)
			break;
	}
	if (i < _admins.size())
	{
		_clients.push_back(_admins[i]);
		_admins.erase(i + _admins.begin());
		return true;
	}
	return false;
}

void	Channel::sendToAll(std::string repl)
{
	for (size_t i = 0; i < _admins.size(); ++i)
		if (send(_admins[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
			std::cout << "send() failed" << std::endl;
	for (size_t i = 0; i < _clients.size(); ++i)
		if (send(_clients[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
			std::cout << "send() failed" << std::endl;
}

void	Channel::sendToEveryoneElse(std::string repl, int fd)
{
	for (size_t i = 0; i < _admins.size(); ++i)
	{
		if (_admins[i].getFd() != fd)
			if (send(_admins[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
				std::cout << "send() failed" << std::endl;
	}
	for (size_t i = 0; i < _clients.size(); ++i)
	{
		if (_clients[i].getFd() != fd)
			if (send(_clients[i].getFd(), repl.c_str(), repl.size(), 0) == -1)
				std::cout << "send() failed" << std::endl;
	}
}
