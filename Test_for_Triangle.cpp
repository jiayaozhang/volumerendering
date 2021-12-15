//
// CS247_prog3
// AMCS/CS247 Programming Assignment 4
//


#include "CS247_Assignment4.h"

#include "tf_tables.h"

#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#define BUFFER_WIDTH 1024
#define BUFFER_HEIGHT 1024

char LOBSTER_DAT[] = "../resource/lobster.dat";
char SKEWED_HEAD_DAT[] = "../resource/skewed_head.dat";

char RAYCAST_DVR_GLSL[] = "../src/raycast_dvr.glsl";
char RAYCAST_ISO_GLSL[] = "../src/raycast_iso.glsl";

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
unsigned int VBO, VAO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

int printOglError( char *file, int line )
{
	//
	// Returns 1 if an OpenGL error occurred, 0 otherwise.
	//
	GLenum glErr;
	int retCode = 0;
	const char *description;

	glErr = glfwGetError( &description );
	while ( glErr != GLFW_NO_ERROR ) {
		printf("glError in file %s @ line %d: %s\n", file, line, description );
		retCode = 1;
		glErr = glfwGetError( &description );
	}
	return retCode;
}


// ==========================================================================
// Parse GL_VERSION and return the major and minor numbers in the supplied
// integers.
// If it fails for any reason, major and minor will be set to 0.
// Assumes a valid OpenGL context.
void getGlVersion(int *major, int *minor)
{
	glfwGetVersion(major, minor, NULL);
}


/*
rotating background color
*/
static void NextClearColor( void )
{
    static int color = 0;

    switch( color++ )
    {
        case 0:
			clear_color[ 0 ] = 0.0f;
			clear_color[ 1 ] = 0.0f;
			clear_color[ 2 ] = 0.0f;
			clear_color[ 3 ] = 1.0f;
			break;
        case 1:
			clear_color[ 0 ] = 0.2f;
			clear_color[ 1 ] = 0.2f;
			clear_color[ 2 ] = 0.3f;
			clear_color[ 3 ] = 1.0f;
			break;
        default:
			clear_color[ 0 ] = 0.7f;
			clear_color[ 1 ] = 0.7f;
			clear_color[ 2 ] = 0.7f;
			clear_color[ 3 ] = 1.0f;
			color = 0;
			break;
    }
	glClearColor( clear_color[ 0 ], clear_color[ 1 ], clear_color[ 2 ], clear_color[ 3 ] );
}

/*
switch between MIP or not
*/
static void SwitchMIP(void)
{
	if (MIP_switch == false)
		MIP_switch = true;
	else
		MIP_switch = false;
}


/******************************************************************************/
/*
 * GLUT glue
 *
 ******************************************************************************/

static
void display( void )
{
	std::cout <<"inside"<<std::endl;
	glClearColor(1.f, 0.5f, 1.f, 1.f);
	// reshape for offscreen render pass
	// set viewport
	//glViewport( 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT );
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();

	// set perspective projection
	//glFrustum( -1.1, 1.1, -1.1, 1.1, 3.0, 10.0 );

	// glMatrixMode( GL_MODELVIEW );
	// glLoadIdentity();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// // object translation
	// glTranslatef( 0.0, 0.0, -5.0 );

	// // object rotation
    // glRotatef( fYDiff, 1, 0, 0 );
    // glRotatef( fXDiff, 0, 1, 0 );
    // glRotatef( fZDiff, 0, 0, 1 );
	
	// // =============================================================
	// // TODO: perform raycasting steps here
	// // i.e. render frontfaces and backfaces, then cast
	// // =============================================================

	// int max_dim = maximum(vol_dim[0], vol_dim[1], vol_dim[2]);
	// float dim_x = (float)vol_dim[0] / (float)max_dim;
	// float dim_y = (float)vol_dim[1] / (float)max_dim;
	// float dim_z = (float)vol_dim[2] / (float)max_dim;


	// RenderFrontFaces(dim_x, dim_y, dim_z);
	// RenderBackFaces (dim_x, dim_y, dim_z);
	// //RenderFrontFaces(1, 1, 1);
	// //RenderBackFaces(1, 1, 1);

	// //RenderBufferToScreen(frontface_buffer);
	// //RenderBufferToScreen(backface_buffer);

	// RenderRaycastPass();
    /*
	glBegin(GL_TRIANGLES);
	glColor3f(0.1, 0.2, 0.3);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();

    glFlush();
	*/
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);

}


