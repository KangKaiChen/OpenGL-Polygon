#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_File_Chooser.H>
#include "reader.h"
#include "GL/glut.h"
#include <gl/gl.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <FL/Fl_Box.H>
#include <FL/Fl_BMP_Image.H>
#include <time.h>
#define PI 3.1415926

Fl_Box* BMPwindows;
Fl_BMP_Image* image;
Fl_Light_Button* option1 ,*option2;
typedef unsigned char BYTE;
//-------------------------polygonEdit---------------//
class PolygonEdit : public Fl_Gl_Window
{
public:
	int mode, x1, x2, y1, y2, px, py;
	bool close;  // if value =1 than link all point
	bool drag; // if value =1 than display current state is drag

	typedef struct node
	{
		int x, y;
		bool close;   
	}Node;
	std::vector<Node> queue;

	static void Timer_CB(void* userdata);

	PolygonEdit(int x, int y, int w, int h, const char* l = 0);

	void Perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

	void ReshapeViewport();

	void GlInit();

	void Point(int x, int y, float r, float g, float b);//draw the point

	void draw();//judge the condition of point is link or not

	int handle(int event);//the condition of click,drag,release,keyboard event

	bool checkPoint(Node evt, Node org, int similar);
};

void PolygonEdit::Timer_CB(void* userdata)
{
	PolygonEdit* mygl = (PolygonEdit*)userdata;
	mygl->redraw();
	Fl::repeat_timeout(1.0 / 24.0, Timer_CB, userdata);
}

PolygonEdit::PolygonEdit(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l)
{
	close = false;
	drag = false;
	x1 = x2 = y1 = y2 = 0;
	mode = 1;
	px = x;
	py = y;
	Fl::add_timeout(1.0 / 24.0, Timer_CB, (void*)this);
}

void PolygonEdit::Perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xMin, xMax, yMin, yMax;
	yMax = zNear * tan(fovy * 3.1415926 / 360.0);
	yMin = -yMax;
	xMin = yMin * aspect;
	xMax = yMax * aspect;
	glFrustum(xMin, xMax, yMin, yMax, zNear, zFar);
}

