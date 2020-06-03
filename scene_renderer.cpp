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

    void RenderScene(const gmtl::Matrix44f& ppose) {
        SetLight(light_pos, true, true, true);

        glPushMatrix();
        glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
        scene_object::DrawLightSource(0.5);
        glPopMatrix();

        scene_object::DrawSkybox(5000);
        scene_object::RenderPlane(ppose);
    }
}