static
void play( void )
{
    double thisTime = glfwGetTime();
}


static
void key( GLFWwindow *window, int keyPressed, int scancode, int action, int mods ) // key handling
{
	switch ( keyPressed ) {
		case GLFW_KEY_1:
			LoadData( LOBSTER_DAT );
			break;
		case GLFW_KEY_2:
			LoadData( SKEWED_HEAD_DAT );
			break;
		case GLFW_KEY_O:
			enable_lighting = 1 - enable_lighting;
			if( enable_lighting ) fprintf( stderr, "lighting is now enabled\n" );
			else fprintf( stderr, "lighting is now disabled\n" );
			break;
		case GLFW_KEY_G:
			enable_gm_scaling = 1 - enable_gm_scaling;
			if( enable_gm_scaling ) fprintf( stderr, "gradient magnitude scaled shading is now enabled\n" );
			else fprintf( stderr, "gradient magnitude scaled shading is now disabled\n" );
			break;
		case GLFW_KEY_F:
			use_dvr = 1 - use_dvr;
			if( use_dvr ) fprintf( stderr, "DVR Rendering\n" );
			else fprintf( stderr, "Iso Value Rendering\n" );
			break;
		case GLFW_KEY_I:
			if ( iso_value < 1.0f ) {
				iso_value += 0.05f;
			}
			fprintf( stderr, "increasing iso value to: %f\n", iso_value );
			break;
		case GLFW_KEY_K:
			if ( iso_value > 0.0f ) {
				iso_value -= 0.05f;
			}
			fprintf( stderr, "decreasing iso value to: %f\n", iso_value );
			break;
		case GLFW_KEY_A:
			if ( ambient < 1.0f ) {
				ambient += 0.05f;
			}
			fprintf( stderr, "increasing ambient brightness value to: %f\n", ambient );
			break;
		case GLFW_KEY_Z:
			if ( ambient > 0.0f ) {
				ambient -= 0.05f;
			}
			fprintf( stderr, "decreasing ambient brightness value to: %f\n", ambient );
			break;
		case GLFW_KEY_B:
			NextClearColor();
			break;
		case GLFW_KEY_M:
			SwitchMIP();
			break;
		case GLFW_KEY_SPACE:
            Rotate = !Rotate;

            if ( !Rotate ) {
                fXInertiaOld = fXInertia;
                fYInertiaOld = fYInertia;
            } else {
                fXInertia = fXInertiaOld;
                fYInertia = fYInertiaOld;

                // to prevent confusion, force some rotation
				if ( ( fXInertia == 0.0 ) && ( fYInertia == 0.0 ) ) {
                    fXInertia = -0.5;
				}
            }
            break;
		case GLFW_KEY_R: //return to inital state/view
			fXDiff    = 0;
			fYDiff    = 35;
			fZDiff    = 0;
			xLastIncr = 0;
			yLastIncr = 0;
			fXInertia = -0.5;
			fYInertia = 0;
			break;
        case GLFW_KEY_KP_ADD:
            step_size /= 1.5f;
			fprintf( stderr, "stepsize is now: %f\n", step_size );
            break;
        case GLFW_KEY_KP_SUBTRACT:
            step_size *= 1.5f;
			fprintf( stderr, "stepsize is now: %f\n", step_size );
            break;
		case GLFW_KEY_W:
			tf_win_min = std::min( tf_win_min + 1, tf_win_max );
			UpdateTransferfunction();
			fprintf( stderr, "lower window boundary increased to: %d\n", tf_win_min );
            break;
		case GLFW_KEY_S:
			tf_win_min = std::max( tf_win_min - 1, 0 );
			UpdateTransferfunction();
			fprintf( stderr, "lower window boundary decreased to: %d\n", tf_win_min );
            break;
		case GLFW_KEY_E:
			tf_win_max = std::min( tf_win_max + 1, 127 );
			UpdateTransferfunction();
			fprintf( stderr, "lower window boundary increased to: %d\n", tf_win_max );
            break;
		case GLFW_KEY_D:
			tf_win_max = std::max( tf_win_max - 1, tf_win_min );
			UpdateTransferfunction();
			fprintf( stderr, "lower window boundary decreased to: %d\n", tf_win_max );
            break;
        case GLFW_KEY_5:
			DownloadTransferFunctionTexture( 0 );
            break;
        case GLFW_KEY_6:
			DownloadTransferFunctionTexture( 1 );
            break;
        case GLFW_KEY_7:
			DownloadTransferFunctionTexture( 2 );
            break;
        case GLFW_KEY_8:
			DownloadTransferFunctionTexture( 3 );
            break;
        case GLFW_KEY_9:
			DownloadTransferFunctionTexture( 4 );
            break;
        case GLFW_KEY_0:
			DownloadTransferFunctionTexture( 5 );
            break;
		case GLFW_KEY_Q:
        case GLFW_KEY_ESCAPE:
            exit( 0 );
            break;
		case GLFW_KEY_HOME: //return to inital state/view
			fXDiff    = 0;
			fYDiff    = 35;
			fZDiff    = 0;
			xLastIncr = 0;
			yLastIncr = 0;
			fXInertia = -0.5;
			fYInertia = 0;
			fScale    = 1.0;
			break;
        case GLFW_KEY_LEFT:
			fXDiff--;
			break;
        case GLFW_KEY_RIGHT:
			fXDiff++;
			break;
        case GLFW_KEY_UP:
			fYDiff--;
			break;
        case GLFW_KEY_DOWN:
			fYDiff++;
			break;
		default:
			fprintf( stderr, "\nKeyboard commands:\n\n"
			"1 - Load lobster dataset\n"
			"2 - Load head dataset\n" 
			"p - Load test dataset\n"
			"b - Toggle among background clear colors\n"
			"m - Switch between MIP or not"
			"o - en-/disable lighting\n"
			"g - use gradient magnitude for shading (highlight edges)\n"
			"f - switch between DVR and Iso value rendering\n"
			"i, k - in-/decrease iso value\n"
			"a, z - in-/decrease ambient brightness\n"
			"w, s - in-/decrease lower tf window value\n"
			"e, d - in-/decrease upper tf window value\n"
			"5,6,7,8,9,0 - select transferfunctions\n"
			"<home>	 - reset zoom and rotation\n"
			"r - reset rotation\n"
            "<space> or <click> - stop rotation\n"
            "<+>, <-> - in-/decrease sampling rate\n"
			"<ctrl + drag> - lock rotation around z-axis\n"
            "<arrow keys> or <drag> - rotate model\n"
			"q, <esc> - Quit\n" );
			break;
	}
}



