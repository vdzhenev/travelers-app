#include <iostream>
#include "trip.hpp"


Trip::Trip(const char* d, ushort g, const char* c,const char* p) :dest(nullptr), comment(nullptr), photos(nullptr)
{
	grade = g;
	setDest(d);
	setComment(c);
	setPhotos(p);
}

Trip::Trip(const Trip& t)
{
	setDest(t.dest);
	setPeriod(t.period[0], t.period[1]);
	grade = t.grade;
	setComment(t.comment);
	setPhotos(t.photos);
}

Trip& Trip::operator=(const Trip& t)
{
	if (this != &t)
	{
		setDest(t.dest);
		grade = t.grade;
		setPeriod(t.period[0], t.period[1]);
		setComment(t.comment);
		setPhotos(t.photos);
	}
	return *this;
}

Trip::~Trip()			/** Освобождаваме динамично заделената памет */
{
	delete[] dest;
	delete[] comment;
	delete[] photos;
}

void Trip::setDest(const char* n)		/** Освобождава паметта от текущата дестинация и динамично заделя памет за нова */
{
	delete[] dest;
	unsigned size = strlen(n);
	dest = new char[size + 1];
	for (unsigned i = 0; i < size; i++)
	{
		dest[i] = *(n + i);
	}
	dest[size] = '\0';
}

void Trip::setComment(const char* c)		/** Освобождава паметта от текущия коментар и динамично заделя памет за нов */
{
	delete[] comment;
	unsigned size = strlen(c);
	comment = new char[size + 1];
	for (unsigned i = 0; i < size; i++)
	{
		comment[i] = *(c + i);
	}
	comment[size] = '\0';
}

void Trip::setPhotos(const char* p)		/** Освобождава паметта от текущите снимки и динамично заделя памет за нови */
{ 
	delete[] photos;
	unsigned size = strlen(p);
	photos = new char[size + 1];
	for (unsigned i = 0; i < size; i++)
	{
		photos[i] = *(p + i);
	}
	photos[size] = '\0';
}

void Trip::setPeriod(const Date& d1, const Date& d2)		/** Задава период по подадени две дати */
{
	period[0] = d1;
	period[1] = d2;	
}

std::ostream& operator<<(std::ostream& o, const Trip& t)		/** Извежда името на дестинацията, периода на посещение, оценка, коментар и снимки */
{
	o << "Destination:\n" << t.dest << "\nTime period:\n" << t.period[0] << std::endl << t.period[1]
		<< "\nGrade:\n" << t.grade << "\nComment:\n" << t.comment<<"\nPhotos:\n";
	for (unsigned i = 0; t.photos[i] != '\0'; i++)
	{
		if (t.photos[i] == ' ')
			o << std::endl;
		else
			o << t.photos[i];
	}
	return o;
}