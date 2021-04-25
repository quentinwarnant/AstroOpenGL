#include <Window/AWindow.h>

AWindow::AWindow(GLuint windowWidth, GLuint windowHeight, const char* title)
{
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;

    m_title = title;

	//Input
	for (size_t i = 0; i < 1024; i++)
	{
		m_keys[i] = false;
	}

	m_mousePosChangeX = m_mousePosChangeY = 0;
	m_mouseHasMoved = false;
	
}

AWindow::~AWindow()
{
	// Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_mainWindow);
		
	glfwTerminate();

}

int AWindow::Init()
{

    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return 1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x multisampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	m_mainWindow = glfwCreateWindow( m_windowWidth, m_windowHeight, m_title, NULL, NULL);
	if( m_mainWindow == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return 1;
	}

	glfwGetFramebufferSize(m_mainWindow, &m_windowBufferWidth, &m_windowBufferHeight);

	glfwMakeContextCurrent(m_mainWindow);
	glewExperimental=true; // Needed in core profile

	//input
	glfwSetWindowUserPointer( m_mainWindow, this);
	glfwSetInputMode(m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	InitCallbacks();

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwDestroyWindow(m_mainWindow);
		glfwTerminate();
		return 1;
	}
    CheckForGLError("after glewInit - Safe to ignore (?)");


	GLint type = -1;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &type );
    printf("depth attachement object type: %i\n ", type); //GL_FRAMEBUFFER_DEFAULT
    CheckForGLError("depth attachement object type");

    GLint depthSize = -1;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depthSize );
    printf("depthSize: %i bits\n ", depthSize);

	glViewport(0,0,m_windowBufferWidth, m_windowBufferHeight);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_mainWindow, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.4f, 0.4f, 0.5f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
    glDepthFunc(GL_LESS);

    CheckForGLError("after enabling depth");

    return 0;
}

void AWindow::CheckForGLError(const char* msg){
    GLenum err = glGetError();
    if( err != GL_NO_ERROR)
    {
        fprintf(stderr, msg);
        fprintf(stderr, " GL ERROR - ");

        if( err == GL_INVALID_FRAMEBUFFER_OPERATION)
        {
            fprintf(stderr, "GL_INVALID_FRAMEBUFFER_OPERATION\n");
        }
        if( err == GL_INVALID_ENUM)
        {
            fprintf(stderr, "GL_INVALID_ENUM\n");
        }
        if( err == GL_INVALID_VALUE)
        {
            fprintf(stderr, "GL_INVALID_VALUE\n");
        }
        if( err == GL_INVALID_OPERATION)
        {
            fprintf(stderr, "GL_INVALID_OPERATION\n");
        }
        if( err == GL_INVALID_FRAMEBUFFER_OPERATION)
        {
            fprintf(stderr, "GL_INVALID_FRAMEBUFFER_OPERATION\n");
        }
        if( err == GL_OUT_OF_MEMORY)
        {
            fprintf(stderr, "GL_OUT_OF_MEMORY\n");
        }
        if( err == GL_STACK_UNDERFLOW)
        {
            fprintf(stderr, "GL_STACK_UNDERFLOW\n");
        }
        if( err == GL_STACK_OVERFLOW)
        {
            fprintf(stderr, "GL_STACK_OVERFLOW\n");
        }
    }
}
void AWindow::InitCallbacks()
{
	glfwSetKeyCallback(m_mainWindow, HandleKeyboardInput);
	glfwSetCursorPosCallback(m_mainWindow, HandleMouseInput);
}

void AWindow::PreUpdate()
{
}

void AWindow::Update()
{

}

void AWindow::PostUpdate()
{
}

void AWindow::HandleKeyboardInput(GLFWwindow* window, int key, int code, int action, int mode)
{
	AWindow* thisWindow = static_cast<AWindow*>(glfwGetWindowUserPointer(window)); 

	if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if( key >= 0 && key < 1024)
	{
		if( action == GLFW_PRESS )
		{
			thisWindow->m_keys[key] = true;
			printf("pressed key %d\n ", key);
		}
		else if(  action == GLFW_RELEASE && thisWindow->m_keys[key] ) // not pressing & key was pressed before
		{
			thisWindow->m_keys[key] = false;
			printf("released key %d\n ", key);

		}
	}
}

void AWindow::HandleMouseInput(GLFWwindow* window, double mousePosX, double mousePosY)
{
	AWindow* thisWindow = static_cast<AWindow*>(glfwGetWindowUserPointer(window)); 

	if( thisWindow->m_mouseHasDoneInitialPositioning == false )
	{
		//Doubles into float?
		thisWindow->m_mouseLastPosX = mousePosX;
		thisWindow->m_mouseLastPosY = mousePosY;
		thisWindow->m_mouseHasDoneInitialPositioning = true;
	}

	//Calculate diff since last call
	thisWindow->m_mousePosChangeX = mousePosX - thisWindow->m_mouseLastPosX;
	thisWindow->m_mousePosChangeY = mousePosY - thisWindow->m_mouseLastPosY;

	// Set mouse pos values ahead of next call
	thisWindow->m_mouseLastPosX = mousePosX;
	thisWindow->m_mouseLastPosY = mousePosY;

	thisWindow->m_mouseHasMoved = true;
}

bool AWindow::IsKeyPressed(int keyId)
{
	return m_keys[keyId];
}

void AWindow::GetMouseChange(GLfloat& mousePosChangeX, GLfloat& mousePosChangeY, bool consume)
{
	if(m_mouseHasMoved)
	{
		mousePosChangeX = *(&m_mousePosChangeX);
		mousePosChangeY = *(&m_mousePosChangeY);
	}
	else
	{
		mousePosChangeX = 0;
		mousePosChangeY = 0;
	}

	if( consume )
	{
		m_mouseHasMoved = false;
	}
}