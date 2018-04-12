/**

	@class RawMessage

*/

#ifndef RAWMESSAGE_H
#define RAWMESSAGE_H

#include <TObject.h> // mother class

class RawMessage : public TObject
{
public:
	RawMessage();
	virtual ~RawMessage();

public:

	/**
	 * From the event header
	 */

	/**
	 * From the subevent header
	 */
	Int_t mSubcrate;
	Int_t mControl;
	Short_t mProcID;

	/**
	 * From the subsubevent header
	 */
	Int_t mGeo;
	Int_t mModule;

	/**
	 * From the message
	 */
	Int_t mCh;
	Int_t mVal;

	ClassDef(RawMessage, 1);
};

#endif // RAWMESSAGE_H
