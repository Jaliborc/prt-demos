#ifndef _VisualHandModel_H_
#define _VisualHandModel_H_

#include <string>
#include <unordered_map>

#include<osg/Image>
#include<osg/StateSet>
#include<osg/Texture2D>

#include <osgAnimation/Bone>
#include <osgAnimation/Skeleton>

#include <osgAnimation/UpdateBone>
#include <osgAnimation/StackedTransform>
#include <osgAnimation/StackedQuaternionElement>

#include <armadillo>

using namespace std;
using namespace osg;

class VisualHandModel
{
public:
    /** Constructor */
    VisualHandModel( const string& fileName );

    /** Destructor */
    virtual ~VisualHandModel();

    //Create a hash map that stores all the stackedQuatElements for each bone
    typedef std::unordered_map<string, ref_ptr<osgAnimation::StackedQuaternionElement>> Map_Bone;

    /** Visit all child bones of the skeleton and add a stackedQuatElement to their updateCallback */
    void initializeBones( osg::Node* currNode );

    /** Update the rotation of the bone defined by searchName */
    void setBoneRotation( const string& searchName, osg::Quat rotationQuat );

    /** Getters */
    osgAnimation::Skeleton* getSkeleton(){ return skeleton.get(); }
    osg::Geode* getMesh(){ return mesh.get(); }
    osg::Image* getDiffuseImage(){ return diffuse_image.get(); }
    osg::Image* getNormalImage(){ return normal_image.get(); }
    osg::Texture2D* getDiffuseTexture(){ return diffuse_texture.get(); }
    osg::Texture2D* getNormalTexture(){ return normal_texture.get(); }
    osg::StateSet* getStateSet(){ return stateSet.get(); }
    // RBF Info
    string getFingerName(int fingerNum, int jointNum){ return fingerNames[fingerNum-1][0] + jointNames[jointNum-1]; }
//    float* getFingerYHat(int finger, int joint);
    arma::mat getrbfData(int finger, int joint){ return rbfData[finger-1][joint-1]; }
    arma::mat getWeightData(int finger, int joint){ return weightData[finger-1][joint-1]; }

    /** Setters */
    // Color
    void setDiffuseTexture( const string& filePath );
    // Bump
    void setNormalTexture( const string& filePath );
    // RBF Info
//    void setSampleDimensions( const string& filePath, int sampleData[][][] );
    void setFingerRBFData( const string& filePath, int finger, int joint, int rbfSampleNum );
    void setFingerWeightData( const string& filePath, int finger, int joint, int rbfSampleNum );
//    void setFingerYHat();


    /** Traverse down tree to find the skeleton node */
    Node* findSkeleton( osg::Node* currNode );

    /** Traverse down tree to find the mesh geometry node */
    Node* findMesh( osg::Node* currNode );

    /** Traverse down tree to find node that matches given name */
    Node* findNamedNode( const string& searchName, osg::Node* currNode );

    /** Print the hierarchy of all the bones in the skeleton */
    void printSkeleton();

private:
    ref_ptr<osgAnimation::Skeleton> skeleton; 
    osg::Node* skelNode;
    ref_ptr<osg::Geode> mesh; 
    osg::Node* meshNode;
    ref_ptr<osg::Image> diffuse_image;
    ref_ptr<osg::Image> normal_image;
    ref_ptr<osg::Texture2D> diffuse_texture;
    ref_ptr<osg::Texture2D> normal_texture;
    ref_ptr<osg::StateSet> stateSet;
    Map_Bone map_bone;

    // RBF Info
    arma::mat rbfData[5][3]; //[finger][joint]
    arma::mat weightData[5][3];
//    float y_hat[5][3];
    string fingerNames[5][5]; 
    string jointNames[3];
//    int sampleData[5][3][3]; //[finger][joint][sampleSize imgHeight imgWidth]
};

#endif  // _VisualHandModel_H_