static
void timer( int value )
{

    /* Increment wrt inertia */
    if ( Rotate )
    {
        fXDiff = fXDiff + fXInertia;
        fYDiff = fYDiff + fYInertia;
    }
}


static
void mouse( GLFWwindow *window, int button, int state, int mods )
{ 

	if ( button == GLFW_MOUSE_BUTTON_LEFT ) { //rotate 
      
		if ( state == GLFW_RELEASE ) {
			xLast = -1;
			yLast = -1;

			if ( xLastIncr > INERTIA_THRESHOLD ) {
				fXInertia = ( xLastIncr - INERTIA_THRESHOLD ) * INERTIA_FACTOR;
			}

			if ( -xLastIncr > INERTIA_THRESHOLD ) {
				fXInertia = ( xLastIncr + INERTIA_THRESHOLD ) * INERTIA_FACTOR;
			}

			if ( yLastIncr > INERTIA_THRESHOLD ) { 
				fYInertia = ( yLastIncr - INERTIA_THRESHOLD ) * INERTIA_FACTOR;
			}

			if ( -yLastIncr > INERTIA_THRESHOLD ) { 
				fYInertia = ( yLastIncr + INERTIA_THRESHOLD ) * INERTIA_FACTOR;
			}

		} else {
			fXInertia = 0;
			fYInertia = 0;
		}

		xLastIncr = 0;
		yLastIncr = 0;
	}
}


