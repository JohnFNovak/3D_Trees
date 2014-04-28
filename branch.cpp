/*
* OpenGLSamples (openglsamples.sf.net) Examples
* VC++ users should create a Win32 Console project and link 
* the program with glut32.lib, glu32.lib, opengl32.lib
*
* GLUT can be downloaded from http: //www.xmission.com/~nate/glut.html
* OpenGL is by default installed on your system.
* For an installation of glut on windows for MS Visual Studio 2010 see: http:   //nafsadh.wordpress.com/2010/08/20/glut-in-ms-visual-studio-2010-msvs10/
*
*/

#include <cstdlib>
#include <iostream>
#include <OpenGL/gl.h>          // Open Graphics Library (OpenGL) header
#include <OpenGL/glu.h>         // Open Graphics Library Utilities header
#include <GLUT/glut.h>      // The GL Utility Toolkit (GLUT) Header
#include <GLKit/GLKMath.h>
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>


#define KEY_ESCAPE 27
#define pi 3.1415926536

// using namespace std;

float g_rotation = 0;
float g_rotation_speed = 0.2f;

float X2, Y2, Z2, X3, Y3, Z3;
float distance, theta, phi;
bool Paused, Pointer, Verbose;
float AutoZoom;

float Sin(float);
float Asin(float);
float Cos(float);
float Acos(float);
float Tan(float);
float Atan(float);

// gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);

float Sin(float x) { return sin(x * pi / 180); }
float Asin(float x) { return asin(x) / pi * 180; }
float Cos(float x) { return cos(x * pi / 180); }
float Acos(float x) { return acos(x) / pi * 180; }
float Tan(float x) { return tan(x * pi / 180); }
float Atan(float x) { return atan(x) / pi * 180; }

typedef struct {
    int width;
    int height;
    char* title;

    float field_of_view_angle;
    float z_near;
    float z_far;
} glutWindow;

class branch {
    public:
        branch();
        branch(branch*);
        ~branch();
        void grow(void);
        void grow(int);
        void bud(void);
        void draw();
        void rotate(double, double);
        void drawSelf(void);
        void getBase(void);
        GLUquadricObj *myCylinder;
        branch* parent;
        std::vector<branch> children;
        double radius;
        double length;
        double X, Y, Z;
        double B_theta, B_phi, r_phi;
    private:
};

// std::vector<branch*> branches;

branch::branch()
{
    parent = 0;
    // id = 0;
    radius = 0.015;
    length = 0.1;
    X = 0;
    Y = 0;
    Z = 0;
    B_theta = 0;
    B_phi = 0;
    r_phi = 0;
    myCylinder = gluNewQuadric();
    children.resize(0);
}

branch::branch(branch* x)
{
    parent = x;
    // id = parent->id;
    radius = 0.015;
    length = 0.1;
    // These get set with getBase()
    X = 0;
    Y = 0;
    Z = 0;
    B_theta = 0;
    B_phi = 0;
    r_phi = 0;
    myCylinder = gluNewQuadric();
    children.resize(0);
    getBase();
}

branch::~branch()
{
}

void branch::draw()
{
    drawSelf();
    // draw the children
    if (children.size() > 0) {
        for ( unsigned int i = 0; i < children.size(); i++ ){
            children[i].draw();
        }
    }
}

void branch::drawSelf()
{
    glPushMatrix();                   
    // glColor3f(1,0,0);

    glTranslatef(X,Y,Z);
    glRotatef(-B_theta, Cos(B_phi), Sin(B_phi),0);
    // glRotatef(-B_theta, X_AXIS, Y_AXIS, 0);

    glColor3f(1,0,0);

    gluCylinder(myCylinder, radius, radius * 0.8, length * 1.1, 20, 200);
    // gluQuadricNormals(myCylinder, GL_SMOOTH);
    // gluQuadricTexture(myCylinder, GL_TRUE);

    glPopMatrix();                      
}

void branch::grow(void)
{
    if(parent != 0){
        // Y = parent->Y + parent->length;
        getBase();
    }
    radius += 0.0001;
    length += 0.001;
    for ( unsigned int i = 0; i < children.size(); i++ ){
        children[i].grow();
    }
}

void branch::grow(int n)
{
    for(int i=0; i<n; i++){
        grow();
    }
}

