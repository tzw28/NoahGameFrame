#include "OCCProcessor.h"

#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <AIS_Shape.hxx>

#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopExp_Explorer.hxx>
#include <Poly_Triangulation.hxx>
#include <StlAPI_Reader.hxx>
#include <StlAPI_Writer.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <Message_ProgressScope.hxx>
#include <NCollection_Vector.hxx>
#include <Message_ProgressScope.hxx>
#include <Message_ProgressIndicator.hxx>
#include <STEPControl_Reader.hxx>

#include <fstream> 
#include <iostream> 
#include <string> 
#include <filesystem>
#include <sstream>

#include "OCCUtil.h"


OCCProcessor::OCCProcessor()
{
    // m_aLogger = new OCCLogger(LogLevel_Info, OCCLogger::GetAppPathA().append("..\\occlog\\"));
}

int OCCProcessor::initializeModel()
{
    // m_aProperty.type = BOX;
    m_aProperty.width = 1.0;
    m_aProperty.height = 1.0;
    m_aProperty.length = 1.0;

    m_aShape = BRepPrimAPI_MakeBox(m_aProperty.width, m_aProperty.height, m_aProperty.length).Shape();
    /*
    BRepBuilderAPI_MakePolygon MP;
    MP.Add(gp_Pnt(0, 0, 0));
    MP.Add(gp_Pnt(100, 0, 0));
    MP.Add(gp_Pnt(100, 100, 0));
    MP.Add(gp_Pnt(0, 100, 0));
    MP.Close();//完成构造 封闭空间
    TopoDS_Face F = BRepBuilderAPI_MakeFace(MP.Wire());
    gp_Vec aPrimVec(0, 0, 300);//拉伸 形成拉伸体
    TopoDS_Shape shape = BRepPrimAPI_MakePrism(F, aPrimVec);
    Handle(AIS_Shape) anAISShape = new AIS_Shape(shape);
    */
    std::cout << "end of create cube\n" << std::endl;
    return 0;
}

OCCProcessor::~OCCProcessor()
{
    // delete m_aLogger;
}

int OCCProcessor::updateShape(ModelProperty newShapeProperty)
{
    std::cout << "start to update shape" << std::endl;
    m_aProperty = newShapeProperty;
    m_aShape = BRepPrimAPI_MakeBox(m_aProperty.width, m_aProperty.height, m_aProperty.length).Shape();
    return 0;
}

int OCCProcessor::readSampleModel()
{
    std::cout << "start to read stl\n" << std::endl;

    std::cout << "Current path is " << std::filesystem::current_path() << '\n';
    StlAPI_Reader aReader_Stl;
    aReader_Stl.Read(m_aShape, "../Models/cube.stl");
    BRepMesh_IncrementalMesh BMesh(m_aShape, 0.1, Standard_True);
    StlAPI_Writer aWriter_Stl;
    aWriter_Stl.Write(m_aShape, "../Models/cube1.stl");
    std::cout << "end of stl reading\n" << std::endl;
    return 0;
}

int OCCProcessor::loadModel(Standard_CString sModelName)
{
    clock_t start, end;
    start = clock();
    std::cout << "load model " << sModelName << endswith(sModelName, ".stl") << std::endl;

    if (endswith(sModelName, ".step") || endswith(sModelName, ".stp"))
        loadStepModel(sModelName);
    else if (endswith(sModelName, ".stl"))
        loadStlModel(sModelName);

    end = clock();
    std::stringstream sstr;
    sstr << "Load model" << sModelName << " "
        << std::to_string((end - start) * 1000 / CLOCKS_PER_SEC).c_str() << "ms";
    // m_aLogger->TraceInfo(sstr.str().c_str());
    return 0;
}


int OCCProcessor::loadStepModel(Standard_CString sModelName)
{
    std::cout << "start to load step " << sModelName << std::endl;

    STEPControl_Reader aReader;
    TCollection_AsciiString sPath("../Models/");
    sPath += sModelName;

    aReader.ReadFile(sPath.ToCString());
    aReader.TransferRoots();
    m_aShape = aReader.OneShape();

    BRepMesh_IncrementalMesh BMesh(m_aShape, 0.1, Standard_True);

    std::cout << "end of step loading\n" << std::endl;
    return 0;
}

int OCCProcessor::loadStlModel(Standard_CString sModelName)
{

    std::cout << "start to load stl " << sModelName << std::endl;

    TCollection_AsciiString sPath("../Models/");
    sPath += sModelName;

    StlAPI_Reader aReader_Stl;
    aReader_Stl.Read(m_aShape, sPath.ToCString());

    BRepMesh_IncrementalMesh BMesh(m_aShape, 0.1, Standard_True);

    std::cout << "end of stl reading" << std::endl;
    return 0;
}

int OCCProcessor::stretchWidth(float newWidth)
{
    std::cout << "start to stretch width " << newWidth << std::endl;
    ModelProperty newProperty = m_aProperty;
    newProperty.width = newWidth;
    updateShape(newProperty);
    return 0;
}

int OCCProcessor::printModelStatus()
{
    std::cout << "start to print status\n" << std::endl;
    // toUnityMesh(m_aShape);
    return 0;
}

