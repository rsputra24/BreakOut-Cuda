
#include <irrklang\irrKlang.h>
#include "Breakout.h"
#include "MainMenu.h"

using namespace std;
using namespace irrklang;
ISoundEngine *SoundEngine = createIrrKlangDevice();
void recomputeFrame(int value);
unsigned int screenWidth = 0;
unsigned int screenHeight = 0;
bool showgame = false;

Engine::Breakout::Breakout() {
	//init();
}

Engine::Breakout::~Breakout() {

}

void Engine::Breakout::display(void) {

    // Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set OpenGL for 2D drawing
    glMatrixMode(GL_PROJECTION);
	glEnable(GL_ALPHA);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glOrtho(0.0f, WINWIDTH, WINHEIGHT, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	renderBackground();
	
    //drawBackground();

    // Select which state of the game to display
    switch (gameState) {
        case INIT:
            // Init values
            init();
            break;
            
        case Menus:
			drawGUI();
            // TODO List menu
            break;
            
        case Gameplay:
            // Draw the game
			if (showgame == true) {
				drawGame();
				// If no balls, player loses the game
				if (balls.size() <= 0 & lifesCount > 0) {
					newBall(-1, -1);
					lifesCount--;
					reward = 100;
				}
				else if (balls.size() <= 0) {
					drawAfterGameGUI("AH lemah gan... booo");
				}

				// If no bricks, player wins the level
				if (bricks.size() <= 0 && level <= 2) {
					level++;
					initBricks();
				}
				else if (bricks.size() <= 0) {
					drawAfterGameGUI("WEW mantap gan!!..");
				}
			}
            break;
            
        case Scoreboard:
			drawScore();// TODO
            break;
        
        default:
            break;
    }
    
    //glutTimerFunc(TIMER, recomputeFrame, 0);
    
    //glutSwapBuffers();
}

void recomputeFrame(int value) {
	//glutPostRedisplay();
}

void Engine::Breakout::renderBackground() {

	/*mat4 model, model2, view, projection;
	model = translate(model, vec3(position.x, position.y + 1, position.z));
	model = rotate(model, rotation, axisRotation);
	model2 = scale(model2, vec3(35, 0, 35));
	// LookAt camera (position, target/direction, up)
	view = lookAt(vec3(position.x + 10, position.y + 15, position.z), vec3(position.x, position.y, position.z), vec3(0.0f, 1.0f, 0.0f));
	// Perspective projection
	projection = perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 1000.0f);*/

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureBG);

	//GLint model2Loc = glGetUniformLocation(this->programBG, "model");
	//GLint view2Loc = glGetUniformLocation(this->programBG, "view");
	//GLint proj2Loc = glGetUniformLocation(this->programBG, "projection");
	UseShader(this->programBG);
	//glUniformMatrix4fv(view2Loc, 1, GL_FALSE, glm::value_ptr(view));
	//glUniformMatrix4fv(proj2Loc, 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(model2Loc, 1, GL_FALSE, glm::value_ptr(model2));
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
	
}

void Engine::Breakout::buildBackgroundImage() {
	this->programBG = BuildShader("bg.vert", "bg.frag");

	glGenTextures(1, &textureBG);
	glBindTexture(GL_TEXTURE_2D, textureBG);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("bg.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float x = width * 0.2f / height;
	float y = 0.2f;

	float ww = WINWIDTH;
	float wh = WINHEIGHT;

	GLfloat vertices[] = {
		// Positions					// Colors           // Texture Coords

		-1, -1, 0.0f,   1.0f, 1.0f, 1.0f,  0, 0, // Bottom Left
		1, -1, 0.0f,   1.0f, 1.0f, 1.0f,   1, 0, // Bottom Right
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,  1,  1, // Top Right
		-1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0,  1  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0,1,2,3
	};

	glGenVertexArrays(1, &VAOBG);
	glGenBuffers(1, &VBOBG);
	glGenBuffers(1, &EBOBG);

	glBindVertexArray(VAOBG);

	glBindBuffer(GL_ARRAY_BUFFER, VBOBG);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBG);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
	
}

