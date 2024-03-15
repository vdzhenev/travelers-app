/** @file main.cpp */


#include <iostream>
#include "user.hpp"
#include "trip.hpp"
#include <fstream>

using std::cout;
using std::cin;
using std::endl;

/** @name Глобални променливи */
///@{
const ushort MAX_CMD_SIZE = 256;	///< Максимална дължина на входа
User* ALL_USERS;				///< Масив, съдържащ всички потребители
ushort CURR_USERS;				///< Брой текущи потребители
ushort MAX_USERS;				///< Максимален брой потребители
User* LOGGED_IN = nullptr;		///< Текущ потребител; Някои функции изискват предварително вписване на потребител
char** DESTINATIONS;			///< Масив с имена на посетени от потребителите дестинации
ushort D_MAX;					///< Максимален размер на масива
ushort D_CURR;					///< Текущ брой запписани дестинации
///@}

/** Структура, която обработва входа */
struct CommandLine
{
	
	char inp[MAX_CMD_SIZE];	///< Вход		
	unsigned p = 0;		///< Показва докъде е прочетен входа		

	void extractWord(char* str)		/** Извлича само една дума от въведения вход (до достигане на празно място или край на входа) */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != ' ' && inp[p] != '\n'; p++)
		{
			str[i++] = inp[p];
		}
		str[i] = '\0';
	}

	void extractName(char* str)		/** Извлича име (на дестинация, всички думи преди срещането на число), премахва празните места в края на низа */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != '\n'; p++)
		{
			if (inp[p] >= '0'&&inp[p] <= '9')
			{
				while (str[--i] == ' ')
				{
					;
				}
				i++;
				break;
			}
			str[i++] = inp[p];
		}
		str[i] = '\0';
	}

	void extractDate(Date &d)		/** Извлича дата (година-месец-ден) */
	{
		ushort y, m, day;
		extractNumberShort(y);
		extractNumberShort(m);
		extractNumberShort(day);
		d.setYear(y);
		d.setMonth(m);
		d.setDay(day);
	}

	void extractNumberShort(ushort &n)			/** Извлича число тип unsigned short */
	{
		clearWhitespaces();
		if (inp[p] != '\0')
			n = 0;
		for (; inp[p] != '\0' && inp[p] >= '0' && inp[p] <= '9'; p++)
		{
			n = n * 10 + inp[p] - '0';
		}
	}

	void extractLeftoverString(char* str)		/** Извлича всичко останало от входа, като премахва празното място в края */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != '\n'; p++)
		{
			str[i++] = inp[p];
		}
		while (str[--i] == ' ')
		{
			;
		}
		i++;
		str[i] = '\0';
	}

	void extractComment(char* str)			/** Извлича коментар; Символът '/' разделя коментара от снимките */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != '\n' && inp[p]!='/'; p++)
		{
			str[i++] = inp[p];
		}
		while (str[--i] == ' ')
		{
			;
		}
		i++;
		str[i] = '\0';
	}

	void clearWhitespaces()						/** Премахва всички празни места до срещането на различен знак */
	{
		while (inp[p] == ' ' || inp[p] == '-'||inp[p]=='/') 	p++;
	}

};

/**@name Работа с потребителски данни */
///@{
void addUser(const char* n, const char* p, const char* e)	/** Добавя потребител в системата по подадени име, парола и имейл; Когато се достигне максимален брой потребители, масивът се разширява с 8. Текущите потребители се копират в нов масив, старият се изтрива */
{
	if (CURR_USERS >= MAX_USERS)							
	{														
		MAX_USERS += 8;
		User* n_users = new User[MAX_USERS];
		for (ushort i = 0; i < CURR_USERS; i++)
		{
			n_users[i] = ALL_USERS[i];
		}
		delete[] ALL_USERS;
		ALL_USERS = n_users;
	}
	ALL_USERS[CURR_USERS].setName(n);
	ALL_USERS[CURR_USERS].setPass(p);
	ALL_USERS[CURR_USERS].setEmail(e);
	CURR_USERS++;
}

