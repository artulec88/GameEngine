#ifndef __RENDERING_MESH_H__
#define __RENDERING_MESH_H__

#include "Rendering.h"

#include "Math\Matrix.h"
#include "Math\Vector.h"
#include "Math\HeightsGenerator.h"
#include "Utility\ILogger.h"

#include <string>
#include <map>
#include <vector>

//#define HEIGHTS_KD_TREE
#define HEIGHTS_HEIGHTMAP

#ifdef HEIGHTS_KD_TREE
#include "Math\KDTree.h"
#endif

//#define TEXTURE_ATLAS_OFFSET_CALCULATION
//#define MEASURE_MESH_TIME_ENABLED
//#define STORE_MESH_FILE_NAME

// Type cast conversion to make Math::Vector3D possible to use in std::unordered_set object.
namespace std
{
	template <>
	struct hash<Math::Vector3D>
	{
		std::size_t operator()(const Math::Vector3D& vector) const
		{
			return vector.to_size_t();
		}
	};
}

namespace Rendering
{
	namespace MeshBufferTypes
	{
		enum MeshBufferType
		{
			INDEX = 0,
			POSITIONS = 1,
			TEXTURE_COORDINATES = 2,
			NORMALS = 3,
			TANGENTS = 4,
			BITANGENTS = 5,
			INSTANCE = 6,
			COUNT
		}; /* end enum MeshBufferType */
	} /* end namespace MeshBufferTypes */

	namespace MeshAttributeLocations
	{
		enum MeshAttributeLocation
		{
			POSITIONS = 0,
			TEXTURE_COORDINATES = 1,
			NORMALS = 2,
			TANGENTS = 3,
			BITANGENTS = 4,
			COUNT
		}; /* end enum MeshAttributeLocation */
	} /* end namespace MeshAttributeLocations */

