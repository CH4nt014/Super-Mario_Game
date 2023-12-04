
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrKlang/irrKlang.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include <thread>
#include <chrono>
#include "shader_s.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "skyBox.h"
#include "mesh.h"
#include "texture.h"
#include "model.h"
#include "player.h"
#include "fixed_camera.h"
#include "Terrain.h"
#include "mainmenu.h"
#include "pgSelect.h"
#include "effects.h"
#include "billboard.h"

#pragma comment(lib, "irrKlang.lib")

using namespace glm;
using namespace std;

// settings
const unsigned int SCR_WIDTH=1920;
const unsigned int SCR_HEIGHT=1080;

const unsigned int SHADOW_WIDTH = 1024 * 10;
const unsigned int SHADOW_HEIGHT = 1024 * 10;

vec3 WORLD_UP(0.0f, 1.0f, 0.0f);

// timing
double startTime;
double currentTime;
double deltaTime = 0.0f;
double previousTime = 0.0f;
int tempo_rimanente = 10000;
float currentFrame = 0;
float tempoTrascorso = 0;
float lastFrame = 0;
int fps = 0;
int frames = 0;

Player player(vec3(-40, 1.0, -10));

//Pointlight
vec3 lightPosition(0.0, -5.0, 27); //Posizione pointlight

//Camera
vec3 pos(-40.0, 25.0, -10.0);	// Posizione camera
vec3 at(0.0, 0.0, 10.0);		// Punto in cui "guarda" la camera
vec3 up(0.0, 1.0, 0.0);		// Vettore up...la camera è sempre parallela al piano

vec3 dir(0.0, 0.0, -0.1);	// Direzione dello sguardo
vec3 side(1.0, 0.0, 0.0);	// Direzione spostamento laterale
vec3 upM = vec3(0.0, 1.0, 0.0);	// Direzione spostamento UpPos = pos + 0.1f * up;

vec3 posVecchia(-10.0, 1.43, 80.0);

Camera camera(posVecchia);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


//////TEXTURE
//Skybox
unsigned int cubeMapTexture;
const vector<string> faces{
  "asset/image/skybox/right.jpg",
  "asset/image/skybox/left.jpg",
  "asset/image/skybox/top.jpg",
  "asset/image/skybox/bottom.jpg",
  "asset/image/skybox/front.jpg",
  "asset/image/skybox/back.jpg"
};

//BillBoard
unsigned int pgbb;
unsigned int mariobb;
unsigned int luigibb;
unsigned int yoshibb;
unsigned int vitebb;
unsigned int separatorbb;
unsigned int numbb;
unsigned int num1bb;
unsigned int num2bb;
unsigned int num3bb;
unsigned int num4bb;
unsigned int num5bb;
unsigned int num6bb;
unsigned int num7bb;
unsigned int num8bb;
unsigned int num9bb;
unsigned int num0bb;
unsigned int monetebb;
unsigned int obiettivobb;
unsigned int fumetto_toad;
unsigned int fumetto_peach;
unsigned int fumetto_peach_star;

//Menu
unsigned int menutexture;
unsigned int menuSelectiontexture1;
unsigned int menuSelectiontexture2;
unsigned int menuSelectiontexture3;
unsigned int menuSelectiontexture4;
unsigned int creditMenutexture;
unsigned int levelMenutexture1;
unsigned int levelMenutexture2;
unsigned int levelMenutexture3;
unsigned int pgMenutexture1;
unsigned int pgMenutexture2;
unsigned int pgMenutexture3;
unsigned int bgtexture;
unsigned int bgtexture2;
unsigned int bgtexture3;
unsigned int gameOvertexture;
unsigned int wintexture;

string pg;
string pg_hurt;

//depth map
unsigned int depthMap;
unsigned int depthMapFBO;

//Vettori e indici
vector<float> vettoriSkyBox;
vector<unsigned int> indiciSkyBox;

vector<Model*> Personaggio;
vector<Model*> Mario;
vector<Model*> Luigi;
vector<Model*> Yoshi;

Billboard bBoard;

Shader* skyBoxShader;
Shader* depthShader;
Shader* simpleShader;
Shader* lightShader;
Shader* pgShader;
Shader* bBoardShader;
Shader* particleShader;

//proprietà illuminazione
vec3 lightPos(-1.0f, 1.0f, -1.0f);
vec3 lightCol(1.0f, 1.0f, 1.0f);
vec3 lightDirection = normalize(lightPos);
mat4 lightSpaceMatrix;

//VAO
unsigned int menuVBO, menuVAO, menuEBO;
unsigned int bgVBO, bgVAO, bgEBO;
unsigned int bgVBO2, bgVAO2, bgEBO2;
unsigned int bgVBO3, bgVAO3, bgEBO3;
unsigned int skyBoxVBO, skyBoxVAO, skyBoxEBO;
unsigned int pgVBO, pgVAO;

//-MENU-----------
int menuSelectionShaderProgram;
int menuSelectionShaderProgram2;
int menuSelectionShaderProgram3;
int menuSelectionShaderProgram4;
int helpMenuShaderProgram;
int creditMenuShaderProgram;
int levelMenuShaderProgram1;
int levelMenuShaderProgram2;
int levelMenuShaderProgram3;
int pgMenuShaderProgram1;
int pgMenuShaderProgram2;
int pgMenuShaderProgram3;
int menushaderProgram;
int bgshaderProgram;
int bgshaderProgram2;
int bgshaderProgram3;
int gameOverShaderProgram;
int winShaderProgram;

// Status game setting
bool top = true;
bool topP = true;
bool topA = true;
bool topS = true;
bool inizio = true;
bool star = true;
bool start = false;
bool posPg;
bool animazione = true;
bool schiacciato = false;
bool toad_sound = true;
bool peach_sound = true;
bool peach_sound2 = true;
bool colpito1 = true;
bool colpito2 = true;
bool colpito3 = true;
bool colpito4 = true;
bool colpito5 = true;
bool colpito6 = true;
bool colpito7 = true;
bool win = false;
bool hurt = false;
float altezza = 4;
float altezzaP = 4;
float zeta = 98;
float rotazione = 0;
float altezzaTubo = -2;
float altezzaS = -5;
float muove = 0;
float angoloG = 0;
float angoloG2 = 0;
float angoloP = 0;
float angoloB = 0;
float cerchioTerraX;
float cerchioTerraX2;
float cerchioTerraZ;
float cerchioTerraZ2;
float booZ;
float bulletX = -61.4986;
float bulletZ = -34.618;
float cerchioAriaX;
float cerchioAriaY;
float cerchioSinX;
float cerchioSinY;
float cerchioSinZ;
float velocita = 1;
int pgindex = 0;
int menuindex = 0;
int levelindex = 0;
int gamestatus = 0;
int oldgamestatus = -1;
int obiettivi = 10;
int monete = 0;
int vite = 2;
int numAlberi = 42;
int animation = 1;

vec3 grandezza;
vec3 grandezzaM = vec3(1.f, 1.f, 1.f);
vec3 grandezzaL = vec3(0.7f, 0.7f, 0.7f);
vec3 grandezzaY = vec3(0.5f, 0.5f, 0.5f);

vec3 posToad = vec3(0.0, 1.0, -16.8);
vec3 posPeach = vec3(-96.87, 4.21, -136.95);
vec3 posThwomp = vec3(-94.37, 3.78, 22.46);
vec3 posBoo = vec3(-43.40, 11.92, 93.61);
vec3 posWhomp = vec3(21.7381, 1.0, -13.0);
vec3 posPuku = vec3(-90.31, 6.82, -91.16);
vec3 posTubo = vec3(0.282f, 1.0f, 2.383f);
vec3 posGoomba1;
vec3 posGoomba2;
vec3 posBoo2;
vec3 posUrchin;
vec3 posSpiny;
vec3 posStar = vec3(-104.172, 6.32398, -134.487);

vec3 posCoin[20]{
	vec3(-129.18, 3.27, -84.88),
	vec3(20.91, 2, -9.416),
	vec3(-21.41, 2.5, -9.13),
	vec3(-86.46, 2.71, -22.61),
	vec3(-94.37, 3.78, 22.46),
	vec3(-90.81, 3.09, 83.13),
	vec3(-32.71, 2.74, 142.52),
	vec3(98.89, 7.72, -4.47),
	vec3(-157.178,  10.46,  108.567),
	vec3(-205.634,  8.2331,  81.8917),
	vec3(-207.078,  21.51, 8.21875),
	vec3(-229.72,  7.18485,-72.6469),
	vec3(-165.889,  3.318,  -199.328),
	vec3(-82.6373,  8.5996,  -199.803),
	vec3(-0.841358,  9.05948,  -174.974),
	vec3(62.1103, 4.26467,  -114.65),
	vec3(12.0577,  18.8928,  -71.8255), //
	vec3(48.9733,  11.2045,  86.9428),
	vec3(-21.2466, 23.6972, 52.6158),
	vec3(-44.3989, 10.7834, 106.232)
};

