//
// Mainly Created and contributed by pushpak on 10/5/18.
//

/* Start Header -------------------------------------------------------
Sinil made a small changes of this code
Copyright (C) FALL2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectMesh.h
Purpose: Class of ObjectMesh. All object in the scene is drawn by this class
Language: C++
Platform: Windows SDK version: 10.0.19041.0, OS: Windows 10. GPU: NVIDIA GeForce840M. OpenGL Driver version: 10.18.15.4279, 8-24-2015
Project: sinil.kang_CS300_1
Author: Sinil Kang = sinil.kang = Colleague ID: 0052782
Creation date: 9/20/2021
End Header --------------------------------------------------------*/
#include <GL/glew.h>
#include <iostream>
#include <set>
#include "Mesh.h"


#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/transform.hpp>

// Initialize the data members in the mesh
void Mesh::initData()
{
    vertexBuffer.clear();
    vertexIndices.clear();
    vertexUVs.clear();
    vertexNormals.clear();
    vertexNormalDisplay.clear();
    faceNormalDisplay.clear();

    normalLength = 0.00f;

    return;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////
GLfloat *Mesh::getVertexBuffer()
{
    return reinterpret_cast<GLfloat *>(vertexBuffer.data());
}

GLfloat *Mesh::getVertexNormals()
{
    return reinterpret_cast<GLfloat *>(vertexNormals.data());
}

GLfloat *Mesh::getVertexUVs()
{
    return reinterpret_cast<GLfloat *>(vertexUVs.data());
}

GLfloat *Mesh::getVertexNormalsForDisplay()
{
    return reinterpret_cast<GLfloat *>(vertexNormalDisplay.data());
}

GLfloat* Mesh::getFaceNormalsForDisplay()
{
    return reinterpret_cast<GLfloat*>(faceNormalDisplay.data());;
}

GLuint *Mesh::getIndexBuffer()
{
    return vertexIndices.data();
}

////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
unsigned int Mesh::getVertexBufferSize()
{
    return (unsigned int) vertexBuffer.size();
}

unsigned int Mesh::getIndexBufferSize()
{
    return (unsigned int) vertexIndices.size();
}

unsigned int Mesh::getTriangleCount()
{
    return getIndexBufferSize() / 3;
}

unsigned int Mesh::getVertexCount()
{
    return getVertexBufferSize();
}

unsigned int Mesh::getVertexNormalCount()
{
    return static_cast<unsigned int>(vertexNormalDisplay.size());
}

unsigned int Mesh::getFaceNormalCount()
{
    return static_cast<unsigned int>(faceNormalDisplay.size());
}

glm::vec3  Mesh::getModelScale()
{
    glm::vec3 scale = boundingBox[1] - boundingBox[0];

    if (scale.x == 0.0)
        scale.x = 1.0;

    if (scale.y == 0.0)
        scale.y = 1.0;

    if (scale.z == 0.0)
        scale.z = 1.0;

    return scale;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

glm::vec3  Mesh::getModelCentroid()
{
    return glm::vec3( boundingBox[0] + boundingBox[1] ) * 0.5f;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

glm::vec3 Mesh::getCentroidVector(glm::vec3 vVertex)
{
    return glm::normalize(vVertex - getModelCentroid());
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

struct compareVec
{
    bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
    {
        float V = glm::dot( lhs, rhs );
        bool bRetCode = glm::epsilonNotEqual( V, 1.0f, FLT_EPSILON);

        return bRetCode;
    }
};

/////////////////////////////////////////////////////////
int Mesh::calcVertexNormals(GLboolean bFlipNormals)
{
    int rFlag = -1;

    // vertices and indices must be populated
    if( vertexBuffer.empty() || vertexIndices.empty() )
    {
        std::cout << "Cannot calculate vertex normals for empty mesh." << std::endl;
        return rFlag;
    }

    // Pre-built vertex normals
//    if( !vertexNormals.empty() )
//    {
//        std::cout << "Vertex normals specified by the file. Skipping this step." << std::endl;
//        return rFlag;
//    }

    // Initialize vertex normals
    GLuint  numVertices = getVertexCount();
    vertexNormals.resize( numVertices, glm::vec3(0.0f) );
    vertexNormalDisplay.resize( static_cast<size_t>(numVertices) * 2U, glm::vec3(0.0f) );

    std::vector< std::set< glm::vec3, compareVec > >  vNormalSet;
    vNormalSet.resize( numVertices );

    // For every face
    GLuint numIndex = static_cast<GLuint>(vertexIndices.size());
    GLuint numFaces = numIndex / 3;
    std::vector<glm::vec3> faceNormals;
    faceNormals.resize(numFaces, glm::vec3(0.0f));
    for (glm::uint index = 0; index < numIndex; )
    {
        GLuint a = vertexIndices.at(index++);
        GLuint b = vertexIndices.at(index++);
        GLuint c = vertexIndices.at(index++);

        glm::vec3  vA = vertexBuffer[a];
        glm::vec3  vB = vertexBuffer[b];
        glm::vec3  vC = vertexBuffer[c];

        // Edge vectors
        glm::vec3  E1 = vB - vA;
        glm::vec3  E2 = vC - vA;

        glm::vec3  N = glm::normalize( glm::cross( E1, E2 ) );

        if (isnan(N.x) || isnan(N.y) || isnan(N.z))
        {
            continue;
        }

        if( bFlipNormals )
            N = N * -1.0f;

        if ((index / 3) - 1 >= faceNormals.size())
        {
            std::cout << "(index / 3) - 1 >= faceNormals.size()" << std::endl;
        }
        faceNormals[(index/3) - 1] = (N);

        // For vertex a
        vNormalSet.at( a ).insert( N );
        vNormalSet.at( b ).insert( N );
        vNormalSet.at( c ).insert( N );
    }

    // Now sum up the values per vertex
    for( int index =0; index < vNormalSet.size(); ++index )
    {
        glm::vec3  vNormal(0.0f);

//        if( vNormalSet[index].size() <= 0 )
//            std::cout << "[Normal Set](" << index << ") Size = " << vNormalSet[index].size() << std::endl;

        auto nIt = vNormalSet[index].begin();
        while(nIt != vNormalSet[index].end())
        {
            vNormal += (*nIt);
            ++nIt;
        }

        // save vertex normal
        vertexNormals[index] = glm::normalize( vNormal );

        // save normal to display
        glm::vec3  vA = vertexBuffer[index];

        if ((2U * static_cast<size_t>(index)) >= vertexNormalDisplay.size())
        {
            std::cout << "2 * index >= vertexNormalDisplay.size()" << std::endl;
        }
        if ((2U * static_cast<size_t>(index))+1 >= vertexNormalDisplay.size())
        {
            std::cout << "2 * index+1 >= vertexNormalDisplay.size()" << std::endl;
        }

        vertexNormalDisplay[2U* static_cast<size_t>(index)] = vA;
        vertexNormalDisplay[(2U* static_cast<size_t>(index)) + 1] = vA + ( normalLength * vertexNormals[index] );

    }


    faceNormalDisplay.resize(static_cast<size_t>(numFaces) * 2U, glm::vec3(0.0f));
    for (unsigned int i = 0; i < numFaces; i++)
    {
        glm::vec3 v1 = vertexBuffer[vertexIndices[(static_cast<size_t>(i) * 3U)]];
        glm::vec3 v2 = vertexBuffer[vertexIndices[(static_cast<size_t>(i) * 3U) + 1]];
        glm::vec3 v3 = vertexBuffer[vertexIndices[(static_cast<size_t>(i) * 3U) + 2]];
        
        glm::vec3 position = (v1 + v2 + v3) / 3.f;

        glm::vec3 fN = faceNormals[i];
        faceNormalDisplay[2U * static_cast<size_t>(i)] = position;
        faceNormalDisplay[(2U * static_cast<size_t>(i)) + 1] = position + (normalLength * fN);
    }

    // success
    rFlag = 0;

    return rFlag;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

void Mesh::calcVertexNormalsForDisplay(GLboolean /*bFlipNormals*/)
{
    GLuint numVertices = getVertexCount();
    vertexNormalDisplay.resize(static_cast<size_t>(numVertices) * 2U, glm::vec3(0.0f));

    for (int iNormal = 0; iNormal < vertexNormals.size(); ++iNormal)
    {
        glm::vec3 normal = vertexNormals[iNormal] * normalLength;

        vertexNormalDisplay[2U * static_cast<size_t>(iNormal)] = vertexBuffer[iNormal];
        vertexNormalDisplay[(2U * static_cast<size_t>(iNormal)) + 1] = vertexBuffer[iNormal] + normal;
    }
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

GLfloat  & Mesh::getNormalLength()
{
    return normalLength;
}

void Mesh::setNormalLength(GLfloat nLength)
{
    normalLength = nLength;
}
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

int Mesh::calcUVs( Mesh::UVType uvType )
{
    int rFlag = -1;

    // clear any existing UV
    vertexUVs.clear();

    glm::vec3 delta = getModelScale();

    for( int nVertex = 0; nVertex < vertexBuffer.size(); ++nVertex )
    {
        glm::vec3 V = vertexBuffer[nVertex];
        glm::vec2 uv(0.0f);

        glm::vec3 normVertex = glm::vec3( (V.x - boundingBox[0].x ) / delta.x,
                                          (V.y - boundingBox[0].y ) / delta.y,
                                          (V.z - boundingBox[0].z ) / delta.z );

//        normVertex = (2.0f * normVertex) - glm::vec3(1.0f);

        glm::vec3 centroidVec = getCentroidVector(V);

        float theta(0.0f);
        float z(0.0f);
        float phi(0.0f);

        switch( uvType )
        {
            case Mesh::UVType::CYLINDRICAL_UV:
                theta = glm::degrees( static_cast<float>( atan2(centroidVec.y, centroidVec.x ) ) );
                theta += 180.0f;

                z = (centroidVec.z + 1.0f) * 0.5f;

                uv.x = theta / 360.0f;
                uv.y = z;
                break;

            case Mesh::UVType::SPHERICAL_UV:
                theta = glm::degrees( static_cast<float>(glm::atan(centroidVec.y, centroidVec.x )) );
                theta += 180.0f;

                z = centroidVec.z;
                phi = glm::degrees(glm::acos(z / centroidVec.length() ));

                uv.x = theta / 360.0f;
                uv.y = 1.0f - ( phi / 180.0f );
                break;

            case Mesh::UVType::CUBE_MAPPED_UV:
                uv = calcCubeMap(centroidVec);
                break;
        }

        vertexUVs.push_back( uv );
    }

    return rFlag;
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

glm::vec2 Mesh::calcCubeMap(glm::vec3 vEntity)
{
    float x = vEntity.x;
    float y = vEntity.y;
    float z = vEntity.z;

    float absX = abs(x);
    float absY = abs(y);
    float absZ = abs(z);

    int isXPositive = x > 0 ? 1 : 0;
    int isYPositive = y > 0 ? 1 : 0;
    int isZPositive = z > 0 ? 1 : 0;

    float maxAxis, uc, vc;
    glm::vec2 uv = glm::vec2(0.0);
    uc = 0.f;
    vc = 0.f;

    // POSITIVE X
    if (bool(isXPositive) && (absX >= absY) && (absX >= absZ))
    {
        // u (0 to 1) goes from +z to -z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = -z;
        vc = y;
    }

        // NEGATIVE X
    else if (!bool(isXPositive) && absX >= absY && absX >= absZ)
    {
        // u (0 to 1) goes from -z to +z
        // v (0 to 1) goes from -y to +y
        maxAxis = absX;
        uc = z;
        vc = y;
    }

        // POSITIVE Y
    else if (bool(isYPositive) && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from +z to -z
        maxAxis = absY;
        uc = x;
        vc = -z;
    }

        // NEGATIVE Y
    else if (!bool(isYPositive) && absY >= absX && absY >= absZ)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -z to +z
        maxAxis = absY;
        uc = x;
        vc = z;
    }

        // POSITIVE Z
    else if (bool(isZPositive) && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from -x to +x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = x;
        vc = y;
    }

        // NEGATIVE Z
    else if (!bool(isZPositive) && absZ >= absX && absZ >= absY)
    {
        // u (0 to 1) goes from +x to -x
        // v (0 to 1) goes from -y to +y
        maxAxis = absZ;
        uc = -x;
        vc = y;
    }

    // Convert range from -1 to 1 to 0 to 1
    uv.s = 0.5f * (uc / maxAxis + 1.0f);
    uv.t = 0.5f * (vc / maxAxis + 1.0f);

    return uv;
}

glm::mat4 Mesh::calcAdjustBoundingBoxMatrix()
{
    return glm::scale(2.f / getModelScale()) * glm::translate(-getModelCentroid());
}