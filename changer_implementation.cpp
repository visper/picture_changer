#include "changer_announcement.hpp"
#include <errno.h>
#include <sstream>
#include <assert.h>
#include <fstream>

using namespace std;

int image::file_open ( const char* img)
{
	if ( (f = fopen( img, "r+" )) == NULL)
		return -1;
	else 
		return 0;
}

//default consturctor must create error
image::image () { 
	perror("name of image isn`t write");
	//is it correct ^
}

//arg is name of image whose open from dir
int image::check_endl () { // "night_city.pgm"
	//проверка на то чтобы концовка имени была .pgm
	size_t name_len = strlen(name);
	name_len++;
	reverse(name); // переворачиваем строку

	char* end = new char[5]; // переменная для копирования расширения
	memcpy (end, name, 4); // копируем расширение в перевернутом виде
	reverse(name);//обратно

	reverse(end); // переворачиваем расширение
	end[strlen(end)] = '\0';

	const char* pgm = ".pgm\0";
//	cout << "pgm is: " << pgm << " end of file is: " << end << endl;

	int rc = strncmp(end, name, 4);

	delete[] end;
	end = NULL;

	//need to delete pgm
//	delete[] pgm;
//	delete const_cast<char*>(pgm);
	pgm = NULL;
       
	if (!rc) { //расширение не подходит
		return -1;
		//требуется вызов деструктора
	}
	else { // //расширение подходит
		//open image for read and write
		int rc = 0;
		if ( rc = file_open( name ))
			return -1; // error
		else
			return 0;  //success
	}
}

image::~image () {
	//close discriptor of image
	errno = 0;

	if ( f != NULL && f > 0) { //дескриптор отобраэает файл
		if (fclose(f) != 0) { // неудачное закрытие, в errno ошибка
			//must be try
			cout << "errno is: " << errno << endl;
		}
	}
	name = NULL;
	cout << "image destr\n";
}

void image::reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int data_image::read_img()
{
	char *line = NULL;
	ssize_t read = 0;
	size_t len = 0;

	if (f <= 0)
		return -1;//must be better solution

	while ( (read = getline(&line, &len, f)) != -1 ) {
		//пишем в список строки
		string str(line); //convert char* to string
		full_img.push_back (str);
		line = NULL;
		//надо проверить getline
	}
	return 0;
}

/*
int data_image::save_img ( )
{
	list<string> img_w;
	list<string>::iterator img_w_itr;

	//	img_w.push_back(this->image::pgm_image::img_head());
	img_w.push_back(*this->image::img_head());

	string pic_len(img_len());
	string pic_wid(img_wid());
	string summ_len_wid = pic_len + ' ' + pic_wid;
	img_w.push_back(summ_len_wid);

	string gray_l(img_gr_lv());
	img_w.push_back(gray_l);

	for(pixels_itr=pixels.begin(); pixels_itr!=pixels.end(); pixels_itr++){
		string a(*pixels_itr);
		img_w.push_back(a);
	}

	img_w.clear();
//	img_w_itr = NULL;
}
*/

int pgm_image::save_pgm()
{
	ofstream fout;
	fout.open("new_pgm.pgm");

//	cout << header.c_str();

	fout << header.c_str();
//	fout << "#new pgm image";

	ostringstream ostr;
	ostr << picture_width << " " << picture_length;
	string temp_str = ostr.str();
	fout << temp_str.c_str();

//	cout << temp_str.c_str() << endl;
	ostr.str("");
	fout << endl;

	ostr << gray_level;
	temp_str = ostr.str();
	fout << temp_str.c_str();
	ostr.str("");
	fout << endl;

//	cout << temp_str.c_str();
/*
	for(pixels_itr=pixels.begin(); pixels_itr!=pixels.end(); pixels_itr++){
		ostr << *pixels_itr;
		temp_str = ostr.str();
		cout << temp_str.c_str() << endl;
		ostr.str("");
	}
*/

	for(pixels_itr=pixels.begin(); pixels_itr!=pixels.end(); pixels_itr++){
		ostr << *pixels_itr;
		temp_str = ostr.str();
//		string a(*pixels_itr);
//		cout << temp_str.c_str() << endl;
		fout << temp_str.c_str();
		ostr.str("");
		fout << endl;
	}

//	fclose(pgm_f);

	fout.close();
	return 0;
}