	/// <summary>
	/// The low-level representation of the mesh.
	/// </summary>
	class MeshData
	{
		/* ==================== Static variables and functions begin ==================== */
		/* ==================== Static variables and functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Simple mesh data constructor. </summary>
		/// <param name="size"> The size of data the mesh data will store. </param>
		explicit MeshData(GLsizei size);
		/// <summary> Mesh data destructor. </summary>
		~MeshData(void);
		/// <summary> Mesh data copy constructor. </summary>
		/// <param name="meshData"> The mesh data to copy from. </param>
		MeshData(const MeshData& meshData) = delete;
		/// <summary> Mesh data move constructor. </summary>
		/// <param name="meshData"> The mesh data to move from. </param>
		MeshData(MeshData&& meshData);
		/// <summary> Mesh data copy assignment operator. </summary>
		/// <param name="meshData"> The mesh data to copy assign from. </param>
		/// <returns> The newly copy assigned mesh data. </returns>
		MeshData& operator=(const MeshData& meshData) = delete;
		/// <summary> Mesh data move assignment operator. </summary>
		/// <param name="meshData"> The mesh data to move assign from. </param>
		/// <returns> The newly move assigned mesh data. </returns>
		MeshData& operator=(MeshData&& meshData) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Returns the handle to the Vertex Array Object (VAO) used by the mesh data. </summary>
		/// <returns> The handle to the Vertex Array Object (VAO) used by the mesh data. </returns>
		inline GLuint GetVAO() const { return m_vao; }

		/// <summary> Binds the mesh data's Vertex Array Object (VAO) to be currently used by the rendering context. </summary>
		inline void Bind() const
		{
			Rendering::CheckErrorCode(__FUNCTION__, "Started mesh data binding");
			//WARNING_LOG_RENDERING("Binding mesh data \"", ToString(), "\".");
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_vao != 0, Utility::Logging::CRITICAL, "Trying to bind the VAO with value 0");
			glBindVertexArray(m_vao);
			Rendering::CheckErrorCode(__FUNCTION__, "Finished mesh data binding");
		}

		/// <summary> Unbinds the mesh data's Vertex Array Object (VAO). </summary>
		inline void Unbind() const
		{
			Rendering::CheckErrorCode(__FUNCTION__, "Started mesh data unbinding");
			//WARNING_LOG_RENDERING("Unbinding mesh data \"", ToString(), "\".");
			//int index = 0;
			//for (std::vector<GLuint>::const_iterator vboItr = m_vbos.begin(); vboItr != m_vbos.end(); ++vboItr, ++index)
			//{
			//	glDisableVertexAttribArray(index);
			//}
			glBindVertexArray(0);
			Rendering::CheckErrorCode(__FUNCTION__, "Finished mesh data unbinding");
		}

		/// <summary> Checks whether the buffer stored under specified key (<paramref name="buffer/>) is created and available. </summary>
		/// <returns> <code>True</code> if the buffer under specified <paramref name="buffer"/> is available (<code>!= 0</code>) and <code>false</code> otherwise. </returns>
		bool HasVBO(MeshBufferTypes::MeshBufferType buffer) const
		{
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(buffer >= 0 && buffer < MeshBufferTypes::COUNT,
				Utility::Logging::CRITICAL, "Cannot access buffer at index ", buffer, ". Mesh data = \"", *this, "\"");
			return m_buffers[buffer] != 0;
		}

		/// <summary>
		/// Returns the handle to the vertex buffer object stored under specified key (<paramref name="buffer"/>).
		/// </summary>
		/// <param name="buffer"> The index of the VBO we want to get the handle to. </param>
		/// <returns> A handle to the vertex buffer object stored under specified key (<paramref name="buffer"/>). </returns>
		GLuint GetVBO(MeshBufferTypes::MeshBufferType buffer) const
		{
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(buffer >= 0 && buffer < MeshBufferTypes::COUNT,
				Utility::Logging::CRITICAL, "Cannot access buffer at index ", buffer, ". Mesh data = \"", *this, "\"");
			CHECK_CONDITION_EXIT_ALWAYS_RENDERING(m_buffers[buffer] != 0, Utility::Logging::CRITICAL,
				"The buffer under index ", buffer, " is 0. Mesh data = \"", *this, "\"");
			return m_buffers[buffer];
		}

		/// <summary>
		/// Creates the buffer and stores its handle in the array of available Vertex Buffer Objects (VBOs).
		/// </summary>
		/// <param name="buffer"> The buffer key we want to store a handle to new VBO in. </param>
		/// <returns> The handle to the newly created vertex buffer object (VBO). </returns>
		GLuint CreateVBO(MeshBufferTypes::MeshBufferType buffer);

		/// <summary>
		/// Returns the size of the mesh.
		/// </summary>
		/// <returns> The size of the mesh. </returns>
		GLsizei GetSize() const { return m_size; }

		friend std::ostream& operator<<(std::ostream& out, const MeshData& meshData)
		{
			out << "VAO = " << meshData.m_vao << "; VBOs = [";
			for (int i = 0; i < MeshBufferTypes::COUNT; ++i)
			{
				out << meshData.m_buffers[i];
				if (i + 1 < MeshBufferTypes::COUNT)
				{
					out << "; ";
				}
			}
			out << "] ";
			return out;
		}
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary>
		/// Vertex array object.
		/// </summary>
		GLuint m_vao;

		/// <summary>
		/// Vertex buffer objects. A handle to data representing the whole mesh (positions, texture coordinates, normals, etc.).
		/// </summary>
		std::array<GLuint, MeshBufferTypes::COUNT> m_buffers;

		/// <summary>
		/// The size. It represents how much data there is in the vertex buffer object.
		/// </summary>
		GLsizei m_size;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class MeshData */

