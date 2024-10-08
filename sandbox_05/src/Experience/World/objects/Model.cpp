#include "Model.h"



Model::Model() :
	log("Model.log"),
	angle(0.0f),
	time(0.0f),
	pos(true),
	localShaderPaths(
		{
			{GL_VERTEX_SHADER, "src/shader/morphing/morphing.vert"},
			{GL_FRAGMENT_SHADER,"src/shader/morphing/morphing.frag"}

		}
	)
{
	// Code
	loadModel("static/models/model.glb");

	setModelMatrix();
}

Model::~Model()
{
	uninitializer(); // Ensure resources are cleaned up
}

void Model::renderGUI()
{

	// ImGui Frame preparation
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::DockSpaceOverViewport();

	// Other ImGui elements
	ImGui::Begin("Properties");

	ImGui::End();

}

void Model::update()
{
	//float deltaTime = timer.getElapsedTime();
	//time += deltaTime * 0.1f;
	// Update time
		if (!waiting) {
			if (pos && time < 1.0f) {
				time += 0.001f;
				if (time >= 1.0f) {
					time = 1.0f;
					pos = false;  // Switch direction
					waiting = true;  // Start waiting
					waitCounter = 0;  // Reset wait counter
				}
			}
			else if (!pos && time > 0.0f) {
				time -= 0.001f;
				if (time <= 0.0f) {
					time = 0.0f;
					pos = true;   // Switch direction
					waiting = true;  // Start waiting
					waitCounter = 0;  // Reset wait counter
				}
			}
		}
		else {
			// Increase wait counter until the wait limit is reached
			waitCounter++;
			if (waitCounter >= WAIT_LIMIT) {
				waiting = false;  // Stop waiting after the counter reaches the limit
			}
		}

	//timer.reset();
	
	angle += 0.0001f;
	updateModelMatrix();
}


void Model::setModelMatrix()
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -6.0f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	log.write("1: modelmatrix");
}


GLuint Model::initializeShaders()
{
	// Code

	Shader shader;

	// Initialize shader with the paths from localShaderPaths

	shader.initialize(this, localShaderPaths);
	shaderProgramObject = shader.getShaderProgram(shader.shaderName);
	log.debug("initializeShaders :shaderProgramObject", shaderProgramObject);

	return shaderProgramObject;
}


GLuint Model::getShaderProgramObject() const
{
	return shaderProgramObject;
}

glm::mat4 Model::getModelMatrix() const
{
	return modelMatrix;
}

void Model::init() {

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	if (!meshVertexPositions.empty()) {
		// Access the first MeshData
		const MeshData& firstMeshData = meshVertexPositions.at(2); // Use .at(0) to safely access the first element
		std::string meshName = firstMeshData.name;
		std::vector<glm::vec3> vertexPositions = firstMeshData.vertexPositions;
		indices = firstMeshData.indices;
		int i = 0;
		for (const auto& meshData : meshVertexPositions) {

			std::string meshName = meshData.name;
			std::vector<glm::vec3> vertexPositions = meshData.vertexPositions;
			std::vector<unsigned int> indices = meshData.indices;
			log.debug("First Mesh Name:", meshName);
			log.debug("Vertex Count:", vertexPositions.size());
			log.debug("Indices Count:", indices.size());
			log.write("in init");

	
		//Bind with vertex array object and start recording buffer and buffer data related steps.
		glGenBuffers(1, &VBO_POSITION[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_POSITION[i]);
		glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(glm::vec3), vertexPositions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);
		glEnableVertexAttribArray(i);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		i++;
		}

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}


void Model::bindAttributeLocationsAndLink(GLuint shaderProgramObject) {
	glBindAttribLocation(shaderProgramObject, 0, "a_position");
	glBindAttribLocation(shaderProgramObject, 1, "a_positionTarget");
	glBindAttribLocation(shaderProgramObject, 2, "a_positionSpare");
	glBindAttribLocation(shaderProgramObject, 7, "a_color");
	glLinkProgram(shaderProgramObject);

	// Post linking Uniforms
	uTime = glGetUniformLocation(shaderProgramObject, "uTime");
	colorA = glGetUniformLocation(shaderProgramObject, "colorA");
	colorB = glGetUniformLocation(shaderProgramObject, "colorB");
}

void Model::updateModelMatrix() {
	modelMatrix = glm::mat4(1.0); // Reset to identity matrix
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -4.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Model::render()
{
	//uniform
	glUniform1f(uTime, time);
	glUniform3fv(colorA, 1, glm::value_ptr(colora));
	glUniform3fv(colorB, 1, glm::value_ptr(colorb));

	//code
	//Bind with vertex array object
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_POINTS, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}


void Model::loadModel(std::string const& path) {
	Logger logger("LoadModel.log");
	logger.write("logger");

	// Read file via ASSIMP
	glm::mat4 identityMatrix = glm::mat4(1.0f); // Identity matrix
	logger.debug("path", path);
	Assimp::Importer importer;
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		logger.debug("Assimp error", std::string(importer.GetErrorString()));
		return;
	}

	logger.debug("Scene loaded successfully with", scene->mNumMeshes);

	// Iterate over all the meshes in the scene
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		// Extract mesh name (if available) or use index as the name
		std::string meshName = mesh->mName.C_Str();
		if (meshName.empty()) {
			meshName = "Mesh_" + std::to_string(i);
		}

		// Create a MeshData struct to hold vertex positions and indices
		MeshData meshData;
		meshData.name = meshName;

		// Vector to store the vertex positions for this specific mesh
		std::vector<glm::vec3> vertexPositions;
		// Vector to store the indices for this specific mesh
		std::vector<unsigned int> indices;

		// Iterate over the vertices of the mesh
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			aiVector3D vertex = mesh->mVertices[j];
			glm::vec3 position(vertex.x, vertex.y, vertex.z);
			vertexPositions.push_back(position);
		}

		// Iterate over the faces of the mesh to get indices
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
				indices.push_back(face.mIndices[k]); // Store each index
			}
		}

		// Store vertex positions and indices in the meshData
		meshData.vertexPositions = vertexPositions;
		meshData.indices = indices;

		// Add the meshData to the meshVertexPositions vector
		meshVertexPositions.push_back(meshData);

		// Log mesh name and vertex positions
		logger.write("Mesh: " + meshName);
		for (const auto& vertex : vertexPositions) {
			std::ostringstream oss;
			oss << "Position: (" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")";
			logger.write(oss.str());
		}

		// Log indices
		logger.write("Indices: ");
		for (const auto& index : indices) {
			logger.write(std::to_string(index));
		}

		logger.write(""); // Print a newline after each mesh
	}

	// Logging the result
	for (const auto& meshData : meshVertexPositions) {
		// Log the mesh name and the number of vertices
		logger.debug("Mesh:", meshData.name);
		logger.debug(" has", meshData.vertexPositions.size());
		logger.debug(" with indices count:", meshData.indices.size());
	}
}
void Model::uninitializer() {

	if (VBO_POSITION[0]) {
		glDeleteBuffers(1, &VBO_POSITION[0]);
		VBO_POSITION[0] = 0;
	}
	if (VBO_COLOR) {
		glDeleteBuffers(1, &VBO_COLOR);
		VBO_COLOR = 0;
	}
	if (VAO) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

