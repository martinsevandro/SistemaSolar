#include <GL/glut.h>
#include "imageloader.h"

int bigOrbitActive = 1;//Órbitas maiores
int changeCamera = 0;//Mudança de câmera
int zoom = 50;//Proximidade da câmera
static float factor = 1;//Fator de velocidade

float lightPos[] = { 0.0, 10.0, 0.0, 0.0 }; // Posição do ponto de luz
static float spotAngle = 180; // Spotlight cone half-angle.
float spotDirection[] = { 0.0, -1.0, 0.0 }; // Direção do ponto de luz
static float spotExponent = 5.0; // Fator de atenuação


class Planet {
public:
    float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
    Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni) {
        radius = _radius;
        distance = _distance;
        orbit = _orbit;
        orbitSpeed = _orbitSpeed;
        axisTilt = _axisTilt;
        axisAni = _axisAni;
    }
	//Linhas da órbita
    void drawSmallOrbit(void) {
        glPushMatrix();
			glColor3ub(255, 255, 255);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glutWireTorus(0.001, distance, 100.0, 100.0);
        glPopMatrix();
    }
	//Gera o satélite natural do planeta
    void drawMoon(void) {
        GLUquadricObj* quadric;
        quadric = gluNewQuadric();
        glPushMatrix();
			glColor3ub(255, 255, 255);
			glRotatef(orbit, 0.0, 1.0, 0.0);
			glTranslatef(distance, 0.0, 0.0);
			gluSphere(quadric, radius, 20.0, 20.0);
        glPopMatrix();
    }

};

Planet sun(5.0, 0, 0, 0, 0, 0);				//Sol
Planet mer(1.0, 7, 0, 4.74, 02.11, 0);		//Mercurio
Planet ven(1.5, 11, 0, 3.50, 177.0, 0);		//Venus
Planet ear(2.0, 16, 0, 2.98, 23.44, 0);		//Terra
Planet mar(1.2, 21, 0, 2.41, 25.00, 0);		//Marte
Planet jup(3.5, 28, 0, 1.31, 03.13, 0);		//Jupiter
Planet sat(3.0, 37, 0, 0.97, 26.70, 0);		//Saturno
Planet ura(2.5, 45.5, 0, 0.68, 97.77, 0);	//Urano
Planet nep(2.3, 53.6, 0, 0.54, 28.32, 0);	//Netuno
Planet lun(.40, 3, 0, 5.40, 0, 0);			//Lua

//Desenha a trajetória das órbitas
void orbitalTrails(void) {
	glPushMatrix();
	glColor3ub(255, 255, 255);
	glTranslatef(0.0, 0.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.001, mer.distance, 100.0, 100.0);
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glutWireTorus(0.001, mar.distance, 100.0, 100.0);
	glutWireTorus(0.001, jup.distance, 100.0, 100.0);
	glutWireTorus(0.001, sat.distance, 100.0, 100.0);
	glutWireTorus(0.001, ura.distance, 100.0, 100.0);
	glutWireTorus(0.001, nep.distance, 100.0, 100.0);
	glPopMatrix();
}

//Carregar texturas
GLuint loadTexture(Image* image) {
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, staTexture;

void setup(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    //Geração de texturas
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
	Image* sta = loadBMP("stars.bmp");		staTexture = loadTexture(sta);		delete sta;
	Image* sun = loadBMP("sun.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercury.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("earth.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("mars.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturn.bmp");		satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("uranus.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("neptune.bmp");	nepTexture = loadTexture(nep);		delete nep;

    //Configurando iluminação da cena
    glEnable(GL_LIGHTING);
    float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
    float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
    float globAmb[] = { 0.5, 0.5, 0.5, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotAngle);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spotExponent);
}

//Redimensionamento de janela
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);
 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
 	glMatrixMode(GL_MODELVIEW);
}

//Captar algumas entradas do teclado
void keyInput(unsigned char key, int x, int y) {
	switch (key) {
	case 27: exit(0); break;//Pressionar esc para fechar a janela
	case 'o': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break; //'o' ativa as órbitas 
	case '1': changeCamera = 0; glutPostRedisplay(); break;//'1' câmera em perspectiva
	case '2': changeCamera = 1; glutPostRedisplay(); break;//'2' câmera lateral
	case '3': changeCamera = 2; glutPostRedisplay(); break;//'3' câmera superior
	case 'f': factor *= 2; glutPostRedisplay; break;//'f' dobra a velocidade de translação dos planetas
	case 's': factor /= 2; glutPostRedisplay; break;//'s' divide por 2 a velocidade de translação dos planetas
	}
}

//Captar entradas do mouse
void mouseControl(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//Botão esquerdo do mouse aumenta o zoom
        zoom+=10;
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//Botão direito do mouse diminui o zoom
		zoom-=10;
	glutPostRedisplay();
}