void Engine::Breakout::init(void) {
    // Reset game statistics
	ImGui_ImplSdlGL3_Init(this->window);
	
	glewInit();
	//drawCircle();

	buildBackgroundImage();

    score = 0;
    level = 1;
    reward = 100;
    lifesCount = 3;
    
    // Remove all balls
    balls.clear();
    
    // Remove all bricks
    bricks.clear();
    
    // Init bricks
    initBricks();
    
    // Add ball and paddle
    initPaddle();
    newBall(-1, -1);
    
    // Start game play
    gameState = Engine::Breakout::Menus;
}

void Engine::Breakout::drawBackground(void) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_QUADS);
    // Top color
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(WINWIDTH, WINHEIGHT);
    glVertex2f(-WINWIDTH, WINHEIGHT);
    // Bottom color
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();
	//renderer = SDL_CreateRenderer(window, 1, SDL_RENDERER_ACCELERATED);
	//back_surface = SDL_LoadBMP("bground.bmp");
	//back_texture = SDL_CreateTextureFromSurface(renderer, back_surface);

}

void Engine::Breakout::drawGame(void) {
    // // Draw coordinates for guidance
    // drawCoordinate();
	//drawCircle();
    // Draw ball,,,,,,,,,,,,,,,,99999999999999ls
    drawBalls();
    
    // Draw bricks
    drawBricks();
    
    // Draw paddle
    drawPaddle();
    
    // Draw game statistics (lifes, score)
    drawGameStats();

}

void Engine::Breakout::newBall(float x = -1, float y = -1) {
    Ball b1;
    if (x < 0 || y < 0) {
        b1.xpos = WINWIDTH / 2.0;
        b1.ypos = WINHEIGHT - 30.0f;
    } else {
        b1.xpos = x;
        b1.ypos = y;
    }
    if ((float) rand() / (RAND_MAX) < 0.5)
        b1.xvel = 5.0f;
    else
        b1.xvel = -5.0f;
    b1.yvel = -3.0f;//ball speed
    b1.radius = BALL_RADIUS;
    b1.r = 0.4f + (float) rand() / (RAND_MAX);
    b1.g = 0.25f + (float) rand() / (RAND_MAX);
    b1.b = 0.4f + (float) rand() / (RAND_MAX);
    balls.push_back(b1);
}

void Engine::Breakout::drawCircle() {
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	this->program = BuildShader("circleShader.vert", "circleShader.frag");
	UseShader(this->program);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* image = SOIL_load_image("cube.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	float pos[] = {
		// format position, tex coords
		//front
		-1.0, -1.0, 1.0, 0, 0,
		1.0, -1.0, 1.0, 1, 0,
		1.0,  1.0, 1.0, 1, 1,
		-1.0,  1.0, 1.0, 0, 1,

		//right
		1.0,  1.0,  1.0, 0, 0,
		1.0,  1.0, -1.0, 1, 0,
		1.0, -1.0, -1.0, 1, 1,
		1.0, -1.0,  1.0, 0, 1,

		//back
		-1.0, -1.0, -1.0, 0, 0,
		1.0,  -1.0, -1.0, 1, 0,
		1.0,   1.0, -1.0, 1, 1,
		-1.0,  1.0, -1.0, 0, 1,

		//left
		-1.0, -1.0, -1.0, 0, 0,
		-1.0, -1.0,  1.0, 1, 0,
		-1.0,  1.0,  1.0, 1, 1,
		-1.0,  1.0, -1.0, 0, 1,

		//upper
		1.0, 1.0,  1.0, 0, 0,
		-1.0, 1.0,  1.0, 1, 0,
		-1.0, 1.0, -1.0, 1, 1,
		1.0, 1.0, -1.0, 0, 1,

		//bottom
		-1.0, -1.0, -1.0, 0, 0,
		1.0, -1.0, -1.0, 1, 0,
		1.0, -1.0,  1.0, 1, 1,
		-1.0, -1.0,  1.0, 0, 1,


	};
		/*-1, -1,
		-1, 1,
		1, 1,
		1, -1*/
	

	float vertices[] = { 0,  1,  2,  0,  2,  3,   //front
		4,  5,  6,  4,  6,  7,   //right
		8,  9,  10, 8,  10, 11,  //back
		12, 13, 14, 12, 14, 15,  //left
		16, 17, 18, 16, 18, 19,  //upper
		20, 21, 22, 20, 22, 23 };
		//{ 0.0,0.0f,0.0f,100.0f };

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), pos, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

}