void branch::bud(void)
{
    if (children.size() == 0) {
        double n_theta;
        double n_phi;
        n_theta = 25;
        n_phi = rand() % 180;
        // n_phi = 30;
        branch child1(this);
        child1.rotate(n_theta, n_phi);
        children.push_back(child1);
        branch child2(this);
        child2.rotate(-1 * n_theta, n_phi);
        children.push_back(child2);
        // std::cout << &child1 << " " << &child2 << std::endl; std::cout.flush();
        // branches.push_back(&child1);
        // branches.push_back(&child2);
    } else {   
        for ( unsigned int i = 0; i < children.size(); i++ ){
            children[i].bud();
        }
    } 
}

void branch::rotate(double A, double B)
{
    float vx, vy, vz, Ax, Ay, Az, Bx, By, Bz, rx, ry, rz, l;
    r_phi = B;
    vx = Sin(parent->B_theta) * Cos(parent->B_phi);
    vy = Sin(parent->B_theta) * Sin(parent->B_phi);
    vz = Cos(parent->B_theta);

    if (parent->B_theta == 0) {
        Ax = 1;
        Ay = 0;
        Az = 0;
        Bx = 0;
        By = 1;
        Bz = 0;
    }
    else if (parent->B_phi == 0) {
        Ax = 1;
        Ay = 0;
        Az = 0;
        Bx = 0;
        By = 1;
        Bz = 0;
    }
    else if (parent->B_phi == 90) {
        Ax = 0;
        Ay = 1;
        Az = 0;
        Bx = -1;
        By = 0;
        Bz = 0;
    }
    else if (parent->B_phi == 180) {
        Ax = -1;
        Ay = 0;
        Az = 0;
        Bx = 0;
        By = -1;
        Bz = 0;
    }
    else if (parent->B_phi == 270) {
        Ax = 0;
        Ay = -1;
        Az = 0;
        Bx = 1;
        By = 0;
        Bz = 0;
    }
    else {
        Ax = vz / sqrt((vx * vx) + (vz * vz));
        Ay = 0;
        Az = -vx / sqrt((vx * vx) + (vz * vz));
        Bx = 0;
        By = vz / sqrt((vy * vy) + (vz * vz));
        Bz = -vy / sqrt((vx * vx) + (vz * vz));
    }

    B += parent->r_phi;

    rx = (vx * Cos(A)) + (Sin(A) * Cos(B) * Ax) + (Sin(A) * Sin(B) * Bx);
    ry = (vy * Cos(A)) + (Sin(A) * Cos(B) * Ay) + (Sin(A) * Sin(B) * By);
    rz = (vz * Cos(A)) + (Sin(A) * Cos(B) * Az) + (Sin(A) * Sin(B) * Bz);

    l = sqrt((rx * rx) + (ry * ry) + (rz * rz));
    rx /= l;
    ry /= l;
    rz /= l;

    if (rz != 0){
        B_theta = Acos(rz);
    } else {
        B_theta = 0;
    }

    if ((ry == 0) && (rx >= 0)) {
        B_phi = 0;
    } else if ((rx == 0) && (ry >= 0)) {
        B_phi = 90;
    } else if ((ry == 0) && (rx < 0)) {
        B_phi = 180;
    } else if ((rx == 0) && (ry < 0)) {
        B_phi = 270;
    } else if (rx >= 0){
        B_phi = Atan(ry / rx);
    } else if (rx < 0){
        B_phi = Atan(ry / rx);
        B_theta *= -1;
    }

    if (Verbose) {
        std::cout << "parent theta: " << parent->B_theta << " phi: " << parent->B_phi << std::endl; std::cout.flush();
        std::cout << "v: " << vx << " " << vy << " " << vz << std::endl; std::cout.flush();
        std::cout << "A: " << Ax << " " << Ay << " " << Az << std::endl; std::cout.flush();
        std::cout << "B: " << Bx << " " << By << " " << Bz << std::endl; std::cout.flush();
        std::cout << "r: " << rx << " " << ry << " " << rz << std::endl; std::cout.flush();
        std::cout << "new theta: " << B_theta << " phi: " << B_phi << std::endl << std::endl; std::cout.flush();
    }
}

void branch::getBase(void)
{
    // X = parent->X;
    X = parent->X - (parent->length * Sin(parent->B_theta) * Sin(parent->B_phi));
    Y = parent->Y + (parent->length * Sin(parent->B_theta) * Cos(parent->B_phi));
    Z = parent->Z + (parent->length * Cos(parent->B_theta));
}