vec3 posLightning[20]{
	vec3(-129.18, 3.27, -84.88),
	vec3(20.91, 2, -9.416),
	vec3(-21.41, 2.5, -9.13),
	vec3(-86.46, 2.71, -22.61),
	vec3(-94.37, 3.78, 22.46),
	vec3(-90.81, 3.09, 83.13),
	vec3(-32.71, 2.74, 142.52),
	vec3(98.89, 7.72, -4.47),
	vec3(-157.178,  10.46,  108.567),
	vec3(-205.634,  8.2331,  81.8917),
	vec3(-207.078,  21.51, 8.21875),
	vec3(-229.72,  7.18485,-72.6469),
	vec3(-165.889,  3.318,  -199.328),
	vec3(-82.6373,  8.5996,  -199.803),
	vec3(-0.841358,  9.05948,  -174.974),
	vec3(62.1103, 4.26467,  -114.65),
	vec3(12.0577,  18.8928,  -71.8255), //
	vec3(48.9733,  11.2045,  86.9428),
	vec3(-21.2466, 23.6972, 52.6158),
	vec3(-44.3989, 10.7834, 106.232)
};

vec3 posAlberi[42]{
	vec3(-131.871,15.8193, -19.2037),
	vec3(-137.361, 16.3136, -17.8872),
	vec3(-132.127, 15.5241, -23.7722),
	vec3(-140.205, 5.01238, -67.1456),
	vec3(-161.647, 5.03135, -65.6213),
	vec3(-191.905, 5.05535, -72.7349),
	vec3(-192.028, 8.99972, -41.1358),
	vec3(-187.77, 9.7506, -20.5608),
	vec3(-176.286, 6.75461, -109.42),
	vec3(-207.558, 5.2753, -122.255),
	vec3(-193.867, 7.84969, -150.751),
	vec3(-173.182, 4.94121, -167.874),
	vec3(-170.453, 3.83849, -199.442),
	vec3(-156.646, 3.2033, -212.306),
	vec3(-164.515, 4.06535, -222.704),
	vec3(-122.611, 4.01282, -218.555),
	vec3(-117.892, 3.11056, -223.085),
	vec3(-78.9989, 4.02721, -218.296),
	vec3(-73.3489, 4.7645, -218.293),
	vec3(-33.6549, 9.37273, -199.489),
	vec3(-28.2755, 7.72059, -203.478),
	vec3(-33.5666, 3.36087, -167.967),
	vec3(22.8169, 15.343, -66.7549),
	vec3(22.4487, 15.6279, -71.325),
	vec3(28.1085, 15.4819, -72.7586),
	vec3(78.4108, 7.82249, -71.5289),
	vec3(83.5822, 7.94019, -49.4754),
	vec3(52.3097, 8.95182, -24.8209),
	vec3(32.0523, 4.38008,-23.218),
	vec3(83.4791, 9.62173, -17.1209),
	vec3(68.2693, 10.518, 19.0757),
	vec3(98.0472, 8.72796, 31.9129),
	vec3(85.673, 10.1421, 60.4342),
	vec3(63.4856, 8.63866, 77.6883),
	vec3(61.0808, 3.44009, 109.267),
	vec3(47.1375, 3.96443, 122.301),
	vec3(55.5595, 8.38903, 131.85),
	vec3(14.0565, 9.46824, 128.597),
	vec3(-8.48655, 9.08142, 132.105),
	vec3(-28.4249, 5.91708, 127.847),
	vec3(-35.9891, 6.49692, 126.604),
	vec3(-75.497, 3.44623, 110.476),
};

///////////// MODEL /////////////////
// World
Terrain* terreno{};
skyBox* sb;
Model* castello;
Model* fontana;
Model* tubo;
Model* tubo1;
Model* cespugli;
Model* albero_grande;
Model* albero_yoshi;
Model* albero;
Model* blocchi;
Model* coin;
Model* stella;

// Particelle
ParticleHendler* smokeParticle;
ParticleHendler* lightningParticle;
ParticleHendler* waterParticle;
ParticleHendler* dustParticle;
ParticleHendler* fireParticle;

// Personaggi
Model* mario;
Model* mario1;
Model* mario2;
Model* mario3;
Model* mario4;
Model* mario5;
Model* mario6;
Model* mario7;
Model* mario8;
Model* mario9;
Model* mario10;
Model* mario11;
Model* mario12;
Model* luigi;
Model* luigi1;
Model* luigi2;
Model* luigi3;
Model* luigi4;
Model* luigi5;
Model* luigi6;
Model* luigi7;
Model* luigi8;
Model* luigi9;
Model* luigi10;
Model* luigi11;
Model* luigi12;
Model* yoshi;
Model* yoshi1;
Model* yoshi2;
Model* yoshi3;
Model* yoshi4;
Model* yoshi5;
Model* yoshi6;
Model* yoshi7;
Model* yoshi8;
Model* yoshi9;
Model* yoshi10;
Model* yoshi11;
Model* yoshi12;
Model* captain_toad;
Model* peach;

// Nemici
Model* thwomp;
Model* goomba;
Model* whomp;
Model* boo;
Model* bulletBill;
Model* chainChomp;
Model* chain;
Model* pokey;
Model* spiny;
Model* urchin;
Model* pukupuku;


