///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// Inside CreateGLTexture(...) before calling stbi_load, add:
	{
		// Quick check: can the runtime find the file at this path?
		std::ifstream fcheck(filename);
		if (!fcheck.good()) {
			std::cout << "CreateGLTexture: file not found or inaccessible: " << filename << std::endl;
		}
		else {
			fcheck.close();
		}
	}

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationX * rotationY * rotationZ * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{

	{
		if (NULL == m_pShaderManager) return;

		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int slot = FindTextureSlot(textureTag);
		if (slot < 0)
		{
			std::cout << "SetShaderTexture: texture tag not found: " << textureTag << std::endl;
			return;
		}

		std::cout << "Setting sampler " << g_TextureValueName << " to slot " << slot << " for tag " << textureTag << std::endl;

		// Ensure shader program is active here (make sure ShaderManager::use() or equivalent is called)
		m_pShaderManager->setSampler2DValue(g_TextureValueName, slot);
	}

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

 /***********************************************************
  *  LoadSceneTextures()
  *
  *  This method is used for preparing the 3D scene by loading
  *  the shapes, textures in memory to support the 3D scene
  *  rendering
  ***********************************************************/
void SceneManager::LoadSceneTextures() 
{
	bool bReturn = false;

	// two '../' if attempting to run from executable in debug folder
	// one '../' if attempting to run from solution in VS

	bReturn = CreateGLTexture("../../Utilities/textures/cloth-texture.jpg", "lampShade");

	bReturn = CreateGLTexture("../../Utilities/textures/wood-grain-texture.jpg", "lampPost");

	bReturn = CreateGLTexture("../../Utilities/textures/desktop-texture.jpg", "desktop");

	bReturn = CreateGLTexture("../../Utilities/textures/plastic-texture.jpg", "headphones");

	bReturn = CreateGLTexture("../../Utilities/textures/drywall.jpg", "bottle");

	bReturn = CreateGLTexture("../../Utilities/textures/blue-plastic-texture.jpg", "lid");

	bReturn = CreateGLTexture("../../Utilities/textures/card-box-texture.jpg", "cardBox");

	bReturn = CreateGLTexture("../../Utilities/textures/label-texture.jpg", "label");
	
	// after the texture image data is loaded into memory, the
	// loaded textures need to be bound to texture slots - there
	// are a total of 16 available slots for scene textures
	BindGLTextures();
}

 /***********************************************************
  *  DefineObjectMaterials()
  *
  *  This method is used for configuring the various material
  *  settings for all of the objects within the 3D scene.
  ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	/*** STUDENTS - add the code BELOW for defining object materials. ***/
	/*** There is no limit to the number of object materials that can ***/
	/*** be defined. Refer to the code in the OpenGL Sample for help  ***/
	OBJECT_MATERIAL shapeMaterial;
	shapeMaterial.ambientColor = glm::vec3(0.5f, 0.5f, 0.5f);
	shapeMaterial.ambientStrength = 0.4f;
	shapeMaterial.diffuseColor = glm::vec3(0.8f, 0.8f, 0.8f);
	shapeMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	shapeMaterial.shininess = 16.0;
	shapeMaterial.tag = "shape";

	m_objectMaterials.push_back(shapeMaterial);

	OBJECT_MATERIAL lampShadeMaterial;
	shapeMaterial.ambientColor = glm::vec3(10.0f, 10.0f, 10.0f);
	shapeMaterial.ambientStrength = 10.0f;
	shapeMaterial.diffuseColor = glm::vec3(10.0f, 10.0f, 10.0f);
	shapeMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	shapeMaterial.shininess = 16.0;
	shapeMaterial.tag = "lampShade";

	m_objectMaterials.push_back(lampShadeMaterial);

}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting, if no light sources have
	// been added then the display window will be black - to use the 
	// default OpenGL lighting then comment out the following line
	m_pShaderManager->setBoolValue(g_UseLightingName, true);

	/*** STUDENTS - add the code BELOW for setting up light sources ***/
	/*** Up to four light sources can be defined. Refer to the code ***/
	/*** in the OpenGL Sample for help                              ***/

	// sets values for first light source
	m_pShaderManager->setVec3Value("lightSources[0].position", -5.0f, 15.0f, -2.5f);
	m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 1.0f, 1.0f, 0.5f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 5.0f, 4.5f, 4.0f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 0.4f, 0.4f, 0.2f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 2.0f);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 2.0f);
	m_pShaderManager->setFloatValue("lightSources[0].constant", 1.0f);
	m_pShaderManager->setFloatValue("lightSources[0].linear", 0.10f);
	m_pShaderManager->setFloatValue("lightSources[0].quadratic", 0.002f);

	// set values for the second light source
	m_pShaderManager->setVec3Value("lightSources[1].position", 0.0f, 30.0f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.75f, 0.6f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 1.5f, 1.25f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", 0.2f, 0.15f, 0.0f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 1.0f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 2.0f);
	m_pShaderManager->setFloatValue("lightSources[1].constant", 1.0f);
	m_pShaderManager->setFloatValue("lightSources[1].linear", 0.10f);
	m_pShaderManager->setFloatValue("lightSources[1].quadratic", 0.002f);
}