void Engine::Breakout::drawBalls(void) {
	
    for (std::vector<Ball>::iterator it = balls.begin(); it != balls.end(); ) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // use GL_LINE if no fill
        glBegin(GL_POLYGON);
        glColor3f(it->r, it->g, it->b);
        for(int j = 0; j < CIRCLE_SEGMENTS; j++) {
            float const theta = 2.0f * 3.1415926f * (float)j / (float)CIRCLE_SEGMENTS;
            float const x = it->radius * cosf(theta);
            float const y = it->radius * sinf(theta);
            glVertex2f(x + it->xpos, y + it->ypos);
        }
        glEnd();
		//UseShader(this->program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		//drawCircle();
        
        // Set new position
        it->xpos += it->xvel;
        it->ypos += it->yvel;
        
        // Collision with left/right/top window sides
        if ( (it->xpos <= (2 * it->radius)) || (it ->xpos >= (WINWIDTH - 2 * it->radius)) ) {
            it->xvel *= -1;
			SoundEngine->play2D("sound/blip.wav", GL_FALSE);
        }
        if ( (it->ypos <= (2 * it->radius)) ) {
            it->yvel *= -1;
			SoundEngine->play2D("sound/blip.wav", GL_FALSE);
        }
        if (it->ypos >= (WINHEIGHT - 2 * it->radius)) {
            it = balls.erase(it);
			SoundEngine->play2D("sound/blip.wav", GL_FALSE);
            continue;
        }
        
        // Collission with the bricks
        for (std::vector<Brick>::iterator br = bricks.begin(); br != bricks.end(); ) {
            // Check collission between circle and vertical brick sides
            if (it->ypos >= br->ypos && it->ypos <= br->ypos + br->height) {
                // brick right edge and left point on circle
                if ((it->xpos - it->radius - br->xpos - br->width) <= 5 && (it->xpos - it->radius - br->xpos - br->width) >= 0) {
                    it->xvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
                
                // brick left edge and right point on circle
                if ((it->xpos + it->radius - br->xpos) >= -5 && (it->xpos + it->radius - br->xpos) <= 0) {
                    it->xvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
            }
            
            // Check collission between circle and horizontal brick sides
            if (it->xpos >= br->xpos && it->xpos <= br->xpos + br->width) {
                // brick bottom edge and top point on circle
                if ((it->ypos - it->radius - br->ypos - br->height) <= 10 && (it->ypos - it->radius - br->ypos - br->height) >= 0) {
                    it->yvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
                
                // brick top edge and bottom point on circle
                if ((it->ypos + it->radius - br->ypos) >= -10 && (it->ypos + it->radius - br->ypos) <= 0) {
                    it->yvel *= -1;
                    br = hitBrick(br);
                    continue;
                }
            }
            
            GLfloat d;
            // Check collission with top left corner
            d = pow((it->xpos - br->xpos), 2.0) + pow((it->ypos - br->ypos), 2.0);
            if (d < it->radius + 5.0) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }

            // Check collission with top right corner
            d = pow((it->xpos - br->xpos - br->width), 2.0) + pow((it->ypos - br->ypos), 2.0);
            if (d < it->radius + 5.0) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }

            // Check collission with bottom left corner
            d = pow((it->xpos - br->xpos), 2.0) + pow((it->ypos - br->ypos - br->height), 2.0);
            if (d < it->radius + 5.0) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }
            
            // Check collission with bottom right corner
            d = pow((it->xpos - br->xpos - br->width), 2.0) + pow((it->ypos - br->ypos - br->height), 2.0);
            if (d < it->radius + 5.0) {
                it->xvel *= -1;
                it->yvel *= -1;
                br = hitBrick(br);
                continue;
            }
            
            ++br; // next brick
        }
        
        // Check collission between paddle's top edge and bottom point on circle
        if (it->xpos >= paddle.xpos && it->xpos <= paddle.xpos + paddle.width) {
            if ((it->ypos + it->radius - paddle.ypos) >= -10 && (it->ypos + it->radius - paddle.ypos) <= 0) {
                it->yvel *= -1;
                reward = 100;
                score += reward;
				SoundEngine->play2D("sound/blip.wav", GL_FALSE);
                continue;
            }
        }
        
        ++it; // next ball
    }
}