void addDest(const char* n)		/** Ако подаденото име не се среща го добавя към масива с имена на дестинации */
{
	bool flag = false;
	for (ushort i = 0; i < D_CURR; i++)
	{
		if (!strcmp(n, DESTINATIONS[i]))
			flag = true; break;
	}
	if (!flag)
	{
		if (D_CURR >= D_MAX)
		{
			D_MAX += 8;
			char** n_dest = new char*[D_MAX];
			for (ushort i = 0; i < D_CURR; i++)
			{
				n_dest[i] = new char[MAX_CMD_SIZE];
				strcpy_s(n_dest[i], MAX_CMD_SIZE, DESTINATIONS[i]);
				delete[]DESTINATIONS[i];
			}
			delete[] DESTINATIONS;
			DESTINATIONS = n_dest;
		}
		strcpy_s(DESTINATIONS[D_CURR++], MAX_CMD_SIZE, n);
	}
}

ushort findUser(const char* n)									/** Намира потребител по дадено име, връща неговия индекс в масива със записани потребители. В случай, че не намери дадения потребител, връща максималния брой потребители */
{
	for (ushort i = 0; i < CURR_USERS; i++)
	{
		if (ALL_USERS[i].name&&!strcmp(ALL_USERS[i].name, n))
			return i;
	}
	return MAX_USERS;	
}

void reg(const char* n, const char* p, const char* e)		/** Регистрира потребител в системата чрез име, парола, имейл. Проверява за съществуването на потребител със същото име */
{
	if (findUser(n) != MAX_USERS)
		cout << "Username already taken!\n";
	else
	{
		addUser(n, p, e);
		cout << "Registration successfull! Welcome aboard, " << n << "!\n" << "To log in, use command \"login\"!\n";
	}
}

void login(const char* n, const char* p)				/** Вписване на потребител чрез въведени правилни име и парола */
{
	if (!LOGGED_IN)
	{
		ushort ind = findUser(n);
		if (ind == MAX_USERS)							/** Проверява дали потребителят е открит */
			cout << "Username not found! To register use command \"register\"!\n";
		else
		{
			if (!strcmp(p, ALL_USERS[ind].password))
			{
				cout << "Log in successful! Welcome back, " << n << "!\n";
				LOGGED_IN = &ALL_USERS[ind];
			}
			else
				cout << "Incorrect password!\n";
		}
	}
	else
		cout << "You have to log out of the current account first!\nUse command \"logout\"!";
}
///@}

/**@name Валидации */
///@{
bool validEmail(char* email)	/*! Валидация на имейл - проверява дали даден низ съдържа символите @ и . */
{
	bool at = false;
	bool dot = false;
	while (*email)
	{
		if (*email == '@')
		{
			if (!at)
				at = true;
			else
				return false;
		}
		else if (*email == '.'&&at)
			dot = true;
		email++;
	}
	return (at&&dot);
}

bool checkExtension(char* str)			//! Проверява за валидност на разширенията на изображение
{
	char* ext1 = ".jpeg";				
	char* ext2 = ".png";				
	unsigned i = 0;						
	while (*(str + i))
	{
		if (*(str + i) == *(ext1 + i))
			i++;
		else
			break;
	}
	if (ext1[i] == '\0')				
		return true;
	i = 0;
	while (*(str + i))
	{
		if (*(str + i) == *(ext2 + i))
			i++;
		else
			return false;
	}
	if (ext2[i] == '\0')				
		return true;
	return false;
}

bool validPhoto(char* p)					//! Проверява дали името на изображението е валидно
{

	CommandLine cmd;
	strcpy_s(cmd.inp, MAX_CMD_SIZE, p);						/*! Копира подадения низ в променлива CommandLine */
	while (cmd.inp[cmd.p] != '\n'&&cmd.inp[cmd.p] != '\0')	/*! Проверява всяка дума от низа чрез CommandLine::extractWord() - трябва да съдържа '.', следвана от поддържано разширение и да не съдържа забранени символи*/
	{
		bool dot = false;									
		char curr[MAX_CMD_SIZE];							
		cmd.extractWord(curr);
		unsigned i = 0;
		while (curr[i] != '\n'&&curr[i] != '\0')
		{
			if (curr[i] == '.')
			{
				dot = true;
				if (!checkExtension(curr + i))				
					return false;
			}
			else if ((curr[i] < 'A' || (curr[i]>'Z'&&curr[i]<'a') || curr[i]>'z') && curr[i] != '_')	
				return false;								

			i++;
		}
		if (!dot)
			return false;
	}
	return true;
}
///@}

