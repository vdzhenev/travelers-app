#include "user.hpp"

User::User(const char* n, const char* p, const char* e) : name(nullptr), password(nullptr), email(nullptr), friends(nullptr), trips(nullptr)
{
	setName(n);
	setPass(p);
	setEmail(e);
}

User::User(const User& u)
{
	setName(u.name);
	setPass(u.password);
	setEmail(u.email);
	setFriends(u);
	t_curr = u.t_curr;
	t_max = u.t_max;
	setTrips(u);
}

User& User::operator=(const User& u)
{
	if (this != &u)
	{
		setName(u.name);
		setPass(u.password);
		setEmail(u.email);
		f_curr = u.f_curr;
		f_max = u.f_max;
		setFriends(u);
		t_curr = u.t_curr;
		t_max = u.t_max;
		setTrips(u);
	}
	return *this;
}

User::~User()			/** Освобождаваме динамично заделената памет за име, парола, имейл, пътувания и всеки от приятелите */
{
	delete[] name;
	delete[] password;
	delete[] email;
	delete[] trips;
	for (ushort i = 0; i < f_curr; i++)
	{
		delete[] friends[i];
	}
	delete[] friends;
}

void User::setName(const char* n)
{
	if (!n)
		name = nullptr;
	else
	{
		unsigned size = strlen(n);
		delete[] name;
		name = new char[size + 1];
		for (unsigned i = 0; i < size; i++)
		{
			name[i] = n[i];
		}
		name[size] = '\0';
	}
}

void User::setPass(const char* p)
{
	if (!p)
		password = nullptr;
	else
	{
		unsigned size = strlen(p);
		delete[] password;
		password = new char[size + 1];
		for (unsigned i = 0; i < size; i++)
		{
			password[i] = p[i];
		}
		password[size] = '\0';
	}
}

void User::setEmail(const char* e)
{
	if (!e)
		email = nullptr;
	else
	{
		unsigned size = strlen(e);
		delete[] email;
		email = new char[size + 1];
		for (unsigned i = 0; i < size; i++)
		{
			email[i] = e[i];
		}
		email[size] = '\0';
	}
}

void User::setTrips(const User& u)		/** Освобождава паметта от текущите пътувания и заделя динамично нова памет спрямо пътуванията на подадения потребител */
{
	delete[] trips;
	trips = new Trip[u.t_max];
	for (ushort i = 0; i < u.t_curr; i++)
	{
		trips[i] = u.trips[i];
	}
}

void User::setFriends(const User& u)			/** Освобождава паметта от текущите приятели и ги заменя с приятелите на подадения потребител. Динамично се заделя памет за всяко от имената */
{
	for (ushort i = 0; i < f_curr; i++)
	{
		delete[] friends[i];
	}
	delete[] friends;
	friends = new char*[u.f_max];
	for (ushort i = 0; i < u.f_curr; i++)
	{
		unsigned size = strlen(u.friends[i]);
		friends[i] = new char[size + 1];
		for (unsigned j = 0; j < size; j++)
		{
			friends[i][j] = u.friends[i][j];
		}
		friends[i][size] = '\0';

	}
}

void User::addTrip(const Trip& t)		/** В случай че се надвишава максималния брой пътувания масива се заменя с нов с по-голяма дължина. Старата информация се прехвърля в новия масив, паметта от стария масив се освобождава */
{
	if (t_curr >= t_max)						
	{
		t_max += 8;
		Trip* n_trips = new Trip[t_max];
		for (ushort i = 0; i < t_curr; i++)	
		{
			n_trips[i] = trips[i];
		}
		delete[]trips;							
		trips = n_trips;
	}
	trips[t_curr++] = t;
}

void User::addFriend(const char* n)						/** Работи на същия принцип като функцията за добавяне на пътуване */
{
	if (f_curr >= f_max)
	{
		f_max += 8;
		char** n_friends = new char*[f_max];
		for (ushort i = 0; i < f_curr; i++)
		{
			unsigned size = strlen(friends[i]);
			n_friends[i] = new char[size + 1];
			for (unsigned j = 0; j < size; j++)
			{
				n_friends[i][j] = friends[i][j];
			}
			n_friends[i][size] = '\0';
			delete[] friends[i];
		}
		delete[] friends;
		friends = n_friends;
	}
	unsigned size = strlen(n);
	friends[f_curr] = new char[size + 1];
	for (unsigned i = 0; i < size; i++)
	{
		friends[f_curr][i] = n[i];
	}
	friends[f_curr++][size] = '\0';
}

std::ostream& operator<<(std::ostream& o, const User& u)			/** Извежда всяка дестинация, която е посетил потребителя, както и коментара, оставен към пътуването */
{
	o << "User: " << u.name <<"\nTrips: "<<u.t_curr<<std::endl;
	for (ushort i = 0; i < u.t_curr; i++)
	{
		o << "\nDestination:\n" << u.trips[i].getDest() << "\nComment:\n" << u.trips[i].getComment() << std::endl;
	}
	return o;
}