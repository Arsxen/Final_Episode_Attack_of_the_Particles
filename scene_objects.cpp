//
// Created by Archawat on 6/2/2020.
//

#include "scene_objects.h"
#include <GL/glut.h>
#include <gmtl/gmtl.h>

namespace scene_object
{
    // Materials
    const GLfloat DARKRED_COL[] = {0.1, 0.0, 0.0, 1.0};
    const GLfloat BRIGHTRED_COL[] = {0.7, 0.0, 0.0, 1.0};
    const GLfloat DARKBLUE_COL[] = {0.0, 0.0, 0.1, 1.0};
    const GLfloat BRIGHTBLUE_COL[] = {0.0, 0.0, 0.7, 1.0};
    const GLfloat DARK_COL[] = {0.1, 0.1, 0.1, 1.0};
    const GLfloat MEDIUMWHITE_COL[] = {0.7, 0.7, 0.7, 1.0};
    const GLfloat SPECULAR_COL[] = {0.7, 0.7, 0.7, 1.0};
    const GLfloat GREEN_COL[] = {0.55, 0.95, 0.64, 1.0};
    const GLfloat DARKGREEN_COL[] = {0.0, 0.1, 0.0, 1.0};
    const GLfloat DARKGRAY_COL[] = {0.26, 0.26, 0.26, 1.0};
    const GLfloat GRAY_COL[] = {0.75, 0.75, 0.75, 1.0};
    const GLfloat WHITE_COL[] = {1.0, 1.0, 1.0, 1.0};
    const GLfloat PASTEL_COL[] = {1.0, 0.69, 0.94, 1.0};
    const GLfloat PASTELDARKER_COL[] = {0.90, 0.59, 0.69, 1.0};

    // Textures
    enum TextureID {
        TID_SKYBACK = 0, TID_SKYLEFT, TID_SKYBOTTOM, TID_SKYTOP, TID_SKYFRONT, TID_SKYRIGHT, TEXTURE_NB
    };
    GLuint textures[TEXTURE_NB];
    GLuint satellite_wing;
    GLuint earth_texture;
    GLuint plane_wing;

    void LoadPPM(const char *fname, unsigned int *w, unsigned int *h, unsigned char **data, int mallocflag) {
        FILE *fp;
        char P, num;
        int max;
        char s[1000];

        if (!(fp = fopen(fname, "rb"))) {
            perror("cannot open image file\n");
            exit(0);
        }

        fscanf(fp, "%c%c\n", &P, &num);
        if ((P != 'P') || (num != '6')) {
            perror("unknown file format for image\n");
            exit(0);
        }

        do {
            fgets(s, 999, fp);
        } while (s[0] == '#');


        sscanf(s, "%d%d", w, h);
        fgets(s, 999, fp);
        sscanf(s, "%d", &max);

        if (mallocflag)
            if (!(*data = (unsigned char *) malloc(*w * *h * 3))) {
                perror("cannot allocate memory for image data\n");
                exit(0);
            }

        fread(*data, 3, *w * *h, fp);

        fclose(fp);
    }