	class Mesh
	{
	public:
		/// <summary>
		/// The <code>Mesh</code> objects comparator.
		/// </summary>
		/// <remarks>
		/// It is more efficient to pass a function-object in a function call than pass function pointers.
		/// That is why, instead of creating a Compare(const Mesh& other) method in the <code>Mesh</code> class itself and pass the pointer to this function whenever it is needed,
		/// it is better to create a <code>MeshComparator</code> object and pass its temporary instance. The reason is that function-objects are almost always inlined by the compiler.
		/// See https://en.wikibooks.org/wiki/Optimizing_C%2B%2B/Writing_efficient_code/Performance_improving_features#Function-objects.
		/// </remarks>
		class MeshComparator
		{
		public:
			bool operator() (const Mesh& lhs, const Mesh& rhs) const
			{
				CHECK_CONDITION_RETURN_RENDERING(lhs.m_meshData != nullptr && rhs.m_meshData != nullptr, false, Utility::Logging::ERR,
					"Cannot compare two meshes' VAOs, because mesh(-es) data is/are NULL.");
				return lhs.m_meshData->GetVAO() < rhs.m_meshData->GetVAO();
			}
		}; // end class MeshComparator

		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Mesh constructor. </summary>
		/// <param name="fileName"> The file name storing the mesh data. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		explicit Mesh(const std::string& fileName, GLenum mode = GL_TRIANGLES);
		/// <summary> Mesh constructor. </summary>
		/// <param name="indices"> The array of indices in the mesh. </param>
		/// <param name="indicesCount"> The number of indices in the <paramref name="indices"/> array. </param>
		/// <param name="verticesCount">
		/// The number of vertices in each non-<code>nullptr</code> arrays: <paramref name="positions"/>,
		/// <paramref name="textureCoordinates"/>, <paramref name="normals"/>, <paramref name="tangents"/> and <paramref name="bitangents"/>.
		/// </param>
		/// <param name="positions"> The array of 3D positions in the mesh. The array must contain <paramref name="verticesCount/> elements.</param>
		/// <param name="textureCoordinates"> The array of 2D texture coordinates in the mesh. The array must contain <paramref name="verticesCount/> elements.</param>
		/// <param name="normals"> The array of 3D normal vectors in the mesh. The array must contain <paramref name="verticesCount/> elements.</param>
		/// <param name="tangents"> The array of 3D tanget vectors in the mesh. The array must contain <paramref name="verticesCount/> elements.</param>
		/// <param name="bitangents"> The array of 3D bitangent vectors in the mesh. The array must contain <paramref name="verticesCount/> elements.</param>
		/// <param name="calcNormalsEnabled"> <code>True</code> if normal vectors should be calculated and <code>false</code> otherwise. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		Mesh(int* indices, int indicesCount, int verticesCount, Math::Vector3D* positions, Math::Vector2D* textureCoordinates = nullptr, Math::Vector3D* normals = nullptr, Math::Vector3D* tangents = nullptr, Math::Vector3D* bitangents = nullptr, bool calcNormalsEnabled = false, GLenum mode = GL_TRIANGLES);
		/// <summary> Mesh constructor. </summary>
		/// <param name="positions"> The array of 2D screen positions in the mesh. </param>
		/// <param name="textureCoordinates"> The array of 2D texture coordinates in the mesh. </param>
		/// <param name="verticesCount"> The number of positions in the <paramref name="positions"/> array. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		Mesh(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, unsigned int verticesCount, GLenum mode = GL_TRIANGLE_STRIP);
		/// <summary> Mesh destructor. </summary>
		virtual ~Mesh(void);
		/// <summary> Mesh copy constructor. </summary>
		Mesh(const Mesh& Mesh) = delete;
		/// <summary> Mesh move constructor. </summary>
		Mesh(Mesh&& mesh);
		/// <summary> Mesh copy assignment operator. </summary>
		Mesh& operator=(const Mesh& mesh) = delete;
		/// <summary> Mesh move assignment operator. </summary>
		Mesh& operator=(Mesh&& mesh) = delete;
	protected:
		/// <summary> Mesh constructor. </summary>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		explicit Mesh(GLenum mode = GL_TRIANGLES);
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Draw() const;
	protected:
		//void FillBuffer(MeshBufferTypes::MeshBufferType buffer, MeshAttributeLocations::MeshAttributeLocation attributeLocation, int* data, unsigned int dataCount);
		void FillBuffer(MeshBufferTypes::MeshBufferType buffer, MeshAttributeLocations::MeshAttributeLocation attributeLocation, Math::Real* data, unsigned int dataCount);

		void AddVertices(Math::Vector2D* positions, Math::Vector2D* textureCoordinates, int verticesCount);
		void AddVertices(Math::Vector3D* positions, Math::Vector2D* textureCoordinates, Math::Vector3D* normals, Math::Vector3D* tangents, Math::Vector3D* bitangents, int verticesCount, int* indices, int indicesCount, bool calcNormalsEnabled);
		void CalcNormals(Math::Vector3D*& normals, Math::Vector3D* positions, size_t verticesCount, const int* indices, size_t indicesCount) const;
		void CalcTangents(Math::Vector3D*& tangents, Math::Vector3D* positions, Math::Vector2D* textureCoordinates, size_t verticesCount) const;
		//void CalcIndices(Vertex* vertices, size_t verticesCount, std::vector<Vertex>& indexedVertices, std::vector<int>& indices) const;
		//bool GetSimilarVertexIndex(const Vertex& vertex, const std::vector<Vertex>& indexedVertices, int& index) const;
		virtual void SavePositions(const std::vector<Math::Vector3D>& positions) { /* does nothing*/ }; // TODO: Remove this function
	/* ==================== Non-static member functions end ==================== */


	/* ==================== Non-static member variables begin ==================== */
	protected:
#ifdef STORE_MESH_FILE_NAME
		std::string m_fileName;
#endif
		GLenum m_mode;
		std::shared_ptr<MeshData> m_meshData;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class Mesh */