static
void motion( GLFWwindow *window, double x, double y )
{
	if ( ( xLast != -1 ) || ( yLast != -1 ) ) { //zoom
		xLastIncr = x - xLast;
		yLastIncr = y - yLast;
		if ( bmModifiers & GLFW_MOD_CONTROL ) {
			if ( xLast != -1 ) {
				fZDiff += xLastIncr;
				fScale += yLastIncr * SCALE_FACTOR;
			}
		} else {
			if ( xLast != -1 ) { //rotate
				fXDiff += xLastIncr;
				fYDiff += yLastIncr;
			}
		}
	}

   xLast = x;
   yLast = y;
}


static
void reshape( GLFWwindow* window, int wid, int ht )
{	
	view_width = wid;
	view_height = ht;

	float vp = 0.8f;
    float aspect = (float)wid / (float)ht;

	// set viewport
	glViewport( 0, 0, wid, ht );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// set perspective projection
	glFrustum( -vp, vp, ( -vp / aspect ), ( vp / aspect ), 3, 10.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0, 0.0, -5.0 );
}


/*
 * load .dat dataset
 */
void LoadData( char* filename )
{
	fprintf(stderr, "loading data %s\n", filename);

	FILE* fp;
	fp = fopen(filename, "rb");
	// fopen_s(&fp, filename, "rb"); // open file, read only, binary mode 
	if (fp == NULL) {
		fprintf(stderr, "Cannot open file %s for reading.\n", filename);
		return;
	}
	
	memset( vol_dim, 0, sizeof( unsigned short ) * 3 );
	
	//read volume dimensions from file
	fread( &vol_dim[ 0 ], sizeof( unsigned short ), 1, fp );
	fread( &vol_dim[ 1 ], sizeof( unsigned short ), 1, fp );
	fread( &vol_dim[ 2 ], sizeof( unsigned short ), 1, fp );
	
	fprintf(stderr, "volume dimensions: x: %i, y: %i, z:%i \n", vol_dim[ 0 ], vol_dim[ 1 ], vol_dim[ 2 ] );
	
	if ( data_array != NULL ) {
		delete[] data_array;
	}

	// 1D array for storing volume data
	data_array = new unsigned short[ vol_dim[ 0 ] * vol_dim[ 1 ] * vol_dim[ 2 ] ]; 
	
	// read volume data
	fread( data_array, sizeof( unsigned short), ( vol_dim[ 0 ] * vol_dim[ 1 ] * vol_dim[ 2 ] ), fp );
	
	// shift volume data by 4 bit (converting 12 bit data to 16 bit data)
	for( int z = 0; z < vol_dim[ 2 ]; z++ ) {
		for( int y = 0; y < vol_dim[ 1 ]; y++ ) { 	
			for( int x = 0; x < vol_dim[ 0 ]; x++ ) { 
				int current_idx = x + ( y * vol_dim[ 0 ] ) + ( z * vol_dim[ 0 ] * vol_dim[ 1 ] );
					data_array[ current_idx ] = data_array[ current_idx ] << 4;
			} 	
		} 	
	} 

	// close file
	fclose( fp );

	// download data into texture
	DownloadVolumeAsTexture();

	data_loaded = true;
}


/*
 * download data to 3D texture
 */
void DownloadVolumeAsTexture()
{
	fprintf( stderr, " correct !! downloading volume to 3D texture\n" );

	glEnable( GL_TEXTURE_3D );
	printf(" correct 504 !!!" );
	// generate and bind 3D texture
	glGenTextures( 1, &vol_texture );
	printf(" correct 507 !!!" );
	glBindTexture( GL_TEXTURE_3D, vol_texture );
	
	printf(" correct 510 !!!" );

	// set necessary texture parameters
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP );

	printf(" correct correct 519!!! !!!" );
	//download texture in correct format (our data only has one channel of unsigned short data, therefore we use GL_INTENSITY16)
	glTexImage3D( GL_TEXTURE_3D, 0,  GL_INTENSITY16, vol_dim[ 0 ], vol_dim[ 1 ], vol_dim[ 2 ], 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, data_array ); 

	glDisable( GL_TEXTURE_3D );
}


/*
 * update tf based on defined window
 */
