/**

	@class cls_logger

	Most of the code here was taken from the FairLogger class of the
	FairRoot package (https://fairroot.gsi.de/), so, I don't owe it.

*/

#ifndef CLS_LOGGER_H
#define CLS_LOGGER_H

#include <iostream> // mother class

#include <cstring> // for strdup
#include <libgen.h> // for basename

#define IMP_CONVERTTOSTRING(s) # s
#define CONVERTTOSTRING(s) IMP_CONVERTTOSTRING(s)
#define MESSAGE_ORIGIN __FILE__, CONVERTTOSTRING(__LINE__), __FUNCTION__

// p_dir - direction - err or out
#define LOG(p_level) cls_logger::Instance()->GetStream(p_level, MESSAGE_ORIGIN)

enum enuLogLevel {FATAL = 0,
                  ERROR = 1,
                  WARNING = 2,
                  INFO = 3,
                  DEBUG = 4,
                  DEBUG1 = 5,
                  DEBUG2 = 6,
                  DEBUG3 = 7,
                  DEBUG4 = 8};

static const char* const LogLevelString[] = {"FATAL ",
                                             "ERROR ",
                                             "WARN  ",
                                             "INFO  ",
                                             "DEBUG ",
                                             "DEBUG1",
                                             "DEBUG2",
                                             "DEBUG3",
                                             "DEBUG4" };

class cls_logger : public std::ostream
{
public:
	cls_logger& GetStream(enuLogLevel p_level, const char* /*p_file*/, const char* /*p_line*/, const char* /*p_func*/) {

		// Just extract the file name from the full path provided as an argument by the __FILE__ macro
		// This may be useful later on
		/*char* v_file = strdup(p_file);
		char* v_filename = basename(v_file);
		free(v_file); // one has to do that for the result of the strdup()
		*mCurStream << "[" << v_filename << ":" << p_line << "] " << p_func << std::endl;*/

		if (p_level <= mCurLevel) {
			// Currently we stick to the stderr stream for all levels
			// stdout stream is used by flex/bison and I don't know how to suppress their output
			mCurStream = mErrStream; // or mOutStream
			*mCurStream << "[" << LogLevelString[p_level] << "] ";
		} else {
			mCurStream = mNullStream;
		}
		return *this;
	}

	template <class T> cls_logger& operator<<(const T& t) {
		*(cls_logger::Instance()->mCurStream) << t;
		return *this;
	}

    cls_logger& operator<<(std::ios_base & (*manip) (std::ios_base&)) {
		*(cls_logger::Instance()->mCurStream) << manip;
		return *this;
    }

    cls_logger& operator<<(std::ostream & (*manip) (std::ostream&)) {
		*(cls_logger::Instance()->mCurStream) << manip;
		return *this;
    }

	static std::ostream& endl(std::ostream& strm) {
		*(cls_logger::Instance()->mCurStream) << std::endl;
		return strm;
	}

	static void SetLevel(enuLogLevel p_level) { cls_logger::Instance()->mCurLevel = p_level; }

private:
	std::ostream* mErrStream;
	std::ostream* mOutStream;
	std::ostream* mNullStream;

	std::ostream* mCurStream;

	enuLogLevel mCurLevel;

// Singleton management
private:
	cls_logger();
	~cls_logger();
	static cls_logger* mInstance;

public:
	static cls_logger* Instance(void) {
		if (!mInstance) { mInstance = new cls_logger(); }
		return mInstance;
	}

};

#endif // CLS_LOGGER_H
