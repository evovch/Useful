#include "cls_logger.h"

/*static*/ cls_logger* cls_logger::mInstance = nullptr;

cls_logger::cls_logger() :
	mErrStream(&std::cerr),
	mOutStream(&std::cout),
	mNullStream(new std::ostream(0)),
	mCurStream(&std::cerr),
	mCurLevel(INFO)
{
}

cls_logger::~cls_logger()
{
	LOG(DEBUG) << "Destructing the cls_logger object." << cls_logger::endl;
}
