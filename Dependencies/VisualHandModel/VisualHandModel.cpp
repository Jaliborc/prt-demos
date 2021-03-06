#include "VisualHandModel.h"

#include <string>

#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/StateSet>
#include <osg/Texture2D>

#include <osgAnimation/Bone>
#include <osgAnimation/Skeleton>
#include <osgAnimation/StackedTransform>
#include <osgAnimation/StackedQuaternionElement>
#include <osgAnimation/UpdateBone>

#include <osgDB/ReadFile>

#include <armadillo>

using namespace osg;
using namespace std;

//---Constructor---//
VisualHandModel::VisualHandModel(const string& filePath)
{
    // load a model
    ref_ptr<Node> meshScene = osgDB::readNodeFile(filePath);

    // Isolate the skeleton node
    skelNode = findSkeleton(meshScene.get());
    skeleton = dynamic_cast<osgAnimation::Skeleton*>( skelNode );
    meshNode = findMesh(meshScene.get());
    mesh = dynamic_cast<osg::Geode*>( meshNode );
    if(skeleton){
        skeleton->setName("handSkeleton");
   
        //Initialize all the bones in the hand (Adds a StackedQuatElement to each bone that we can manipulate later)
        initializeBones(skeleton.get());

        // Get the model's State Set
        stateSet = skeleton->getOrCreateStateSet();

        // Create Textures
        diffuse_texture = new osg::Texture2D();
        normal_texture = new osg::Texture2D();   
    }
    else{
        cout << "Error: filepath provided in VisualHandModel constructor could not find skeleton node" << endl;
    }

    fingerNames[0][0] = "Thumb";
    fingerNames[1][0] = "Index";
    fingerNames[2][0] = "Mid";
    fingerNames[3][0] = "Ring";
    fingerNames[4][0] = "Pinky"; 
    jointNames[0] = "1";
    jointNames[1] = "2";
    jointNames[2] = "3";

}

//---Destructor---//
VisualHandModel::~VisualHandModel()
{
/*    delete diffuse_texture;
    delete this->normal_texture; 

    if(skeleton){
        delete skeleton;  
    }
    if(diffuse_image){
        delete diffuse_image;
    }
    if(diffuse_image){
        delete diffuse_image;
    }
    if(normal_image){
        delete normal_image;  
    }
    if(state){
        delete state;    
    }
*/
}

//---Setters---//
void VisualHandModel::setDiffuseTexture(const string& filePath){
    if(stateSet){
        diffuse_image = osgDB::readImageFile( filePath );
        diffuse_texture->setImage( diffuse_image.get() );
//        stateSet->setTextureAttributeAndModes( 0, diffuse_texture, osg::StateAttribute::ON );    
    }
}

void VisualHandModel::setNormalTexture(const string& filePath){
    if(stateSet){
        normal_image = osgDB::readImageFile( filePath );
        normal_texture->setImage( normal_image.get() );
//        stateSet->setTextureAttributeAndModes( 1, normal_texture, osg::StateAttribute::ON );    
    }
}

/*
void setSampleDimensions( const string& filePath ){
    string currentData; 
    double convertedData;

    //Load the sampling data
    ifstream dimensionData (filePath);
    if(dimensionData){
        // sampleData[finger][joint][imgHeight imgWidth sampleSize]
        for(int i = 0; i < 5; i++){
            for(int j = 0; j < 3; j++){
                int k = 0;
                //Get the image Height
                getline (dimensionData, currentData);
                sampleData[i][j][k] = atoi(currentData.c_str()); // sampleData[finger][joint][imgHeight imgWidth sampleSize]
                k += 1;
                //Get the image Width
                getline (dimensionData, currentData);
                sampleData[i][j][k] = atoi(currentData.c_str()); //[finger][joint][imgHeight imgWidth sampleSize]
                k += 1;
                //Get the number of EigenVectors
                getline (dimensionData, currentData);
                sampleData[i][j][k] = atoi(currentData.c_str()); //[finger][joint][imgHeight imgWidth sampleSize]
            }
        }
    }
}
*/

void VisualHandModel::setFingerRBFData( const string& filePath, int finger, int joint, int rbfSampleNum ){
    string fileData; 
    int numOfSensors = 16;

    //Initialize the rbf Matrix array
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            rbfData[i][j] = arma::zeros<arma::mat>(rbfSampleNum,numOfSensors);
        }
    }

    //Load Pressure Data
    ifstream pressure (filePath);
    for (int i = 0; i < numOfSensors; i++){
        for (int j = 0; j < rbfSampleNum; j++){
            getline (pressure, fileData);
            rbfData[finger-1][joint-1](j,i) = atof(fileData.c_str());
        }
    }
}

void VisualHandModel::setFingerWeightData( const string& filePath, int finger, int joint, int rbfSampleNum ){
    string fileData; 

    //Initialize the Weight Matrix array
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 3; j++){
            weightData[i][j] = arma::zeros<arma::mat>(rbfSampleNum,rbfSampleNum);
        }
    }

    //Load Radial-Basis-Function Weight Data
    ifstream weights (filePath);
    for (int i = 0; i < rbfSampleNum; i++){
        for (int j = 0; j < rbfSampleNum; j++){
            getline (weights, fileData);
            weightData[finger-1][joint-1](j,i) = atof(fileData.c_str());
        }
    }
}


