#include <list>
//#include <fstream> //work with file
#include <cstring> // заменить
#include <stdio.h>
#include <iostream>

using namespace std;

class image
{
protected:
	char* name;
	FILE *f;
private:
	int file_open ( const char* img );
public:
	image ();
	image (char* img) {
		name = img;
		cout << "image constr\n";
	}

	~image ();
	int check_endl();
	char* get_name() { return name; }
	void reverse (char s[]); //переворачивает строку
};

//просто читает картинку в список
class data_image : public image
{
protected:
	list<string> full_img;
	list<string>::iterator full_itr;
public:
	data_image( char* img ) : image(img){
		full_itr = full_img.begin();
		cout << "data_img_constr\n";
	}

//	friend class pgm_image;

	~data_image( ) {
		full_img.clear(); //clear must free mem from list
		cout << "data_img_destr\n";
	} // itr = NULL??

	int read_img( );
  //	int save_img( ); //Записываем картинку используя дискриптор в список
	//фнкция для определения размеров кртинки.
};

// в этом классе нужно хранить парамтры картинки, которые свойственны 
// только картинке в формате pgm
//class pgm_image : public data_image , public image
class pgm_image : public data_image
{
private:
	string header;

	size_t picture_length;
	size_t picture_width;
	size_t gray_level;

	list<size_t> pixels;
	list<size_t>::iterator pixels_itr;
/*
	friend size_t img_len(pgm_image& obj) {
		return obj.picture_length;
	}

	friend size_t img_wid(pgm_image& obj) {
		return obj.picture_width;
	}

	friend size_t img_gr_lv(pgm_image& obj) {
		return obj.gray_level;
	}

	friend string img_head(pgm_image& obj) {
		return obj.header;
	}

	friend list<size_t> img_pixels(pgm_image& obj) {
		return pixels;
	}

	friend list<size_t>::iterator img_pixels(pgm_image& obj) {
		return pixels_itr;
	}
*/
public:
	pgm_image( char* img ) : data_image(img) {
		picture_length = 0;
		picture_width = 0;
		gray_level = 0;
		pixels_itr = pixels.begin();
		cout << "pgm_img constr\n";
	}

	~pgm_image( ) {
		pixels.clear();
		cout << "pgm_img_destr\n";
	} // itr = NULL??

	int parse_if_pgm (); //хедер картинки храним отдельно

	void lighten ( size_t value ); //осветлить картинку
	void blur ( size_t value ); //затемнить
	int lpf(); //low pass filter

	int save_pgm();
  /*
	size_t img_len( ) {
		return picture_length;
	}

	size_t img_wid( ) {
		return picture_width;
	}

	size_t img_gr_lv( ) {
		return gray_level;
	}

	string img_head( ) {
		return header;
	}

	list<size_t> img_pixels( ) {
		return pixels;
	}

	list<size_t>::iterator img_pixels( ) {
		return pixels_itr;
	}
  */
//нужно написать функции для увеличения и для уменьшения размера картинки
};