//Inizilizzazione musica
irrklang::ISoundEngine* SoundEngine_menu = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_scelta = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_player = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_coin = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_world = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_stella = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_thwomp = irrklang::createIrrKlangDevice();

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_menuSelection(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_levelSelection(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_pgselect(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_menuhelp(GLFWwindow* window, int key, int scancode, int action, int mods);
void handleKeyInput(GLFWwindow* window);

// creazione di un quadrato
float bgvertices[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

//indici texture quadrato
unsigned int bgindices[] = {
	0,1,2,
	0,2,3,
};

//creazione di un cubo 
float pgvertices[] =  {
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	-0.1f, 0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f, -0.1f, 0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,
	-0.1f, -0.1f, 0.1f,

	-0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,

	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,

	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, -0.1f,
};

// --------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void pointLight(Shader* lightingShader) {
	simpleShader->use();
	lightingShader->setVec3("light.position", vec3(-68.5, 275.0, -85.5));
	lightingShader->setVec3("viewPos", lightPosition);

	// light properties
	lightingShader->setVec3("light.ambient", 1000.0, 1000.0, 1000.0);
	lightingShader->setVec3("light.diffuse", 1000.0, 1000.0, 1000.0);
	lightingShader->setVec3("light.specular", 1000.0, 1000.0, 1000.0);
	lightingShader->setFloat("light.constant", 1.0f);
	lightingShader->setFloat("light.linear", 0.09f);
	lightingShader->setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader->setFloat("material.shininess", 32.0f);
}

void cubeSkyBoxInit()
{
	sb = new skyBox();
	skyBoxVAO = sb->generateCubeBuffer(skyBoxVAO, skyBoxVBO);

	cubeMapTexture = sb->loadCubemap(faces);
}

string pgSelect() {
	string path = "asset/music/";
	if (gamestatus == 2)path += "announcer_";
	else path += "start-";
	if (pgindex == 0) {
		path += "mario.wav";
	}
	else if (pgindex == 1) {
		path += "luigi.wav";
	}
	else
		path += "yoshi.wav";
	return path;
}

void initShader(Shader* shaderToUse) {
	// create transformations
	mat4 projection = mat4(1.0f);	//identity matrix
	projection = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shaderToUse->use();
	shaderToUse->setMat4("projection", projection);
	mat4 view = camera.GetViewMatrix();
	shaderToUse->setMat4("view", view);
}


//----------- RENDER ------------------------

void renderNemico(Model* nemico, Shader* shader, vec3 position, vec3 size, float rotazione, vec3 rotation, bool ruota = false)
{
	mat4 viewMatrix = camera.GetViewMatrix();
	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("projection", projMatrix);

	mat4 modelMatrix = glm::mat4(1.0f);

	modelMatrix = translate(modelMatrix, position);
	modelMatrix = scale(modelMatrix, size);
	if (ruota) {
		modelMatrix = rotate(modelMatrix, (float)rotazione, rotation);
	}

	shader->setMat4("model", modelMatrix);
	nemico->Draw(shader);
}

void renderCoin(Model* coin, Shader* shader, vec3 posCoin) {

	mat4 viewMatrix = camera.GetViewMatrix();
	shader->setMat4("view", viewMatrix);

	mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = translate(modelMatrix, posCoin);
	modelMatrix = rotate(modelMatrix, 1 * (float)glfwGetTime(), WORLD_UP);

	shader->setMat4("model", modelMatrix);

	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader->setMat4("projection", projMatrix);

	coin->Draw(shader);
}

void renderStella(Model* stella, Shader* shader, vec3 position)
{
	mat4 viewMatrix = camera.GetViewMatrix();
	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("projection", projMatrix);

	mat4 modelMatrix = mat4(1.0f);

	modelMatrix = translate(modelMatrix, position);
	modelMatrix = scale(modelMatrix, vec3(6, 6, 6));
	modelMatrix = rotate(modelMatrix, 0.7f * (float)glfwGetTime(), WORLD_UP);

	shader->setMat4("model", modelMatrix);
	stella->Draw(shader);
}

void renderPersonaggio(Model* personaggio, Shader* shader, vec3 grandezza, bool pos,bool terra = false, bool ruota = false)
{
	mat4 viewMatrix = camera.GetViewMatrix();
	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("projection", projMatrix);

	mat4 modelMatrix = glm::mat4(1.0f);
	if (pos) {
		if (terra) {
			modelMatrix = translate(modelMatrix, player.getMidValPosition());
		}else modelMatrix = translate(modelMatrix, player.getMidValPosition() + vec3(0.0, -0.4, 0.0));
	}
	else
	modelMatrix = translate(modelMatrix, player.getMidValPosition());
	modelMatrix = rotate(modelMatrix, radians(player.getYaw()), WORLD_UP);
	modelMatrix = rotate(modelMatrix, 3.1f, vec3(0.0f, 1.0f, 0.0f));
	if (ruota) {
		modelMatrix = rotate(modelMatrix, 1.5f, vec3(1.0f, 0.0f, 0.0f));
	}
	modelMatrix = scale(modelMatrix, grandezza);

	shader->setMat4("model", modelMatrix);

	personaggio->Draw(shader);
}

void renderWorld(Model* modello, Shader* shader, vec3 pos, float size) {
	mat4 viewMatrix = camera.GetViewMatrix();
	shader->setMat4("view", viewMatrix);
	
	mat4 model = mat4(1);
	model = translate(model, pos);
	model = scale(model, vec3(size, size, size));
	shader->setMat4("model", model);

	modello->Draw(shader);
}

void renderTubo(Model* modello, Shader* shader, vec3 pos, float size, float rotazione) {
	mat4 viewMatrix = camera.GetViewMatrix();
	shader->setMat4("view", viewMatrix);

	mat4 model = mat4(1);
	model = translate(model, pos);
	model = rotate(model, rotazione, vec3(0.0f, 1.0f, 0.0f));
	model = scale(model, vec3(size, size, size));
	shader->setMat4("model", model);

	modello->Draw(shader);
}

//------------ AZIONI -------------------------
bool collisionDetect(vec3 oggetto, float grandezza) {
	if ((oggetto.x - player.getMidValPosition().x) * (oggetto.x - player.getMidValPosition().x) + (oggetto.z - player.getMidValPosition().z) * (oggetto.z - player.getMidValPosition().z) + (oggetto.y - player.getMidValPosition().y) * (oggetto.y - player.getMidValPosition().y) < grandezza)
	{
		return true;
	}
	else
		return false;
}

//void collisionNpc( bool condizione, unsigned int fumetto, bool pg_sound, string sound, vec3 pos1, vec3 pos2) {
//	if (condizione) {
//		cout << "condizione " << condizione << endl;
//		initShader(bBoardShader);
//		bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(0.0f, 0.026f, 0.0f), vec3(0.06f, 0.035f, 0.1f), fumetto, bBoardShader);
//		if (collisionDetect(pos, 3.0)) {
//			if (pg_sound) {
//				SoundEngine_world->play2D(sound.c_str());
//				pg_sound = false;
//			}
//			pos1 = pos2;
//		}
//	}
//	else pg_sound = true;
//}

vec3 movimentoY(vec3 pos, float speedTop, float speedDown, float max, float min) {
	if (top) {
		altezza -= speedTop * velocita;
		if (altezza < min) {
			altezza = min;
			top = false;
			if (collisionDetect(posThwomp, 600)) {
				SoundEngine_thwomp->play2D("asset/music/thwomp.wav");
				if (collisionDetect(posThwomp, 10)) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
				}
			}
		}
	}
	else if (!top) {
		altezza += speedDown * velocita;
		if (altezza > max) {
			altezza = max;
			top = true;
		}
	}
	pos = vec3(pos.x, altezza, pos.z);
	return pos;
}

vec3 movimentoZ(vec3 pos, float max, float min, float raggio, float angle) {
	if (topA) {
		zeta -= 0.2;
		if (zeta < min) {
			zeta = min;
			rotazione = 0;
			topA = false;
			if (collisionDetect(posBoo, 600)) {
				SoundEngine_thwomp->play2D("asset/music/boo.wav");
			}
		}
	}
	else if (!topA) {
		zeta += 0.2;
		if (zeta > max) {
			zeta = max;
			rotazione = 3.1;
			topA = true;
			if (collisionDetect(posBoo, 600)) {
				SoundEngine_thwomp->play2D("asset/music/boo.wav");
			}
		}
	}
	angoloB += angle;
	booZ = pos.y + raggio * sin(angoloB);
	pos = vec3(pos.x, booZ, zeta);
	return pos;
}

vec3 movimentoPokey(vec3 pos, float max, float min) {
	if (topP) {
		altezzaP -= 0.04 * velocita;
		if (altezzaP < min) {
			altezzaP = min;
			topP = false;
			if (collisionDetect(posThwomp, 600)) {
				SoundEngine_thwomp->play2D("asset/music/thwomp.wav");
				if (collisionDetect(posThwomp, 10)) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
				}
			}
		}
	}
	else if (!topP) {
		altezzaP += 0.6 * velocita;
		if (altezzaP > max) {
			altezzaP = max;
			topP = true;
		}
	}
	pos = vec3(pos.x, altezzaP, pos.z);
	return pos;
}

float movimento(float max, float min) {
	if (inizio) {
		muove += 0.1 * velocita;
		if (muove > max) {
			muove = max;
			inizio = false;
			if (collisionDetect(posWhomp, 600)) {
				SoundEngine_thwomp->play2D("asset/music/whomp.mp3" );
				if (collisionDetect(posWhomp, 20)) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
				}
			}
		}
	}
	else if (!inizio) {
		muove -= 0.01 * velocita;
		if (muove < min) {
			muove = min;
			inizio = true;
		}
	}
	return muove;
}

vec3 movimentoXZ(vec3 pos,float raggio, float angolo) {
	bulletX -= 0.2;
	bulletZ += 0.1;
	if (bulletX < -105.663 && bulletZ > -17.1178) {
		bulletX = -62.4986;
		bulletZ = -33.618;
		if (collisionDetect(pos, 600)) {
			SoundEngine_thwomp->play2D("asset/music/bulletBill.wav");
		}
	}
	return vec3(bulletX, pos.y, bulletZ);
}

vec3 movimentoCircolareXZ(vec3 centro, float raggio, float speed) {
	angoloG += speed * velocita;
	cerchioTerraX = centro.x + raggio * cos(angoloG);
	cerchioTerraZ = centro.z + raggio * sin(angoloG);
	return vec3(cerchioTerraX, centro.y, cerchioTerraZ);
}

vec3 movimentoCircolareXY(vec3 centro, float raggio) {
	angoloP += 0.04;
	cerchioAriaX = centro.x + raggio * cos(angoloP);
	cerchioAriaY = centro.y + raggio * sin(angoloP);
	return vec3(cerchioAriaX, cerchioAriaY, centro.z);;
}

vec3 movimentoCircolareXYZ(vec3 centro, float raggio) {
	angoloP += 0.04;
	cerchioSinX = centro.x + raggio * cos(angoloP);
	cerchioSinY = centro.y + raggio * sin(angoloP);
	cerchioSinZ = centro.z + raggio * cos(angoloP);
	return vec3(cerchioSinX, cerchioSinY, cerchioSinZ);;
}

vec3 movimentoSin(vec3 centro, float raggio, float angle) {
	angoloB += angle;
	booZ = centro.y + raggio * sin(angoloB);
	return vec3(centro.x, booZ, centro.z);
}

vec3 movimentoStella() {
	if (monete == obiettivi) {
		if (star) {
			SoundEngine_stella->play2D("asset/music/star_appears.wav", GL_FALSE);
			star = false;
		}
		if (topS) {
			altezzaS -= 0.1;
			if (altezzaS < 5.32398f) {
				altezzaS = 5.32398f;
				topS = false;
			}
		}
		else if (!topS) {
			altezzaS += 0.1;
			if (altezzaS > 10.32398f) {
				altezzaS = 10.32398f;
				topS = true;
			}
		}
	}
	else altezzaS = -10;
	posStar = vec3(posStar.x, altezzaS, posStar.z);
	//cout << "altezza = "<< altezzaS << endl;
	return posStar;
}

vec3 movimentoTubo(vec3 pos) {
	if (altezzaTubo == -2) {
		SoundEngine_world->play2D("asset/music/tubo.mp3", GL_FALSE);
	}
	altezzaTubo += 0.1;
	if (altezzaTubo > 1) {
		start = true;
		altezzaTubo = 1;
	}
	pos = vec3(pos.x, altezzaTubo, pos.z);
	return pos;
}