void PolygonEdit::ReshapeViewport()
{
	glViewport(0, 0, w(), h());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat ratio = w() / h();
	Perspective(30.0, 1.0 * ratio, 1.0, 30.0);
	glTranslatef(0.0, 0.0, -8.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void PolygonEdit::GlInit()
{
	static int Init = 1;
	if (Init)
	{
		Init = 0;
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
}

void PolygonEdit::Point(int x, int y, float r, float g, float b)//Draw point
{
	glColor3f(r, g, b);
	glVertex2f((x - this->w() / 2) / (float)(this->w() / 2), (this->h() / 2 - y) / (float)(this->h() / 2));
}

void PolygonEdit::draw()//draw the line to connect all the point
{
	if (!mode)
	{
		return;
	}
	if (!valid())
	{
		valid(1);
		GlInit();
		ReshapeViewport();
	}

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glPointSize(5.0f);

	if (queue.size())
	{
		for (int i = 0; i < queue.size(); i++)
		{
			glBegin(GL_POINTS);

			if (queue[i].close)
				Point(queue[i].x, queue[i].y, 1.0, 1.0, 0.0);
			else if (i == 0)
				Point(queue[i].x, queue[i].y, 1.0, 0.0, 0.0);
			else {
				Point(queue[i].x, queue[i].y, 0.0, 1.0, 1.0);

			}
			glEnd();
			if (i == 0)
				continue;
			else {
				glColor3f(1.0, 0, 0);
				glBegin(GL_LINES);
				glVertex2f((queue[i].x - this->w() / 2) / (float)(this->w() / 2), (this->h() / 2 - queue[i].y) / (float)(this->h() / 2));
				glVertex2f((queue[i - 1].x - this->w() / 2) / (float)(this->w() / 2), (this->h() / 2 - queue[i - 1].y) / (float)(this->h() / 2));
				glEnd();
			}
		}
	}

	if (close)
	{
		for (int i = 0; i < queue.size(); i++)
		{
			glColor3f(1.0, 1.0, 1.0);
			glBegin(GL_LINES);
			glVertex2f((queue[i].x - this->w() / 2) / (float)(this->w() / 2), (this->h() / 2 - queue[i].y) / (float)(this->h() / 2));
			if (i + 1 < queue.size())
				glVertex2f((queue[i + 1].x - this->w() / 2) / (float)(this->w() / 2), (this->h() / 2 - queue[i + 1].y) / (float)(this->h() / 2));
			else
				glVertex2f((queue[0].x - this->w() / 2) / (float)(this->w() / 2), (this->h() / 2 - queue[0].y) / (float)(this->h() / 2));
			glEnd();
		}
	}
}

int PolygonEdit::handle(int event)//the condition of different case
{
	switch (event)
	{
	case FL_PUSH:
		if (mode == 0)
		{
			x1 = Fl::event_x();
			y1 = Fl::event_y();
			return 1;
		}
		printf("Click(%d,%d)\n", Fl::event_x(), Fl::event_y());
		Node evt;
		evt.x = x1 = Fl::event_x();
		evt.y = y1 = Fl::event_y();
		evt.close = false;
		if (!close)
			queue.push_back(evt);
		else
		{
			if (queue.size() != 0)
				for (int i = 0, one = 1; i < queue.size(); i++)
				{
					if (one && checkPoint(evt, queue[i], 2))
					{
						queue[i].close = true;
						one = 0;
						continue;
					}
					queue[i].close = false;
				}
		}
		return 1;
	case FL_DRAG:
		if (mode == 0)
		{
			x2 = Fl::event_x();
			y2 = Fl::event_y();
			px = px + (x2 - x1);
			py = py + (y2 - y1);
			this->position(px, py);
			return 1;
		}

		if (close)
		{
			printf("Drag(%d,%d)\n", Fl::event_x(), Fl::event_y());
			for (int i = 0; i < queue.size(); i++)
				if (queue[i].close)
				{
					queue[i].x = Fl::event_x();
					queue[i].y = Fl::event_y();
					drag= true;
					break;
				}
		}
		return 1;
	case FL_RELEASE:
		if (drag && close)
		{
			printf("Release(%d,%d)\n", Fl::event_x(), Fl::event_y());
			for (int i = 0; i < queue.size(); i++)
				if (queue[i].close)
				{
					queue[i].close = false;
					break;
				}
			drag = false;
		}
		return 1;
	case FL_KEYBOARD:
	case FL_SHORTCUT:
		if (Fl::event_key() == 'c')
		{
			for (int i = 0; i < queue.size(); i++)
				printf("(%d,%d) ", queue[i].x, queue[i].y);
			printf("\n");
			close = true;
			this->redraw();
		}
		else if (Fl::event_key() == 'd')
		{
			for (int i = 0; i < queue.size(); i++)
				if (queue[i].close)
				{
					queue.erase(queue.begin() + i);
					if (queue.size() == 0)
						close = false;
					break;
				}
		}
		return 1;
	default:
		return Fl_Gl_Window::handle(event);
	}
}

bool PolygonEdit::checkPoint(Node evt, Node org, int similar)
{
	for (int x = -similar; x <= similar; x++)
		for (int y = -similar; y <= similar; y++)
		{
			if (evt.x + x == org.x && evt.y + y == org.y)
				return true;
		}
	return false;
}

//----------------------------------GeneratePolygons--------------------------------------//
class GeneratePolygons : public Fl_Gl_Window
{
public:
	int mode, x1, x2, y1, y2, px, py;

	typedef struct node
	{
		float x, y;
	}Node;

	typedef struct shape
	{
		std::vector<Node> Gpoly;
		float r, g, b;
		int R;
		float offsetx,offsety;
		float mx,my;

	}Shape;

	std::vector<Shape> polygons;

	static void Timer_CB(void* userdata);

	GeneratePolygons(int x, int y, int w, int h, const char* l = 0);

	void ReshapeViewport();

	void GlInit();

	void draw();//draw the different polygons

	int handle(int event);//the condition of keyboard event
};

void GeneratePolygons::Timer_CB(void* userdata)
{
	GeneratePolygons* GPolygons = (GeneratePolygons*)userdata;
	GPolygons->redraw();
	Fl::repeat_timeout(1.0 / 24.0, Timer_CB, userdata);
}

GeneratePolygons::GeneratePolygons(int x, int y, int w, int h, const char* l) : Fl_Gl_Window(x, y, w, h, l)
{
	srand(time(NULL));
	x1 = x2 = y1 = y2 = 0;
	mode = 1;
	px = x;
	py = y;
	Fl::add_timeout(1.0 / 24.0, Timer_CB, (void*)this);
}

void GeneratePolygons::ReshapeViewport()
{
	glViewport(0, 0, w(), h());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(-10, 10, -10, 10);
}

void GeneratePolygons::GlInit()
{
	static int first_time = 1;
	if (first_time)
	{
		first_time = 0;
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}
}

void GeneratePolygons::draw()
{	
	int i = 0, n = 3;
	if (!valid())
	{
		valid(1);
		GlInit();
		ReshapeViewport();
	}
	glClearColor(0, 0, 0, 0);//將先前儲存的color數值buffer清除掉
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (polygons.size() != 0)
	{

		for (int i = 0; i < polygons.size(); i++)
		{
			float D = 10.4;
			for (int k = 0; k < polygons[i].Gpoly.size(); k++)
			{
				float x = polygons[i].Gpoly[k].x + polygons[i].mx;
				float y = polygons[i].Gpoly[k].y + polygons[i].my;
				if (x > D || x < -D)
				{
					polygons[i].mx = -polygons[i].mx;
				}
				else if (y > D || y < -D)
				{
					polygons[i].my = -polygons[i].my;
				}
			}
			glPushMatrix();
			glColor3f(polygons[i].r, polygons[i].g, polygons[i].b);
			glBegin(GL_POLYGON);
			for (int j = 0; j < polygons[i].Gpoly.size(); j++)
			{
				glVertex2f(polygons[i].Gpoly[j].x , polygons[i].Gpoly[j].y );
				polygons[i].Gpoly[j].x += polygons[i].mx;
				polygons[i].Gpoly[j].y += polygons[i].my;
			}
			glEnd();
		}
	}
}
int GeneratePolygons::handle(int event)
{
	switch (event)
	{
		case FL_KEYBOARD:
		case FL_SHORTCUT:
		{
			if (Fl::event_key() == 'c' || Fl::event_key() == 'C')//使用C鍵原因發想為"create"
			{
				float r = 1.0f / (rand() % 10 + 1) + 0.3, g = 1.0f / (rand() % 10 + 1) + 0.3, b = 1.0f / (rand() % 10 + 1) + 0.3;
				double max = 7,min = -7;
				double x = (max - min) * rand() / (RAND_MAX + 1.0) + min;//改變多邊形初始X軸位置
				double y = (max - min) * rand() / (RAND_MAX + 1.0) + min;//改變多邊形初始Y軸位置
				float R = (rand() % 5 + 1);//多邊形半徑長
				int n = rand() % 10 + 3;//亂數定義多邊形
				float mx = (double)rand() * 2 / RAND_MAX + (-1);//change the move of axis x 
				float my = (double)rand() * 2 / RAND_MAX + (-1);//change the move of axis y
				printf("r=%.2f,g=%.2f,b=%.2f,x=%.2f,y=%.2f,R=%.2f,n=%d,mx=%.2f,my=%.2f\n", r, g, b, x, y, R, n,mx,my);

				shape parameter;
				parameter.r = r;
				parameter.g = g;
				parameter.b = b;
				parameter.R = R;
				parameter.offsetx = x;
				parameter.offsety = y;
				parameter.mx = mx;
				parameter.my = my;

				for (int i = 1; i <= n; i++)
				{
					Node polygons;
					polygons.x = R * cos(2 * PI / n * i)+x;
					polygons.y = R * sin(2 * PI / n * i)+y;
					parameter.Gpoly.push_back(polygons);
				}
				polygons.push_back(parameter);
			}
			else if ( Fl::event_key() == 'r')//按下R鍵可加深多邊形三元素色中r值
			{
				for (int i = 0; i < polygons.size(); i++)
				{
					polygons[i].r += 0.05;
					printf("r=%.2f\n", polygons[i].r);
				}
			}
			else if (Fl::event_key() == 'e')//按下e鍵可遞減多邊形三元素色中的r值
			{
				for (int i = 0; i < polygons.size(); i++)
				{
					polygons[i].r -= 0.05;
					printf("r=%.2f\n", polygons[i].r);
				}
			}
			else if ( Fl::event_key() == 'g')//按下G鍵可加深多邊形三元素色中的g值
			{
				for (int i = 0; i < polygons.size(); i++)
				{
					polygons[i].g += 0.05;
					printf("g=%.2f\n", polygons[i].g);
				}
			}
			else if (Fl::event_key() == 'f')//按下f鍵可遞減多邊形三元素色中的g值
			{
				for (int i = 0; i < polygons.size(); i++)
				{
					polygons[i].g -= 0.05;
					printf("g=%.2f\n", polygons[i].g);
				}
			}
			else if ( Fl::event_key() == 'b')//按下B鍵可加深多邊形三元素色中的b值
			{
				for (int i = 0; i < polygons.size(); i++)
				{
					polygons[i].b += 0.05;
					printf("b=%.2f\n", polygons[i].b);
				}
			}
			else if (Fl::event_key() == 'v')//按下B鍵可遞減多邊形三元素色中的b值
			{
				for (int i = 0; i < polygons.size(); i++)
				{
					polygons[i].b -= 0.05;
					printf("b=%.2f\n", polygons[i].b);
				}
			}
			else if (Fl::event_key() == 'd' || Fl::event_key() == 'D')//按下D鍵可刪除當前產生的多邊形
			{
					polygons.pop_back();
			}
		}
		return 1;
		default:
			return Fl_Gl_Window::handle(event);
	}
}

//----------------------------------------------------------------------------------------//
class openGL_window : public Fl_Gl_Window { // Create a OpenGL class in FLTK 
	void draw();            // Draw function. 
	void draw_overlay();    // Draw overlay function. 

	static void Timer_CB(void* userdata) {
		openGL_window* pb = (openGL_window*)userdata;
		pb->redraw();
		Fl::repeat_timeout(1.0 / 24.0, Timer_CB, userdata);
	}

public:
	openGL_window(int x, int y, int w, int h, const char* l = 0);  // Class constructor 
	int frame;
};

openGL_window::openGL_window(int x, int y, int w, int h, const char* l) :
	Fl_Gl_Window(x, y, w, h, l)
{
	mode(FL_RGB | FL_ALPHA | FL_DOUBLE | FL_STENCIL);
	Fl::add_timeout(1.0 / 24.0, Timer_CB, (void*)this);
	frame = 0;
}

void openGL_window::draw() {
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
	if (!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0, 0, w(), h());
	}

	// draw an amazing but slow graphic:--------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0); glVertex2f(-1, -1);
	glColor3f(1, 0, 0); glVertex2f(-1, 1);
	glColor3f(1, 0, 0); glVertex2f(1, 1);
	glColor3f(1, 0, 0); glVertex2f(1, -1);
	glEnd();
	//--------------------------------------------------
	++frame;
}

void openGL_window::draw_overlay() {
	// the valid() property may be used to avoid reinitializing your
	// GL transformation for each redraw:
	if (
		!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0, 0, w(), h());
	}
	// draw an amazing graphic:-------------

	//---------------------------------------
}
PolygonEdit* PE;
GeneratePolygons* GP;
//-------Button callback funtion
void quit(Fl_Widget*, void*)
{
	exit(0);
}

void buta(Fl_Widget*, void*)
{
	option1->value(1);
	PE->show();
	GP->hide();
	option2->value(0);

}

void butb(Fl_Widget*, void*)
{
	GP->show();
	PE->hide();
	option2->value(1);
	option1->value(0);

}
//


//  main function
int main(int argc, char** argv) {

	int width, heith;
	BYTE* img = (BYTE*)malloc(sizeof(BYTE*));
	BMP_Read("pepper.bmp", &img, width, heith);

	Fl_Window window(800, 500,"HW1");  // Create a FLTK window. Resolution 800*500. 

	image = new Fl_BMP_Image("peppers.bmp");//load "peppers.bmp" in FLTK windows
	BMPwindows = new Fl_Box(420, 30, 300, 300);
	BMPwindows->image(image);
	BMPwindows->redraw();

	option1 = new Fl_Light_Button(10, 350, 120, 30, "PolygonEdit");
	option1->labelsize(12);
	option1->callback(buta);
	option1->selection_color(FL_RED);
	
	option2 = new Fl_Light_Button(150, 350, 120, 30, "GeneratePolygons");
	option2->labelsize(12);
	option2->callback(butb);
	option2->selection_color(FL_BLACK);

	PE = new PolygonEdit(10, 30, 300, 300);//the function of PolygonEdit		
	GP = new GeneratePolygons(10, 30, 300, 300);//the function of GeneratePolygons
	
	GP->hide();


	Fl_Button* but = new Fl_Button(30, 450, 80, 30, "Quit");//the function of exit
	but->callback(quit);

	window.end();                  // End of FLTK windows setting. 
	window.show(argc, argv);        // Show the FLTK window
	return Fl::run();
}