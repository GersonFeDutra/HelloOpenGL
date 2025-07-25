#include "TestTexture2D.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"


namespace test {
	Texture2D::Texture2D(GLFWwindow* window, ImGuiIO& io) : Test{ window, io }
	{
		// Carrega a textura que será usada na construção do viewport e que será impressa
		texture = std::make_unique<Texture>("res/textures/kintaro_oe.png");

		int left, top, right, bottom;
		glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);
		//   left   = largura da borda esquerda
		//   right  = largura da borda direita
		//   top    = altura da barra de título + borda superior
		//   bottom = altura da borda inferior

		// Let's change the size using the image resolution
		Vector2_int window_size = { texture->getWidth(), texture->getHeight() };
		Vector2 texture_size = window_size; // Save the coords in floats
		Vector2 texture_center = texture_size / 2.0f;

		// Reset window size considering the image resolution and the system's decorations
		window_size.x += left + right;
		window_size.y += top + bottom;
		std::cout << window_size.x << ", l:" << left << ", r:" << right << '\n';
		glfwSetWindowSize(window, window_size.x, window_size.y);

		Quadrilateral quadrilateral = {
			.vertexes = {
				{.position = { -texture_center.x, -texture_center.y }, .textureCoords = { 0.0f, 0.0f } }, // 0
				{.position = {  texture_center.x, -texture_center.y }, .textureCoords = { 1.0f, 0.0f } }, // 1
				{.position = {  texture_center.x,  texture_center.y }, .textureCoords = { 1.0f, 1.0f } }, // 2
				{.position = { -texture_center.x,  texture_center.y }, .textureCoords = { 0.0f, 1.0f } }, // 3
			},
			.indices = {
				0, 1, 2,
				2, 3, 0,
			},
		};

		// Supply the Graphics Card with data
		vertexBuffer = std::make_unique<VertexBuffer>(quadrilateral.vertexes, sizeof(quadrilateral.vertexes));

		/* Render Setup Stage */

		// Atualiza o viewport para o novo framebuffer
		int fb_width, fb_height;
		glfwGetFramebufferSize(window, &fb_width, &fb_height);
		glViewport(0, 0, fb_width, fb_height);

		// redicionamentos interativos
		// glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		//glViewport(0, 0, width, height);
		//});

		VertexBufferLayout layout;
		layout.push<float>(sizeof(Vector2) / sizeof(float));
		layout.push<float>(sizeof(Vector2) / sizeof(float));

		vertexArray = std::make_unique<VertexArray>();
		vertexArray->addBuffer(*vertexBuffer, layout);

		// Setup the Index Buffer Object
		indexBuffer = std::make_unique<IndexBuffer>(quadrilateral.indices, sizeof(quadrilateral.indices) / sizeof(decltype(*quadrilateral.indices)));

		texture->bind();
		shader = std::make_unique<Shader>("res/shaders/Test.shader");
		shader->bind();

		// cria uma projeção hortogonal para consertar o aspect ratio do plano 2D
		//glm::mat4 proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, -1.0f, 1.0f);

		// cria uma projeção hortogonal para mapear as coordenadas equivalentes ao tamanho da 
		Matrix4 proj = glm::ortho(0.0f, texture_size.x, 0.0f, texture_size.y, -1.0f, 1.0f);
		// matriz de transformação que simula o "olho" da câmera
		//Matrix4 view = glm::translate(Matrix4(1.0f), Vector3(-100, 0, 0)); // offset

		Vector3 translation = { texture_center.x, texture_center.y, 0.0f };
		// transformação de um modelo na cena, nesse caso a imagem
		Matrix4 view{ 1.0f };
		Matrix4 model = glm::translate(Matrix4(1.0f), translation);
		mvp = proj * view * model; // Model View Projection Matrix

		shader->setUniform1i("u_Texture", 0);

		// unbound all vertex arrays, buffers and programs
		vertexArray->unbind();
		vertexBuffer->unbind();
		indexBuffer->unbind();
		shader->unbind();
	}

	Texture2D::~Texture2D()
	{
	}

	void Texture2D::onUpdate(float deltaTime)
	{
	}

	void Texture2D::onRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT)) ;

		/* Bind all data to be used */
		shader->bind();
		{
			Renderer renderer{};
			shader->setUniformMat4f("u_MVP", mvp);
			renderer.draw(*vertexArray, *indexBuffer, *shader);
		}
	}
} // namespace test