//----------- BILLBOARD --------------
unsigned int loadTextureAlpha(string path){
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);


        //std::cout << "Texture loaded " << path << std::endl;
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

unsigned int loadPg(int pg) {

	switch (pg) {
	case 0: {
		pgbb = mariobb;
		break;
	}
	case 1: {
		pgbb = luigibb;
		break;
	}
	case 2: {
		pgbb = yoshibb;
		break;
	}
	}
	return pgbb;
}

unsigned int findNum(int number) {
	switch (number) {
	case 0: {
		numbb = num0bb;
		break;
	}
	case 1: {
		numbb = num1bb;
		break;
	}
	case 2: {
		numbb = num2bb;
		break;
	}
	case 3: {
		numbb = num3bb;
		break;
	}
	case 4: {
		numbb = num4bb;
		break;
	}
	case 5: {
		numbb = num5bb;
		break;
	}
	case 6: {
		numbb = num6bb;
		break;
	}
	case 7: {
		numbb = num7bb;
		break;
	}
	case 8: {
		numbb = num8bb;
		break;
	}
	case 9: {
		numbb = num9bb;
		break;
	}
	}
	return numbb;
}

void renderBillBoard(Shader* shader, int pg) {
	////////// Personaggio ////////////

	bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.07f, 0.036f, 0.0f), vec3(0.01f, 0.01f, 0.1f), loadPg(pg), shader);
	

	///////// Vite ////////////
	switch (vite) {
		case 0: {
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.061f, 0.035f, 0.0f), vec3(0.006f, 0.006f, 0.1f), vitebb, shader);
			break;
		}
		case 1: {
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.061f, 0.035f, 0.0f), vec3(0.006f, 0.006f, 0.1f), vitebb, shader);
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.055f, 0.035f, 0.0f), vec3(0.006f, 0.006f, 0.1f), vitebb, shader);
			break;
		}
		case 2: {
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.061f, 0.035f, 0.0f), vec3(0.006f, 0.006f, 0.1f), vitebb, shader);
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.055f, 0.035f, 0.0f), vec3(0.006f, 0.006f, 0.1f), vitebb, shader);
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.049f, 0.035f, 0.0f), vec3(0.006f, 0.006f, 0.1f), vitebb, shader);
			break;
		}
	}

	////////// Coin /////////
	bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.07f, 0.027f, 0.0f), vec3(0.008f, 0.009f, 0.1f), monetebb, shader);

	///////// Numero Coin ///////////
	string numberString = to_string(monete);
	if (numberString.length() == 1) {
		bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.0625f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num0bb, shader);
		bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.058f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), findNum(numberString[0] - '0'), shader);
	}else{
		bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.062f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), findNum(numberString[0] - '0'), shader);
		bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.058f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), findNum(numberString[1] - '0'), shader);
	}
	

	//////// Separatore ///////////

	bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.054f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), separatorbb, shader);

	////////// Obiettivo /////////
	switch (levelindex) {
		case 0: {
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.050f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num1bb, shader);
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.046f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num0bb, shader);
			break;
		}
	case 1: {
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.05f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num1bb, shader);
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.046f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num5bb, shader);
			break;
		}
	case 2: {
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.05f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num2bb, shader);
			bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(-0.046f, 0.027f, 0.0f), vec3(0.005f, 0.005f, 0.1f), num0bb, shader);
			break;
		}
	}
}


//-------------- INIT --------------------

unsigned int loadTexture(string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
																					// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}
	stbi_image_free(data);

	return texture;
}

void setDeltaTime() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - previousTime;
	previousTime = currentFrame;
}

void depthMapFBOInit()
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void textureLoad() {

	num1bb = loadTextureAlpha("asset/image/BillBoard/1.png");
	num2bb = loadTextureAlpha("asset/image/BillBoard/2.png");
	num3bb = loadTextureAlpha("asset/image/BillBoard/3.png");
	num4bb = loadTextureAlpha("asset/image/BillBoard/4.png");
	num5bb = loadTextureAlpha("asset/image/BillBoard/5.png");
	num6bb = loadTextureAlpha("asset/image/BillBoard/6.png");
	num7bb = loadTextureAlpha("asset/image/BillBoard/7.png");
	num8bb = loadTextureAlpha("asset/image/BillBoard/8.png");
	num9bb = loadTextureAlpha("asset/image/BillBoard/9.png");
	num0bb = loadTextureAlpha("asset/image/BillBoard/0.png");
	separatorbb = loadTextureAlpha("asset/image/BillBoard/separatore.png");
	vitebb = loadTextureAlpha("asset/image/BillBoard/cuore.png");
	mariobb = loadTextureAlpha("asset/image/BillBoard/mario.png");
	luigibb = loadTextureAlpha("asset/image/BillBoard/luigi.png");
	yoshibb = loadTextureAlpha("asset/image/BillBoard/yoshi.png");
	monetebb = loadTextureAlpha("asset/image/BillBoard/coin.png");
	fumetto_toad = loadTextureAlpha("asset/image/BillBoard/fumetto_toad.png");
	fumetto_peach = loadTextureAlpha("asset/image/BillBoard/fumetto_peach.png");
	fumetto_peach_star = loadTextureAlpha("asset/image/BillBoard/fumetto_peach_star.png");
}

void createBuffer() {
	//--------------Creazione quadrato per i menu-----------------------
	glGenVertexArrays(1, &menuVAO);
	glGenBuffers(1, &menuVBO);
	glGenBuffers(1, &menuEBO);

	glBindVertexArray(menuVAO);

	glBindBuffer(GL_ARRAY_BUFFER, menuVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, menuEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgindices), bgindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//--------------Open Menu-------------------

	menushaderProgram = glCreateProgram();
	menutexture = MenuInit(menushaderProgram, "asset/image/menuStart/SuperMarioHub.jpg");

	//--------Select Menu------------------

	menuSelectionShaderProgram = glCreateProgram();
	menuSelectionShaderProgram2 = glCreateProgram();
	menuSelectionShaderProgram3 = glCreateProgram();
	menuSelectionShaderProgram4 = glCreateProgram();
	menuSelectiontexture1 = MenuInit(menuSelectionShaderProgram, "asset/image/menuStart/play.jpg");
	menuSelectiontexture2 = MenuInit(menuSelectionShaderProgram2, "asset/image/menuStart/level.jpg");
	menuSelectiontexture3 = MenuInit(menuSelectionShaderProgram3, "asset/image/menuStart/crediti.jpg");
	menuSelectiontexture4 = MenuInit(menuSelectionShaderProgram4, "asset/image/menuStart/esc.jpg");

	//---------Level menu-------------------------

	levelMenuShaderProgram1 = glCreateProgram();
	levelMenutexture1 = MenuInit(levelMenuShaderProgram1, "asset/image/menuStart/easy.jpg");

	levelMenuShaderProgram2 = glCreateProgram();
	levelMenutexture2 = MenuInit(levelMenuShaderProgram2, "asset/image/menuStart/normal.jpg");

	levelMenuShaderProgram3 = glCreateProgram();
	levelMenutexture3 = MenuInit(levelMenuShaderProgram3, "asset/image/menuStart/hard.jpg");

	//---------Credit menu-------------------------

	creditMenuShaderProgram = glCreateProgram();
	creditMenutexture = MenuInit(creditMenuShaderProgram, "asset/image/menuStart/credits.jpg");

	//--------CharacterMenu---------

	pgMenuShaderProgram1 = glCreateProgram();
	pgMenutexture1 = MenuInit(pgMenuShaderProgram1, "asset/image/PgSelection/mario.jpg");

	pgMenuShaderProgram2 = glCreateProgram();
	pgMenutexture2 = MenuInit(pgMenuShaderProgram2, "asset/image/PgSelection/luigi.jpg");

	pgMenuShaderProgram3 = glCreateProgram();
	pgMenutexture3 = MenuInit(pgMenuShaderProgram3, "asset/image/PgSelection/yoshi.jpg");

	//-----------SelectFirstPG---------------
	glGenVertexArrays(1, &bgVAO);
	glGenBuffers(1, &bgVBO);
	glGenBuffers(1, &bgEBO);

	glBindVertexArray(bgVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgindices), bgindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	bgshaderProgram = glCreateProgram();
	bgtexture = BgInit_1(bgshaderProgram, "asset/image/PgSelection/mario.jpg");

	//-------------SelectSecondPG-------------------
	glGenVertexArrays(1, &bgVAO2);
	glGenBuffers(1, &bgVBO2);
	glGenBuffers(1, &bgEBO2);

	glBindVertexArray(bgVAO2);

	glBindBuffer(GL_ARRAY_BUFFER, bgVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgindices), bgindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	bgshaderProgram2 = glCreateProgram();
	bgtexture2 = BgInit_2(bgshaderProgram2, "asset/image/PgSelection/luigi.jpg");

	//-------------SelectSecondPG-------------------
	glGenVertexArrays(1, &bgVAO3);
	glGenBuffers(1, &bgVBO3);
	glGenBuffers(1, &bgEBO3);

	glBindVertexArray(bgVAO3);

	glBindBuffer(GL_ARRAY_BUFFER, bgVBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgindices), bgindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	bgshaderProgram3 = glCreateProgram();
	bgtexture3 = BgInit_3(bgshaderProgram3, "asset/image/PgSelection/yoshi.jpg");

	//---------------End----------------------------

	gameOverShaderProgram = glCreateProgram();
	gameOvertexture = MenuInit(gameOverShaderProgram, "asset/image/menuStart/gameover.jpg");

	winShaderProgram = glCreateProgram();
	wintexture = MenuInit(winShaderProgram, "asset/image/menuStart/win.jpg");

	//--------------Player-----------------
	glGenVertexArrays(1, &pgVAO);
	glGenBuffers(1, &pgVBO);
	glBindVertexArray(pgVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pgVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pgvertices), pgvertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	bBoard.generateBuffers();

}