void Engine::Breakout::initPaddle(void) {
    paddle.r = 0.2f;
    paddle.g = 0.9f;
    paddle.b = 0.5f;
    paddle.width = 150.0f;
    paddle.height = 12.0f;
    paddle.xpos = WINWIDTH / 2.0f - paddle.width / 2.0f;
    paddle.ypos = WINHEIGHT - 20.0f;
}

void Engine::Breakout::drawPaddle() {
    // Make sure paddle is larger than 25px
    if (paddle.width < 25) {
        paddle.width = 25;
    }
    
    glColor3f(paddle.r, paddle.g, paddle.b);
    //glRectf(paddle.xpos, paddle.ypos, paddle.xpos + 5.0f, paddle.ypos + paddle.height);
    glRectf(paddle.xpos + 10.0f, paddle.ypos, paddle.xpos + paddle.width - 10.0f, paddle.ypos + paddle.height);
    //glRectf(paddle.xpos + paddle.width - 5.0f, paddle.ypos, paddle.xpos + paddle.width, paddle.ypos + paddle.height);
}

void Engine::Breakout::drawBricks(void) {
    for (std::vector<Brick>::iterator it = bricks.begin(); it != bricks.end(); ++it) {
        glColor4f(it->r, it->g, it->b, 0.5f);
        glRectf(it->xpos, it->ypos, it->xpos + it->width, it->ypos + it->height);
        
        glBegin(GL_QUADS);
        glColor4f(it->r-0.2f, it->g-0.2f, it->b-0.2f, 0.5f);
        glVertex2f(it->xpos, it->ypos);
        glColor4f(it->r-0.05f, it->g-0.05f, it->b-0.05f,0.5f);
        glVertex2f(it->xpos + it->width, it->ypos);
        glColor4f(it->r-0.15f, it->g-0.15f, it->b-0.15f, 0.5f);
        glVertex2f(it->xpos + it->width, it->ypos + it->height);
        glVertex2f(it->xpos, it->ypos);
        glEnd();
    }
}

template <typename Iterator>
Iterator Engine::Breakout::hitBrick(Iterator brick) {
    score += reward;
    reward += 25;
	SoundEngine->play2D("sound/crack.wav", GL_FALSE);
//    system("afpqlay ../../cartoon008.wav");
	//PlaySound(TEXT("sound/blip.wav"), NULL, SND_ALIAS);
    // Decrease brick health
    if (brick->health > 1) {
        brick->r = 0.95f;
        brick->g = 0.95f;
        brick->b = 0.95f;
        brick->health -= 1;
        return ++brick;
    } else {
        return bricks.erase(brick);
    }
}

