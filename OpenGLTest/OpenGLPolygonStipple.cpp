#include "stdafx.h"

#include "OpenGLPolygonStipple.h"
#include "Math_Unit2.h"
#include <bitset>

namespace OGLKit
{
	namespace PolygonStipple
	{
		typedef std::bitset<32>	 RowBits;
		typedef std::array<DWORD, 32>	PolygonStippleBitsMask;

		struct SlashBitsRotateOperation
		{
			void operator()(RowBits &rowBits, size_t stepLenInBits)
			{
				rowBits = rowBits << stepLenInBits | rowBits >> (rowBits.size() - stepLenInBits);
			}

			void operator()(std::string &str, size_t stepLenInBits)
			{
				std::rotate(std::begin(str), std::end(str) - stepLenInBits, std::end(str));
			}
		};

		struct BackSlashBitsRotateOperation
		{
			void operator()(RowBits &rowBits, size_t stepLenInBits)
			{
				rowBits = rowBits >> stepLenInBits | rowBits << (rowBits.size() - stepLenInBits);		
			}

			void operator()(std::string &str, size_t stepLenInBits)
			{
				std::rotate(std::begin(str), std::begin(str) + stepLenInBits, std::end(str));
			}
		
		};

		template<class RowBitsRotateOperation>
		struct SlashStippleOperationBase
		{
			void operator()(const std::string &strContent, PolygonStippleBitsMask &vMask)
			{
				RowBits bits(strContent);
				RowBitsRotateOperation	rowBitsRotateOp;
				for ( auto iter = std::begin(vMask); iter != std::end(vMask); )
				{
					*iter++ |= bits.to_ulong();
					rowBitsRotateOp(bits, 1);
				}
			}

			void operator()(const std::string &strContent, Color3DArray &clrs)
			{
				std::string strNew(strContent);
				RowBitsRotateOperation rotateOp;

				for ( auto iterClr = std::begin(clrs); iterClr != std::end(clrs); )
				{
					for ( auto iterChar = std::begin(strNew); iterChar != std::end(strNew); ++iterChar )
					{
						if ('1' == *iterChar )
							iterClr->a = 255;

						++iterClr;
					}

					rotateOp(strNew, 1);
				}
			}
		};

		struct SlashStippleOperation 
			: public SlashStippleOperationBase<SlashBitsRotateOperation>
		{};
		struct BackSlashStippleOperation 
			: public SlashStippleOperationBase<BackSlashBitsRotateOperation>
		{};

		struct VerticalStippleOperation
		{
			void operator()(const std::string &bitsContent, PolygonStippleBitsMask &vMask)
			{
				for ( auto iterMask = std::begin(vMask); iterMask != std::end(vMask); ++iterMask )
				{
					*iterMask |= RowBits(bitsContent).to_ulong();
				}
			}

			void operator()(const std::string &strContent, Color3DArray &clrs)
			{
				for ( auto iterClr = std::begin(clrs); iterClr != std::end(clrs); )
				{
					for ( auto iterCh = std::begin(strContent); iterCh != std::end(strContent); ++iterCh )
					{
						if ( '1' == *iterCh )
							iterClr->a = 255;

						++iterClr;
					}
				}
			}
		};

		struct HorizontalStippleOperation
		{
			void operator()(const std::string &bitsContent, PolygonStippleBitsMask &vMask)
			{
				auto iterMask = std::begin(vMask);
				for ( auto iter = std::begin(bitsContent); iter != std::end(bitsContent); ++iter )
				{
					*iterMask++ |= '1' == *iter ? 0xFFFFFFFF : 0;
				}
			}

			void operator()(const std::string &strContent, Color3DArray &clrs)
			{
				auto iterClr = std::begin(clrs);
				for ( auto iterCh = std::begin(strContent);iterCh != std::end(strContent); ++iterCh )
				{
					if ( '1' == *iterCh )
					{
						std::for_each(iterClr, iterClr + strContent.length(), [](Color3D &clr){clr.a = 255; });
					}
				}
			}
		};

		template<class FirstStippleOperation, class SecondStippleOperation>
		struct BinaryStippleOperation
		{
			template<class StippleMask>
			void operator()(const std::string &strContent, StippleMask &vMask)
			{
				FirstStippleOperation firstOp;
				SecondStippleOperation secondOp;
				firstOp(strContent, vMask);
				secondOp(strContent, vMask);
			}
		};

		struct DoubleSlashStippleOperation
			: public BinaryStippleOperation<SlashStippleOperation, BackSlashStippleOperation>
		{};

		struct HVStippleOperation
			: public BinaryStippleOperation<VerticalStippleOperation, HorizontalStippleOperation>
		{};


		template<class GenerateStippleOperation>
		void draw_one_rect_with_stipple(const Color3D &clr, const Rect &rect, const std::string &strContent, GenerateStippleOperation stippleOp)
		{
			PolygonStippleBitsMask	vMask;
			memset(&(*std::begin(vMask)), 0, vMask.size() * sizeof(PolygonStippleBitsMask::value_type));

			std::string strNewContent(strContent);
			strNewContent.erase(std::remove(begin(strNewContent), end(strNewContent), ' '), end(strNewContent));

			stippleOp(strNewContent, vMask);

			glColor3f(1.0f, 1.0f, 1.0f);
			glRectf(rect.left, rect.bottom, rect.right, rect.top);

			auto pBuf = &(*std::begin(vMask));
			glEnable (GL_POLYGON_STIPPLE);

			glPixelStorei(GL_UNPACK_LSB_FIRST, GL_TRUE);
			glPolygonStipple((GLubyte*)pBuf);
			glPixelStorei(GL_UNPACK_LSB_FIRST, GL_FALSE);

			glColor3ub(clr.r, clr.g, clr.b);
			glRectf(rect.left, rect.bottom, rect.right, rect.top);

			glDisable (GL_POLYGON_STIPPLE);
		}