void modelLoad() {
	////// MAPPA //////////

	terreno = new Terrain("asset/object/world/terreno/giardino2.obj", "asset/object/world/terreno/erba.jpg");
	castello = new Model("asset/object/world/castello/castello.obj");
	fontana = new Model("asset/object/world/fontana/fontana.obj");
	tubo = new Model("asset/object/world/tubo/tubo.obj");
	tubo1 = new Model("asset/object/world/tubo/tubo1.obj");
	cespugli = new Model("asset/object/world/cespuglio/cespugli.obj");
	albero_grande = new Model("asset/object/world/alberi/albero_grande.obj");
	albero_yoshi = new Model("asset/object/world/alberi/albero_yoshi.obj");
	albero = new Model("asset/object/world/alberi/albero.obj");
	blocchi = new Model("asset/object/world/blocchi/blocchi.obj");


	///////// PARTICELLE /////////////
	smokeParticle = new ParticleHendler("asset/object/Particelle/Nuvole/cloud.obj", 2, 0.1, 0.3);
	lightningParticle = new ParticleHendler("asset/object/Particelle/Scintille/scintille.obj", 2, 2, 0.3);
	waterParticle = new ParticleHendler("asset/object/Particelle/Acqua/water.obj", 100, 1.8, 1);
	fireParticle = new ParticleHendler("asset/object/Particelle/Fuoco/smoke.obj", 2, 0.1, 0.3);
	dustParticle = new ParticleHendler("asset/object/Particelle/Polvere/dust.obj", 2, 1, 0.3);


	////////// PERSONAGGI ////////////
	Mario = {
		mario = new Model("asset/object/Personaggi/Mario/mario.obj"),
		mario1 = new Model("asset/object/Personaggi/Mario/mario_run1.obj"),
		mario2 = new Model("asset/object/Personaggi/Mario/mario_run2.obj"),
		mario3 = new Model("asset/object/Personaggi/Mario/mario_run3.obj"),
		mario4 = new Model("asset/object/Personaggi/Mario/mario_run4.obj"),
		mario5 = new Model("asset/object/Personaggi/Mario/mario_run5.obj"),
		mario6 = new Model("asset/object/Personaggi/Mario/mario_run6.obj"),
		mario7 = new Model("asset/object/Personaggi/Mario/mario_run7.obj"),
		mario8 = new Model("asset/object/Personaggi/Mario/mario_run8.obj"),
		mario9 = new Model("asset/object/Personaggi/Mario/mario_run9.obj"),
		mario10 = new Model("asset/object/Personaggi/Mario/mario_run10.obj"),
		mario11 = new Model("asset/object/Personaggi/Mario/mario_run11.obj"),
		mario12 = new Model("asset/object/Personaggi/Mario/mario_schiacciato.obj")
	};

	Luigi = {
		luigi = new Model("asset/object/Personaggi/Luigi/Luigi.obj"),
		luigi1 = new Model("asset/object/Personaggi/Luigi/Luigi1.obj"),
		luigi2 = new Model("asset/object/Personaggi/Luigi/Luigi2.obj"),
		luigi3 = new Model("asset/object/Personaggi/Luigi/Luigi3.obj"),
		luigi4 = new Model("asset/object/Personaggi/Luigi/Luigi4.obj"),
		luigi5 = new Model("asset/object/Personaggi/Luigi/Luigi5.obj"),
		luigi6 = new Model("asset/object/Personaggi/Luigi/Luigi6.obj"),
		luigi7 = new Model("asset/object/Personaggi/Luigi/Luigi7.obj"),
		luigi8 = new Model("asset/object/Personaggi/Luigi/Luigi8.obj"),
		luigi9 = new Model("asset/object/Personaggi/Luigi/Luigi9.obj"),
		luigi10 = new Model("asset/object/Personaggi/Luigi/Luigi10.obj"),
		luigi11 = new Model("asset/object/Personaggi/Luigi/Luigi11.obj"),
		luigi12 = new Model("asset/object/Personaggi/Luigi/Luigi_schiacciato.obj")
	};

	Yoshi = {
		yoshi = new Model("asset/object/Personaggi/Yoshi/Yoshi.obj"),
		yoshi1 = new Model("asset/object/Personaggi/Yoshi/Yoshi1.obj"),
		yoshi2 = new Model("asset/object/Personaggi/Yoshi/Yoshi2.obj"),
		yoshi3 = new Model("asset/object/Personaggi/Yoshi/Yoshi3.obj"),
		yoshi4 = new Model("asset/object/Personaggi/Yoshi/Yoshi4.obj"),
		yoshi5 = new Model("asset/object/Personaggi/Yoshi/Yoshi5.obj"),
		yoshi6 = new Model("asset/object/Personaggi/Yoshi/Yoshi6.obj"),
		yoshi7 = new Model("asset/object/Personaggi/Yoshi/Yoshi7.obj"),
		yoshi8 = new Model("asset/object/Personaggi/Yoshi/Yoshi8.obj"),
		yoshi9 = new Model("asset/object/Personaggi/Yoshi/Yoshi9.obj"),
		yoshi10 = new Model("asset/object/Personaggi/Yoshi/Yoshi10.obj"),
		yoshi11 = new Model("asset/object/Personaggi/Yoshi/Yoshi11.obj"),
		yoshi12 = new Model("asset/object/Personaggi/Yoshi/Yoshi_schiacciato.obj")
	};

	captain_toad = new Model("asset/object/Personaggi/Toad/captain_toad.obj");

	peach = new Model("asset/object/Personaggi/Peach/peach.obj");

	/////// NEMICI ////////
	thwomp = new Model("asset/object/Nemici/Thwomp/Thwomp_down.obj");
	goomba = new Model("asset/object/Nemici/Goomba/goomba.obj");
	whomp = new Model("asset/object/Nemici/Whomp/whomp.obj");
	boo = new Model("asset/object/Nemici/Boo/boo.obj");
	bulletBill = new Model("asset/object/Nemici/Bullet Bill/bulletbill2.obj");
	chainChomp = new Model("asset/object/Nemici/Chain Chomp/chain_chomp.obj");
	chain = new Model("asset/object/Nemici/Chain Chomp/chain.obj");
	pokey = new Model("asset/object/Nemici/Pokey/pokey.obj");
	spiny = new Model("asset/object/Nemici/spiny/spiny.obj");
	urchin = new Model("asset/object/Nemici/Urchin/urchin.obj");

	/////// COIN /////////////
	coin = new Model("asset/object/Coin/coin.obj");

	/////// STELLA /////////
	stella = new Model("asset/object/Stella/star.obj");

	////// PESCE ///////////
	pukupuku = new Model("asset/object/Nemici/PukuPuku/pukupuku.obj");
}

void shaderLoad() {
	simpleShader = new Shader("shader/point_light.vert", "shader/point_light.frag");
	bBoardShader = new Shader("shader/billboardShader.vs", "shader/billboardShader.fs");
	depthShader = new Shader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");
	skyBoxShader = new Shader("shader/skybox.vs", "shader/skybox.fs");
	pgShader = new Shader("shader/car_select.vs", "shader/car_select.fs");
	particleShader = new Shader("shader/particle_shader.vs", "shader/particle_shader.fs");

	skyBoxShader->use();
	skyBoxShader->setInt("skybox", 0);

	simpleShader->use();
	simpleShader->setInt("diffuseTexture", 0);
	simpleShader->setInt("shadowMap", 10);

	particleShader->use();
	particleShader->setInt("shadowMap", 4);
}

void renderCubeSkyBox(Shader* shader)
{
	glDepthFunc(GL_LEQUAL);
	skyBoxShader->use();

	mat4 viewMatrix = mat4(mat3(camera.GetViewMatrix()));  // remove translation from the view matrix
	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_WIDTH);

	shader->setMat4("view", viewMatrix);
	shader->setMat4("projection", projMatrix);

	sb->drawCube(shader, skyBoxVAO, cubeMapTexture);
	
	glDepthFunc(GL_LESS);
}