void UpdateTransferfunction( void ) 
{

	// =============================================================
	// TODO: update custom transferfunction tf0 here
	// =============================================================

	for (int i = 0; i < tf_win_min; i++)
	{
		tf0[i * 4 + 0] = 0.0f;
		tf0[i * 4 + 1] = 0.0f;
		tf0[i * 4 + 2] = 0.0f;
		tf0[i * 4 + 3] = 0.0f;
	}

	float len = tf_win_max - tf_win_min;
	for (int i = 0; i < len; i++)
	{
		tf0[(i+tf_win_min) * 4 + 0] = (float)(i) / len;
		tf0[(i+tf_win_min) * 4 + 1] = (float)(i) / len;
		tf0[(i+tf_win_min) * 4 + 2] = (float)(i) / len;
		tf0[(i+tf_win_min) * 4 + 3] = (float)(i) / len;
	}

	for (int i = tf_win_max; i < 128; i++)
	{
		tf0[i * 4 + 0] = 1.0f;
		tf0[i * 4 + 1] = 1.0f;
		tf0[i * 4 + 2] = 1.0f;
		tf0[i * 4 + 3] = 1.0f;
	}

	DownloadTransferFunctionTexture( 0 );
}


/*
 * download tf data to 1D texture
 */
void DownloadTransferFunctionTexture( int tf_id )
{
	fprintf( stderr, " error !! downloading transfer function to 1D texture\n" );

	glEnable( GL_TEXTURE_1D );

	// generate and bind 1D texture
	glGenTextures( 1, &tf_texture );
	glBindTexture( GL_TEXTURE_1D, tf_texture );
	
	// set necessary texture parameters
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP );

	//download texture in correct format
	switch( tf_id ){
		case 0:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 128, 0, GL_RGBA, GL_FLOAT, tf0 ); 
			break;
		case 1:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 2, 0, GL_RGBA, GL_FLOAT, tf1 ); 
			break;
		case 2:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 4, 0, GL_RGBA, GL_FLOAT, tf2 ); 
			break;
		case 3:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 12, 0, GL_RGBA, GL_FLOAT, tf3 ); 
			break;
		case 4:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 12, 0, GL_RGBA, GL_FLOAT, tf4 ); 
			break;
		case 5:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 12, 0, GL_RGBA, GL_FLOAT, tf5 ); 
			break;
		default:
			glTexImage1D( GL_TEXTURE_1D, 0,  GL_RGBA, 128, 0, GL_RGBA, GL_FLOAT, tf0 ); 
			break;
	}

	glDisable( GL_TEXTURE_1D );
}


void drawVertex(float* V, float* V_Color)
{
	glColor3f(V_Color[0], V_Color[1], V_Color[2]);
	glVertex3f(V[0], V[1], V[2]);
}


void drawBoundingBox(float dim_x, float dim_y, float dim_z)
{
	// Vertex
	float A[3] = {  dim_x,  dim_y,  dim_z };
	float B[3] = {  dim_x,  dim_y, -dim_z };
	float C[3] = {  dim_x, -dim_y, -dim_z };
	float D[3] = {  dim_x, -dim_y,  dim_z };
	float E[3] = { -dim_x,  dim_y,  dim_z };
	float F[3] = { -dim_x,  dim_y, -dim_z };
	float G[3] = { -dim_x, -dim_y, -dim_z };
	float H[3] = { -dim_x, -dim_y,  dim_z };

	// Color to index the volume texture
	float A_Color[3] = { 1, 1, 1 };
	float B_Color[3] = { 1, 1, 0 };
	float C_Color[3] = { 1, 0, 0 };
	float D_Color[3] = { 1, 0, 1 };
	float E_Color[3] = { 0, 1, 1 };
	float F_Color[3] = { 0, 1, 0 };
	float G_Color[3] = { 0, 0, 0 };
	float H_Color[3] = { 0, 0, 1 };

	glBegin(GL_QUADS);

		drawVertex(D, D_Color);
		drawVertex(C, C_Color);
		drawVertex(B, B_Color);
		drawVertex(A, A_Color);

		drawVertex(G, G_Color);
		drawVertex(H, H_Color);
		drawVertex(E, E_Color);
		drawVertex(F, F_Color);

		drawVertex(C, C_Color);
		drawVertex(G, G_Color);
		drawVertex(F, F_Color);
		drawVertex(B, B_Color);

		drawVertex(H, H_Color);
		drawVertex(D, D_Color);
		drawVertex(A, A_Color);
		drawVertex(E, E_Color);

		drawVertex(E, E_Color);
		drawVertex(A, A_Color);
		drawVertex(B, B_Color);
		drawVertex(F, F_Color);

		drawVertex(G, G_Color);
		drawVertex(C, C_Color);
		drawVertex(D, D_Color);
		drawVertex(H, H_Color);

	glEnd();

}