	/// <summary>
	/// The billboards mesh. It holds a mesh to be used by a specific number of billboards at the same time.
	/// </summary>
	class BillboardMesh : public Mesh
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Billboards mesh constructor. </summary>
		/// <param name="modelMatricesValues"> The array of model matrices for each billboard. </param>
		/// <param name="billboardsCount"> The number of billboards that will use this billboards mesh. </param>
		/// <param name="billboardDataLength"> The data length for a single billboard in the <paramref name="modelMatricesValues"/>. </param>
		BillboardMesh(Math::Real* modelMatricesValues, unsigned int billboardsCount, unsigned int billboardDataLength);
		/// <summary> Billboards mesh destructor. </summary>
		virtual ~BillboardMesh(void);
		/// <summary> Billboards mesh copy constructor. </summary>
		BillboardMesh(const BillboardMesh& billboardMesh) = delete;
		/// <summary> Billboards mesh move constructor. </summary>
		BillboardMesh(BillboardMesh&& billboardMesh);
		/// <summary> Billboards mesh copy assignment operator. </summary>
		BillboardMesh& operator=(const BillboardMesh& billboardMesh) = delete;
		/// <summary> Billboards mesh move assignment operator. </summary>
		BillboardMesh& operator=(BillboardMesh&& billboardMesh) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Draw() const override;
		/* ==================== Non-static member functions end ==================== */


		/* ==================== Non-static member variables begin ==================== */
	private:
		unsigned int m_billboardsCount;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class BillboardMesh */

	/// <summary>
	/// The instance mesh that is used by particles.
	/// </summary>
	class InstanceMesh : public Mesh
	{
		/* ==================== Non-static member functions begin ==================== */
	private:
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary>
		/// Instance mesh constructor.
		/// </summary>
		InstanceMesh(Math::Vector2D* positions, unsigned int positionsCount, unsigned int maxParticlesCount, unsigned int instanceDataLength);
		/// <summary>
		/// Instance mesh destructor.
		/// </summary>
		virtual ~InstanceMesh(void);
		/// <summary>
		/// Instance mesh copy constructor.
		/// </summary>
		InstanceMesh(const InstanceMesh& instanceMesh) = delete;
		/// <summary>
		/// Instance mesh move constructor.
		/// </summary>
		InstanceMesh(InstanceMesh&& instanceMesh);
		/// <summary>
		/// Instance mesh copy assignment operator.
		/// </summary>
		InstanceMesh& operator=(const InstanceMesh& instanceMesh) = delete;
		/// <summary>
		/// Instance mesh move assignment operator.
		/// </summary>
		InstanceMesh& operator=(InstanceMesh&& instanceMesh) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		/// <summary> Gives information about the amount of data one instance uses. </summary>
		/// <returns> The amount of data used by every instance. </returns>
		unsigned int GetInstanceDataLength() const { return m_instanceDataLength; }
		void Draw(Math::Real* data, unsigned int dataSize, unsigned int particlesCount) const;
		/* ==================== Non-static member functions end ==================== */

		/* ==================== Non-static member variables begin ==================== */
	private:
		unsigned int m_positionsCount;
		unsigned int m_maxParticlesCount;
		unsigned int m_instanceDataLength;
		/* ==================== Non-static member variables end ==================== */
	}; /* end class InstanceMesh */