void Engine::Breakout::initBricks(void) {
    if (level == 1)
        bricksLevel1();
    else if (level == 2)
        bricksLevel2();
}

void Engine::Breakout::bricksLevel1(void) {
    Brick newBrick;
    newBrick.r = 0.95f;
    newBrick.g = 0.95f;
    newBrick.b = 0.95f;
    newBrick.health = 1;
    newBrick.width = (WALLWIDTH - (WALLCOLS - 2) * WALLSPACE) / (WALLCOLS*2);
    newBrick.height = (WALLHEIGHT - (WALLROWS - 2) * WALLSPACE)*2 / WALLROWS;
    
    for (int i = 0; i < WALLROWS; ++i) {
        for (int j = 0; j < WALLCOLS; ++j) {
            // Set stronger bricks
            if (i+1 > ceil(WALLROWS / 2.0) - 2 && i < ceil(WALLROWS / 2.0) + 2 && j+2 > ceil(WALLCOLS / 2.0) - 3 && j < ceil(WALLCOLS / 2.0) + 3) {
                newBrick.r = 1.0f;
                newBrick.g = 0.5f;
                newBrick.b = 0.5f;
                newBrick.health = 2;
            } else {
                newBrick.r = 0.95f;
                newBrick.g = 0.95f;
                newBrick.b = 0.95f;
                newBrick.health = 1;
            }
            
            newBrick.xpos = WALLX + j * newBrick.width + j * WALLSPACE;
            newBrick.ypos = WALLY + i * newBrick.height + i * WALLSPACE;
            bricks.push_back(newBrick);
        }
    }
}

void Engine::Breakout::bricksLevel2(void) {
    Brick newBrick;
    newBrick.width = (WALLWIDTH - (WALLCOLS - 2) * WALLSPACE) / WALLCOLS;
    newBrick.height = (WALLHEIGHT - (WALLROWS - 2) * WALLSPACE) / WALLROWS;
    
    for (int i = 0; i < WALLROWS; i++) {
        for (int j = 0; j < WALLCOLS; j++) {
            // Set stronger bricks
            if (i == 1 || i == WALLROWS - 2 || j == 1 || j == WALLCOLS - 2) {
                newBrick.r = 1.0f;
                newBrick.g = 0.5f;
                newBrick.b = 0.5f;
                newBrick.health = 2;
            } else {
                newBrick.r = 0.95f;
                newBrick.g = 0.95f;
                newBrick.b = 0.95f;
                newBrick.health = 1;
            }
            
            newBrick.xpos = WALLX + j * newBrick.width + j * WALLSPACE;
            newBrick.ypos = WALLY + i * newBrick.height + i * WALLSPACE;
            bricks.push_back(newBrick);
        }
    }
}

void Engine::Breakout::drawGameStats(void) {
    glBegin(GL_LINES);
    // Bottom right (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(20.0f, 30.0f);
    glVertex2f(WINWIDTH - 20.0f, 30.0f);
    glEnd();
    
    float offset = 25.0f;
    for (int i = 0; i < lifesCount & i < 10; ++i) {
        drawLife(35 + offset * i, 15);
    }
    
    drawScore();
}

void Engine::Breakout::drawLife(float x, float y) {
    // Scale the heart symbol
    float const scale = 0.5f;
    
    // Heart symbol equations from Walfram Mathworld: http://mathworld.wolfram.com/HeartCurve.html
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.2f, 0.2f);
    for(int j = 0; j < CIRCLE_SEGMENTS; j++) {
        float const theta = 2.0f * 3.1415926f * (float)j / (float)CIRCLE_SEGMENTS;
        float const xx = scale * 16.0f * sinf(theta) * sinf(theta) * sinf(theta);
        float const yy = -1 * scale * (13.0f * cosf(theta) - 5.0f * cosf(2.0f * theta) - 2 * cosf(3.0f * theta) - cosf(4.0f * theta));
        glVertex2f(x + xx, y + yy);
    }
    glEnd();
}

