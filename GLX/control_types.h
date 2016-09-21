// Requires "GLX.h" already included.

// Opaque button with no text or border
void empty_button()
{
    Color color;
    if (control->active)
        color = control->color["hilite"] ;
    else
        color = control->color["background"] ;
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    glRectf(
        control->rect[0], 
        control->rect[1],
        control->rect[2],
        control->rect[3]);
}

// Plain text field
void text_field()
{
    Color color = control->color["text"] ;
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    right_align_text(false, 4, 4);
}

// Plain text field
void text_field_editable()
{
    Color color = control->color["text"] ;
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    right_align_text(true, 4, 4);
}

// Bordered text field
void bordered_text_field() // Bordered text field
{
    Color color = control->color["border"];
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    glBegin(GL_LINE_LOOP);
        glVertex2d(control->rect[0], control->rect[1]);
        glVertex2d(control->rect[2], control->rect[1]);
        glVertex2d(control->rect[2], control->rect[3]);
        glVertex2d(control->rect[0], control->rect[3]);
    glEnd();
    
    color = control->color["text"] ;
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    right_align_text(false, 4, 4);
}

// Opaque (filed) text field
void opaque_text_field() // Opaque text field
{
    Color color = control->color["background"] ;
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    glRectf(
        control->rect[0], 
        control->rect[1],
        control->rect[2],
        control->rect[3]);
        
    color = control->color["text"] ;
    
    glColor3f(
        color[0], 
        color[1], 
        color[2]);
        
    right_align_text(false, 4, 4);
}
