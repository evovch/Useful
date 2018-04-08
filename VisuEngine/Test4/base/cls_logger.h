/**

	@class cls_logger

*/

#ifndef CLS_LOGGER_H
#define CLS_LOGGER_H

#include <iostream> // mother class

class cls_logger : public std::ostream
{
public:
	cls_logger();
	~cls_logger();

	template <class T> cls_logger& operator<<(const T& t) {
		*(mScreenStream) << t;
		return *this;
	}

	std::ostream* mScreenStream;

	static cls_logger* GetLogger(void) { return cls_logger::Instance(); }
	static cls_logger* Instance(void);

private:
	static cls_logger* mInstance;

};

#endif // CLS_LOGGER_H
