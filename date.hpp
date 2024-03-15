#ifndef __DATE_HPP
#define __DATE_HPP

#include <iostream>

typedef unsigned short ushort;

/*! Клас дата */

class Date				
{						
	ushort year;		/**< Година */
	ushort month;		/**< Месец */
	ushort day;			/**< Ден */

public:
	/**@name Голяма четворка */
	///@{
	Date(ushort y = 2020, ushort m = 1, ushort d = 1) :year(y), month(m), day(d) {}		
	Date(const Date& d) :year(d.year), month(d.month), day(d.day) {}
	~Date() {}
	Date& operator=(const Date&);
	///@}
	/**@name get-функции */
	///@{
	const ushort getYear() { return year; }			
	const ushort getMonth() { return month; }
	const ushort getDay(){ return day; }
	///@}

	/**@name set-функции */
	///@{
	void setYear(const ushort y) { year = y; }		
	void setMonth(const ushort m) { month = m; }
	void setDay(const ushort d) { day = d; }
	///@}

	bool isLeap(ushort y);							//!< Проверява дали годината е високосна
	bool isValid();									//!< Проверява дали датата е валидна

	/**@name Предефинирани операции за сравнение*/
	///@{
	friend bool operator<=(const Date&, const Date&);		
	friend bool operator>(const Date&, const Date&);
	///@}

	friend std::ostream& operator<<(std::ostream& o, const Date& d)		/** Предефинирана операция за изход. Извежда датата по формат ISO 8601 */
	{
		return o << d.year << "-" << d.month << "-" << d.day;
	}
};

#endif