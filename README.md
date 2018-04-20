# Autostrada A2
##### Zanfir Andrei
##### Grupa 254


## Descrierea proiectului
Tema aleasa pentru acest proiect este reprezentarea unui domino 2D format din piese, avand aceeasi dimensiune, care incep sa se miste atunci cand sunt lovite de un alt obiect.
Animatia este simpla si interactiva.
![N|Solid](http://3.bp.blogspot.com/-UkCAootJS8o/UKP8oi_rIEI/AAAAAAAAAJA/QwTLAIngOS8/w1200-h630-p-k-no-nu/La+causa+1.jpg)


## Obiectele desenate
##### Desenarea se realizeaza cu ajutorul :
- liniilor
- dreptunghiurilor
- poligoanelor
- triunghiurilor
- listelor de display

##### Apar 3 tipuri de obiecte:
- *masa* pe care este desenata scena
    - reprezentata de 2 picioare si o placa
    - contur desenat cu linii si umplut cu poligoane
```C++
glColor3f(0.5, 0.2, 0.0);	// culoarea interiorului mesei
    // piciorul stang
    glBegin(GL_POLYGON);
        glVertex2i(-400, -200);
        glVertex2i(-400, -1);
        glVertex2i(-360, -1);
        glVertex2i(-360, -200);
    glEnd();

glColor3f(0.0, 0.0, 0.0);	// culoarea conturului mesei
    glBegin(GL_LINES);
        // piciorul stang
        glVertex2i(-400, -200);
        glVertex2i(-400, -1);
        glVertex2i(-360, -200);
        glVertex2i(-360, -50);
    glEnd();
```
- *masina* cu care pot fi daramate piesele
    - reprezentata prin 2 roti si un triunghi
    - rotile au 2 dreptunghiuri asezate in + pentru a evidentia rotatia
```C++
// Roata 1
regHex1 = glGenLists(1);
glNewList(regHex1, GL_COMPILE);
        glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_POLYGON);
                for (k = 0; k < 100; k++){
                    hexTheta = TWO_PI * k / 100;
                    hexVertex.x = 195 + raza * cos(hexTheta);
                    hexVertex.y = 35 + raza * sin(hexTheta);
                    glVertex2i(hexVertex.x, hexVertex.y);
                }
            glEnd();
            
        glColor3f(0.6, 0.6, 0.6);
            glRecti(165, 33, 225, 37);
            glRecti(193, 5, 197, 65);
    glEndList();
```
- 6 *piese* de domino 
    - se stiu coordonatele primei piese iar restul sunt create in functie de aceasta si sunt retinute intr-o structura
    - desenate separat pentru ca fiecare piesa sa aiba rotatia sa
```C++
void createDominos(){
    for (int c = 1; c <= nr_dominos; c++){
        if (c == 1){
            dominos[c].xJos = 0;
            dominos[c].xSus = 30;
        }else{
            dominos[c].xJos = dominos[c - 1].xJos - 60;
            dominos[c].xSus = dominos[c - 1].xSus - 60;
        }
    }
}
void drawD1(){
    glColor3f(0.1, 0.0, 0.1);
        glRecti(dominos[1].xJos, yJos, dominos[1].xSus, ySus);
}
```

##### Transformarile folosite pe aceste obiecte sunt:
- translatie (miscarea masinii)
```C++
glPushMatrix();
    glTranslated(x_tr, 0, 0);
    drawCar();
glPopMatrix();
```
- rotatie (miscarea pieselor)
```C++
glPushMatrix();
    glRotated(rotate_angle_1, 0.0, 0.0, 1.0);
    drawD1();
glPopMatrix();
```
- translatie compusa cu rotatie (invartirea rotilor)
```C++
glPushMatrix();
    glTranslatef(195, 35, 0.0);        //3. muta obiectul in pozitia sa initiala
    glRotatef(alpha, 0.0, 0.0, 1.0);   //2. roteste obiectul
    glTranslatef(-195, -35, 0.0);      //1. muta obiectul in origine
    glCallList(regHex1);
glPopMatrix();
```


## Functionarea animatiei
 -> Instructiunile sunt afisate si in partea de sus a ferestrei.
#### Input de la utilizator - miscarea masinii
Aceasta se realizeaza cu ajutorul mouse-ului: miscarea incepe la apasarea oricarui buton, click dreapta directioneaza masina spre dreapta, respectiv click stanga o directioneaza spre stanga.

```c++
void moveLeft(void){
    if (x_tr > max_stg){
        x_tr += -tr_speed; //scade val de translatie => miscare la stanga
        alpha += tr_speed; //creste unghiul de rotatie al rotilor => rotatie spre stanga
    }
    glutPostRedisplay();
}
void moveRight(void){
    if (x_tr < max_dr){
        x_tr += tr_speed;  //creste val de translatie => miscare la dreapta
        alpha -= tr_speed; //scade unghiul de rotatie al rotilor => rotatie spre dreapta
    }
    glutPostRedisplay();
}
void mouse(int key, int state, int x, int y){
    switch (key){
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(moveLeft);
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN)
                glutIdleFunc(moveRight);
            break;
        default:
            break;
    }
}
```

#### Declansarea miscarii pieselor
1. atunci cand varful masinii loveste prima piesa de domino aceasta incepe rotatia
2. limita stanga de miscare a masinii se modifica deoarece prima piesa se muta
3. cand unghiul cu care se roteste o piesa ajunge in punctul de coliziune (calculat matematic) se declanseaza rotatia urmatoarei piese
4. ultima piesa se opreste dupa o rotatie de 90 de grade
```C++
if (x_tr <= max_stg){
    start = 1;
    if (max_stg > -148)
        max_stg -= 1;
}
if (rotate_angle_1 < 60 && start == 1)
    rotate_angle_1 += 3;
if (rotate_angle_2 < 60 && rotate_angle_1 >= 11.5)
    rotate_angle_2 += 3;
if (rotate_angle_3 < 60 && rotate_angle_2 >= 11.5)
    rotate_angle_3 += 3;
if (rotate_angle_4 < 60 && rotate_angle_3 >= 11.5)
    rotate_angle_4 += 3;
if (rotate_angle_5 < 62 && rotate_angle_4 >= 11.5)
    rotate_angle_5 += 3;
if (rotate_angle_6 < 90 && rotate_angle_5 >= 11.5)
    rotate_angle_6 += 3;
```

## Originalitate
Fiind o simpla animatie a unor piese de domino elementul principal de originalitate este reprezentat de metoda aleasa pentru inceperea miscarii pieselor (lovite de o masina), impreuna cu avertizarile afisate prin text in momentele in care masina nu se mai poate deplasa sau nu ar mai trebui sa se deplaseze. Nu este o idee nemaiintalnita insa am preferat-o in detrimentul impingerii primei piese cu un deget sau o bila.