//==================================================================

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//Diferentes posições da câmera
	if (changeCamera == 0)gluLookAt(0.0, zoom, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 2)gluLookAt(0.0, zoom, 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Órbitas 
	if (bigOrbitActive == 1) orbitalTrails();

	GLUquadric* quadric;
	quadric = gluNewQuadric();

	//Sol
	glPushMatrix();
		glRotatef(sun.orbit, 0.0, 1.0, 0.0);
		glTranslatef(sun.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(sun.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(sun.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, sunTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, sun.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Mercúrio e órbita
	glPushMatrix();
		glRotatef(mer.orbit, 0.0, 1.0, 0.0);
		glTranslatef(mer.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, merTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, mer.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Vênus e órbita
	glPushMatrix();
		glRotatef(ven.orbit, 0.0, 1.0, 0.0);
		glTranslatef(ven.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(ven.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(ven.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, venTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, ven.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Planeta Terra e órbita
	glPushMatrix();
		glRotatef(ear.orbit, 0.0, 1.0, 0.0);
		glTranslatef(ear.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, earTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, ear.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		if (bigOrbitActive == 1) {
			lun.drawSmallOrbit(); //Desenha a órbita da lua
		}
		lun.drawMoon(); //Desenha a lua
	glPopMatrix();

	//Marte e órbita
	glPushMatrix();
		glRotatef(mar.orbit, 0.0, 1.0, 0.0);
		glTranslatef(mar.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(mar.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(mar.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, marTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, mar.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Júpiter e órbita
	glPushMatrix();
		glRotatef(jup.orbit, 0.0, 1.0, 0.0);
		glTranslatef(jup.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(jup.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(jup.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, jupTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, jup.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Saturno e órbita
	glPushMatrix();
		glRotatef(sat.orbit, 0.0, 1.0, 0.0);
		glTranslatef(sat.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(sat.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(sat.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, satTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, sat.radius, 20.0, 20.0);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glColor3ub(158, 145, 137);
			glRotatef(-63.0, 1.0, 0.0, 0.0);
			glutWireTorus(0.2, 6.0, 30.0, 30.0);//Anéis de saturno
			glutWireTorus(0.4, 5.0, 30.0, 30.0);//Anéis de saturno
		glPopMatrix();
	glPopMatrix();

	glColor3ub(255, 255, 255);	

	//Urano e órbita
	glPushMatrix();
		glRotatef(ura.orbit, 0.0, 1.0, 0.0);
		glTranslatef(ura.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(ura.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(ura.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, uraTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, ura.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Netuno e órbita
	glPushMatrix();
		glRotatef(nep.orbit, 0.0, 1.0, 0.0);
		glTranslatef(nep.distance, 0.0, 0.0);
		glPushMatrix();
			glRotatef(nep.axisTilt, 1.0, 0.0, 0.0);
			glRotatef(nep.axisAni, 0.0, 1.0, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, nepTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			gluQuadricTexture(quadric, 1);
			gluSphere(quadric, nep.radius, 20.0, 20.0);
			glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	glPopMatrix();

	//Skybox
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, staTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_POLYGON);
			glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
			glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
			glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
			glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, staTexture);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
			glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
			glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
			glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glutSwapBuffers();
}

//Translação dos planetas
void TimerFunction(int value) {
	mer.orbit += mer.orbitSpeed*factor;
    if (mer.orbit >= 360) mer.orbit = 0;
	ven.orbit += ven.orbitSpeed * factor;
	if (ven.orbit >= 360) ven.orbit = 0;
	ear.orbit += ear.orbitSpeed * factor;
	if (ear.orbit >= 360) ear.orbit = 0;
	mar.orbit += mar.orbitSpeed * factor;
	if (mar.orbit >= 360) mar.orbit = 0;
	jup.orbit += jup.orbitSpeed * factor;
	if (jup.orbit >= 360) jup.orbit = 0;
	sat.orbit += sat.orbitSpeed * factor;
	if (sat.orbit >= 360) sat.orbit = 0;
	ura.orbit += ura.orbitSpeed * factor;
	if (ura.orbit >= 360) ura.orbit = 0;
	nep.orbit += nep.orbitSpeed * factor;
	if (nep.orbit >= 360) nep.orbit = 0;
	lun.orbit += lun.orbitSpeed * factor;
	if (lun.orbit >= 360) lun.orbit = 0;

	mer.axisAni += 10.0;
	if (mer.axisAni > 360.0) mer.axisAni -= 360.0;
	ven.axisAni += 10.0;
	if (ven.axisAni > 360.0) ven.axisAni -= 360.0;
	ear.axisAni += 10.0;
	if (ear.axisAni > 360.0) ear.axisAni -= 360.0;
	mar.axisAni += 10.0;
	if (mar.axisAni > 360.0) mar.axisAni -= 360.0;
	jup.axisAni += 10.0;
	if (jup.axisAni > 360.0) jup.axisAni -= 360.0;
	sat.axisAni += 10.0;
	if (sat.axisAni > 360.0) sat.axisAni -= 360.0;
	ura.axisAni += 10.0;
	if (ura.axisAni > 360.0) ura.axisAni -= 360.0;
	nep.axisAni += 10.0;
	if (nep.axisAni > 360.0) nep.axisAni -= 360.0;

    glutPostRedisplay();
    glutTimerFunc(33, TimerFunction, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Sistema Solar");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseControl);
	glutKeyboardFunc(keyInput);
    glutTimerFunc(100, TimerFunction, 0);
    glEnable(GL_DEPTH_TEST);
	setup();
    glutMainLoop();
}