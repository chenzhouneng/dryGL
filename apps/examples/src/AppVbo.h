//
//  AppVbo.h
//
//  Example: How to setup VertexBufferObjects and use them for custom rendering
//

#pragma once

#include "dry.h"
#include "Shaders.h"

class AppVbo : public dry::AppiOS
{
public:
    
    AppVbo(dry::AppParams const &params) : dry::AppiOS(params) { }

    //------------------------------------------------------------------------------------------------
    // OnInit
    //------------------------------------------------------------------------------------------------
    void OnInit()
    {
        // Vertices
        GLfloat vertices[] = {
            // front
            -1.0, -1.0,  1.0,
            1.0, -1.0,  1.0,
            1.0,  1.0,  1.0,
            -1.0,  1.0,  1.0,
            // top
            -1.0,  1.0,  1.0,
            1.0,  1.0,  1.0,
            1.0,  1.0, -1.0,
            -1.0,  1.0, -1.0,
            // back
            1.0, -1.0, -1.0,
            -1.0, -1.0, -1.0,
            -1.0,  1.0, -1.0,
            1.0,  1.0, -1.0,
            // bottom
            -1.0, -1.0, -1.0,
            1.0, -1.0, -1.0,
            1.0, -1.0,  1.0,
            -1.0, -1.0,  1.0,
            // left
            -1.0, -1.0, -1.0,
            -1.0, -1.0,  1.0,
            -1.0,  1.0,  1.0,
            -1.0,  1.0, -1.0,
            // right
            1.0, -1.0,  1.0,
            1.0, -1.0, -1.0,
            1.0,  1.0, -1.0,
            1.0,  1.0,  1.0,
        };
        // TexCoords
        GLfloat texcoords[2*4*6] = {
            0.0, 0.0,
            1.0, 0.0,
            1.0, 1.0,
            0.0, 1.0,
        };
        for (int i = 1; i < 6; i++)
            memcpy(&texcoords[i*4*2], &texcoords[0], 2*4*sizeof(GLfloat));
        // Indices
        GLushort indices[] = {
            // front
            0,  1,  2,
            2,  3,  0,
            // top
            4,  5,  6,
            6,  7,  4,
            // back
            8,  9, 10,
            10, 11,  8,
            // bottom
            12, 13, 14,
            14, 15, 12,
            // left
            16, 17, 18,
            18, 19, 16,
            // right
            20, 21, 22,
            22, 23, 20,
        };

        // Buffers
        Indices.Init(indices, 36, dry::DataTypeUShort, false);
        Vertices.Init(vertices, 24, dry::DataTypeVec3, false);
        TexCoords.Init(texcoords, 24, dry::DataTypeVec2, false);

        // Texture
        dry::ImageUtils::Load(Texture, dry::GetFilePath("grid.jpg"), dry::Texture::Params(false, false, false));
        
        // Shader
        Shader.Load(dry::Shaders::Texture2D_VS, dry::Shaders::Texture2D_FS);
        
        // Camera
        int w = GetParams().Width;
        int h = GetParams().Height;
        Camera.Init(45.f, (float)w / h, 0.1f, 10000.f);
        Camera.LookAt(glm::vec3(0.0, 2.0, -8.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        
        // Uniforms
        UTexture = Shader.GetUniformByName("Texture");
        UTexture->Update(0);
        UModel = Shader.GetUniformByName("Model");
        UView = Shader.GetUniformByName("View");
        UProjection = Shader.GetUniformByName("Projection");
    }

    //------------------------------------------------------------------------------------------------
    // OnDraw
    //------------------------------------------------------------------------------------------------
    void OnDraw()
    {
        m_Renderer->Clear(true, true, false);
        
        // Bind
        Texture.Bind(0);
        Shader.Bind();

        // Matrices
        float angle = GetTimer().GetTime() * 45;
        glm::mat4 rotation = glm::rotate(angle, glm::vec3(0, 1, 0));
        glm::mat4 position = glm::translate(glm::vec3(0.0, 0.0, 0.0));
        UModel->Update(position * rotation);
        UView->Update(Camera.GetMatView());
        UProjection->Update(Camera.GetMatProjection());

        // Buffers
        Vertices.Bind(Shader.GetAttribByName("Position"));
        TexCoords.Bind(Shader.GetAttribByName("TexCoord"));
        Indices.Bind();
        
        // Draw!
        m_Renderer->DrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT);
        
        // Unbind
        Vertices.Unbind();
        TexCoords.Unbind();
        Indices.Unbind();
    }

private:

    dry::CameraPerspective  Camera;
    dry::Shader             Shader;
    dry::Texture            Texture;
    dry::Vbo                Vertices;
    dry::Vbo                TexCoords;
    dry::Ibo                Indices;
    dry::UniformInterface   *UTexture;
    dry::UniformInterface   *UModel;
    dry::UniformInterface   *UView;
    dry::UniformInterface   *UProjection;
};
