#include <osg/MatrixTransform>
#include <osg/Projection>
using namespace osg;

class FullscreenQuad : public Projection {
public:
    FullscreenQuad() : Projection() { 
        Geometry* quad = new Geometry;
        Vec3Array* vertices = new Vec3Array(4);
        vertices->at(0).set(0, 1, 0);
        vertices->at(1).set(0, 0, 0);
        vertices->at(2).set(1, 0, 0);
        vertices->at(3).set(1, 1, 0);

        Vec2Array* coords = new Vec2Array(4);
        coords->at(0).set(0, 1);
        coords->at(1).set(0, 0);
        coords->at(2).set(1, 0);
        coords->at(3).set(1, 1);

        quad->setVertexArray(vertices);
        quad->setTexCoordArray(0, coords);
        quad->addPrimitiveSet(new DrawArrays(PrimitiveSet::QUADS, 0, 4)); 

        Geode* geode = new Geode; 
        geode->addDrawable(quad);

        MatrixTransform* transform = new MatrixTransform(); 
        transform->setReferenceFrame(Transform::ABSOLUTE_RF);
        transform->addChild(geode);  

        setMatrix(Matrix::ortho(0, 1, 0, 1, 0, -1));
        addChild(transform);
    }
};