/*
 * render backfaces to start raycasting
 */
void RenderBackFaces(float dim_x, float dim_y, float dim_z)
{
	enableRenderToBuffer( backface_buffer );

	// =============================================================
	// TODO: render your backfaces here
	// result gets piped to backface_buffer automatically
	// =============================================================
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	drawBoundingBox(dim_x, dim_y, dim_z);

	glDisable(GL_CULL_FACE);


	disableRenderToBuffer(  );
}


/*
 * render frontfaces to start raycasting
 */
 void RenderFrontFaces(float dim_x, float dim_y, float dim_z)
{
	enableRenderToBuffer( frontface_buffer );

	// =============================================================
	// TODO: render your backfaces here
	// result gets piped to frontface_buffer automatically
	// =============================================================

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	drawBoundingBox(dim_x, dim_y, dim_z);
	glDisable(GL_CULL_FACE);

	disableRenderToBuffer(  );
}


void RenderRaycastPass( void )
{
	// switch for dvr and iso shader programs
	GLuint shader_program = use_dvr ? raycast_dvr_shader_program : raycast_iso_shader_program;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	// enable shader
	glUseProgram( shader_program );

	// example for a texture uniform
	int tf_location = glGetUniformLocation( shader_program, "transferfunction" );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_1D, tf_texture );
	glUniform1i( tf_location, 0 );

	// =============================================================
	// TODO: add additional uniforms as needed
	// i.e. front and backface buffers ( frontface_buffer, backface_buffer )

	// Pass volume texture to the shader
	int volume_texloc = glGetUniformLocation( shader_program, "volume_tex" );
	glActiveTexture( GL_TEXTURE1 );
	glBindTexture( GL_TEXTURE_3D, vol_texture );
	glUniform1i(volume_texloc, 1); 

	// back & front buffers
	int backface_bufferloc = glGetUniformLocation(shader_program, "backface_tex");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, backface_buffer);
	glUniform1i(backface_bufferloc, 2); 

	int frontface_bufferloc = glGetUniformLocation(shader_program, "frontface_tex");
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, frontface_buffer);
	glUniform1i(frontface_bufferloc, 3); 

	// pass step size
	int stepsizeloc = glGetUniformLocation(shader_program, "step_size");
	glUniform1f(stepsizeloc, step_size);

	int ambient_loc = glGetUniformLocation(shader_program, "ambient");
	glUniform1f(ambient_loc, ambient);

	// pass if MIP
	int MIP_loc = glGetUniformLocation(shader_program, "MIP_switch");
	glUniform1f(MIP_loc, MIP_switch);

	// pass vol dim
	int max_dim = maximum(vol_dim[0], vol_dim[1], vol_dim[2]);
	float dim_x = (float)vol_dim[0] / (float)max_dim;
	float dim_y = (float)vol_dim[1] / (float)max_dim;
	float dim_z = (float)vol_dim[2] / (float)max_dim;
	int vol_dim_loc = glGetUniformLocation(shader_program, "vol_dim");
	glUniform3f(vol_dim_loc, dim_x, dim_y, dim_z);

	int vol_max_dim_loc = glGetUniformLocation(shader_program, "vol_max_dim");
	glUniform1i(vol_max_dim_loc, max_dim);

	// pass iso_value
	int iso_value_loc = glGetUniformLocation(shader_program, "iso_value");
	glUniform1f(iso_value_loc, iso_value);



	// =============================================================

	// Render screen filling quad
	// use squared centered quad to not disturb ratio
	int x[2] = { 0, 0 };
	int y[2] = { 0, 0 };
	int dim, off;
	if( view_width < view_height ){

		dim = view_width;
		off = ( view_height - view_width ) / 2;
	
		x[ 0 ] = 0;
		x[ 1 ] = dim;
		y[ 0 ] = off;
		y[ 1 ] = dim + off;

	} else {

		dim = view_height;
		off = ( view_width - view_height ) / 2;
	
		x[ 0 ] = off;
		x[ 1 ] = dim + off;
		y[ 0 ] = 0;
		y[ 1 ] = dim;
	}

	glViewport( 0, 0, view_width, view_height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, view_width, 0, view_height, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	
	//draw_fullscreen_quad
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);

	// red screen means your shader is broken
	glColor3f( 1.0, 0.0, 0.0 );

	glTexCoord2i(0, 0); glVertex2i(x[ 0 ], y[ 0 ] );
	glTexCoord2i(1, 0); glVertex2i(x[ 1 ], y[ 0 ] );
	glTexCoord2i(1, 1); glVertex2i(x[ 1 ], y[ 1 ]);
	glTexCoord2i(0, 1); glVertex2i(x[ 0 ], y[ 1 ]);

	glEnd();
	glEnable(GL_DEPTH_TEST);

	//disable shader
	glUseProgram( 0 );
}