/** @name Работа с файлове */
///@{
/** Записва пътуване във файл, данните са подредени както в класа, предхождани от число - дължина, където е необходимо (име, коментар...) */
void saveTrip(std::ofstream& outfile, Trip& t)		
{
	ushort dlen = strlen(t.getDest());				/// Първо е число, представляващо дължината на името на дестинацията
	outfile.write((char*)&dlen, sizeof(dlen));		
	outfile.write(t.getDest(), dlen*sizeof(char));	/// Името на дестинацията

	ushort y, m, d;

	/** Начало на периода */
	y = t.getDateFrom().getYear();
	m = t.getDateFrom().getMonth();
	d = t.getDateFrom().getDay();
	outfile.write((char*)&y, sizeof(y));
	outfile.write((char*)&m, sizeof(m));
	outfile.write((char*)&d, sizeof(d));

	/** Край на периода */
	y = t.getDateTo().getYear();					
	m = t.getDateTo().getMonth();
	d = t.getDateTo().getDay();
	outfile.write((char*)&y, sizeof(y));
	outfile.write((char*)&m, sizeof(m));
	outfile.write((char*)&d, sizeof(d));

	ushort g = t.getGrade();						/// Оценка
	outfile.write((char*)&g, sizeof(g));

	ushort clen = strlen(t.getComment());			/// Дължина на коментара
	outfile.write((char*)&clen, sizeof(clen));
	outfile.write(t.getComment(), clen*sizeof(char));	/// Коментар

	ushort plen = strlen(t.getPhotos());			/// Дължина на низа, съдържащ имената на снимките
	outfile.write((char*)&plen, sizeof(plen));
	outfile.write(t.getPhotos(), plen*sizeof(char));	/// Имената на снимките
}

/** Запазва данните на потребител във файл - пътувания, приятели */
void saveUser(const User& u)				
{
	std::ofstream outfile;
	ushort len = strlen(u.name);			
	char* n = new char[len + 4];			/** Името на файла е <името на потребителя>.db */
	const char* ext = ".db";
	for (ushort i = 0; i < len; i++)
	{
		n[i] = u.name[i];
	}
	for (ushort i = len; i <len + 3; i++)
	{
		n[i] = ext[i - len];
	}
	n[len + 3] = '\0';
	outfile.open(n,std::ios::binary|std::ios::out|std::ios::trunc);
	if (!outfile.is_open())
		cout << "Error while saving user " << u.name << "'s data!\n";
	else
	{
		ushort trips = u.t_curr;						/** Брой пътувания */
		outfile.write((char*)&trips, sizeof(trips));	
		for (ushort i = 0; i < trips; i++)				
		{
			saveTrip(outfile, u.trips[i]);				/** Записва всяко пътуване чрез функцията saveTrip() */
		}
		ushort friends = u.f_curr;						/** Брой приятели */
		outfile.write((char*)&friends, sizeof(friends));
		for (ushort j = 0; j < friends; j++)			/** Записва имената на всички приятели */
		{
			ushort flen = strlen(u.friends[j]);				/** Дължина на името */
			outfile.write((char*)&flen, sizeof(flen));
			outfile.write(u.friends[j],flen*sizeof(char ));	/** Име на приятел */
		}
	}
	delete[] n;
}

