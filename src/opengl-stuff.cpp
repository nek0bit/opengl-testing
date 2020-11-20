#include "opengl-stuff.hpp"

float vel_rx = 0;
float vel_ry = 0;

int view_width = 0;
int view_height = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	view_width = width;
	view_height = height;
	glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		std::cout << "Escape pressed..." << std::endl;
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		vel_rx += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		vel_rx -= 1;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		vel_ry += 1;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		vel_ry -= 1;
	}
}

int main()
{
	std::string dir = "../../../";
	constexpr int FPS = 120;
	constexpr int frame_del = 1000 / FPS;
	view_width = 800;
	view_height = 600;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(view_width, view_height, "Cool program", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Error: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Error: Could not initialize GLAD" << std::endl;
		return -2;
	}

	glEnable(GL_DEPTH_TEST);

	framebuffer_size_callback(window, view_width, view_height);

	// Update on resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::ifstream vertShader_f, fragShader_f;
	vertShader_f.open(dir + "shaders/vert_shade.glsl");
	fragShader_f.open(dir + "shaders/frag_shade.glsl");

	std::stringstream vertShader_strst, fragShader_strst;
	vertShader_strst << vertShader_f.rdbuf();
	fragShader_strst << fragShader_f.rdbuf();

	std::string vertShader_str = vertShader_strst.str(), fragShader_str = fragShader_strst.str();

	const char* vertShader = vertShader_str.c_str();
	const char* fragShader = fragShader_str.c_str();

	//********************
	// Shaders
	//********************
	unsigned int shaderProgram = glCreateProgram();
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	// Compile shaders
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glShaderSource(fragmentShader, 1, &fragShader, NULL);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	// Check if shaders compilation succeeded
	int success;

	std::vector<unsigned int> grouping = { vertexShader, fragmentShader };
	std::for_each(grouping.begin(), grouping.end(), [&](unsigned int& id) {
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char info[256];
			glGetShaderInfoLog(id, 256, NULL, info);
			std::cout << "Shader compilation failed: " << info << std::endl;
		}
		else {
			glAttachShader(shaderProgram, id);
		}
	});

	// Link shaders
	glLinkProgram(shaderProgram);

	// Check if linking succeeded
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char info[256];
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "Shader program linking failed: " << info << std::endl;
	}


	// We don't need the shader objects anymore
	std::for_each(grouping.begin(), grouping.end(), [&](unsigned int& id) {
		glDeleteShader(id);
	});

	//********************
	// Game
	//********************

	// Create VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);

	// Create VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// Create vertex for our cube-y friend
	std::vector<float> vert = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	int amount = vert.size() / 5;


	glBindVertexArray(VAO);
	// Copy vertices array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Note: &vert[0] just gets the raw array from the vector
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(&vert[0]), &vert[0], GL_DYNAMIC_DRAW);
	// attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture position
	glEnableVertexAttribArray(1);

	// EBO (for reference)
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	// Loading an an image to the texture
	std::string image_str = dir + "data/sample.jpg";

	// Create texture and load image data into texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char* data = stbi_load(image_str.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Warning: Failed to load texture at " << image_str << std::endl;
	}

	stbi_image_free(data);

	// Transformations
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");

	double smooth_rx = 0;
	
	
	// Loop
	while (!glfwWindowShouldClose(window))
	{
		auto time_before = std::chrono::system_clock::now();

		// Begin
		handleInput(window);

		glClearColor(0.25f, 0.10f, 0.40f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Define some objects
		std::vector<glm::vec3> objects = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 1.0f, -5.0f),
			glm::vec3(-2.0f, -0.3f, 2.0f),
		};

		// Smooth values
		smooth_rx = smooth_rx * 0.80;
		smooth_rx += vel_rx;

		// Transformations and math
		glm::mat4 projection{ 1.0f };
		projection = glm::perspective(glm::radians(45.0f), (float)view_width / (float)view_height, 0.1f, 100.0f);
		//projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

		// - Setup view
		glm::mat4 view{ 1.0f };
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		
		// Setup shader for this frame
		glUseProgram(shaderProgram);

		// Do transformations on objects
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw
		// - bind textures
		glBindTexture(GL_TEXTURE_2D, texture);
		// - rebind to vao
		glBindVertexArray(VAO);
		// - draw
		for (size_t i = 0; i < objects.size(); ++i) {
			glm::mat4 model{ 1.0f }; // Transition for this object
			model = glm::translate(model, objects[i]);
			model = glm::rotate(model, glm::radians((30.0f*i) + static_cast<float>(smooth_rx)), glm::vec3(1.0, 1.0, 1.0));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, amount);
		}
		
		// - rebind to 0
		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();
		// End
		
		auto time_after = std::chrono::system_clock::now();

		auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(time_after - time_before);

		std::this_thread::sleep_for(std::chrono::milliseconds{ frame_del - time_taken.count() });
	}

	return 0;
}
