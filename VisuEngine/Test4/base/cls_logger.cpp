#include "cls_logger.h"

cls_logger::cls_logger() :
	mScreenStream(&std::cout)
{
}

cls_logger::~cls_logger()
{
}

/*static*/
cls_logger* cls_logger::Instance(void)
{
	if (!mInstance) {
		mInstance = new cls_logger();
	}
	return mInstance;
}