/** Записва база данни на всички потребители - имена, пароли, имейли */
void saveFile(char* filename)			
{
	bool flag = false;
	if (*filename == '\0')				/** Ако не е отворен файл дава опция за отказ от презаписване на файла по подразбиране */
	{
		char proceed = 'n';
		cout << "No opened file! This action will overwrite \"users.db\"!\n";
		do
		{
			char temp = ' ';
			cout << "Proceed ? [y / n]\n> ";
			cin.get(proceed);
			cin.get(temp);

		} while (proceed != 'y'&&proceed != 'n');
		if (proceed == 'y')
			strcpy_s(filename, MAX_CMD_SIZE, "users.db");
		else
		{
			cout << "Progress not saved! You can use command \"saveas\" to save to a different file!\n";
			flag = true;
		}
	}
	if (!flag)
	{
		std::ofstream outfile;
		outfile.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
		if (!outfile.is_open())
			cout << "Error! There was a problem while saving!\n";
		else
		{
			cout << "Saving user data...\n";
			outfile.write((char*)&CURR_USERS, sizeof(CURR_USERS));		/** Първо записва брой регистрирани потребители */
			for (ushort i = 0; i < CURR_USERS; i++)
			{
				ushort nlen, plen, elen;


				/** За име, парола и имейл записва първо дължина на низа, след което самия низ */
				nlen = strlen(ALL_USERS[i].name);						
				outfile.write((char*)&nlen, sizeof(nlen));
				outfile.write(ALL_USERS[i].name, nlen*sizeof(char));

				plen = strlen(ALL_USERS[i].password);
				outfile.write((char*)&plen, sizeof(plen));
				outfile.write(ALL_USERS[i].password, plen*sizeof(char));

				elen = strlen(ALL_USERS[i].email);
				outfile.write((char*)&elen, sizeof(elen));
				outfile.write(ALL_USERS[i].email, elen*sizeof(char));
				saveUser(ALL_USERS[i]);									/** Запазване на данните на всеки потребител чрез saveUser() */
			}
			cout << "Successfully saved " << filename << "!\n";
			outfile.close();
		}
	}
}

/** Зарежда пътувания от файл, като използва set-функциите на класа Trip. Работи аналогично на функцията за запазване на пътувания във файл saveTrip()*/
void loadTrip(std::istream& infile,Trip& t)		
{
	ushort dlen;
	infile.read((char*)&dlen, sizeof(dlen));
	char* dest = new char[dlen + 1];
	infile.read(dest, dlen*sizeof(char));
	dest[dlen] = '\0';
	t.setDest(dest);
	delete[]dest;

	ushort y, m, d;
	infile.read((char*)&y, sizeof(y));
	infile.read((char*)&m, sizeof(m));
	infile.read((char*)&d, sizeof(d));
	Date from(y, m, d);

	infile.read((char*)&y, sizeof(y));
	infile.read((char*)&m, sizeof(m));
	infile.read((char*)&d, sizeof(d));
	Date to(y, m, d);
	
	t.setPeriod(from, to);

	ushort g;
	infile.read((char*)&g, sizeof(g));
	t.setGrade(g);

	ushort clen;
	infile.read((char*)&clen, sizeof(clen));
	char* cmnt = new char[clen + 1];
	infile.read(cmnt, clen*sizeof(char));
	cmnt[clen] = '\0';
	t.setComment(cmnt);
	delete[] cmnt;

	ushort plen;
	infile.read((char*)&plen, sizeof(plen));
	char* photos = new char[plen + 1];
	infile.read(photos, plen*sizeof(char));
	photos[plen] = '\0';
	t.setPhotos(photos);
	delete[] photos;

}

/** Зарежда пътуванията и приятелите на потребител */
void loadUserData(User& u)					
{
	ushort len = strlen(u.name);
	char* filename = new char[len + 4];		/** Създава името на файла - <име на потребител>.db */
	const char* ext = ".db";
	for (ushort i = 0; i < len; i++)
	{
		filename[i] = u.name[i];
	}
	for (ushort i = len; i <len + 3; i++)
	{
		filename[i] = ext[i - len];
	}
	filename[len + 3] = '\0';

	std::ifstream infile;
	infile.open(filename, std::ios::in | std::ios::binary);
	if (!infile.is_open())
		cout << "Error! Couldn't find user " << u.name << "'s personal database!\n";
	else
	{
		cout << filename << endl;
		ushort trips;
		infile.read((char*)& trips, sizeof(trips));				/** Прочита броя пътувания и записва информацията за всяко от тях чрез loadTrip()*/
		for (ushort i = 0; i < trips&&!infile.eof(); i++)
		{
			Trip temp;
			loadTrip(infile, temp);
			u.addTrip(temp);
		}
		ushort friends;											
		infile.read((char*)&friends, sizeof(friends));			/** Аналогично и за приятелите на потребителя */
		for (ushort i = 0; i < friends&&!infile.eof(); i++)
		{
			ushort flen;
			infile.read((char*)&flen, sizeof(flen));

			char* fr_name = new char[flen + 1];
			infile.read(fr_name, flen*sizeof(flen));
			fr_name[flen] = '\0';
			u.addFriend(fr_name);
			delete[] fr_name;
		}
	}
	infile.close();
	delete[] filename;

}

