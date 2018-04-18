/**

	@class cls_aneu_interface

**/

#ifndef CLS_ANEU_INTERFACE_H
#define CLS_ANEU_INTERFACE_H

class cls_aneu_file;

class cls_aneu_interface
{
private:
	cls_aneu_interface();
	~cls_aneu_interface();

public:
	static cls_aneu_file* Import(const char* p_filename);

};

#endif // CLS_ANEU_INTERFACE_H
