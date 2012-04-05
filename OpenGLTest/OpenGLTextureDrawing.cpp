#include "stdafx.h"

#include "OpenGLRoot.h"
#include "OpenGLCamera.h"
#include "OpenGLRenderWin.h"
#include "OpenGLTextureDrawing.h"

namespace OGLKit
{
	TextureDrawingObj::TextureDrawingObj()
		: m_TextureID(0)
	{

	}

	//static 
	string TextureDrawingObj::GetObjName()
	{
		return STR_TEX_DRAWING_OBJ_NAME;
	}


	//virtual 
	string	TextureDrawingObj::GetName() const
	{
		return GetObjName();
	}

	static inline size_t IMAGE_WIDTH(){ return 64; }
	static inline size_t IMAGE_HEIGHT(){return 64; }

	typedef std::array<GLubyte, 4> ImageUint;

	typedef std::array<ImageUint, 8> ImageUnitArray8;
	typedef std::array<ImageUnitArray8, 8> Image2D8;

	typedef std::array<ImageUint, 16> ImageUintArray16;
	typedef std::array<ImageUintArray16, 16> Image2D16;

	typedef std::array<ImageUint, 32> ImageUintArray32;
	typedef std::array<ImageUintArray32, 32> Image2D32;

	typedef std::array<ImageUint, 128> ImageUintArray128;
	typedef std::array<ImageUintArray128, 128> Image2D128;

	static Image2D8		g_Image8;
	static Image2D16	g_Image16;
	static Image2D32	g_Image32;

	static Image2D128	g_Image128;


	void printData(const Image2D16 &image)
	{
		const size_t height = Image2D16::_EEN_SIZE;
		const size_t width = Image2D16::value_type::_EEN_SIZE;

		for ( size_t ii = 0; ii < height; ++ii )
		{
			for ( size_t jj = 0; jj < width; ++jj )
			{
				std::for_each(image[ii][jj].begin(), image[ii][jj].end(), 
					[](ImageUint::value_type data)
				{
					cout << int(data) << " ";
				}
				);
				cout << "\n";
			}

			cout << "\n";
		}

		cout << endl;
	}

	typedef std::pair<size_t, size_t>	Size;

	template<class Container, class BinaryOperator>
	static Size _make_data(Container &con, BinaryOperator BinOper)
	{
		const size_t height = Container::_EEN_SIZE;
		const size_t width = Container::value_type::_EEN_SIZE;

		for ( size_t ii = 0; ii < height; ++ii )
		{
			for ( size_t jj = 0; jj < width; ++jj )
			{
				//GLubyte c = (((ii&0x8) == 0)^((jj&0x8) == 0)) * 255;

				typename Container::value_type::value_type::value_type c = (unsigned char)BinOper(ii, jj);

				std::fill(std::begin(con[ii][jj]), std::end(con[ii][jj]), c);
				//g_Image16[ii][jj][0] = c;
				//g_Image16[ii][jj][1] = c;
				//g_Image16[ii][jj][2] = c;
				//g_Image16[ii][jj][3] = c;
			}
		}

		return Size(width, height);
	}

	const size_t image_width = 128;
	const size_t image_height = 128;
	static GLubyte images[image_width][image_height][4];
	static GLubyte other_images[image_width][image_height][4];

	static GLuint texName[2];

	void makeImages()
	{
		for ( size_t i = 0; i < image_height; ++i )
		{
			for ( size_t j = 0; j < image_width; ++j )
			{
				const GLubyte c = (((i&0x8) == 0) ^ ((j&0x8) == 0)) * 255;
				images[i][j][0] = c;
				images[i][j][1] = c;
				images[i][j][2] = c;
				images[i][j][3] = 255;

				const GLubyte oc = (((i&0x10) == 0) ^ ((j&0x10) == 0)) *255;
				other_images[i][j][0] = oc;
				other_images[i][j][1] = 0;
				other_images[i][j][2] = 0;
				other_images[i][j][3] = 255;
			}
		}
	}

	//virtual 
	bool TextureDrawingObj::PrepareObj()
	{
		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);

		makeImages();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(2, texName);

		glBindTexture(GL_TEXTURE_2D, texName[0]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, images);

		glBindTexture(GL_TEXTURE_2D, texName[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, other_images);

		glEnable(GL_TEXTURE_2D);
		return true;
	}

	static inline float GET_CAMERA_ADVANCE(){return 1.2f;}

	//virtual 
	bool	TextureDrawingObj::OnKeyUp(const KeyInfo &info)
	{
		return true;
	}

	//virtual 
	bool	TextureDrawingObj::OnKeyDown(const KeyInfo &info)
	{
		if ( info.keyDown == 'A' || info.keyDown == 'a' )
		{
			m_pCamera->Position().x -= GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'D' || info.keyDown == 'd' )
		{
			m_pCamera->Position().x += GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'W' || info.keyDown == 'w' )
		{
			m_pCamera->Position().z += GET_CAMERA_ADVANCE();
		}

		if ( info.keyDown == 'S' || info.keyDown == 's' )
		{
			m_pCamera->Position().z -= GET_CAMERA_ADVANCE();
		}

		return true;
	}

	//virtual 
	bool TextureDrawingObj::Draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindTexture(GL_TEXTURE_2D, texName[0]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex3f(-2.0, -1.0, 0.0);
			glTexCoord2f(1, 0);	glVertex3f(-2.0, 1.0, 0.0);
			glTexCoord2f(1, 1);	glVertex3f(0.0, 1.0, 0.0);
			glTexCoord2f(0, 1);	glVertex3f(0.0, -1.0, 0.0);
		glEnd();


		glBindTexture(GL_TEXTURE_2D, texName[1]);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);	glVertex3f(1.0, -1.0, 0.0);
			glTexCoord2f(1, 0);	glVertex3f(1.0, 1.0, 0.0);
			glTexCoord2f(1, 1);	glVertex3f(2.41421f, 1.0, -1.41421f);
			glTexCoord2f(0, 1);	glVertex3f(2.41421f, -1.0, -1.41421f);
		glEnd();		

		return true;
	}
}