//
// Created by Archawat on 6/2/2020.
//

#include "scene_renderer.h"
#include "scene_objects.h"
#include <GL/glut.h>
#include <gmtl/gmtl.h>

namespace scene_renderer
{
    // Lighting
    const GLfloat NO_LIGHT[] = { 0.0, 0.0, 0.0, 1.0 };
    const GLfloat AMBIENT_LIGHT[] = { 0.1, 0.1, 0.1, 1.0 };
    const GLfloat DIFFUSE_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };
    const GLfloat SPECULAR_LIGHT[] = { 0.5, 0.5, 0.5, 1.0 };

    gmtl::Point4f light_pos(0.0, 600.0, 0.0, 1.0);

    /* Plane Constant */
    // Plane dimensions
    const float P_WIDTH = 3.0f;
    const float P_LENGTH = 3.0f;
    const float P_HEIGHT = 1.5f;

    // Propeller dimensions (subpart)
    const float GUNBARREL_RADIUS = 0.02f;
    const float GUNBARREL_HEIGHT = 0.5f;

    // Propeller transforms
    const gmtl::Point3f WING_POS(0, 0, 0);     // Propeller position on the plane (w.r.t. plane's frame)
    const gmtl::Point3f PLATFORM_POS(0, 0, (P_HEIGHT * 2) / 3);
    const gmtl::Point3f GUNBARREL_POS(0.1f, -0.525f, -0.3f);
    const float PROPELLER_ROTATION = 0.5f;                  // Propeller rotated by 2 degs per input

    // Propeller rotation (subpart)
    bool isRotateUp = true;
    float pp_angle = 0;         // Rotation angle for subpart C
    float pp_angle_2 = 0;       // Rotation angle for subpart B
    float platform = 0;			// Rotation angle for subpart A

    void SceneInit() {

        // Disable global ambient
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, NO_LIGHT);

        // NOTE: for specular reflections, the "local viewer" model produces better
        // results than the default, but is slower. The default would not use the correct
        // vertex-to-eyepoint vector, treating it as always parallel to Z.
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

        // Disable two sided lqighting because we cannot see the inside of the most object in the scene (except satellite)
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

        // Enable lighting
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        //Load Texture
        scene_object::LoadTexture();
    }

    //|____________________________________________________________________
    //|
    //| Function: SetLight
    //|
    //! \param pos          [in] Light position.
    //! \param is_ambient   [in] Is ambient enabled?
    //! \param is_diffuse   [in] Is diffuse enabled?
    //! \param is_specular  [in] Is specular enabled?
    //! \return None.
    //!
    //! Set light properties.
    //|____________________________________________________________________
    void SetLight(const gmtl::Point4f& pos, const bool is_ambient, const bool is_diffuse, const bool is_specular)
    {
        glLightfv(GL_LIGHT0, GL_POSITION, pos.mData);
        if (is_ambient) {
            glLightfv(GL_LIGHT0, GL_AMBIENT, AMBIENT_LIGHT);
        }
        else {
            glLightfv(GL_LIGHT0, GL_AMBIENT, NO_LIGHT);
        }
        if (is_diffuse) {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE_LIGHT);
        }
        else {
            glLightfv(GL_LIGHT0, GL_DIFFUSE, NO_LIGHT);
        }
        if (is_specular) {
            glLightfv(GL_LIGHT0, GL_SPECULAR, SPECULAR_LIGHT);
        }
        else {
            glLightfv(GL_LIGHT0, GL_SPECULAR, NO_LIGHT);
        }
    }

    void AnimateWing() {
        if (isRotateUp) {
            pp_angle += PROPELLER_ROTATION;
            pp_angle_2 -= PROPELLER_ROTATION;
        }
        else {
            pp_angle -= PROPELLER_ROTATION;
            pp_angle_2 += PROPELLER_ROTATION;
        }

        if (pp_angle >= 30 || pp_angle_2 <= -30) {
            isRotateUp = false;
        }
        else if (pp_angle <= -10 || pp_angle_2 >= 10) {
            isRotateUp = true;
        }
    }

    void RenderPlane(const gmtl::Matrix44f& ppose) {
        // Plane 2 body:
        glPushMatrix();
        glMultMatrixf(ppose.mData);
        scene_object::DrawPlaneBody(P_WIDTH, P_LENGTH, P_HEIGHT);

        // LeftWing (subpart B):
        glPushMatrix();
        glTranslatef(WING_POS[0], WING_POS[1], WING_POS[2]);
        glRotatef(pp_angle, 0, 0, 1);
        scene_object::DrawLeftWing(P_WIDTH, P_LENGTH, P_HEIGHT);
        glPopMatrix();

        // RightWing (subpart C):
        glPushMatrix();
        glTranslatef(WING_POS[0], WING_POS[1], WING_POS[2]);
        glRotatef(pp_angle_2, 0, 0, 1);
        scene_object::DrawRightWing(P_WIDTH, P_LENGTH, P_HEIGHT);
        glPopMatrix();

        // Plaform (subpart A):
        glPushMatrix();
        glTranslatef(PLATFORM_POS[0], PLATFORM_POS[1], PLATFORM_POS[2]);
        glRotatef(platform, 0, 1, 0);
        scene_object::DrawPlatform(P_WIDTH, P_LENGTH, P_HEIGHT);

        //Left Gun Barrel (subsubpart A)
        glPushMatrix();
        glTranslatef(-GUNBARREL_POS[0], GUNBARREL_POS[1], GUNBARREL_POS[2]);
        scene_object::DrawGunBarrel(GUNBARREL_RADIUS, GUNBARREL_HEIGHT);
        glPopMatrix();

        //Right Gun Barrel (subsubpart B)
        glPushMatrix();
        glTranslatef(GUNBARREL_POS[0], GUNBARREL_POS[1], GUNBARREL_POS[2]);
        scene_object::DrawGunBarrel(GUNBARREL_RADIUS, GUNBARREL_HEIGHT);
        glPopMatrix();
        glPopMatrix();

        glPopMatrix();
    }

    void RenderScene(const gmtl::Matrix44f& ppose) {
        SetLight(light_pos, true, true, true);

        glPushMatrix();
        glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
        scene_object::DrawLightSource(0.5);
        glPopMatrix();

        scene_object::DrawSkybox(5000);
        RenderPlane(ppose);
    }
}

