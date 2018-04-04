/**

	@class cls_stl_interface

**/

#ifndef CLS_STL_INTERFACE_H
#define CLS_STL_INTERFACE_H

class cls_stl_file;

class cls_stl_interface
{
private:
	cls_stl_interface();
	~cls_stl_interface();

public:
	static cls_stl_file* Import(const char* p_filename);

};

#endif // CLS_STL_INTERFACE_H