	class TerrainMesh : public Mesh
	{
		/* ==================== Static variables begin ==================== */
	private:
		/// <summary> The size of the single squared-size terrain tile. </summary>
		static const int SIZE;
		/// <summary>
		/// The maximal height of the terrain. The negative value represents the minimal height of the terrain.
		/// In other words, the terrain height always lies in range [-HEIGHTMAP_MAX_HEIGHT; HEIGHTMAP_MAX_HEIGHT].
		/// </summary>
		static const float HEIGHTMAP_MAX_HEIGHT;
		/// <summary> The maximum value for color of the single pixel in the height map. </summary>
		static const float MAX_PIXEL_COLOR;
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Terrain mesh constructor. </summary>
		/// <param name="fileName"> The file storing the model for the terrain mesh. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		explicit TerrainMesh(const std::string& fileName, GLenum mode = GL_TRIANGLES);
		/// <summary> Terrain mesh constructor. </summary>
		/// <param name="gridX"> The X position on the grid. </param>
		/// <param name="gridZ"> The Z position on the grid. </param>
		/// <param name="heightMapFileName">
		/// The height map file name. The height map is a grey-scale texture where the brighter the color the higher the respective point on the terrain will be.
		/// </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		TerrainMesh(int gridX, int gridZ, const std::string& heightMapFileName, GLenum mode = GL_TRIANGLES);
		/// <summary> Terrain mesh constructor. </summary>
		/// <param name="gridX"> The X position on the grid. </param>
		/// <param name="gridZ"> The Z position on the grid. </param>
		/// <param name="heightsGenerator"> The heights generator. </param>
		/// <param name="vertexCount"> The number of vertices for the terrain. </param>
		/// <param name="mode"> The mode in which the mesh will be stored. </param>
		TerrainMesh(int gridX, int gridZ, const Math::HeightsGenerator& heightsGenerator, int vertexCount, GLenum mode = GL_TRIANGLES);
		/// <summary> Terrain mesh destructor. </summary>
		virtual ~TerrainMesh(void);
		/// <summary> Terrain mesh copy constructor. </summary>
		TerrainMesh(const TerrainMesh& terrainMesh) = delete;
		/// <summary> Terrain mesh move constructor. </summary>
		TerrainMesh(TerrainMesh&& terrainMesh) = delete;
		/// <summary> Terrain mesh copy assignment operator. </summary>
		TerrainMesh& operator=(const TerrainMesh& terrainMesh) = delete;
		/// <summary> Terrain mesh copy assignment operator. </summary>
		TerrainMesh& operator=(TerrainMesh&& terrainMesh) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		RENDERING_API Math::Real GetHeightAt(const Math::Vector2D& xz) const { return GetHeightAt(xz.GetX(), xz.GetY()); }
		RENDERING_API Math::Real GetHeightAt(Math::Real x, Math::Real y) const;
		RENDERING_API void TransformPositions(const Math::Matrix4D& transformationMatrix);
	protected:
		virtual void SavePositions(const std::vector<Math::Vector3D>& positions) override;
	private:
		int GetHeightMapIndex(int x, int z) const;
		Math::Real CalculateHeightAt(int x, int z, unsigned char* heightMapData);
		Math::Real CalculateHeightAt(int x, int z, const Math::HeightsGenerator& heightsGenerator);
		Math::Vector3D CalculateNormal(int x, int z, unsigned char* heightMapData);
		Math::Vector3D CalculateNormal(int x, int z, const Math::HeightsGenerator& heightsGenerator);
		/* ==================== Non-static member functions end ==================== */


		/* ==================== Non-static member variables begin ==================== */
	private:
		/// <summary> The position of the single terrain tile along the X axis. </summary>
		int m_x;
		/// <summary> The position of the single terrain tile along the Z axis. </summary>
		int m_z;

		int m_vertexCount;
#ifdef HEIGHTS_KD_TREE
		std::vector<Math::Vector3D> m_positions;
		const unsigned int m_kdTreeSamples;
		std::unique_ptr<Math::KDTree> m_kdTree;
#elif defined HEIGHTS_HEIGHTMAP
		int m_heightMapWidth, m_heightMapHeight;
		std::vector<Math::Real> m_heights;
		Math::Real m_gridSquareSize;
#endif
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TerrainMesh */


	// TODO: TextMesh could probably be removed. Its functionality should go directly into the Mesh class.
	/// <summary>
	/// The text mesh that is going to be rendered on the screen.
	/// </summary>
	class TextMesh : public Mesh
	{
		/* ==================== Static variables begin ==================== */
		/* ==================== Static variables end ==================== */

		/* ==================== Constructors and destructors begin ==================== */
	public:
		/// <summary> Text mesh constructor. </summary>
		TextMesh(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount, GLenum mode = GL_TRIANGLES);
		/// <summary> Text mesh destructor. </summary>
		virtual ~TextMesh(void);
		/// <summary> Text mesh copy constructor. </summary>
		TextMesh(const TextMesh& textMesh) = delete;
		/// <summary> Text mesh move constructor. </summary>
		TextMesh(TextMesh&& textMesh);
		/// <summary> Text mesh copy assignment operator. </summary>
		TextMesh& operator=(const TextMesh& textMesh) = delete;
		/// <summary> Text mesh move assignment operator. </summary>
		TextMesh& operator=(TextMesh&& textMesh) = delete;
		/* ==================== Constructors and destructors end ==================== */

		/* ==================== Non-static member functions begin ==================== */
	public:
		virtual void Draw() const override;
		void ReplaceData(Math::Vector2D* screenPositions, Math::Vector2D* textureCoordinates, int verticesCount);
		/* ==================== Non-static member functions end ==================== */


		/* ==================== Non-static member variables begin ==================== */
	private:
		/* ==================== Non-static member variables end ==================== */
	}; /* end class TextMesh */

} /* end namespace Rendering */

#endif // __RENDERING_MESH_H__