// for debugging purposes
void RenderBufferToScreen( GLuint buffer ){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	
	glEnable( GL_TEXTURE_2D );
	glBindTexture (GL_TEXTURE_2D, buffer );

	// use squared centered quad to not disturb ratio
	int x[2] = { 0, 0 };
	int y[2] = { 0, 0 };
	int dim, off;
	if( view_width < view_height ){

		dim = view_width;
		off = ( view_height - view_width ) / 2;
	
		x[ 0 ] = 0;
		x[ 1 ] = dim;
		y[ 0 ] = off;
		y[ 1 ] = dim + off;

	} else {

		dim = view_height;
		off = ( view_width - view_height );
	
		x[ 0 ] = off;
		x[ 1 ] = dim + off;
		y[ 0 ] = 0;
		y[ 1 ] = dim;
	}
	
	glViewport( 0, 0, view_width, view_height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, view_width, 0, view_height, -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	
	//draw_fullscreen_quad
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_QUADS);

	glTexCoord2i(0, 0); /*glColor3f( 0.0f, 0.0f, 1.0f );*/ glVertex2i( x[ 0 ], y[ 0 ] );
	glTexCoord2i(1, 0); /*glColor3f( 1.0f, 0.0f, 1.0f );*/ glVertex2i( x[ 1 ], y[ 0 ] );
	glTexCoord2i(1, 1); /*glColor3f( 1.0f, 1.0f, 1.0f );*/ glVertex2i( x[ 1 ], y[ 1 ] );
	glTexCoord2i(0, 1); /*glColor3f( 0.0f, 1.0f, 1.0f );*/ glVertex2i( x[ 0 ], y[ 1 ] );

	glEnd();
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_TEXTURE_2D);
}

/*
 * enable buffer to render to
 */
void enableRenderToBuffer( GLuint buffer )
{
	glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );

	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer, 0 );
}

/*
 * unbinds buffer
 */
void disableRenderToBuffer()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}