    void LoadTexture() {
        unsigned char *img_data;               // Texture image data
        unsigned int width;                   // Texture width
        unsigned int height;                  // Texture height

        // Describe how data will be stored in memory
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Select the method for combining texture color with the lighting equation
        // (look up the third parameter)
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        // Generate and setup texture objects
        glGenTextures(TEXTURE_NB, textures);

        // Skybox back wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBACK]);
        LoadPPM("skybox_back.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Skybox left wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYLEFT]);
        LoadPPM("skybox_left.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Skybox buttom wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBOTTOM]);
        LoadPPM("skybox_bottom.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Skybox front wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYFRONT]);
        LoadPPM("skybox_front.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Skybox right wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYRIGHT]);
        LoadPPM("skybox_right.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Skybox top wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYTOP]);
        LoadPPM("skybox_top.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Satellite Wing Texture
        glGenTextures(1, &satellite_wing);
        glBindTexture(GL_TEXTURE_2D, satellite_wing);
        LoadPPM("satellite_wing.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Earth Texture
        glGenTextures(1, &earth_texture);
        glBindTexture(GL_TEXTURE_2D, earth_texture);
        LoadPPM("earth.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Plane Wing Texture
        glGenTextures(1, &plane_wing);
        glBindTexture(GL_TEXTURE_2D, plane_wing);
        LoadPPM("wing_texture.ppm", &width, &height, &img_data, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
        free(img_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }


    void drawCylinder(const float radius, const float height) {
        int a;
        float x, y, xp, yp, mag;
        bool bright_col = true;

        xp = radius;
        yp = 0;
        glBegin(GL_QUADS);
        for (a = 1; a <= 360; a++) {
            x = radius * cos(gmtl::Math::deg2Rad((float) a));
            y = radius * sin(gmtl::Math::deg2Rad((float) a));

            //mag = sqrt(pow(x, 2) + pow(y, 2) + pow(height / 2, 2));
            gmtl::Vec3f normal(x, y, 0.0);
            gmtl::normalize(normal);
            glNormal3f(normal[0], normal[1], normal[2]);
            glVertex3f(xp, yp, height);
            glVertex3f(xp, yp, 0);
            glVertex3f(x, y, 0);
            glVertex3f(x, y, height);

            xp = x;
            yp = y;
        }
        glEnd();
    }

    void DrawPlaneBody(const float width, const float length, const float height) {
        float w = width / 2;
        float l = length / 2;

        // Sets materials
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR_COL);

        //Plane Core
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKGRAY_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, GRAY_COL);
        glColor3ub(126, 198, 204);
        GLUquadric *sphere = gluNewQuadric();
        drawCylinder(0.5f, (height * 2) / 3);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARK_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DARKGRAY_COL);
        gluSphere(sphere, 0.5f, 360, 360);
        glPushMatrix();
        glTranslatef(0, 0, (height * 2) / 3);
        gluSphere(sphere, 0.5f, 360, 360);
        glPopMatrix();
    }

    void DrawLeftWing(const float width, const float length, const float height) {
        float w = width / 2;
        float l = length / 2;

        // Sets materials
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR_COL);

        float mag = sqrt(pow(1.0f, 2) + pow(3.0f, 2) + pow(0.0f, 2));
        float mag1 = sqrt(pow(2.0f, 2) + pow(0.0f, 2) + pow(-3.0f, 2));
        float mag2 = sqrt(pow(4.5f, 2) + pow(0.0f, 2) + pow(3.0f, 2));
        float mag3 = sqrt(pow(0.946f, 2) + pow(2.795f, 2) + pow(0.0f, 2));
        float mag4 = sqrt(pow(2.15f, 2) + pow(6.0f, 2) + pow(0.0f, 2));
        float mag5 = sqrt(pow(0.285f, 2) + pow(0.7875f, 2) + pow(-0.0275f, 2));


        glBegin(GL_QUADS);
        // Left Wing (top plane)
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MEDIUMWHITE_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, WHITE_COL);
        glNormal3f(1.0f / mag, 3.0f / mag, 0.0f / mag);
        glVertex3f(0.5f, 0.0f, -0.5f);
        glVertex3f(w + 0.5f, -0.5f, 0.5f);
        glVertex3f(w + 0.5f, -0.5f, (height / 2) + 0.5f);
        glVertex3f(0.5f, 0.0f, (2 * height) + 0.5f);


        // Left Wing (bottom plane)
        glNormal3f(-1.0f / mag, -3.0f / mag, 0.0f / mag);
        glVertex3f(0.5f, -0.08f, -0.5f);
        glVertex3f(w + 0.5f, -0.58f, 0.5f);
        glVertex3f(w + 0.5f, -0.58f, (height / 2) + 0.5f);
        glVertex3f(0.5f, -0.08f, (2 * height) + 0.5f);

        //Connected parts between top plane and bottom plane
        glColor3ub(54, 208, 219);
        glNormal3f(2.0f / mag1, 0.0f / mag1, -3.0f / mag1);
        glVertex3f(0.5f, 0.0f, -0.5f);
        glVertex3f(w + 0.5f, -0.5f, 0.5f);
        glVertex3f(w + 0.5f, -0.58f, 0.5f);
        glVertex3f(0.5f, -0.08f, -0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(w + 0.5f, -0.5f, 0.5f);
        glVertex3f(w + 0.5f, -0.5f, (height / 2) + 0.5f);
        glVertex3f(w + 0.5f, -0.58f, (height / 2) + 0.5f);
        glVertex3f(w + 0.5f, -0.58f, 0.5f);

        glNormal3f(4.5f / mag2, 0.0f / mag2, 3.0f / mag2);
        glVertex3f(w + 0.5f, -0.5f, (height / 2) + 0.5f);
        glVertex3f(0.5f, 0.0f, (2 * height) + 0.5f);
        glVertex3f(0.5f, -0.08f, (2 * height) + 0.5f);
        glVertex3f(w + 0.5f, -0.58f, (height / 2) + 0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(0.5f, 0.0f, -0.5f);
        glVertex3f(0.5f, 0.0f, (2 * height) + 0.5f);
        glVertex3f(0.5f, -0.08f, (2 * height) + 0.5f);
        glVertex3f(0.5f, -0.08f, -0.5f);
        glEnd();

        // Left Wing details
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, plane_wing);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MEDIUMWHITE_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, WHITE_COL);
        glBegin(GL_TRIANGLES);

        //Piece1
        glNormal3f(0.946f / mag3, 2.795f / mag3, 0.0f / mag3);

        glTexCoord2f(0, 0);
        glVertex3f(0.6f, -0.011f, (2 * height) + 0.2f);

        glTexCoord2f(0, 0.5);
        glVertex3f(w + 0.4f, -0.451f, (height / 2) + 0.5f);

        glTexCoord2f(0.5, 0.5);
        glVertex3f(0.6f, -0.011f, (height / 2) + 0.3f);
        glEnd();

        //Piece2
        glBegin(GL_QUADS);
        glNormal3f(2.15f / mag4, 6.0f / mag4, 0.0f / mag4);

        glTexCoord2f(0, 0.15f);
        glVertex3f(0.7f, -0.021f, (height / 2) + 0.2f);

        glTexCoord2f(0.5f, 0.25f);
        glVertex3f(w + 0.4f, -0.451f, (height / 2) + 0.4f);

        glTexCoord2f(0.5f, 0);
        glVertex3f(w + 0.4f, -0.451f, 0.55f);

        glTexCoord2f(0, 0);
        glVertex3f(0.7f, -0.021f, 0.55f);


        //Piece3
        glNormal3f(0.285f / mag5, 0.7875f / mag5, -0.0275f / mag5);

        glTexCoord2f(0.1f, 0.1f);
        glVertex3f(0.7f, -0.021f, 0.45f);

        glTexCoord2f(0.5, 0.2f);
        glVertex3f(w + 0.25f, -0.401f, 0.45f);

        glTexCoord2f(0, 0);
        glVertex3f(0.6f, -0.011f, -0.3f);

        glTexCoord2f(0, 0.15f);
        glVertex3f(0.6f, -0.011f, 0.4f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void DrawRightWing(const float width, const float length, const float height) {
        float w = width / 2;
        float l = length / 2;
        float mag = sqrt(pow(1.0f, 2) + pow(-3.0f, 2) + pow(0.0f, 2));
        float mag1 = sqrt(pow(-2.0f, 2) + pow(0.0f, 2) + pow(-3.0f, 2));
        float mag2 = sqrt(pow(-4.5f, 2) + pow(0.0f, 2) + pow(3.0f, 2));
        float mag3 = sqrt(pow(-0.946f, 2) + pow(2.795f, 2) + pow(0.0f, 2));
        float mag4 = sqrt(pow(-0.43f, 2) + pow(1.2f, 2) + pow(0.0f, 2));
        float mag5 = sqrt(pow(-0.39f, 2) + pow(1.15f, 2) + pow(0.0f, 2));

        // Sets materials
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR_COL);

        glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MEDIUMWHITE_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, WHITE_COL);
        // Right Wing (top plane)
        glColor3ub(35, 80, 119);
        glNormal3f(-1.0f / mag, 3.0f / mag, 0.0f / mag);
        glVertex3f(-0.5f, 0.0f, (2 * height) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.5f, (height / 2) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.0f, -0.5f);

        // Right Wing (bottom plane)
        glNormal3f(1.0f / mag, -3.0f / mag, 0.0f / mag);
        glVertex3f(-0.5f, -0.08f, (2 * height) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.58f, (height / 2) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.58f, 0.5f);
        glVertex3f(-0.5f, -0.08f, -0.5f);


        //Connected parts between top plane and bottom plane
        glColor3ub(54, 208, 219);
        glNormal3f(-2.0f / mag1, 0.0f / mag1, -3.0f / mag1);
        glVertex3f(-0.5f, -0.08f, -0.5f);
        glVertex3f(-(w + 0.5f), -0.58f, 0.5f);
        glVertex3f(-(w + 0.5f), -0.5f, 0.5f);
        glVertex3f(-0.5f, 0.0f, -0.5f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-(w + 0.5f), -0.58f, 0.5f);
        glVertex3f(-(w + 0.5f), -0.58f, (height / 2) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.5f, (height / 2) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.5f, 0.5f);

        glNormal3f(-4.5f / mag2, 0.0f / mag2, 3.0f / mag2);
        glVertex3f(-(w + 0.5f), -0.58f, (height / 2) + 0.5f);
        glVertex3f(-0.5f, -0.08f, (2 * height) + 0.5f);
        glVertex3f(-0.5f, 0.0f, (2 * height) + 0.5f);
        glVertex3f(-(w + 0.5f), -0.5f, (height / 2) + 0.5f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.08f, -0.5f);
        glVertex3f(-0.5f, -0.08f, (2 * height) + 0.5f);
        glVertex3f(-0.5f, 0.0f, (2 * height) + 0.5f);
        glVertex3f(-0.5f, 0.0f, -0.5f);
        glEnd();

        // Right Wing details
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, plane_wing);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, GRAY_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MEDIUMWHITE_COL);
        glBegin(GL_TRIANGLES);

        //Piece 1
        glNormal3f(-0.946f / mag3, 2.795f / mag3, 0.0f / mag3);

        glTexCoord2f(0.5, 0.5);
        glVertex3f(-0.6f, -0.011f, (height / 2) + 0.3f);

        glTexCoord2f(0, 0.5);
        glVertex3f(-(w + 0.4f), -0.451f, (height / 2) + 0.5f);

        glTexCoord2f(0, 0);
        glVertex3f(-0.6f, -0.011f, (2 * height) + 0.2f);

        glEnd();

        //Piece 2
        glBegin(GL_QUADS);
        glNormal3f(-0.43f / mag4, 1.2f / mag4, 0.0f / mag4);

        glTexCoord2f(0, 0.15f);
        glVertex3f(-0.7f, -0.021f, 0.55f);

        glTexCoord2f(0.5f, 0.25f);
        glVertex3f(-(w + 0.4f), -0.451f, 0.55f);

        glTexCoord2f(0.5f, 0);
        glVertex3f(-(w + 0.4f), -0.451f, (height / 2) + 0.4f);

        glTexCoord2f(0, 0);
        glVertex3f(-0.7f, -0.021f, (height / 2) + 0.2f);

        //Piece 3
        glNormal3f(-0.39f / mag5, 1.15f / mag5, 0.0f / mag5);

        glTexCoord2f(0.5f, 0.15f);
        glVertex3f(-0.6f, -0.011f, 0.4f);

        glTexCoord2f(0.5, 0.25f);
        glVertex3f(-0.6f, -0.011f, -0.3f);

        glTexCoord2f(0, 0);
        glVertex3f(-(w + 0.25f), -0.401f, 0.45f);

        glTexCoord2f(0, 0.15f);
        glVertex3f(-0.7f, -0.021f, 0.45f);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

    void DrawPlatform(const float width, const float length, const float height) {
        float w = width / 2;
        float l = length / 2;

        // Sets materials
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR_COL);

        //Platform
        glBegin(GL_QUADS);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARK_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DARKGRAY_COL);
        glColor3ub(113, 195, 132);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-0.125f, -0.5f, 0.0f);
        glVertex3f(0.125f, -0.5f, 0.0f);
        glVertex3f(0.125f, -0.5f, -0.3f);
        glVertex3f(-0.125f, -0.5f, -0.3f);

        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(-0.125f, -0.55f, -0.3f);
        glVertex3f(0.125f, -0.55f, -0.3f);
        glVertex3f(0.125f, -0.55f, 0.0f);
        glVertex3f(-0.125f, -0.55f, 0.0f);

        glNormal3f(-1.0f, 0.0f, 0.0f);
        glVertex3f(-0.125f, -0.5f, 0.0f);
        glVertex3f(-0.125f, -0.5f, -0.3f);
        glVertex3f(-0.125f, -0.55f, -0.3f);
        glVertex3f(-0.125f, -0.55f, 0.0f);

        glNormal3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.125f, -0.55f, 0.0f);
        glVertex3f(0.125f, -0.55f, -0.3f);
        glVertex3f(0.125f, -0.5f, -0.3f);
        glVertex3f(0.125f, -0.5f, 0.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-0.125f, -0.55f, 0.0f);
        glVertex3f(0.125f, -0.55f, 0.0f);
        glVertex3f(0.125f, -0.5f, 0.0f);
        glVertex3f(-0.125f, -0.5f, 0.0f);

        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.125f, -0.55f, -0.3f);
        glVertex3f(-0.125f, -0.55f, -0.3f);
        glVertex3f(-0.125f, -0.5f, -0.3f);
        glVertex3f(0.125f, -0.5f, -0.3f);
        glEnd();
    }

    void DrawGunBarrel(const float radius, const float height) {
        // Sets materials
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SPECULAR_COL);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DARKGRAY_COL);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, GRAY_COL);
        //Gun Barrel
        glColor3ub(0, 0, 0);
        drawCylinder(radius, height);
    }

    void DrawSkybox(const float s) {
        float s2 = s / 2;

        // Turn on texture mapping and disable lighting
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);

        // Back wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBACK]);  // Specify which texture will be used
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(-s2, -s2, -s2);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(s2, -s2, -s2);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(s2, s2, -s2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(-s2, s2, -s2);
        glEnd();

        // Left wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYLEFT]);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(-s2, -s2, s2);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-s2, -s2, -s2);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-s2, s2, -s2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(-s2, s2, s2);
        glEnd();

        // Bottom wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYBOTTOM]);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-s2, -s2, s2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(s2, -s2, s2);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(s2, -s2, -s2);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-s2, -s2, -s2);
        glEnd();

        // Top Wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYTOP]);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-s2, s2, s2);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(s2, s2, s2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(s2, s2, -s2);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-s2, s2, -s2);
        glEnd();

        // Front Wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYFRONT]);  // Specify which texture will be used
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-s2, -s2, s2);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(s2, -s2, s2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(s2, s2, s2);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-s2, s2, s2);
        glEnd();

        // Right Wall
        glBindTexture(GL_TEXTURE_2D, textures[TID_SKYRIGHT]);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(s2, -s2, s2);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(s2, -s2, -s2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(s2, s2, -s2);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(s2, s2, s2);
        glEnd();

        // Turn off texture mapping and enable lighting
        glEnable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
    }

    void DrawLightSource(const float radius) {
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidSphere(radius, 20, 20);
        glEnable(GL_LIGHTING);
    }
}


