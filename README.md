# Proiect Grafica pe calculator : Sala de lectura pentru copii
##### Nicolescu Bristena | Grupa 253
##### Zanfir Andrei | Grupa 254
##
##
##
## Descrierea proiectului
Tema aleasa pentru acest proiect este o camera de lectura pentru copii.
Proiectul este simplu si intruneste aproape toate cunostiintele acumulate la curs.
![N|Solid](https://lh5.googleusercontent.com/dsubGilKyHnQeFPpCsVmBATX0M3OAeUPoszU_uhuMt3PEKSYXdzOzfH9c7vHs44WPDqfM49r82Ktzq8iiE2q=w1304-h677-rw)


## Scena a fost realizeaza cu ajutorul :
- obiectelor cuadrice
- obiectelor 3D solide
- transformarilor 3D
- testelor de adancime
- texturilor
- iluminarii
- umbrelor
### Obiectele desenate impreuna cu transformarile aplicate:
- ##### bec
```C++
void lumina(void) {
	glPushMatrix();
    	glTranslatef(lightpos[X], lightpos[Y], lightpos[Z]);
    	glDisable(GL_LIGHTING);
    	glColor3f(1.f, 1.f, .8f);
    	glCallList(LIGHT);
    	glEnable(GL_LIGHTING);
	glPopMatrix();
}

//format dintr-o sfera cuadrica
    glNewList(LIGHT, GL_COMPILE);
    	sphere = gluNewQuadric();
    	gluSphere(sphere, 5.f, 20, 20);
    	gluDeleteQuadric(sphere);
    glEndList();
```
- ##### mese
```C++
void masa1(void)
{
	glPushMatrix();
    	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa1_mat);
    	glTranslatef(-130.f, -100.f, -480.f);
    	glCallList(CONE);
	glPopMatrix();

	glPushMatrix();
    	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa1_mat);
    	glTranslatef(-130.f, -70.f, -480.f);
    	glCallList(MASA);
	glPopMatrix();

	glPushMatrix();
    	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
    	glTranslatef(-130.f, -40.f, -480.f);
    	glCallList(SPHERE);
	glPopMatrix();
}

////formate din obiecte cuadrice
//con - piciorul
    glNewList(CONE, GL_COMPILE);
    	cone = gluNewQuadric();
    	glRotatef(-90.f, 1.f, 0.f, 0.f);
    	gluCylinder(cone, 10., 1., 55., 20, 20);
    	gluDeleteQuadric(cone);
	glEndList();
//cilindru - blatul mesei
	glNewList(MASA, GL_COMPILE);
    	masa = gluNewQuadric();
    	base = gluNewQuadric();
    	glRotatef(-90.f, 1.f, 0.f, 0.f);
    	glPushMatrix();
        	glTranslatef(0. ,0., 10. );
        	gluDisk(base, 0., 40., 100, 1);
    	glPopMatrix();
    	gluDisk(base, 0., 40., 100, 1);
    	gluCylinder(masa, 40., 40., 10., 100, 100);
    	gluDeleteQuadric(masa);
    	gluDeleteQuadric(base);
	glEndList();
//sfera din mijlocul mesei
	glNewList(SPHERE, GL_COMPILE);
    	sphere = gluNewQuadric();
    	gluSphere(sphere, 10.f, 50, 20);
    	gluDeleteQuadric(sphere);
	glEndList();
```
- ##### perna
```C++
void perna(void)
{
	glPushMatrix();
    	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, scaun_mat);
    	glTranslatef(-180.f, -72.f, -360.f);
    	glRotatef(90.f, 20.f, 90.f, 40.f);
//formata dintr-un obiect 3D solid
	    glutSolidTorus(10.0, 20.0, 100, 100);
	glPopMatrix();
}
```

### Teste de adancime si iluminare
```C++
//Am salvat pozitia luminii intr-un vector pentru a o putea folosi si la desenarea becului
GLfloat lightpos[] = { 0.f, 80.f, -350.f, 1.f };

//In functia main() am aplicat proprietatile de lumina si test de adancime
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//dupa care am setat pozitia si culoarea luminii
	float alb[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, alb);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
```
 Iluminarea poate fi observata pe sferele din mijlocul meselor, acestea fiind mai albe in partea dinspre bec, iar testul de adancime face ca varful conului sa fie desenat prin mijlocul blatului si nu in fata sau spatele lui.

### Texturare
Am folosit 3 poze diferite pentru texturi (podea, pereti, tavan). Functia de incarcare a texturii peretilor este folosita in cele 3 functii de desenare pentru fiecare perete in parte.
```c++
void LoadTexturePerete(void)
{
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image("perete.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}

void drawTexturePereteSpate() {
	LoadTexturePerete();

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(-200.f, -100.f, -600.f);
		glTexCoord2f(0.0, 0.0); glVertex3f(-200.f, 120.f, -600.f);
		glTexCoord2f(1.0, 0.0); glVertex3f(210.f, 120.f, -600.f);
		glTexCoord2f(1.0, 1.0); glVertex3f(210.f, -100.f, -600.f);
	glEnd();
}

// Desenarea texturilor este apelata in functia de desen
	glEnable(GL_TEXTURE_2D);
		drawTextureParchet();
		drawTexturePereteDreapta();
		drawTexturePereteSpate();
		drawTextureTavan();
	glDisable(GL_TEXTURE_2D);
```
### Umbre



## Originalitate
Nota de originalitate a proiectului o reprezinta design-ul nonconformist al meselor
