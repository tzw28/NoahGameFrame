#pragma once

#include <TopoDS_Shape.hxx>
#include <string> 

#include "OCCLogger.h"
using namespace LOGGER;

struct ModelProperty {
    enum type {
        BOX = 1
    };
    float width;
    float height;
    float length;
};

class OCCProcessor {
private:
    TopoDS_Shape m_aShape;
    ModelProperty m_aProperty;
    int m_aCurrentIndex = 0;
    // OCCLogger* m_aLogger;

    static const Standard_Integer THE_STL_SIZEOF_FACET = 50;
    static const Standard_Integer IND_THRESHOLD = 1000; // increment the indicator every 1k triangles
    static const size_t THE_BUFFER_SIZE = 1024; // The length of buffer to read (in bytes)
public:
    OCCProcessor();
    virtual ~OCCProcessor();
    int initializeModel();
    int updateShape(ModelProperty newShapeProperty);
    int stretchWidth(float newWidth);
    int readSampleModel();
    int loadModel(Standard_CString sModelName);
    int loadStepModel(Standard_CString sModelName);
    int loadStlModel(Standard_CString sModelName);
    int printModelStatus();
    int toMeshString(std::string& mesh_str);
    static int toUnityMesh(const TopoDS_Shape& theShape);
};
