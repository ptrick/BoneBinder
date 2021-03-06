#include "Assert.h"
#include "ContentManager.h"
#include "DisplayWindow.h"
#include "Font.h"
#include "Model.h" 
#include "MeshGLWrapper.h"
#include "ShaderManager.h"
#include "Vertex.h"
#include "Texture2D.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>
#include <glm/glm.hpp>

ContentManager::ContentManager(ShaderManager &shaderManager, DisplayWindow &displayWindow)
: mShaderManager(shaderManager),
mDisplayWindow(displayWindow)
{
}

ContentManager::~ContentManager()
{
}

ENGINE_API std::vector<Model> ContentManager::LoadModelsFromFile(const std::string &path) const
{
	try
	{
		std::vector<Model> models;

		Assimp::Importer importer;

		const aiScene *scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		if (!scene)
		{
			throwLoadException(importer.GetErrorString());
		}

		if (!scene->mMeshes || scene->mNumMeshes <= 0)
		{
			throwLoadException("No meshes found.");
		}

		for (unsigned int n = 0; n < scene->mNumMeshes; n++)
		{
			aiMesh *mesh = scene->mMeshes[n];

			if (!mesh)
			{
				throwLoadException("Empty mesh.");
			}

			std::string meshName = std::string(mesh->mName.C_Str());

			if (!mesh->HasFaces())
			{
				throwLoadException("Mesh " + meshName + " has no faces.");
			}

			if (!mesh->HasPositions())
			{
				throwLoadException("Mesh " + meshName + " has no positions.");
			}

			if (!mesh->HasNormals())
			{
				throwLoadException("Mesh " + meshName + " has no normals.");
			}

			Model model;

			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;

			aiColor4D *vertexColors = nullptr;
			bool hasVertexColors = false;
			for (unsigned int n = 0; n < AI_MAX_NUMBER_OF_COLOR_SETS; n++)
			{
				if (mesh->HasVertexColors(n))
				{
					hasVertexColors = true;
					vertexColors = mesh->mColors[n];
					break;
				}
			}

			bool hasTextureCoords = false;
			aiVector3D *textureCoords = nullptr;
			for (unsigned int n = 0; n < AI_MAX_NUMBER_OF_TEXTURECOORDS; n++)
			{
				if (mesh->HasTextureCoords(n))
				{
					hasTextureCoords = true;
					textureCoords = mesh->mTextureCoords[n];
				}
			}

			for (unsigned int n = 0; n < mesh->mNumVertices; n++)
			{
				aiVector3D &pos = mesh->mVertices[n];

				glm::vec3 col(1.0f, 1.0f, 1.0f);
				if (hasVertexColors)
				{
					aiColor4D &vc = vertexColors[n];
					col = glm::vec3(vc.r, vc.g, vc.b);
				}
				
				glm::vec2 texCrd(0.0f, 0.0f);
				if (hasTextureCoords)
				{
					aiVector3D &vt = textureCoords[n];
					texCrd = glm::vec2(vt.x, vt.y);
				}

				aiVector3D &normalV = mesh->mNormals[n];

				glm::vec3 normal = glm::vec3(normalV.x, normalV.y, normalV.z);

				vertices.push_back(Vertex(
					glm::vec3(pos.x, pos.y, pos.z),
					col, texCrd, normal));
			}

			for (unsigned int n = 0; n < mesh->mNumFaces; n++)
			{
				aiFace &f = mesh->mFaces[n];
				Assert(f.mNumIndices == 3, "Face number of indices != 3.");
				for (unsigned int i = 0; i < 3; i++)
					indices.push_back(f.mIndices[i]);
			}

			std::shared_ptr<MeshGLWrapper> targetMesh = std::shared_ptr<MeshGLWrapper>(new MeshGLWrapper());

			targetMesh->Initialize(vertices, indices);

			model.AddMesh(targetMesh);

			models.push_back(model);
		}

		return models;
	}
	catch (EngineException &e)
	{
		mDisplayWindow.ShowNotificationMessageBox(e.What(), MessageType::ERROR_MESSAGE_TYPE);
		return std::vector<Model>();
	}
}

std::shared_ptr<Shader> ContentManager::LoadShaderFromFile(const std::string &path) const
{
	try
	{
		return std::shared_ptr<Shader>(new Shader(path, mShaderManager));
	}
	catch (EngineException &e)
	{
		mDisplayWindow.ShowNotificationMessageBox(e.What(), MessageType::ERROR_MESSAGE_TYPE);
		return std::shared_ptr<Shader>();
	}
}

std::shared_ptr<Texture2D> ContentManager::LoadTextureFromFile(const std::string &path) const
{
	try
	{
		return std::shared_ptr<Texture2D>(new Texture2D(path));
	}
	catch (EngineException &e)
	{
		mDisplayWindow.ShowNotificationMessageBox(e.What(), MessageType::ERROR_MESSAGE_TYPE);
		return std::shared_ptr<Texture2D>();
	}
}

std::shared_ptr<Font> ContentManager::LoadFontFromFile(const std::string &path, float fontSize) const
{
	try
	{
		return std::shared_ptr<Font>(new Font(path, fontSize));
	}
	catch (EngineException &e)
	{
		mDisplayWindow.ShowNotificationMessageBox(e.What(), MessageType::ERROR_MESSAGE_TYPE);
		return std::shared_ptr<Font>();
	}
}

void ContentManager::throwLoadException(std::string message) const
{
	ContentLoadException e(message);
	throw e;
}

ContentLoadException::ContentLoadException(std::string errorMsg)
: EngineException("Content Load", errorMsg)
{
}