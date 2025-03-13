#pragma once

#include <wx/wx.h>
#include <GL/glew.h>
#include <wx/glcanvas.h>

class GLCanvas : public wxGLCanvas {
public:
    GLCanvas(wxFrame* parent);
    ~GLCanvas() = default;
    
    void Render(const unsigned int* buffer);
    void InitGL();
    void OnPaint(wxPaintEvent &event);
    
private:
    wxGLContext* gl_context;
    GLuint texture, shaderProgram, VAO, VBO, EBO;
};