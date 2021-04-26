#pragma once

#include <TopoDS_Shape.hxx>
#include <string> 

struct ModelProperty {
    enum type {
        BOX = 1;
    };
    float width;
    float height;
    float length;
};

class OCCProcessor {
private:
    TopoDS_Shape m_aShape;
    ModelProperty m_aProperty;

    static const Standard_Integer THE_STL_SIZEOF_FACET = 50;
    static const Standard_Integer IND_THRESHOLD = 1000; // increment the indicator every 1k triangles
    static const size_t THE_BUFFER_SIZE = 1024; // The length of buffer to read (in bytes)
public:
    int initializeModel();
    int updateShape(ModelProperty newShapeProperty);
    int stretchWidth(float newWidth);
    int readSampleModel();
    int printModelStatus();
    int toMeshString(std::string& mesh_str);
    static int toUnityMesh(const TopoDS_Shape& theShape);
};