//Visit all child bones of the skeleton and add a stackedTransform to their updateCallback
void VisualHandModel::initializeBones(osg::Node* currNode)
{
    if(currNode){
        ref_ptr<Group> currGroup;
        currGroup = currNode->asGroup();

        //Visit all children
        for(unsigned int i = 0; i < currGroup->getNumChildren(); i++){
            if(strcmp(currGroup->getChild(i)->className(),"Bone") == 0){
                initializeBones(currGroup->getChild(i));
            } 
        }

        //Add transformElement to Callback Stack
        if(strcmp(currNode->className(), "Bone") == 0){
            //Convert node to bone
            if(ref_ptr<osgAnimation::Bone> bone = dynamic_cast<osgAnimation::Bone*>(currNode)){
                //Create a pointer to the node's updateCallback  
                if(ref_ptr<osgAnimation::UpdateBone> boneUpdate = dynamic_cast<osgAnimation::UpdateBone*>( bone->getUpdateCallback() )){
                    //Add a transformElement to the stack
                    boneUpdate->getStackedTransforms().push_back(new osgAnimation::StackedQuaternionElement(Quat(Vec4f(0.0f, 0.0f, 0.0f, 1.0f))));
                    //Insert pointer to the new transformElement into the Map_Bone
                    map_bone.insert(Map_Bone::value_type( currNode->getName(), dynamic_cast<osgAnimation::StackedQuaternionElement*>(boneUpdate->getStackedTransforms().back().get()) ));
                }
            }
        }
    }
}


//Update the bone rotation
void VisualHandModel::setBoneRotation( const string& searchName, osg::Quat rotationQuat ){
    try{
        map_bone.at(searchName)->setQuaternion( rotationQuat );    
    }
    catch(int e){
        cout << "Error: " << searchName << " not found" << endl;
    }
}


//Traverse down tree to find the skeleton node
Node* VisualHandModel::findSkeleton(osg::Node* currNode)
{
    ref_ptr<osg::Group> currGroup;
    ref_ptr<osg::Node> foundNode;

    if(!currNode){
       return NULL;
    }

    if(strcmp(currNode->className(), "Skeleton") == 0){
       return currNode;
    }

    currGroup = currNode->asGroup();
    if(currGroup){
        for(unsigned int i = 0; i < currGroup->getNumChildren(); i++){
           foundNode = findSkeleton(currGroup->getChild(i));
           if(foundNode){
              return foundNode.get();
           }
        }
    }
    return NULL;
}

/** Traverse down tree to find the mesh geometry node */
Node* VisualHandModel::findMesh( osg::Node* currNode )
{
    ref_ptr<osg::Group> currGroup;
    ref_ptr<osg::Node> foundNode;

    if(!currNode){
       return NULL;
    }

    if(strcmp(currNode->className(), "Geode") == 0){
       return currNode;
    }

    currGroup = currNode->asGroup();
    if(currGroup){
        for(unsigned int i = 0; i < currGroup->getNumChildren(); i++){
           foundNode = findMesh(currGroup->getChild(i));
           if(foundNode){
              return foundNode.get();
           }
        }
    }
    return NULL;
}


//Traverse down tree to find node that matches given name
Node* VisualHandModel::findNamedNode(const string& searchName, osg::Node* currNode)
{
    ref_ptr<Group> currGroup;
    ref_ptr<Node> foundNode;

    if(!currNode){
       return NULL;
    }

    if(currNode->getName() == searchName){
       return currNode;
    }

    currGroup = currNode->asGroup();
    if(currGroup){
        for(unsigned int i = 0; i < currGroup->getNumChildren(); i++){
           foundNode = findNamedNode(searchName, currGroup->getChild(i));
           if(foundNode){
              return foundNode.get();
           }
        }
    }
    return NULL;
}


//---Print bone hierarchy---// ( As found here: //www2.denizyuret.com/bib/wang/Wang2010osg/OpenSceneGraph.3.0.Beginners.Guide.Dec.2010.pdf (pg 119) )
class InfoVisitor : public osg::NodeVisitor
{
public:
    InfoVisitor() : _level(0){
       setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
    }
    std::string spaces(){
       return std::string(_level*2, ' ');
    }
    virtual void apply(osg::Node& node);
    virtual void apply(osg::Geode& node);

protected:
    unsigned int _level;
};

void InfoVisitor::apply(osg::Node& node){
    std::cout << spaces() << node.libraryName() << "::" << node.className() << " - " << node.getName() << std::endl;
    
    _level++;
    traverse(node);
    _level--;
}

void InfoVisitor::apply(osg::Geode& geode){
    std::cout << spaces() << geode.libraryName() << "::" << geode.className() <<  " - " << geode.getName() << std::endl;

    _level++;
    for(unsigned int i=0; i < geode.getNumDrawables(); ++i){
       osg::Drawable* drawable = geode.getDrawable(i);
       std::cout << spaces() << drawable->libraryName() << "::" << drawable->className() <<  " - " << geode.getName() << std::endl;
    }

    traverse(geode);
    _level--;
}

void VisualHandModel::printSkeleton()
{
   InfoVisitor infoVisitor;
   skelNode->accept(infoVisitor);
}