void Engine::Breakout::drawScore(void) {
    glPushMatrix();
    // Write score word
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(WINWIDTH - 120, 20);
    char buf[300], *p;
    p = buf;
    sprintf_s(buf, "Score: ");
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while(*(++p));
    // Print the score
    p = buf;
    sprintf_s(buf, "           %d", score);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(WINWIDTH - 120, 20);
    do glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p); while(*(++p));
    glPopMatrix();
}

void Engine::Breakout::drawCoordinate(void) {
    glBegin(GL_LINES);
        // Top left (white)
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(20.0f, 10.0f);
        glVertex2f(20.0f, 30.0f);
        glVertex2f(10.0f, 20.0f);
        glVertex2f(30.0f, 20.0f);
    
        // Bottom right (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(WINWIDTH - 20.0f, WINHEIGHT - 10.0f);
        glVertex2f(WINWIDTH - 20.0f, WINHEIGHT - 30.0f);
        glVertex2f(WINWIDTH - 10.0f, WINHEIGHT - 20.0f);
        glVertex2f(WINWIDTH - 30.0f, WINHEIGHT - 20.0f);
    glEnd();
}

void Engine::Breakout::reshape(int width, int height) {
    if (width != WINWIDTH || height != WINHEIGHT)
        glutReshapeWindow(WINWIDTH, WINHEIGHT);
	
}

void Engine::Breakout::mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        newBall(x, y);
    }
    
    // Force redraw
	glutPostRedisplay();
}

void Engine::Breakout::PollInput()
{
	SDL_Event evt;

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evt)) {
		OnSDLGameEvent(evt);
		OnUserDefinedEvent(evt);
	}
}

void Engine::Breakout::OnSDLGameEvent(SDL_Event& evt)
{
	switch (evt.type) {
	case SDL_QUIT:
		state = State::EXIT;
		break;
	case SDL_MOUSEMOTION:
		SetMouseCoords((float)evt.motion.x, (float)evt.motion.y);
		break;
	case SDL_KEYDOWN:
		PressKey(evt.key.keysym.sym);
		break;
	case SDL_KEYUP:
		ReleaseKey(evt.key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		PressKey(evt.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		ReleaseKey(evt.button.button);
		break;
	}
}

void Engine::Breakout::PressKey(unsigned int keyID) {
	// Here we are treating _keyMap as an associative array.
	// if keyID doesn't already exist in _keyMap, it will get added
	_keyMap[keyID] = true;
}

void Engine::Breakout::ReleaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}

void Engine::Breakout::SetMouseCoords(float x, float y) {
	_mouseCoords.x = x;
	_mouseCoords.y = y;
	//if (gameState = Gameplay) {
		y = WINHEIGHT - y;
		if (x - paddle.width / 2.0f >= 0 && x + paddle.width / 2.0f <= WINWIDTH) {
			paddle.xpos = x - paddle.width / 2.0f;
		}
		else if (x - paddle.width / 2.0f <= 0) {
			paddle.xpos = 0;
		}
		else if (x + paddle.width / 2.0f >= WINWIDTH) {
			paddle.xpos = WINWIDTH - paddle.width;
		}
	//}
}

bool Engine::Breakout::IsKeyDown(unsigned int keyID) {
	auto it = _keyMap.find(keyID);
	if (it != _keyMap.end()) {
		return it->second;
	}
	else {
		return false;
	}
}

bool Engine::Breakout::IsKeyPressed(unsigned int keyID) {
	if (IsKeyDown(keyID) == true && WasKeyDown(keyID) == false) {
		return true;
	}
	return false;
}

bool Engine::Breakout::WasKeyDown(unsigned int keyID) {
	auto it = _previousKeyMap.find(keyID);
	if (it != _previousKeyMap.end()) {
		return it->second;
	}
	else {
		return false;
	}
}

void Engine::Breakout::mouseMove(int x, int y) {
    y = WINHEIGHT - y;
    if (x - paddle.width / 2.0f >= 0 && x + paddle.width / 2.0f <= WINWIDTH) {
        paddle.xpos = x - paddle.width / 2.0f;
    } else if (x - paddle.width / 2.0f <= 0) {
        paddle.xpos = 0;
    } else if (x + paddle.width / 2.0f >= WINWIDTH) {
        paddle.xpos = WINWIDTH - paddle.width;
    }
	glutPostRedisplay();
}

void Engine::Breakout::keyStroke(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': // Exit
            exit(0);
            break;
        case 'n': // New game
            init();
            break;
        case 'h':
            lifesCount++;
            break;
        case 27: // Esc button
            exit(0);
            break;
        default:
            break;
    }
}

