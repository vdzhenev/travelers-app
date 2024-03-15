#ifndef __USER_HPP
#define __USER_HPP

#include <iostream>
#include "trip.hpp"

/*! Клас потребител */

struct User					
{
	char* name;				///< Потребителско име
	char* password;			///< Парола
	char* email;			///< Имейл
	Trip* trips;			///< Масив от пътувания
	ushort t_curr = 0;		///< Брой извършени пътувания
	ushort t_max = 0;		///< Максимален брой пътувания
	char** friends;			///< Масив, съдържащ имената на приятелите на потребителя
	ushort f_curr = 0;		///< Текущ брой приятели
	ushort f_max = 0;		///< Максимален брой приятели

	/** @name Голяма четворка
	* Използват set-функциите 
	*/
	///@{
	User(const char* n = "<username>" , const char* p = "<password>", const char* e = "<email>");		
	User(const User&);
	User& operator=(const User&);
	~User();
	///@}

	/** @name set-функции 
	*	Предварително освобождават текущо заделената памет
	*/
	///@{
	/** задаване на име по даден низ */
	void setName(const char*);
	/** задаване на парола */
	void setPass(const char*);
	/** задаване на имейл */
	void setEmail(const char*);	
	/** задаване на пътувания от друг потребител */
	void setTrips(const User&);		
	/** задаване на приятели от друг потребител */
	void setFriends(const User&);		
	///@}
	
	void addTrip(const Trip&);		//!< добавяне на пътуване
	void addFriend(const char*);	//!< добавяне на приятел


	friend std::ostream& operator<<(std::ostream& o, const User& u);		//!< предефинирана операция за изход
};

#endif