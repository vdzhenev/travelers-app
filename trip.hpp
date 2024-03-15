#ifndef __TRIP_HPP
#define __TRIP_HPP

#include <iostream>
#include "date.hpp"

/*! Клас пътуване */

class Trip					
{
	char* dest;				///< Съдържа име на дестинацията
	Date period[2];			///< Масив от две дати - начало и край на пътуването
	ushort grade;			///< Оценка от потребител
	char* comment;			///< Коментар от потребител
	char* photos;			///< Снимки

public:
	/**@name Голяма четворка */
	///@{
	Trip(const char* d = "-", ushort g = 1, const char* c = "-", const char* p = "-");		
	Trip(const Trip&);
	Trip& operator=(const Trip&);
	~Trip();
	///@}

	/**@name get-функции */
	///@{
	const char* getDest(){ return dest; }			
	Date getDateFrom(){ return period[0]; }
	Date getDateTo(){ return period[1]; }
	const ushort getGrade(){ return grade; }
	const char* getComment(){ return comment; }
	const char* getPhotos(){ return photos; }
	///@}

	/**@name set-функции 
	*	Предварително освобождават динамично заделената памет (където е необходимо)
	*/
	///@{
	void setDest(const char*);						
	void setGrade(const ushort g){ grade = g; }
	void setComment(const char*);
	void setPhotos(const char*);
	void setPeriod(const Date&, const Date&);
	///@}
	friend std::ostream& operator<<(std::ostream& o, const Trip& t);		//!< Предефинирана операция за изход
};


#endif