/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{
	// define the materials for the objects in the scene and set up the light sources
	DefineObjectMaterials();
	SetupSceneLights();

	// loads the texture images from files and creates OpenGL textures in memory
	LoadSceneTextures();

	// enable blending for transparency in textures
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadTaperedCylinderMesh();
	m_basicMeshes->LoadTorusMesh();
	m_basicMeshes->LoadSphereMesh();
}

/* method that eliminates the need to copy and paste lengthy
   code block to set transformation and shader parameters */
void SceneManager::SetParameters(glm::vec3 scaleXYZ, glm::vec3 rotationXYZ, glm::vec3 positionXYZ, glm::vec4 RGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
{

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		rotationXYZ[0],
		rotationXYZ[1],
		rotationXYZ[2],
		positionXYZ);

	// set the color values into the shader
	SetShaderColor(RGBA[0], RGBA[1], RGBA[2], RGBA[3]);
}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	// create plane to act as desktop
	SetParameters(
		// set scale
		glm::vec3(20.0f, 1.0f, 10.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(10.0f, 0.0f, 0.0f),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);

	// sets texture in shader manager to be applied to shape
	SetShaderTexture("desktop");
	SetShaderMaterial("shape");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();

	// create plane to act as wall
	SetParameters(
		// set scale
		glm::vec3(30.0f, 1.0f, 20.0f),
		// set rotation
		glm::vec3(90.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(0.0f, -5.0f, -10.0f),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);

	// sets texture in shader manager to be applied to shape
	SetShaderTexture("bottle");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();
	/****************************************************************/
	/****************************************************************/

	/*******************BEGIN BOX COMPONENTS*************************/

	/****************************************************************/
	/****************************************************************/

	// create plane to act as background/landscape
	SetParameters(
		// set scale
		glm::vec3(4.0f, 2.0f, 1.5f),
		// set rotation
		glm::vec3(0.0f, -45.0f, 0.0f),
		// set position
		glm::vec3(0.0f, 1.0f, -2.0f),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("cardBox");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();
	/****************************************************************/
	/****************************************************************/

	/********************END BOX COMPONENTS**************************/

	/****************************************************************/
	/****************************************************************/

	/****************************************************************/
	/****************************************************************/

	/******************BEGIN BOTTLE COMPONENTS***********************/

	/****************************************************************/
	/****************************************************************/

	// variables used to easily adjust the position and scale of vitamin bottle
	float bottleOffsetX = -7.0f;
	float bottleOffsetY = 0.0f;
	float bottleOffsetZ = 2.0f;
	float bottleScale = 1.5f;

	// TaperCylinder; bottom of bottle
	SetParameters(
		// set scale
		glm::vec3(bottleScale * 0.6f,
			bottleScale * 0.15f,
			bottleScale * 0.6f),
		// set rotation
		glm::vec3(180.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 0.20f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("bottle");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// Cylinder; lower body of bottle
	SetParameters(
		// set scale
		glm::vec3(bottleScale * 0.6f,
			bottleScale * 0.25f,
			bottleScale * 0.6f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 0.20f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("bottle");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// Cylinder; label of bottle
	SetParameters(
		// set scale
		glm::vec3(bottleScale * 0.6f,
			bottleScale * 1.0f,
			bottleScale * 0.6f),
		// set rotation
		glm::vec3(0.0f, -60.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 0.45f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("label");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh(false, false, true);
	/****************************************************************/

	// Cylinder; upper body of bottle
	SetParameters(
		// set scale
		glm::vec3(bottleScale * 0.6f,
			bottleScale * 0.25f,
			bottleScale * 0.6f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 1.45f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("bottle");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// TaperCylinder; top of bottle
	SetParameters(
		// set scale
		glm::vec3(bottleScale * 0.6f,
			bottleScale * 0.15f,
			bottleScale * 0.6f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 1.7f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("bottle");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// Cylinder; neck of bottle
	SetParameters(
		// set scales
		glm::vec3(bottleScale * 0.3f,
			bottleScale * 0.15f,
			bottleScale * 0.3f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 1.8f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("bottle");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// Cylinder; lid of bottle
	SetParameters(
		// set scales
		glm::vec3(bottleScale * 0.4f,
			bottleScale * 0.20f,
			bottleScale * 0.4f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(bottleOffsetX, bottleOffsetY + (bottleScale * 1.9f), bottleOffsetZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lid");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/
	/****************************************************************/

	/*******************END BOTTLE COMPONENTS************************/

	/****************************************************************/
	/****************************************************************/

	/****************************************************************/
	/****************************************************************/

	/*****************BEGIN HEADPHONE COMPONENTS*********************/

	/****************************************************************/
	/****************************************************************/
	float headphoneOffsetX = -1.0f;
	float headphoneOffsetY = 0.0f;
	float headphoneOffsetZ = 2.5f;

	// half torus; head band
	SetParameters(
		// set scale
		glm::vec3(1.5f, 2.5f, 2.0f),
		// set rotation
		glm::vec3(-90.0f, -10.0f, 90.0f),
		// set position
		glm::vec3(headphoneOffsetX, headphoneOffsetY + 0.6f, headphoneOffsetZ),
		// set shader color
		glm::vec4(0.05f, 0.05f, 0.05f, 1.0f)
	);
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawHalfTorusMesh();
	/****************************************************************/

	// cylinder; left ear cover
	SetParameters(
		// set scale
		glm::vec3(0.6f, 0.6f, 0.6f),
		// set rotation
		glm::vec3(0.0f, 90.0f, 90.0f),
		// set position
		glm::vec3(headphoneOffsetX + 0.5f, headphoneOffsetY + 0.6f, headphoneOffsetZ - 2.0f),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("headphones");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// cylinder; right ear cover
	SetParameters(
		// set scale
		glm::vec3(0.6f, 0.6f, 0.6f),
		// set rotation
		glm::vec3(0.0f, 90.0f, -90.0f),
		// set position
		glm::vec3(headphoneOffsetX + 0.5f, headphoneOffsetY + 0.6f, headphoneOffsetZ + 2.0f),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("headphones");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/
	/****************************************************************/

	/*******************END HEADPHONE COMPONENTS*********************/

	/****************************************************************/
	/****************************************************************/

	/****************************************************************/
	/****************************************************************/

	/********************BEGIN LAMP COMPONENTS***********************/

	/****************************************************************/
	/****************************************************************/

	// variables to use for X and Z axis position of lamp components
	// for adjusting position of lamp in scene
	float lampPosX = -5.0f;
	float lampPosZ = -2.5f;

	// base of lamp
	SetParameters(
		// set scale
		glm::vec3(4.0f, 1.0f, 4.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 0.0f, lampPosZ),
		// set shader color
		glm::vec4(0.5f, 0.2f, 0.25f, 1.0f)
	);

	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();
	/****************************************************************/

	// cylinder; start of lamp pole
	SetParameters(
		// set scale
		glm::vec3(1.0f, 0.5f, 1.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 0.5f, lampPosZ),
		// set shader color
		glm::vec4(0.0f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// tapered cylinder; bottom of 1st groove in lamp pole
	SetParameters(
		// set scale
		glm::vec3(1.0f, 0.5f, 1.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 1.0f, lampPosZ),
		// set shader color
		glm::vec4(0.5f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// tapered cylinder; top of 1st groove in lamp pole
	SetParameters(
		// set scale
		glm::vec3(1.0f, 0.5f, 1.0f),
		// set rotation
		glm::vec3(180.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 1.25f, lampPosZ),
		// set shader color
		glm::vec4(0.5f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// cylinder; large center of pole
	SetParameters(
		// set scale
		glm::vec3(1.0f, 3.0f, 1.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 1.25f, lampPosZ),
		// set shader color
		glm::vec4(0.0f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// tapered cylinder; bottom of 1st groove in lamp pole
	SetParameters(
		// set scale
		glm::vec3(1.0f, 0.5f, 1.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 4.25f, lampPosZ),
		// set shader color
		glm::vec4(0.5f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// cylinder; middle of 2nd groove in lamp pole
	SetParameters(
		// set scale
		glm::vec3(0.8f, 0.75f, 0.8f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 4.25f, lampPosZ),
		// set shader color
		glm::vec4(0.0f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// tapered cylinder; top of 1st groove in lamp pole
	SetParameters(
		// set scale
		glm::vec3(1.0f, 0.5f, 1.0f),
		// set rotation
		glm::vec3(180.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 5.0f, lampPosZ),
		// set shader color
		glm::vec4(0.5f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// cylinder; upper section of pole after 2nd groove
	SetParameters(
		// set scale
		glm::vec3(1.0f, 0.75f, 1.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 5.0f, lampPosZ),
		// set shader color
		glm::vec4(0.0f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();
	/****************************************************************/

	// tapered cylinder; upper section of pole leading to lamp shade
	SetParameters(
		// set scale
		glm::vec3(1.0f, 1.0f, 1.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 5.75f, lampPosZ),
		// set shader color
		glm::vec4(0.5f, 0.2f, 0.25f, 1.0f)
	);
	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampPost");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh();
	/****************************************************************/

	// sphere; lightbulb
	SetParameters(
		// set scale
		glm::vec3(0.5f, 0.75f, 0.5f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 7.5f, lampPosZ),
		// set shader color
		glm::vec4(10.0f, 10.0f, 5.0f, 0.8f)
	);
	// draw the mesh with transformation values
	m_basicMeshes->DrawSphereMesh();
	/****************************************************************/
	// sphere; lightbulb
	SetParameters(
		// set scale
		glm::vec3(0.5f, 0.3f, 0.5f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 7.0f, lampPosZ),
		// set shader color
		glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)
	);
	// draw the mesh with transformation values
	m_basicMeshes->DrawSphereMesh();
	/****************************************************************/

	// tapered cylinder; lamp shade
	SetParameters(
		// set scale
		glm::vec3(3.0f, 4.0f, 3.0f),
		// set rotation
		glm::vec3(0.0f, 0.0f, 0.0f),
		// set position
		glm::vec3(lampPosX, 6.5f, lampPosZ),
		// set shader color
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
	);

	// sets texture in shader manager to be applied to shape
	SetShaderTexture("lampShade");
	// sets material in shader manager to be applied to shape
	SetShaderMaterial("shape");
	
	
	// sets the alpha multiplier in the fragment shader to make shape transparent
	glUseProgram(m_pShaderManager->m_programID);
	glUniform1f(
		glGetUniformLocation(m_pShaderManager->m_programID, "alphaMultiplier"),
		0.95f // adjust transparency here
	);

	// draw the mesh with transformation values
	m_basicMeshes->DrawTaperedCylinderMesh(false, false, true);

	// resets alpha multiplier
	glUniform1f(
		glGetUniformLocation(m_pShaderManager->m_programID, "alphaMultiplier"),
		1.0f
	);
	/****************************************************************/
	/****************************************************************/

	/*********************END LAMP COMPONENTS************************/

	/****************************************************************/
	/****************************************************************/
}