int pgm_image::parse_if_pgm()
{
	full_itr = full_img.begin();
	string p5 = "P5\n";
	string p2 = "P2\n";

	if (*full_itr == p2) {
		header = p2;
	}
	else if (*full_itr == p5) {
		header = p5;
	}
	else { //write into header
//		cout << "test_else\n";
		//try
		//вызов деструктора
		perror("error pgn. wrong image\n");
	}

	//Line 1 - The version,           already write  
	//Line 2 - The comment,           not count
	//Line 3 - The width and height,                  1
	//Line 4 - The gray level,                        2
	//Line 5 to end - The pixels,     not counted     3

	size_t counter = 1; //need to read width and heigth

	full_itr++;

	for ( ; full_itr != full_img.end(); full_itr++) {
		//translate from string to char array
		string temp = *full_itr;
		char ctemp[32];
		strncpy(ctemp, temp.c_str(), sizeof(ctemp));
		ctemp[sizeof(ctemp) - 1] = 0;

		//comment
		if (ctemp[0] == '#') { 
			continue;
		}
		else {
			errno = 0;
			if (counter < 3) { //header isn`t read
				if (counter == 1 && 2 == sscanf(ctemp,"%zu %zu",
						&picture_width, 
						&picture_length)) {
					//read length and width
					counter++;
					if (errno != 0) {
						perror("scanf");
						return -1;
					}
					continue;
				}

				errno = 0;
				if (counter==2 && 1==sscanf(ctemp, "%zu", 
							    &gray_level)){
					//read gray_level
					counter++;
					if (errno != 0) {
						perror("scanf");
						return -1;
					}
					continue;
				}
			}
			else {
				//read pixels
				errno = 0;
				size_t temp_pixel = 0;
				if (counter==3 && 
				    1==sscanf(ctemp, "%zu", &temp_pixel)){
					pixels.push_back(temp_pixel);
					//cout << "test\n";
					pixels_itr++;
					if (errno != 0) {
						perror("scanf");
						return -1;
					}
					continue;
				}
			}
		}

	}
/*
	cout << "pixels:\n";
	for (pixels_itr=pixels.begin(); pixels_itr!=pixels.end();
	     pixels_itr++){
		size_t temp_pixel = *pixels_itr;
		cout << temp_pixel;
	}
	cout << endl;
*/
	return 0;
}

//+
void pgm_image::lighten ( size_t value ) //осветлить картинку
{
	for (pixels_itr = pixels.begin(); pixels_itr != pixels.end();
	     pixels_itr++) {
		if (*pixels_itr+value >= gray_level) {
			*pixels_itr = gray_level;
		}
		else {
			*pixels_itr += value;
		}
	}
}

//-
void pgm_image::blur ( size_t value ) //затемнить
{
	for (pixels_itr = pixels.begin(); pixels_itr != pixels.end();
	     pixels_itr++) {
		//*pixels_itr-value > 255 if negative number it will be more
		if (*pixels_itr-value <= 0 || *pixels_itr-value > 255) {
			*pixels_itr = 0;
		}
		else {
			*pixels_itr -= value;
		}
	}
}

int main(int argc, char** argv)
{
  //enum { CITY = "night_city.pgm", PLANE = "plane.pgm", T = "T.pgm" };
//	image img();
//	image image("night_city.pgm");
	//image image(argv[1]);

	if (argv[1] == NULL) {
		cout << "error. argument is not write!\n";
		//call destructor
		return -1;
	}

//	data_image img(argv[1]);
	pgm_image img(argv[1]);

	int rr = img.data_image::check_endl();

	if (rr != 0)
		return -1;

	img.read_img();

	rr = img.parse_if_pgm();
	//void data_image::read_img()

//	img.lighten(50);
	img.blur(10);

	rr = img.save_pgm();
	assert(rr == 0);


	return 0;
}
