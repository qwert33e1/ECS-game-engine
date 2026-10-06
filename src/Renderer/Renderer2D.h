#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include "common.h"
#include "Utility/Transformation.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

struct VtxData
{
    glm::vec2 pos;
    glm::vec2 tex;
    glm::vec4 color;

    VtxData(glm::vec2 pos, glm::vec2 tex, glm::vec4 color) : pos(pos), tex(tex), color(color) {}
};

class Renderer2D
{
    const char *vertSource = R"(
	#version 330
	uniform mat4 MVP;
	layout(location = 0) in vec2 vtxPos;
	layout(location = 1) in vec2 vtxTex; 
	layout(location = 2) in vec4 vtxColor; 
	out vec2 v_texCoord;
    out vec4 v_color;
	void main() {
		v_texCoord = vtxTex;
		gl_Position = MVP * vec4(vtxPos, 0, 1); 
        v_color = vtxColor;
    }
)";

    const char *fragSource = R"(
	#version 330
	uniform sampler2D textureUnit;
	in vec2 v_texCoord;
	in vec4 v_color;
	out vec4 fragmentColor;

	void main() {
		fragmentColor = texture(textureUnit, v_texCoord) * v_color;
	}
)";

    unsigned int shaderId;
    unsigned int vao, vbo;
    std::vector<VtxData> vtx;

    unsigned int whiteTexture;
    unsigned int currentTexture;

public:
    Renderer2D()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        int nb = sizeof(VtxData);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, nb, (void *)offsetof(VtxData, pos));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, nb, (void *)offsetof(VtxData, tex));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, nb, (void *)offsetof(VtxData, color));

        shaderId = createShader();

        glGenTextures(1, &whiteTexture);
        glBindTexture(GL_TEXTURE_2D, whiteTexture);
        unsigned char whitePixel[] = {255, 255, 255, 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void updateGPU()
    {
        if (vtx.size() > 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(VtxData), &vtx[0], GL_DYNAMIC_DRAW);
        }
    }

    void Bind()
    {
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    void Flush()
    {
        if (vtx.size() > 0)
        {
            updateGPU();

            glUseProgram(shaderId);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, currentTexture);

            glm::mat4 MVP = glm::ortho(0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);

            int mvpLocation = glGetUniformLocation(shaderId, "MVP");
            glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(MVP));

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, (int)vtx.size());

            Clear();
        }
    }

    /// @param pos the middle of the quad
    void AddQuad(glm::vec2 pos, float w, float h, float rotation, glm::vec4 color, unsigned int textureId, glm::vec2 uvMin = glm::vec2(0.0f, 0.0f), glm::vec2 uvMax = glm::vec2(1.0f, 1.0f))
    {
        if (textureId != currentTexture && !vtx.empty())
        {
            Flush();
        }

        currentTexture = textureId;

        float halfW = w / 2.0f;
        float halfH = h / 2.0f;

        glm::vec2 tr = glm::vec2(halfW, halfH);
        glm::vec2 tl = glm::vec2(-halfW, halfH);
        glm::vec2 br = glm::vec2(halfW, -halfH);
        glm::vec2 bl = glm::vec2(-halfW, -halfH);

        tr = Transformation::rotatePoint(tr, pos, rotation);
        tl = Transformation::rotatePoint(tl, pos, rotation);
        br = Transformation::rotatePoint(br, pos, rotation);
        bl = Transformation::rotatePoint(bl, pos, rotation);

        vtx.push_back(VtxData(bl, glm::vec2(uvMin.x, uvMin.y), color));
        vtx.push_back(VtxData(br, glm::vec2(uvMax.x, uvMin.y), color));
        vtx.push_back(VtxData(tl, glm::vec2(uvMin.x, uvMax.y), color));
        vtx.push_back(VtxData(tl, glm::vec2(uvMin.x, uvMax.y), color));
        vtx.push_back(VtxData(br, glm::vec2(uvMax.x, uvMin.y), color));
        vtx.push_back(VtxData(tr, glm::vec2(uvMax.x, uvMax.y), color));
    }

    unsigned int createShader()
    {
        int res;

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, (const GLchar **)&vertSource, NULL);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &res);
        if (!res)
        {
            exit(1);
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, (const GLchar **)&fragSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &res);
        if (!res)
        {
            exit(1);
        }

        unsigned int programId = glCreateProgram();

        glAttachShader(programId, vertexShader);
        glAttachShader(programId, fragmentShader);

        glLinkProgram(programId);

        glGetProgramiv(programId, GL_LINK_STATUS, &res);
        if (!res)
        {
            exit(1);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return programId;
    }

    void Clear()
    {
        vtx.clear();
    }

    virtual ~Renderer2D()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(shaderId);
    }
};