void initLoad() {
	//---------SkyBox------------
	cubeSkyBoxInit();
	//---------Shader--------------
	shaderLoad();
	// ----------Model------------
	modelLoad();
	//------createBuffer----------
	createBuffer();
	//--------Texture------------
	textureLoad();
}

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation e imposta la finestra in full screen
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Super Mario", NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Super Mario", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------

	depthMapFBOInit();;
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	initLoad();

	player = Player(terreno->updateCameraPositionOnMap(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2, true));
	
	// ---------------------------------
	// RENDER CICLO
	// ---------------------------------
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// render loop
	SoundEngine_menu->play2D("asset/music/main.wav", GL_TRUE);
	while (!glfwWindowShouldClose(window))
	{

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		tempoTrascorso += deltaTime;
		frames++;
		lastFrame = currentFrame;
		if (tempoTrascorso >= 1) {
			fps = frames / tempoTrascorso;
			frames = 0;
			tempoTrascorso = 0;
			lastFrame = currentFrame;
		}

		posVecchia = player.position;
		tempo_rimanente -= tempoTrascorso;
		player.position = terreno->updateCameraPositionOnMap(player.position, posVecchia, 2, false);
		terreno->setShaders(simpleShader);


		/////////// START //////////////
		if (gamestatus == 0) {
			if(oldgamestatus == -1 || oldgamestatus == 6 ) {
				SoundEngine_menu->stopAllSounds();
				//SoundEngine_menu->play2D("asset/music/main.wav", GL_TRUE);
			}
			oldgamestatus = gamestatus;
			
			if (!camera.isFixed()) {
				camera.setFixed(player.front);
				//camera.setFixed(pos);
			}

			glfwSetKeyCallback(window, key_callback_menu);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			DrawMenu(menutexture, menushaderProgram, menuVAO,  window);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		//////////////// MENU INIZIALE ////////////////
		else if (gamestatus == 1) {
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_menuSelection);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (menuindex)
			{
			case 0: {
				DrawMenu(menuSelectiontexture1, menuSelectionShaderProgram, menuVAO, window);
				break;
			}
			case 1: {
				DrawMenu(menuSelectiontexture2, menuSelectionShaderProgram2, menuVAO, window);
				break;
			}
			case 2: {
				DrawMenu(menuSelectiontexture3, menuSelectionShaderProgram3, menuVAO, window);
				break;
			}
			case 3: {
				DrawMenu(menuSelectiontexture4, menuSelectionShaderProgram4, menuVAO, window);
				break;
			}
			default:
				break;
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		////////////////// MENU SCELTA DEL PERSONAGGIO /////////////////
		else if (gamestatus == 2) {
			oldgamestatus = gamestatus;
			glfwSetKeyCallback(window, key_callback_pgselect);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (pgindex)
			{
				case 0: {
					Drawcarselect2(bgtexture, bgshaderProgram, bgVAO, pgShader, mario, window);
					Personaggio = Mario;
					grandezza = grandezzaM;
					pg_hurt = "asset/music/mario_hurt.wav";
					posPg = true;
					break;
				}
				case 1: {
					Drawcarselect2(bgtexture2, bgshaderProgram2, bgVAO2, pgShader, luigi, window);
					Personaggio = Luigi;
					grandezza = grandezzaL;
					pg_hurt = "asset/music/luigi_hurt.wav";
					posPg = false;
				break;
				}
				case 2: {
					Drawcarselect2(bgtexture3, bgshaderProgram3, bgVAO3, pgShader, yoshi, window);
					Personaggio = Yoshi;
					grandezza = grandezzaY;
					pg_hurt = "asset/music/yoshi_hurt.wav";
					posPg = false;
				break;
				}
				default:
					break;
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		//////////////////// SCELTA DEL LIVELLO ///////////////////
		else if (gamestatus == 3) {
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_levelSelection);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (levelindex)
			{
			case 0: {
				DrawMenu(levelMenutexture1, levelMenuShaderProgram1, menuVAO, window);
				obiettivi = 10;
				velocita = 1.0;
				//tempo_rimanente = 144000;
				break;
			}
			case 1: {
				DrawMenu(levelMenutexture2, levelMenuShaderProgram2, menuVAO, window);
				obiettivi = 15;
				velocita = 1.5;
				//tempo_rimanente = 80000;
				break;
			}
			case 2: {
				DrawMenu(levelMenutexture3, levelMenuShaderProgram3, menuVAO, window);
				obiettivi = 20;
				velocita = 2.0;
				//tempo_rimanente = 50000;
				break;
			}
			default:
				break;
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		////////////// CREDITI //////////////////////
		else if (gamestatus == 4) {
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_menu);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			DrawMenu(creditMenutexture, creditMenuShaderProgram, menuVAO, window);

			glfwPollEvents();
			glfwSwapBuffers(window);

		}
		//////////////////// GIOCO /////////////////
		else if (gamestatus == 5) {

			if (oldgamestatus != 5) {
				SoundEngine_menu->stopAllSounds();
				//SoundEngine_menu->play2D("asset/music/main.wav", GL_TRUE);
			}

			oldgamestatus = gamestatus;
			setDeltaTime();
			camera.setDeltaTime();

			glfwSetKeyCallback(window, NULL);

			handleKeyInput(window);
			camera.FixView(player.getMidValPosition(), player.front+vec3(0.0,-0.5,0.0));

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			////////// RENDER SKYBOX /////////
			renderCubeSkyBox(skyBoxShader);

			///////// RENDER BILLBOARD //////////
			initShader(bBoardShader);
			renderBillBoard(bBoardShader, pgindex);

			mat4 lightProjection = ortho(
				-200.0f, 200.0f,
				-200.0f, 200.0f,
				-200.0f, 200.0f);

			mat4 lightView = lookAt(lightPos, vec3(0.0f), WORLD_UP);
			lightSpaceMatrix = lightProjection * lightView;

			initShader(simpleShader);
			pointLight(simpleShader);

			////////// RENDER PARTICELLE ///////////
			smokeParticle->setShaders(simpleShader);
			lightningParticle->setShaders(simpleShader);
			waterParticle->setShaders(simpleShader);
			fireParticle->setShaders(simpleShader);
			dustParticle->setShaders(simpleShader);


			///////// RENDER STAR ////////////
			initShader(simpleShader);
			renderStella(stella, simpleShader, movimentoStella());

			if (collisionDetect(movimentoStella(), 2)) {
				win = true;
				gamestatus = 6;
			}

			////////// RENDER COIN //////////////
			for (int i = 0; i < obiettivi; i++) {
				if (collisionDetect(posLightning[i], 10.0)) {
					lightningParticle->addParticleGenerator(currentFrame, posLightning[i] + vec3(0.0, 1.0, 0.0), posLightning[i] + vec3(0.0, -0.5, 0.0));
					lightningParticle->Draw(currentFrame, vec3(1., 1., 1.));
					posLightning[i].y -= 1;

				}
				if (collisionDetect(posCoin[i], 10.0)) {
					SoundEngine_world->play2D("asset/music/coin.wav");
					posCoin[i].y = -20;
					monete += 1;
				}
				renderCoin(coin, simpleShader, posCoin[i]);

			}

			////////////// RENDER NEMICI ///////////////
			// Goomba
			posGoomba1 = movimentoCircolareXZ(vec3(98.89, 6.72, -4.47), 5, 0.015);
			renderNemico(goomba, simpleShader, posGoomba1, vec3(2.8, 2.8, 2.8), -1.55f * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f), true);
			if (collisionDetect(posGoomba1, 500)) {
				dustParticle->addParticleGenerator(currentFrame, posGoomba1, posGoomba1 + vec3(0.0,0.5,0.0));
				dustParticle->Draw(currentFrame, vec3(1.0, 1.0, 1.0));
			}
			if (collisionDetect(vec3(98.89, 6.72, -4.47), 30.0)) {
				if (collisionDetect(posGoomba1, 1.0)) {
					if (colpito1){
						vite -= 1;
						SoundEngine_player->play2D(pg_hurt.c_str());
						schiacciato = true;
						colpito1 = false;
					}
				}
			}
			else colpito1 = true;

			posGoomba2 = movimentoCircolareXZ(vec3(-91.65, 2.21, 83.47), 5, 0.015);
			renderNemico(goomba, simpleShader, posGoomba2, vec3(2.8, 2.8, 2.8), -1.55f * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f), true);
			if (collisionDetect(posGoomba2, 500)) {
				dustParticle->addParticleGenerator(currentFrame, posGoomba2 + vec3(0.0, -0.5, 0.0), posGoomba2 );
				dustParticle->Draw(currentFrame, vec3(1.0, 1.0, 1.0));
			}
			if (collisionDetect(vec3(-91.65, 2.71, 83.47), 30.0)) {
				if (collisionDetect(posGoomba2, 1.0)) {
					if (colpito2) {
						vite -= 1;
						SoundEngine_player->play2D(pg_hurt.c_str());
						schiacciato = true;
						colpito2 = false;
					}
				}
			}
			else colpito2 = true;

			// Twhomp
			renderNemico(thwomp, simpleShader, movimentoY(posThwomp, 1.0, 0.1, 8.72707f, 2.2707f), vec3(11, 11, 11), 0.0f, vec3(0.0f, 0.0f, 0.0f),false);
			if (collisionDetect(movimentoY(posThwomp, 1.0, 0.1, 8.72707f, 2.2707f), 3.0)) {
				player.position = posVecchia;
			}

			// Whomp
			renderNemico(whomp, simpleShader, posWhomp, vec3(0.3, 0.3, 0.3), movimento(1.5f, 0.0f), vec3(1.0f, 0.0f, 0.0f), true);
			if (collisionDetect(posWhomp, 3.0)) {
				player.position = posVecchia;
			}

			// Pukupuku
			renderNemico(pukupuku, simpleShader, movimentoCircolareXY(vec3(-98.19, 6.94, -89.50), 7), vec3(1.5, 1.5, 1.5), 1.75f * (float)glfwGetTime(), vec3(0.0f, 0.0f, 1.0f), true);

			// Boo
			//// cerchio
			posBoo2 = movimentoCircolareXZ(movimentoSin(vec3(-207.078, 21.51, 8.21875), 1, 0.1), 5, 0.015);
			renderNemico(boo, simpleShader, posBoo2, vec3(1.5, 1.5, 1.5), 1.75f * (float)glfwGetTime(), vec3(0.0f, 1.0f, 0.0f), true);
			if (collisionDetect(posBoo2, 3)) {
				if (colpito3) {
					vite -= 1;
					SoundEngine_thwomp->play2D("asset/music/boo.wav");
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
					colpito3 = false;
				}
			}else colpito3 = true;

			//// avanti e indietro
			renderNemico(boo, simpleShader, movimentoZ(vec3(-43.40, 11.92, 93.61), 119.86, 93.61, 1, 0.015), vec3(1.5, 1.5, 1.5), rotazione, vec3(0.0f, 1.0f, 0.0f), true);
			if (collisionDetect(movimentoZ(vec3(-43.40, 11.92, 93.61), 119.86, 93.61, 1, 0.025), 3.0)) {
				if (colpito4) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
					colpito4 = false;
				}
			}
			else colpito4 = true;

			// Urchin
			posUrchin = movimentoCircolareXZ(vec3(-98.19, 5.94, -89.50), 35, 0.01);
			renderNemico(urchin, simpleShader, posUrchin, vec3(2.5, 2.5, 2.5), -1.55f * (float)glfwGetTime(), vec3(1.0f, 0.0f, 1.0f), true);
			if (collisionDetect(posUrchin + vec3(0.0, -2.0, 0.0), 20)) {
				if (colpito7) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
					colpito7 = false;
				}
			}
			else colpito7 = true;

			// Pokey
			renderNemico(pokey, simpleShader, movimentoPokey(vec3(-1.33, 9.24, -174.35),8.24, -2), vec3(2, 2, 2), -1.55f * (float)glfwGetTime(), vec3(1.0f, 1.0f, 1.0f), false);
			if (collisionDetect(vec3(-1.33, 9.24, -174.35), 600.0)) {
				dustParticle->addParticleGenerator(currentFrame, vec3(-1.33, 7.24, -174.35), vec3(-1.33, 7.74, -174.35));
				dustParticle->Draw(currentFrame, vec3(1.0, 1.0, 1.0));
			}
			if (collisionDetect(movimentoPokey(vec3(-1.33, 9.24, -174.35), 7.74, -2), 3.0)) {
				if (colpito5) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
					colpito5 = false;
				}
			}
			else colpito5 = true;

			// Bullet Bill
			//renderNemico(bulletBill, simpleShader, movimentoXZ(vec3(-61.4986, 3.20, -34.618), 5, 0.005), vec3(0.05,0.05,0.05), -1.15, vec3(0.0f, 1.0f, 0.0f), true);
			/*fireParticle->addParticleGenerator(currentFrame, movimentoXZ(vec3(-61.4986, 2.70, -34.618), 5, 0.005), movimentoXZ(vec3(-61.4986, 3.70, -34.618), 5, 0.005));
			fireParticle->Draw(currentFrame, vec3(1.0,1.0,1.0));*/
			if (collisionDetect(vec3(-80.4986, 2.20, -24.618), 800.0)) {
				renderNemico(bulletBill, simpleShader, movimentoXZ(vec3(-61.4986, 3.20, -34.618), 5, 0.005), vec3(0.05, 0.05, 0.05), -1.15, vec3(0.0f, 1.0f, 0.0f), true);
				fireParticle->addParticleGenerator(currentFrame, movimentoXZ(vec3(-61.4986, 2.70, -34.618), 5, 0.005), movimentoXZ(vec3(-61.4986, 3.70, -34.618), 5, 0.005));
				fireParticle->Draw(currentFrame, vec3(1.0, 1.0, 1.0));
			}
			if (collisionDetect(movimentoXZ(vec3(-61.4986, 2.20, -34.618), 5, 0.005), 4.0)) {
				if (colpito6) {
					vite -= 1;
					SoundEngine_player->play2D(pg_hurt.c_str());
					schiacciato = true;
					colpito6 = false;
				}
			}
			else colpito6 = true;

			////////// RENDER PLAYER //////////////
			if(start){
				if (animazione) {
					initShader(simpleShader);
					renderPersonaggio(Personaggio[animation], simpleShader, grandezza, posPg, false, false);
					smokeParticle->addParticleGenerator(currentFrame, player.position + vec3(0.0, -3.0, 0.0), player.position + vec3(0.0, -1.5, 0.0));
					smokeParticle->Draw(currentFrame, vec3(0.5, 0.7, 0.5));
					if (animation >= 11 && animation < 12) {
						animation = 1;
					}
				}
				else if (schiacciato) {
					renderPersonaggio(Personaggio[12], simpleShader, grandezza, posPg, true, false);
				}
				else renderPersonaggio(Personaggio[0], simpleShader, grandezza, posPg, false, false);
			}
						
			//////// RENDER TOAD //////////////
			initShader(simpleShader);
			renderWorld(captain_toad, simpleShader, posToad, 0.01);
			if (collisionDetect(posToad, 4)) {
				initShader(bBoardShader);
				bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(0.0f, 0.026f, 0.0f), vec3(0.06f, 0.035f, 0.1f), fumetto_toad, bBoardShader);
				if (collisionDetect(posToad, 3.0)) {
					if (toad_sound) {
						SoundEngine_world->play2D("asset/music/toad.wav");
						toad_sound = false;
					}
					player.position = posVecchia;
				}
			}
			else toad_sound = true;

			//////// RENDER PEACH //////////////
			initShader(simpleShader);
			renderWorld(peach, simpleShader, posPeach, 0.15);
			if (collisionDetect(posPeach, 4.0) && monete == obiettivi) {
				initShader(bBoardShader);
				bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(0.0f, 0.026f, 0.0f), vec3(0.06f, 0.035f, 0.1f), fumetto_peach_star, bBoardShader);
				if (collisionDetect(posPeach, 3.0)) {
					if (peach_sound) {
						SoundEngine_world->play2D("asset/music/peach_star.wav");
						peach_sound = false;
					}
					player.position = posVecchia;
				}
			}
			else peach_sound = true;
				
			if (collisionDetect(posPeach, 4.0)) {
				initShader(bBoardShader);
				bBoard.drawBillboard(vec3(0.0f, 0.0f, -0.11f), vec3(0.0f, 0.026f, 0.0f), vec3(0.06f, 0.035f, 0.1f), fumetto_peach, bBoardShader);
				if (collisionDetect(posPeach, 3.0)) {
					if (peach_sound2) {
						SoundEngine_world->play2D("asset/music/peach.wav");
						peach_sound2 = false;
					}
					player.position = posVecchia;
				}
			}
			else peach_sound2 = true;

			////////// RENDER TERRENO //////////////
			// terreno
			terreno->traslate(glm::vec3(0.0f, 0.0f, 0.0f));
			terreno->scale(glm::vec3(1.0f, 1.0f, 1.0f));
			terreno->Draw();

			// castello
			renderWorld(castello, simpleShader, vec3(-105.391f, -2.50679f, -167.198f), 10.0);
			if (collisionDetect(vec3(-105.391f, -2.00679f, -167.198f), 800.0)) {
				player.position = posVecchia;
			}

			// fontana
			renderWorld(fontana, simpleShader, vec3(-98.0069f, 4.933f, -89.6022f), 8.0);
			if (collisionDetect(vec3(-98.0069f, 4.933f, -89.6022f), 500.0)) {
				waterParticle->addParticleGenerator(currentFrame, vec3(-98.0069f, 11.933f, -89.6022f), vec3(-98.0069f, 5.933f, -89.6022f));
				waterParticle->Draw(currentFrame, vec3(0.15, 0.15, 0.15));
				if (collisionDetect(vec3(-98.0069f, 4.933f, -89.6022f), 130.0)) {
					player.position = posVecchia;
				}
			}
			
			// tubo iniziale
			renderWorld(tubo, simpleShader, movimentoTubo(posTubo), 0.2);
			if (collisionDetect(posTubo, 3.0)) {
				player.position = posVecchia;
			}

			// Tubo 1
			renderTubo(tubo1, simpleShader, vec3(-62.4986, 3.20, -33.618), 0.25, 2.1);
			if (collisionDetect(vec3(-62.4986, 3.20, -33.618), 3.0)) {
				player.position = posVecchia;
			}
			// Tubo 2
			renderTubo(tubo1, simpleShader, vec3(-99.663, 3.20, -15.4178), 0.25, -1.2);
			if (collisionDetect(vec3(-99.663, 3.20, -15.4178), 3.0)) {
				player.position = posVecchia;
			}

			// cespugli
			renderWorld(cespugli, simpleShader,vec3(0.0,0.0,0.0), 1.0); 

			// albero grande
			renderWorld(albero_grande, simpleShader,vec3(-22.148,4.857,-149.109), 10.0);
			if (collisionDetect(vec3(-22.148, 5.857, -149.109), 37.0)) {
				player.position = posVecchia;
			}

			// albero Yoshi
			renderWorld(albero_yoshi, simpleShader,vec3(-24.995,14.883,-54.685), 10.0);
			if (collisionDetect(vec3(-24.995, 14.883, -54.685), 18.0)) {
				player.position = posVecchia;
			}

			// alberi
			for (int i = 0; i < numAlberi; i++) {
				renderWorld(albero, simpleShader, posAlberi[i]+vec3(0.0, -2.0, 0.0), 10.0);
				if (collisionDetect(posAlberi[i], 10.0)) {
					player.position = posVecchia;
				}
			}
			
			// blocchi
			renderWorld(blocchi, simpleShader, vec3(0.0, 0.0, 0.0), 1.0);

			/////////// GAME OVER ///////////////
			if (vite <= -1) {
				gamestatus = 6;
			}

			//glDepthFunc(GL_LEQUAL);

			glDepthFunc(GL_LESS);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		/////////////// FINE GIOCO //////////////////
		else if (gamestatus == 6) {
			
			if (oldgamestatus != 6){ SoundEngine_menu->stopAllSounds();
				if (win) {
					SoundEngine_stella->play2D("asset/music/star.wav", GL_FALSE);
				}
				else {
					SoundEngine_stella->play2D("asset/music/Game-Over.wav", GL_FALSE);
				}
			}	
			
			oldgamestatus = gamestatus;
			setDeltaTime();

			glfwSetKeyCallback(window, key_callback_menu);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (win) {
				DrawMenu(wintexture, winShaderProgram, menuVAO, window);
			}
			else {
				DrawMenu(gameOvertexture, gameOverShaderProgram, menuVAO, window);
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	// optional: de-allocate all resources once they've outlived their purpose:

	glDeleteVertexArrays(1, &skyBoxVAO);
	glDeleteVertexArrays(1, &skyBoxVBO);
	glDeleteVertexArrays(1, &skyBoxEBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

//---------- CALLBACK ------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = (xpos - lastX)*0.3;
	float yoffset = (lastY - ypos)*0; // cosi blocco l'asse y e il mouse si muove solo sull'asse x

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	if(key == GLFW_KEY_ESCAPE){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key == GLFW_KEY_ENTER && gamestatus != 6) {
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 1;
	}
}

void key_callback_menuhelp(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_B:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 1;
		break;
	case GLFW_KEY_H:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 2;
		break;
	default:
		break;
	}
}

void key_callback_menuSelection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_DOWN:
		menuindex = (menuindex + 1) % 4;
		SoundEngine_scelta->play2D("asset/music/menuselect.wav", GL_FALSE);
		break;
	case GLFW_KEY_UP:
		if (menuindex == 0)menuindex = 4;
		SoundEngine_scelta->play2D("asset/music/menuselect.wav", GL_FALSE);
		menuindex = (menuindex - 1) % 4;
		break;
	case GLFW_KEY_ENTER:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		if (menuindex == 0) {
			gamestatus = 2;
			SoundEngine_scelta->play2D(pgSelect().c_str(), GL_FALSE);
		}
		else if (menuindex == 1) {
			gamestatus = 3;
		}
		else if (menuindex == 2) {
			gamestatus = 4;
		}
		else if (menuindex == 3) {
			SoundEngine_scelta->play2D("asset/music/arrivederci.wav", GL_FALSE);
			//deve aspettare
			this_thread::sleep_for(chrono::milliseconds(1000));
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	default:
		break;
	}
}