void initGL( void ){
	/*
	int gl_major, gl_minor;

	// query and display OpenGL version
	getGlVersion( &gl_major, &gl_minor );
	printf( "GL_VERSION major=%d minor=%d\n", gl_major, gl_minor );

	// Create the to FBO's one for the backsides and one for the frontsides
	glGenFramebuffers( 1, &framebuffer );
	glBindFramebuffer( GL_FRAMEBUFFER ,framebuffer );

	glGenTextures( 1, &backface_buffer);
	glBindTexture( GL_TEXTURE_2D, backface_buffer );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexImage2D( GL_TEXTURE_2D, 0,GL_RGBA16F, BUFFER_WIDTH, BUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, backface_buffer, 0 );


	glGenTextures( 1, &frontface_buffer );
	glBindTexture( GL_TEXTURE_2D, frontface_buffer );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexImage2D( GL_TEXTURE_2D, 0,GL_RGBA16F, BUFFER_WIDTH, BUFFER_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frontface_buffer, 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	// Create Shader Program
	LoadAndLinkShaders();

	// download initial transfer function
	DownloadTransferFunctionTexture( 0 );
	*/

	float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 


    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

char *textFileRead( char *fn ) {

	FILE *fp;
	char *content = NULL;

	int count=0;

	if ( fn != NULL ) {
		
		fp = fopen(fn, "rb");
		// fopen_s( &fp, fn,"rt" );
		
		if ( fp != NULL ) {
			fseek( fp, 0, SEEK_END );
			count = ftell( fp );
			rewind( fp );

			if ( count > 0 ) {
				content = (char *)malloc( sizeof( char ) * ( count + 1 ) );
				count = fread( content, sizeof(char), count, fp );
				content[ count ] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}


void LoadAndLinkShaders( void ){

	char *fs = NULL;

	raycast_dvr_fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
	raycast_iso_fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );

	fs = textFileRead( RAYCAST_DVR_GLSL );

	if ( fs == NULL  ){
		
		printf( "DVR shader file not found\n" );
		exit( 1 );
	}

	const char *f1 = fs;

	glShaderSource( raycast_dvr_fragment_shader, 1, &f1, NULL );

	free( fs );

	fs = textFileRead( RAYCAST_ISO_GLSL );

	if ( fs == NULL ) {
		
		printf( "ISO shader file not found\n" );
		exit( 1 );
	}

	const char *f2 = fs;

	glShaderSource( raycast_iso_fragment_shader, 1, &f2, NULL );

	free( fs );

	glCompileShader( raycast_dvr_fragment_shader );
	glCompileShader( raycast_iso_fragment_shader );

	raycast_dvr_shader_program = glCreateProgram();
	raycast_iso_shader_program = glCreateProgram();
	glAttachShader( raycast_dvr_shader_program, raycast_dvr_fragment_shader );
	glAttachShader( raycast_iso_shader_program, raycast_iso_fragment_shader );

	glLinkProgram( raycast_dvr_shader_program );
	glLinkProgram( raycast_iso_shader_program );
}


/******************************************************************************/
/*
 * Main
 *
 ******************************************************************************/

int main( int argc, char **argv )
{
	// init variables
	fXDiff = 206;		//rotation
	fYDiff = 16;
	fZDiff = 10;
	xLastIncr = 0;		//last difference in mouse movement
	yLastIncr = 0;
	fXInertia = -0.5;	//inertia for keeping rotating even after mouse movement
	fYInertia = 0;
	// fXInertiaOld;		//old inertia
	// fYInertiaOld;
	fScale = 1.0;		//scale/zoom factor
	xLast = -1;			//last mouse position
	yLast = -1;
	// bmModifiers;		//keyboard modifiers (e.g. ctrl,...)
	Rotate = 1;			//auto-rotate
	view_width = 0;
	view_height = 0;

	use_dvr = 1;

	iso_value = 0.5f;

	enable_lighting = 0;
	enable_gm_scaling = 0;
	ambient = 0.5;
	step_size = 0.0025f;

	tf_win_min = 0;
	tf_win_max = 127;

	for( int i = 0; i < 128; i++ ){
	
		tf0[ 4 * i + 0 ] = (float)(i) / 128.0f;
		tf0[ 4 * i + 1 ] = (float)(i) / 128.0f;
		tf0[ 4 * i + 2 ] = (float)(i) / 128.0f;
		tf0[ 4 * i + 3 ] = (float)(i) / 128.0f;
	}

	data_array = NULL;
	data_loaded = false;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow( 512, 512, "AMCS/CS247 Scientific Visualization", NULL, NULL );
	if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwMakeContextCurrent(window);
	/*
	glfwSetKeyCallback(window, key);
	glfwSetFramebufferSizeCallback(window, reshape);
	glfwSetCursorPosCallback(window, motion);
	glfwSetMouseButtonCallback(window, mouse);
	*/

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

	// init OpenGL
	initGL();
	
	// glDepthFunc( GL_LESS );
    // glEnable( GL_DEPTH_TEST );

	// NextClearColor();

	// display help
	// key( window, GLFW_KEY_SLASH, 0, GLFW_PRESS, 0 );

	// float previous = glfwGetTime();

	while (!glfwWindowShouldClose(window))
    {
		display();
        glfwPollEvents();

		/*
		float now = glfwGetTime();
		float delta = now - previous;
		if (delta <= TIMER_FREQUENCY_MILLIS * 0.001)
		{
			timer(0);
			previous = now;
		}
		*/
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}