/** Отваря база с основни данни на потребители - имена, пароли, имейли */
void openFile(char* filename)			
{
	if (!filename)
		strcpy_s(filename, MAX_CMD_SIZE, "users.db");
	std::ifstream infile;
	
	infile.open(filename, std::ios::in | std::ios::binary);
	if (!infile.is_open())
		cout << "Error! Couldn't open file!\n";
	else
	{
		cout << "Loading user files...\n";
		infile.read((char*)&CURR_USERS, sizeof(CURR_USERS));		/** Извлича брой регистрирани потребители */
		MAX_USERS = 8 * (CURR_USERS / 8) + 8;						/** Пресмята брой нужни места, като се придържа към това, максималният брой да е кратен на 8 */
		delete[] ALL_USERS;

		ALL_USERS = new User[MAX_USERS];							/** Изтрива текущия масив с потребители и създава нов */
		for (ushort i = 0; i < CURR_USERS && !infile.eof(); i++)	/** Попълва масива с информацията от файла, използва set-функциите на User */
		{
			ushort nlen, plen, elen;								

			/** Задава име, парола и имейл на потребител като ги прочита от файл */
			infile.read((char*)&nlen, sizeof(nlen));
			char* n = new char[nlen + 1];
			infile.read(n, nlen*sizeof(char));
			n[nlen] = '\0';
			ALL_USERS[i].setName(n);
			delete[] n;

			infile.read((char*)&plen, sizeof(plen));
			char* p = new char[plen + 1];
			infile.read(p, plen*sizeof(char));
			p[plen] = '\0';
			ALL_USERS[i].setPass(p);
			delete[]p;

			infile.read((char*)&elen, sizeof(elen));
			char* e = new char[elen+1];
			infile.read(e, elen*sizeof(char));
			e[elen] = '\0';
			ALL_USERS[i].setEmail(e);
			delete[] e;

			loadUserData(ALL_USERS[i]);									/** Зарежда данните на всеки потребител чрез loadUserData()*/
			for (ushort j = 0; j < ALL_USERS[i].t_curr; j++)
			{
				addDest(ALL_USERS[i].trips[j].getDest());				/** Добавя имената на посетените от потребителя дестинации в масива чрез addDest()*/
			}
		}
		infile.close();
	}
}
///@}