glutWindow win;

branch trunk;

branch *temp;

void display() 
{
    // Clear Screen and Depth Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);        
    glLoadIdentity();

    // Define a viewing transformation
    // gluLookAt( 7,5,0, -2,0,0, 0,1,0);            
    gluLookAt( distance * Cos(theta) * Sin(phi) + X2,
               distance * Sin(theta) * Sin(phi) + Y2,
               distance * Cos(phi) + Z2,
               X2,
               Y2,
               Z2, 
               X3, Y3, Z3);  

    // // Ground
    // glRotatef(0,0,1,0);
    // glRotatef(90,0,1,0);
    glPushMatrix();  
        glTranslatef(-10,-10,0);

        glColor3f(0.2, 1, 0);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glVertex3f(-10, -10, 0);
        glTexCoord2f(0, 1);
        glVertex3f(10, -10, 0);
        glTexCoord2f(0, 0);
        glVertex3f(10, 10, 0);
        glTexCoord2f(1, 0);
        glVertex3f(-10, 10, 0);
        glEnd();
    glPopMatrix();  

    glPushMatrix();  
        glTranslatef(10,-10,0);

        glColor3f(0, 0, 1);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glVertex3f(-10, -10, 0);
        glTexCoord2f(0, 1);
        glVertex3f(10, -10, 0);
        glTexCoord2f(0, 0);
        glVertex3f(10, 10, 0);
        glTexCoord2f(1, 0);
        glVertex3f(-10, 10, 0);
        glEnd();
    glPopMatrix();  

    glPushMatrix();  
        glTranslatef(10,10,0);

        glColor3f(0.2, 1, 0);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glVertex3f(-10, -10, 0);
        glTexCoord2f(0, 1);
        glVertex3f(10, -10, 0);
        glTexCoord2f(0, 0);
        glVertex3f(10, 10, 0);
        glTexCoord2f(1, 0);
        glVertex3f(-10, 10, 0);
        glEnd();
    glPopMatrix();  

    glPushMatrix();  
        glTranslatef(-10,10,0);

        glColor3f(0, 0, 1);
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);
        glVertex3f(-10, -10, 0);
        glTexCoord2f(0, 1);
        glVertex3f(10, -10, 0);
        glTexCoord2f(0, 0);
        glVertex3f(10, 10, 0);
        glTexCoord2f(1, 0);
        glVertex3f(-10, 10, 0);
        glEnd();
    glPopMatrix();  

    if(Pointer){
        glPushMatrix();
        glColor3f(0,0,1);
        glTranslatef(X2, Y2, Z2);
        glutSolidSphere(0.2, 32, 32);
        glPopMatrix();
    }

    if(!Paused){
        trunk.grow();
        distance *= (1 + AutoZoom);
    }

    trunk.draw();
    // for ( unsigned int i = 0; i < branches.size(); i++ ){
    //     glPushMatrix();                   
    //     glTranslatef(branches[i]->X,branches[i]->Y,branches[i]->Z);
    //     glRotatef(-branches[i]->B_theta, Cos(branches[i]->B_phi), Sin(branches[i]->B_phi),0);
    //     glColor3f(1,0,0);
    //     std::cout << "test " << i << std::endl; std::cout.flush();
    //     std::cout << branches[i] << std::endl; std::cout.flush();
    //     std::cout << branches[i]->myCylinder << std::endl; std::cout.flush();
    //     std::cout << branches[i]->radius << std::endl; std::cout.flush();
    //     std::cout << branches[i]->length << std::endl; std::cout.flush();
    //     gluCylinder(branches[i]->myCylinder, branches[i]->radius, branches[i]->radius * 0.8, branches[i]->length * 1.1, 20, 200);
    //     std::cout << "test7" << std::endl; std::cout.flush();
    //     glPopMatrix();  
    // }

    // g_rotation += g_rotation_speed;
    glutSwapBuffers();
}