void key_callback_levelSelection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_DOWN:
		if (levelindex == 2)break;
		levelindex = (levelindex + 1) % 3;
		SoundEngine_scelta->play2D("asset/music/menuselect.wav", GL_FALSE);
		break;
	case GLFW_KEY_UP:
		if (levelindex == 0)break;
		levelindex = (levelindex - 1) % 3;
		SoundEngine_scelta->play2D("asset/music/menuselect.wav", GL_FALSE);
		break;
	case GLFW_KEY_ENTER:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		if (levelindex == 0) {
			SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
			gamestatus = 1;
		}
		else if (levelindex == 1) {
			SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
			gamestatus = 1;
		}
		else if (levelindex == 2) {
			SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
			gamestatus = 1;
		}
		break;
	case GLFW_KEY_LEFT:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 1;
		break;
	default:
		break;
	}
}

void key_callback_pgselect(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_LEFT:
		if (pgindex == 0)break;
		pgindex = (pgindex - 1) % 3;
		SoundEngine_scelta->play2D(pgSelect().c_str(), GL_FALSE);
		break;
	case GLFW_KEY_RIGHT:
		if (pgindex == 2)break;
		pgindex = (pgindex + 1) % 3;
		SoundEngine_scelta->play2D(pgSelect().c_str(), GL_FALSE);
		break;
	case GLFW_KEY_ENTER:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 5;
		SoundEngine_player->play2D(pgSelect().c_str(), GL_FALSE);
		break;
	case GLFW_KEY_B:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 1;
		break;
	case GLFW_KEY_H:
		SoundEngine_scelta->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 3;
		break;
	default:
		break;
	}
}

void handleKeyInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		gamestatus = 1;
		SoundEngine_menu->play2D("asset/music/pause.wav");
	}

	////// Movimento in avanti
	if(start){
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			player.ProcessKeyboard(PG_FORWARD, deltaTime);
			//camera.ProcessKeyboard(FORWARD, deltaTime);
			schiacciato = false;
			animazione = true;
			animation++;

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				player.ProcessKeyboard(PG_LEFT, deltaTime);
				camera.ProcessKeyboard(ROTATE_LEFT, deltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				player.ProcessKeyboard(PG_RIGHT, deltaTime);
				camera.ProcessKeyboard(ROTATE_RIGHT, deltaTime);
			}
			if (camera.isFixed()) {
				if (abs(dot(normalize(camera.Front), normalize(player.front)) > 0.5))
					camera.BackEffect();
				else
					camera.ForwardEffect();
			}
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { //da aggiustare l'animazione del movimento indietro
			player.ProcessKeyboard(PG_BACKWARD, deltaTime);
			//camera.ProcessKeyboard(FORWARD, deltaTime);
			schiacciato = false;
			animazione = true;
			animation++;

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				player.ProcessKeyboard(PG_LEFT, deltaTime);
				camera.ProcessKeyboard(ROTATE_LEFT, deltaTime);
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				player.ProcessKeyboard(PG_RIGHT, deltaTime);
				camera.ProcessKeyboard(ROTATE_RIGHT, deltaTime);
			}
			if (camera.isFixed()) {
				if (abs(dot(normalize(camera.Front), normalize(player.front)) > 0.5))
					camera.BackEffect();
				else
					camera.ForwardEffect();
			}
		}

		else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
			animazione = false;
			////// Movimento a sinistra
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				player.ProcessKeyboard(PG_LEFT, deltaTime);
				//camera.ProcessKeyboard(ROTATE_LEFT, deltaTime);
			}
			////// Movimento a destra
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				player.ProcessKeyboard(PG_RIGHT, deltaTime);
				//camera.ProcessKeyboard(ROTATE_RIGHT, deltaTime);
			}
		}

		/////// Fermo
		////// Movimento a sinistra
		else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			player.ProcessKeyboard(PG_LEFT, deltaTime);
		////// Movimento a destra
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			player.ProcessKeyboard(PG_RIGHT, deltaTime);
	}
	


}