		void draw_rects_stipple(float rectWidth, float rectHeight)
		{
			using namespace PolygonStipple;
			// slash
			draw_one_rect_with_stipple(Color3D(255, 0, 0, 0), Rect(-2*rectWidth, 0, -rectWidth, rectHeight),
				std::string("00000000 00000000 00000000 11111111"), DoubleSlashStippleOperation());

			draw_one_rect_with_stipple(Color3D(255, 128, 0, 0), Rect(-rectWidth, 0, 0, rectHeight),
				std::string("00000000 11111111 00000000 111111111"), DoubleSlashStippleOperation());

			draw_one_rect_with_stipple(Color3D(0, 255, 0, 0), Rect(0, 0, rectWidth, rectHeight),
				std::string("00001111 00001111 00001111 000011111"), DoubleSlashStippleOperation());

			draw_one_rect_with_stipple(Color3D(255, 0, 255, 0), Rect(rectWidth, 0, 2 * rectWidth, rectHeight),
				std::string("0111 0111 0111 0111 0111 0111 0111 0111"), DoubleSlashStippleOperation());

			// strike line
			draw_one_rect_with_stipple(Color3D(255, 0, 0, 0), Rect(-2*rectWidth, -rectHeight, -rectWidth, 0),
				std::string("00000000 00000000 00000000 00000001"), HVStippleOperation());

			draw_one_rect_with_stipple(Color3D(255, 128, 0, 0), Rect(-rectWidth, -rectHeight, 0, 0),
				std::string("00000000 00000001 00000000 00000001"), HVStippleOperation());

			draw_one_rect_with_stipple(Color3D(0, 255, 0, 0), Rect(0, -rectHeight, rectWidth, 0),
				std::string("00000011 00000011 00000011 00000011"), HVStippleOperation());

			draw_one_rect_with_stipple(Color3D(255, 0, 255, 0), Rect(rectWidth, -rectHeight, 2 * rectWidth, 0),
				std::string("0001 0001 0001 0001 0001 0001 0001 0001"), HVStippleOperation());
		}

	}


	GLuint texID;

	size_t stipple_len;
	template<class PolygonStippleOperation>
	void create_image(Color3DArray &clrs, const std::string &strBits, PolygonStippleOperation psOp)
	{
		std::all_of(std::begin(strBits), std::end(strBits), [](char ch){ return ch == '0' || ch == '1'; });
	
		const size_t imageLen = strBits.length();
		clrs.resize(imageLen * imageLen);
		std::fill(std::begin(clrs), std::end(clrs), Color3D(255, 0, 0, 0));

		psOp(strBits, clrs);

		return ;
	}

	class CO3DPolygonStipple
	{
	public:
		CO3DPolygonStipple();
		CO3DPolygonStipple(std::string &strContent);

		bool PreparePolygonStipple(std::string &strContent);
		bool RenderFillPtn();

	private:
		//Color3D		m_clrs;
		std::string m_strContent;

		GLuint	m_textID;
	};

	bool CO3DPolygonStipple::PreparePolygonStipple(std::string &strContent)
	{
		return true;
	}

	bool RenderFillPtn()
	{

	}

	//virtual  
	bool PolygonStippleDrawObj::PrepareObj()
	{
		Color3DArray clrs;
		std::string strBits("00000000 00000000 00000000 00000001");

		CO3DPolygonStipple fillPtn;

		fillPtn.PreparePolygonStipple(strBits);

		strBits.erase(std::remove(std::begin(strBits), std::end(strBits), ' '), std::end(strBits));

		create_image(clrs, strBits, PolygonStipple::HVStippleOperation());
		
		stipple_len = strBits.length();

		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D, texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)stipple_len, (GLsizei)stipple_len, 0, GL_RGBA, GL_UNSIGNED_BYTE, clrs[0]);

		glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_TEXTURE_2D);

		return true;
	}

	//virtual 
	bool PolygonStippleDrawObj::Draw()
	{
		//PolygonStipple::draw_rects_stipple(2.0f, 2.0f);
		//return true;

		glRotatef(60, 0.0, 1.0, 0.0);
		Point3D vertexes[4] = 
		{
			Point3D(0, 0, 0),
			Point3D(4, 0, 0),
			Point3D(4, 4, 0),
			Point3D(0, 4, 0),
		};

		const float width = vertexes[1].x - vertexes[0].x;
		const float height = vertexes[3].y - vertexes[0].y;

		const float repeatUnitTimes = 128 / (float)stipple_len;
		const float xMax = width/height * repeatUnitTimes;
		const float yMax = repeatUnitTimes;

		glColor3f(255, 255, 255);
		glBegin(GL_QUADS);
			glVertex3fv(vertexes[0]);
			glVertex3fv(vertexes[1]);
			glVertex3fv(vertexes[2]);
			glVertex3fv(vertexes[3]);
		glEnd();

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.f);
		glBindTexture(GL_TEXTURE_2D, texID);

		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);			glVertex3fv(vertexes[0]);
			glTexCoord2f(xMax, 0);		glVertex3fv(vertexes[1]);
			glTexCoord2f(xMax, yMax);	glVertex3fv(vertexes[2]);
			glTexCoord2f(0, yMax);		glVertex3fv(vertexes[3]);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_ALPHA_TEST);
		return true;
	}
}