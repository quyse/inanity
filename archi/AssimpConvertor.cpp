#include "AssimpConvertor.hpp"
#include "Vertex.hpp"
#include "../deps/assimp/repo/include/assimp/Importer.hpp"
#include "../deps/assimp/repo/include/assimp/scene.h"
#include "../deps/assimp/repo/include/assimp/postprocess.h"

String AssimpConvertor::GetCommand() const
{
	return "assimp";
}

void AssimpConvertor::PrintHelp() const
{
	std::cout << "Converts model to Inanity geometry format.\n";
	std::cout << "assimp [--bump] [--] <source model file> <result .geo>\n";
	Assimp::Importer importer;
	aiString extensions;
	importer.GetExtensionList(extensions);
	std::cout << "Supported extensions: " << extensions.C_Str() << "\n";
}

void AssimpConvertor::Run(const std::vector<String>& arguments)
{
	bool needBump = false;
	String sourceFileName, destFileName;

	for(size_t i = 0; i < arguments.size(); ++i)
	{
		const String& arg = arguments[i];
		if(arg.length() > 2 && arg[0] == '-' && arg[1] == '-')
		{
			String flag = arg.substr(2);
			if(flag == "bump")
				needBump = true;
			else
				THROW("Unknown flag: " + arg);
		}
		else
		{
			// skip -- flag separator
			if(arg == "--")
				++i;

			// the rest should be exactly two arguments
			if(i + 2 != arguments.size())
				THROW("Should be exactly two arguments after flags");

			sourceFileName = arguments[i];
			destFileName = arguments[i + 1];

			break;
		}
	}

	ptr<FileSystem> fileSystem = Platform::FileSystem::GetNativeFileSystem();

	Assimp::Importer importer;
	ptr<File> file = fileSystem->LoadFile(sourceFileName);
	const aiScene* scene = importer.ReadFileFromMemory(file->GetData(), file->GetSize(),
		// calculate tangents and binormals
		(needBump ? aiProcess_CalcTangentSpace : 0) |
		// this flag is required for index buffer
		aiProcess_JoinIdenticalVertices |
		// only 3-vertex faces
		aiProcess_Triangulate |
		// optimize order of vertices
		aiProcess_ImproveCacheLocality |
		// sort by primitive type
		aiProcess_SortByPType |
		// get right Y uv coord
		aiProcess_FlipUVs |
		// CW order
		aiProcess_FlipWindingOrder
		);

	if(!scene)
		THROW(String("Can't load geometry with assimp: ") + importer.GetErrorString());

	if(scene->mNumMeshes != 1)
		THROW("Number of meshes is not one");

	const aiMesh* mesh = scene->mMeshes[0];

	if(mesh->GetNumUVChannels() != 1)
		THROW("Number of UV channels is not one");

	ptr<File> verticesFile;

	if(needBump)
	{
		verticesFile = NEW(MemoryFile(mesh->mNumVertices * sizeof(BumpVertex)));
		BumpVertex* vertices = (BumpVertex*)verticesFile->GetData();
		for(int i = 0; i < (int)mesh->mNumVertices; ++i)
		{
			// inverse bump matrix
			Eigen::Matrix3f bump;
			bump.row(0)(0) = mesh->mTangents[i].x;
			bump.row(0)(1) = mesh->mTangents[i].y;
			bump.row(0)(2) = mesh->mTangents[i].z;
			bump.row(1)(0) = mesh->mBitangents[i].x;
			bump.row(1)(1) = mesh->mBitangents[i].y;
			bump.row(1)(2) = mesh->mBitangents[i].z;
			bump.row(2)(0) = mesh->mNormals[i].x;
			bump.row(2)(1) = mesh->mNormals[i].y;
			bump.row(2)(2) = mesh->mNormals[i].z;

			Eigen::Matrix3f invBump = bump.inverse();

			BumpVertex& vertex = vertices[i];
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;
			vertex.transform1 = vec3(invBump.row(0)(0), invBump.row(0)(1), invBump.row(0)(2));
			vertex.transform2 = vec3(invBump.row(1)(0), invBump.row(1)(1), invBump.row(1)(2));
			vertex.transform3 = vec3(invBump.row(2)(0), invBump.row(2)(1), invBump.row(2)(2));
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}
	}
	else
	{
		verticesFile = NEW(MemoryFile(mesh->mNumVertices * sizeof(Vertex)));
		Vertex* vertices = (Vertex*)verticesFile->GetData();
		for(int i = 0; i < (int)mesh->mNumVertices; ++i)
		{
			Vertex& vertex = vertices[i];
			vertex.position.x = mesh->mVertices[i].x;
			vertex.position.y = mesh->mVertices[i].y;
			vertex.position.z = mesh->mVertices[i].z;
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}
	}

	int facesCount = (int)mesh->mNumFaces;
	int indicesCount = facesCount * 3;
	ptr<File> indicesFile = NEW(MemoryFile(indicesCount * sizeof(unsigned short)));
	unsigned short* indices = (unsigned short*)indicesFile->GetData();
	for(int i = 0; i < facesCount; ++i)
	{
		for(int j = 0; j < 3; ++j)
			indices[i * 3 + j] = mesh->mFaces[i].mIndices[j];
	}

	fileSystem->SaveFile(verticesFile, destFileName + ".vertices");
	fileSystem->SaveFile(indicesFile, destFileName + ".indices");
}
