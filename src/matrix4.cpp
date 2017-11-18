#include <stdio.h>
#include <string.h>
#include <math.h>
#include "matrix4.h"
#include "vector4.h"

Matrix4::Matrix4() {
    float m[4][4] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    setMatrix(m);
}

Matrix4::Matrix4(float matrix[][4]) {
    memcpy(m_matrix, matrix, sizeof(m_matrix));
}

void Matrix4::setMatrix(float matrix[][4]) {
    memcpy(m_matrix, matrix, sizeof(m_matrix));
}

float Matrix4::get(int i, int j) {
    return m_matrix[i][j];
}

Matrix4 Matrix4::initScreenSpaceTransform(float width, float height) {
        float half_width = width / 2.0f;
        float half_height = height / 2.0f;

        float m[4][4] = {
            {half_width,    0,              0,  half_width},
            {0,             -half_height,   0,  half_height},
            {0,             0,              1,  0          },
            {0,             0,              0,  1          }
        };

        return Matrix4(m);
}

Matrix4 Matrix4::initPerspective(float fov, float aspectRatio, float zNear, float zFar) {
    float tanHalfFOV = (float)tan(fov / 2);
    float zRange = zNear - zFar;
    float m[4][4] = {
        {1.0f / (tanHalfFOV * aspectRatio), 0,  0, 0},
        {0, 1.0f / tanHalfFOV, 0, 0},
        {0, 0, (-zNear -zFar)/zRange,  2 * zFar * zNear / zRange},
        {0, 0, 1, 0}
    };

    return Matrix4(m);
}

Matrix4 Matrix4::initTranslation(float x, float y, float z) {
    float m[4][4] = {
        {1, 0, 0, x},
        {0, 1, 0, y},
        {0, 0, 1, z},
        {0, 0, 0, 1}
    };

    return Matrix4(m);
}

Matrix4 Matrix4::initRotation(float x, float y, float z) {
    float mz[4][4] = {
        {(float)cos(z), -(float)sin(z), 0, 0},
        {(float)sin(z), (float)cos(z), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    float my[4][4] = {
        {(float)cos(y), 0, -(float)sin(y), 0},
        {0, 1, 0, 0},
        {(float)sin(y), 0, (float)cos(y), 0},
        {0, 0, 0, 1}
    };

    float mx[4][4] = {
        {1, 0, 0, 0},
        {0, (float)cos(x), -(float)sin(x), 0},
        {0, (float)sin(x), (float)cos(x), 0},
        {0, 0, 0, 1}
    };

    return Matrix4(mx).mul(Matrix4(my)).mul(Matrix4(mz));



}

Matrix4 Matrix4::initScale(float x, float y, float z) {
    float m[4][4] = {
        {x, 0, 0, 0},
        {0, y, 0, 0},
        {0, 0, z, 0},
        {0, 0, 0, 1}
    };

    return Matrix4(m);
}

Vector4 Matrix4::transform(Vector4 r) {
    return Vector4(
        m_matrix[0][0] * r.x + m_matrix[0][1] * r.y + m_matrix[0][2] * r.z + m_matrix[0][3] * r.w,
        m_matrix[1][0] * r.x + m_matrix[1][1] * r.y + m_matrix[1][2] * r.z + m_matrix[1][3] * r.w,
        m_matrix[2][0] * r.x + m_matrix[2][1] * r.y + m_matrix[2][2] * r.z + m_matrix[2][3] * r.w,
    	m_matrix[3][0] * r.x + m_matrix[3][1] * r.y + m_matrix[3][2] * r.z + m_matrix[3][3] * r.w
    );
}

Matrix4 Matrix4::mul(Matrix4 r) {
    float m[4][4] = {};

		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				m[i][j] =
                    m_matrix[i][0] * r.get(0, j) +
					m_matrix[i][1] * r.get(1, j) +
					m_matrix[i][2] * r.get(2, j) +
					m_matrix[i][3] * r.get(3, j);
			}
		}

		return Matrix4(m);
}