bool play_scene = false;
bool score_scene = false;
bool exit_scene = false;
bool restart = false;
bool gotomenu = false;
//bool showafter = false;
ImVec4 clear_color = ImColor(114, 144, 154);


void Engine::Breakout::drawAfterGameGUI(char* gameresult) {
	ImGui_ImplSdlGL3_NewFrame(this->window);
	
	{
		ImGui::SetWindowSize(ImVec2(500, 100));
		ImGui::SetWindowPos(ImVec2(this->screenWidth / 2, this->screenHeight / 2));
		ImGui::Begin("Game Over!!", &restart);
		ImGui::Text(gameresult);
		ImGui::Text("Main lagi gan?");
		if (ImGui::Button("Ya")) restart ^= 1;
		ImGui::SameLine(40);
		if (ImGui::Button("Tidak")) gotomenu ^= 1;
		ImGui::End();
	}

	if (restart) {
		score = 0;
		level = 1;
		reward = 100;
		lifesCount = 3;
		newBall(-1,-1);
		
		//gameState = Engine::Breakout::INIT;
		restart = false;
	}

	if (gotomenu) {
		showgame = false;
		score = 0;
		level = 1;
		reward = 100;
		lifesCount = 3;
		newBall(-2, -2);
		gameState = Engine::Breakout::Menus;
		gotomenu = false;
	}

	ImGui::Render();
}

void Engine::Breakout::drawGUI(void)
{
	
	ImGui_ImplSdlGL3_NewFrame(this->window);
	//ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(400, 150));

	// 1. Show a simple window
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
	{
		ImGui::Begin("Filkom Petjah!!", &play_scene);
		if (ImGui::Button("Mulai Bermain")) play_scene ^= 1;
		if (ImGui::Button("Lihat Skor")) score_scene ^= 1;
		if (ImGui::Button("Keluar")) exit_scene ^= 1;
		ImGui::End();
	}

	if (play_scene)
	{
		ImGui::SetNextWindowSize(ImVec2(10,0), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Loading", &score_scene);
		gameState = Engine::Breakout::Gameplay;
		showgame = true;
		play_scene = false;
		ImGui::End();
	}

	if (score_scene)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&play_scene);
		gameState = Engine::Breakout::Scoreboard;
		score_scene = false;
	}

	if (exit_scene) {
		SDL_Quit();
		ImGui::Shutdown();
		exit(1);
	}
	ImGui::Render();
}


void Engine::Breakout::specialKeyPos(int key, int x, int y) {
    switch(key)
	{
		case GLUT_KEY_LEFT:
            if (paddle.xpos > 0) {
                paddle.xpos -= 5.0f;
                paddle.xpos -= 5.0f;
                glutPostRedisplay();
                paddle.xpos -= 5.0f;
                paddle.xpos -= 5.0f;
                glutPostRedisplay();
            }
            break;
        case GLUT_KEY_RIGHT:
            if (paddle.xpos + paddle.width < WINWIDTH) {
                paddle.xpos += 5.0f;
                paddle.xpos += 5.0f;
                glutPostRedisplay();
                paddle.xpos += 5.0f;
                paddle.xpos += 5.0f;
                glutPostRedisplay();
            }
            break;
        default:
            break;
    }
}

