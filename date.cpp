#include <iostream>
#include "date.hpp"

bool Date::isLeap(ushort y)
{
	if (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
		return true;
	return false;
}

bool Date::isValid()
{
	if (year<1000||year>9999)
	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > 31)
		return false;

	if (month == 2)
	{
		if (isLeap(year))
			return (day <= 29);
		else
			return (day <= 28);
	}

	if (month == 4 || month == 6 || month == 9 || month == 11)
		return (day <= 30);

	return true;
}

Date& Date::operator=(const Date& d)
{
	if (this != &d)
	{
		year = d.year;
		month = d.month;
		day = d.day;
	}
	return *this;
}

bool operator<=(const Date& d1, const Date& d2)
{
	if (d1.year < d2.year)
		return true;
	else if (d1.year == d2.year)
	{
		if (d1.month < d2.month)
			return true;
		else if (d1.month == d2.month)
		{
			if (d1.day <= d2.day)
				return true;
		}
	}
	return false;
}

bool operator>(const Date& d1, const Date& d2)
{
	if (d1.year > d2.year)
		return true;
	else if (d1.year == d2.year)
	{
		if (d1.month > d2.month)
			return true;
		else if (d1.month == d2.month)
		{
			if (d1.day > d2.day)
				return true;
		}
	}
	return false;
}