/*! Основно тяло на програмата*/
int main()
{
	char OPENED_FILE[MAX_CMD_SIZE];
	OPENED_FILE[0] = '\0';

	D_MAX = 8;													
	D_CURR = 0;
	DESTINATIONS = new char*[D_MAX];
	for (ushort i = 0; i < 8; i++)
	{
		DESTINATIONS[i] = new char[MAX_CMD_SIZE];
		DESTINATIONS[i][0] = '\0';
	}
	
	CURR_USERS = MAX_USERS = 0;	
	cout << "Type \"help\" for a list of available commands.\n";
	while (true)
	{
		CommandLine cmd;
		char command[MAX_CMD_SIZE];
		cout << "\n> ";
		cin.getline(cmd.inp, MAX_CMD_SIZE);
		cmd.extractWord(command);
		//! "exit" - затваря приложението
		if (!strcmp(command,"exit"))							
		{
			cout << "Closing the program...\n";
			delete[] ALL_USERS;
			break;
		}

//!==========================================================================================================================================
		//! "help" - показва списък с поддържаните команди
		else if (!strcmp(command, "help"))						
		{
			cout << "\nThe following commands are supported:\n\n"
				<< "open <file>\n"
				<< "close\n"
				<< "save\n"
				<< "saveas <file>\n"
				<< "help\n"
				<< "exit\n\n"

				<< "register <username> <password> <email>\n"
				<< "login <username> <password>\n"
				<< "logout\n"
				<< "addfriend <username>\n"
				<< "friendlist\n"
				<< "post <destination_name> <grade> <from_date> <to_date> <comment> <photos>\n"
				<< "destination [<destination_name>]\n";

		}

//!==========================================================================================================================================
		//! "register" - добавя потребител към системата, използва функция reg()
		else if (!strcmp(command, "register"))					
		{
			if (!LOGGED_IN)										//! Нужно е да няма вписан потребител
			{
				char name[MAX_CMD_SIZE];
				char pass[MAX_CMD_SIZE];
				char email[MAX_CMD_SIZE];
				cmd.extractWord(name);
				cmd.extractWord(pass);
				cmd.extractWord(email);

				if (!*name)										
					cout << "Username is required!\n";
				else if (!*pass)
					cout << "A password is required!\n";
				else if (!validEmail(email))
					cout << "Please, enter a valid email!";
				else
					reg(name, pass, email);

			}
			else
				cout << "You must first log out of your account! You can log out using command \"logout\"!\n";

		}

//!==========================================================================================================================================
		//! "login" - вписва потребител, използва функция login()
		else if (!strcmp(command, "login"))						
		{
			if (!LOGGED_IN)
			{
				char name[MAX_CMD_SIZE];
				char pass[MAX_CMD_SIZE];
				cmd.extractWord(name);
				cmd.extractWord(pass);
				login(name, pass);
			}
			else
				cout << "User already logged in! You can log out using command \"logout\"!\n";

		}

//!==========================================================================================================================================
		//! "logout" - изход от текущия профил 
		else if (!strcmp(command, "logout"))
		{
			if (!LOGGED_IN)
				cout << "You must be logged in to use this command!\n";
			else
			{
				cout << "Logged out successfully! See you soon, " << LOGGED_IN->name << "!\n";
				LOGGED_IN = nullptr;
			}	

		}
//!==========================================================================================================================================
		//! "addfriend" - Добавя име към списъка с приятели на текущия потребител
		else if (!strcmp(command, "addfriend"))					
		{
			if (!LOGGED_IN)
				cout << "You must be logged in your account to add friends!\nUse command \"login\"!\n";
			else
			{
				char name[MAX_CMD_SIZE];
				cmd.extractWord(name);
				if (!strcmp(name, LOGGED_IN->name))	
					cout << "That's you!\n";
				else
				{												
					bool flag = false;							/** Проверява дали името вече е в списъка с приятели */
					for (ushort j = 0; j < LOGGED_IN->f_curr; j++)
					{
						if (!strcmp(name, LOGGED_IN->friends[j]))
						{
							cout << "You and " << name << " are already friends!\n";
							flag = true;
						}
					}
					if (!flag)
					{
						ushort i = 0;
						for (; i < CURR_USERS; i++)
						{
							if (!strcmp(name, ALL_USERS[i].name))		/** Търси въведеното име в масива с регистрирани потребители */
							{
								LOGGED_IN->addFriend(name);				/** В случай, че е намерено, добавя името към списъка с приятели на текущия потребител чрез User::addFriend() */
								cout << "User " << name << " is now your friend!\n" <<
									"To see a list of all of your current friends and their trips, use command \"friendlist\"!\n";
								break;
							}
						}
						if (i == CURR_USERS)
							cout << "Error! User not found!\n";
					}
				}
			}

		}
//!==========================================================================================================================================
		//! "friendlist" - Изважда списък с приятелите на текущо вписаният потребител
		else if (!strcmp(command, "friendlist"))				
		{
			if (!LOGGED_IN)
				cout << "You must be logged in your account to see your friendlist!\nUse command \"login\"!\n";
			else
			{
				ushort size = LOGGED_IN->f_curr;
				cout << "Your friends: " << size << endl << endl;
				for (ushort i = 0; i < size; i++)
				{
					for (ushort j = 0; j < CURR_USERS; j++)
					{
						if (!strcmp(LOGGED_IN->friends[i], ALL_USERS[j].name))
						{
							cout << ALL_USERS[j] << endl;
							break;
						}
					}
				}
				cout << "You can always add more friends by using the command \"addfriend\"!\n";
			}

		}
//!==========================================================================================================================================
		//! "post" - Добавя дестинация към пътуванията на текущия потребител
		/**  (След като валидира името на дестинацията, оценката, датите, периода и изображенията) */
		else if (!strcmp(command, "post"))				
		{
			if (!LOGGED_IN)
				cout << "You must be logged in to post!\nUse command \"login\"!\n";
			else
			{
				char name[MAX_CMD_SIZE];		
				ushort g;						
				Date d_from;					
				Date d_to;						
				char cmnt[MAX_CMD_SIZE];		
				char img[MAX_CMD_SIZE];			
				cmd.extractName(name);
				if (name[0] == '\0')			
					cout << "Please enter a destination!\n";
				else
				{
					addDest(name);
					cmd.extractNumberShort(g);
					if (g<1 || g>5)					
						cout << "Invalid grade! Must be between 1 and 5!\n";
					else
					{
						cmd.extractDate(d_from);
						cmd.extractDate(d_to);
						if (!d_from.isValid() || !d_to.isValid())		
							cout << "Invalid date(s)!\n";
						else if (d_from > d_to)
							cout << "Invalid period!\n";				
						else
						{
							cmd.extractComment(cmnt);
							cmd.extractLeftoverString(img);
							if (validPhoto(img))						
							{
								Trip t(name, g, cmnt, img);
								t.setPeriod(d_from, d_to);
								LOGGED_IN->addTrip(t);
								cout << t << endl;
								cout << "Success! You can check other people's experience in " << name << " by using command \"destination\".\n";
							}
							else
								cout << "Invalid image name/extension!\n";
						}
					}
				}
			}

		}
//!==========================================================================================================================================
		//! "destination" - Търси информация за дадена дестинация - коментари от други потребители и техните оценки.
		/** Извежда средно аритметично от оценките */

		else if (!strcmp(command, "destination"))
		{
			char name[MAX_CMD_SIZE];
			cmd.extractName(name);
			if (name[0] == '\0')							/** Ако не е въведено име се извеждат всички дестинации, посетени от потребители */
			{
				cout << "People made posts about these destinations:\n";
				for (ushort i = 0; i < D_CURR; i++)
				{
					cout << DESTINATIONS[i] << endl;
				}
			}
			else
			{
				ushort visitors = 0;							
				double total = 0;								
				bool flag = false;								
				for (ushort i = 0; i < CURR_USERS; i++)
				{
					for (ushort j = 0; j < ALL_USERS[i].t_curr; j++)
					{
						if (!strcmp(name, ALL_USERS[i].trips[j].getDest()))	
						{
							flag = true;
							ushort tmp = ALL_USERS[i].trips[j].getGrade();
							cout << "User: " << ALL_USERS[i].name << "\tGrade: " << tmp << endl;
							visitors++;
							total += tmp;
						}
					}
				}
				if (flag)		
					cout << "Average grade: " << total / visitors << "  (" << visitors << ")\n";	
				else
					cout << "So far nobody has posted about " << name << ".\n";		
			}

		}
//!==========================================================================================================================================
		//! "save" - Запазва текущия файл
		else if (!strcmp(command, "save"))	
		{
			saveFile(OPENED_FILE);
		}


//!==========================================================================================================================================
		//! "saveas - Запазва в посочен файл
		else if (!strcmp(command, "saveas"))	
		{
			cmd.extractLeftoverString(OPENED_FILE);
			saveFile(OPENED_FILE);
		}

//!==========================================================================================================================================
		//! "open" - Отваря файл
		else if (!strcmp(command, "open"))		
		{
			cmd.extractLeftoverString(OPENED_FILE);
			openFile(OPENED_FILE);
		}

//!==========================================================================================================================================
		//! "close" - "Затваря" текущия файл
		else if (!strcmp(command, "close"))		
		{
			cout << "Successfully closed " << OPENED_FILE << "!\n";
			OPENED_FILE[0]='\0';
		}

//!==========================================================================================================================================
		else if (!strcmp(command, "hackermans"))
		{
			cout << "HACKING";
			for (ushort i = 0; i < 69; i++)
			{
				cout << '.';
			}
			cout << endl<<"SECURITY BREACHED!\nUSER INFO:\n";
			for (ushort i = 0; i < CURR_USERS; i++)
			{
				cout << ALL_USERS[i].name << ' ' << ALL_USERS[i].password << endl;
			}
		}
//!==========================================================================================================================================
		else
			cout << "Unknown command! For a list of available commands, type \"help\"!\n";
	}

	/** След приключване на работа с програмата освобождаваме динамично заделената памет за масива с имена на дестинации */
	for (ushort i = 0; i < D_CURR; i++)		
	{
		delete[]DESTINATIONS[i];
	}
	delete[]DESTINATIONS;

	return 0;
}