GLuint Engine::Breakout::BuildShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	string vertexCode;
	string fragmentCode;
	string geometryCode;
	ifstream vShaderFile;
	ifstream fShaderFile;
	ifstream gShaderFile;
	// ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	gShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();
	// 2. Compile shaders
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];
	// Vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	//CheckShaderErrors(vertex, "VERTEX");
	// Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkShaderErrors(fragment, "FRAGMENT");
	// If geometry shader is given, compile geometry shader
	GLuint geometry;
	if (geometryPath != nullptr)
	{
		const GLchar * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkShaderErrors(geometry, "GEOMETRY");
	}
	// Shader Program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	if (geometryPath != nullptr)
		glAttachShader(program, geometry);
	glLinkProgram(program);
	checkShaderErrors(program, "PROGRAM");
	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
		glDeleteShader(geometry);
	return program;

}

void Engine::Breakout::UseShader(GLuint program)
{
	// Uses the current shader
	glUseProgram(program);
}

void Engine::Breakout::displayMenu(void) {
}

void Engine::Breakout::startGame(string windowTitle, unsigned int screenWidth, unsigned int screenHeight, bool vsync, WindowFlag windowFlag, unsigned int targetFrameRate, float timeScale) {
	
	SDL_Init(SDL_INIT_EVERYTHING); // inisialisasi sdl
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//ImGui_ImplSdlGL3_Init(this->window);

	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->timeScale = timeScale;

	if (targetFrameRate > 0) {
		targetFrame = 1000.0f / targetFrameRate;
	}

	Uint32 flags = SDL_WINDOW_OPENGL;

	if (WindowFlag::EXCLUSIVE_FULLSCREEN == windowFlag) 
		flags |= SDL_WINDOW_FULLSCREEN;
	if (WindowFlag::FULLSCREEN == windowFlag)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	if (WindowFlag::BORDERLESS == windowFlag)
		flags |= SDL_WINDOW_BORDERLESS;

	window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags); //buat window sdl baru
	//windowGUI = SDL_CreateWindow("tes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 200, SDL_WINDOW_OPENGL);

	glContext = SDL_GL_CreateContext(window);

	glewInit(); //inisialisasi glew

	SDL_GL_SetSwapInterval(vsync); //atur vsync

	this->state = State::INIT;

	last = SDL_GetTicks();
	SoundEngine->play2D("sound/UpbeatFunk.wav", GL_TRUE);
	init();

	while (State::INIT == state) {
		float deltaTime = getDeltaTime();
		getFPS();
		//init();
		PollInput();
		display();
		SDL_GL_SwapWindow(window);
		//SDL_RenderClear(renderer);
		//SDL_RenderCopy(renderer, back_texture, NULL, NULL);
		//SDL_RenderPresent(renderer);
		limitFPS();
	}

	


}

float Engine::Breakout::getDeltaTime()
{
	unsigned int time = SDL_GetTicks();
	unsigned int delta = time - lastFrame;
	lastFrame = time;

	return delta * timeScale;
}

void Engine::Breakout::getFPS()
{
	if (SDL_GetTicks() - last > 1000) {
		fps = _fps;
		_fps = 0;
		last += 1000;
	}
	_fps++;
}

void Engine::Breakout::Err(string errorString)
{
	cout << errorString << endl;
	SDL_Quit();
	exit(1);
}

int Engine::Breakout::getFrameRate()
{
	return (int)this->fps;
}

void Engine::Breakout::limitFPS()
{
	//Limit the FPS to the max FPS
	SDL_Delay((Uint32)(targetFrame));

}

void Engine::Breakout::checkShaderErrors(GLuint shader, string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << endl;
		}
	}
}