class Scene
{
public:
    Scene();
    virtual ~Scene();

public:
    void init();
    
    //: These functions call the displaylists 
    //  created by init()
    void drawFloor();
    void drawSphere( const float& width, 
		    const float& height,
		    const float& depth, 
		    const bool& highlighted );
    void drawCone( const float& width, 
		    const float& height,
		    const float& depth, 
		    const bool& highlighted );
    void drawCube( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void drawTable();  
    
    
    
//:  Scene components: table, floor, and some objects.    
private:
    //: these functions render the geometry using opengl functions
    void _drawTable();  
    void _drawFloor();
    void _drawSphere( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void _drawCone( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    void _drawCube( const float& _width, 
		    const float& _height,
		    const float& _depth, 
		    const bool& highlighted );
    
    // these hold the display list IDs
    int _tableList;
    int _floorList;
    
    int _coneList;
    int _cubeList;
    int _sphereList;
    
    int _coneHlList;
    int _cubeHlList;
    int _sphereHlList; 
    
    char woodBitmap[16*3]; 
};

inline Scene::Scene() : _tableList(0), 
		    _floorList(0), 
		    _coneList(0), 
		    _coneHlList(0), 
		    _cubeList(0), 
		    _cubeHlList(0), 
		    _sphereList(0), 
		    _sphereHlList(0)
{
    
}

inline Scene::~Scene()
{
    glDeleteLists(_tableList, 1);
    glDeleteLists(_floorList, 1);
    glDeleteLists(_coneList, 1);
    glDeleteLists(_coneHlList, 1);
    glDeleteLists(_cubeList, 1);
    glDeleteLists(_cubeHlList, 1);
    glDeleteLists(_sphereList, 1);
    glDeleteLists(_sphereHlList, 1);
}

inline void Scene::drawFloor()
{
    glCallList( _floorList );
}

inline void Scene::drawSphere( const float& width, 
		const float& height,
		const float& depth, 
		const bool& highlighted )
{
    glPushMatrix();
	glScalef( width, height, depth );
	if (highlighted)
	    glCallList( _sphereHlList );
	else glCallList( _sphereList );
    glPopMatrix();
}

inline void Scene::drawCone( const float& width, 
		const float& height,
		const float& depth, 
		const bool& highlighted )
{
    glPushMatrix();
	glScalef( width, height, depth );
	if (highlighted)
	    glCallList( _coneHlList );
	else glCallList( _coneList );
    glPopMatrix();
}

inline void Scene::drawCube( const float& width, 
		const float& height,
		const float& depth, 
		const bool& highlighted )
{
    glPushMatrix();
	glScalef( width, height, depth );
	if (highlighted)
	    glCallList( _cubeHlList );
	else glCallList( _cubeList );
    glPopMatrix();
}

inline void Scene::drawTable()
{
    glCallList( _tableList );
}



///////////////////////////////////////////////
// Private... shape drawing routines:
///////////////////////////////////////////////

inline void Scene::_drawFloor()
{
  // floor
  for (int y = -10; y < 10; ++y)
  {
     //glColor3f( 1.0f, 1.0f, 1.0f );
     glPolygonMode( GL_FRONT, GL_LINE );
     glLineWidth( 2 );
     glBegin(GL_TRIANGLE_STRIP);
     for (int x = -10; x < 10; ++x)
     {
	glColor3f( 0.7f, 0.4f, 0.4f );
	glNormal3f( 0.1f, 0.9f, -0.1f );
	glVertex3f( (float)x, 0.01f,  y );
	
	glColor3f( 0.0f, 0.0f, 0.4f );
	glNormal3f( -0.1f, 0.9f, 0.1f );
	glVertex3f( (float)x, 0.01f,  y + 1.0f );
     }
     glEnd();
     
     glPolygonMode( GL_FRONT,  GL_FILL );
     glBegin(GL_TRIANGLE_STRIP);
     for ( x = -10; x < 10; ++x)
     {
	glColor3f( 0.0f, 0.0f, 0.4f );
	glNormal3f( 0.1f, 0.9f, -0.1f );
	glVertex3f( (float)x, 0.0f,  y );
	
	glColor3f( 0.5f, 0.5f, 0.0f );
	glNormal3f( -0.1f, 0.9f, 0.1f );
	glVertex3f( (float)x, 0.0f,  y + 1.0f );
     }
     glEnd();
  }
}

inline void Scene::_drawSphere( const float& width, 
                                  const float& height,
                                  const float& depth, 
                                  const bool& highlighted )
{
    static GLUquadric* sphereQuadric = gluNewQuadric();
    
    glPushMatrix();
	glScalef( width, height, depth );
	gluSphere(sphereQuadric, 0.5f, 15, 15);
    glPopMatrix();
    
    // draw the highlight if requested.
    if ( highlighted == true )
    {
	glColor3f( 0.0f, 0.0f, 1.0f );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 2 );
	this->drawSphere( width+0.01f, height+0.01f, depth+0.01f, false );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

inline void Scene::_drawCone( const float& width, 
                                const float& height,
                                const float& depth, 
                                const bool& highlighted )
{
    static GLUquadric* cylinderQuadric = gluNewQuadric();
    
    glPushMatrix();
	glScalef( width, height, depth );
	gluCylinder(cylinderQuadric, 0.5f, 0.0f, 1.0f, 15, 15);
    glPopMatrix();
    
    // draw the highlight if requested.
    if ( highlighted == true )
    {
	glColor3f( 0.0f, 0.0f, 1.0f );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 2 );
	this->drawCone( width+0.01f, height+0.01f, depth+0.01f, false );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

inline void Scene::_drawCube( const float& _width, 
		                const float& _height,
		                const float& _depth, 
		                const bool& highlighted )
{
    float width = _width * 0.5f;
    float height = _height * 0.5f;
    float depth = _depth * 0.5f;
    
    // cube top
  glBegin(GL_TRIANGLE_STRIP);
     glNormal3f( -0.75f, 0.75f, 0.75f );
     glVertex3f( -width, height,  depth );
     
     glNormal3f(  0.75f, 0.75f, 0.75f );
     glVertex3f(  width, height,  depth );
     
     glNormal3f( -0.75f, 0.75f, -0.75f );
     glVertex3f( -width, height, -depth );
     
     glNormal3f(  0.75f, 0.75f, -0.75f );
     glVertex3f(  width, height, -depth );
  glEnd();
    
    // cube bottom
  glBegin(GL_TRIANGLE_STRIP);
     glNormal3f(  0.75f, -0.75f, 0.75f );
     glVertex3f(  width, -height,  depth );
     
     glNormal3f( -0.75f, -0.75f, 0.75f );
     glVertex3f( -width, -height,  depth );
     
     glNormal3f(  0.75f, -0.75f, -0.75f );
     glVertex3f(  width, -height, -depth );
     
     glNormal3f( -0.75f, -0.75f, -0.75f );
     glVertex3f( -width, -height, -depth );
  glEnd();
  
    // cube sides
  glBegin(GL_TRIANGLE_STRIP);
     glNormal3f( -0.75f, 0.75f, 0.75f );
     glVertex3f( -width, height,  depth );
     
     glNormal3f( -0.75f, -0.75f, 0.75f );
     glVertex3f( -width, -height, depth );
     
     glNormal3f( 0.75f, 0.75f, 0.75f );
     glVertex3f( width, height,  depth );
     
     glNormal3f( 0.75f, -0.75f, 0.75f );
     glVertex3f( width, -height, depth );
     
     glNormal3f( 0.75f, 0.75f, -0.75f );
     glVertex3f( width, height,  -depth );
     
     glNormal3f( 0.75f, -0.75f, -0.75f );
     glVertex3f( width, -height, -depth );
     
     glNormal3f( -0.75f, 0.75f, -0.75f );
     glVertex3f( -width, height,  -depth );
     
     glNormal3f( -0.75f, -0.75f, -0.75f );
     glVertex3f( -width, -height, -depth );
     
     glNormal3f( -0.75f, 0.75f, 0.75f );
     glVertex3f( -width, height,  depth );
     
     glNormal3f( -0.75f, -0.75f, 0.75f );
     glVertex3f( -width, -height, depth );
  glEnd();
  
  // draw the highlight if requested.
    if ( highlighted == true )
    {
	glColor3f( 0.0f, 0.0f, 1.0f );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth( 2 );
	this->drawCube( _width+0.01f, _height+0.01f, _depth+0.01f, false );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

inline void Scene::_drawTable()
{
  glColor3f( 0.4f, 0.3f, 0.1f );
  
    // table top (top side) - flat shaded
    glPushMatrix();
	glTranslatef(0.0f, 2.85f, 0.0f);
	this->drawCube( 10.0f, 0.25f, 4.0f, false );
    glPopMatrix();    
    
    // Table Legs:
    glPushMatrix();
	glTranslatef( -4.65f, 1.5f, 1.65f );
	this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();
    
    glPushMatrix();
	glTranslatef( 4.65f, 1.5f, -1.65f );
	this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();
    
    glPushMatrix();
	glTranslatef( -4.65f, 1.5f, -1.65f );
	this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();
    
    glPushMatrix();
	glTranslatef( 4.65f, 1.5f, 1.65f );
	this->drawCube( 0.25f, 3.0f, 0.25f, false );
    glPopMatrix();
}

inline void Scene::init()
{
    _tableList = glGenLists(1);
    _floorList = glGenLists(1);
    
    _coneList = glGenLists(1);
    _cubeList = glGenLists(1);
    _sphereList = glGenLists(1);
    
    _coneHlList = glGenLists(1);
    _cubeHlList = glGenLists(1);
    _sphereHlList = glGenLists(1); 
    
    glNewList(_tableList, GL_COMPILE);
	this->_drawTable();
    glEndList();
    
    glNewList(_floorList, GL_COMPILE);
	this->_drawFloor();
    glEndList();
    
    glNewList(_coneList, GL_COMPILE);
	this->_drawCone( 1.0f, 1.0f, 1.0f, false);
    glEndList();
    
    glNewList(_coneHlList, GL_COMPILE);
	this->_drawCone( 1.0f, 1.0f, 1.0f, true);
    glEndList();
    
    glNewList(_cubeList, GL_COMPILE);
	this->_drawCube( 1.0f, 1.0f, 1.0f, false);
    glEndList();
    
    glNewList(_cubeHlList, GL_COMPILE);
	this->_drawCube( 1.0f, 1.0f, 1.0f, true);
    glEndList();
    
    glNewList(_sphereList, GL_COMPILE);
	this->_drawSphere( 1.0f, 1.0f, 1.0f, false);
    glEndList();
    
    glNewList(_sphereHlList, GL_COMPILE);
	this->_drawSphere( 1.0f, 1.0f, 1.0f, true);
    glEndList();
}