void initialize () 
{
    // select projection matrix
    glMatrixMode(GL_PROJECTION);                        

    // set the viewport
    glViewport(0, 0, win.width, win.height);                  

    // set matrix mode
    glMatrixMode(GL_PROJECTION);                        

    // reset projection matrix
    glLoadIdentity();                             
    GLfloat aspect = (GLfloat) win.width / win.height;

    // set up a perspective projection matrix
    gluPerspective(win.field_of_view_angle, aspect, win.z_near, win.z_far);   

    // specify which matrix is the current matrix
    glMatrixMode(GL_MODELVIEW);                         
    glShadeModel( GL_SMOOTH );

    // specify the clear value for the depth buffer
    glClearDepth( 1.0f );                           
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // specify implementation-specific hints
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );            

    GLfloat amb_light[] = { 0.1, 0.5, 0.1, 1.0 };
    GLfloat diffuse[] = { 0.6, 0.6, 0.6, 1 };
    GLfloat specular[] = { 0.7, 0.7, 0.3, 1 };
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); 
    glClearColor(0.0, 0.0, 0.0, 0.0);
}


void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )   
{ 
    switch ( key ) 
    {
        case KEY_ESCAPE:        
            std::exit(0);   
            break;      

        case 'z':
            distance += 0.1;
            break; 

        case 'Z':
            distance -= 0.1;
            break; 

        case 'x':
            theta += 1;
            break; 

        case 'X':
            theta -= 1;
            break; 

        case 'c':
            phi += 1;
            break; 

        case 'C':
            phi -= 1;
            break; 


        case 'a':
            X2 += 0.1;
            break;     

        case 'A':
            X2 -= 0.1;
            break;     

        case 's':
            Y2 += 0.1;
            break;     

        case 'S':
            Y2 -= 0.1;
            break;     

        case 'd':
            Z2 += 0.1;
            break;     

        case 'D':
            Z2 -= 0.1;
            break;


        case ' ':
            if (Paused) {
                Paused = false;
                if (Verbose) { std::cout << "Unpausing" << std::endl; }
            }
            else {
                Paused = true;
                if (Verbose) { std::cout << "Pausing" << std::endl; }
            }
            break;

        case 'v':
            if (Verbose) {
                Verbose = false;
                std::cout << "Verbose off" << std::endl;
            }
            else {
                Verbose = true;
                std::cout << "Verbose on" << std::endl;
            }
            break;

        case 'p':
            std::cout << distance << " " << theta << " " << phi << " "
                      << X2 << " " << Y2 << " " << Z2 << " "
                      << X3 << " " << Y3 << " " << Z3 << std::endl;
            break;

        case 'r':
            theta = 0;
            // phi = 0;
            break;

        case 'R':
            theta = 0;
            phi = 1;
            break;

        case '.':
            trunk.bud();
            break;

        case ',':
            if (Pointer) { Pointer = false;}
            else { Pointer = true; }
            break;

        case '[':
            AutoZoom += 0.001;
            if (Verbose) { std::cout << "AutoZoom: " << AutoZoom << std::endl; }
            break;

        case ']':
            AutoZoom -= 0.001;
            if (Verbose) { std::cout << "AutoZoom: " << AutoZoom << std::endl; }
            break;

        default:      
            break;
    }
}

int main(int argc, char **argv) 
{
    // set window values
    win.width = 640*2;
    win.height = 480*2;
    win.title = "just trying to draw a tree";
    win.field_of_view_angle = 45;
    win.z_near = 1.0f;
    win.z_far = 500.0f;

    Paused = true;
    Pointer = true;
    // Verbose = true;
    Verbose = false;
    AutoZoom = 0.000;

    // X2 = gsl_ran_gaussian(r, 0.5);

    X2 = 0;
    Y2 = 0;
    Z2 = 4.5;

    X3 = 0;
    Y3 = 0;
    Z3 = 90;

    distance = 17;
    theta = 15;
    phi = 65;

    // branches.push_back(&trunk);
    trunk.bud();
    trunk.grow(200);
    trunk.bud();
    trunk.grow(500);
    trunk.bud();
    trunk.grow(500);

    // initialize and run program
    glutInit(&argc, argv);                                          // GLUT initialization
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );      // Display Mode
    glutInitWindowSize(win.width,win.height);           // set window size
    glutCreateWindow(win.title);            // create Window
    glutDisplayFunc(display);               // register Display Function
    glutIdleFunc( display );                // register Idle Function
    glutKeyboardFunc( keyboard );            // register Keyboard Handler
    initialize();
    glutMainLoop();                 // run GLUT mainloop
    return 0;
}