int OCCProcessor::toMeshString(std::string& mesh_str)
{
    std::cout << "start to generate mesh" << std::endl;
    clock_t start, end;
    start = clock();
    BRepMesh_IncrementalMesh BMesh(m_aShape, 0.1, Standard_True);

    ////////////// to mesh
    mesh_str = "";
    Message_ProgressRange theProgress = Message_ProgressRange();
    Standard_Integer aNbNodes = 0;
    Standard_Integer aNbTriangles = 0;

    // calculate total number of the nodes and triangles
    int count = 0;
    for (TopExp_Explorer anExpSF(m_aShape, TopAbs_FACE); anExpSF.More(); anExpSF.Next(), count++)
    {
        std::cout << "calculate face " << count << std::endl;
        TopLoc_Location aLoc;
        Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(TopoDS::Face(anExpSF.Current()), aLoc);
        if (!aTriangulation.IsNull())
        {
            aNbNodes += aTriangulation->NbNodes();
            aNbTriangles += aTriangulation->NbTriangles();
        }
    }

    if (aNbTriangles == 0)
    {
        // No triangulation on the shape
        std::cout << "No triangulation on the shape";
        return Standard_False;
    }

    // create temporary triangulation
    Handle(Poly_Triangulation) aMesh = new Poly_Triangulation(aNbNodes, aNbTriangles, Standard_False);
    // count faces missing triangulation
    Standard_Integer aNbFacesNoTri = 0;
    // fill temporary triangulation
    Standard_Integer aNodeOffset = 0;
    Standard_Integer aTriangleOffet = 0;
    // retrival all faces
    for (TopExp_Explorer anExpSF(m_aShape, TopAbs_FACE); anExpSF.More(); anExpSF.Next())
    {
        const TopoDS_Shape& aFace = anExpSF.Current();
        TopLoc_Location aLoc;
        Handle(Poly_Triangulation) aTriangulation = BRep_Tool::Triangulation(TopoDS::Face(aFace), aLoc);
        if (aTriangulation.IsNull())
        {
            ++aNbFacesNoTri;
            continue;
        }

        const TColgp_Array1OfPnt& aNodes = aTriangulation->Nodes();
        const Poly_Array1OfTriangle& aTriangles = aTriangulation->Triangles();

        // copy nodes
        gp_Trsf aTrsf = aLoc.Transformation();
        for (Standard_Integer aNodeIter = aNodes.Lower(); aNodeIter <= aNodes.Upper(); ++aNodeIter)
        {
            gp_Pnt aPnt = aNodes(aNodeIter);
            aPnt.Transform(aTrsf);
            aMesh->ChangeNode(aNodeIter + aNodeOffset) = aPnt;
        }

        // copy triangles
        const TopAbs_Orientation anOrientation = anExpSF.Current().Orientation();
        for (Standard_Integer aTriIter = aTriangles.Lower(); aTriIter <= aTriangles.Upper(); ++aTriIter)
        {
            Poly_Triangle aTri = aTriangles(aTriIter);

            Standard_Integer anId[3];
            aTri.Get(anId[0], anId[1], anId[2]);
            if (anOrientation == TopAbs_REVERSED)
            {
                // Swap 1, 2.
                Standard_Integer aTmpIdx = anId[1];
                anId[1] = anId[2];
                anId[2] = aTmpIdx;
            }

            // Update nodes according to the offset.
            anId[0] += aNodeOffset;
            anId[1] += aNodeOffset;
            anId[2] += aNodeOffset;

            aTri.Set(anId[0], anId[1], anId[2]);
            aMesh->ChangeTriangle(aTriIter + aTriangleOffet) = aTri;
        }

        aNodeOffset += aNodes.Size();
        aTriangleOffet += aTriangles.Size();
    }

    ////////////// to string
    std::cout << "start to convert mesh string" << std::endl;
    // note that space after 'solid' is necessary for many systems
    char aBuffer[512];
    memset(aBuffer, 0, sizeof(aBuffer));

    const Standard_Integer NBTriangles = aMesh->NbTriangles();
    Message_ProgressScope aPS(theProgress, "Triangles", NBTriangles);

    const TColgp_Array1OfPnt& aNodes = aMesh->Nodes();
    const Poly_Array1OfTriangle& aTriangles = aMesh->Triangles();
    Standard_Integer anElem[3] = { 0, 0, 0 };
    for (Standard_Integer aTriIter = 1; aTriIter <= NBTriangles; ++aTriIter)
    {
        const Poly_Triangle& aTriangle = aTriangles(aTriIter);
        aTriangle.Get(anElem[0], anElem[1], anElem[2]);

        const gp_Pnt aP1 = aNodes(anElem[0]);
        const gp_Pnt aP2 = aNodes(anElem[1]);
        const gp_Pnt aP3 = aNodes(anElem[2]);

        const gp_Vec aVec1(aP1, aP2);
        const gp_Vec aVec2(aP1, aP3);
        gp_Vec aVNorm = aVec1.Crossed(aVec2);
        if (aVNorm.SquareMagnitude() > gp::Resolution())
        {
            aVNorm.Normalize();
        }
        else
        {
            aVNorm.SetCoord(0.0, 0.0, 0.0);
        }

        Sprintf(aBuffer,
            " facet normal % 12e % 12e % 12e\n"
            "   outer loop\n"
            "     vertex % 12e % 12e % 12e\n"
            "     vertex % 12e % 12e % 12e\n"
            "     vertex % 12e % 12e % 12e\n"
            "   endloop\n"
            " endfacet\n",
            aVNorm.X(), aVNorm.Y(), aVNorm.Z(),
            aP1.X(), aP1.Y(), aP1.Z(),
            aP2.X(), aP2.Y(), aP2.Z(),
            aP3.X(), aP3.Y(), aP3.Z());
        mesh_str = mesh_str + std::string(aBuffer);
        // update progress only per 1k triangles
        if ((aTriIter % IND_THRESHOLD) == 0)
        {
            if (!aPS.More())
                return Standard_False;
            aPS.Next(IND_THRESHOLD);
        }
    }
    end = clock();
    std::stringstream sstr;
    sstr << "To mesh " << std::to_string((end - start) * 1000 / CLOCKS_PER_SEC).c_str() << "ms";
    // m_aLogger->TraceInfo(sstr.str().c_str());
    return 0;
}
